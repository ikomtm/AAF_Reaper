-- AAF CSV Import Script for Reaper
-- Простой и надежный парсер CSV файлов

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
    
    -- Читаем заголовок
    local header = file:read("*line")
    if not header then
        file:close()
        reaper.ShowMessageBox("Empty CSV file", "Error", 0)
        return nil
    end
    
    reaper.ShowConsoleMsg("CSV Header: " .. header .. "\n")
    local headerFields = parseCSVLine(header)
    reaper.ShowConsoleMsg("CSV has " .. #headerFields .. " columns\n")
    
    -- Проверяем обязательные колонки
    local expectedColumns = {"TrackIndex", "TrackName", "TrackType", "Volume", "Pan", 
                           "ClipFileName", "TimelineStart", "Length", "SourceStart", "Gain"}
    
    -- Читаем данные
    local lineNumber = 1
    for line in file:lines() do
        lineNumber = lineNumber + 1
        
        if line:trim() ~= "" then
            local fields = parseCSVLine(line)
            
            if #fields >= 10 then
                local trackIndex = tonumber(fields[1])
                local trackName = fields[2]
                local trackType = fields[3]
                local volume = tonumber(fields[4]) or 1.0
                local pan = tonumber(fields[5]) or 0.0
                local mute = fields[6] == "1"
                local solo = fields[7] == "1"
                local clipFileName = fields[8]
                local timelineStart = tonumber(fields[9]) or 0
                local timelineEnd = tonumber(fields[10]) or 0
                local length = tonumber(fields[11]) or 0
                local sourceStart = tonumber(fields[12]) or 0
                local gain = tonumber(fields[13]) or 0
                local clipVolume = tonumber(fields[14]) or 1.0
                local clipPan = tonumber(fields[15]) or 0.0
                local mobID = fields[16] or ""
                local effects = fields[17] or ""
                
                -- Если новый трек
                if trackIndex ~= currentTrackIndex then
                    currentTrackIndex = trackIndex
                    currentTrack = {
                        trackIndex = trackIndex,
                        trackName = trackName,
                        trackType = trackType,
                        volume = volume,
                        pan = pan,
                        mute = mute,
                        solo = solo,
                        clips = {}
                    }
                    table.insert(tracks, currentTrack)
                    reaper.ShowConsoleMsg("Found track " .. trackIndex .. ": " .. trackName .. "\n")
                end
                
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
            else
                reaper.ShowConsoleMsg("Warning: Line " .. lineNumber .. " has only " .. #fields .. " fields\n")
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

-- Главная функция импорта
local function importAAFFromCSV()
    -- Выбираем CSV файл
    local retval, csvPath = reaper.GetUserFileNameForRead("", "Select AAF Export CSV", "csv")
    if not retval then
        return
    end
    
    reaper.ShowConsoleMsg("=== AAF CSV Import Started ===\n")
    reaper.ShowConsoleMsg("Loading from: " .. csvPath .. "\n")
    
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
                
                local filePath = findAudioFile(clipData.fileName, csvPath)
                if filePath then
                    reaper.ShowConsoleMsg("    Found file: " .. filePath .. "\n")
                    
                    -- Создаем медиа элемент
                    local mediaItem = reaper.AddMediaItemToTrack(track)
                    if mediaItem then
                        -- Устанавливаем позицию и длительность
                        reaper.SetMediaItemInfo_Value(mediaItem, "D_POSITION", clipData.timelineStart)
                        reaper.SetMediaItemInfo_Value(mediaItem, "D_LENGTH", clipData.length)
                        
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
                                    local gainMult = math.pow(10, clipData.gain / 20)
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
