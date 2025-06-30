#include "aaf_clip_processor.h"
#include "aaf_audio_properties.h"
#include "aaf_utils.h"
#include <AAFTypeDefUIDs.h>
#include <AAFClassDefUIDs.h>
#include <AAFDataDefs.h>
#include <AAFPropertyDefs.h>
#include <AAFOperationDefs.h>
#include "debug_logger.h"
#include <vector>
#include <cstdio>
#include <cmath>
#include <fstream>
#include <algorithm>
#include <cstring>

// Глобальные файлы для логирования (открываются при первом вызове)
static std::ofstream logClips("log_clips.txt", std::ios::app);
static std::ofstream logFades("log_fades.txt", std::ios::app);
static std::ofstream logTransitions("log_transitions.txt", std::ios::app);
static std::ofstream logErrors("log_errors.txt", std::ios::app);

AAFClipProcessor::AAFClipProcessor(IAAFHeader* pHeader, DebugLogger& logger)
    : m_pHeader(pHeader), logger(logger), audioProperties(nullptr), essenceExtractor(nullptr) {
}

AAFClipProcessor::~AAFClipProcessor() {
}

void AAFClipProcessor::processSegmentForClips(IAAFSegment* pSegment, 
                                            AAFAudioTrackInfo& trackInfo, 
                                            aafPosition_t currentPosition) {
    logTransitions << "DEBUG: processSegmentForClips called, currentPosition=" << currentPosition << std::endl;
    if (!pSegment) {
        logTransitions << "DEBUG: pSegment is null" << std::endl;
        return;
    }
    // Проверяем тип сегмента
    IAAFSequence* pSequence = nullptr;
    IAAFSourceClip* pSourceClip = nullptr;
    IAAFOperationGroup* pOpGroup = nullptr;
    IAAFFiller* pFiller = nullptr;
    IAAFVaryingValue* pVaryingValue = nullptr;
    if (SUCCEEDED(pSegment->QueryInterface(IID_IAAFSequence, (void**)&pSequence))) {
        logTransitions << "DEBUG: pSegment is IAAFSequence" << std::endl;
        IEnumAAFComponents* pEnum = nullptr;
        if (SUCCEEDED(pSequence->GetComponents(&pEnum))) {
            int compCount = 0;
            IAAFComponent* pComponent = nullptr;
            aafPosition_t seqPosition = currentPosition;
            while (SUCCEEDED(pEnum->NextOne(&pComponent))) {
                ++compCount;
                aafLength_t componentLength = 0;
                pComponent->GetLength(&componentLength);
                // Проверяем тип компонента
                IAAFSegment* pComponentSegment = nullptr;
                IAAFTransition* pComponentTransition = nullptr;
                if (SUCCEEDED(pComponent->QueryInterface(IID_IAAFTransition, (void**)&pComponentTransition))) {
                    logTransitions << "DEBUG: Found IAAFTransition in Sequence at position " << seqPosition << std::endl;
                    processTransitionForCrossfade(pComponentTransition, trackInfo, seqPosition);
                    pComponentTransition->Release();
                } else if (SUCCEEDED(pComponent->QueryInterface(IID_IAAFSegment, (void**)&pComponentSegment))) {
                    logTransitions << "DEBUG: Found IAAFSegment in Sequence, recursing" << std::endl;
                    processSegmentForClips(pComponentSegment, trackInfo, seqPosition);
                    pComponentSegment->Release();
                } else {
                    logTransitions << "DEBUG: Unknown component type in Sequence" << std::endl;
                }
                seqPosition += componentLength;
                pComponent->Release();
            }
            logTransitions << "DEBUG: Total components in Sequence: " << compCount << std::endl;
            pEnum->Release();
        } else {
            logTransitions << "DEBUG: GetComponents failed for Sequence" << std::endl;
        }
        pSequence->Release();
    } else if (SUCCEEDED(pSegment->QueryInterface(IID_IAAFSourceClip, (void**)&pSourceClip))) {
        logTransitions << "DEBUG: pSegment is IAAFSourceClip" << std::endl;
        // Прямой SourceClip
        logger.debug(LogCategory::CLIPS, "Processing SourceClip");
        // (GetParameters не поддерживается для SourceClip, только для OperationGroup)
        // Шаг 3: Обрабатываем SourceClip -> MasterMob цепочку
        AAFAudioClipInfo clipInfo = processSourceClipChain(pSourceClip, trackInfo.editRate, currentPosition);
        if (!clipInfo.mobID.empty()) {
            trackInfo.clips.push_back(clipInfo);
        }
        pSourceClip->Release();
    } else if (SUCCEEDED(pSegment->QueryInterface(IID_IAAFOperationGroup, (void**)&pOpGroup))) {
        logTransitions << "DEBUG: pSegment is IAAFOperationGroup" << std::endl;
        // OperationGroup (эффекты, фейды)
        logger.debug(LogCategory::CLIPS, "Processing OperationGroup (effects)");
        // Логируем параметры OperationGroup
        IEnumAAFParameters* pEnumParams = nullptr;
        if (SUCCEEDED(pOpGroup->GetParameters(&pEnumParams))) {
            logTransitions << "DEBUG: OperationGroup has parameters:" << std::endl;
            IAAFParameter* pParam = nullptr;
            int paramCount = 0;
            while (SUCCEEDED(pEnumParams->NextOne(&pParam))) {
                ++paramCount;
                IAAFParameterDef* pParamDef = nullptr;
                if (SUCCEEDED(pParam->GetParameterDefinition(&pParamDef))) {
                    IAAFDefObject* pParamDefObj = nullptr;
                    std::string paramNameUtf8 = "";
                    aafUID_t paramID = {};
                    if (SUCCEEDED(pParamDef->QueryInterface(IID_IAAFDefObject, (void**)&pParamDefObj))) {
                        aafUInt32 nameSize = 0;
                        if (SUCCEEDED(pParamDefObj->GetNameBufLen(&nameSize)) && nameSize > 0) {
                            std::vector<aafCharacter> nameBuffer(nameSize);
                            if (SUCCEEDED(pParamDefObj->GetName(nameBuffer.data(), nameSize))) {
                                paramNameUtf8 = wideToUtf8(nameBuffer.data());
                            }
                        }
                        pParamDefObj->GetAUID(&paramID);
                        pParamDefObj->Release();
                    }
                    // Попробуем получить значение параметра (если это ConstantValue)
                    IAAFConstantValue* pConstVal = nullptr;
                    if (SUCCEEDED(pParam->QueryInterface(IID_IAAFConstantValue, (void**)&pConstVal))) {
                        aafUInt32 bytesRead = 0;
                        aafUInt8 valueBuf[32] = {0};
                        if (SUCCEEDED(pConstVal->GetValue(sizeof(valueBuf), valueBuf, &bytesRead))) {
                            int intVal = 0;
                            std::memcpy(&intVal, valueBuf, std::min<size_t>(bytesRead, sizeof(int)));
                            logTransitions << "\tOperationGroup Param=" << paramNameUtf8 << "\tValue=" << intVal << std::endl;
                        }
                        pConstVal->Release();
                    } else {
                        logTransitions << "\tOperationGroup Param=" << paramNameUtf8 << "\tValue=ComplexType" << std::endl;
                    }
                    pParamDef->Release();
                } else {
                    logTransitions << "DEBUG: GetParameterDefinition failed for OperationGroup parameter" << std::endl;
                }
                pParam->Release();
            }
            logTransitions << "DEBUG: Total OperationGroup parameters found: " << paramCount << std::endl;
            pEnumParams->Release();
        }
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
    } else if (SUCCEEDED(pSegment->QueryInterface(IID_IAAFFiller, (void**)&pFiller))) {
        logTransitions << "DEBUG: pSegment is IAAFFiller" << std::endl;
        // Логируем Filler
        IAAFComponent* pComp = nullptr;
        aafLength_t length = 0;
        if (SUCCEEDED(pFiller->QueryInterface(IID_IAAFComponent, (void**)&pComp))) {
            pComp->GetLength(&length);
            pComp->Release();
        }
        logTransitions << "Filler\tLength=" << length << "\tPosition=" << currentPosition << std::endl;
        // Проверим OperationGroup внутри Filler (часто используется для fade)
        IAAFOperationGroup* pFillerOpGroup = nullptr;
        if (SUCCEEDED(pFiller->QueryInterface(IID_IAAFOperationGroup, (void**)&pFillerOpGroup))) {
            logTransitions << "DEBUG: Filler contains OperationGroup" << std::endl;
            // Логика аналогична обработке OpGroup выше
            IEnumAAFParameters* pEnumParams = nullptr;
            if (SUCCEEDED(pFillerOpGroup->GetParameters(&pEnumParams))) {
                logTransitions << "DEBUG: Filler OperationGroup has parameters:" << std::endl;
                IAAFParameter* pParam = nullptr;
                int paramCount = 0;
                while (SUCCEEDED(pEnumParams->NextOne(&pParam))) {
                    ++paramCount;
                    IAAFParameterDef* pParamDef = nullptr;
                    if (SUCCEEDED(pParam->GetParameterDefinition(&pParamDef))) {
                        IAAFDefObject* pParamDefObj = nullptr;
                        std::string paramNameUtf8 = "";
                        aafUID_t paramID = {};
                        if (SUCCEEDED(pParamDef->QueryInterface(IID_IAAFDefObject, (void**)&pParamDefObj))) {
                            aafUInt32 nameSize = 0;
                            if (SUCCEEDED(pParamDefObj->GetNameBufLen(&nameSize)) && nameSize > 0) {
                                std::vector<aafCharacter> nameBuffer(nameSize);
                                if (SUCCEEDED(pParamDefObj->GetName(nameBuffer.data(), nameSize))) {
                                    paramNameUtf8 = wideToUtf8(nameBuffer.data());
                                }
                            }
                            pParamDefObj->GetAUID(&paramID);
                            pParamDefObj->Release();
                        }
                        IAAFConstantValue* pConstVal = nullptr;
                        if (SUCCEEDED(pParam->QueryInterface(IID_IAAFConstantValue, (void**)&pConstVal))) {
                            aafUInt32 bytesRead = 0;
                            aafUInt8 valueBuf[32] = {0};
                            if (SUCCEEDED(pConstVal->GetValue(sizeof(valueBuf), valueBuf, &bytesRead))) {
                                int intVal = 0;
                                std::memcpy(&intVal, valueBuf, std::min<size_t>(bytesRead, sizeof(int)));
                                logTransitions << "\tFiller OperationGroup Param=" << paramNameUtf8 << "\tValue=" << intVal << std::endl;
                            }
                            pConstVal->Release();
                        } else {
                            logTransitions << "\tFiller OperationGroup Param=" << paramNameUtf8 << "\tValue=ComplexType" << std::endl;
                        }
                        pParamDef->Release();
                    } else {
                        logTransitions << "DEBUG: GetParameterDefinition failed for Filler OperationGroup parameter" << std::endl;
                    }
                    pParam->Release();
                }
                logTransitions << "DEBUG: Total Filler OperationGroup parameters found: " << paramCount << std::endl;
                pEnumParams->Release();
            }
            pFillerOpGroup->Release();
        }
        pFiller->Release();
    } else if (SUCCEEDED(pSegment->QueryInterface(IID_IAAFVaryingValue, (void**)&pVaryingValue))) {
        logTransitions << "DEBUG: pSegment is IAAFVaryingValue (automation/envelope)" << std::endl;
        // Логируем контрольные точки автоматизации
        IEnumAAFControlPoints* pEnumCP = nullptr;
        if (SUCCEEDED(pVaryingValue->GetControlPoints(&pEnumCP))) {
            logTransitions << "DEBUG: VaryingValue has control points:" << std::endl;
            IAAFControlPoint* pCP = nullptr;
            int cpCount = 0;
            while (SUCCEEDED(pEnumCP->NextOne(&pCP))) {
                ++cpCount;
                aafRational_t pos = {0,1};
                // Исправлено: используем GetTime вместо GetPosition
                if (SUCCEEDED(pCP->GetTime(&pos))) {
                    // Получаем значение (обычно через GetValue(size, buf, &bytesRead))
                    aafUInt8 valueBuf[32] = {0};
                    aafUInt32 bytesRead = 0;
                    if (SUCCEEDED(pCP->GetValue(sizeof(valueBuf), valueBuf, &bytesRead))) {
                        // Для простоты: выводим как int (или можно декодировать по типу)
                        int intVal = 0;
                        std::memcpy(&intVal, valueBuf, std::min<size_t>(bytesRead, sizeof(int)));
                        logTransitions << "\tControlPoint time=" << pos.numerator << "/" << pos.denominator << ", value=" << intVal << std::endl;
                    } else {
                        logTransitions << "\tControlPoint time=" << pos.numerator << "/" << pos.denominator << ", value=ComplexType" << std::endl;
                    }
                }
                pCP->Release();
            }
            logTransitions << "DEBUG: Total control points: " << cpCount << std::endl;
            pEnumCP->Release();
        }
        pVaryingValue->Release();
    } else {
        logTransitions << "DEBUG: Unknown segment type, skipping" << std::endl;
    }
}

// --- Descriptive Metadata logging ---
void AAFClipProcessor::logDescriptiveMetadata(IAAFMob* pMob, const std::string& context) {
    if (!pMob) return;
    logTransitions << "DEBUG: logDescriptiveMetadata called for context=" << context << std::endl;
    // Получаем user-defined comments через GetComments
    IEnumAAFTaggedValues* pEnumTagged = nullptr;
    if (SUCCEEDED(pMob->GetComments(&pEnumTagged))) {
        IAAFTaggedValue* pTag = nullptr;
        int tagCount = 0;
        while (SUCCEEDED(pEnumTagged->NextOne(&pTag))) {
            ++tagCount;
            aafWChar tagName[256] = {0};
            if (SUCCEEDED(pTag->GetName(tagName, 256))) {
                std::string tagNameUtf8 = wideToUtf8(tagName);
                logTransitions << "DescriptiveMetadata\tContext=" << context << "\tTagName=" << tagNameUtf8 << std::endl;
            }
            pTag->Release();
        }
        logTransitions << "DEBUG: Total descriptive tags: " << tagCount << std::endl;
        pEnumTagged->Release();
    }
    // Обход через DescriptiveFramework невозможен в вашей версии SDK
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
    clipInfo.fadeInType = "None";
    clipInfo.fadeOutType = "None";
    
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
        // --- Логируем Descriptive Metadata на уровне MasterMob ---
        logDescriptiveMetadata(pMasterMob, "MasterMob");
        // Шаг 4: Найти FileSourceMob из MasterMob
        IAAFSourceMob* pFileSourceMob = findFileSourceMobFromMaster(pMasterMob);
        if (pFileSourceMob) {
            logger.debug(LogCategory::CLIPS, "Found FileSourceMob");
            // --- Логируем Descriptive Metadata на уровне FileSourceMob ---
            IAAFMob* pMob = nullptr;
            if (SUCCEEDED(pFileSourceMob->QueryInterface(IID_IAAFMob, (void**)&pMob))) {
                logDescriptiveMetadata(pMob, "FileSourceMob");
                aafWChar name[256] = {0};
                if (SUCCEEDED(pMob->GetName(name, sizeof(name)))) {
                    clipInfo.originalFileName = wideToUtf8(name);
                }
                pMob->Release();
            }
            
            // Получить имя файла из различных источников
            IAAFMob* pMob = nullptr;
            if (SUCCEEDED(pFileSourceMob->QueryInterface(IID_IAAFMob, (void**)&pMob))) {
                aafWChar name[256] = {0};
                if (SUCCEEDED(pMob->GetName(name, sizeof(name)))) {
                    clipInfo.originalFileName = wideToUtf8(name);
                }
                pMob->Release();
            }
            
            // Получаем аудио свойства через AAFAudioProperties
            if (audioProperties) {
                audioProperties->getAudioProperties(pFileSourceMob, clipInfo);
            }
            
            // Устанавливаем тип источника
            clipInfo.sourceType = "External";
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
    
    // Извлекаем fade-информацию из SourceClip
    extractSourceClipFades(pSourceClip, clipInfo);
    
    // После получения всей информации о клипе:
    logClips << "MobID=" << clipInfo.mobID << "\tName=" << clipInfo.originalFileName
             << "\tTimelineStart=" << clipInfo.timelineStart << "\tTimelineEnd=" << clipInfo.timelineEnd
             << "\tSourceStart=" << clipInfo.sourceStart << "\tDuration=" << clipInfo.duration
             << "\tFileType=" << clipInfo.sourceType << std::endl;
    
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

void AAFClipProcessor::extractSourceClipFades(IAAFSourceClip* pSourceClip, AAFAudioClipInfo& clipInfo) {
    if (!pSourceClip) {
        logger.trace(LogCategory::FADES, "SourceClip is null, no fade information");
        return;
    }
    
    logger.info(LogCategory::FADES, "TESTING: Checking for fade via GetFade method (legacy approach)");
    
    // Попытка использовать старый метод GetFade (может не работать)
    aafLength_t fadeInLen = 0;
    aafFadeType_t fadeInType = kAAFFadeNone;
    aafBoolean_t fadeInPresent = kAAFFalse;
    aafLength_t fadeOutLen = 0;
    aafFadeType_t fadeOutType = kAAFFadeNone;
    aafBoolean_t fadeOutPresent = kAAFFalse;
    
    HRESULT hr = pSourceClip->GetFade(&fadeInLen, &fadeInType, &fadeInPresent,
                                     &fadeOutLen, &fadeOutType, &fadeOutPresent);
    
    if (SUCCEEDED(hr) && (fadeInPresent || fadeOutPresent)) {
        logger.info(LogCategory::FADES, "Found fade via GetFade method");
        
        if (fadeInPresent && fadeInLen > 0) {
            clipInfo.hasFadeIn = true;
            clipInfo.fadeInLength = fadeInLen;
            clipInfo.fadeInType = (fadeInType == kAAFFadeLinearAmp) ? "Linear Amplitude" : 
                                 (fadeInType == kAAFFadeLinearPower) ? "Linear Power" : "None";
            logger.info(LogCategory::FADES, "Fade-In: " + std::to_string(fadeInLen) + " " + clipInfo.fadeInType);
        }
        
        if (fadeOutPresent && fadeOutLen > 0) {
            clipInfo.hasFadeOut = true;
            clipInfo.fadeOutLength = fadeOutLen;
            clipInfo.fadeOutType = (fadeOutType == kAAFFadeLinearAmp) ? "Linear Amplitude" : 
                                  (fadeOutType == kAAFFadeLinearPower) ? "Linear Power" : "None";
            logger.info(LogCategory::FADES, "Fade-Out: " + std::to_string(fadeOutLen) + " " + clipInfo.fadeOutType);
        }
    } else {
        logger.info(LogCategory::CLIPS, "GetFade method failed or no fade present - this is normal");
        // Устанавливаем значения по умолчанию
        clipInfo.hasFadeIn = false;
        clipInfo.hasFadeOut = false;
        clipInfo.fadeInLength = 0;
        clipInfo.fadeOutLength = 0;
        clipInfo.fadeInType = "None";
        clipInfo.fadeOutType = "None";
    }
    
    // ВАЖНО: Основная fade-информация будет извлекаться через анализ Transition 
    // в функции processSegmentForClips, где мы обрабатываем Sequence
}

void AAFClipProcessor::processTransitionForCrossfade(IAAFTransition* pTransition, 
                                                   AAFAudioTrackInfo& trackInfo, 
                                                   aafPosition_t currentPosition) {
    logFades << "DEBUG: processTransitionForCrossfade called, currentPosition=" << currentPosition << std::endl;
    if (!pTransition) {
        logFades << "DEBUG: pTransition is null" << std::endl;
        return;
    }
    logger.info(LogCategory::FADES, "Processing Transition for fade detection (correct approach)");
    // Получаем длительность перехода
    IAAFComponent* pComponent = nullptr;
    aafLength_t transitionLength = 0;
    if (SUCCEEDED(pTransition->QueryInterface(IID_IAAFComponent, (void**)&pComponent))) {
        pComponent->GetLength(&transitionLength);
        pComponent->Release();
    }
    logFades << "DEBUG: transitionLength=" << transitionLength << std::endl;
    if (transitionLength == 0) {
        logFades << "DEBUG: Transition has zero length, skipping" << std::endl;
        return;
    }
    // Получаем операционную группу
    IAAFOperationGroup* pOpGroup = nullptr;
    if (FAILED(pTransition->GetOperationGroup(&pOpGroup))) {
        logFades << "DEBUG: Failed to get OperationGroup from Transition" << std::endl;
        return;
    }
    // Получаем определение операции
    IAAFOperationDef* pOpDef = nullptr;
    if (SUCCEEDED(pOpGroup->GetOperationDefinition(&pOpDef))) {
        logFades << "DEBUG: Got OperationDef" << std::endl;
        // Получаем интерфейс IAAFDefObject для доступа к GetAUID
        IAAFDefObject* pDefObj = nullptr;
        if (SUCCEEDED(pOpDef->QueryInterface(IID_IAAFDefObject, (void**)&pDefObj))) {
            // Получаем AUID операции
            aafUID_t opDefID;
            if (SUCCEEDED(pDefObj->GetAUID(&opDefID))) {
                aafUInt32 nameSize = 0;
                std::string opName = "";
                if (SUCCEEDED(pDefObj->GetNameBufLen(&nameSize)) && nameSize > 0) {
                    std::vector<aafCharacter> nameBuffer(nameSize);
                    if (SUCCEEDED(pDefObj->GetName(nameBuffer.data(), nameSize))) {
                        opName = wideToUtf8(nameBuffer.data());
                    }
                }
                // Логируем имя и AUID операции
                char auidStr[64];
                sprintf(auidStr, "%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x", 
                    opDefID.Data1, opDefID.Data2, opDefID.Data3, 
                    opDefID.Data4[0], opDefID.Data4[1], opDefID.Data4[2], opDefID.Data4[3],
                    opDefID.Data4[4], opDefID.Data4[5], opDefID.Data4[6], opDefID.Data4[7]);
                logFades << "DEBUG: Operation name: " << opName << ", AUID: " << auidStr << std::endl;
                // Расширенный фильтр по имени
                std::vector<std::string> fadeNames = {
                    "Fade", "fade", "FADE", "Fade In", "Fade Out", "Audio Fade In", "Audio Fade Out",
                    "Audio FadeIn", "Audio FadeOut", "FadeIn", "FadeOut"
                };
                bool isFade = false;
                for (const auto& fadeStr : fadeNames) {
                    if (opName.find(fadeStr) != std::string::npos) {
                        isFade = true;
                        break;
                    }
                }
                if (isFade) {
                    logFades << "FADE operation detected: " << opName << ", AUID: " << auidStr << ", Length=" << transitionLength << ", pos=" << currentPosition << std::endl;
                }
                // Проверяем, является ли это MonoAudioMixdown (crossfade)
                if (memcmp(&opDefID, &kAAFOperationDef_MonoAudioMixdown, sizeof(aafUID_t)) == 0) {
                    logFades << "CROSSFADE detected: Length=" << transitionLength << ", pos=" << currentPosition << std::endl;
                }
            } else {
                logFades << "DEBUG: GetAUID failed for OperationDef" << std::endl;
            }
            pDefObj->Release();
        } else {
            logFades << "DEBUG: QueryInterface for IAAFDefObject failed" << std::endl;
        }
        pOpDef->Release();
    } else {
        logFades << "DEBUG: GetOperationDefinition failed for OperationGroup" << std::endl;
    }
    pOpGroup->Release();
}

// Вспомогательная функция для логирования всех Transition и их параметров
static void logTransitionDetails(IAAFTransition* pTransition, const std::string& trackName, aafPosition_t seqPos) {
    logTransitions << "DEBUG: logTransitionDetails called for track=" << trackName << ", seqPos=" << seqPos << std::endl;
    if (!pTransition) {
        logTransitions << "DEBUG: pTransition is null" << std::endl;
        return;
    }
    // Получаем IAAFComponent для длины
    IAAFComponent* pComp = nullptr;
    aafLength_t length = 0;
    if (SUCCEEDED(pTransition->QueryInterface(IID_IAAFComponent, (void**)&pComp))) {
        pComp->GetLength(&length);
        pComp->Release();
    }
    // Получаем OperationGroup
    IAAFOperationGroup* pOpGroup = nullptr;
    if (SUCCEEDED(pTransition->GetOperationGroup(&pOpGroup))) {
        logTransitions << "DEBUG: Got OperationGroup" << std::endl;
        // Получаем OperationDef (имя операции)
        IAAFOperationDef* pOpDef = nullptr;
        if (SUCCEEDED(pOpGroup->GetOperationDefinition(&pOpDef))) {
            logTransitions << "DEBUG: Got OperationDef" << std::endl;
            IAAFDefObject* pDefObj = nullptr;
            if (SUCCEEDED(pOpDef->QueryInterface(IID_IAAFDefObject, (void**)&pDefObj))) {
                aafWChar opName[256] = {0};
                if (SUCCEEDED(pDefObj->GetName(opName, 256))) {
                    std::string opNameUtf8 = wideToUtf8(opName);
                    logTransitions << "Transition\tTrack=" << trackName << "\tSeqPos=" << seqPos
                                  << "\tLength=" << length << "\tOpName=" << opNameUtf8 << std::endl;
                } else {
                    logTransitions << "DEBUG: GetName failed for OperationDef" << std::endl;
                }
                pDefObj->Release();
            } else {
                logTransitions << "DEBUG: QueryInterface for IAAFDefObject failed" << std::endl;
            }
            // Логируем параметры OperationGroup
            IEnumAAFParameters* pEnumParams = nullptr;
            if (SUCCEEDED(pOpGroup->GetParameters(&pEnumParams))) {
                logTransitions << "DEBUG: Got Parameters enum" << std::endl;
                IAAFParameter* pParam = nullptr;
                int paramCount = 0;
                while (SUCCEEDED(pEnumParams->NextOne(&pParam))) {
                    ++paramCount;
                    IAAFParameterDef* pParamDef = nullptr;
                    if (SUCCEEDED(pParam->GetParameterDefinition(&pParamDef))) {
                        IAAFDefObject* pParamDefObj = nullptr;
                        std::string paramNameUtf8 = "";
                        aafUID_t paramID = {};
                        if (SUCCEEDED(pParamDef->QueryInterface(IID_IAAFDefObject, (void**)&pParamDefObj))) {
                            aafUInt32 nameSize = 0;
                            if (SUCCEEDED(pParamDefObj->GetNameBufLen(&nameSize)) && nameSize > 0) {
                                std::vector<aafCharacter> nameBuffer(nameSize);
                                if (SUCCEEDED(pParamDefObj->GetName(nameBuffer.data(), nameSize))) {
                                    paramNameUtf8 = wideToUtf8(nameBuffer.data());
                                }
                            }
                            pParamDefObj->GetAUID(&paramID);
                            pParamDefObj->Release();
                        }
                        // Попробуем получить значение параметра (если это ConstantValue)
                        IAAFConstantValue* pConstVal = nullptr;
                        if (SUCCEEDED(pParam->QueryInterface(IID_IAAFConstantValue, (void**)&pConstVal))) {
                            aafUInt32 bytesRead = 0;
                            aafUInt8 valueBuf[32] = {0};
                            if (SUCCEEDED(pConstVal->GetValue(sizeof(valueBuf), valueBuf, &bytesRead))) {
                                int intVal = 0;
                                std::memcpy(&intVal, valueBuf, std::min<size_t>(bytesRead, sizeof(int)));
                                logTransitions << "\tParam=" << paramNameUtf8 << "\tValue=" << intVal << std::endl;
                            }
                            pConstVal->Release();
                        } else {
                            logTransitions << "\tParam=" << paramNameUtf8 << "\tValue=ComplexType" << std::endl;
                        }
                        pParamDef->Release();
                    } else {
                        logTransitions << "DEBUG: GetParameterDefinition failed for parameter" << std::endl;
                    }
                    pParam->Release();
                }
                logTransitions << "DEBUG: Total parameters found: " << paramCount << std::endl;
                pEnumParams->Release();
            } else {
                logTransitions << "DEBUG: GetParameters failed for OperationGroup" << std::endl;
            }
            pOpDef->Release();
        } else {
            logTransitions << "DEBUG: GetOperationDefinition failed for OperationGroup" << std::endl;
        }
        pOpGroup->Release();
    } else {
        logTransitions << "DEBUG: GetOperationGroup failed for Transition" << std::endl;
    }
}

// Рекурсивный обход Sequence для поиска всех Transition
static void processSequenceForTransitions(IAAFSequence* pSequence, const std::string& trackName, aafPosition_t seqStart) {
    logTransitions << "DEBUG: processSequenceForTransitions called for track=" << trackName << ", seqStart=" << seqStart << std::endl;
    if (!pSequence) {
        logTransitions << "DEBUG: pSequence is null" << std::endl;
        return;
    }
    IEnumAAFComponents* pEnum = nullptr;
    if (SUCCEEDED(pSequence->GetComponents(&pEnum))) {
        int compCount = 0;
        IAAFComponent* pComponent = nullptr;
        aafPosition_t seqPos = seqStart;
        while (SUCCEEDED(pEnum->NextOne(&pComponent))) {
            ++compCount;
            aafLength_t componentLength = 0;
            pComponent->GetLength(&componentLength);
            // Проверяем тип компонента
            IAAFTransition* pTransition = nullptr;
            if (SUCCEEDED(pComponent->QueryInterface(IID_IAAFTransition, (void**)&pTransition))) {
                logTransitions << "DEBUG: Found IAAFTransition in processSequenceForTransitions at seqPos=" << seqPos << std::endl;
                logTransitionDetails(pTransition, trackName, seqPos);
                pTransition->Release();
            } else {
                // Новый блок: OperationGroup
                IAAFOperationGroup* pOpGroup = nullptr;
                if (SUCCEEDED(pComponent->QueryInterface(IID_IAAFOperationGroup, (void**)&pOpGroup))) {
                    logTransitions << "DEBUG: Found IAAFOperationGroup in processSequenceForTransitions at seqPos=" << seqPos << std::endl;
                    // Логируем имя операции, параметры и AUID
                    IAAFOperationDef* pOpDef = nullptr;
                    if (SUCCEEDED(pOpGroup->GetOperationDefinition(&pOpDef))) {
                        IAAFDefObject* pDefObj = nullptr;
                        if (SUCCEEDED(pOpDef->QueryInterface(IID_IAAFDefObject, (void**)&pDefObj))) {
                            aafWChar opName[256] = {0};
                            if (SUCCEEDED(pDefObj->GetName(opName, 256))) {
                                std::string opNameUtf8 = wideToUtf8(opName);
                                logTransitions << "OperationGroup\tTrack=" << trackName << "\tSeqPos=" << seqPos << "\tOpName=" << opNameUtf8 << std::endl;
                            }
                            aafUID_t opAUID;
                            if (SUCCEEDED(pDefObj->GetAUID(&opAUID))) {
                                char auidStr[64];
                                sprintf(auidStr, "%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x", 
                                    opAUID.Data1, opAUID.Data2, opAUID.Data3, 
                                    opAUID.Data4[0], opAUID.Data4[1], opAUID.Data4[2], opAUID.Data4[3],
                                    opAUID.Data4[4], opAUID.Data4[5], opAUID.Data4[6], opAUID.Data4[7]);
                                logTransitions << "\tAUID=" << auidStr << std::endl;
                            }
                            pDefObj->Release();
                        }
                        pOpDef->Release();
                    }
                    // Логируем параметры
                    IEnumAAFParameters* pEnumParams = nullptr;
                    if (SUCCEEDED(pOpGroup->GetParameters(&pEnumParams))) {
                        IAAFParameter* pParam = nullptr;
                        int paramCount = 0;
                        while (SUCCEEDED(pEnumParams->NextOne(&pParam))) {
                            ++paramCount;
                            IAAFParameterDef* pParamDef = nullptr;
                            if (SUCCEEDED(pParam->GetParameterDefinition(&pParamDef))) {
                                IAAFDefObject* pParamDefObj = nullptr;
                                std::string paramNameUtf8 = "";
                                aafUID_t paramID = {};
                                if (SUCCEEDED(pParamDef->QueryInterface(IID_IAAFDefObject, (void**)&pParamDefObj))) {
                                    aafUInt32 nameSize = 0;
                                    if (SUCCEEDED(pParamDefObj->GetNameBufLen(&nameSize)) && nameSize > 0) {
                                        std::vector<aafCharacter> nameBuffer(nameSize);
                                        if (SUCCEEDED(pParamDefObj->GetName(nameBuffer.data(), nameSize))) {
                                            paramNameUtf8 = wideToUtf8(nameBuffer.data());
                                        }
                                    }
                                    pParamDefObj->GetAUID(&paramID);
                                    pParamDefObj->Release();
                                }
                                IAAFConstantValue* pConstVal = nullptr;
                                if (SUCCEEDED(pParam->QueryInterface(IID_IAAFConstantValue, (void**)&pConstVal))) {
                                    aafUInt32 bytesRead = 0;
                                    aafUInt8 valueBuf[32] = {0};
                                    if (SUCCEEDED(pConstVal->GetValue(sizeof(valueBuf), valueBuf, &bytesRead))) {
                                        int intVal = 0;
                                        std::memcpy(&intVal, valueBuf, std::min<size_t>(bytesRead, sizeof(int)));
                                        logTransitions << "\tOperationGroup Param=" << paramNameUtf8 << "\tValue=" << intVal << std::endl;
                                    }
                                    pConstVal->Release();
                                } else {
                                    logTransitions << "\tOperationGroup Param=" << paramNameUtf8 << "\tValue=ComplexType" << std::endl;
                                }
                                pParamDef->Release();
                            }
                            pParam->Release();
                        }
                        logTransitions << "DEBUG: Total OperationGroup parameters found: " << paramCount << std::endl;
                        pEnumParams->Release();
                    }
                    // Рекурсивно обрабатываем входные сегменты
                    aafUInt32 numInputs = 0;
                    if (SUCCEEDED(pOpGroup->CountSourceSegments(&numInputs))) {
                        for (aafUInt32 i = 0; i < numInputs; ++i) {
                            IAAFSegment* pInputSegment = nullptr;
                            if (SUCCEEDED(pOpGroup->GetInputSegmentAt(i, &pInputSegment))) {
                                processSequenceForTransitions(dynamic_cast<IAAFSequence*>(pInputSegment), trackName, seqPos); // рекурсивно если это Sequence
                                pInputSegment->Release();
                            }
                        }
                    }
                    pOpGroup->Release();
                } else {
                    // Новый блок: Filler
                    IAAFFiller* pFiller = nullptr;
                    if (SUCCEEDED(pComponent->QueryInterface(IID_IAAFFiller, (void**)&pFiller))) {
                        logTransitions << "DEBUG: Found IAAFFiller in processSequenceForTransitions at seqPos=" << seqPos << std::endl;
                        // Логируем информацию о Filler
                        IAAFComponent* pFillerComp = nullptr;
                        if (SUCCEEDED(pFiller->QueryInterface(IID_IAAFComponent, (void**)&pFillerComp))) {
                            aafLength_t fillerLength = 0;
                            pFillerComp->GetLength(&fillerLength);
                            logTransitions << "Filler\tTrack=" << trackName << "\tSeqPos=" << seqPos << "\tLength=" << fillerLength << std::endl;
                            pFillerComp->Release();
                        }
                        // Логируем параметры OperationGroup внутри Filler (если есть)
                        IAAFOperationGroup* pFillerOpGroup = nullptr;
                        if (SUCCEEDED(pFiller->QueryInterface(IID_IAAFOperationGroup, (void**)&pFillerOpGroup))) {
                            logTransitions << "DEBUG: Filler contains OperationGroup" << std::endl;
                            // Логика аналогична обработке OpGroup выше
                            IEnumAAFParameters* pEnumParams = nullptr;
                            if (SUCCEEDED(pFillerOpGroup->GetParameters(&pEnumParams))) {
                                logTransitions << "DEBUG: Filler OperationGroup has parameters:" << std::endl;
                                IAAFParameter* pParam = nullptr;
                                int paramCount = 0;
                                while (SUCCEEDED(pEnumParams->NextOne(&pParam))) {
                                    ++paramCount;
                                    IAAFParameterDef* pParamDef = nullptr;
                                    if (SUCCEEDED(pParam->GetParameterDefinition(&pParamDef))) {
                                        IAAFDefObject* pParamDefObj = nullptr;
                                        std::string paramNameUtf8 = "";
                                        aafUID_t paramID = {};
                                        if (SUCCEEDED(pParamDef->QueryInterface(IID_IAAFDefObject, (void**)&pParamDefObj))) {
                                            aafUInt32 nameSize = 0;
                                            if (SUCCEEDED(pParamDefObj->GetNameBufLen(&nameSize)) && nameSize > 0) {
                                                std::vector<aafCharacter> nameBuffer(nameSize);
                                                if (SUCCEEDED(pParamDefObj->GetName(nameBuffer.data(), nameSize))) {
                                                    paramNameUtf8 = wideToUtf8(nameBuffer.data());
                                                }
                                            }
                                            pParamDefObj->GetAUID(&paramID);
                                            pParamDefObj->Release();
                                        }
                                        IAAFConstantValue* pConstVal = nullptr;
                                        if (SUCCEEDED(pParam->QueryInterface(IID_IAAFConstantValue, (void**)&pConstVal))) {
                                            aafUInt32 bytesRead = 0;
                                            aafUInt8 valueBuf[32] = {0};
                                            if (SUCCEEDED(pConstVal->GetValue(sizeof(valueBuf), valueBuf, &bytesRead))) {
                                                int intVal = 0;
                                                std::memcpy(&intVal, valueBuf, std::min<size_t>(bytesRead, sizeof(int)));
                                                logTransitions << "\tFiller OperationGroup Param=" << paramNameUtf8 << "\tValue=" << intVal << std::endl;
                                            }
                                            pConstVal->Release();
                                        } else {
                                            logTransitions << "\tFiller OperationGroup Param=" << paramNameUtf8 << "\tValue=ComplexType" << std::endl;
                                        }
                                        pParamDef->Release();
                                    } else {
                                        logTransitions << "DEBUG: GetParameterDefinition failed for Filler OperationGroup parameter" << std::endl;
                                    }
                                    pParam->Release();
                                }
                                logTransitions << "DEBUG: Total Filler OperationGroup parameters found: " << paramCount << std::endl;
                                pEnumParams->Release();
                            }
                            pFillerOpGroup->Release();
                        }
                        pFiller->Release();
                    }
                }
            }
            pComponent->Release();
        }
        pEnum->Release();
    } else {
        logTransitions << "DEBUG: GetComponents failed for Sequence" << std::endl;
    }
    pSequence->Release();
}

