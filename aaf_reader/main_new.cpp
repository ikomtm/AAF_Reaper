#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING

#include <AAF.h>
#include <AAFResult.h>
#include <AAFTypes.h>
#include <AAFTypeDefUIDs.h>
#include <AAFDataDefs.h>
#include <AAFFileKinds.h>

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <memory>
#include <iomanip>

// Наши модули
#include "data_structures.h"
#include "aaf_utils.h"
#include "aaf_parser.h"
#include "media_utils.h"
#include "csv_export.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "ERROR: Usage: aaf_reader <file.aaf>" << std::endl;
        return 1;
    }

    ProjectData projectData;
    
    // Переменные для статистики
    int audioTrackCount = 0;
    int audioClipCount = 0;
    int audioFadeCount = 0;
    int audioEffectCount = 0;

    // Создаем папки для медиафайлов
    createExtractedMediaFolder();

    std::ofstream out("output.txt");
    out << "[*] Opening AAF file..." << std::endl;
    out << "[*] Created extracted_media and audio_files folders" << std::endl;

    // Конвертируем путь в wide string  
    size_t len = strlen(argv[1]) + 1;
    std::wstring widePath(len, L'\0');
    size_t convertedChars = 0;
    mbstowcs_s(&convertedChars, &widePath[0], len, argv[1], _TRUNCATE);

    // Открываем AAF файл
    IAAFFile* pFile = nullptr;
    if (FAILED(AAFFileOpenExistingRead(widePath.c_str(), 0, &pFile)) || !pFile) {
        out << "ERROR: Failed to open file." << std::endl;
        return 1;
    }

    IAAFHeader* pHeader = nullptr;
    if (FAILED(pFile->GetHeader(&pHeader))) {
        out << "ERROR: Failed to get header." << std::endl;
        pFile->Close(); 
        pFile->Release();
        return 1;
    }

    std::map<std::string, std::string> mobIdToName;

    // 1. Индексируем все мобы для ссылок
    {
        IEnumAAFMobs* pIter = nullptr;
        if (SUCCEEDED(pHeader->GetMobs(nullptr, &pIter))) {
            IAAFMob* pMob = nullptr;
            while (SUCCEEDED(pIter->NextOne(&pMob))) {
                aafMobID_t mobID;
                if (FAILED(pMob->GetMobID(&mobID))) { 
                    pMob->Release(); 
                    continue; 
                }
                
                aafWChar name[256] = {0};
                pMob->GetName(name, sizeof(name)); // Игнорируем ошибки
                
                std::string mobIdStr = formatMobID(mobID);
                std::string nameStr = wideToUtf8(name);
                
                if (nameStr.empty()) {
                    IAAFCompositionMob* pComp = nullptr;
                    IAAFMasterMob* pMaster = nullptr;
                    IAAFSourceMob* pSource = nullptr;
                    
                    if (SUCCEEDED(pMob->QueryInterface(IID_IAAFCompositionMob, (void**)&pComp))) {
                        nameStr = "[CompositionMob]";
                        pComp->Release();
                    } else if (SUCCEEDED(pMob->QueryInterface(IID_IAAFMasterMob, (void**)&pMaster))) {
                        nameStr = "[MasterMob]";
                        pMaster->Release();
                    } else if (SUCCEEDED(pMob->QueryInterface(IID_IAAFSourceMob, (void**)&pSource))) {
                        // Для SourceMob пытаемся извлечь имя файла из EssenceDescriptor
                        IAAFEssenceDescriptor* pEssDesc = nullptr;
                        if (SUCCEEDED(pSource->GetEssenceDescriptor(&pEssDesc))) {
                            // Пытаемся получить имя файла через Locators
                            aafUInt32 numLocators = 0;
                            if (SUCCEEDED(pEssDesc->CountLocators(&numLocators)) && numLocators > 0) {
                                out << "[DEBUG] Found " << numLocators << " locators for MobID " << mobIdStr << std::endl;
                                IEnumAAFLocators* pLocatorEnum = nullptr;
                                if (SUCCEEDED(pEssDesc->GetLocators(&pLocatorEnum))) {
                                    IAAFLocator* pLocator = nullptr;
                                    if (SUCCEEDED(pLocatorEnum->NextOne(&pLocator))) {
                                        // Получаем размер буфера для пути
                                        aafUInt32 pathBufSize = 0;
                                        if (SUCCEEDED(pLocator->GetPathBufLen(&pathBufSize)) && pathBufSize > 0) {
                                            std::vector<aafCharacter> pathBuffer(pathBufSize / sizeof(aafCharacter));
                                            if (SUCCEEDED(pLocator->GetPath(pathBuffer.data(), pathBufSize))) {
                                                std::string path = wideToUtf8(pathBuffer.data());
                                                out << "[DEBUG] Got path from Locator: " << path << std::endl;
                                                // Извлекаем имя файла из пути
                                                size_t lastSlash = path.find_last_of("/\\");
                                                if (lastSlash != std::string::npos) {
                                                    nameStr = path.substr(lastSlash + 1);
                                                } else {
                                                    nameStr = path;
                                                }
                                                out << "[DEBUG] Extracted filename: " << nameStr << std::endl;
                                            }
                                        }
                                        pLocator->Release();
                                    }
                                    pLocatorEnum->Release();
                                }
                            } else {
                                out << "[DEBUG] No locators found for MobID " << mobIdStr << std::endl;
                            }
                            
                            // Если не нашли через Locator, пытаемся получить через другие методы
                            if (nameStr.empty()) {
                                // Для embedded файлов генерируем имя на основе MobID
                                nameStr = "embedded_audio_" + mobIdStr + ".wav";
                            }
                            pEssDesc->Release();
                        }
                        
                        if (nameStr.empty()) {
                            nameStr = "[SourceMob]";
                        }
                        pSource->Release();
                    }
                }
                
                mobIdToName[mobIdStr] = nameStr;
                pMob->Release();
            }
            pIter->Release();
        }
    }

    out << "[*] Indexed " << mobIdToName.size() << " MobIDs" << std::endl;

    // 2. Строим карту embedded файлов
    std::set<std::string> embeddedMobIDs = buildEmbeddedFilesMap(pHeader, out);
    
    out << "[DEBUG] About to build embedded file name mapping..." << std::endl;
    // 3. Строим маппинг embedded файлов с правильными именами
    std::map<std::string, std::string> embeddedNameMapping = buildEmbeddedFileNameMapping(pHeader, mobIdToName, out);
    out << "[DEBUG] Built embedded name mapping with " << embeddedNameMapping.size() << " entries" << std::endl;
    
    // 3.5. Очищаем глобальную карту для сбора имён embedded файлов во время анализа клипов
    g_embeddedFileNames.clear();
    out << "[DEBUG] Cleared global embedded file names map for clip analysis" << std::endl;
    
    // 4. Обрабатываем композиции сначала для сбора информации о embedded файлах
    aafSearchCrit_t searchCrit;
    searchCrit.searchTag = kAAFByMobKind;
    searchCrit.tags.mobKind = kAAFCompMob;

    IEnumAAFMobs* pCompMobIter = nullptr;
    if (SUCCEEDED(pHeader->GetMobs(&searchCrit, &pCompMobIter))) {
        IAAFMob* pMob = nullptr;
        int compMobCount = 0;
        
        while (SUCCEEDED(pCompMobIter->NextOne(&pMob))) {
            aafWChar name[256] = {0};
            pMob->GetName(name, sizeof(name));
            projectData.projectName = wideToUtf8(name);
            
            if (projectData.projectName.empty()) {
                projectData.projectName = "AAF_Import_Project";
            }
            
            // Получаем начальный timecode сессии
            aafPosition_t sessionStartTC = getCompositionStartTimecode(pMob);
            projectData.sessionStartTimecode = (double)sessionStartTC / 25.0; // предполагаем 25fps
            
            out << "\n=== COMPOSITION #" << (++compMobCount) << ": " << projectData.projectName << " ===" << std::endl;
            out << "Session Start Timecode: " << formatTimecode(sessionStartTC, {25, 1}) << std::endl;
            
            // Обрабатываем слоты композиции
            aafNumSlots_t numSlots = 0;
            if (SUCCEEDED(pMob->CountSlots(&numSlots))) {
                for (aafUInt32 i = 0; i < numSlots; ++i) {
                    IAAFMobSlot* pSlot = nullptr;
                    if (SUCCEEDED(pMob->GetSlotAt(i, &pSlot))) {
                    
                        // Для текстового отчета
                        processCompositionSlot(pSlot, out, i, mobIdToName, 
                                             audioTrackCount, audioClipCount, audioFadeCount, audioEffectCount, sessionStartTC);
                        
                        // Для CSV экспорта
                        if (isAudioTrack(pSlot)) {
                            AudioTrackData trackData;
                            trackData.trackIndex = i;
                        
                            aafWChar slotName[256] = {0};
                            pSlot->GetName(slotName, sizeof(slotName));
                            trackData.trackName = wideToUtf8(slotName);
                            if (trackData.trackName.empty()) {
                                trackData.trackName = "Audio Track " + std::to_string(i);
                            }
                            trackData.trackType = "Audio";
                        
                            processAudioTrackForExportWithHeader(pSlot, mobIdToName, sessionStartTC, trackData, pHeader, embeddedMobIDs, out);
                        
                            // Экспортируем ВСЕ аудио треки, даже пустые
                            projectData.tracks.push_back(trackData);
                        }
                    
                        pSlot->Release();
                    }
                }
            }
            pMob->Release();
            break; // Обрабатываем только первую композицию
        }
        pCompMobIter->Release();
    }
    
    out << "[DEBUG] After clip analysis, global embedded file names map has " << g_embeddedFileNames.size() << " entries" << std::endl;
    
    // 5. Теперь извлекаем все embedded файлы с правильными именами
    std::map<std::string, std::string> embeddedFileMap = extractAllEmbeddedFiles(pHeader, mobIdToName, embeddedNameMapping, out);
    
    // 6. Обновляем mobIdToName картой извлеченных embedded файлов
    for (const auto& embeddedFile : embeddedFileMap) {
        const std::string& mobId = embeddedFile.first;
        const std::string& extractedPath = embeddedFile.second;
        
        // Переопределяем путь для embedded файлов
        mobIdToName[mobId] = extractedPath;
        out << "[EMBEDDED_MAP] " << mobId << " -> " << extractedPath << std::endl;
    }
    
    out << "[*] Updated mobIdToName with " << embeddedFileMap.size() << " embedded file paths" << std::endl;

    // Вычисляем общую длительность проекта
    for (const auto& track : projectData.tracks) {
        for (const auto& clip : track.clips) {
            if (clip.timelineEnd > projectData.totalLength) {
                projectData.totalLength = clip.timelineEnd;
            }
        }
    }
    
    // Экспортируем в CSV
    exportToCSV(projectData, "aaf_export.csv");
    
    // Отладка EssenceData перед обработкой файлов
    debugEssenceData(pHeader, out);
    
    // Обрабатываем аудиофайлы (копируем/извлекаем)
    processAudioFiles(projectData, out);
    
    // Статистика
    out << "\n📊 === AUDIO TRACKS SUMMARY ===" << std::endl;
    out << "Total audio tracks processed: " << audioTrackCount << std::endl;
    out << "Total audio clips: " << audioClipCount << std::endl;  // Это OperationGroup (клипы)
    out << "Total audio fades/crossfades: " << audioFadeCount << std::endl;
    out << "Total audio effects: " << audioEffectCount << std::endl;
    
    out << "\n✅ CSV Export: aaf_export.csv" << std::endl;
    out << "Total tracks exported: " << projectData.tracks.size() << std::endl;
    int totalClipsExported = 0;
    for (const auto& track : projectData.tracks) {
        totalClipsExported += track.clips.size();
    }
    out << "Total clips exported: " << totalClipsExported << std::endl;
    out << "Project length: " << std::fixed << std::setprecision(3) << projectData.totalLength << "s" << std::endl;

    // Освобождаем ресурсы
    pHeader->Release();
    pFile->Close();
    pFile->Release();
    out.close();

    // Успешное завершение
    std::cout << "✅ AAF processed successfully!" << std::endl;
    std::cout << "📊 CSV exported: aaf_export.csv" << std::endl;
    std::cout << "📋 Report: output.txt" << std::endl;

    return 0;
}
