#include "aaf_proper_parser.h"
#include "aaf_utils.h"
#include <AAFTypeDefUIDs.h>
#include <AAFClassDefUIDs.h>
#include <AAFDataDefs.h>
#include <cstring>

AAFProperParser::AAFProperParser(IAAFHeader* pHeader, std::ofstream& outLog, LogLevel logLevel)
    : m_pHeader(pHeader), m_out(outLog), logger(outLog, logLevel) {
    
    // Инициализируем модульные компоненты
    clipProcessor = std::make_unique<AAFClipProcessor>(pHeader, logger);
    audioProperties = std::make_unique<AAFAudioProperties>(logger);
    essenceExtractor = std::make_unique<AAFEssenceExtractor>(pHeader, logger);
    
    // Настраиваем зависимости между модулями
    clipProcessor->setAudioProperties(audioProperties.get());
    clipProcessor->setEssenceExtractor(essenceExtractor.get());
}

AAFProperParser::~AAFProperParser() {
    // Уникальные указатели автоматически освободят ресурсы
}

bool AAFProperParser::parseAAFFile() {
    if (!m_pHeader) {
        logger.error(LogCategory::PARSER, "No header provided");
        return false;
    }
    
    logger.beginSection("AAF PROPER PARSER");
    
    // Шаг 1: Найти TopLevel CompositionMob
    IAAFMob* pTopLevelComp = findTopLevelCompositionMob();
    if (!pTopLevelComp) {
        logger.error(LogCategory::PARSER, "No TopLevel CompositionMob found");
        return false;
    }
    
    // Получаем имя проекта
    aafWChar projectName[256] = {0};
    if (SUCCEEDED(pTopLevelComp->GetName(projectName, sizeof(projectName)))) {
        std::string projectNameStr = wideToUtf8(projectName);
        logger.info(LogCategory::PARSER, "Project: " + projectNameStr);
    }
    
    // Шаг 2: Найти все аудио TimelineMobSlot
    std::vector<IAAFMobSlot*> audioSlots = findAudioTimelineSlots(pTopLevelComp);
    logger.info(LogCategory::TRACKS, "Found " + std::to_string(audioSlots.size()) + " audio timeline slots");
    
    // Обрабатываем каждый аудио слот
    for (size_t i = 0; i < audioSlots.size(); ++i) {
        IAAFMobSlot* pSlot = audioSlots[i];
        AAFAudioTrackInfo trackInfo;
        
        // Получаем основную информацию о треке
        pSlot->GetSlotID(&trackInfo.slotID);
        
        aafWChar slotName[256] = {0};
        if (SUCCEEDED(pSlot->GetName(slotName, sizeof(slotName)))) {
            trackInfo.trackName = wideToUtf8(slotName);
        }
        if (trackInfo.trackName.empty()) {
            trackInfo.trackName = "Audio Track " + std::to_string(i + 1);
        }
        
        // Получаем EditRate и Origin
        IAAFTimelineMobSlot* pTimelineSlot = nullptr;
        if (SUCCEEDED(pSlot->QueryInterface(IID_IAAFTimelineMobSlot, (void**)&pTimelineSlot))) {
            pTimelineSlot->GetEditRate(&trackInfo.editRate);
            pTimelineSlot->GetOrigin(&trackInfo.origin);
            pTimelineSlot->Release();
        } else {
            // Значения по умолчанию
            trackInfo.editRate = {25, 1};
            trackInfo.origin = 0;
        }
        
        logger.debug(LogCategory::TRACKS, "Processing track: " + trackInfo.trackName + 
                    " (SlotID: " + std::to_string(trackInfo.slotID) + ")");
        
        // Получаем сегмент слота и обрабатываем через модульный компонент
        IAAFSegment* pSegment = nullptr;
        if (SUCCEEDED(pSlot->GetSegment(&pSegment))) {
            clipProcessor->processSegmentForClips(pSegment, trackInfo, trackInfo.origin);
            pSegment->Release();
        }
        
        // Обновляем информацию о клипах с помощью модульных компонентов
        for (auto& clip : trackInfo.clips) {
            // Пытаемся найти FileSourceMob и получить аудио свойства
            aafMobID_t mobID;
            if (essenceExtractor->parseMobIDString(clip.mobID, mobID)) {
                IAAFMob* pMob = nullptr;
                if (SUCCEEDED(m_pHeader->LookupMob(mobID, &pMob))) {
                    IAAFMasterMob* pMasterMob = nullptr;
                    if (SUCCEEDED(pMob->QueryInterface(IID_IAAFMasterMob, (void**)&pMasterMob))) {
                        IAAFSourceMob* pFileSourceMob = clipProcessor->findFileSourceMobFromMaster(pMob);
                        if (pFileSourceMob) {
                            // Получаем аудио свойства
                            audioProperties->getAudioProperties(pFileSourceMob, clip);
                            
                            // Проверяем embedded статус
                            clip.isEmbedded = essenceExtractor->checkIfEmbedded(pFileSourceMob);
                            clip.sourceType = clip.isEmbedded ? "Embedded" : "External";
                            
                            // Получаем имя файла
                            if (clip.originalFileName.empty()) {
                                clip.originalFileName = essenceExtractor->getFileNameFromLocator(pFileSourceMob);
                                if (clip.originalFileName.empty()) {
                                    aafMobID_t fileSourceMobID;
                                    IAAFMob* pFileSourceMobBase = nullptr;
                                    if (SUCCEEDED(pFileSourceMob->QueryInterface(IID_IAAFMob, (void**)&pFileSourceMobBase))) {
                                        if (SUCCEEDED(pFileSourceMobBase->GetMobID(&fileSourceMobID))) {
                                            clip.originalFileName = essenceExtractor->getFileNameFallbackFromSourceMob(pFileSourceMob, fileSourceMobID);
                                        }
                                        pFileSourceMobBase->Release();
                                    }
                                }
                            }
                            
                            pFileSourceMob->Release();
                        }
                        pMasterMob->Release();
                    }
                    pMob->Release();
                }
            }
        }
        
        logger.info(LogCategory::TRACKS, "Track \"" + trackInfo.trackName + 
                   "\" contains " + std::to_string(trackInfo.clips.size()) + " clips");
        audioTracks.push_back(trackInfo);
        
        pSlot->Release();
    }
    
    pTopLevelComp->Release();
    
    // Подготавливаем сводку
    std::map<std::string, std::string> summary;
    summary["Tracks parsed"] = std::to_string(audioTracks.size());
    
    int totalClips = 0;
    for (const auto& track : audioTracks) {
        totalClips += static_cast<int>(track.clips.size());
    }
    summary["Total clips found"] = std::to_string(totalClips);
    
    logger.logSummary("AAF Parsing Summary", summary);
    logger.logResult(LogCategory::PARSER, "AAF parsing", true, 
                    std::to_string(audioTracks.size()) + " tracks parsed");
    logger.endSection("AAF PROPER PARSER");
    return true;
}

IAAFMob* AAFProperParser::findTopLevelCompositionMob() {
    logger.info(LogCategory::PARSER, "Searching for TopLevel CompositionMob...");
    
    // Ищем CompositionMob
    aafSearchCrit_t searchCrit;
    searchCrit.searchTag = kAAFByMobKind;
    searchCrit.tags.mobKind = kAAFCompMob;
    
    IEnumAAFMobs* pMobEnum = nullptr;
    if (FAILED(m_pHeader->GetMobs(&searchCrit, &pMobEnum))) {
        logger.error(LogCategory::PARSER, "Failed to get CompositionMobs");
        return nullptr;
    }
    
    IAAFMob* pMob = nullptr;
    IAAFMob* pTopLevelMob = nullptr;
    
    while (SUCCEEDED(pMobEnum->NextOne(&pMob))) {
        // Проверяем UsageCode
        if (isTopLevelComposition(pMob)) {
            logger.info(LogCategory::PARSER, "Found TopLevel CompositionMob");
            pTopLevelMob = pMob;
            pMob = nullptr; // Не освобождаем, так как возвращаем
            break;
        }
        pMob->Release();
    }
    
    pMobEnum->Release();
    
    // Если не нашли по UsageCode, берем первый CompositionMob
    if (!pTopLevelMob) {
        logger.warn(LogCategory::PARSER, "No UsageCode TopLevel found, using first CompositionMob");
        pMobEnum = nullptr;
        if (SUCCEEDED(m_pHeader->GetMobs(&searchCrit, &pMobEnum))) {
            if (SUCCEEDED(pMobEnum->NextOne(&pTopLevelMob))) {
                logger.info(LogCategory::PARSER, "Using first CompositionMob as TopLevel");
            }
            pMobEnum->Release();
        }
    }
    
    return pTopLevelMob;
}

bool AAFProperParser::isTopLevelComposition(IAAFMob* pMob) {
    if (!pMob) return false;
    
    // Упрощенная проверка - считаем первый CompositionMob как TopLevel
    IAAFCompositionMob* pCompMob = nullptr;
    if (SUCCEEDED(pMob->QueryInterface(IID_IAAFCompositionMob, (void**)&pCompMob))) {
        pCompMob->Release();
        return true; // Первый найденный CompositionMob считаем TopLevel
    }
    
    return false;
}

std::vector<IAAFMobSlot*> AAFProperParser::findAudioTimelineSlots(IAAFMob* pCompositionMob) {
    logger.info(LogCategory::TRACKS, "Searching for Audio TimelineMobSlots...");
    
    std::vector<IAAFMobSlot*> audioSlots;
    
    aafNumSlots_t numSlots = 0;
    if (FAILED(pCompositionMob->CountSlots(&numSlots))) {
        logger.error(LogCategory::TRACKS, "Failed to count slots in CompositionMob");
        return audioSlots;
    }
    
    for (aafUInt32 i = 0; i < numSlots; ++i) {
        IAAFMobSlot* pSlot = nullptr;
        if (FAILED(pCompositionMob->GetSlotAt(i, &pSlot))) {
            continue;
        }
        
        // Проверяем, что это TimelineMobSlot
        IAAFTimelineMobSlot* pTimelineSlot = nullptr;
        bool isTimeline = SUCCEEDED(pSlot->QueryInterface(IID_IAAFTimelineMobSlot, (void**)&pTimelineSlot));
        if (pTimelineSlot) {
            pTimelineSlot->Release();
        }
        
        if (!isTimeline) {
            logger.trace(LogCategory::TRACKS, "Slot " + std::to_string(i) + " is not TimelineMobSlot, skipping");
            pSlot->Release();
            continue;
        }
        
        // Проверяем DataDef - должен быть Sound
        IAAFSegment* pSegment = nullptr;
        if (SUCCEEDED(pSlot->GetSegment(&pSegment))) {
            IAAFComponent* pComponent = nullptr;
            if (SUCCEEDED(pSegment->QueryInterface(IID_IAAFComponent, (void**)&pComponent))) {
                IAAFDataDef* pDataDef = nullptr;
                if (SUCCEEDED(pComponent->GetDataDef(&pDataDef))) {
                    if (audioProperties->isAudioDataDef(pDataDef)) {
                        logger.debug(LogCategory::TRACKS, "Found audio TimelineMobSlot " + std::to_string(i));
                        audioSlots.push_back(pSlot);
                        pSlot = nullptr; // Не освобождаем, так как добавили в вектор
                    } else {
                        logger.trace(LogCategory::TRACKS, "Slot " + std::to_string(i) + " is not audio, skipping");
                    }
                    pDataDef->Release();
                }
                pComponent->Release();
            }
            pSegment->Release();
        }
        
        if (pSlot) {
            pSlot->Release();
        }
    }
    
    logger.info(LogCategory::TRACKS, "Found " + std::to_string(audioSlots.size()) + " audio timeline slots");
    return audioSlots;
}

bool AAFProperParser::extractAllEmbeddedAudio(const std::string& outputDir) {
    // Делегируем в AAFEssenceExtractor
    return essenceExtractor->extractAllEmbeddedAudio(audioTracks, outputDir);
}
