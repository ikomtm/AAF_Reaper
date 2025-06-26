#pragma once

#include <AAF.h>
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <set>
#include "data_structures.h"

// Глобальная карта для сбора имён embedded файлов во время анализа
extern std::map<std::string, std::string> g_embeddedFileNames; // MobID -> правильное имя файла

// Парсинг AAF файлов
void processCompositionSlot(IAAFMobSlot* pSlot, std::ofstream& out, int slotIndex, 
                           const std::map<std::string, std::string>& mobIdToName,
                           int& audioTrackCount, int& audioClipCount, int& audioFadeCount, 
                           int& audioEffectCount, aafPosition_t sessionStartTC);

void processComponent(IAAFComponent* pComp, std::ofstream& out, int compIndex, 
                     const std::map<std::string, std::string>& mobIdToName, 
                     const aafRational_t& editRate,
                     int& audioClipCount, int& audioFadeCount, int& audioEffectCount);

void processComponentWithPosition(IAAFComponent* pComp, std::ofstream& out, int compIndex, 
                                 aafPosition_t startPosition, 
                                 const std::map<std::string, std::string>& mobIdToName, 
                                 const aafRational_t& editRate,
                                 int& audioClipCount, int& audioFadeCount, int& audioEffectCount);

void processAudioComponentWithPosition(IAAFComponent* pComp, std::ofstream& out, int compIndex, 
                                      aafPosition_t startPosition, 
                                      const std::map<std::string, std::string>& mobIdToName, 
                                      const aafRational_t& editRate,
                                      int& audioClipCount, int& audioFadeCount, int& audioEffectCount);

void extractClipsFromSegment(IAAFSegment* pSegment, const std::map<std::string, std::string>& mobIdToName,
                           const aafRational_t& editRate, aafPosition_t sessionStartTC,
                           aafPosition_t currentPosition, AudioTrackData& trackData,
                           IAAFHeader* pHeader, const std::set<std::string>& embeddedMobIDs, std::ofstream& out);

void extractClipsFromComponent(IAAFComponent* pComp, const std::map<std::string, std::string>& mobIdToName,
                             const aafRational_t& editRate, aafPosition_t sessionStartTC,
                             aafPosition_t currentPosition, AudioTrackData& trackData,
                             IAAFHeader* pHeader, const std::set<std::string>& embeddedMobIDs, std::ofstream& out);

void processAudioTrackForExport(IAAFMobSlot* pSlot, const std::map<std::string, std::string>& mobIdToName, 
                               aafPosition_t sessionStartTC, AudioTrackData& trackData);

void processAudioTrackForExportWithHeader(IAAFMobSlot* pSlot, const std::map<std::string, std::string>& mobIdToName, 
                                         aafPosition_t sessionStartTC, AudioTrackData& trackData, 
                                         IAAFHeader* pHeader, const std::set<std::string>& embeddedMobIDs, std::ofstream& out);

// Вспомогательные функции для обработки fade параметров
AudioClipData createClipFromSourceClip(IAAFSourceClip* pClip, const std::map<std::string, std::string>& mobIdToName,
                                      const aafRational_t& editRate, aafPosition_t sessionStartTC,
                                      aafPosition_t currentPosition, IAAFHeader* pHeader, 
                                      const std::set<std::string>& embeddedMobIDs, std::ofstream& out);

void analyzeFadePatterns(const std::vector<IAAFComponent*>& components, 
                        const std::vector<aafPosition_t>& positions,
                        size_t clipIndex, const aafRational_t& editRate,
                        AudioClipData& clipData);

// Генерирует правильное имя файла для Audio Clip
std::string generateAudioClipFileName(IAAFSourceClip* pClip, const std::map<std::string, std::string>& mobIdToName,
                                     std::ofstream& out);
