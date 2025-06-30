#pragma once

#include <AAF.h>
#include <AAFResult.h>
#include <AAFTypes.h>
#include <string>
#include <vector>
#include <iostream>

// Утилиты для работы с AAF
std::string wideToUtf8(const std::wstring& wstr);
std::string formatMobID(const aafMobID_t& mobID);
std::string formatTimecode(aafPosition_t position, const aafRational_t& editRate);
aafPosition_t getCompositionStartTimecode(IAAFMob* pMob);

// Find the first Composition Mob in the AAF file
IAAFMob* findCompositionMob(IAAFHeader* pHeader);

bool isAudioTrack(IAAFMobSlot* pSlot);
std::string getDataDefName(IAAFDataDef* pDataDef);

// Утилиты для извлечения имён файлов
std::string getFileNameFromEssenceDescriptor(IAAFEssenceDescriptor* pEssenceDesc);

// Утилиты для связывания MasterMob и EssenceData
bool isSameMobID(const aafMobID_t& a, const aafMobID_t& b);
bool componentRefersToEssenceMob(IAAFComponent* pComponent, const aafMobID_t& essenceMobID);
IAAFMob* findMasterMobFromEssenceData(IAAFHeader* pHeader, IAAFEssenceData* pEssenceData);

namespace aaf_utils {
    // Keep for logging/display purposes
    std::string formatMobID_Legacy(const aafMobID_t& mobID);

    // New full-fidelity MobID conversion functions
    std::string formatMobIDFull(const aafMobID_t& mobID);
    bool parseMobIDFull(const std::string& mobIdStr, aafMobID_t& mobID);

    void listAllMobs(IAAFHeader* header, std::ostream& out = std::cout);

} // namespace aaf_utils
