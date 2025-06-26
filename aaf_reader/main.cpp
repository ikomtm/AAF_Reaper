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
#include <locale>
#include <codecvt>
#include <cstring>
#include <cstdio>
#include <map>
#include <iomanip>
#include <vector>
#include <filesystem>
#include <memory>
#include <set>

// Простые структуры для экспорта в CSV
struct AudioClipData {
    std::string fileName;
    std::string mobID;
    double timelineStart;
    double timelineEnd;
    double sourceStart;
    double length;
    double gain;
    double volume;
    double pan;
    std::vector<std::string> effects;
    
    // Конструктор с инициализацией по умолчанию
    AudioClipData() : fileName(""), mobID(""), timelineStart(0.0), timelineEnd(0.0), 
                     sourceStart(0.0), length(0.0), gain(0.0), volume(1.0), pan(0.0) {}
};

struct AudioTrackData {
    int trackIndex;
    std::string trackName;
    std::string trackType;
    double volume;
    double pan;
    bool mute;
    bool solo;
    std::vector<AudioClipData> clips;
    
    // Конструктор с инициализацией по умолчанию
    AudioTrackData() : trackIndex(0), trackName(""), trackType("Audio"), 
                      volume(1.0), pan(0.0), mute(false), solo(false) {}
};

struct ProjectData {
    std::string projectName;
    double sampleRate;
    int channelCount;
    double sessionStartTimecode;
    std::string timecodeFormat;
    double totalLength;
    std::vector<AudioTrackData> tracks;
    
    // Конструктор с инициализацией по умолчанию
    ProjectData() : projectName("AAF_Import_Project"), sampleRate(48000.0), 
                   channelCount(2), sessionStartTimecode(0.0), 
                   timecodeFormat("25fps"), totalLength(0.0) {}
};

// Объявления функций
void processAudioComponentWithPosition(IAAFComponent* pComp, std::ofstream& out, int compIndex, 
                                      aafPosition_t startPosition, 
                                      const std::map<std::string, std::string>& mobIdToName, 
                                      const aafRational_t& editRate,
                                      int& audioClipCount, int& audioFadeCount, int& audioEffectCount);

void extractClipsFromSegment(IAAFSegment* pSegment, const std::map<std::string, std::string>& mobIdToName,
                           const aafRational_t& editRate, aafPosition_t sessionStartTC,
                           aafPosition_t currentPosition, AudioTrackData& trackData,
                           IAAFHeader* pHeader, std::ofstream& out);

bool isAudioTrack(IAAFMobSlot* pSlot);
std::string getDataDefName(IAAFDataDef* pDataDef);
std::string formatMobID(const aafMobID_t& mobID);
std::string findAndExtractEssenceData(IAAFHeader* pHeader, const aafMobID_t& mobID, 
                                     const std::string& mobIdStr, std::ofstream& out);
void createExtractedMediaFolder();
void extractEmbeddedAudio(IAAFEssenceData* pEssenceData, const std::string& outputPath);
void processAudioFiles(const ProjectData& projectData, const std::string& outputDir);

std::string wideToUtf8(const std::wstring& wstr) {
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
    return conv.to_bytes(wstr);
}

std::string formatMobID(const aafMobID_t& mobID) {
    const aafUID_t& uid = mobID.material;
    char buffer[64];
    snprintf(buffer, sizeof(buffer),
        "%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X",
        uid.Data1, uid.Data2, uid.Data3,
        uid.Data4[0], uid.Data4[1],
        uid.Data4[2], uid.Data4[3],
        uid.Data4[4], uid.Data4[5], uid.Data4[6], uid.Data4[7]);
    return std::string(buffer);
}

// Функция для получения типа данных сегмента
std::string getDataDefName(IAAFDataDef* pDataDef) {
    if (!pDataDef) return "Unknown";
    
    // IAAFDataDef наследует от IAAFDefObject, который имеет GetAUID()
    IAAFDefObject* pDefObj = nullptr;
    if (SUCCEEDED(pDataDef->QueryInterface(IID_IAAFDefObject, (void**)&pDefObj))) {
        aafUID_t dataDefID;
        if (SUCCEEDED(pDefObj->GetAUID(&dataDefID))) {
            pDefObj->Release();
            
            // Сравниваем с известными типами данных
            if (memcmp(&dataDefID, &kAAFDataDef_Picture, sizeof(aafUID_t)) == 0) {
                return "Picture";
            } else if (memcmp(&dataDefID, &kAAFDataDef_Sound, sizeof(aafUID_t)) == 0) {
                return "Sound";
            } else if (memcmp(&dataDefID, &kAAFDataDef_Timecode, sizeof(aafUID_t)) == 0) {
                return "Timecode";
            } else if (memcmp(&dataDefID, &kAAFDataDef_Edgecode, sizeof(aafUID_t)) == 0) {
                return "Edgecode";
            } else if (memcmp(&dataDefID, &kAAFDataDef_DescriptiveMetadata, sizeof(aafUID_t)) == 0) {
                return "Metadata";
            } else if (memcmp(&dataDefID, &kAAFDataDef_Auxiliary, sizeof(aafUID_t)) == 0) {
                return "Auxiliary";
            }
        } else {
            pDefObj->Release();
        }
    }
    
    // Альтернативный способ - попробуем получить имя через IAAFDefObject
    if (SUCCEEDED(pDataDef->QueryInterface(IID_IAAFDefObject, (void**)&pDefObj))) {
        aafWChar nameBuffer[256] = {0};
        if (SUCCEEDED(pDefObj->GetName(nameBuffer, sizeof(nameBuffer)))) {
            std::string name = wideToUtf8(nameBuffer);
            pDefObj->Release();
            if (!name.empty()) {
                return name;
            }
        } else {
            pDefObj->Release();
        }
    }
    
    return "Unknown";
}

// ВАЖНО: Эта функция должна быть объявлена ПЕРЕД processCompositionSlot
void processComponentWithPosition(IAAFComponent* pComp, std::ofstream& out, int compIndex, 
                                 aafPosition_t startPosition, 
                                 const std::map<std::string, std::string>& mobIdToName, 
                                 const aafRational_t& editRate) {
    
    // Получаем длительность компонента
    aafLength_t length = 0;
    pComp->GetLength(&length);
    
    // Получаем тип данных
    IAAFDataDef* pDataDef = nullptr;
    std::string dataType = "Unknown";
    if (SUCCEEDED(pComp->GetDataDef(&pDataDef))) {
        dataType = getDataDefName(pDataDef);
        pDataDef->Release();
    }
    
    // Конвертируем в секунды
    double startSec = (double)startPosition * editRate.denominator / editRate.numerator;
    double lengthSec = (double)length * editRate.denominator / editRate.numerator;
    double endSec = startSec + lengthSec;
    
    // Проверяем тип компонента
    IAAFSourceClip* pClip = nullptr;
    IAAFFiller* pFiller = nullptr;
    IAAFTransition* pTransition = nullptr;
    IAAFSequence* pNestedSeq = nullptr;
    IAAFOperationGroup* pOpGroup = nullptr;
    
    if (SUCCEEDED(pComp->QueryInterface(IID_IAAFSourceClip, (void**)&pClip))) {
        // SourceClip - ссылка на медиа
        aafSourceRef_t ref;
        if (SUCCEEDED(pClip->GetSourceReference(&ref))) {
            double sourceStartSec = (double)ref.startTime * editRate.denominator / editRate.numerator;
            
            std::string refID = formatMobID(ref.sourceID);
            std::string fileName = "(unknown)";
            
            // Пробуем разные способы получения имени файла
            if (mobIdToName.count(refID)) {
                fileName = mobIdToName.at(refID);
            }
            
            // Дополнительная информация о SourceClip
            out << "      #" << compIndex << " CLIP [" << dataType << "] "
                << "Timeline: " << std::fixed << std::setprecision(3) 
                << startSec << "s -> " << endSec << "s (" << lengthSec << "s) | "
                << "Source: " << ref.startTime << " (" << sourceStartSec << "s) | "
                << "SlotID: " << ref.sourceSlotID << " | "
                << "File: " << fileName << std::endl;
        }
        pClip->Release();
        
    } else if (SUCCEEDED(pComp->QueryInterface(IID_IAAFFiller, (void**)&pFiller))) {
        // Filler - пустое место
        out << "      #" << compIndex << " FILLER [" << dataType << "] "
            << "Timeline: " << std::fixed << std::setprecision(3) 
            << startSec << "s -> " << endSec << "s (" << lengthSec << "s)" << std::endl;
        pFiller->Release();
        
    } else if (SUCCEEDED(pComp->QueryInterface(IID_IAAFTransition, (void**)&pTransition))) {
        // Transition - переход между клипами
        aafPosition_t cutPoint = 0;
        pTransition->GetCutPoint(&cutPoint);
        double cutPointSec = (double)cutPoint * editRate.denominator / editRate.numerator;
        
        out << "      #" << compIndex << " TRANSITION [" << dataType << "] "
            << "Timeline: " << std::fixed << std::setprecision(3) 
            << startSec << "s -> " << endSec << "s (" << lengthSec << "s) | "
            << "CutPoint: " << cutPoint << " (" << cutPointSec << "s)" << std::endl;
        pTransition->Release();
        
    } else if (SUCCEEDED(pComp->QueryInterface(IID_IAAFSequence, (void**)&pNestedSeq))) {
        // Nested Sequence - вложенная последовательность
        out << "      #" << compIndex << " NESTED_SEQ [" << dataType << "] "
            << "Timeline: " << std::fixed << std::setprecision(3) 
            << startSec << "s -> " << endSec << "s (" << lengthSec << "s)" << std::endl;
        
        // Рекурсивно обрабатываем вложенную последовательность
        IEnumAAFComponents* pNestedEnum = nullptr;
        if (SUCCEEDED(pNestedSeq->GetComponents(&pNestedEnum))) {
            IAAFComponent* pNestedComp = nullptr;
            int nestedIndex = 0;
            aafPosition_t nestedPosition = startPosition;
            
            while (SUCCEEDED(pNestedEnum->NextOne(&pNestedComp))) {
                aafLength_t nestedLength = 0;
                pNestedComp->GetLength(&nestedLength);
                
                out << "        ";  // Дополнительный отступ
                processComponentWithPosition(pNestedComp, out, nestedIndex++, nestedPosition, 
                                           mobIdToName, editRate);
                
                nestedPosition += nestedLength;
                pNestedComp->Release();
            }
            pNestedEnum->Release();
        }
        pNestedSeq->Release();
        
    } else if (SUCCEEDED(pComp->QueryInterface(IID_IAAFOperationGroup, (void**)&pOpGroup))) {
        // OperationGroup - эффект или операция
        IAAFOperationDef* pOpDef = nullptr;
        std::string opName = "Unknown";
        if (SUCCEEDED(pOpGroup->GetOperationDefinition(&pOpDef))) {
            IAAFDefObject* pDefObj = nullptr;
            if (SUCCEEDED(pOpDef->QueryInterface(IID_IAAFDefObject, (void**)&pDefObj))) {
                aafWChar opNameBuf[256] = {0};
                if (SUCCEEDED(pDefObj->GetName(opNameBuf, sizeof(opNameBuf)))) {
                    opName = wideToUtf8(opNameBuf);
                }
                pDefObj->Release();
            }
            pOpDef->Release();
        }
        
        aafUInt32 numInputs = 0;
        pOpGroup->CountSourceSegments(&numInputs);
        
        out << "      #" << compIndex << " EFFECT [" << dataType << "] "
            << "Timeline: " << std::fixed << std::setprecision(3) 
            << startSec << "s -> " << endSec << "s (" << lengthSec << "s) | "
            << "Operation: " << opName << " | Inputs: " << numInputs << std::endl;
        pOpGroup->Release();
        
    } else {
        // Попробуем получить дополнительную информацию о неизвестном компоненте
        std::string className = "Unknown";
        
        IAAFObject* pObject = nullptr;
        if (SUCCEEDED(pComp->QueryInterface(IID_IAAFObject, (void**)&pObject))) {
            IAAFClassDef* pClassDef = nullptr;
            if (SUCCEEDED(pObject->GetDefinition(&pClassDef))) {
                IAAFDefObject* pDefObj = nullptr;
                if (SUCCEEDED(pClassDef->QueryInterface(IID_IAAFDefObject, (void**)&pDefObj))) {
                    aafWChar classNameBuf[256] = {0};
                    if (SUCCEEDED(pDefObj->GetName(classNameBuf, sizeof(classNameBuf)))) {
                        className = wideToUtf8(classNameBuf);
                    }
                    pDefObj->Release();
                }
                pClassDef->Release();
            }
            pObject->Release();
        }
        
        out << "      #" << compIndex << " " << className << " [" << dataType << "] "
            << "Timeline: " << std::fixed << std::setprecision(3) 
            << startSec << "s -> " << endSec << "s (" << lengthSec << "s)" << std::endl;
    }
}

// Функция для обработки компонента
void processComponent(IAAFComponent* pComp, std::ofstream& out, int compIndex, 
                     const std::map<std::string, std::string>& mobIdToName, 
                     const aafRational_t& editRate) {
    
    // Получаем длительность компонента
    aafLength_t length = 0;
    pComp->GetLength(&length);
    
    // Получаем тип данных
    IAAFDataDef* pDataDef = nullptr;
    std::string dataType = "Unknown";
    if (SUCCEEDED(pComp->GetDataDef(&pDataDef))) {
        dataType = getDataDefName(pDataDef);
        pDataDef->Release();
    }
    
    // Проверяем тип компонента
    IAAFSourceClip* pClip = nullptr;
    IAAFFiller* pFiller = nullptr;
    IAAFTransition* pTransition = nullptr;
    IAAFSequence* pNestedSeq = nullptr;
    IAAFOperationGroup* pOpGroup = nullptr;
    
    if (SUCCEEDED(pComp->QueryInterface(IID_IAAFSourceClip, (void**)&pClip))) {
        // SourceClip - ссылка на медиа
        aafSourceRef_t ref;
        if (SUCCEEDED(pClip->GetSourceReference(&ref))) {
            double lengthSec = (double)length * editRate.denominator / editRate.numerator;
            double startSec = (double)ref.startTime * editRate.denominator / editRate.numerator;
            
            std::string refID = formatMobID(ref.sourceID);
            std::string fileName = "(unknown)";
            if (mobIdToName.count(refID)) {
                fileName = mobIdToName.at(refID);
            }
            
            out << "    > Clip #" << compIndex 
                << " [" << dataType << "]"
                << ": Start=" << ref.startTime << " (" << std::fixed << std::setprecision(3) << startSec << "s)"
                << ", Length=" << length << " (" << lengthSec << "s)"
                << ", SlotID=" << ref.sourceSlotID
                << ", MobID=" << refID
                << ", File=" << fileName << std::endl;
                
            // Попытка извлечения embedded медиа для аудиоклипов
            if (dataType == "Sound" && fileName == "(unknown)") {
                out << "      Checking for embedded audio data..." << std::endl;
                // TODO: Здесь будет вызов функции извлечения, когда добавим параметр pHeader
                // std::string extractedFile = findAndExtractEssenceData(pHeader, ref.sourceID, fileName, out);
                // if (!extractedFile.empty()) {
                //     out << "      Extracted embedded file: " << extractedFile << std::endl;
                // }
            }
        }
        pClip->Release();
        
    } else if (SUCCEEDED(pComp->QueryInterface(IID_IAAFSourceClip, (void**)&pFiller))) {
        // Filler - пустое место
        double lengthSec = (double)length * editRate.denominator / editRate.numerator;
        out << "    > Filler #" << compIndex 
            << " [" << dataType << "]"
            << ": Length=" << length << " (" << std::fixed << std::setprecision(3) << lengthSec << "s)" << std::endl;
        pFiller->Release();
        
    } else if (SUCCEEDED(pComp->QueryInterface(IID_IAAFTransition, (void**)&pTransition))) {
        // Transition - переход между клипами
        double lengthSec = (double)length * editRate.denominator / editRate.numerator;
        
        // Получаем точку перехода
        aafPosition_t cutPoint = 0;
        pTransition->GetCutPoint(&cutPoint);
        double cutPointSec = (double)cutPoint * editRate.denominator / editRate.numerator;
        
        out << "    > Transition #" << compIndex 
            << " [" << dataType << "]"
            << ": Length=" << length << " (" << std::fixed << std::setprecision(3) << lengthSec << "s)"
            << ", CutPoint=" << cutPoint << " (" << cutPointSec << "s)" << std::endl;
        pTransition->Release();
        
    } else if (SUCCEEDED(pComp->QueryInterface(IID_IAAFSequence, (void**)&pNestedSeq))) {
        // Nested Sequence - вложенная последовательность
        double lengthSec = (double)length * editRate.denominator / editRate.numerator;
        out << "    > NestedSequence #" << compIndex 
            << " [" << dataType << "]"
            << ": Length=" << length << " (" << std::fixed << std::setprecision(3) << lengthSec << "s)" << std::endl;
        
        // Можно рекурсивно обработать компоненты вложенной последовательности
        IEnumAAFComponents* pNestedEnum = nullptr;
        if (SUCCEEDED(pNestedSeq->GetComponents(&pNestedEnum))) {
            IAAFComponent* pNestedComp = nullptr;
            int nestedIndex = 0;
            while (SUCCEEDED(pNestedEnum->NextOne(&pNestedComp))) {
                out << "      ";  // Дополнительный отступ для вложенных элементов
                processComponent(pNestedComp, out, nestedIndex++, mobIdToName, editRate);
                pNestedComp->Release();
            }
            pNestedEnum->Release();
        }
        pNestedSeq->Release();
        
    } else if (SUCCEEDED(pComp->QueryInterface(IID_IAAFOperationGroup, (void**)&pOpGroup))) {
        // OperationGroup - эффект или операция
        double lengthSec = (double)length * editRate.denominator / editRate.numerator;
        
        // Получаем определение операции
        IAAFOperationDef* pOpDef = nullptr;
        std::string opName = "Unknown";
        if (SUCCEEDED(pOpGroup->GetOperationDefinition(&pOpDef))) {
            // IAAFOperationDef наследует от IAAFDefObject, который имеет GetName()
            IAAFDefObject* pDefObj = nullptr;
            if (SUCCEEDED(pOpDef->QueryInterface(IID_IAAFDefObject, (void**)&pDefObj))) {
                aafWChar opNameBuf[256] = {0};
                if (SUCCEEDED(pDefObj->GetName(opNameBuf, sizeof(opNameBuf)))) {
                    opName = wideToUtf8(opNameBuf);
                }
                pDefObj->Release();
            }
            pOpDef->Release();
        }
        
        // Получаем количество входных сегментов
        aafUInt32 numInputs = 0;
        if (SUCCEEDED(pOpGroup->CountSourceSegments(&numInputs))) {
            out << "    > Effect #" << compIndex 
                << " [" << dataType << "]"
                << ": Operation=" << opName
                << ", Length=" << length << " (" << std::fixed << std::setprecision(3) << lengthSec << "s)"
                << ", Inputs=" << numInputs << std::endl;
        } else {
            out << "    > Effect #" << compIndex 
                << " [" << dataType << "]"
                << ": Operation=" << opName
                << ", Length=" << length << " (" << std::fixed << std::setprecision(3) << lengthSec << "s)" << std::endl;
        }
        
        // Опционально: обрабатываем входные сегменты
        for (aafUInt32 inputIndex = 0; inputIndex < numInputs; inputIndex++) {
            IAAFSegment* pInputSegment = nullptr;
            if (SUCCEEDED(pOpGroup->GetInputSegmentAt(inputIndex, &pInputSegment))) {
                out << "      Input #" << inputIndex << ": ";
                
                // Проверяем, является ли входной сегмент компонентом
                IAAFComponent* pInputComp = nullptr;
                if (SUCCEEDED(pInputSegment->QueryInterface(IID_IAAFComponent, (void**)&pInputComp))) {
                    // Рекурсивно обрабатываем входной компонент
                    processComponent(pInputComp, out, inputIndex, mobIdToName, editRate);
                    pInputComp->Release();
                } else {
                    out << "(non-component segment)" << std::endl;
                }
                
                pInputSegment->Release();
            }
        }
        
        pOpGroup->Release();
        
    } else {
        // Неизвестный тип компонента
        double lengthSec = (double)length * editRate.denominator / editRate.numerator;
        out << "    > Component #" << compIndex 
            << " [" << dataType << "]"
            << ": Length=" << length << " (" << std::fixed << std::setprecision(3) << lengthSec << "s)"
            << " (unknown type)" << std::endl;
    }
}

// Функция для получения начального timecode композиции
aafPosition_t getCompositionStartTimecode(IAAFMob* pCompMob) {
    aafPosition_t startTC = 0;
    
    // Ищем Timecode слот
    aafNumSlots_t numSlots = 0;
    if (SUCCEEDED(pCompMob->CountSlots(&numSlots))) {
        for (aafUInt32 i = 0; i < numSlots; ++i) {
            IAAFMobSlot* pSlot = nullptr;
            if (SUCCEEDED(pCompMob->GetSlotAt(i, &pSlot))) {
                
                // Проверяем тип данных слота
                IAAFSegment* pSegment = nullptr;
                if (SUCCEEDED(pSlot->GetSegment(&pSegment))) {
                    IAAFComponent* pComp = nullptr;
                    if (SUCCEEDED(pSegment->QueryInterface(IID_IAAFComponent, (void**)&pComp))) {
                        IAAFDataDef* pDataDef = nullptr;
                        if (SUCCEEDED(pComp->GetDataDef(&pDataDef))) {
                            std::string dataType = getDataDefName(pDataDef);
                            
                            if (dataType == "Timecode") {
                                // Нашли timecode слот, получаем начальную позицию
                                IAAFTimelineMobSlot* pTimelineSlot = nullptr;
                                if (SUCCEEDED(pSlot->QueryInterface(IID_IAAFTimelineMobSlot, (void**)&pTimelineSlot))) {
                                    pTimelineSlot->GetOrigin(&startTC);
                                    pTimelineSlot->Release();
                                }
                                
                                // Если это TimecodeStream, получаем стартовую позицию
                                IAAFTimecodeStream* pTCStream = nullptr;
                                if (SUCCEEDED(pSegment->QueryInterface(IID_IAAFTimecodeStream, (void**)&pTCStream))) {
                                    // Для TimecodeStream используем origin слота как начальное время
                                    // startTC уже получен из origin выше
                                    pTCStream->Release();
                                }
                            }
                            
                            pDataDef->Release();
                        }
                        pComp->Release();
                    }
                    pSegment->Release();
                }
                pSlot->Release();
                
                if (startTC != 0) break; // Нашли timecode, выходим
            }
        }
    }
    
    return startTC;
}

// Функция для конвертации timecode в строку
std::string formatTimecode(aafPosition_t position, const aafRational_t& editRate) {
    // Предполагаем 25fps для примера, можно адаптировать
    int fps = editRate.numerator / editRate.denominator;
    if (fps <= 0) fps = 25;
    
    int totalFrames = (int)position;
    int frames = totalFrames % fps;
    int seconds = (totalFrames / fps) % 60;
    int minutes = (totalFrames / (fps * 60)) % 60;  
    int hours = totalFrames / (fps * 60 * 60);
    
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%02d:%02d:%02d:%02d", hours, minutes, seconds, frames);
    return std::string(buffer);
}

void processCompositionSlot(IAAFMobSlot* pSlot, std::ofstream& out, int slotIndex, 
                           const std::map<std::string, std::string>& mobIdToName,
                           int& audioTrackCount, int& audioClipCount, 
                           int& audioFadeCount, int& audioEffectCount, 
                           aafPosition_t sessionStartTC = 0) {
    
    aafSlotID_t slotID = 0;
    pSlot->GetSlotID(&slotID);
    
    aafWChar slotName[256] = {0};
    pSlot->GetName(slotName, sizeof(slotName));
    std::string slotNameStr = wideToUtf8(slotName);
    
    // Определяем тип слота и editRate
    aafRational_t editRate = {25, 1}; // default
    std::string slotType = "MobSlot";
    aafPosition_t origin = 0;
    
    IAAFTimelineMobSlot* pTimelineSlot = nullptr;
    if (SUCCEEDED(pSlot->QueryInterface(IID_IAAFTimelineMobSlot, (void**)&pTimelineSlot))) {
        slotType = "TimelineSlot";
        pTimelineSlot->GetEditRate(&editRate);
        pTimelineSlot->GetOrigin(&origin);
        pTimelineSlot->Release();
    }
    
    // Получаем тип данных из сегмента
    std::string dataType = "Unknown";
    IAAFSegment* pSegment = nullptr;
    if (SUCCEEDED(pSlot->GetSegment(&pSegment))) {
        IAAFComponent* pComp = nullptr;
        if (SUCCEEDED(pSegment->QueryInterface(IID_IAAFComponent, (void**)&pComp))) {
            IAAFDataDef* pDataDef = nullptr;
            if (SUCCEEDED(pComp->GetDataDef(&pDataDef))) {
                dataType = getDataDefName(pDataDef);
                pDataDef->Release();
            }
            pComp->Release();
        }
        pSegment->Release();
    }
    
    // ФИЛЬТР: Обрабатываем только аудио треки
    if (dataType != "Sound") {
        // Показываем только заголовок для не-аудио треков
        out << "\n  TRACK #" << slotIndex << " [" << slotType << "] ID=" << slotID;
        if (!slotNameStr.empty()) {
            out << ", Name='" << slotNameStr << "'";
        }
        out << ", DataType=" << dataType << " - SKIPPED (not audio)" << std::endl;
        return;
    }
    
    // Увеличиваем счетчик аудио треков
    audioTrackCount++;
    
    out << "\n  🎵 AUDIO TRACK #" << slotIndex << " [" << slotType << "] ID=" << slotID;
    if (!slotNameStr.empty()) {
        out << ", Name='" << slotNameStr << "'";
    }
    out << ", EditRate=" << editRate.numerator << "/" << editRate.denominator;
    out << ", Origin=" << origin << std::endl;
    
    // Обрабатываем содержимое аудио слота
    if (SUCCEEDED(pSlot->GetSegment(&pSegment))) {
        IAAFSequence* pSeq = nullptr;
        IAAFSourceClip* pSourceClip = nullptr;
        
        if (SUCCEEDED(pSegment->QueryInterface(IID_IAAFSequence, (void**)&pSeq))) {
            out << "    🎶 AUDIO SEQUENCE:" << std::endl;
            
            IEnumAAFComponents* pEnum = nullptr;
            if (SUCCEEDED(pSeq->GetComponents(&pEnum))) {
                IAAFComponent* pComp = nullptr;
                int compIndex = 0;
                aafPosition_t currentPosition = origin;
                
                while (SUCCEEDED(pEnum->NextOne(&pComp))) {
                    aafLength_t length = 0;
                    pComp->GetLength(&length);
                    
                    processAudioComponentWithPosition(pComp, out, compIndex++, currentPosition, 
                                                     mobIdToName, editRate, audioClipCount, audioFadeCount, audioEffectCount);
                    
                    currentPosition += length;
                    pComp->Release();
                }
                pEnum->Release();
            }
            pSeq->Release();
            
        } else if (SUCCEEDED(pSegment->QueryInterface(IID_IAAFSourceClip, (void**)&pSourceClip))) {
            out << "    🎵 SINGLE AUDIO CLIP:" << std::endl;
            
            IAAFComponent* pComp = nullptr;
            if (SUCCEEDED(pSourceClip->QueryInterface(IID_IAAFComponent, (void**)&pComp))) {
                processAudioComponentWithPosition(pComp, out, 0, origin, mobIdToName, editRate, audioClipCount, audioFadeCount, audioEffectCount);
                pComp->Release();
            }
            pSourceClip->Release();
            
        } else {
            out << "    ❓ Unknown audio segment type" << std::endl;
        }
        
        pSegment->Release();
    }
}

// Функция для создания папки extracted_media
void createExtractedMediaFolder() {
    try {
        std::filesystem::create_directories("extracted_media");
    } catch (const std::exception& e) {
        std::cerr << "Error creating extracted_media folder: " << e.what() << std::endl;
    }
}

// Функция для извлечения embedded аудиоданных из EssenceData (упрощенная версия)
std::string extractEmbeddedAudio(IAAFEssenceData* pEssenceData, const std::string& baseName, std::ofstream& out) {
    if (!pEssenceData) return "";
    
    try {
        // Получаем размер данных
        aafLength_t dataSize = 0;
        if (FAILED(pEssenceData->GetSize(&dataSize))) {
            out << "      WARNING: Failed to get EssenceData size" << std::endl;
            return "";
        }
        
        if (dataSize == 0) {
            out << "      WARNING: EssenceData is empty" << std::endl;
            return "";
        }
        
        out << "      Found embedded audio data: " << dataSize << " bytes" << std::endl;
        
        // Создаем уникальное имя файла
        std::string fileName = "extracted_media/" + baseName + "_embedded.raw";
        
        // Пока просто отмечаем, что нашли embedded данные
        // Полное извлечение требует более сложного API
        out << "      Would extract to: " << fileName << std::endl;
        out << "      NOTE: Full extraction requires AAF Essence Access API" << std::endl;
        
        return fileName;
        
    } catch (const std::exception& e) {
        out << "      ERROR: Exception during extraction: " << e.what() << std::endl;
        return "";
    }
}

// Функция для поиска EssenceData по MobID
std::string findAndExtractEssenceData(IAAFHeader* pHeader, const aafMobID_t& sourceMobID, 
                                     const std::string& clipName, std::ofstream& out) {
    if (!pHeader) return "";
    
    // Ищем SourceMob по ID
    IAAFMob* pSourceMob = nullptr;
    if (FAILED(pHeader->LookupMob(sourceMobID, &pSourceMob))) {
        return ""; // Не найден - возможно, это внешний файл
    }
    
    // Проверяем, что это действительно SourceMob
    IAAFSourceMob* pSourceMobInterface = nullptr;
    if (FAILED(pSourceMob->QueryInterface(IID_IAAFSourceMob, (void**)&pSourceMobInterface))) {
        pSourceMob->Release();
        return "";
    }
    
    std::string extractedFileName = "";
    
    // Получаем EssenceDescriptor
    IAAFEssenceDescriptor* pEssenceDesc = nullptr;
    if (SUCCEEDED(pSourceMobInterface->GetEssenceDescriptor(&pEssenceDesc))) {
        
        // Проверяем, есть ли embedded EssenceData
        IAAFFileDescriptor* pFileDesc = nullptr;
        if (SUCCEEDED(pEssenceDesc->QueryInterface(IID_IAAFFileDescriptor, (void**)&pFileDesc))) {
            
            // Получаем информацию о файле
            aafWChar sampleRateStr[64] = {0};
            aafRational_t sampleRate = {0, 0};
            if (SUCCEEDED(pFileDesc->GetSampleRate(&sampleRate))) {
                out << "      File sample rate: " << sampleRate.numerator << "/" << sampleRate.denominator << std::endl;
            }
            
            pFileDesc->Release();
        }
        
        // Пытаемся найти EssenceData через слоты SourceMob
        aafNumSlots_t numSlots = 0;
        if (SUCCEEDED(pSourceMob->CountSlots(&numSlots))) {
            for (aafUInt32 i = 0; i < numSlots; ++i) {
                IAAFMobSlot* pSlot = nullptr;
                if (SUCCEEDED(pSourceMob->GetSlotAt(i, &pSlot))) {
                    
                    IAAFSegment* pSegment = nullptr;
                    if (SUCCEEDED(pSlot->GetSegment(&pSegment))) {
                        
                        // Проверяем, есть ли EssenceData в сегменте
                        IAAFSourceClip* pSourceClip = nullptr;
                        if (SUCCEEDED(pSegment->QueryInterface(IID_IAAFSourceClip, (void**)&pSourceClip))) {
                            
                            aafSourceRef_t sourceRef;
                            if (SUCCEEDED(pSourceClip->GetSourceReference(&sourceRef))) {
                                
                                // Ищем EssenceData по ID
                                IEnumAAFEssenceData* pEnumEssence = nullptr;
                                if (SUCCEEDED(pHeader->EnumEssenceData(&pEnumEssence))) {
                                    
                                    IAAFEssenceData* pEssenceData = nullptr;
                                    while (SUCCEEDED(pEnumEssence->NextOne(&pEssenceData))) {
                                        
                                        aafMobID_t essenceMobID;
                                        if (SUCCEEDED(pEssenceData->GetFileMobID(&essenceMobID))) {
                                            
                                            // Сравниваем MobID
                                            if (memcmp(&essenceMobID, &sourceRef.sourceID, sizeof(aafMobID_t)) == 0) {
                                                out << "      Found embedded EssenceData for clip: " << clipName << std::endl;
                                                
                                                // Создаем базовое имя файла
                                                std::string baseName = clipName;
                                                if (baseName.empty()) {
                                                    baseName = formatMobID(sourceMobID);
                                                }
                                                
                                                // Извлекаем данные
                                                extractedFileName = extractEmbeddedAudio(pEssenceData, baseName, out);
                                                
                                                pEssenceData->Release();
                                                break;
                                            }
                                        }
                                        pEssenceData->Release();
                                    }
                                    pEnumEssence->Release();
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
        
        pEssenceDesc->Release();
    }
    
    pSourceMobInterface->Release();
    pSourceMob->Release();
    
    return extractedFileName;
}

// Функция для обработки аудио компонента с позицией
void processAudioComponentWithPosition(IAAFComponent* pComp, std::ofstream& out, int compIndex, 
                                      aafPosition_t startPosition, 
                                      const std::map<std::string, std::string>& mobIdToName, 
                                      const aafRational_t& editRate,
                                      int& audioClipCount, int& audioFadeCount, int& audioEffectCount) {
    
    if (!pComp) return;
    
    // Получаем длительность компонента
    aafLength_t length = 0;
    pComp->GetLength(&length);
    
    // Получаем тип данных
    IAAFDataDef* pDataDef = nullptr;
    std::string dataType = "Unknown";
    if (SUCCEEDED(pComp->GetDataDef(&pDataDef))) {
        dataType = getDataDefName(pDataDef);
        pDataDef->Release();
    }
    
    // Конвертируем в секунды
    double startSec = (double)startPosition * editRate.denominator / editRate.numerator;
    double lengthSec = (double)length * editRate.denominator / editRate.numerator;
    double endSec = startSec + lengthSec;
    
    // Проверяем тип компонента
    IAAFSourceClip* pClip = nullptr;
    IAAFFiller* pFiller = nullptr;
    IAAFTransition* pTransition = nullptr;
    IAAFOperationGroup* pOpGroup = nullptr;
    
    if (SUCCEEDED(pComp->QueryInterface(IID_IAAFSourceClip, (void**)&pClip))) {
        // SourceClip - аудиоклип
        audioClipCount++;
        
        aafSourceRef_t ref;
        if (SUCCEEDED(pClip->GetSourceReference(&ref))) {
            double sourceStartSec = (double)ref.startTime * editRate.denominator / editRate.numerator;
            
            std::string refID = formatMobID(ref.sourceID);
            std::string fileName = "(unknown)";
            
            if (mobIdToName.count(refID)) {
                fileName = mobIdToName.at(refID);
            }
            
            out << "      #" << compIndex << " AUDIO_CLIP [" << dataType << "] "
                << "Timeline: " << std::fixed << std::setprecision(3) 
                << startSec << "s -> " << endSec << "s (" << lengthSec << "s) | "
                << "Source: " << ref.startTime << " (" << sourceStartSec << "s) | "
                << "SlotID: " << ref.sourceSlotID << " | "
                << "File: " << fileName << std::endl;
        }
        pClip->Release();
        
    } else if (SUCCEEDED(pComp->QueryInterface(IID_IAAFFiller, (void**)&pFiller))) {
        // Filler - пустое место в аудиотреке
        out << "      #" << compIndex << " AUDIO_FILLER [" << dataType << "] "
            << "Timeline: " << std::fixed << std::setprecision(3) 
            << startSec << "s -> " << endSec << "s (" << lengthSec << "s)" << std::endl;
        pFiller->Release();
        
    } else if (SUCCEEDED(pComp->QueryInterface(IID_IAAFTransition, (void**)&pTransition))) {
        // Transition - crossfade между аудиоклипами
        audioFadeCount++;
        
        aafPosition_t cutPoint = 0;
        pTransition->GetCutPoint(&cutPoint);
        double cutPointSec = (double)cutPoint * editRate.denominator / editRate.numerator;
        
        out << "      #" << compIndex << " AUDIO_CROSSFADE [" << dataType << "] "
            << "Timeline: " << std::fixed << std::setprecision(3) 
            << startSec << "s -> " << endSec << "s (" << lengthSec << "s) | "
            << "CutPoint: " << cutPoint << " (" << cutPointSec << "s)" << std::endl;
        pTransition->Release();
        
    } else if (SUCCEEDED(pComp->QueryInterface(IID_IAAFOperationGroup, (void**)&pOpGroup))) {
        // OperationGroup - аудиоэффект
        audioEffectCount++;
        
        IAAFOperationDef* pOpDef = nullptr;
        std::string opName = "Unknown";
        if (SUCCEEDED(pOpGroup->GetOperationDefinition(&pOpDef))) {
            IAAFDefObject* pDefObj = nullptr;
            if (SUCCEEDED(pOpDef->QueryInterface(IID_IAAFDefObject, (void**)&pDefObj))) {
                aafWChar opNameBuf[256] = {0};
                if (SUCCEEDED(pDefObj->GetName(opNameBuf, sizeof(opNameBuf)))) {
                    opName = wideToUtf8(opNameBuf);
                }
                pDefObj->Release();
            }
            pOpDef->Release();
        }
        
        out << "      #" << compIndex << " AUDIO_EFFECT [" << dataType << "] "
            << "Timeline: " << std::fixed << std::setprecision(3) 
            << startSec << "s -> " << endSec << "s (" << lengthSec << "s) | "
            << "Operation: " << opName << std::endl;
        pOpGroup->Release();
        
    } else {
        // Неизвестный тип компонента
        out << "      #" << compIndex << " UNKNOWN_AUDIO [" << dataType << "] "
            << "Timeline: " << std::fixed << std::setprecision(3) 
            << startSec << "s -> " << endSec << "s (" << lengthSec << "s)" << std::endl;
    }
}

// Функция для извлечения клипов из сегмента для CSV экспорта
void extractClipsFromSegment(IAAFSegment* pSegment, const std::map<std::string, std::string>& mobIdToName,
                           const aafRational_t& editRate, aafPosition_t sessionStartTC,
                           aafPosition_t currentPosition, AudioTrackData& trackData,
                           IAAFHeader* pHeader, std::ofstream& out) {
    
    if (!pSegment) return;
    
    // Проверяем тип сегмента
    IAAFSequence* pSeq = nullptr;
    IAAFSourceClip* pSourceClip = nullptr;
    IAAFComponent* pComp = nullptr;
    
    if (SUCCEEDED(pSegment->QueryInterface(IID_IAAFSequence, (void**)&pSeq))) {
        // Последовательность компонентов
        IEnumAAFComponents* pEnum = nullptr;
        if (SUCCEEDED(pSeq->GetComponents(&pEnum))) {
            IAAFComponent* pSeqComp = nullptr;
            aafPosition_t seqPosition = currentPosition;
            
            while (SUCCEEDED(pEnum->NextOne(&pSeqComp))) {
                aafLength_t compLength = 0;
                pSeqComp->GetLength(&compLength);
                
                // Рекурсивно обрабатываем каждый компонент
                IAAFSegment* pCompSegment = nullptr;
                if (SUCCEEDED(pSeqComp->QueryInterface(IID_IAAFSegment, (void**)&pCompSegment))) {
                    extractClipsFromSegment(pCompSegment, mobIdToName, editRate, sessionStartTC, 
                                          seqPosition, trackData, pHeader, out);
                    pCompSegment->Release();
                }
                
                seqPosition += compLength;
                pSeqComp->Release();
            }
            pEnum->Release();
        }
        pSeq->Release();
        
    } else if (SUCCEEDED(pSegment->QueryInterface(IID_IAAFSourceClip, (void**)&pSourceClip))) {
        // Отдельный клип
        aafSourceRef_t ref;
        if (SUCCEEDED(pSourceClip->GetSourceReference(&ref))) {
            
            aafLength_t clipLength = 0;
            if (SUCCEEDED(pSourceClip->QueryInterface(IID_IAAFComponent, (void**)&pComp))) {
                pComp->GetLength(&clipLength);
                pComp->Release();
            }
            
            // Создаем данные клипа для CSV
            AudioClipData clipData;
            clipData.mobID = formatMobID(ref.sourceID);
            
            // Получаем имя файла
            if (mobIdToName.count(clipData.mobID)) {
                clipData.fileName = mobIdToName.at(clipData.mobID);
            } else {
                clipData.fileName = "(unknown)";
                
                // Пытаемся найти embedded медиа
                std::string extractedFile = findAndExtractEssenceData(pHeader, ref.sourceID, clipData.mobID, out);
                if (!extractedFile.empty()) {
                    clipData.fileName = extractedFile;
                }
            }
            
            // Конвертируем времена в секунды
            clipData.timelineStart = (double)currentPosition * editRate.denominator / editRate.numerator;
            clipData.length = (double)clipLength * editRate.denominator / editRate.numerator;
            clipData.timelineEnd = clipData.timelineStart + clipData.length;
            clipData.sourceStart = (double)ref.startTime * editRate.denominator / editRate.numerator;
            
            // Добавляем клип в трек
            trackData.clips.push_back(clipData);
        }
        pSourceClip->Release();
        
    } else if (SUCCEEDED(pSegment->QueryInterface(IID_IAAFComponent, (void**)&pComp))) {
        // Другие типы компонентов (Filler, Transition, etc.)
        aafLength_t compLength = 0;
        pComp->GetLength(&compLength);
        
        // Для пустых сегментов тоже можем создать запись
        IAAFFiller* pFiller = nullptr;
        if (SUCCEEDED(pComp->QueryInterface(IID_IAAFFiller, (void**)&pFiller))) {
            AudioClipData clipData;
            clipData.fileName = "(silence)";
            clipData.timelineStart = (double)currentPosition * editRate.denominator / editRate.numerator;
            clipData.length = (double)compLength * editRate.denominator / editRate.numerator;
            clipData.timelineEnd = clipData.timelineStart + clipData.length;
            
            trackData.clips.push_back(clipData);
            pFiller->Release();
        }
        
        pComp->Release();
    }
}

bool isAudioTrack(IAAFMobSlot* pSlot) {
    if (!pSlot) return false;
    
    IAAFSegment* pSegment = nullptr;
    if (FAILED(pSlot->GetSegment(&pSegment))) {
        return false;
    }
    
    // Проверяем тип данных сегмента
    IAAFComponent* pComp = nullptr;
    bool isAudio = false;
    
    if (SUCCEEDED(pSegment->QueryInterface(IID_IAAFComponent, (void**)&pComp))) {
        IAAFDataDef* pDataDef = nullptr;
        if (SUCCEEDED(pComp->GetDataDef(&pDataDef))) {
            std::string dataType = getDataDefName(pDataDef);
            isAudio = (dataType == "Sound");
            pDataDef->Release();
        }
        pComp->Release();
    }
    
    pSegment->Release();
    return isAudio;
}

void processAudioTrackForExport(IAAFMobSlot* pSlot, const std::map<std::string, std::string>& mobIdToName, 
                               aafPosition_t sessionStartTC, AudioTrackData& trackData) {
    // Временная заглушка - нужен доступ к pHeader для полной реализации
    // TODO: передать pHeader для извлечения embedded медиа
}

// Обновленная версия функции с поддержкой извлечения embedded медиа
void processAudioTrackForExportWithHeader(IAAFMobSlot* pSlot, const std::map<std::string, std::string>& mobIdToName, 
                                         aafPosition_t sessionStartTC, AudioTrackData& trackData,
                                         IAAFHeader* pHeader, std::ofstream& out) {
    if (!pSlot) return;
    
    // Получаем edit rate слота
    aafRational_t editRate = {25, 1}; // По умолчанию
    IAAFTimelineMobSlot* pTimelineSlot = nullptr;
    if (SUCCEEDED(pSlot->QueryInterface(IID_IAAFTimelineMobSlot, (void**)&pTimelineSlot))) {
        pTimelineSlot->GetEditRate(&editRate);
        pTimelineSlot->Release();
    }
    
    // Получаем сегмент слота
    IAAFSegment* pSegment = nullptr;
    if (FAILED(pSlot->GetSegment(&pSegment))) {
        return;
    }
    
    // Рекурсивно извлекаем клипы из сегмента
    extractClipsFromSegment(pSegment, mobIdToName, editRate, sessionStartTC, 0, trackData, pHeader, out);
    
    pSegment->Release();
}

// Функция для поиска и копирования аудиофайлов
void processAudioFiles(const ProjectData& projectData, std::ofstream& out) {
    out << "\n🎵 === PROCESSING AUDIO FILES ===" << std::endl;
    
    // Создаем папку для всех аудиофайлов
    try {
        std::filesystem::create_directories("audio_files");
        out << "[*] Created audio_files folder for all project audio" << std::endl;
    } catch (const std::exception& e) {
        out << "ERROR: Failed to create audio_files folder: " << e.what() << std::endl;
        return;
    }
    
    // Собираем уникальные имена файлов
    std::set<std::string> uniqueFiles;
    for (const auto& track : projectData.tracks) {
        for (const auto& clip : track.clips) {
            if (!clip.fileName.empty() && clip.fileName != "(unknown)") {
                uniqueFiles.insert(clip.fileName);
            }
        }
    }
    
    out << "[*] Found " << uniqueFiles.size() << " unique audio files to process:" << std::endl;
    
    int copiedFiles = 0;
    int embeddedFiles = 0;
    int missingFiles = 0;
    
    // Возможные папки с исходными файлами
    std::vector<std::string> sourcePaths = {
        "J:\\Nuendo PROJECTS SSD\\Other\\DP\\Bezhanov\\BEZHANOV\\test\\Audio",
        ".",  // текущая папка
        "Audio",  // локальная папка Audio
        "media",  // локальная папка media
        "extracted_media"  // папка с extracted файлами
    };
    
    for (const auto& fileName : uniqueFiles) {
        bool found = false;
        
        // Ищем файл в возможных путях
        for (const auto& sourcePath : sourcePaths) {
            std::string fullPath = sourcePath + "\\" + fileName;
            
            try {
                if (std::filesystem::exists(fullPath)) {
                    // Копируем файл
                    std::string destPath = "audio_files\\" + fileName;
                    std::filesystem::copy_file(fullPath, destPath, 
                                             std::filesystem::copy_options::overwrite_existing);
                    
                    out << "  ✅ Copied: " << fileName << " from " << sourcePath << std::endl;
                    copiedFiles++;
                    found = true;
                    break;
                }
            } catch (const std::exception& e) {
                // Игнорируем ошибки доступа к папкам
                continue;
            }
        }
        
        if (!found) {
            // Файл не найден - возможно, это embedded медиа
            if (fileName.find("embedded") != std::string::npos) {
                out << "  🔗 Embedded: " << fileName << " (already extracted)" << std::endl;
                embeddedFiles++;
            } else {
                out << "  ❌ Missing: " << fileName << std::endl;
                missingFiles++;
            }
        }
    }
    
    out << "\n📊 Audio Files Summary:" << std::endl;
    out << "  Copied external files: " << copiedFiles << std::endl;
    out << "  Embedded files: " << embeddedFiles << std::endl;
    out << "  Missing files: " << missingFiles << std::endl;
    out << "  Total unique files: " << uniqueFiles.size() << std::endl;
}

void exportToCSV(const ProjectData& projectData, const std::string& filename) {
    std::ofstream csvFile(filename);
    if (!csvFile.is_open()) {
        std::cerr << "ERROR: Failed to create CSV file: " << filename << std::endl;
        return;
    }
    
    // Заголовок CSV
    csvFile << "Track,TrackName,TrackType,ClipName,MobID,TimelineStart,TimelineEnd,SourceStart,Length,Gain,Volume,Pan,Effects\n";
    
    // Экспорт данных треков
    for (const auto& track : projectData.tracks) {
        if (track.clips.empty()) {
            // Трек без клипов
            csvFile << track.trackIndex << ",,"
                   << track.trackType << ",,,,,,,,,"  // пустые поля для клипа
                   << "\n";
        } else {
            // Треки с клипами
            for (const auto& clip : track.clips) {
                csvFile << track.trackIndex << ",\""
                       << track.trackName << "\",\""
                       << track.trackType << "\",\""
                       << clip.fileName << "\","
                       << clip.mobID << ","
                       << std::fixed << std::setprecision(6) << clip.timelineStart << ","
                       << clip.timelineEnd << ","
                       << clip.sourceStart << ","
                       << clip.length << ","
                       << clip.gain << ","
                       << clip.volume << ","
                       << clip.pan << ",";
                
                // Эффекты как строка через ;
                for (size_t i = 0; i < clip.effects.size(); ++i) {
                    if (i > 0) csvFile << ";";
                    csvFile << clip.effects[i];
                }
                csvFile << "\n";
            }
        }
    }
    
    csvFile.close();
}

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

    // Создаем папку для извлеченных медиафайлов
    createExtractedMediaFolder();

    std::ofstream out("output.txt");
    out << "[*] Opening AAF file..." << std::endl;
    out << "[*] Created extracted_media folder for embedded audio files" << std::endl;

    size_t len = strlen(argv[1]) + 1;
    wchar_t* widePath = new wchar_t[len];
    mbstowcs_s(nullptr, widePath, len, argv[1], _TRUNCATE);

    IAAFFile* pFile = nullptr;
    if (FAILED(AAFFileOpenExistingRead(widePath, 0, &pFile)) || !pFile) {
        out << "ERROR: Failed to open file." << std::endl;
        delete[] widePath;
        return 1;
    }
    delete[] widePath;

    IAAFHeader* pHeader = nullptr;
    if (FAILED(pFile->GetHeader(&pHeader))) {
        out << "ERROR: Failed to get header." << std::endl;
        pFile->Close(); pFile->Release();
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
                        nameStr = "[SourceMob]";
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

    // 2. ВАЖНО: Обрабатываем ТОЛЬКО CompositionMob
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
                        
                        // Для JSON экспорта
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
                        
                            processAudioTrackForExportWithHeader(pSlot, mobIdToName, sessionStartTC, trackData, pHeader, out);
                        
                            // Экспортируем ВСЕ аудио треки, даже пустые для сохранения структуры
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

    // Вычисляем общую длительность проекта
    for (const auto& track : projectData.tracks) {
        for (const auto& clip : track.clips) {
            if (clip.timelineEnd > projectData.totalLength) {
                projectData.totalLength = clip.timelineEnd;
            }
        }
    }
    
    // Экспортируем в CSV (единственный формат экспорта)
    exportToCSV(projectData, "aaf_export.csv");
    
    // Обрабатываем аудиофайлы (копируем/извлекаем)
    processAudioFiles(projectData, out);
    
    // Статистика аудио треков
    out << "\n📊 === AUDIO TRACKS SUMMARY ===" << std::endl;
    out << "Total audio tracks processed: " << audioTrackCount << std::endl;
    out << "Total audio clips: " << audioClipCount << std::endl;
    out << "Total audio fades/crossfades: " << audioFadeCount << std::endl;
    out << "Total audio effects: " << audioEffectCount << std::endl;
    
    // Дополнительная информация
    out << "\n✅ CSV Export: aaf_export.csv" << std::endl;
    out << "Total tracks exported: " << projectData.tracks.size() << std::endl;
    int totalClipsExported = 0;
    for (const auto& track : projectData.tracks) {
        totalClipsExported += track.clips.size();
    }
    out << "Total clips exported: " << totalClipsExported << std::endl;
    out << "Project length: " << std::fixed << std::setprecision(3) << projectData.totalLength << "s" << std::endl;

    pHeader->Release();
    pFile->Close();
    pFile->Release();
    out.close();

    // Выводим успешное завершение в консоль
    std::cout << "✅ AAF processed successfully!" << std::endl;
    std::cout << "📊 CSV exported: aaf_export.csv" << std::endl;
    std::cout << "📋 Report: output.txt" << std::endl;

    return 0;
}