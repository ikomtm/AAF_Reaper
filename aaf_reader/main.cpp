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

// Объявления функций для экспорта
void extractClipsFromSegment(IAAFSegment* pSegment, const std::map<std::string, std::string>& mobIdToName,
                           const aafRational_t& editRate, aafPosition_t sessionStartTC, 
                           aafPosition_t currentPosition, AudioTrackData& trackData, 
                           IAAFHeader* pHeader, std::ofstream& out);

void extractClipFromComponent(IAAFComponent* pComp, const std::map<std::string, std::string>& mobIdToName,
                            const aafRational_t& editRate, aafPosition_t sessionStartTC,
                            aafPosition_t position, AudioTrackData& trackData,
                            IAAFHeader* pHeader, std::ofstream& out);

// Структура для хранения аудио параметров
struct AudioParams {
    bool hasGain = false;
    double gainValue = 0.0;
    bool hasPan = false;
    double panValue = 0.0;
    bool hasVolume = false;
    double volumeValue = 1.0;
    std::string effectName = "";
};

// Функция для извлечения параметров из OperationGroup
AudioParams extractAudioParams(IAAFOperationGroup* pOpGroup) {
    AudioParams params;
    
    // Получаем название операции
    IAAFOperationDef* pOpDef = nullptr;
    if (SUCCEEDED(pOpGroup->GetOperationDefinition(&pOpDef))) {
        IAAFDefObject* pDefObj = nullptr;
        if (SUCCEEDED(pOpDef->QueryInterface(IID_IAAFDefObject, (void**)&pDefObj))) {
            aafWChar opNameBuf[256] = {0};
            if (SUCCEEDED(pDefObj->GetName(opNameBuf, sizeof(opNameBuf)))) {
                params.effectName = wideToUtf8(opNameBuf);
            }
            pDefObj->Release();
        }
        pOpDef->Release();
    }
    
    // Получаем параметры операции через enumerator
    aafUInt32 numParams = 0;
    if (SUCCEEDED(pOpGroup->CountParameters(&numParams)) && numParams > 0) {
        IEnumAAFParameters* pParamEnum = nullptr;
        if (SUCCEEDED(pOpGroup->GetParameters(&pParamEnum))) {
            IAAFParameter* pParam = nullptr;
            
            while (SUCCEEDED(pParamEnum->NextOne(&pParam))) {
                // Получаем определение параметра
                IAAFParameterDef* pParamDef = nullptr;
                if (SUCCEEDED(pParam->GetParameterDefinition(&pParamDef))) {
                    IAAFDefObject* pParamDefObj = nullptr;
                    if (SUCCEEDED(pParamDef->QueryInterface(IID_IAAFDefObject, (void**)&pParamDefObj))) {
                        aafWChar paramNameBuf[256] = {0};
                        if (SUCCEEDED(pParamDefObj->GetName(paramNameBuf, sizeof(paramNameBuf)))) {
                            std::string paramName = wideToUtf8(paramNameBuf);
                            
                            // Получаем значение параметра
                            IAAFConstantValue* pConstValue = nullptr;
                            if (SUCCEEDED(pParam->QueryInterface(IID_IAAFConstantValue, (void**)&pConstValue))) {
                                aafUInt32 valueSize = 0;
                                if (SUCCEEDED(pConstValue->GetValueBufLen(&valueSize)) && valueSize > 0) {
                                    aafUInt8* valueBuffer = new aafUInt8[valueSize];
                                    if (SUCCEEDED(pConstValue->GetValue(valueSize, valueBuffer, &valueSize))) {
                                        
                                        // Интерпретируем значение в зависимости от типа параметра
                                        if (paramName.find("Gain") != std::string::npos || 
                                            paramName.find("Level") != std::string::npos) {
                                            if (valueSize >= sizeof(float)) {
                                                params.hasGain = true;
                                                params.gainValue = *((float*)valueBuffer);
                                            } else if (valueSize >= sizeof(double)) {
                                                params.hasGain = true;
                                                params.gainValue = *((double*)valueBuffer);
                                            }
                                        } else if (paramName.find("Pan") != std::string::npos) {
                                            if (valueSize >= sizeof(float)) {
                                                params.hasPan = true;
                                                params.panValue = *((float*)valueBuffer);
                                            } else if (valueSize >= sizeof(double)) {
                                                params.hasPan = true;
                                                params.panValue = *((double*)valueBuffer);
                                            }
                                        } else if (paramName.find("Volume") != std::string::npos) {
                                            if (valueSize >= sizeof(float)) {
                                                params.hasVolume = true;
                                                params.volumeValue = *((float*)valueBuffer);
                                            } else if (valueSize >= sizeof(double)) {
                                                params.hasVolume = true;
                                                params.volumeValue = *((double*)valueBuffer);
                                            }
                                        }
                                    }
                                    delete[] valueBuffer;
                                }
                                pConstValue->Release();
                            }
                        }
                        pParamDefObj->Release();
                    }
                    pParamDef->Release();
                }
                pParam->Release();
            }
            pParamEnum->Release();
        }
    }
    
    return params;
}

// Функция для обработки слота в деталях
void processSlotInDetail(IAAFMobSlot* pSlot, std::ofstream& out, int slotIndex, 
                        const std::map<std::string, std::string>& mobIdToName) {
    
    aafSlotID_t slotID = 0;
    pSlot->GetSlotID(&slotID);
    
    aafWChar slotName[256] = {0};
    pSlot->GetName(slotName, sizeof(slotName));
    std::string slotNameStr = wideToUtf8(slotName);
    
    // Определяем тип слота и editRate
    aafRational_t editRate = {25, 1}; // default
    std::string slotType = "MobSlot";
    
    IAAFTimelineMobSlot* pTimelineSlot = nullptr;
    if (SUCCEEDED(pSlot->QueryInterface(IID_IAAFTimelineMobSlot, (void**)&pTimelineSlot))) {
        slotType = "TimelineSlot";
        pTimelineSlot->GetEditRate(&editRate);
        
        aafPosition_t origin = 0;
        pTimelineSlot->GetOrigin(&origin);
        
        out << "  - Track #" << slotIndex << " [" << slotType << "] ID=" << slotID;
        if (!slotNameStr.empty()) {
            out << ", Name='" << slotNameStr << "'";
        }
        out << ", EditRate=" << editRate.numerator << "/" << editRate.denominator;
        out << ", Origin=" << origin << std::endl;
        
        pTimelineSlot->Release();
    }
    
    // Получаем сегмент
    IAAFSegment* pSegment = nullptr;
    if (SUCCEEDED(pSlot->GetSegment(&pSegment))) {
        
        IAAFSequence* pSeq = nullptr;
        IAAFSourceClip* pSourceClip = nullptr;
        
        if (SUCCEEDED(pSegment->QueryInterface(IID_IAAFSequence, (void**)&pSeq))) {
            out << "    Sequence with components:" << std::endl;
            
            IEnumAAFComponents* pEnum = nullptr;
            if (SUCCEEDED(pSeq->GetComponents(&pEnum))) {
                IAAFComponent* pComp = nullptr;
                int compIndex = 0;
                
                while (SUCCEEDED(pEnum->NextOne(&pComp))) {
                    processComponent(pComp, out, compIndex++, mobIdToName, editRate);
                    pComp->Release();
                }
                pEnum->Release();
            }
            pSeq->Release();
            
        } else if (SUCCEEDED(pSegment->QueryInterface(IID_IAAFSourceClip, (void**)&pSourceClip))) {
            out << "    Single SourceClip:" << std::endl;
            
            IAAFComponent* pComp = nullptr;
            if (SUCCEEDED(pSourceClip->QueryInterface(IID_IAAFComponent, (void**)&pComp))) {
                processComponent(pComp, out, 0, mobIdToName, editRate);
                pComp->Release();
            }
            pSourceClip->Release();
            
        } else {
            out << "    Unknown segment type" << std::endl;
        }
        
        pSegment->Release();
    }
}
void processAudioComponentWithPosition(IAAFComponent* pComp, std::ofstream& out, int compIndex, 
                                      aafPosition_t startPosition, 
                                      const std::map<std::string, std::string>& mobIdToName, 
                                      const aafRational_t& editRate,
                                      int& audioClipCount, int& audioFadeCount, int& audioEffectCount) {
    
    aafLength_t length = 0;
    pComp->GetLength(&length);
    
    // Конвертируем в секунды
    double startSec = (double)startPosition * editRate.denominator / editRate.numerator;
    double lengthSec = (double)length * editRate.denominator / editRate.numerator;
    double endSec = startSec + lengthSec;
    
    // Проверяем тип аудио компонента
    IAAFSourceClip* pClip = nullptr;
    IAAFFiller* pFiller = nullptr;
    IAAFTransition* pTransition = nullptr;
    IAAFOperationGroup* pOpGroup = nullptr;
    
    if (SUCCEEDED(pComp->QueryInterface(IID_IAAFSourceClip, (void**)&pClip))) {
        // Аудио клип
        audioClipCount++;
        
        aafSourceRef_t ref;
        if (SUCCEEDED(pClip->GetSourceReference(&ref))) {
            double sourceStartSec = (double)ref.startTime * editRate.denominator / editRate.numerator;
            
            std::string refID = formatMobID(ref.sourceID);
            std::string fileName = "(unknown)";
            if (mobIdToName.count(refID)) {
                fileName = mobIdToName.at(refID);
            }
            
            out << "      📻 #" << compIndex << " AUDIO_CLIP "
                << "Timeline: " << std::fixed << std::setprecision(3) 
                << startSec << "s -> " << endSec << "s (" << lengthSec << "s) | "
                << "Source: " << ref.startTime << " (" << sourceStartSec << "s) | "
                << "File: " << fileName << std::endl;
        }
        pClip->Release();
        
    } else if (SUCCEEDED(pComp->QueryInterface(IID_IAAFFiller, (void**)&pFiller))) {
        // Аудио переходы/тишина
        audioFadeCount++;
        
        out << "      🔇 #" << compIndex << " AUDIO_FADE/SILENCE "
            << "Timeline: " << std::fixed << std::setprecision(3) 
            << startSec << "s -> " << endSec << "s (" << lengthSec << "s)" << std::endl;
        pFiller->Release();
        
    } else if (SUCCEEDED(pComp->QueryInterface(IID_IAAFTransition, (void**)&pTransition))) {
        // Аудио переходы (CrossFade)
        audioFadeCount++;
        
        aafPosition_t cutPoint = 0;
        pTransition->GetCutPoint(&cutPoint);
        double cutPointSec = (double)cutPoint * editRate.denominator / editRate.numerator;
        
        out << "      🎚️ #" << compIndex << " AUDIO_CROSSFADE "
            << "Timeline: " << std::fixed << std::setprecision(3) 
            << startSec << "s -> " << endSec << "s (" << lengthSec << "s) | "
            << "CutPoint: " << cutPoint << " (" << cutPointSec << "s)" << std::endl;
        pTransition->Release();
        
    } else if (SUCCEEDED(pComp->QueryInterface(IID_IAAFOperationGroup, (void**)&pOpGroup))) {
        // Аудио обработка (игнорируем базовые операции)
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
        
        // Пропускаем базовые аудио операции
        if (opName == "Mono Audio Gain" || opName == "Audio Gain" || opName == "Volume") {
            // Не показываем базовые операции, обрабатываем входные сегменты
            aafUInt32 numInputs = 0;
            if (SUCCEEDED(pOpGroup->CountSourceSegments(&numInputs)) && numInputs > 0) {
                for (aafUInt32 inputIndex = 0; inputIndex < numInputs; inputIndex++) {
                    IAAFSegment* pInputSegment = nullptr;
                    if (SUCCEEDED(pOpGroup->GetInputSegmentAt(inputIndex, &pInputSegment))) {
                        IAAFComponent* pInputComp = nullptr;
                        if (SUCCEEDED(pInputSegment->QueryInterface(IID_IAAFComponent, (void**)&pInputComp))) {
                            // Рекурсивно обрабатываем входной компонент
                            processAudioComponentWithPosition(pInputComp, out, compIndex, startPosition, 
                                                             mobIdToName, editRate, audioClipCount, audioFadeCount, audioEffectCount);
                            pInputComp->Release();
                        }
                        pInputSegment->Release();
                    }
                }
            }
        } else {
            // Показываем только значимые аудио эффекты
            audioEffectCount++;
            
            aafUInt32 numInputs = 0;
            pOpGroup->CountSourceSegments(&numInputs);
            
            out << "      🎛️ #" << compIndex << " AUDIO_EFFECT "
                << "Timeline: " << std::fixed << std::setprecision(3) 
                << startSec << "s -> " << endSec << "s (" << lengthSec << "s) | "
                << "Effect: " << opName << " | Inputs: " << numInputs << std::endl;
        }
        
        pOpGroup->Release();
        
    } else {
        // Неизвестный аудио компонент
        out << "      ❓ #" << compIndex << " UNKNOWN_AUDIO "
            << "Timeline: " << std::fixed << std::setprecision(3) 
            << startSec << "s -> " << endSec << "s (" << lengthSec << "s)" << std::endl;
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

// Заглушки для функций, которые будут реализованы позже
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


// Функция для извлечения клипов из сегмента
void extractClipsFromSegment(IAAFSegment* pSegment, const std::map<std::string, std::string>& mobIdToName,
                           const aafRational_t& editRate, aafPosition_t sessionStartTC, 
                           aafPosition_t currentPosition, AudioTrackData& trackData, 
                           IAAFHeader* pHeader, std::ofstream& out) {
    if (!pSegment) return;
    
    // Проверяем тип сегмента
    IAAFSequence* pSeq = nullptr;
    IAAFSourceClip* pSourceClip = nullptr;
    IAAFOperationGroup* pOpGroup = nullptr;
    
    if (SUCCEEDED(pSegment->QueryInterface(IID_IAAFSequence, (void**)&pSeq))) {
        // Последовательность - обрабатываем все компоненты
        IEnumAAFComponents* pEnum = nullptr;
        if (SUCCEEDED(pSeq->GetComponents(&pEnum))) {
            IAAFComponent* pComp = nullptr;
            aafPosition_t position = currentPosition;
            
            while (SUCCEEDED(pEnum->NextOne(&pComp))) {
                aafLength_t length = 0;
                pComp->GetLength(&length);
                
                extractClipFromComponent(pComp, mobIdToName, editRate, sessionStartTC, 
                                       position, trackData, pHeader, out);
                
                position += length;
                pComp->Release();
            }
            pEnum->Release();
        }
        pSeq->Release();
        
    } else if (SUCCEEDED(pSegment->QueryInterface(IID_IAAFSourceClip, (void**)&pSourceClip))) {
        // Одиночный клип
        IAAFComponent* pComp = nullptr;
        if (SUCCEEDED(pSourceClip->QueryInterface(IID_IAAFComponent, (void**)&pComp))) {
            extractClipFromComponent(pComp, mobIdToName, editRate, sessionStartTC, 
                                   currentPosition, trackData, pHeader, out);
            pComp->Release();
        }
        pSourceClip->Release();
        
    } else if (SUCCEEDED(pSegment->QueryInterface(IID_IAAFOperationGroup, (void**)&pOpGroup))) {
        // Операция - обрабатываем входные сегменты
        aafUInt32 numInputs = 0;
        if (SUCCEEDED(pOpGroup->CountSourceSegments(&numInputs))) {
            for (aafUInt32 i = 0; i < numInputs; i++) {
                IAAFSegment* pInputSegment = nullptr;
                if (SUCCEEDED(pOpGroup->GetInputSegmentAt(i, &pInputSegment))) {
                    extractClipsFromSegment(pInputSegment, mobIdToName, editRate, 
                                          sessionStartTC, currentPosition, trackData, pHeader, out);
                    pInputSegment->Release();
                }
            }
        }
        pOpGroup->Release();
    }
}

// Функция для извлечения клипа из компонента
void extractClipFromComponent(IAAFComponent* pComp, const std::map<std::string, std::string>& mobIdToName,
                            const aafRational_t& editRate, aafPosition_t sessionStartTC,
                            aafPosition_t position, AudioTrackData& trackData,
                            IAAFHeader* pHeader, std::ofstream& out) {
    if (!pComp) return;
    
    // Проверяем, что это аудио компонент
    IAAFDataDef* pDataDef = nullptr;
    if (SUCCEEDED(pComp->GetDataDef(&pDataDef))) {
        std::string dataType = getDataDefName(pDataDef);
        pDataDef->Release();
        
        if (dataType != "Sound") {
            return; // Не аудио
        }
    }
    
    // Получаем длину
    aafLength_t length = 0;
    pComp->GetLength(&length);
    
    // Конвертируем позиции в секунды
    double timelineStart = (double)position * editRate.denominator / editRate.numerator;
    double lengthSec = (double)length * editRate.denominator / editRate.numerator;
    double timelineEnd = timelineStart + lengthSec;
    
    // Проверяем тип компонента
    IAAFSourceClip* pClip = nullptr;
    IAAFOperationGroup* pOpGroup = nullptr;
    
    if (SUCCEEDED(pComp->QueryInterface(IID_IAAFSourceClip, (void**)&pClip))) {
        // Аудиоклип
        aafSourceRef_t ref;
        if (SUCCEEDED(pClip->GetSourceReference(&ref))) {
            AudioClipData clipData;
            
            // Заполняем данные клипа
            std::string refID = formatMobID(ref.sourceID);
            clipData.mobID = refID;
            
            if (mobIdToName.count(refID)) {
                clipData.fileName = mobIdToName.at(refID);
            } else {
                clipData.fileName = "(unknown)";
            }
            
            clipData.timelineStart = timelineStart;
            clipData.timelineEnd = timelineEnd;
            clipData.length = lengthSec;
            clipData.sourceStart = (double)ref.startTime * editRate.denominator / editRate.numerator;
            
            // Значения по умолчанию
            clipData.gain = 0.0;
            clipData.volume = 1.0;
            clipData.pan = 0.0;
            
            trackData.clips.push_back(clipData);
        }
        pClip->Release();
        
    } else if (SUCCEEDED(pComp->QueryInterface(IID_IAAFOperationGroup, (void**)&pOpGroup))) {
        // Операция - обрабатываем входные сегменты
        aafUInt32 numInputs = 0;
        if (SUCCEEDED(pOpGroup->CountSourceSegments(&numInputs))) {
            for (aafUInt32 i = 0; i < numInputs; i++) {
                IAAFSegment* pInputSegment = nullptr;
                if (SUCCEEDED(pOpGroup->GetInputSegmentAt(i, &pInputSegment))) {
                    extractClipsFromSegment(pInputSegment, mobIdToName, editRate, 
                                          sessionStartTC, position, trackData, pHeader, out);
                    pInputSegment->Release();
                }
            }
        }
        pOpGroup->Release();
    }
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