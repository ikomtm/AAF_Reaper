#include "aaf_clip_processor.h"
#include "aaf_fade_detector.h"
#include "aaf_utils.h"
#include <AAFTypeDefUIDs.h>
#include <AAFClassDefUIDs.h>
#include <AAFDataDefs.h>

AAFClipProcessor::AAFClipProcessor(IAAFHeader* pHeader, DebugLogger& logger)
    : m_pHeader(pHeader), logger(logger), audioProperties(nullptr), essenceExtractor(nullptr) {
    fadeDetector = std::make_unique<AAFFadeDetector>(logger);
}

AAFClipProcessor::~AAFClipProcessor() {
}

void AAFClipProcessor::processSegmentForClips(IAAFSegment* pSegment, 
                                            AAFAudioTrackInfo& trackInfo, 
                                            aafPosition_t currentPosition) {
    if (!pSegment) return;
    
    // Проверяем тип сегмента
    IAAFSequence* pSequence = nullptr;
    IAAFSourceClip* pSourceClip = nullptr;
    IAAFOperationGroup* pOpGroup = nullptr;
    
    if (SUCCEEDED(pSegment->QueryInterface(IID_IAAFSequence, (void**)&pSequence))) {
        // Последовательность компонентов
        logger.debug(LogCategory::CLIPS, "Processing Sequence");
        
        IEnumAAFComponents* pEnum = nullptr;
        if (SUCCEEDED(pSequence->GetComponents(&pEnum))) {
            IAAFComponent* pComponent = nullptr;
            aafPosition_t seqPosition = currentPosition;
            
            while (SUCCEEDED(pEnum->NextOne(&pComponent))) {
                aafLength_t componentLength = 0;
                pComponent->GetLength(&componentLength);
                
                // Рекурсивно обрабатываем компонент
                IAAFSegment* pComponentSegment = nullptr;
                if (SUCCEEDED(pComponent->QueryInterface(IID_IAAFSegment, (void**)&pComponentSegment))) {
                    processSegmentForClips(pComponentSegment, trackInfo, seqPosition);
                    pComponentSegment->Release();
                }
                
                seqPosition += componentLength;
                pComponent->Release();
            }
            pEnum->Release();
        }
        pSequence->Release();
        
    } else if (SUCCEEDED(pSegment->QueryInterface(IID_IAAFSourceClip, (void**)&pSourceClip))) {
        // Прямой SourceClip
        logger.debug(LogCategory::CLIPS, "Processing SourceClip");
        
        // Шаг 3: Обрабатываем SourceClip -> MasterMob цепочку
        AAFAudioClipInfo clipInfo = processSourceClipChain(pSourceClip, trackInfo.editRate, currentPosition);
        if (!clipInfo.mobID.empty()) {
            if (fadeDetector) {
                fadeDetector->analyzeSegmentForFade(pSegment, clipInfo);
            }
            trackInfo.clips.push_back(clipInfo);
        }
        
        pSourceClip->Release();
        
    } else if (SUCCEEDED(pSegment->QueryInterface(IID_IAAFOperationGroup, (void**)&pOpGroup))) {
        // OperationGroup (эффекты, фейды)
        logger.debug(LogCategory::CLIPS, "Processing OperationGroup (effects)");
        
        // Обрабатываем входные сегменты
        aafUInt32 numInputs = 0;
        if (SUCCEEDED(pOpGroup->CountSourceSegments(&numInputs))) {
            for (aafUInt32 i = 0; i < numInputs; ++i) {
                IAAFSegment* pInputSegment = nullptr;
                if (SUCCEEDED(pOpGroup->GetInputSegmentAt(i, &pInputSegment))) {
                    processSegmentForClips(pInputSegment, trackInfo, currentPosition);
                    pInputSegment->Release();
                }
            }
        }
        
        pOpGroup->Release();
    } else {
        logger.trace(LogCategory::CLIPS, "Unknown segment type, skipping");
    }
}

AAFAudioClipInfo AAFClipProcessor::processSourceClipChain(IAAFSourceClip* pSourceClip, 
                                                        const aafRational_t& editRate,
                                                        aafPosition_t timelinePosition) {
    AAFAudioClipInfo clipInfo = {};
    
    // Инициализируем поля
    clipInfo.originalFileName = "";
    clipInfo.extractedFilePath = "";
    clipInfo.mobID = "";
    clipInfo.timelineStart = 0;
    clipInfo.timelineEnd = 0;
    clipInfo.sourceStart = 0;
    clipInfo.duration = 0;
    clipInfo.sampleRate = {48000, 1};
    clipInfo.channelCount = 2;
    clipInfo.bitsPerSample = 16;
    clipInfo.compressionType = "PCM";
    clipInfo.sourceType = "Unknown";
    clipInfo.isEmbedded = false;
    clipInfo.hasFadeIn = false;
    clipInfo.hasFadeOut = false;
    clipInfo.fadeInLength = 0;
    clipInfo.fadeOutLength = 0;
    
    if (!pSourceClip) return clipInfo;
    
    logger.debug(LogCategory::CLIPS, "Processing SourceClip chain...");
    
    // Получаем SourceReference
    aafSourceRef_t sourceRef;
    if (FAILED(pSourceClip->GetSourceReference(&sourceRef))) {
        logger.error(LogCategory::CLIPS, "Failed to get SourceReference");
        return clipInfo;
    }
    
    clipInfo.mobID = formatMobID(sourceRef.sourceID);
    clipInfo.sourceStart = sourceRef.startTime;
    clipInfo.timelineStart = timelinePosition;
    
    // Получаем длительность
    IAAFComponent* pComponent = nullptr;
    if (SUCCEEDED(pSourceClip->QueryInterface(IID_IAAFComponent, (void**)&pComponent))) {
        pComponent->GetLength(&clipInfo.duration);
        clipInfo.timelineEnd = timelinePosition + clipInfo.duration;
        pComponent->Release();
    }
    
    logger.debug(LogCategory::CLIPS, "SourceClip references MobID: " + clipInfo.mobID);
    
    // Ищем MasterMob
    IAAFMob* pMasterMob = nullptr;
    if (SUCCEEDED(m_pHeader->LookupMob(sourceRef.sourceID, &pMasterMob))) {
        logger.debug(LogCategory::CLIPS, "Found MasterMob");
        
        // Шаг 4: Найти FileSourceMob из MasterMob
        IAAFSourceMob* pFileSourceMob = findFileSourceMobFromMaster(pMasterMob);
        if (pFileSourceMob) {
            logger.debug(LogCategory::CLIPS, "Found FileSourceMob");
            
            // Получить имя файла из различных источников - временно используем простое извлечение
            IAAFMob* pMob = nullptr;
            if (SUCCEEDED(pFileSourceMob->QueryInterface(IID_IAAFMob, (void**)&pMob))) {
                aafWChar name[256] = {0};
                if (SUCCEEDED(pMob->GetName(name, sizeof(name)))) {
                    clipInfo.originalFileName = wideToUtf8(name);
                }
                pMob->Release();
            }
            
            // Аудио свойства и embedded статус будут установлены в основном парсере
            clipInfo.sourceType = "Unknown";
            clipInfo.isEmbedded = false;
            
            pFileSourceMob->Release();
        } else {
            // Если FileSourceMob не найден, попробуем получить имя из MasterMob
            aafWChar masterMobName[256] = {0};
            if (SUCCEEDED(pMasterMob->GetName(masterMobName, sizeof(masterMobName)))) {
                clipInfo.originalFileName = wideToUtf8(masterMobName);
            }
            
            // Если нет FileSourceMob, это может быть embedded
            clipInfo.isEmbedded = true;
            clipInfo.sourceType = "Embedded";
        }
        
        pMasterMob->Release();
    } else {
        logger.warn(LogCategory::CLIPS, "MasterMob not found for " + clipInfo.mobID);
    }
    
    return clipInfo;
}

IAAFSourceMob* AAFClipProcessor::findFileSourceMobFromMaster(IAAFMob* pMasterMob) {
    if (!pMasterMob) return nullptr;
    
    logger.debug(LogCategory::ESSENCE, "Searching for FileSourceMob from MasterMob...");
    
    // Получаем слоты MasterMob
    aafNumSlots_t numSlots = 0;
    if (FAILED(pMasterMob->CountSlots(&numSlots))) {
        return nullptr;
    }
    
    for (aafUInt32 i = 0; i < numSlots; ++i) {
        IAAFMobSlot* pSlot = nullptr;
        if (FAILED(pMasterMob->GetSlotAt(i, &pSlot))) {
            continue;
        }
        
        IAAFSegment* pSegment = nullptr;
        if (SUCCEEDED(pSlot->GetSegment(&pSegment))) {
            IAAFSourceClip* pSourceClip = nullptr;
            if (SUCCEEDED(pSegment->QueryInterface(IID_IAAFSourceClip, (void**)&pSourceClip))) {
                aafSourceRef_t sourceRef;
                if (SUCCEEDED(pSourceClip->GetSourceReference(&sourceRef))) {
                    // Ищем SourceMob (FileSourceMob)
                    IAAFMob* pSourceMob = nullptr;
                    if (SUCCEEDED(m_pHeader->LookupMob(sourceRef.sourceID, &pSourceMob))) {
                        IAAFSourceMob* pFileSourceMob = nullptr;
                        if (SUCCEEDED(pSourceMob->QueryInterface(IID_IAAFSourceMob, (void**)&pFileSourceMob))) {
                            logger.debug(LogCategory::ESSENCE, "Found FileSourceMob");
                            pSourceMob->Release();
                            pSourceClip->Release();
                            pSegment->Release();
                            pSlot->Release();
                            return pFileSourceMob;
                        }
                        pSourceMob->Release();
                    }
                }
                pSourceClip->Release();
            }
            pSegment->Release();
        }
        pSlot->Release();
    }
    
    return nullptr;
}
