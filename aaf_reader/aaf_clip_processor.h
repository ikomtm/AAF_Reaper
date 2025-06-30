#pragma once

#include "data_structures.h"
#include "debug_logger.h"
#include <AAF.h>
#include <AAFOperationDefs.h>
#include <AAFParameterDefs.h>
#include <vector>

// Forward declarations
class AAFAudioProperties;
class AAFEssenceExtractor;

class AAFClipProcessor {
public:
    AAFClipProcessor(IAAFHeader* pHeader, DebugLogger& logger);
    ~AAFClipProcessor();

    // Установка зависимостей на другие модули
    void setAudioProperties(AAFAudioProperties* audioProps) { audioProperties = audioProps; }
    void setEssenceExtractor(AAFEssenceExtractor* essenceExt) { essenceExtractor = essenceExt; }

    // Обработка сегментов для поиска клипов
    void processSegmentForClips(IAAFSegment* pSegment, 
                               AAFAudioTrackInfo& trackInfo, 
                               aafPosition_t currentPosition);

    // Обработка цепочки SourceClip -> MasterMob -> FileSourceMob
    AAFAudioClipInfo processSourceClipChain(IAAFSourceClip* pSourceClip, 
                                           const aafRational_t& editRate,
                                           aafPosition_t timelinePosition);

    // Поиск FileSourceMob из MasterMob
    IAAFSourceMob* findFileSourceMobFromMaster(IAAFMob* pMasterMob);
    
    // Извлечение fade-информации из SourceClip (новая реализация)
    void extractSourceClipFades(IAAFSourceClip* pSourceClip, AAFAudioClipInfo& clipInfo);
    
    // Обработка Transition для поиска crossfade
    void processTransitionForCrossfade(IAAFTransition* pTransition, 
                                      AAFAudioTrackInfo& trackInfo, 
                                      aafPosition_t currentPosition);

    // --- Descriptive Metadata logging ---
    void logDescriptiveMetadata(IAAFMob* pMob, const std::string& context);

private:
    IAAFHeader* m_pHeader;
    DebugLogger& logger;
    
    // Зависимости на другие модули (не владеющие указатели)
    AAFAudioProperties* audioProperties;
    AAFEssenceExtractor* essenceExtractor;
};
