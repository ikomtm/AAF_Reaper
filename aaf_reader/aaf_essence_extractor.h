#pragma once

#include "data_structures.h"
#include "debug_logger.h"
#include <AAF.h>
#include <string>
#include <map>

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

    // Генерация правильного имени файла с суффиксом
    std::string generateProperFileName(const std::string& originalFileName, const std::string& mobID);

    // Парсинг MobID из строки
    bool parseMobIDString(const std::string& mobIdStr, aafMobID_t& mobID);

    // Определение форматов файлов
    std::string detectFileExtension(IAAFEssenceData* pEssenceData);
    std::string getEmbeddedFileExtension(const aafMobID_t& mobID);
    std::string determineFormatFromEssenceDescriptor(IAAFEssenceDescriptor* pEssDesc);
    std::string getExtensionFromCodingUID(const aafUID_t& codingUID);

private:
    // Извлечение данных из SourceMob
    bool extractFromSourceMob(IAAFSourceMob* pSourceMob, 
                             const std::string& originalFileName,
                             const std::string& outputDir, 
                             std::string& outPath);
    
    // Запись EssenceData в файл
    bool extractEssenceDataToFile(IAAFEssenceData* pEssenceData, const std::string& filePath);

    IAAFHeader* m_pHeader;
    DebugLogger& logger;
    
    // Счетчики для генерации суффиксов файлов
    std::map<std::string, int> fileCounters;
};
