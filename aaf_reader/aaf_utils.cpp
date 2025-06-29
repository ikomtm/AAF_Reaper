#include "aaf_utils.h"
#include <AAFDataDefs.h>
#include <locale>
#include <codecvt>
#include <cstdio>
#include <iostream>
#include <vector>
#include <cstring>

std::string wideToUtf8(const std::wstring& wstr) {
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    return converter.to_bytes(wstr);
}

// Global formatMobID function - now uses the full formatting function
std::string formatMobID(const aafMobID_t& mobID) {
    return aaf_utils::formatMobIDFull(mobID);
}

// This is the definition for the legacy function.
std::string aaf_utils::formatMobID_Legacy(const aafMobID_t& mobID) {
    const aafUID_t& uid = mobID.material;
    char buffer[64];
    snprintf(buffer, sizeof(buffer), 
             "%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X",
             uid.Data1, uid.Data2, uid.Data3,
             uid.Data4[0], uid.Data4[1], uid.Data4[2], uid.Data4[3], 
             uid.Data4[4], uid.Data4[5], uid.Data4[6], uid.Data4[7]);
    return std::string(buffer);
}

// New function to format the entire MobID into a full UMID string
std::string aaf_utils::formatMobIDFull(const aafMobID_t& mobID) {
    char buffer[128];
    const unsigned char* m = (const unsigned char*)&mobID;
    snprintf(buffer, sizeof(buffer),
             "urn:smpte:umid:%02x%02x%02x%02x.%02x%02x%02x%02x.%02x%02x%02x%02x.%02x%02x%02x%02x.%02x%02x%02x%02x.%02x%02x%02x%02x.%02x%02x%02x%02x.%02x%02x%02x%02x",
             m[0], m[1], m[2], m[3], m[4], m[5], m[6], m[7],
             m[8], m[9], m[10], m[11], m[12], m[13], m[14], m[15],
             m[16], m[17], m[18], m[19], m[20], m[21], m[22], m[23],
             m[24], m[25], m[26], m[27], m[28], m[29], m[30], m[31]);
    return std::string(buffer);
}

// New function to parse a UMID string back into a mobID
bool aaf_utils::parseMobIDFull(const std::string& mobIdStr, aafMobID_t& mobID) {
    std::string hexPart = mobIdStr;

    // Strip URN prefix if it exists
    const std::string urnPrefix = "urn:smpte:umid:";
    if (hexPart.rfind(urnPrefix, 0) == 0) {
        hexPart = hexPart.substr(urnPrefix.length());
    }

    // Remove all dots and hyphens
    hexPart.erase(std::remove(hexPart.begin(), hexPart.end(), '.'), hexPart.end());
    hexPart.erase(std::remove(hexPart.begin(), hexPart.end(), '-'), hexPart.end());

    if (hexPart.length() != 64) { // 32 bytes * 2 hex chars/byte
        return false;
    }

    unsigned char* m = (unsigned char*)&mobID;
    for (size_t i = 0; i < 32; ++i) {
        std::string byteString = hexPart.substr(i * 2, 2);
        m[i] = static_cast<unsigned char>(strtol(byteString.c_str(), NULL, 16));
    }

    return true;
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

void aaf_utils::listAllMobs(IAAFHeader* header, std::ostream& out) {
    if (!header) {
        out << "ERROR: listAllMobs called with null header." << std::endl;
        return;
    }

    IEnumAAFMobs* mobEnum = nullptr;
    if (FAILED(header->GetMobs(nullptr, &mobEnum))) {
        out << "ERROR: Failed to get Mob enumerator." << std::endl;
        return;
    }

    IAAFMob* pMob = nullptr;
    unsigned long count = 0;
    out << "=== Listing all Mobs in AAF file ===" << std::endl;
    while (SUCCEEDED(mobEnum->NextOne(&pMob))) {
        aafMobID_t mobID;
        if (FAILED(pMob->GetMobID(&mobID))) {
            out << "ERROR: Failed to get MobID for a Mob." << std::endl;
            pMob->Release();
            continue;
        }

        // Use the new full formatting function
        std::string mobIDStr = aaf_utils::formatMobIDFull(mobID);

        aafUInt32 nameBufSize = 0;
        pMob->GetNameBufLen(&nameBufSize);
        std::string mobName = "(No Name)";
        if (nameBufSize > 0) {
            std::vector<aafCharacter> nameBuffer(nameBufSize / sizeof(aafCharacter));
            if (SUCCEEDED(pMob->GetName(nameBuffer.data(), nameBufSize))) {
                mobName = wideToUtf8(nameBuffer.data());
            }
        }

        out << "Mob[" << count++ << "]: Name='" << mobName << "', MobID='" << mobIDStr << "'" << std::endl;

        pMob->Release();
    }

    out << "=== End of Mob list ===" << std::endl;
    mobEnum->Release();
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

std::string getFileNameFromEssenceDescriptor(IAAFEssenceDescriptor* pEssenceDesc) {
    if (!pEssenceDesc) return "";
    
    std::string fileName = "";
    
    // Попытаемся получить имя через Locators
    aafUInt32 numLocators = 0;
    if (SUCCEEDED(pEssenceDesc->CountLocators(&numLocators)) && numLocators > 0) {
        IEnumAAFLocators* pLocatorEnum = nullptr;
        if (SUCCEEDED(pEssenceDesc->GetLocators(&pLocatorEnum))) {
            IAAFLocator* pLocator = nullptr;
            if (SUCCEEDED(pLocatorEnum->NextOne(&pLocator))) {
                aafUInt32 pathBufSize = 0;
                if (SUCCEEDED(pLocator->GetPathBufLen(&pathBufSize)) && pathBufSize > 0) {
                    std::vector<aafCharacter> pathBuffer(pathBufSize / sizeof(aafCharacter));
                    if (SUCCEEDED(pLocator->GetPath(pathBuffer.data(), pathBufSize))) {
                        std::wstring path(pathBuffer.data());
                        std::string pathStr = wideToUtf8(path);
                        // Извлекаем имя файла из пути
                        size_t lastSlash = pathStr.find_last_of("/\\");
                        if (lastSlash != std::string::npos) {
                            fileName = pathStr.substr(lastSlash + 1);
                        } else {
                            fileName = pathStr;
                        }
                    }
                }
                pLocator->Release();
            }
            pLocatorEnum->Release();
        }
    }
    
    return fileName;
}

bool isSameMobID(const aafMobID_t& a, const aafMobID_t& b) {
    return memcmp(&a, &b, sizeof(aafMobID_t)) == 0;
}

// Recursively check components (incl. EssenceGroup)
bool componentRefersToEssenceMob(IAAFComponent* pComponent, const aafMobID_t& essenceMobID) {
    IAAFSourceClip* pSrcClip = nullptr;
    if (SUCCEEDED(pComponent->QueryInterface(IID_IAAFSourceClip, (void**)&pSrcClip))) {
        aafSourceRef_t ref;
        if (SUCCEEDED(pSrcClip->GetSourceReference(&ref))) {
            pSrcClip->Release();
            return isSameMobID(ref.sourceID, essenceMobID);
        }
        pSrcClip->Release();
    }
    IAAFSequence* pSeq = nullptr;
    if (SUCCEEDED(pComponent->QueryInterface(IID_IAAFSequence, (void**)&pSeq))) {
        IEnumAAFComponents* pEnum = nullptr;
        if (SUCCEEDED(pSeq->GetComponents(&pEnum))) {
            IAAFComponent* pSubComp = nullptr;
            while (pEnum->NextOne(&pSubComp) == AAFRESULT_SUCCESS) {
                if (componentRefersToEssenceMob(pSubComp, essenceMobID)) {
                    pSubComp->Release();
                    pEnum->Release();
                    pSeq->Release();
                    return true;
                }
                pSubComp->Release();
            }
            pEnum->Release();
        }
        pSeq->Release();
    }
    return false;
}

IAAFMob* findMasterMobFromEssenceData(IAAFHeader* pHeader, IAAFEssenceData* pEssenceData) {
    aafMobID_t essenceMobID;
    IAAFMob* pMob = nullptr;
    if (FAILED(pEssenceData->QueryInterface(IID_IAAFMob, (void**)&pMob))) return nullptr;
    pMob->GetMobID(&essenceMobID);
    pMob->Release();

    IEnumAAFMobs* pEnum = nullptr;
    aafSearchCrit_t crit;
    crit.searchTag = kAAFByMobKind;
    crit.tags.mobKind = kAAFMasterMob;
    if (FAILED(pHeader->GetMobs(&crit, &pEnum))) return nullptr;

    while (pEnum->NextOne(&pMob) == AAFRESULT_SUCCESS) {
        IEnumAAFMobSlots* pSlotEnum = nullptr;
        if (SUCCEEDED(pMob->GetSlots(&pSlotEnum))) {
            IAAFMobSlot* pSlot = nullptr;
            while (pSlotEnum->NextOne(&pSlot) == AAFRESULT_SUCCESS) {
                IAAFSegment* pSegment = nullptr;
                if (SUCCEEDED(pSlot->GetSegment(&pSegment))) {
                    IAAFComponent* pComponent = nullptr;
                    if (SUCCEEDED(pSegment->QueryInterface(IID_IAAFComponent, (void**)&pComponent))) {
                        if (componentRefersToEssenceMob(pComponent, essenceMobID)) {
                            pComponent->Release();
                            pSegment->Release();
                            pSlot->Release();
                            pSlotEnum->Release();
                            pEnum->Release();
                            return pMob;
                        }
                        pComponent->Release();
                    }
                    pSegment->Release();
                }
                pSlot->Release();
            }
            pSlotEnum->Release();
        }
        pMob->Release();
    }
    pEnum->Release();
    return nullptr;
}
