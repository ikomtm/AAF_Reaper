#pragma once

#include <AAF.h>
#include <AAFResult.h>
#include <AAFTypes.h>
#include <AAFDataDefs.h>
#include <AAFClassDefUIDs.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include "debug_logger.h"

// Forward declaration
std::string wideToUtf8(const std::wstring& wstr);
std::string formatMobID(const aafMobID_t& mobID);

// Структура для хранения информации об аудио клипе
struct AAFAudioClipInfo {
    std::string originalFileName;    // Имя исходного файла
    std::string extractedFilePath;   // Путь к извлеченному файлу
    std::string mobID;              // MobID клипа
    aafPosition_t timelineStart;    // Начало на таймлайне
    aafPosition_t timelineEnd;      // Конец на таймлайне
    aafPosition_t sourceStart;      // Начало в исходном файле
    aafLength_t duration;           // Длительность
    
    // Аудио свойства
    aafRational_t sampleRate;
    aafUInt32 channelCount;
    aafUInt32 bitsPerSample;
    std::string compressionType;
    
    // Тип источника и статус embedded
    std::string sourceType;         // "Embedded", "External", "Unknown"
    bool isEmbedded;               // true если клип embedded
    
    // Эффекты и фейды
    std::vector<std::string> effects;
    bool hasFadeIn;
    bool hasFadeOut;
    aafLength_t fadeInLength;
    aafLength_t fadeOutLength;
};

// Структура для хранения информации об аудио треке
struct AAFAudioTrackInfo {
    aafSlotID_t slotID;
    std::string trackName;
    aafRational_t editRate;
    aafPosition_t origin;
    std::vector<AAFAudioClipInfo> clips;
    
    // Трек свойства
    double volume;
    double pan;
    bool mute;
    bool solo;
};

// Класс для правильного парсинга AAF согласно спецификации
class AAFProperParser {
public:
    AAFProperParser(IAAFHeader* pHeader, std::ofstream& outLog, LogLevel logLevel = LogLevel::LOG_INFO);
    ~AAFProperParser();
    
    // Основные методы парсинга
    bool parseAAFFile();
    std::vector<AAFAudioTrackInfo> getAudioTracks() const { return audioTracks; }
    
    // Доступ к логгеру
    DebugLogger& getLogger() { return logger; }
    
    // Методы извлечения
    bool extractAllEmbeddedAudio(const std::string& outputDir);
    bool extractEmbeddedAudioClip(const AAFAudioClipInfo& clip, const std::string& outputDir, std::string& outPath);
    
    // Улучшенные методы извлечения (на основе правильной логики AAF SDK)
    bool extractEmbeddedEssenceByMobID(const aafMobID_t& mobID, const std::string& outputPath);
    bool extractEmbeddedEssenceByMobID_Fallback(const aafMobID_t& mobID, const std::string& outputPath);
    
    // Утилитные методы
    std::string getFileNameFromEssenceDescriptor(IAAFEssenceDescriptor* pEssenceDesc);
    std::string getFileNameFromLocator(IAAFSourceMob* pSourceMob);
    std::string getFileNameFallbackFromSourceMob(IAAFSourceMob* pSourceMob, const aafMobID_t& mobID);
    bool parseMobIDString(const std::string& mobIdStr, aafMobID_t& mobID);
    
    // Управление логированием
    void setLogLevel(LogLevel level) { logger.setLogLevel(level); }

private:
    IAAFHeader* m_pHeader;
    std::ofstream& m_out;
    DebugLogger logger;
    std::vector<AAFAudioTrackInfo> audioTracks;
    
    // Шаг 1: Найти TopLevel CompositionMob
    IAAFMob* findTopLevelCompositionMob();
    
    // Шаг 2: Найти аудио TimelineMobSlot
    std::vector<IAAFMobSlot*> findAudioTimelineSlots(IAAFMob* pCompositionMob);
    
    // Шаг 3: Обработать SourceClip -> MasterMob цепочку
    AAFAudioClipInfo processSourceClipChain(IAAFSourceClip* pSourceClip, 
                                           const aafRational_t& editRate,
                                           aafPosition_t timelinePosition);
    
    // Шаг 4: Найти FileSourceMob из MasterMob
    IAAFSourceMob* findFileSourceMobFromMaster(IAAFMob* pMasterMob);
    
    // Шаг 5: Получить аудио свойства из EssenceDescriptor
    bool getAudioProperties(IAAFSourceMob* pSourceMob, AAFAudioClipInfo& clipInfo);
    
    // Определить тип источника (embedded или external)
    bool checkIfEmbedded(IAAFSourceMob* pSourceMob);
    
    // Шаг 6: Найти и извлечь EssenceData
    bool extractEssenceData(IAAFSourceMob* pSourceMob, AAFAudioClipInfo& clipInfo, 
                           const std::string& outputDir);
    
    // Шаг 7: Использовать IAAFEssenceAccess для правильного извлечения
    bool extractAudioUsingEssenceAccess(IAAFSourceMob* pSourceMob, 
                                       const std::string& outputPath);
    
    // Методы для извлечения essence
    bool extractUsingOpenEssence(IAAFSourceMob* pSourceMob, 
                               const std::string& outputPath);
    bool extractSamplesFromEssenceAccess(IAAFEssenceAccess* pEssenceAccess,
                                       const std::string& outputPath);
    bool extractUsingDirectEssenceData(IAAFSourceMob* pSourceMob,
                                     const std::string& outputPath);
    bool extractEssenceDataToFile(IAAFEssenceData* pEssenceData, 
                                 const std::string& outputPath);
    
    // Вспомогательные методы
    bool isTopLevelComposition(IAAFMob* pMob);
    bool isAudioDataDef(IAAFDataDef* pDataDef);
    std::string getCompressionTypeFromDescriptor(IAAFEssenceDescriptor* pEssDesc);
    // Дополнительные методы для обработки сегментов
    void processSegmentForClips(IAAFSegment* pSegment, AAFAudioTrackInfo& trackInfo, 
                               aafPosition_t currentPosition);
};
