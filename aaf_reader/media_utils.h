#pragma once

#include <AAF.h>
#include <string>
#include <fstream>
#include <set>
#include <map>
#include "data_structures.h"

// Глобальные карты для embedded файлов
extern std::map<std::string, std::string> g_embeddedFileNames; // embedded MobID -> правильное имя файла
extern std::map<std::string, std::string> g_masterMobToFileName; // MasterMob MobID -> имя файла

// Работа с медиафайлами и извлечение embedded контента
void createExtractedMediaFolder();
void extractEmbeddedAudio(IAAFEssenceData* pEssenceData, const std::string& outputPath, std::ofstream& out);
bool isFileEmbedded(const std::set<std::string>& embeddedMobIDs, const std::string& mobIdStr);
std::set<std::string> buildEmbeddedFilesMap(IAAFHeader* pHeader, std::ofstream& out);
void debugEssenceData(IAAFHeader* pHeader, std::ofstream& out);
std::string findAndExtractEssenceData(IAAFHeader* pHeader, const aafMobID_t& mobID, 
                                     const std::string& mobIdStr, const std::map<std::string, std::string>& mobIdToName, std::ofstream& out);
void processAudioFiles(const ProjectData& projectData, std::ofstream& out);
// Построение маппинга embedded файлов с правильными именами (НОВЫЙ ROBUST ПОДХОД)
std::map<std::string, std::string> buildEmbeddedFileNameMapping(IAAFHeader* pHeader, 
                                                               const std::map<std::string, std::string>& mobIdToName, 
                                                               std::ofstream& out);

// Функция для определения правильного расширения файла по essence data
std::string detectFileExtension(IAAFEssenceData* pEssenceData, std::ofstream& out);

// Функция для определения расширения embedded файла по MobID из метаданных AAF
std::string getEmbeddedFileExtension(IAAFHeader* pHeader, const aafMobID_t& mobID, std::ofstream& out);

// Функция для определения формата из EssenceDescriptor
std::string determineFormatFromEssenceDescriptor(IAAFEssenceDescriptor* pEssDesc, std::ofstream& out);

// Функция для получения расширения из UID кодировки
std::string getExtensionFromCodingUID(const aafUID_t& codingUID, std::ofstream& out);

// Извлечение всех embedded файлов (ROBUST подход)
std::map<std::string, std::string> extractAllEmbeddedFiles(IAAFHeader* pHeader, 
                                                       const std::map<std::string, std::string>& mobIdToName, 
                                                       const std::map<std::string, std::string>& embeddedNameMapping, 
                                                       std::ofstream& out);
