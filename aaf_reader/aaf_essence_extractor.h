#pragma once

#include "data_structures.h"
#include "debug_logger.h"
#include <AAF.h>
#include <string>

class AAFEssenceExtractor {
public:
    AAFEssenceExtractor(IAAFHeader* pHeader, DebugLogger& logger);
    ~AAFEssenceExtractor();

    // Извлечение embedded аудио клипа
    bool extractEmbeddedAudioClip(const AAFAudioClipInfo& clip, 
                                  const std::string& outputDir, 
                                  std::string& outPath);

    // Извлечение всех embedded аудио файлов
    bool extractAllEmbeddedAudio(const std::vector<AAFAudioTrackInfo>& audioTracks,
                                 const std::string& outputDir);

    // Проверка является ли SourceMob embedded
    bool checkIfEmbedded(IAAFSourceMob* pSourceMob);

    // Получение имени файла из Locator
    std::string getFileNameFromLocator(IAAFSourceMob* pSourceMob);

    // Fallback получение имени файла
    std::string getFileNameFallbackFromSourceMob(IAAFSourceMob* pSourceMob, const aafMobID_t& mobID);

    // Парсинг MobID из строки
    bool parseMobIDString(const std::string& mobIdStr, aafMobID_t& mobID);

    // Определение форматов файлов
    std::string detectFileExtension(IAAFEssenceData* pEssenceData);
    std::string getEmbeddedFileExtension(const aafMobID_t& mobID);
    std::string determineFormatFromEssenceDescriptor(IAAFEssenceDescriptor* pEssDesc);
    std::string getExtensionFromCodingUID(const aafUID_t& codingUID);

private:
    // Извлечение EssenceData по MobID
    bool extractEmbeddedEssenceByMobID(const aafMobID_t& mobID, const std::string& outputPath);

    // Fallback метод извлечения через перечисление
    bool extractEmbeddedEssenceByMobID_Fallback(const aafMobID_t& mobID, const std::string& outputPath);

    IAAFHeader* m_pHeader;
    DebugLogger& logger;
};
