// aaf_essence_parser.cpp
// Updated logic for resolving EssenceData ↔ MasterMob link in AAF
// Fully compatible with AAF SDK 1.2.0 (no IAAFSourceMob::OpenEssence)

#include <assert.h>
#include <map>
#include <string>
#include <fstream>
#include <filesystem>
#include "AAF.h"

// Utility: Compare MobIDs
bool IsSameMobID(const aafMobID_t& a, const aafMobID_t& b) {
    return memcmp(&a, &b, sizeof(aafMobID_t)) == 0;
}

// Find MasterMob by name
IAAFMob* FindMasterMobByName(IAAFHeader* pHeader, const std::wstring& name) {
    IEnumAAFMobs* pEnum = nullptr;
    aafSearchCrit_t crit = { kByMobKind, { kMasterMob } };
    if (FAILED(pHeader->GetMobs(&crit, &pEnum))) return nullptr;

    IAAFMob* pMob = nullptr;
    while (pEnum->NextOne(&pMob) == AAFRESULT_SUCCESS) {
        aafCharacter mobName[256] = {};
        if (SUCCEEDED(pMob->GetName(mobName, 256)) && name == std::wstring(mobName)) {
            pEnum->Release();
            return pMob;  // caller owns
        }
        pMob->Release();
    }
    pEnum->Release();
    return nullptr;
}

// Find top-level CompositionMob
IAAFMob* FindTopLevelCompositionMob(IAAFHeader* pHeader) {
    IEnumAAFMobs* pEnum = nullptr;
    aafSearchCrit_t crit = { kByMobKind, { kCompositionMob } };
    if (FAILED(pHeader->GetMobs(&crit, &pEnum))) return nullptr;

    IAAFMob* pMob = nullptr;
    while (pEnum->NextOne(&pMob) == AAFRESULT_SUCCESS) {
        // Here we could use GetName() or check for slot types to filter
        pEnum->Release();
        return pMob;  // return first CompositionMob found
    }
    pEnum->Release();
    return nullptr;
}

// Retrieve suggested filename from MobName if Locator is missing
std::wstring GetFileNameFallbackFromSourceMob(IAAFSourceMob* pSrcMob, const aafMobID_t& mobID) {
    IAAFMob* pMob = nullptr;
    if (SUCCEEDED(pSrcMob->QueryInterface(IID_IAAFMob, (void**)&pMob))) {
        aafCharacter name[128] = {0};
        if (SUCCEEDED(pMob->GetName(name, 128))) {
            pMob->Release();
            return std::wstring(name) + L".dat";
        }
        pMob->Release();
    }

    wchar_t fallbackName[64];
    swprintf(fallbackName, 64, L"essence_%08x.dat", mobID.Data1);
    return std::wstring(fallbackName);
}

// Retrieve suggested filename from Locator if present
std::wstring GetFileNameFromEssenceDescriptor(IAAFMob* pMob) {
    IAAFObject* pObject = nullptr;
    if (FAILED(pMob->QueryInterface(IID_IAAFObject, (void**)&pObject))) return L"embedded.dat";

    IAAFEssenceDescriptor* pDesc = nullptr;
    if (FAILED(pObject->QueryInterface(IID_IAAFEssenceDescriptor, (void**)&pDesc))) {
        pObject->Release();
        return L"embedded.dat";
    }

    aafUInt32 count = 0;
    if (FAILED(pDesc->CountLocators(&count)) || count == 0) {
        pDesc->Release();
        pObject->Release();
        return L"embedded.dat";
    }

    IAAFLocator* pLoc = nullptr;
    if (FAILED(pDesc->GetLocatorAt(0, &pLoc))) {
        pDesc->Release();
        pObject->Release();
        return L"embedded.dat";
    }

    aafCharacter filePath[1024] = {0};
    std::wstring result = L"embedded.dat";
    if (SUCCEEDED(pLoc->GetPath(filePath, 1024))) {
        std::wstring path(filePath);
        size_t pos = path.find_last_of(L"/\\");
        result = (pos != std::wstring::npos) ? path.substr(pos + 1) : path;
    }

    pLoc->Release();
    pDesc->Release();
    pObject->Release();
    return result;
}

    IAAFFileDescriptor* pDesc = nullptr;
    if (FAILED(pObject->QueryInterface(IID_IAAFFileDescriptor, (void**)&pDesc))) {
        pObject->Release();
        return L"embedded.dat";
    }

    IEnumAAFLocators* pLocEnum = nullptr;
    if (FAILED(pDesc->GetLocators(&pLocEnum))) {
        pDesc->Release();
        pObject->Release();
        return L"embedded.dat";
    }

    IAAFLocator* pLoc = nullptr;
    while (pLocEnum->NextOne(&pLoc) == AAFRESULT_SUCCESS) {
        aafCharacter filePath[1024] = {0};
        if (SUCCEEDED(pLoc->GetPath(filePath, 1024))) {
            std::wstring path(filePath);
            size_t pos = path.find_last_of(L"/\\");
            pLoc->Release();
            pLocEnum->Release();
            pDesc->Release();
            pObject->Release();
            return (pos != std::wstring::npos) ? path.substr(pos + 1) : path;
        }
        pLoc->Release();
    }

    pLocEnum->Release();
    pDesc->Release();
    pObject->Release();
    return L"embedded.dat";
}

    IEnumAAFLocators* pLocEnum = nullptr;
    if (FAILED(pDesc->GetLocators(&pLocEnum))) {
        pDesc->Release();
        return L"embedded.dat";
    }

    IAAFLocator* pLoc = nullptr;
    while (pLocEnum->NextOne(&pLoc) == AAFRESULT_SUCCESS) {
        aafCharacter filePath[1024] = {0};
        if (SUCCEEDED(pLoc->GetPath(filePath, 1024))) {
                std::wstring path(filePath);
                size_t pos = path.find_last_of(L"/\\");
                pNetLoc->Release();
                pLoc->Release();
                pLocEnum->Release();
                pDesc->Release();
                return (pos != std::wstring::npos) ? path.substr(pos + 1) : path;
            }
            pLoc->Release();
    }

    pLocEnum->Release();
    pDesc->Release();
    return L"embedded.dat";
}

// Extract essence using IAAFEssenceData (compatible with AAF SDK 1.2.0)
bool ExtractEmbeddedEssenceByMobID(IAAFHeader* pHeader,
                                    const aafMobID_t& mobID,
                                    const std::wstring& outputPath) {
    IAAFContentStorage* pStorage = nullptr;
    if (FAILED(pHeader->GetContentStorage(&pStorage))) return false;

    IAAFEssenceData* pEssence = nullptr;
    if (FAILED(pStorage->LookupEssenceData(mobID, &pEssence))) {
        pStorage->Release();
        return false;
    }

    std::ofstream outFile(outputPath, std::ios::binary);
    if (!outFile.is_open()) {
        pEssence->Release();
        pStorage->Release();
        return false;
    }

    const size_t bufSize = 8192;
    std::vector<aafUInt8> buffer(bufSize);
    aafUInt32 readBytes = 0;

    while (SUCCEEDED(pEssence->Read(bufSize, buffer.data(), &readBytes)) && readBytes > 0) {
        outFile.write(reinterpret_cast<const char*>(buffer.data()), readBytes);
    }

    outFile.close();
    pEssence->Release();
    pStorage->Release();
    return true;
}

// Extract audio from CompositionMob via SourceClips
bool ExtractAudioFromCompositionMob(IAAFHeader* pHeader, const std::wstring& outputDir) {
    IAAFMob* pCompMob = FindTopLevelCompositionMob(pHeader);
    if (!pCompMob) return false;

    IEnumAAFMobSlots* pSlotEnum = nullptr;
    if (FAILED(pCompMob->GetSlots(&pSlotEnum))) return false;

    IAAFMobSlot* pSlot = nullptr;
    while (pSlotEnum->NextOne(&pSlot) == AAFRESULT_SUCCESS) {
        IAAFDataDef* pDef = nullptr;
        if (SUCCEEDED(pSlot->GetDataDef(&pDef))) {
            aafUID_t defId;
            if (SUCCEEDED(pDef->GetAUID(&defId)) && memcmp(&defId, &kAAFDataDef_Sound, sizeof(aafUID_t)) == 0) {
                IAAFSegment* pSegment = nullptr;
                if (SUCCEEDED(pSlot->GetSegment(&pSegment))) {
                    IAAFSourceClip* pSrcClip = nullptr;
                    if (SUCCEEDED(pSegment->QueryInterface(IID_IAAFSourceClip, (void**)&pSrcClip))) {
                        aafSourceRef_t ref;
                        if (SUCCEEDED(pSrcClip->GetSourceReference(&ref))) {
                            IAAFMob* pMob = nullptr;
                            if (SUCCEEDED(pHeader->LookupMob(ref.sourceID, &pMob))) {
                                IAAFSourceMob* pSrcMob = nullptr;
                                if (SUCCEEDED(pMob->QueryInterface(IID_IAAFSourceMob, (void**)&pSrcMob))) {
                                    std::wstring name = GetFileNameFromEssenceDescriptor(pMob);
                                    if (name == L"embedded.dat") {
                                        name = GetFileNameFallbackFromSourceMob(pSrcMob, ref.sourceID);
                                    }
                                    std::filesystem::path outPath = std::filesystem::path(outputDir) / name;
                                    ExtractEmbeddedEssenceByMobID(pHeader, ref.sourceID, outPath);
                                    pSrcMob->Release();
                                }
                                pMob->Release();
                            }
                        }
                        pSrcClip->Release();
                    }
                    pSegment->Release();
                }
            }
            pDef->Release();
        }
        pSlot->Release();
    }
    pSlotEnum->Release();
    return true;
}

// Example usage inside extraction routine
bool ExtractAllAudioEssenceTracks(IAAFHeader* pHeader, const std::wstring& outputDir) {
    IEnumAAFMobs* pMobEnum = nullptr;
    aafSearchCrit_t crit = { kByMobKind, { kSourceMob } };
    if (FAILED(pHeader->GetMobs(&crit, &pMobEnum))) return false;

    IAAFMob* pMob = nullptr;
    while (pMobEnum->NextOne(&pMob) == AAFRESULT_SUCCESS) {
        IAAFSourceMob* pSrc = nullptr;
        if (SUCCEEDED(pMob->QueryInterface(IID_IAAFSourceMob, (void**)&pSrc))) {
            IEnumAAFMobSlots* pSlotEnum = nullptr;
            if (SUCCEEDED(pSrc->GetSlots(&pSlotEnum))) {
                IAAFMobSlot* pSlot = nullptr;
                while (pSlotEnum->NextOne(&pSlot) == AAFRESULT_SUCCESS) {
                    IAAFDataDef* pDef = nullptr;
                    if (SUCCEEDED(pSlot->GetDataDef(&pDef))) {
                        aafUID_t defId;
                        if (SUCCEEDED(pDef->GetAUID(&defId)) &&
                            memcmp(&defId, &kAAFDataDef_Sound, sizeof(aafUID_t)) == 0) {

                            aafMobID_t mobID;
                            IAAFMob* pBaseMob = nullptr;
                            if (SUCCEEDED(pSrc->QueryInterface(IID_IAAFMob, (void**)&pBaseMob)) && SUCCEEDED(pBaseMob->GetMobID(&mobID))) {
                                pBaseMob->Release();
                                std::wstring name = GetFileNameFromEssenceDescriptor(pMob);
                                if (name == L"embedded.dat") {
                                    name = GetFileNameFallbackFromSourceMob(pSrc, mobID);
                                }
                                std::filesystem::path outPath = std::filesystem::path(outputDir) / name;
                                ExtractEmbeddedEssenceByMobID(pHeader, mobID, outPath);
                            }
                        }
                        pDef->Release();
                    }
                    pSlot->Release();
                }
                pSlotEnum->Release();
            }
            pSrc->Release();
        }
        pMob->Release();
    }
    pMobEnum->Release();
    return true;
}
