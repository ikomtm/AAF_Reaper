#pragma once

#include <AAF.h>
#include <string>
#include <fstream>
#include <set>
#include <map>
#include "data_structures.h"

// Работа с медиафайлами и извлечение embedded контента
void createExtractedMediaFolder();
void extractEmbeddedAudio(IAAFEssenceData* pEssenceData, const std::string& outputPath, std::ofstream& out);
bool isFileEmbedded(const std::set<std::string>& embeddedMobIDs, const std::string& mobIdStr);
std::set<std::string> buildEmbeddedFilesMap(IAAFHeader* pHeader, std::ofstream& out);
void debugEssenceData(IAAFHeader* pHeader, std::ofstream& out);
std::string findAndExtractEssenceData(IAAFHeader* pHeader, const aafMobID_t& mobID, 
                                     const std::string& mobIdStr, const std::map<std::string, std::string>& mobIdToName, std::ofstream& out);
void processAudioFiles(const ProjectData& projectData, std::ofstream& out);
// Структура для хранения информации об embedded файле
struct EmbeddedFileInfo {
    std::string filePath;
    std::string embeddedMobID;
    bool isEmbedded;
    
    EmbeddedFileInfo() : isEmbedded(false) {}
    EmbeddedFileInfo(const std::string& path, const std::string& mobId) 
        : filePath(path), embeddedMobID(mobId), isEmbedded(true) {}
};

std::string findEmbeddedFileRecursive(IAAFHeader* pHeader, const aafMobID_t& mobID, 
                                     const std::set<std::string>& embeddedMobIDs,
                                     const std::map<std::string, std::string>& mobIdToName, 
                                     std::ofstream& out);

EmbeddedFileInfo findEmbeddedFileInfo(IAAFHeader* pHeader, const aafMobID_t& mobID, 
                                     const std::set<std::string>& embeddedMobIDs,
                                     const std::map<std::string, std::string>& mobIdToName, 
                                     std::ofstream& out);
// Построение маппинга embedded файлов с правильными именами
std::map<std::string, std::string> buildEmbeddedFileNameMapping(IAAFHeader* pHeader, 
                                                               const std::map<std::string, std::string>& mobIdToName, 
                                                               std::ofstream& out);

// Вспомогательная функция для рекурсивного обхода сегментов
void traverseSegmentForEmbeddedMapping(IAAFSegment* pSegment, 
                                     IAAFHeader* pHeader,
                                     const std::set<std::string>& embeddedMobIDs,
                                     const std::map<std::string, std::string>& mobIdToName,
                                     std::map<std::string, std::string>& embeddedMapping,
                                     std::ofstream& out);

// Функция для определения правильного расширения файла по essence data
std::string detectFileExtension(IAAFEssenceData* pEssenceData, std::ofstream& out);

std::map<std::string, std::string> extractAllEmbeddedFiles(IAAFHeader* pHeader, 
                                                       const std::map<std::string, std::string>& mobIdToName, 
                                                       const std::map<std::string, std::string>& embeddedNameMapping, 
                                                       std::ofstream& out);
