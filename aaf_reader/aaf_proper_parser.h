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
#include <memory>
#include "debug_logger.h"
#include "data_structures.h"
#include "aaf_clip_processor.h"
#include "aaf_audio_properties.h"
#include "aaf_essence_extractor.h"

// Forward declaration
std::string wideToUtf8(const std::wstring& wstr);
std::string formatMobID(const aafMobID_t& mobID);

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
    
    // Методы извлечения (делегируются в AAFEssenceExtractor)
    bool extractAllEmbeddedAudio(const std::string& outputDir);
    
    // Управление логированием
    void setLogLevel(LogLevel level) { logger.setLogLevel(level); }

private:
    IAAFHeader* m_pHeader;
    std::ofstream& m_out;
    DebugLogger logger;
    std::vector<AAFAudioTrackInfo> audioTracks;
    
    // Модульные компоненты
    std::unique_ptr<AAFClipProcessor> clipProcessor;
    std::unique_ptr<AAFAudioProperties> audioProperties;
    std::unique_ptr<AAFEssenceExtractor> essenceExtractor;
    
    // Основные методы парсинга
    IAAFMob* findTopLevelCompositionMob();
    std::vector<IAAFMobSlot*> findAudioTimelineSlots(IAAFMob* pCompositionMob);
    
    // Вспомогательные методы
    bool isTopLevelComposition(IAAFMob* pMob);
};
