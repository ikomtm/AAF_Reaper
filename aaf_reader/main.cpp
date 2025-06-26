#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING

#include <AAF.h>
#include <AAFResult.h>
#include <AAFTypes.h>

#include <iostream>
#include <string>
#include <locale>
#include <codecvt>
#include <cstring>
#include <cstdio>

// Теневые интерфейсы
struct IAAFSequence;
struct IAAFFiller;
struct IEnumAAFComponents;
struct IAAFSourceClip;

extern "C" const IID IID_IAAFSequence;
extern "C" const IID IID_IAAFFiller;
extern "C" const IID IID_IEnumAAFComponents;
extern "C" const IID IID_IAAFSourceClip;

// Конвертация wchar → UTF-8
std::string wideToUtf8(const std::wstring& wstr) {
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
    return conv.to_bytes(wstr);
}

// Форматированный вывод aafMobID_t
std::string formatMobID(const aafMobID_t& mobID) {
    char buffer[64];
    snprintf(buffer, sizeof(buffer),
        "%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X",
        mobID.Data1,
        mobID.Data2,
        mobID.Data3,
        mobID.Data4[0], mobID.Data4[1],
        mobID.Data4[2], mobID.Data4[3],
        mobID.Data4[4], mobID.Data4[5], mobID.Data4[6], mobID.Data4[7]);
    return std::string(buffer);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "❌ Usage: aaf_reader <file.aaf>" << std::endl;
        return 1;
    }

    std::cout << "📁 Opening AAF file...\n";

    size_t len = strlen(argv[1]) + 1;
    wchar_t* widePath = new wchar_t[len];
    mbstowcs_s(nullptr, widePath, len, argv[1], _TRUNCATE);

    IAAFFile* pFile = nullptr;
    if (FAILED(AAFFileOpenExistingRead(widePath, 0, &pFile)) || !pFile) {
        std::cerr << "❌ Failed to open file.\n";
        delete[] widePath;
        return 1;
    }
    delete[] widePath;

    IAAFHeader* pHeader = nullptr;
    if (FAILED(pFile->GetHeader(&pHeader))) {
        std::cerr << "❌ Failed to get header.\n";
        pFile->Close(); pFile->Release();
        return 1;
    }

    IEnumAAFMobs* pMobIter = nullptr;
    if (FAILED(pHeader->GetMobs(nullptr, &pMobIter))) {
        std::cerr << "❌ Failed to get Mobs.\n";
        pHeader->Release(); pFile->Close(); pFile->Release();
        return 1;
    }

    int mobCount = 0;
    IAAFMob* pMob = nullptr;
    while (SUCCEEDED(pMobIter->NextOne(&pMob))) {
        aafWChar name[256] = {0};
        if (SUCCEEDED(pMob->GetName(name, sizeof(name)))) {
            std::string utf8Name = wideToUtf8(name);
            std::string kindStr = "Unknown";

            IAAFCompositionMob* pComp = nullptr;
            if (SUCCEEDED(pMob->QueryInterface(IID_IAAFCompositionMob, (void**)&pComp))) {
                kindStr = "CompositionMob"; pComp->Release();
            } else {
                IAAFMasterMob* pMaster = nullptr;
                if (SUCCEEDED(pMob->QueryInterface(IID_IAAFMasterMob, (void**)&pMaster))) {
                    kindStr = "MasterMob"; pMaster->Release();
                } else {
                    IAAFSourceMob* pSource = nullptr;
                    if (SUCCEEDED(pMob->QueryInterface(IID_IAAFSourceMob, (void**)&pSource))) {
                        kindStr = "SourceMob"; pSource->Release();
                    }
                }
            }

            std::cout << "🔹 Mob #" << (++mobCount) << " [" << kindStr << "]: " << utf8Name << std::endl;

            aafNumSlots_t numSlots = 0;
            if (SUCCEEDED(pMob->CountSlots(&numSlots))) {
                for (aafUInt32 i = 0; i < numSlots; ++i) {
                    IAAFMobSlot* pSlot = nullptr;
                    if (SUCCEEDED(pMob->GetSlotAt(i, &pSlot))) {
                        IAAFSegment* pSegment = nullptr;
                        if (SUCCEEDED(pSlot->GetSegment(&pSegment))) {
                            std::string segmentType = "Unknown";

                            IAAFSourceClip* pSourceClip = nullptr;
                            if (SUCCEEDED(pSegment->QueryInterface(IID_IAAFSourceClip, (void**)&pSourceClip))) {
                                segmentType = "SourceClip";
                                pSourceClip->Release();
                            } else {
                                IAAFSequence* pSeq = nullptr;
                                if (SUCCEEDED(pSegment->QueryInterface(IID_IAAFSequence, (void**)&pSeq))) {
                                    segmentType = "Sequence";
                                    std::cout << "   🎵 Track #" << i << " (" << segmentType << ")\n";

                                    IEnumAAFComponents* pEnum = nullptr;
                                    if (SUCCEEDED(pSeq->GetComponents(&pEnum))) {
                                        IAAFComponent* pComp = nullptr;
                                        int compIndex = 0;

                                        while (SUCCEEDED(pEnum->NextOne(&pComp))) {
                                            IAAFSourceClip* pClip = nullptr;
                                            if (SUCCEEDED(pComp->QueryInterface(IID_IAAFSourceClip, (void**)&pClip))) {
                                                aafSourceRef_t ref;
                                                if (SUCCEEDED(pClip->GetSourceReference(&ref))) {
                                                    std::cout << "     📌 Clip #" << compIndex++
                                                              << ": Start=" << ref.startTime
                                                              << ", TrackID=" << ref.sourceTrackID
                                                              << ", MobID=" << formatMobID(ref.sourceID) << "\n";
                                                }
                                                pClip->Release();
                                            } else {
                                                std::cout << "     📎 Component #" << compIndex++ << ": not a SourceClip\n";
                                            }
                                            pComp->Release();
                                        }
                                        pEnum->Release();
                                    }
                                    pSeq->Release();
                                    pSegment->Release();
                                    pSlot->Release();
                                    continue;
                                }
                            }

                            std::cout << "   🎵 Track #" << i << " (" << segmentType << ")\n";
                            pSegment->Release();
                        }
                        pSlot->Release();
                    }
                }
            }
        }
        pMob->Release();
    }

    pMobIter->Release();
    pHeader->Release();
    pFile->Close();
    pFile->Release();

    return 0;
}
