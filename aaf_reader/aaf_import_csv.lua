-- AAF CSV Import Script for Reaper
-- Импортирует CSV файл, созданный C++ программой AAF Reader, и собирает секвенцию в Reaper
-- 
-- ВАЖНО! Порядок использования:
-- 1. Сначала запустите C++ программу aaf_reader.exe с AAF файлом:
--    aaf_reader.exe "path/to/your/file.aaf" --log-level=INFO
--    Это создаст:
--    - CSV файл с информацией о клипах (aaf_export.csv)
--    - Папку extracted_media/ с извлеченными аудио файлами
-- 2. Затем запустите этот скрипт в Reaper для импорта CSV и создания секвенции
--
-- Скрипт автоматически найдет извлеченные файлы в папке extracted_media/
-- и импортирует их в правильном порядке согласно CSV файлу

-- Функция для чтения метаданных из CSV комментариев
local function readCSVMetadata(csvPath)
    local metadata = {
        projectName = "",
        sessionStartTimecode = 0,
        totalLength = 0,
        sampleRate = 48000,
        channelCount = 2
    }
    
    local file = io.open(csvPath, "r")
    if not file then
        reaper.ShowConsoleMsg("ERROR: Could not open CSV file: " .. csvPath .. "\n")
        return metadata
    end
    
    -- Читаем комментарии в начале файла
    for line in file:lines() do
        if line:sub(1, 1) ~= "#" then
            break -- Закончились комментарии
        end
        
        -- Парсим метаданные
        if line:find("Project Name:") then
            metadata.projectName = line:match("Project Name:%s*(.+)")
        elseif line:find("Session Start Timecode %(frames%):") then
            local timecode = line:match("Session Start Timecode %(frames%):%s*(%d+)")
            if timecode then
                metadata.sessionStartTimecode = tonumber(timecode)
            end
        elseif line:find("Total Project Length:") then
            local length = line:match("Total Project Length:%s*([%d%.]+)")
            if length then
                metadata.totalLength = tonumber(length)
            end
        elseif line:find("Sample Rate:") then
            local rate = line:match("Sample Rate:%s*([%d%.]+)")
            if rate then
                metadata.sampleRate = tonumber(rate)
            end
        elseif line:find("Channel Count:") then
            local count = line:match("Channel Count:%s*(%d+)")
            if count then
                metadata.channelCount = tonumber(count)
            end
        end
    end
    
    file:close()
    return metadata
end

-- Функция для поиска извлеченного файла (упрощенная версия)
local function findExtractedFile(fileName, extractedDir)
    reaper.ShowConsoleMsg("    Searching for: " .. fileName .. "\n")
    
    -- Теперь fileName уже содержит правильное имя с суффиксом
    local searchPatterns = {
        extractedDir .. "/" .. fileName,
        extractedDir .. "\\" .. fileName
    }
    
    for _, pattern in ipairs(searchPatterns) do
        local file = io.open(pattern, "r")
        if file then
            file:close()
            reaper.ShowConsoleMsg("    Found file: " .. pattern .. "\n")
            return pattern
        end
    end
    
    reaper.ShowConsoleMsg("    File not found: " .. fileName .. "\n")
    return nil
end

-- Функция для парсинга CSV строки с учетом кавычек
local function parseCSVLine(line)
    local fields = {}
    local field = ""
    local inQuotes = false
    local i = 1
    
    while i <= #line do
        local char = line:sub(i, i)
        
        if char == '"' then
            if inQuotes and i < #line and line:sub(i + 1, i + 1) == '"' then
                -- Экранированная кавычка
                field = field .. '"'
                i = i + 1
            else
                -- Переключаем режим кавычек
                inQuotes = not inQuotes
            end
        elseif char == ',' and not inQuotes then
            -- Разделитель полей вне кавычек
            table.insert(fields, field)
            field = ""
        else
            field = field .. char
        end
        
        i = i + 1
    end
    
    -- Добавляем последнее поле
    table.insert(fields, field)
    
    return fields
end

-- Функция для чтения CSV файла
local function readCSVFile(filename)
    local file = io.open(filename, "r")
    if not file then
        reaper.ShowConsoleMsg("Cannot open CSV file: " .. filename .. "\n")
        reaper.ShowMessageBox("Cannot open CSV file: " .. filename, "Error", 0)
        return nil
    end
    
    local tracks = {}
    local currentTrackIndex = -1
    local currentTrack = nil
    
    -- Пропускаем комментарии и читаем заголовок
    local header = nil
    while true do
        local line = file:read("*line")
        if not line then
            file:close()
            reaper.ShowMessageBox("No data found in CSV file", "Error", 0)
            return nil
        end
        
        -- Пропускаем строки, начинающиеся с # (комментарии)
        if line:sub(1, 1) ~= "#" and line:trim() ~= "" then
            header = line
            break
        end
    end
    
    if not header then
        file:close()
        reaper.ShowMessageBox("No header found in CSV file", "Error", 0)
        return nil
    end
    
    reaper.ShowConsoleMsg("CSV Header: " .. header .. "\n")
    local headerFields = parseCSVLine(header)
    reaper.ShowConsoleMsg("CSV has " .. #headerFields .. " columns\n")
    
    -- Проверяем обязательные колонки
    local expectedColumns = {"TrackIndex", "TrackName", "TrackType", "Volume", "Pan", 
                           "ClipFileName", "TimelineStart", "Length", "SourceStart", "Gain",
                           "EventType", "EventStart", "EventLength", "EventShape"}
    
    -- Читаем данные
    local lineNumber = 1
    for line in file:lines() do
        lineNumber = lineNumber + 1
        
        if line:trim() ~= "" then
            local fields = parseCSVLine(line)
            
            if #fields >= 20 then  -- Увеличили минимальное количество полей
                local trackIndex = tonumber(fields[1])
                local trackName = fields[2]
                local trackType = fields[3]
                local trackVolume = tonumber(fields[4]) or 1.0
                local trackPan = tonumber(fields[5]) or 0.0
                local trackMute = fields[6] == "true"
                local trackSolo = fields[7] == "true"
                local clipFileName = fields[8]
                local mobID = fields[9] or ""
                local timelineStart = tonumber(fields[10]) or 0
                local timelineEnd = tonumber(fields[11]) or 0
                local sourceStart = tonumber(fields[12]) or 0
                local length = tonumber(fields[13]) or 0
                local gain = tonumber(fields[14]) or 0
                local clipVolume = tonumber(fields[15]) or 1.0
                local clipPan = tonumber(fields[16]) or 0.0
                local effects = fields[17] or ""
                local eventType = fields[18] or "CLIP"
                local eventStart = tonumber(fields[19]) or 0
                local eventLength = tonumber(fields[20]) or 0
                local eventShape = fields[21] or ""
                
                -- Если новый трек
                if trackIndex ~= currentTrackIndex then
                    currentTrackIndex = trackIndex
                    currentTrack = {
                        trackIndex = trackIndex,
                        trackName = trackName,
                        trackType = trackType,
                        volume = trackVolume,
                        pan = trackPan,
                        mute = trackMute,
                        solo = trackSolo,
                        clips = {},
                        fadeEvents = {}  -- Добавляем массив для fade-событий
                    }
                    table.insert(tracks, currentTrack)
                    reaper.ShowConsoleMsg("Found track " .. trackIndex .. ": " .. trackName .. "\n")
                end
                
                -- Обрабатываем различные типы событий
                if eventType == "CLIP" then
                    -- Добавляем клип (если есть файл)
                    if clipFileName and clipFileName ~= "" then
                        local clip = {
                            fileName = clipFileName,
                            timelineStart = timelineStart,
                            timelineEnd = timelineEnd,
                            length = length,
                            sourceStart = sourceStart,
                            gain = gain,
                            volume = clipVolume,
                            pan = clipPan,
                            mobID = mobID,
                            effects = {}
                        }
                        
                        -- Парсим эффекты (разделенные ';')
                        if effects ~= "" then
                            for effect in effects:gmatch("[^;]+") do
                                table.insert(clip.effects, effect)
                            end
                        end
                        
                        table.insert(currentTrack.clips, clip)
                    end
                elseif eventType == "FADE_IN" or eventType == "FADE_OUT" or eventType == "CROSSFADE" then
                    -- Добавляем fade-событие
                    local fadeEvent = {
                        type = eventType,
                        start = eventStart,
                        length = eventLength,
                        shape = eventShape,
                        clipMobID = mobID  -- Связываем с клипом
                    }
                    
                    table.insert(currentTrack.fadeEvents, fadeEvent)
                    reaper.ShowConsoleMsg("Found " .. eventType .. " at " .. eventStart .. " length " .. eventLength .. "\n")
                end
            else
                reaper.ShowConsoleMsg("Warning: Line " .. lineNumber .. " has only " .. #fields .. " fields (expected at least 20)\n")
            end
        end
    end
    
    file:close()
    
    reaper.ShowConsoleMsg("Loaded " .. #tracks .. " tracks from CSV\n")
    for i, track in ipairs(tracks) do
        reaper.ShowConsoleMsg("Track " .. i .. ": " .. track.trackName .. " (" .. #track.clips .. " clips)\n")
    end
    
    return tracks
end

-- Функция поиска аудио файла
local function findAudioFile(originalPath, csvPath)
    -- Если файл содержит полный путь
    if reaper.file_exists(originalPath) then
        return originalPath
    end
    
    -- Извлекаем только имя файла
    local fileName = originalPath:match("([^/\\]+)$") or originalPath
    
    -- Пути для поиска
    local searchPaths = {}
    
    -- В папке с CSV
    local csvDir = csvPath:match("(.+)[/\\]") or ""
    if csvDir ~= "" then
        table.insert(searchPaths, csvDir)
    end
    
    -- В папке проекта
    local projectPath = reaper.GetProjectPath("")
    if projectPath ~= "" then
        table.insert(searchPaths, projectPath)
    end
    
    -- Стандартные папки
    table.insert(searchPaths, "C:\\Audio")
    table.insert(searchPaths, "C:\\Users\\KSN_PC\\Desktop")
    table.insert(searchPaths, "C:\\Users\\KSN_PC\\C++Projects\\AAF_Reaper\\aaf_reader\\build")
    
    -- Ищем файл
    for _, path in ipairs(searchPaths) do
        local fullPath = path .. "\\" .. fileName
        if reaper.file_exists(fullPath) then
            return fullPath
        end
    end
    
    -- Пробуем с разными расширениями
    local baseName = fileName:match("(.+)%.[^%.]*$") or fileName
    local extensions = {".wav", ".aif", ".aiff", ".mp3", ".flac", ".ogg"}
    
    for _, ext in ipairs(extensions) do
        local testFile = baseName .. ext
        for _, path in ipairs(searchPaths) do
            local fullPath = path .. "\\" .. testFile
            if reaper.file_exists(fullPath) then
                return fullPath
            end
        end
    end
    
    return nil
end

-- Функция для применения fade-события к треку
local function applyFadeEvent(track, fadeEvent)
    reaper.ShowConsoleMsg("    Applying " .. fadeEvent.type .. " at " .. fadeEvent.start .. " length " .. fadeEvent.length .. "\n")
    
    -- Получаем количество медиа-элементов на треке
    local numItems = reaper.CountTrackMediaItems(track)
    
    for i = 0, numItems - 1 do
        local mediaItem = reaper.GetTrackMediaItem(track, i)
        if mediaItem then
            local itemStart = reaper.GetMediaItemInfo_Value(mediaItem, "D_POSITION")
            local itemLength = reaper.GetMediaItemInfo_Value(mediaItem, "D_LENGTH")
            local itemEnd = itemStart + itemLength
            
            -- Проверяем, попадает ли fade-событие в диапазон этого медиа-элемента
            local fadeStart = fadeEvent.start
            local fadeEnd = fadeEvent.start + fadeEvent.length
            
            -- Определяем пересечение
            local overlapStart = math.max(itemStart, fadeStart)
            local overlapEnd = math.min(itemEnd, fadeEnd)
            
            if overlapStart < overlapEnd then
                -- Есть пересечение - применяем fade
                reaper.ShowConsoleMsg("      Applying to item at " .. itemStart .. " (overlap: " .. overlapStart .. " - " .. overlapEnd .. ")\n")
                
                if fadeEvent.type == "FADE_IN" then
                    -- Применяем fade-in
                    local fadeInTime = overlapEnd - overlapStart
                    reaper.SetMediaItemInfo_Value(mediaItem, "D_FADEINLEN", fadeInTime)
                    reaper.SetMediaItemInfo_Value(mediaItem, "C_FADEINSHAPE", getFadeShape(fadeEvent.shape))
                    
                elseif fadeEvent.type == "FADE_OUT" then
                    -- Применяем fade-out
                    local fadeOutTime = overlapEnd - overlapStart
                    reaper.SetMediaItemInfo_Value(mediaItem, "D_FADEOUTLEN", fadeOutTime)
                    reaper.SetMediaItemInfo_Value(mediaItem, "C_FADEOUTSHAPE", getFadeShape(fadeEvent.shape))
                    
                elseif fadeEvent.type == "CROSSFADE" then
                    -- Crossfade обычно применяется между двумя элементами
                    -- В данном случае можем применить как fade-out к текущему элементу
                    local crossfadeTime = overlapEnd - overlapStart
                    reaper.SetMediaItemInfo_Value(mediaItem, "D_FADEOUTLEN", crossfadeTime)
                    reaper.SetMediaItemInfo_Value(mediaItem, "C_FADEOUTSHAPE", 0) -- Linear
                end
            end
        end
    end
end

-- Функция для конвертации типа fade в номер формы для Reaper
local function getFadeShape(shapeString)
    -- Reaper fade shapes:
    -- 0 = Linear
    -- 1 = Fast start
    -- 2 = Fast end
    -- 3 = Slow start/end
    -- 4 = Sharp curve
    -- 5 = Slow curve
    
    if shapeString == "Linear" or shapeString == "1" then
        return 0
    elseif shapeString == "Fast start" or shapeString == "2" then
        return 1
    elseif shapeString == "Fast end" or shapeString == "3" then
        return 2
    else
        return 0 -- По умолчанию linear
    end
end

-- Главная функция импорта
local function importAAFFromCSV()
    -- Выбираем CSV файл
    local retval, csvPath = reaper.GetUserFileNameForRead("", "Select AAF Export CSV", "csv")
    if not retval then
        return
    end
    
    reaper.ShowConsoleMsg("=== AAF CSV Import Started ===\n")
    reaper.ShowConsoleMsg("Loading from: " .. csvPath .. "\n")
    
    -- Получаем директорию CSV файла для поиска извлеченных медиа файлов
    local csvDir = csvPath:match("(.*)/[^/]*$") or csvPath:match("(.*)[/\\][^/\\]*$") or ""
    
    reaper.ShowConsoleMsg("CSV directory: " .. csvDir .. "\n")
    reaper.ShowConsoleMsg("Note: Make sure you have already run the C++ AAF extractor to create the extracted_media folder and files.\n")
    
    -- Определяем папку для поиска извлеченных файлов
    local extractedDir = csvDir .. "/extracted_media"
    
    -- Читаем CSV
    local tracks = readCSVFile(csvPath)
    if not tracks or #tracks == 0 then
        reaper.ShowMessageBox("No tracks found in CSV file", "Error", 0)
        return
    end
    
    -- Начинаем undo блок
    reaper.Undo_BeginBlock()
    
    -- Спрашиваем про очистку проекта
    local clearProject = reaper.ShowMessageBox("Clear current project before import?", "AAF CSV Import", 4)
    if clearProject == 6 then
        reaper.Main_OnCommand(40004, 0) -- File: New project
    end
    
    local totalTracks = #tracks
    local totalClips = 0
    local successfulClips = 0
    
    -- Считаем общее количество клипов
    for _, track in ipairs(tracks) do
        totalClips = totalClips + #track.clips
    end
    
    reaper.ShowConsoleMsg("Total clips to process: " .. totalClips .. "\n\n")
    
    -- Создаем треки
    for i, trackData in ipairs(tracks) do
        reaper.ShowConsoleMsg("--- Creating track " .. i .. "/" .. totalTracks .. ": " .. trackData.trackName .. " ---\n")
        
        -- Создаем трек
        reaper.InsertTrackAtIndex(i - 1, false)
        local track = reaper.GetTrack(0, i - 1)
        
        if track then
            -- Устанавливаем параметры трека
            reaper.GetSetMediaTrackInfo_String(track, "P_NAME", trackData.trackName, true)
            reaper.SetMediaTrackInfo_Value(track, "D_VOL", trackData.volume)
            reaper.SetMediaTrackInfo_Value(track, "D_PAN", trackData.pan)
            
            if trackData.mute then
                reaper.SetMediaTrackInfo_Value(track, "B_MUTE", 1)
            end
            
            if trackData.solo then
                reaper.SetMediaTrackInfo_Value(track, "I_SOLO", 1)
            end
            
            reaper.ShowConsoleMsg("Track created with " .. #trackData.clips .. " clips\n")
            
            -- Добавляем клипы
            for j, clipData in ipairs(trackData.clips) do
                reaper.ShowConsoleMsg("  Processing clip " .. j .. ": " .. clipData.fileName .. "\n")
                reaper.ShowConsoleMsg("    Timeline: " .. clipData.timelineStart .. " - " .. clipData.timelineEnd .. " (length: " .. clipData.length .. ")\n")
                reaper.ShowConsoleMsg("    Source start: " .. clipData.sourceStart .. "\n")
                
                -- Сначала ищем извлеченный файл, потом оригинальный
                local filePath = findExtractedFile(clipData.fileName, extractedDir)
                if not filePath then
                    filePath = findAudioFile(clipData.fileName, csvPath)
                end
                
                if filePath then
                    reaper.ShowConsoleMsg("    Found file: " .. filePath .. "\n")
                    
                    -- Создаем медиа элемент
                    local mediaItem = reaper.AddMediaItemToTrack(track)
                    if mediaItem then
                        -- Устанавливаем позицию и длительность
                        reaper.SetMediaItemInfo_Value(mediaItem, "D_POSITION", clipData.timelineStart)
                        reaper.SetMediaItemInfo_Value(mediaItem, "D_LENGTH", clipData.length)
                        
                        reaper.ShowConsoleMsg("    Set position: " .. clipData.timelineStart .. ", length: " .. clipData.length .. "\n")
                        
                        -- Добавляем аудио источник
                        local mediaSource = reaper.PCM_Source_CreateFromFile(filePath)
                        if mediaSource then
                            local take = reaper.AddTakeToMediaItem(mediaItem)
                            if take then
                                reaper.SetMediaItemTake_Source(take, mediaSource)
                                
                                -- Устанавливаем offset источника
                                if clipData.sourceStart > 0 then
                                    reaper.SetMediaItemTakeInfo_Value(take, "D_STARTOFFS", clipData.sourceStart)
                                end
                                
                                -- Применяем параметры клипа
                                if clipData.gain ~= 0 then
                                    local gainMult = 10 ^ (clipData.gain / 20)
                                    reaper.SetMediaItemTakeInfo_Value(take, "D_VOL", gainMult * clipData.volume)
                                else
                                    reaper.SetMediaItemTakeInfo_Value(take, "D_VOL", clipData.volume)
                                end
                                
                                if clipData.pan ~= 0 then
                                    reaper.SetMediaItemTakeInfo_Value(take, "D_PAN", clipData.pan)
                                end
                                
                                -- Устанавливаем имя take
                                local fileName = filePath:match("([^/\\]+)$") or filePath
                                reaper.GetSetMediaItemTakeInfo_String(take, "P_NAME", fileName, true)
                                
                                successfulClips = successfulClips + 1
                                reaper.ShowConsoleMsg("    Successfully created media item\n")
                            else
                                reaper.ShowConsoleMsg("    Error: Cannot create take\n")
                            end
                        else
                            reaper.ShowConsoleMsg("    Error: Cannot create audio source\n")
                        end
                    else
                        reaper.ShowConsoleMsg("    Error: Cannot create media item\n")
                    end
                else
                    reaper.ShowConsoleMsg("    File not found: " .. clipData.fileName .. "\n")
                end
            end
            
            -- Применяем fade-события к созданным клипам
            if trackData.fadeEvents then
                reaper.ShowConsoleMsg("  Applying " .. #trackData.fadeEvents .. " fade events...\n")
                for j, fadeEvent in ipairs(trackData.fadeEvents) do
                    applyFadeEvent(track, fadeEvent)
                end
            end
        else
            reaper.ShowConsoleMsg("Error: Cannot create track " .. i .. "\n")
        end
        
        reaper.ShowConsoleMsg("\n")
    end
    
    -- Обновляем и масштабируем
    reaper.UpdateArrange()
    reaper.Main_OnCommand(40031, 0) -- Zoom out project
    
    reaper.Undo_EndBlock("Import AAF from CSV", -1)
    
    -- Показываем результаты
    local message = string.format(
        "AAF CSV Import completed!\n\n" ..
        "Tracks created: %d\n" ..
        "Clips processed: %d/%d successful (%.1f%%)\n\n" ..
        "Check console for detailed log.",
        totalTracks, 
        successfulClips, totalClips, 
        totalClips > 0 and (successfulClips * 100.0 / totalClips) or 0
    )
    
    reaper.ShowConsoleMsg("=== CSV Import Completed ===\n")
    reaper.ShowConsoleMsg("Results: " .. successfulClips .. "/" .. totalClips .. " clips imported successfully\n")
    reaper.ShowConsoleMsg("Success rate: " .. (totalClips > 0 and string.format("%.1f%%", successfulClips * 100.0 / totalClips) or "0%") .. "\n")
    
    reaper.ShowMessageBox(message, "AAF CSV Import Results", 0)
end

-- Добавляем функцию trim для строк (если её нет)
if not string.trim then
    function string.trim(s)
        return s:match("^%s*(.-)%s*$")
    end
end

-- Запускаем импорт
importAAFFromCSV()
