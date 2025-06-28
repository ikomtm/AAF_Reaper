#include "media_utils.h"
#include "aaf_utils.h"
#include <AAF.h>
#include <AAFResult.h>
#include <AAFTypes.h>
#include <AAFDataDefs.h>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <sstream>

// Глобальные карты для сбора имён embedded файлов во время анализа
std::map<std::string, std::string> g_embeddedFileNames; // MobID -> правильное имя файла
std::map<std::string, std::string> g_masterMobToFileName; // MasterMob MobID (из CSV) -> имена файлов

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
        
        // Получаем информацию о свойствах аудио из FileMob
        aafMobID_t essenceMobID;
        if (SUCCEEDED(pEssenceData->GetFileMobID(&essenceMobID))) {
            out << "  📊 AUDIO PROPERTIES:" << std::endl;
            out << "    • Data Size: " << dataSize << " bytes" << std::endl;
            
            // Попытаемся получить дополнительную информацию через FileMob
            IAAFMob* pFileMob = nullptr;
            if (SUCCEEDED(pEssenceData->QueryInterface(IID_IAAFMob, (void**)&pFileMob))) {
                IAAFSourceMob* pSourceMob = nullptr;
                if (SUCCEEDED(pFileMob->QueryInterface(IID_IAAFSourceMob, (void**)&pSourceMob))) {
                    IAAFEssenceDescriptor* pEssenceDesc = nullptr;
                    if (SUCCEEDED(pSourceMob->GetEssenceDescriptor(&pEssenceDesc))) {
                        // Пытаемся получить SoundDescriptor для аудио информации
                        IAAFSoundDescriptor* pSoundDesc = nullptr;
                        if (SUCCEEDED(pEssenceDesc->QueryInterface(IID_IAAFSoundDescriptor, (void**)&pSoundDesc))) {
                            aafRational_t sampleRate;
                            if (SUCCEEDED(pSoundDesc->GetAudioSamplingRate(&sampleRate))) {
                                double rate = (double)sampleRate.numerator / (double)sampleRate.denominator;
                                out << "    • Sample Rate: " << rate << " Hz" << std::endl;
                            }
                            
                            aafUInt32 channels = 0;
                            if (SUCCEEDED(pSoundDesc->GetChannelCount(&channels))) {
                                out << "    • Channels: " << channels << std::endl;
                            }
                            
                            aafUInt32 quantizationBits = 0;
                            if (SUCCEEDED(pSoundDesc->GetQuantizationBits(&quantizationBits))) {
                                out << "    • Bit Depth: " << quantizationBits << " bits" << std::endl;
                            }
                            
                            pSoundDesc->Release();
                        }
                        pEssenceDesc->Release();
                    }
                    pSourceMob->Release();
                }
                pFileMob->Release();
            }
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
        
        out << "  ✅ Extracted: " << outputPath << std::endl;
        
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





std::map<std::string, std::string> buildEmbeddedFileNameMapping(IAAFHeader* pHeader, 
                                                               const std::map<std::string, std::string>& mobIdToName, 
                                                               std::ofstream& out) {
    std::map<std::string, std::string> embeddedMapping;
    out << "\n🔗 === BUILDING EMBEDDED FILE NAME MAPPING (ROBUST) ===" << std::endl;
    
    // Используем robust mapping для связи EssenceData с MasterMob
    IEnumAAFEssenceData* pEssenceEnum = nullptr;
    if (SUCCEEDED(pHeader->EnumEssenceData(&pEssenceEnum))) {
        IAAFEssenceData* pEssenceData = nullptr;
        int essenceCount = 0;
        
        while (SUCCEEDED(pEssenceEnum->NextOne(&pEssenceData))) {
            essenceCount++;
            aafMobID_t essenceMobID;
            if (SUCCEEDED(pEssenceData->GetFileMobID(&essenceMobID))) {
                std::string essenceMobIdStr = formatMobID(essenceMobID);
                out << "[ESSENCE " << essenceCount << "] Processing EssenceData with MobID: " << essenceMobIdStr << std::endl;
                
                // Используем robust mapping для поиска соответствующего MasterMob
                IAAFMob* pMasterMob = findMasterMobFromEssenceData(pHeader, pEssenceData);
                if (pMasterMob) {
                    out << "[DEBUG] Found MasterMob for EssenceData " << essenceMobIdStr << std::endl;
                    aafMobID_t masterMobID;
                    if (SUCCEEDED(pMasterMob->GetMobID(&masterMobID))) {
                        std::string masterMobIdStr = formatMobID(masterMobID);
                        std::string finalFileName;
                        
                        // ПРИОРИТЕТ 1: Попытаемся получить имя файла из EssenceDescriptor (Locator)
                        IAAFSourceMob* pSourceMob = nullptr;
                        if (SUCCEEDED(pMasterMob->QueryInterface(IID_IAAFSourceMob, (void**)&pSourceMob))) {
                            IAAFEssenceDescriptor* pEssenceDesc = nullptr;
                            if (SUCCEEDED(pSourceMob->GetEssenceDescriptor(&pEssenceDesc))) {
                                std::string locatorFileName = getFileNameFromEssenceDescriptor(pEssenceDesc);
                                if (!locatorFileName.empty() && locatorFileName != "embedded_essence.dat") {
                                    finalFileName = locatorFileName;
                                    out << "[LOCATOR] Found file name from EssenceDescriptor: " << locatorFileName << std::endl;
                                }
                                pEssenceDesc->Release();
                            }
                            
                            // ПРИОРИТЕТ 1.5: Если EssenceDescriptor не дал результата, пробуем имя SourceMob
                            if (finalFileName.empty()) {
                                IAAFMob* pMobInterface = nullptr;
                                if (SUCCEEDED(pSourceMob->QueryInterface(IID_IAAFMob, (void**)&pMobInterface))) {
                                    aafCharacter mobNameBuf[256] = {0};
                                    if (SUCCEEDED(pMobInterface->GetName(mobNameBuf, 256))) {
                                        std::string mobName;
                                        for (int i = 0; i < 256 && mobNameBuf[i] != 0; i++) {
                                            mobName += (char)mobNameBuf[i];
                                        }
                                        if (!mobName.empty() && mobName != "[SourceMob]") {
                                            // Добавляем расширение если его нет
                                            if (mobName.find('.') == std::string::npos) {
                                                mobName += ".wav"; // По умолчанию для аудио
                                            }
                                            finalFileName = mobName;
                                            out << "[SOURCE_MOB] Found file name from SourceMob name: " << mobName << std::endl;
                                        }
                                    }
                                    pMobInterface->Release();
                                }
                            }
                            
                            pSourceMob->Release();
                        }
                        
                        // ПРИОРИТЕТ 2: Если предыдущие методы не дали результата, используем имя MasterMob из карты
                        if (finalFileName.empty()) {
                            auto it = mobIdToName.find(masterMobIdStr);
                            if (it != mobIdToName.end() && !it->second.empty() && it->second != "[MasterMob]") {
                                finalFileName = it->second;
                                out << "[FALLBACK] Using MasterMob name: " << finalFileName << std::endl;
                            }
                        }
                        
                        // Если получили имя файла, заполняем карты
                        if (!finalFileName.empty()) {
                            embeddedMapping[essenceMobIdStr] = finalFileName;
                            
                            // Также заполняем глобальные карты для использования при экстракции
                            g_embeddedFileNames[essenceMobIdStr] = finalFileName;
                            g_masterMobToFileName[masterMobIdStr] = finalFileName;
                            
                            out << "[ROBUST MAPPING] EssenceData " << essenceMobIdStr 
                                << " -> MasterMob " << masterMobIdStr 
                                << " -> File: " << finalFileName << std::endl;
                        } else {
                            out << "[WARNING] No readable name found for MasterMob " << masterMobIdStr << std::endl;
                            
                            // FALLBACK: Генерируем уникальное имя на основе MobID
                            std::ostringstream fallbackName;
                            fallbackName << "embedded_audio_" << essenceMobIdStr.substr(0, 8) << ".wav";
                            finalFileName = fallbackName.str();
                            
                            embeddedMapping[essenceMobIdStr] = finalFileName;
                            g_embeddedFileNames[essenceMobIdStr] = finalFileName;
                            g_masterMobToFileName[masterMobIdStr] = finalFileName;
                            
                            out << "[FALLBACK] Generated fallback name: " << finalFileName << std::endl;
                        }
                    }
                    pMasterMob->Release();
                } else {
                    out << "[DEBUG] No MasterMob found for EssenceData " << essenceMobIdStr << std::endl;
                    
                    // FALLBACK: Даже без MasterMob создаем имя для embedded файла
                    std::ostringstream fallbackName;
                    fallbackName << "embedded_audio_" << essenceMobIdStr.substr(0, 8) << ".wav";
                    std::string finalFileName = fallbackName.str();
                    
                    embeddedMapping[essenceMobIdStr] = finalFileName;
                    g_embeddedFileNames[essenceMobIdStr] = finalFileName;
                    
                    out << "[FALLBACK] Generated fallback name without MasterMob: " << finalFileName << std::endl;
                }
            }
            pEssenceData->Release();
        }
        pEssenceEnum->Release();
    }
    
    out << "[*] Built " << embeddedMapping.size() << " robust embedded file name mappings" << std::endl;
    return embeddedMapping;
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
                    // **НОВАЯ РОБАСТНАЯ ЛОГИКА: Используем предоставленные функции**
                    std::string outputPath; // Объявление переменной для пути файла
                    
                    // Приоритет 1: Используем findMasterMobFromEssenceData для робастного поиска
                    IAAFMob* pMasterMob = findMasterMobFromEssenceData(pHeader, pEssenceData);
                    if (pMasterMob) {
                        aafMobID_t masterMobID;
                        if (SUCCEEDED(pMasterMob->GetMobID(&masterMobID))) {
                            std::string masterMobIdStr = formatMobID(masterMobID);
                            
                            // Ищем имя файла по MasterMob ID в глобальной карте
                            auto masterIt = g_masterMobToFileName.find(masterMobIdStr);
                            if (masterIt != g_masterMobToFileName.end() && !masterIt->second.empty()) {
                                std::string originalName = masterIt->second;
                                out << "[DEBUG] ROBUST MAPPING: Found master mob " << masterMobIdStr 
                                    << " for essence " << mobIdStr << " -> " << originalName << std::endl;
                                
                                // Определяем правильное расширение файла по метаданным AAF
                                std::string detectedExtension = detectFileExtension(pEssenceData, out);
                                
                                // Убираем старое расширение и добавляем правильное
                                size_t dotPos = originalName.find_last_of('.');
                                if (dotPos != std::string::npos) {
                                    originalName = originalName.substr(0, dotPos);
                                }
                                
                                outputPath = "extracted_media/" + originalName + detectedExtension;
                            }
                        }
                        pMasterMob->Release();
                    }
                    
                    // Fallback: Старая логика для случаев, где робастный поиск не сработал
                    if (outputPath.empty()) {
                        // Приоритет 2: Прямая проверка в карте g_masterMobToFileName 
                        auto directMasterIt = g_masterMobToFileName.find(mobIdStr);
                        if (directMasterIt != g_masterMobToFileName.end() && !directMasterIt->second.empty()) {
                            std::string originalName = directMasterIt->second;
                            out << "[DEBUG] Found direct master mob mapping for " << mobIdStr << " -> " << originalName << std::endl;
                            
                            // Определяем правильное расширение файла по метаданным AAF
                            std::string detectedExtension = detectFileExtension(pEssenceData, out);
                            
                            // Убираем старое расширение и добавляем правильное
                            size_t dotPos = originalName.find_last_of('.');
                            if (dotPos != std::string::npos) {
                                originalName = originalName.substr(0, dotPos);
                            }
                            
                            outputPath = "extracted_media/" + originalName + detectedExtension;
                        }
                    }
                    
                    // Приоритет 3: Глобальная карта embedded файлов (собранная во время анализа клипов)
                    if (outputPath.empty()) {
                        auto globalIt = g_embeddedFileNames.find(mobIdStr);
                        if (globalIt != g_embeddedFileNames.end() && !globalIt->second.empty()) {
                            std::string originalName = globalIt->second;
                            out << "[DEBUG] Found global mapping for " << mobIdStr << " -> " << originalName << std::endl;
                            
                            // Определяем правильное расширение файла по метаданным AAF
                            std::string detectedExtension = detectFileExtension(pEssenceData, out);
                            
                            // Убираем старое расширение и добавляем правильное
                            size_t dotPos = originalName.find_last_of('.');
                            if (dotPos != std::string::npos) {
                                originalName = originalName.substr(0, dotPos);
                            }
                            
                            outputPath = "extracted_media/" + originalName + detectedExtension;
                        }
                    } 
                    // Приоритет 4: Переданный маппинг embedded файлов
                    if (outputPath.empty()) {
                        auto embeddedIt = embeddedNameMapping.find(mobIdStr);
                        if (embeddedIt != embeddedNameMapping.end() && !embeddedIt->second.empty()) {
                            std::string originalName = embeddedIt->second;
                            out << "[DEBUG] Found embedded mapping for " << mobIdStr << " -> " << originalName << std::endl;
                            
                            // Определяем правильное расширение файла по метаданным AAF
                            std::string detectedExtension = detectFileExtension(pEssenceData, out);
                            
                            // Убираем старое расширение и добавляем правильное
                            size_t dotPos = originalName.find_last_of('.');
                            if (dotPos != std::string::npos) {
                                originalName = originalName.substr(0, dotPos);
                            }
                            
                            outputPath = "extracted_media/" + originalName + detectedExtension;
                        }
                    }
                    
                    // Приоритет 5: Fallback к стандартному маппингу mobIdToName
                    if (outputPath.empty()) {
                        auto generalIt = mobIdToName.find(mobIdStr);
                        if (generalIt != mobIdToName.end() && !generalIt->second.empty()) {
                            std::string mappedName = generalIt->second;
                            out << "[DEBUG] Found general mapping for " << mobIdStr << " -> " << mappedName << std::endl;
                            
                            // Определяем правильное расширение файла по метаданным AAF
                            std::string detectedExtension = detectFileExtension(pEssenceData, out);
                            
                            // Убираем старое расширение и добавляем правильное
                            size_t dotPos = mappedName.find_last_of('.');
                            if (dotPos != std::string::npos) {
                                mappedName = mappedName.substr(0, dotPos);
                            }
                            
                            outputPath = "extracted_media/" + mappedName + detectedExtension;
                        }
                    }
                    
                    // Приоритет 6: Последний fallback - генерируем имя на основе MobID
                    if (outputPath.empty()) {
                        std::string detectedExtension = detectFileExtension(pEssenceData, out);
                        outputPath = "extracted_media/embedded_audio_" + mobIdStr + detectedExtension;
                        out << "[DEBUG] Using final fallback name for " << mobIdStr << " -> " << outputPath << std::endl;
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

// Функция для определения расширения embedded файла по MobID из метаданных AAF
std::string getEmbeddedFileExtension(IAAFHeader* pHeader, const aafMobID_t& mobID, std::ofstream& out) {
    if (!pHeader) return ".aif";  // fallback
    
    std::string mobIdStr = formatMobID(mobID);
    out << "  [FORMAT] Getting extension for embedded MobID: " << mobIdStr << std::endl;
    
    // Ищем SourceMob (FileMob) с данным MobID
    aafSearchCrit_t searchCrit;
    searchCrit.searchTag = kAAFByMobKind;
    searchCrit.tags.mobKind = kAAFFileMob;
    
    IEnumAAFMobs* pSourceMobEnum = nullptr;
    if (SUCCEEDED(pHeader->GetMobs(&searchCrit, &pSourceMobEnum))) {
        IAAFMob* pSourceMob = nullptr;
        while (SUCCEEDED(pSourceMobEnum->NextOne(&pSourceMob))) {
            aafMobID_t sourceMobID;
            if (SUCCEEDED(pSourceMob->GetMobID(&sourceMobID))) {
                if (memcmp(&mobID, &sourceMobID, sizeof(aafMobID_t)) == 0) {
                    out << "  [FORMAT] Found matching SourceMob!" << std::endl;
                    
                    // Получаем EssenceDescriptor
                    IAAFSourceMob* pSrcMob = nullptr;
                    if (SUCCEEDED(pSourceMob->QueryInterface(IID_IAAFSourceMob, (void**)&pSrcMob))) {
                        IAAFEssenceDescriptor* pEssDesc = nullptr;
                        if (SUCCEEDED(pSrcMob->GetEssenceDescriptor(&pEssDesc))) {
                            std::string extension = determineFormatFromEssenceDescriptor(pEssDesc, out);
                            
                            pEssDesc->Release();
                            pSrcMob->Release();
                            pSourceMob->Release();
                            pSourceMobEnum->Release();
                            return extension;
                        }
                        pSrcMob->Release();
                    }
                }
            }
            pSourceMob->Release();
        }
        pSourceMobEnum->Release();
    }
    
    // Если не найдено, пытаемся найти через EssenceData
    IEnumAAFEssenceData* pEssenceEnum = nullptr;
    if (SUCCEEDED(pHeader->EnumEssenceData(&pEssenceEnum))) {
        IAAFEssenceData* pEssenceData = nullptr;
        while (SUCCEEDED(pEssenceEnum->NextOne(&pEssenceData))) {
            aafMobID_t essenceMobID;
            if (SUCCEEDED(pEssenceData->GetFileMobID(&essenceMobID))) {
                std::string essenceMobIdStr = formatMobID(essenceMobID);
                
                if (essenceMobIdStr == mobIdStr) {
                    out << "  [FORMAT] Found matching EssenceData, using header detection as fallback..." << std::endl;
                    std::string extension = detectFileExtension(pEssenceData, out);
                    pEssenceData->Release();
                    pEssenceEnum->Release();
                    return extension;
                }
            }
            pEssenceData->Release();
        }
        pEssenceEnum->Release();
    }
    
    // Если не найдено, возвращаем .aif по умолчанию для embedded файлов
    out << "  [FORMAT] EssenceDescriptor not found, using default .aif for embedded" << std::endl;
    return ".aif";
}

// Функция для определения формата из EssenceDescriptor
std::string determineFormatFromEssenceDescriptor(IAAFEssenceDescriptor* pEssDesc, std::ofstream& out) {
    if (!pEssDesc) {
        out << "  [FORMAT] No EssenceDescriptor, using default .aif" << std::endl;
        return ".aif";
    }
    
    // Пытаемся получить DataEssenceDescriptor для информации о кодировке
    IAAFDataEssenceDescriptor* pDataEssDesc = nullptr;
    if (SUCCEEDED(pEssDesc->QueryInterface(IID_IAAFDataEssenceDescriptor, (void**)&pDataEssDesc))) {
        aafUID_t codingUID;
        if (SUCCEEDED(pDataEssDesc->GetDataEssenceCoding(&codingUID))) {
            std::string extension = getExtensionFromCodingUID(codingUID, out);
            pDataEssDesc->Release();
            return extension;
        }
        pDataEssDesc->Release();
    }
    
    // Пытаемся получить SoundDescriptor для аудио информации
    IAAFSoundDescriptor* pSoundDesc = nullptr;
    if (SUCCEEDED(pEssDesc->QueryInterface(IID_IAAFSoundDescriptor, (void**)&pSoundDesc))) {
        // Получаем информацию о сжатии
        aafUID_t compressionUID;
        if (SUCCEEDED(pSoundDesc->GetCompression(&compressionUID))) {
            std::string extension = getExtensionFromCodingUID(compressionUID, out);
            pSoundDesc->Release();
            return extension;
        }
        
        // Получаем другую информацию для диагностики
        aafUInt32 channelCount = 0;
        aafRational_t sampleRate = {0, 1};
        aafUInt32 quantizationBits = 0;
        
        if (SUCCEEDED(pSoundDesc->GetChannelCount(&channelCount))) {
            out << "  [FORMAT] Channels: " << channelCount << std::endl;
        }
        if (SUCCEEDED(pSoundDesc->GetAudioSamplingRate(&sampleRate))) {
            out << "  [FORMAT] Sample Rate: " << sampleRate.numerator << "/" << sampleRate.denominator << std::endl;
        }
        if (SUCCEEDED(pSoundDesc->GetQuantizationBits(&quantizationBits))) {
            out << "  [FORMAT] Bit Depth: " << quantizationBits << std::endl;
        }
        
        pSoundDesc->Release();
    }
    
    // По умолчанию для embedded аудио возвращаем AIFF
    out << "  [FORMAT] No specific coding found, using default .aif for embedded audio" << std::endl;
    return ".aif";
}

// Функция для получения расширения из UID кодировки
std::string getExtensionFromCodingUID(const aafUID_t& codingUID, std::ofstream& out) {
    // Определяем известные кодировки AAF
    const aafUID_t kAAFCodecDef_WAVE = {0x820f09b1, 0xeb9b, 0x11d2, {0x80, 0x9f, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f}};
    const aafUID_t kAAFCodecDef_AIFC = {0x4b1c1a45, 0x03f2, 0x11d4, {0x80, 0xfb, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f}};
    const aafUID_t kAAFCodecDef_PCM = {0x90ac17c8, 0xe3e2, 0x4596, {0x9e, 0x9e, 0xa6, 0xdd, 0x1c, 0x70, 0xc8, 0x92}};
    
    // Сравниваем UID
    if (memcmp(&codingUID, &kAAFCodecDef_WAVE, sizeof(aafUID_t)) == 0) {
        out << "  [FORMAT] Detected WAVE codec -> .wav" << std::endl;
        return ".wav";
    }
    else if (memcmp(&codingUID, &kAAFCodecDef_AIFC, sizeof(aafUID_t)) == 0) {
        out << "  [FORMAT] Detected AIFC codec -> .aif" << std::endl;
        return ".aif";
    }
    else if (memcmp(&codingUID, &kAAFCodecDef_PCM, sizeof(aafUID_t)) == 0) {
        out << "  [FORMAT] Detected PCM codec -> .aif (default for uncompressed)" << std::endl;
        return ".aif";
    }
    else {
        // Выводим UID для отладки
        char uidStr[256];
        sprintf(uidStr, "  [FORMAT] Unknown codec UID: {%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x}",
            codingUID.Data1, codingUID.Data2, codingUID.Data3,
            codingUID.Data4[0], codingUID.Data4[1], codingUID.Data4[2], codingUID.Data4[3],
            codingUID.Data4[4], codingUID.Data4[5], codingUID.Data4[6], codingUID.Data4[7]);
        out << uidStr << " -> .aif (default)" << std::endl;
        return ".aif";
    }
}


