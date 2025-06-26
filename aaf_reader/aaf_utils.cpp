#include "aaf_utils.h"
#include <locale>
#include <codecvt>
#include <cstdio>
#include <iostream>

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
             uid.Data4[0], uid.Data4[1], uid.Data4[2], uid.Data4[3], 
             uid.Data4[4], uid.Data4[5], uid.Data4[6], uid.Data4[7]);
    return std::string(buffer);
}

std::string formatTimecode(aafPosition_t position, const aafRational_t& editRate) {
    if (editRate.denominator == 0) return "00:00:00:00";
    
    double framesPerSecond = (double)editRate.numerator / editRate.denominator;
    aafPosition_t totalFrames = position;
    
    aafPosition_t hours = totalFrames / (aafPosition_t)(framesPerSecond * 3600);
    totalFrames %= (aafPosition_t)(framesPerSecond * 3600);
    
    aafPosition_t minutes = totalFrames / (aafPosition_t)(framesPerSecond * 60);
    totalFrames %= (aafPosition_t)(framesPerSecond * 60);
    
    aafPosition_t seconds = totalFrames / (aafPosition_t)framesPerSecond;
    aafPosition_t frames = totalFrames % (aafPosition_t)framesPerSecond;
    
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%02lld:%02lld:%02lld:%02lld", 
             hours, minutes, seconds, frames);
    return std::string(buffer);
}

aafPosition_t getCompositionStartTimecode(IAAFMob* pMob) {
    aafPosition_t startTC = 0;
    
    // Ищем Timecode слот
    aafNumSlots_t numSlots = 0;
    if (SUCCEEDED(pMob->CountSlots(&numSlots))) {
        for (aafUInt32 i = 0; i < numSlots; ++i) {
            IAAFMobSlot* pSlot = nullptr;
            if (SUCCEEDED(pMob->GetSlotAt(i, &pSlot))) {
                
                // Проверяем тип данных слота
                IAAFSegment* pSegment = nullptr;
                if (SUCCEEDED(pSlot->GetSegment(&pSegment))) {
                    IAAFComponent* pComp = nullptr;
                    if (SUCCEEDED(pSegment->QueryInterface(IID_IAAFComponent, (void**)&pComp))) {
                        IAAFDataDef* pDataDef = nullptr;
                        if (SUCCEEDED(pComp->GetDataDef(&pDataDef))) {
                            // Используем простую проверку имени вместо IsTimecodeKind
                            // так как эти методы могут быть недоступны в базовом интерфейсе
                            
                            // Если это TimecodeStream, получаем стартовую позицию
                            IAAFTimecodeStream* pTCStream = nullptr;
                            if (SUCCEEDED(pSegment->QueryInterface(IID_IAAFTimecodeStream, (void**)&pTCStream))) {
                                // Получаем origin слота как начальное время
                                IAAFTimelineMobSlot* pTimelineSlot = nullptr;
                                if (SUCCEEDED(pSlot->QueryInterface(IID_IAAFTimelineMobSlot, (void**)&pTimelineSlot))) {
                                    pTimelineSlot->GetOrigin(&startTC);
                                    pTimelineSlot->Release();
                                }
                                pTCStream->Release();
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

bool isAudioTrack(IAAFMobSlot* pSlot) {
    IAAFSegment* pSegment = nullptr;
    if (FAILED(pSlot->GetSegment(&pSegment))) {
        return false;
    }
    
    IAAFComponent* pComp = nullptr;
    bool result = false;
    
    if (SUCCEEDED(pSegment->QueryInterface(IID_IAAFComponent, (void**)&pComp))) {
        IAAFDataDef* pDataDef = nullptr;
        if (SUCCEEDED(pComp->GetDataDef(&pDataDef))) {
            aafBoolean_t isSoundKind = kAAFFalse;
            if (SUCCEEDED(pDataDef->IsSoundKind(&isSoundKind))) {
                result = (isSoundKind == kAAFTrue);
            }
            pDataDef->Release();
        }
        pComp->Release();
    }
    
    pSegment->Release();
    return result;
}

std::string getDataDefName(IAAFDataDef* pDataDef) {
    if (!pDataDef) return "Unknown";
    
    aafBoolean_t result = kAAFFalse;
    
    // Проверяем, является ли это Sound DataDef
    if (SUCCEEDED(pDataDef->IsSoundKind(&result)) && result) {
        return "Sound";
    }
    
    // Проверяем, является ли это Picture DataDef  
    if (SUCCEEDED(pDataDef->IsPictureKind(&result)) && result) {
        return "Picture";
    }
    
    // Проверяем, является ли это Matte DataDef
    if (SUCCEEDED(pDataDef->IsMatteKind(&result)) && result) {
        return "Matte";
    }
    
    // Проверяем, является ли это PictureWithMatte DataDef
    if (SUCCEEDED(pDataDef->IsPictureWithMatteKind(&result)) && result) {
        return "PictureWithMatte";
    }
    
    return "Other";
}
