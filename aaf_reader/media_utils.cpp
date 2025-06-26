#include "media_utils.h"
#include "aaf_utils.h"
#include "aaf_parser.h"
#include <AAF.h>
#include <AAFResult.h>
#include <AAFTypes.h>
#include <AAFDataDefs.h>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <vector>
#include <set>

void createExtractedMediaFolder() {
    try {
        if (!std::filesystem::exists("extracted_media")) {
            std::filesystem::create_directory("extracted_media");
        }
        if (!std::filesystem::exists("audio_files")) {
            std::filesystem::create_directory("audio_files");
        }
    } catch (const std::exception& e) {
        std::cerr << "Error creating media folders: " << e.what() << std::endl;
    }
}

void extractEmbeddedAudio(IAAFEssenceData* pEssenceData, const std::string& outputPath, std::ofstream& out) {
    if (!pEssenceData) return;
    
    try {
        // Получаем размер embedded данных
        aafLength_t dataSize = 0;
        HRESULT hr = pEssenceData->GetSize(&dataSize);
        if (FAILED(hr) || dataSize == 0) {
            out << "Failed to get essence data size" << std::endl;
            return;
        }
        
        // Создаем буфер для чтения данных
        std::vector<aafUInt8> buffer(static_cast<size_t>(dataSize));
        aafUInt32 bytesRead = 0;
        
        // Читаем данные
        hr = pEssenceData->Read(static_cast<aafUInt32>(dataSize), buffer.data(), &bytesRead);
        if (FAILED(hr)) {
            out << "Failed to read essence data" << std::endl;
            return;
        }
        
        // Создаем папку, если не существует
        std::filesystem::path filePath(outputPath);
        std::filesystem::create_directories(filePath.parent_path());
        
        // Сохраняем в файл
        std::ofstream outFile(outputPath, std::ios::binary);
        if (!outFile) {
            out << "Failed to create output file: " << outputPath << std::endl;
            return;
        }
        
        outFile.write(reinterpret_cast<const char*>(buffer.data()), bytesRead);
        outFile.close();
        
    } catch (const std::exception& e) {
        out << "Error extracting embedded audio: " << e.what() << std::endl;
    }
}

std::string findAndExtractEssenceData(IAAFHeader* pHeader, const aafMobID_t& mobID, 
                                     const std::string& mobIdStr, const std::map<std::string, std::string>& mobIdToName, std::ofstream& out) {
    if (!pHeader) return "";
    
    out << "  [EMBEDDED] Searching for EssenceData with MobID: " << mobIdStr << std::endl;
    
    // Сначала попробуем найти SourceMob (FileMob) с этим ID
    aafSearchCrit_t searchCrit;
    searchCrit.searchTag = kAAFByMobKind;
    searchCrit.tags.mobKind = kAAFFileMob;  // SourceMob это FileMob в AAF
    
    IEnumAAFMobs* pSourceMobEnum = nullptr;
    if (SUCCEEDED(pHeader->GetMobs(&searchCrit, &pSourceMobEnum))) {
        IAAFMob* pSourceMob = nullptr;
        while (SUCCEEDED(pSourceMobEnum->NextOne(&pSourceMob))) {
            aafMobID_t sourceMobID;
            if (SUCCEEDED(pSourceMob->GetMobID(&sourceMobID))) {
                if (memcmp(&mobID, &sourceMobID, sizeof(aafMobID_t)) == 0) {
                    out << "  [EMBEDDED] Found matching SourceMob!" << std::endl;
                    
                    // Проверяем, есть ли у этого SourceMob EssenceDescriptor
                    IAAFSourceMob* pSrcMob = nullptr;
                    if (SUCCEEDED(pSourceMob->QueryInterface(IID_IAAFSourceMob, (void**)&pSrcMob))) {
                        IAAFEssenceDescriptor* pEssDesc = nullptr;
                        if (SUCCEEDED(pSrcMob->GetEssenceDescriptor(&pEssDesc))) {
                            out << "  [EMBEDDED] Found EssenceDescriptor - potentially embedded!" << std::endl;
                            
                            // Создаем имя файла для embedded контента, используя правильное имя из маппинга
                            std::string fileName = "embedded_" + mobIdStr + ".wav";
                            auto it = mobIdToName.find(mobIdStr);
                            if (it != mobIdToName.end() && !it->second.empty()) {
                                // Используем правильное имя файла из маппинга
                                fileName = it->second;
                                // Добавляем расширение .wav если его нет
                                if (fileName.find('.') == std::string::npos) {
                                    fileName += ".wav";
                                }
                                out << "  [EMBEDDED] Using mapped filename: " << fileName << std::endl;
                            }
                            std::string outputPath = "extracted_media/" + fileName;
                            
                            // Пытаемся найти соответствующие EssenceData
                            IEnumAAFEssenceData* pEssenceEnum = nullptr;
                            if (SUCCEEDED(pHeader->EnumEssenceData(&pEssenceEnum))) {
                                IAAFEssenceData* pEssenceData = nullptr;
                                while (SUCCEEDED(pEssenceEnum->NextOne(&pEssenceData))) {
                                    aafMobID_t essenceMobID;
                                    if (SUCCEEDED(pEssenceData->GetFileMobID(&essenceMobID))) {
                                        if (memcmp(&mobID, &essenceMobID, sizeof(aafMobID_t)) == 0) {
                                            out << "  [EMBEDDED] Found matching EssenceData!" << std::endl;
                                            
                                            // Извлекаем данные
                                            extractEmbeddedAudio(pEssenceData, outputPath, out);
                                            
                                            pEssenceData->Release();
                                            pEssenceEnum->Release();
                                            pEssDesc->Release();
                                            pSrcMob->Release();
                                            pSourceMob->Release();
                                            pSourceMobEnum->Release();
                                            return outputPath;
                                        }
                                    }
                                    pEssenceData->Release();
                                }
                                pEssenceEnum->Release();
                            }
                            
                            // Если EssenceData не найдена, все равно помечаем как embedded
                            out << "  [EMBEDDED] No EssenceData found, but marked as embedded file" << std::endl;
                            pEssDesc->Release();
                        }
                        pSrcMob->Release();
                    }
                }
            }
            pSourceMob->Release();
        }
        pSourceMobEnum->Release();
    }
    
    // Если SourceMob не найден, попробуем прямой поиск EssenceData
    IEnumAAFEssenceData* pEssenceEnum = nullptr;
    if (SUCCEEDED(pHeader->EnumEssenceData(&pEssenceEnum))) {
        IAAFEssenceData* pEssenceData = nullptr;
        while (SUCCEEDED(pEssenceEnum->NextOne(&pEssenceData))) {
            aafMobID_t essenceMobID;
            if (SUCCEEDED(pEssenceData->GetFileMobID(&essenceMobID))) {
                if (memcmp(&mobID, &essenceMobID, sizeof(aafMobID_t)) == 0) {
                    out << "  [EMBEDDED] Found direct EssenceData match!" << std::endl;
                    
                    // Создаем имя файла для embedded контента, используя правильное имя из маппинга
                    std::string fileName = "embedded_" + mobIdStr + ".wav";
                    auto it = mobIdToName.find(mobIdStr);
                    if (it != mobIdToName.end() && !it->second.empty()) {
                        // Используем правильное имя файла из маппинга
                        fileName = it->second;
                        // Добавляем расширение .wav если его нет
                        if (fileName.find('.') == std::string::npos) {
                            fileName += ".wav";
                        }
                        out << "  [EMBEDDED] Using mapped filename: " << fileName << std::endl;
                    }
                    std::string outputPath = "extracted_media/" + fileName;
                    extractEmbeddedAudio(pEssenceData, outputPath, out);
                    
                    pEssenceData->Release();
                    pEssenceEnum->Release();
                    return outputPath;
                }
            }
            pEssenceData->Release();
        }
        pEssenceEnum->Release();
    }
    
    out << "  [EMBEDDED] No matching EssenceData found" << std::endl;
    return "";
}

bool isFileEmbedded(const std::set<std::string>& embeddedMobIDs, const std::string& mobIdStr) {
    return embeddedMobIDs.find(mobIdStr) != embeddedMobIDs.end();
}

void processAudioFiles(const ProjectData& projectData, std::ofstream& out) {
    out << "\n🎵 === PROCESSING AUDIO FILES ===" << std::endl;
    out << "[*] Created audio_files folder for all project audio" << std::endl;
    
    std::set<std::string> uniqueFiles;
    int copiedFiles = 0;
    int embeddedFiles = 0;
    int missingFiles = 0;
    
    // Собираем все уникальные файлы из всех треков
    for (const auto& track : projectData.tracks) {
        for (const auto& clip : track.clips) {
            if (clip.fileName.empty()) continue;
            uniqueFiles.insert(clip.fileName);
        }
    }
    
    out << "[*] Found " << uniqueFiles.size() << " unique audio files to process:" << std::endl;
    
    // Возможные расширения аудиофайлов
    std::vector<std::string> audioExtensions = {".wav", ".aiff", ".aif", ".mp3", ".flac", ".ogg", ".m4a"};
    
    // Возможные пути для поиска
    std::vector<std::string> searchPaths = {
        ".",
        "..",
        "../../",
        "../../../",
        "../../../../",
        "audio/",
        "Audio/",
        "media/",
        "Media/",
        "sounds/",
        "Sounds/",
        "J:/Nuendo PROJECTS SSD/Other/DP/Bezhanov/BEZHANOV/",
        "J:/Nuendo PROJECTS SSD/Other/DP/Bezhanov/",
        "J:/Nuendo PROJECTS SSD/Other/DP/",
        "J:/Nuendo PROJECTS SSD/Other/",
        "J:/Nuendo PROJECTS SSD/",
        "J:/"
    };
    
    for (const auto& fileName : uniqueFiles) {
        try {
            // Проверяем, является ли файл embedded
            if (fileName.find("embedded_") == 0 || fileName.find("extracted_media/embedded_") != std::string::npos) {
                out << "  📦 Embedded: " << fileName << std::endl;
                embeddedFiles++;
            } else if (fileName == "(silence)" || fileName.find("unknown_") == 0) {
                out << "  ❌ Missing: " << fileName << std::endl;
                missingFiles++;
            } else {
                // Обычный внешний файл - пытаемся найти и скопировать
                bool found = false;
                std::string foundPath;
                
                // Сначала проверяем точное имя файла
                if (std::filesystem::exists(fileName)) {
                    foundPath = fileName;
                    found = true;
                } else {
                    // Ищем файл в различных директориях
                    for (const auto& searchPath : searchPaths) {
                        std::string fullPath = searchPath + "/" + fileName;
                        if (std::filesystem::exists(fullPath)) {
                            foundPath = fullPath;
                            found = true;
                            break;
                        }
                        
                        // Если файл имеет нестандартное расширение (например, .mxf_L), 
                        // попробуем найти с аудио расширениями
                        std::string baseName = fileName;
                        size_t lastDot = baseName.find_last_of('.');
                        if (lastDot != std::string::npos) {
                            baseName = baseName.substr(0, lastDot);
                        }
                        
                        // Убираем суффиксы типа _L, _R
                        if (baseName.length() > 2) {
                            if (baseName.substr(baseName.length() - 2) == "_L" || 
                                baseName.substr(baseName.length() - 2) == "_R") {
                                baseName = baseName.substr(0, baseName.length() - 2);
                            }
                        }
                        
                        for (const auto& ext : audioExtensions) {
                            std::string testPath = searchPath + "/" + baseName + ext;
                            if (std::filesystem::exists(testPath)) {
                                foundPath = testPath;
                                found = true;
                                break;
                            }
                        }
                        
                        if (found) break;
                    }
                }
                
                if (found) {
                    try {
                        std::string destPath = "audio_files/" + std::filesystem::path(foundPath).filename().string();
                        std::filesystem::copy_file(foundPath, destPath, 
                                                 std::filesystem::copy_options::overwrite_existing);
                        out << "  📁 Copied: " << foundPath << " -> " << destPath << std::endl;
                        copiedFiles++;
                    } catch (const std::exception& e) {
                        out << "  ❌ Error copying " << foundPath << ": " << e.what() << std::endl;
                        missingFiles++;
                    }
                } else {
                    out << "  ❌ File not found: " << fileName << " (searched in multiple locations)" << std::endl;
                    missingFiles++;
                }
            }
        } catch (const std::exception& e) {
            out << "  ❌ Error processing " << fileName << ": " << e.what() << std::endl;
            missingFiles++;
        }
    }
    
    out << "\n📊 Audio Files Summary:" << std::endl;
    out << "  Copied external files: " << copiedFiles << std::endl;
    out << "  Embedded files: " << embeddedFiles << std::endl;
    out << "  Missing files: " << missingFiles << std::endl;
    out << "  Total unique files: " << uniqueFiles.size() << std::endl;
}

void debugEssenceData(IAAFHeader* pHeader, std::ofstream& out) {
    if (!pHeader) return;
    
    out << "\n🔍 === DEBUGGING ESSENCE DATA ===" << std::endl;
    
    // Подсчитываем общее количество EssenceData
    int essenceCount = 0;
    IEnumAAFEssenceData* pEssenceEnum = nullptr;
    if (SUCCEEDED(pHeader->EnumEssenceData(&pEssenceEnum))) {
        IAAFEssenceData* pEssenceData = nullptr;
        while (SUCCEEDED(pEssenceEnum->NextOne(&pEssenceData))) {
            essenceCount++;
            
            aafMobID_t essenceMobID;
            if (SUCCEEDED(pEssenceData->GetFileMobID(&essenceMobID))) {
                std::string mobIdStr = formatMobID(essenceMobID);
                
                aafLength_t dataSize = 0;
                if (SUCCEEDED(pEssenceData->GetSize(&dataSize))) {
                    out << "  EssenceData #" << essenceCount << ": MobID=" << mobIdStr 
                        << ", Size=" << dataSize << " bytes" << std::endl;
                } else {
                    out << "  EssenceData #" << essenceCount << ": MobID=" << mobIdStr 
                        << ", Size=unknown" << std::endl;
                }
            }
            
            pEssenceData->Release();
        }
        pEssenceEnum->Release();
    }
    
    out << "Total EssenceData found: " << essenceCount << std::endl;
}

std::set<std::string> buildEmbeddedFilesMap(IAAFHeader* pHeader, std::ofstream& out) {
    std::set<std::string> embeddedMobIDs;
    
    if (!pHeader) return embeddedMobIDs;
    
    out << "\n🔍 === BUILDING EMBEDDED FILES MAP ===" << std::endl;
    
    // Получаем все EssenceData MobID
    std::set<std::string> essenceDataMobIDs;
    IEnumAAFEssenceData* pEssenceEnum = nullptr;
    if (SUCCEEDED(pHeader->EnumEssenceData(&pEssenceEnum))) {
        IAAFEssenceData* pEssenceData = nullptr;
        while (SUCCEEDED(pEssenceEnum->NextOne(&pEssenceData))) {
            aafMobID_t essenceMobID;
            if (SUCCEEDED(pEssenceData->GetFileMobID(&essenceMobID))) {
                std::string mobIdStr = formatMobID(essenceMobID);
                essenceDataMobIDs.insert(mobIdStr);
            }
            pEssenceData->Release();
        }
        pEssenceEnum->Release();
    }
    
    out << "[*] Found " << essenceDataMobIDs.size() << " EssenceData entries" << std::endl;
    
    // Теперь ищем FileMob, которые ссылаются на эти EssenceData
    aafSearchCrit_t searchCrit;
    searchCrit.searchTag = kAAFByMobKind;
    searchCrit.tags.mobKind = kAAFFileMob;
    
    IEnumAAFMobs* pMobEnum = nullptr;
    if (SUCCEEDED(pHeader->GetMobs(&searchCrit, &pMobEnum))) {
        IAAFMob* pMob = nullptr;
        while (SUCCEEDED(pMobEnum->NextOne(&pMob))) {
            aafMobID_t mobID;
            if (SUCCEEDED(pMob->GetMobID(&mobID))) {
                std::string mobIdStr = formatMobID(mobID);
                
                // Проверяем, есть ли EssenceData для этого FileMob
                if (essenceDataMobIDs.find(mobIdStr) != essenceDataMobIDs.end()) {
                    embeddedMobIDs.insert(mobIdStr);
                    out << "[EMBEDDED] FileMob " << mobIdStr << " has EssenceData" << std::endl;
                }
            }
            pMob->Release();
        }
        pMobEnum->Release();
    }
    
    out << "[*] Total embedded files found: " << embeddedMobIDs.size() << std::endl;
    return embeddedMobIDs;
}

// Функция для поиска embedded файлов через цепочку ссылок (SourceClip -> MasterMob -> FileMob)
std::string findEmbeddedFileRecursive(IAAFHeader* pHeader, const aafMobID_t& mobID, 
                                     const std::set<std::string>& embeddedMobIDs,
                                     const std::map<std::string, std::string>& mobIdToName, 
                                     std::ofstream& out) {
    if (!pHeader) return "";
    
    std::string mobIdStr = formatMobID(mobID);
    out << "  [DEBUG] Checking for embedded file: " << mobIdStr << std::endl;
    
    // Сначала проверяем, не является ли сам MobID embedded
    if (embeddedMobIDs.find(mobIdStr) != embeddedMobIDs.end()) {
        out << "  [EMBEDDED] Direct embedded file found: " << mobIdStr << std::endl;
        // Используем имя файла из mobIdToName (которое уже содержит пути к извлеченным файлам)
        if (mobIdToName.count(mobIdStr)) {
            std::string filePath = mobIdToName.at(mobIdStr);
            out << "  [EMBEDDED] Using extracted file: " << filePath << std::endl;
            return filePath;
        }
        return "";
    }
    
    // Если нет, ищем MasterMob/CompositionMob с этим ID
    IAAFMob* pMob = nullptr;
    if (SUCCEEDED(pHeader->LookupMob(mobID, &pMob))) {
        out << "  [DEBUG] Found Mob with ID: " << mobIdStr << std::endl;
        
        // Получаем слоты этого Mob
        IEnumAAFMobSlots* pSlotEnum = nullptr;
        if (SUCCEEDED(pMob->GetSlots(&pSlotEnum))) {
            IAAFMobSlot* pSlot = nullptr;
            while (SUCCEEDED(pSlotEnum->NextOne(&pSlot))) {
                IAAFSegment* pSegment = nullptr;
                if (SUCCEEDED(pSlot->GetSegment(&pSegment))) {
                    // Проверяем, является ли сегмент SourceClip
                    IAAFSourceClip* pSourceClip = nullptr;
                    if (SUCCEEDED(pSegment->QueryInterface(IID_IAAFSourceClip, (void**)&pSourceClip))) {
                        aafSourceRef_t sourceRef;
                        if (SUCCEEDED(pSourceClip->GetSourceReference(&sourceRef))) {
                            std::string refMobIdStr = formatMobID(sourceRef.sourceID);
                            out << "  [DEBUG] Found SourceClip reference: " << refMobIdStr << std::endl;
                            
                            // Проверяем, является ли этот SourceRef embedded
                            if (embeddedMobIDs.find(refMobIdStr) != embeddedMobIDs.end()) {
                                out << "  [EMBEDDED] Found embedded file via MasterMob: " << refMobIdStr << std::endl;
                                // Используем имя файла из mobIdToName (которое уже содержит пути к извлеченным файлам)
                                if (mobIdToName.count(refMobIdStr)) {
                                    std::string filePath = mobIdToName.at(refMobIdStr);
                                    out << "  [EMBEDDED] Using extracted file: " << filePath << std::endl;
                                    pSourceClip->Release();
                                    pSegment->Release();
                                    pSlot->Release();
                                    pSlotEnum->Release();
                                    pMob->Release();
                                    return filePath;
                                }
                            }
                        }
                        pSourceClip->Release();
                    }
                    pSegment->Release();
                }
                pSlot->Release();
            }
            pSlotEnum->Release();
        }
        pMob->Release();
    } else {
        out << "  [DEBUG] Mob not found: " << mobIdStr << std::endl;
    }
    
    return "";
}

// Новая функция для получения полной информации об embedded файле
EmbeddedFileInfo findEmbeddedFileInfo(IAAFHeader* pHeader, const aafMobID_t& mobID, 
                                     const std::set<std::string>& embeddedMobIDs,
                                     const std::map<std::string, std::string>& mobIdToName, 
                                     std::ofstream& out) {
    if (!pHeader) return EmbeddedFileInfo();
    
    std::string mobIdStr = formatMobID(mobID);
    out << "  [DEBUG] Checking for embedded file info: " << mobIdStr << std::endl;
    
    // Сначала проверяем, не является ли сам MobID embedded
    if (embeddedMobIDs.find(mobIdStr) != embeddedMobIDs.end()) {
        out << "  [EMBEDDED] Direct embedded file found: " << mobIdStr << std::endl;
        // Используем имя файла из mobIdToName (которое уже содержит пути к извлеченным файлам)
        if (mobIdToName.count(mobIdStr)) {
            std::string filePath = mobIdToName.at(mobIdStr);
            out << "  [EMBEDDED] Using extracted file: " << filePath << " with MOB ID: " << mobIdStr << std::endl;
            return EmbeddedFileInfo(filePath, mobIdStr);
        }
        return EmbeddedFileInfo();
    }
    
    // Если нет, ищем MasterMob/CompositionMob с этим ID
    IAAFMob* pMob = nullptr;
    if (SUCCEEDED(pHeader->LookupMob(mobID, &pMob))) {
        out << "  [DEBUG] Found Mob with ID: " << mobIdStr << std::endl;
        
        // Получаем слоты этого Mob
        IEnumAAFMobSlots* pSlotEnum = nullptr;
        if (SUCCEEDED(pMob->GetSlots(&pSlotEnum))) {
            IAAFMobSlot* pSlot = nullptr;
            while (SUCCEEDED(pSlotEnum->NextOne(&pSlot))) {
                IAAFSegment* pSegment = nullptr;
                if (SUCCEEDED(pSlot->GetSegment(&pSegment))) {
                    // Проверяем, является ли сегмент SourceClip
                    IAAFSourceClip* pSourceClip = nullptr;
                    if (SUCCEEDED(pSegment->QueryInterface(IID_IAAFSourceClip, (void**)&pSourceClip))) {
                        aafSourceRef_t sourceRef;
                        if (SUCCEEDED(pSourceClip->GetSourceReference(&sourceRef))) {
                            std::string refMobIdStr = formatMobID(sourceRef.sourceID);
                            out << "  [DEBUG] Found SourceClip reference: " << refMobIdStr << std::endl;
                            
                            // Проверяем, является ли этот SourceRef embedded
                            if (embeddedMobIDs.find(refMobIdStr) != embeddedMobIDs.end()) {
                                out << "  [EMBEDDED] Found embedded file via MasterMob: " << refMobIdStr << std::endl;
                                // Используем имя файла из mobIdToName и ПРАВИЛЬНЫЙ MOB ID embedded файла
                                if (mobIdToName.count(refMobIdStr)) {
                                    std::string filePath = mobIdToName.at(refMobIdStr);
                                    out << "  [EMBEDDED] Using extracted file: " << filePath << " with MOB ID: " << refMobIdStr << std::endl;
                                    pSourceClip->Release();
                                    pSegment->Release();
                                    pSlot->Release();
                                    pSlotEnum->Release();
                                    pMob->Release();
                                    return EmbeddedFileInfo(filePath, refMobIdStr);
                                }
                            }
                        }
                        pSourceClip->Release();
                    }
                    pSegment->Release();
                }
                pSlot->Release();
            }
            pSlotEnum->Release();
        }
        pMob->Release();
    } 
    
    return EmbeddedFileInfo(); // Не найден embedded файл
}

std::map<std::string, std::string> buildEmbeddedFileNameMapping(IAAFHeader* pHeader, 
                                                               const std::map<std::string, std::string>& mobIdToName, 
                                                               std::ofstream& out) {
    std::map<std::string, std::string> embeddedMapping;
    out << "\n🔗 === BUILDING EMBEDDED FILE NAME MAPPING ===" << std::endl;
    
    // Сначала собираем все embedded MobID
    std::set<std::string> embeddedMobIDs;
    IEnumAAFEssenceData* pEssenceEnum = nullptr;
    if (SUCCEEDED(pHeader->EnumEssenceData(&pEssenceEnum))) {
        IAAFEssenceData* pEssenceData = nullptr;
        while (SUCCEEDED(pEssenceEnum->NextOne(&pEssenceData))) {
            aafMobID_t essenceMobID;
            if (SUCCEEDED(pEssenceData->GetFileMobID(&essenceMobID))) {
                std::string mobIdStr = formatMobID(essenceMobID);
                embeddedMobIDs.insert(mobIdStr);
            }
            pEssenceData->Release();
        }
        pEssenceEnum->Release();
    }
    
    out << "[*] Found " << embeddedMobIDs.size() << " embedded files" << std::endl;
    
    // Упрощенный подход: ищем все MasterMob, которые ссылаются на embedded SourceMob
    IEnumAAFMobs* pMobEnum = nullptr;
    if (SUCCEEDED(pHeader->GetMobs(nullptr, &pMobEnum))) {
        IAAFMob* pMob = nullptr;
        while (SUCCEEDED(pMobEnum->NextOne(&pMob))) {
            IAAFMasterMob* pMasterMob = nullptr;
            if (SUCCEEDED(pMob->QueryInterface(IID_IAAFMasterMob, (void**)&pMasterMob))) {
                aafMobID_t masterMobID;
                if (SUCCEEDED(pMob->GetMobID(&masterMobID))) {
                    std::string masterMobIdStr = formatMobID(masterMobID);
                    
                    // Получаем имя MasterMob (это будет читаемое имя файла)
                    std::string readableName;
                    auto it = mobIdToName.find(masterMobIdStr);
                    if (it != mobIdToName.end()) {
                        readableName = it->second;
                        out << "[DEBUG] Processing MasterMob " << masterMobIdStr << " with name: " << readableName << std::endl;
                    }
                    
                    // Ищем слоты MasterMob
                    aafNumSlots_t numSlots = 0;
                    if (SUCCEEDED(pMob->CountSlots(&numSlots))) {
                        for (aafUInt32 i = 0; i < numSlots; ++i) {
                            IAAFMobSlot* pSlot = nullptr;
                            if (SUCCEEDED(pMob->GetSlotAt(i, &pSlot))) {
                                IAAFSegment* pSegment = nullptr;
                                if (SUCCEEDED(pSlot->GetSegment(&pSegment))) {
                                    IAAFSourceClip* pSourceClip = nullptr;
                                    if (SUCCEEDED(pSegment->QueryInterface(IID_IAAFSourceClip, (void**)&pSourceClip))) {
                                        aafSourceRef_t sourceRef;
                                        if (SUCCEEDED(pSourceClip->GetSourceReference(&sourceRef))) {
                                            std::string sourceMobIdStr = formatMobID(sourceRef.sourceID);
                                            out << "[DEBUG] MasterMob slot " << i << " references SourceMob: " << sourceMobIdStr << std::endl;
                                            
                                            // Проверяем, является ли этот SourceMob embedded
                                            if (embeddedMobIDs.find(sourceMobIdStr) != embeddedMobIDs.end()) {
                                                if (!readableName.empty() && readableName != "[MasterMob]") {
                                                    embeddedMapping[sourceMobIdStr] = readableName;
                                                    out << "[MAPPING] " << sourceMobIdStr << " -> " << readableName << std::endl;
                                                }
                                            }
                                        }
                                        pSourceClip->Release();
                                    }
                                    pSegment->Release();
                                }
                                pSlot->Release();
                            }
                        }
                    }
                }
                pMasterMob->Release();
            }
            pMob->Release();
        }
        pMobEnum->Release();
    }
    
    out << "[*] Built " << embeddedMapping.size() << " embedded file name mappings" << std::endl;
    return embeddedMapping;
}

// Вспомогательная функция для рекурсивного обхода сегментов
void traverseSegmentForEmbeddedMapping(IAAFSegment* pSegment, 
                                     IAAFHeader* pHeader,
                                     const std::set<std::string>& embeddedMobIDs,
                                     const std::map<std::string, std::string>& mobIdToName,
                                     std::map<std::string, std::string>& embeddedMapping,
                                     std::ofstream& out) {
    if (!pSegment) return;
    
    // Проверяем, является ли сегмент SourceClip
    IAAFSourceClip* pSourceClip = nullptr;
    if (SUCCEEDED(pSegment->QueryInterface(IID_IAAFSourceClip, (void**)&pSourceClip))) {
        aafSourceRef_t sourceRef;
        if (SUCCEEDED(pSourceClip->GetSourceReference(&sourceRef))) {
            std::string masterMobIdStr = formatMobID(sourceRef.sourceID);
            out << "[DEBUG] Found SourceClip referencing MasterMob: " << masterMobIdStr << std::endl;
            
            // Получаем имя для этого MasterMob
            std::string readableName;
            auto it = mobIdToName.find(masterMobIdStr);
            if (it != mobIdToName.end()) {
                readableName = it->second;
                out << "[DEBUG] MasterMob " << masterMobIdStr << " has name: " << readableName << std::endl;
            }
            
            // Теперь ищем MasterMob и смотрим, на какие SourceMob он ссылается
            IEnumAAFMobs* pMobEnum = nullptr;
            if (SUCCEEDED(pHeader->GetMobs(nullptr, &pMobEnum))) {
                IAAFMob* pMob = nullptr;
                while (SUCCEEDED(pMobEnum->NextOne(&pMob))) {
                    aafMobID_t mobID;
                    if (SUCCEEDED(pMob->GetMobID(&mobID))) {
                        std::string mobIdStr = formatMobID(mobID);
                        
                        // Если это нужный MasterMob
                        if (mobIdStr == masterMobIdStr) {
                            IAAFMasterMob* pMasterMob = nullptr;
                            if (SUCCEEDED(pMob->QueryInterface(IID_IAAFMasterMob, (void**)&pMasterMob))) {
                                out << "[DEBUG] Found MasterMob: " << masterMobIdStr << std::endl;
                                
                                // Ищем слоты MasterMob
                                aafNumSlots_t numSlots = 0;
                                if (SUCCEEDED(pMob->CountSlots(&numSlots))) {
                                    for (aafUInt32 j = 0; j < numSlots; ++j) {
                                        IAAFMobSlot* pMasterSlot = nullptr;
                                        if (SUCCEEDED(pMob->GetSlotAt(j, &pMasterSlot))) {
                                            IAAFSegment* pMasterSegment = nullptr;
                                            if (SUCCEEDED(pMasterSlot->GetSegment(&pMasterSegment))) {
                                                IAAFSourceClip* pMasterSourceClip = nullptr;
                                                if (SUCCEEDED(pMasterSegment->QueryInterface(IID_IAAFSourceClip, (void**)&pMasterSourceClip))) {
                                                    aafSourceRef_t masterSourceRef;
                                                    if (SUCCEEDED(pMasterSourceClip->GetSourceReference(&masterSourceRef))) {
                                                        std::string sourceMobIdStr = formatMobID(masterSourceRef.sourceID);
                                                        out << "[DEBUG] MasterMob slot " << j << " references SourceMob: " << sourceMobIdStr << std::endl;
                                                        
                                                        // Проверяем, является ли этот SourceMob embedded
                                                        if (embeddedMobIDs.find(sourceMobIdStr) != embeddedMobIDs.end()) {
                                                            if (!readableName.empty() && readableName != "[MasterMob]") {
                                                                embeddedMapping[sourceMobIdStr] = readableName;
                                                                out << "[MAPPING] " << sourceMobIdStr << " -> " << readableName << std::endl;
                                                            }
                                                        }
                                                    }
                                                    pMasterSourceClip->Release();
                                                }
                                                pMasterSegment->Release();
                                            }
                                            pMasterSlot->Release();
                                        }
                                    }
                                }
                                pMasterMob->Release();
                            }
                            break; // Нашли нужный MasterMob
                        }
                    }
                    pMob->Release();
                }
                pMobEnum->Release();
            }
        }
        pSourceClip->Release();
    }
    
    // Также проверяем, является ли сегмент Sequence (может содержать вложенные сегменты)
    IAAFSequence* pSequence = nullptr;
    if (SUCCEEDED(pSegment->QueryInterface(IID_IAAFSequence, (void**)&pSequence))) {
        out << "[DEBUG] Found Sequence, traversing components..." << std::endl;
        IEnumAAFComponents* pCompEnum = nullptr;
        if (SUCCEEDED(pSequence->GetComponents(&pCompEnum))) {
            IAAFComponent* pComponent = nullptr;
            while (SUCCEEDED(pCompEnum->NextOne(&pComponent))) {
                IAAFSegment* pComponentSegment = nullptr;
                if (SUCCEEDED(pComponent->QueryInterface(IID_IAAFSegment, (void**)&pComponentSegment))) {
                    traverseSegmentForEmbeddedMapping(pComponentSegment, pHeader, embeddedMobIDs, mobIdToName, embeddedMapping, out);
                    pComponentSegment->Release();
                }
                pComponent->Release();
            }
            pCompEnum->Release();
        }
        pSequence->Release();
    }
}

std::map<std::string, std::string> extractAllEmbeddedFiles(IAAFHeader* pHeader, 
                                                       const std::map<std::string, std::string>& mobIdToName, 
                                                       const std::map<std::string, std::string>& embeddedNameMapping, 
                                                       std::ofstream& out) {
    std::map<std::string, std::string> embeddedFileMap; // MobID -> extracted file path
    
    if (!pHeader) return embeddedFileMap;
    
    out << "\n🎵 === EXTRACTING ALL EMBEDDED FILES ===" << std::endl;
    
    int extractedCount = 0;
    
    IEnumAAFEssenceData* pEssenceEnum = nullptr;
    if (SUCCEEDED(pHeader->EnumEssenceData(&pEssenceEnum))) {
        IAAFEssenceData* pEssenceData = nullptr;
        while (SUCCEEDED(pEssenceEnum->NextOne(&pEssenceData))) {
            aafMobID_t essenceMobID;
            if (SUCCEEDED(pEssenceData->GetFileMobID(&essenceMobID))) {
                std::string mobIdStr = formatMobID(essenceMobID);
                
                aafLength_t dataSize = 0;
                if (SUCCEEDED(pEssenceData->GetSize(&dataSize)) && dataSize > 0) {
                    // Генерируем имя файла
                    std::string outputPath;
                    
                    // Приоритет 1: Глобальная карта embedded файлов (собранная во время анализа клипов)
                    auto globalIt = g_embeddedFileNames.find(mobIdStr);
                    if (globalIt != g_embeddedFileNames.end() && !globalIt->second.empty()) {
                        std::string originalName = globalIt->second;
                        out << "[DEBUG] Found global mapping for " << mobIdStr << " -> " << originalName << std::endl;
                        
                        // Определяем правильное расширение файла по содержимому
                        std::string detectedExtension = detectFileExtension(pEssenceData, out);
                        
                        // Убираем старое расширение и добавляем правильное
                        size_t dotPos = originalName.find_last_of('.');
                        if (dotPos != std::string::npos) {
                            originalName = originalName.substr(0, dotPos);
                        }
                        
                        outputPath = "extracted_media/" + originalName + detectedExtension;
                    } 
                    // Приоритет 2: Переданный маппинг embedded файлов
                    else {
                        auto embeddedIt = embeddedNameMapping.find(mobIdStr);
                        if (embeddedIt != embeddedNameMapping.end() && !embeddedIt->second.empty()) {
                            std::string originalName = embeddedIt->second;
                            out << "[DEBUG] Found embedded mapping for " << mobIdStr << " -> " << originalName << std::endl;
                            
                            // Определяем правильное расширение файла по содержимому
                            std::string detectedExtension = detectFileExtension(pEssenceData, out);
                            
                            // Убираем старое расширение и добавляем правильное
                            size_t dotPos = originalName.find_last_of('.');
                            if (dotPos != std::string::npos) {
                                originalName = originalName.substr(0, dotPos);
                            }
                            
                            outputPath = "extracted_media/" + originalName + detectedExtension;
                        } else {
                            // Приоритет 3: Пытаемся найти SourceMob для этого EssenceData и извлечь имя файла
                        std::string extractedName;
                        IEnumAAFMobs* pMobEnum = nullptr;
                        if (SUCCEEDED(pHeader->GetMobs(nullptr, &pMobEnum))) {
                            IAAFMob* pMob = nullptr;
                            while (SUCCEEDED(pMobEnum->NextOne(&pMob))) {
                                aafMobID_t mobID;
                                if (SUCCEEDED(pMob->GetMobID(&mobID))) {
                                    std::string currentMobIdStr = formatMobID(mobID);
                                    if (currentMobIdStr == mobIdStr) {
                                        // Это SourceMob для этого embedded файла
                                        IAAFSourceMob* pSourceMob = nullptr;
                                        if (SUCCEEDED(pMob->QueryInterface(IID_IAAFSourceMob, (void**)&pSourceMob))) {
                                            IAAFEssenceDescriptor* pEssDesc = nullptr;
                                            if (SUCCEEDED(pSourceMob->GetEssenceDescriptor(&pEssDesc))) {
                                                // Пытаемся получить имя файла через Locators
                                                aafUInt32 numLocators = 0;
                                                if (SUCCEEDED(pEssDesc->CountLocators(&numLocators)) && numLocators > 0) {
                                                    IEnumAAFLocators* pLocatorEnum = nullptr;
                                                    if (SUCCEEDED(pEssDesc->GetLocators(&pLocatorEnum))) {
                                                        IAAFLocator* pLocator = nullptr;
                                                        if (SUCCEEDED(pLocatorEnum->NextOne(&pLocator))) {
                                                            aafUInt32 pathBufSize = 0;
                                                            if (SUCCEEDED(pLocator->GetPathBufLen(&pathBufSize)) && pathBufSize > 0) {
                                                                std::vector<aafCharacter> pathBuffer(pathBufSize / sizeof(aafCharacter));
                                                                if (SUCCEEDED(pLocator->GetPath(pathBuffer.data(), pathBufSize))) {
                                                                    std::string path = wideToUtf8(pathBuffer.data());
                                                                    // Извлекаем имя файла из пути
                                                                    size_t lastSlash = path.find_last_of("/\\");
                                                                    if (lastSlash != std::string::npos) {
                                                                        extractedName = path.substr(lastSlash + 1);
                                                                    } else {
                                                                        extractedName = path;
                                                                    }
                                                                    out << "[DEBUG] Extracted filename from SourceMob " << mobIdStr << ": " << extractedName << std::endl;
                                                                }
                                                            }
                                                            pLocator->Release();
                                                        }
                                                        pLocatorEnum->Release();
                                                    }
                                                }
                                                pEssDesc->Release();
                                            }
                                            pSourceMob->Release();
                                        }
                                        break;
                                    }
                                }
                                pMob->Release();
                            }
                            pMobEnum->Release();
                        }
                        
                        if (!extractedName.empty()) {
                            // Определяем правильное расширение файла по содержимому
                            std::string detectedExtension = detectFileExtension(pEssenceData, out);
                            
                            // Убираем старое расширение и добавляем правильное
                            size_t dotPos = extractedName.find_last_of('.');
                            if (dotPos != std::string::npos) {
                                extractedName = extractedName.substr(0, dotPos);
                            }
                            
                            outputPath = "extracted_media/" + extractedName + detectedExtension;
                        } else {
                            // Используем общий маппинг MobID -> имя файла
                            auto generalIt = mobIdToName.find(mobIdStr);
                            if (generalIt != mobIdToName.end() && !generalIt->second.empty()) {
                                std::string mappedName = generalIt->second;
                                out << "[DEBUG] Found general mapping for " << mobIdStr << " -> " << mappedName << std::endl;
                                
                                // Определяем правильное расширение файла
                                std::string detectedExtension = detectFileExtension(pEssenceData, out);
                                
                                // Убираем старое расширение и добавляем правильное
                                size_t dotPos = mappedName.find_last_of('.');
                                if (dotPos != std::string::npos) {
                                    mappedName = mappedName.substr(0, dotPos);
                                }
                                
                                outputPath = "extracted_media/" + mappedName + detectedExtension;
                            } else {
                                // Fallback: генерируем имя на основе MobID с правильным расширением
                                std::string detectedExtension = detectFileExtension(pEssenceData, out);
                                outputPath = "extracted_media/embedded_audio_" + mobIdStr + detectedExtension;
                                out << "[DEBUG] Using fallback name for " << mobIdStr << " -> " << outputPath << std::endl;
                            }
                        }
                        }
                    }
                    
                    out << "[EXTRACT] Extracting embedded file " << mobIdStr << " (" << dataSize << " bytes) -> " << outputPath << std::endl;
                    
                    // Извлекаем файл
                    extractEmbeddedAudio(pEssenceData, outputPath, out);
                    
                    // Добавляем в карту извлеченных файлов
                    embeddedFileMap[mobIdStr] = outputPath;
                    extractedCount++;
                }
            }
            pEssenceData->Release();
        }
        pEssenceEnum->Release();
    }
    
    out << "[*] Extracted " << extractedCount << " embedded files" << std::endl;
    return embeddedFileMap;
}

// Функция для определения правильного расширения файла по essence data
std::string detectFileExtension(IAAFEssenceData* pEssenceData, std::ofstream& out) {
    if (!pEssenceData) return ".wav";
    
    // Читаем первые байты для определения формата
    aafUInt32 bytesToRead = 12;
    aafUInt8 buffer[12];
    aafUInt32 bytesRead = 0;
    
    if (SUCCEEDED(pEssenceData->Read(bytesToRead, buffer, &bytesRead)) && bytesRead >= 4) {
        // Возвращаемся к началу файла
        pEssenceData->SetPosition(0);
        
        // Проверяем заголовок файла
        if (bytesRead >= 4) {
            // AIFF/AIFC файлы начинаются с "FORM"
            if (buffer[0] == 'F' && buffer[1] == 'O' && buffer[2] == 'R' && buffer[3] == 'M') {
                if (bytesRead >= 12) {
                    // Проверяем тип AIFF
                    if (buffer[8] == 'A' && buffer[9] == 'I' && buffer[10] == 'F') {
                        if (buffer[11] == 'C') {
                            out << "[FORMAT] Detected AIFF-C format -> .aif" << std::endl;
                            return ".aif";
                        } else if (buffer[11] == 'F') {
                            out << "[FORMAT] Detected AIFF format -> .aif" << std::endl;
                            return ".aif";
                        }
                    }
                }
                out << "[FORMAT] Detected FORM container -> .aif" << std::endl;
                return ".aif";
            }
            // WAV файлы начинаются с "RIFF"
            else if (buffer[0] == 'R' && buffer[1] == 'I' && buffer[2] == 'F' && buffer[3] == 'F') {
                out << "[FORMAT] Detected WAV format -> .wav" << std::endl;
                return ".wav";
            }
        }
    }
    
    // По умолчанию возвращаем .wav
    out << "[FORMAT] Unknown format, using default -> .wav" << std::endl;
    return ".wav";
}
