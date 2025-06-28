#include "aaf_essence_extractor.h"
#include "aaf_utils.h"
#include <AAFTypeDefUIDs.h>
#include <AAFClassDefUIDs.h>
#include <filesystem>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cstring>
#include <cstdlib>
#include <cstdio>

AAFEssenceExtractor::AAFEssenceExtractor(IAAFHeader* pHeader, DebugLogger& logger)
    : m_pHeader(pHeader), logger(logger) {
}

AAFEssenceExtractor::~AAFEssenceExtractor() {
}

bool AAFEssenceExtractor::extractEmbeddedAudioClip(const AAFAudioClipInfo& clip, 
                                                  const std::string& outputDir, 
                                                  std::string& outPath) {
    logger.debug(LogCategory::EXTRACTION, "Extracting embedded audio clip: " + clip.mobID);
    
    // 1. Парсим MobID из строки - это может быть ID любого Mob
    aafMobID_t mobID;
    if (!parseMobIDString(clip.mobID, mobID)) {
        logger.warn(LogCategory::EXTRACTION, "Failed to parse MobID: " + clip.mobID);
        return false;
    }
    
    // 2. Ищем любой Mob по ID и проверяем его тип
    IAAFMob* pMob = nullptr;
    if (FAILED(m_pHeader->LookupMob(mobID, &pMob))) {
        logger.warn(LogCategory::EXTRACTION, "Failed to find any Mob with ID: " + clip.mobID);
        return false;
    }
    
    logger.debug(LogCategory::EXTRACTION, "Found Mob, checking type...");
    
    // Проверим, может это сразу SourceMob?
    IAAFSourceMob* pSourceMob = nullptr;
    if (SUCCEEDED(pMob->QueryInterface(IID_IAAFSourceMob, (void**)&pSourceMob))) {
        logger.debug(LogCategory::EXTRACTION, "Mob is SourceMob - checking if embedded");
        bool isEmbedded = checkIfEmbedded(pSourceMob);
        if (isEmbedded) {
            logger.debug(LogCategory::EXTRACTION, "SourceMob is embedded, proceeding with extraction...");
            pMob->Release();
            // Выполняем извлечение essence напрямую
            IAAFSourceMob* pFileSourceMob = pSourceMob;
            IAAFMob* pFileSourceMobBase = nullptr;
            if (FAILED(pFileSourceMob->QueryInterface(IID_IAAFMob, (void**)&pFileSourceMobBase))) {
                logger.warn(LogCategory::EXTRACTION, "Failed to get IAAFMob interface from FileSourceMob");
                pFileSourceMob->Release();
                return false;
            }
            aafMobID_t fileSourceMobID;
            if (FAILED(pFileSourceMobBase->GetMobID(&fileSourceMobID))) {
                logger.warn(LogCategory::EXTRACTION, "Failed to get FileSourceMob ID");
                pFileSourceMobBase->Release();
                pFileSourceMob->Release();
                return false;
            }
            std::string fileSourceMobIDStr = formatMobID(fileSourceMobID);
            logger.debug(LogCategory::EXTRACTION, "FileSourceMob ID: " + fileSourceMobIDStr);
            std::string fileName = getFileNameFromLocator(pFileSourceMob);
            if (fileName.empty()) {
                fileName = getFileNameFallbackFromSourceMob(pFileSourceMob, fileSourceMobID);
            }
            std::filesystem::path outFilePath = std::filesystem::path(outputDir) / fileName;
            outPath = outFilePath.string();
            std::filesystem::create_directories(std::filesystem::path(outputDir));
            bool extracted = extractEmbeddedEssenceByMobID(fileSourceMobID, outPath);
            pFileSourceMobBase->Release();
            pFileSourceMob->Release();
            if (extracted) {
                logger.info(LogCategory::EXTRACTION, "Successfully extracted: " + outPath);
            } else {
                logger.warn(LogCategory::EXTRACTION, "Failed to extract essence data for: " + fileSourceMobIDStr);
            }
            return extracted;
        } else {
            logger.debug(LogCategory::EXTRACTION, "SourceMob is not embedded");
            pSourceMob->Release();
            pMob->Release();
            return false;
        }
    }
    
    // Если это не SourceMob, возможно это MasterMob
    IAAFMasterMob* pMasterMob = nullptr;
    if (SUCCEEDED(pMob->QueryInterface(IID_IAAFMasterMob, (void**)&pMasterMob))) {
        logger.debug(LogCategory::EXTRACTION, "Mob is MasterMob - looking for FileSourceMob...");
        
        // Нужно найти FileSourceMob из MasterMob - это будет в AAFClipProcessor
        // Временно возвращаем false
        logger.warn(LogCategory::EXTRACTION, "MasterMob processing not implemented in extractor");
        pMasterMob->Release();
        pMob->Release();
        return false;
    }
    
    pMob->Release();
    return false;
}

bool AAFEssenceExtractor::extractAllEmbeddedAudio(const std::vector<AAFAudioTrackInfo>& audioTracks,
                                                 const std::string& outputDir) {
    logger.info(LogCategory::EXTRACTION, "Starting extraction of all embedded audio to: " + outputDir);
    
    int extractedCount = 0;
    int failedCount = 0;
    
    // Создаем выходную директорию
    std::filesystem::create_directories(outputDir);
    
    // Проходим по всем трекам и клипам
    for (const auto& track : audioTracks) {
        logger.debug(LogCategory::EXTRACTION, "Processing track: " + track.trackName);
        
        for (const auto& clip : track.clips) {
            // Проверяем, является ли клип embedded
            if (clip.sourceType == "Embedded" || clip.isEmbedded) {
                std::string extractedPath;
                if (extractEmbeddedAudioClip(clip, outputDir, extractedPath)) {
                    logger.info(LogCategory::EXTRACTION, "✅ Extracted: " + extractedPath);
                    extractedCount++;
                } else {
                    logger.warn(LogCategory::EXTRACTION, "❌ Failed to extract: " + clip.mobID);
                    failedCount++;
                }
            } else {
                logger.debug(LogCategory::EXTRACTION, "Skipping external clip: " + clip.originalFileName);
            }
        }
    }
    
    logger.logSummary("Embedded Audio Extraction", {
        {"Extracted", std::to_string(extractedCount)},
        {"Failed", std::to_string(failedCount)},
        {"Output Directory", outputDir}
    });
    
    return extractedCount > 0;
}

bool AAFEssenceExtractor::parseMobIDString(const std::string& mobIdStr, aafMobID_t& mobID) {
    if (mobIdStr.length() < 36)
        return false; // Минимальная длина UUID

    // Сначала пытаемся найти Mob с совпадающей строкой ID
    aafSearchCrit_t crit;
    crit.searchTag = kAAFByMobKind;
    crit.tags.mobKind = kAAFAllMob;

    IEnumAAFMobs* pEnum = nullptr;
    if (SUCCEEDED(m_pHeader->GetMobs(&crit, &pEnum))) {
        IAAFMob* pMob = nullptr;
        while (SUCCEEDED(pEnum->NextOne(&pMob))) {
            aafMobID_t id;
            if (SUCCEEDED(pMob->GetMobID(&id))) {
                if (formatMobID(id) == mobIdStr) {
                    mobID = id;
                    pMob->Release();
                    pEnum->Release();
                    return true;
                }
            }
            pMob->Release();
        }
        pEnum->Release();
    }

    // Если не нашли полный MobID, парсим только material часть как раньше
    std::string cleanStr = mobIdStr;
    cleanStr.erase(std::remove(cleanStr.begin(), cleanStr.end(), '-'), cleanStr.end());

    if (cleanStr.length() < 32)
        return false;

    try {
        mobID.material.Data1 = static_cast<aafUInt32>(std::stoul(cleanStr.substr(0, 8), nullptr, 16));
        mobID.material.Data2 = static_cast<aafUInt16>(std::stoul(cleanStr.substr(8, 4), nullptr, 16));
        mobID.material.Data3 = static_cast<aafUInt16>(std::stoul(cleanStr.substr(12, 4), nullptr, 16));
        for (int i = 0; i < 8; i++) {
            mobID.material.Data4[i] = static_cast<aafUInt8>(std::stoul(cleanStr.substr(16 + i * 2, 2), nullptr, 16));
        }
    } catch (...) {
        return false;
    }

    // Значения по умолчанию из спецификации AAF
    static const aafUInt8 prefix[12] = {0x06,0x0e,0x2b,0x34,0x04,0x01,0x01,0x01,0x0d,0x01,0x02,0x01};
    memcpy(mobID.SMPTELabel, prefix, sizeof(prefix));
    mobID.length = 0x10;
    mobID.instanceHigh = 0x60;
    mobID.instanceMid = 0x01;
    mobID.instanceLow = 0x01;

    return true;
}

bool AAFEssenceExtractor::checkIfEmbedded(IAAFSourceMob* pSourceMob) {
    if (!pSourceMob) return false;
    
    logger.debug(LogCategory::ESSENCE, "Checking if SourceMob has embedded essence...");
    
    // Получаем MobID SourceMob через IAAFMob интерфейс
    IAAFMob* pMob = nullptr;
    if (FAILED(pSourceMob->QueryInterface(IID_IAAFMob, (void**)&pMob))) {
        logger.warn(LogCategory::ESSENCE, "Failed to query IAAFMob interface");
        return false;
    }
    
    aafMobID_t sourceMobID;
    if (FAILED(pMob->GetMobID(&sourceMobID))) {
        logger.warn(LogCategory::ESSENCE, "Failed to get SourceMob ID");
        pMob->Release();
        return false;
    }
    pMob->Release();
    
    std::string sourceMobIDStr = formatMobID(sourceMobID);
    logger.debug(LogCategory::ESSENCE, "Checking SourceMob ID: " + sourceMobIDStr);
    
    // Сначала проверяем наличие внешних ссылок через Locator
    IAAFEssenceDescriptor* pEssenceDesc = nullptr;
    if (SUCCEEDED(pSourceMob->GetEssenceDescriptor(&pEssenceDesc))) {
        aafUInt32 numLocators = 0;
        if (SUCCEEDED(pEssenceDesc->CountLocators(&numLocators)) && numLocators > 0) {
            logger.debug(LogCategory::ESSENCE, "Found " + std::to_string(numLocators) + " external locators - this is EXTERNAL audio");
            pEssenceDesc->Release();
            return false; // Есть внешние ссылки = external
        }
        pEssenceDesc->Release();
    }
    
    // Если нет внешних локаторов, проверяем наличие embedded EssenceData
    IAAFContentStorage* pStorage = nullptr;
    if (SUCCEEDED(m_pHeader->GetContentStorage(&pStorage))) {
        IAAFEssenceData* pEssenceData = nullptr;
        if (SUCCEEDED(pStorage->LookupEssenceData(sourceMobID, &pEssenceData))) {
            // Проверяем размер данных
            aafLength_t dataSize;
            bool hasData = SUCCEEDED(pEssenceData->GetSize(&dataSize)) && dataSize > 0;
            
            if (hasData) {
                logger.debug(LogCategory::ESSENCE, "Found embedded EssenceData with size: " + std::to_string(dataSize) + " - this is EMBEDDED audio");
            } else {
                logger.debug(LogCategory::ESSENCE, "Found EssenceData but size is 0 - this is EXTERNAL audio");
            }
            
            pEssenceData->Release();
            pStorage->Release();
            return hasData;
        }
        pStorage->Release();
    }
    
    // Fallback к методу через перечисление
    IEnumAAFEssenceData* pEssenceEnum = nullptr;
    if (FAILED(m_pHeader->EnumEssenceData(&pEssenceEnum))) {
        logger.debug(LogCategory::ESSENCE, "No EssenceData enumeration available - assuming EXTERNAL");
        return false;
    }
    
    IAAFEssenceData* pEssenceData = nullptr;
    bool hasEmbeddedData = false;
    
    while (SUCCEEDED(pEssenceEnum->NextOne(&pEssenceData))) {
        aafMobID_t essenceMobID;
        if (SUCCEEDED(pEssenceData->GetFileMobID(&essenceMobID))) {
            if (isSameMobID(sourceMobID, essenceMobID)) {
                aafLength_t dataSize;
                if (SUCCEEDED(pEssenceData->GetSize(&dataSize)) && dataSize > 0) {
                    hasEmbeddedData = true;
                    logger.debug(LogCategory::ESSENCE, "Found embedded EssenceData via enumeration with size: " + std::to_string(dataSize));
                } else {
                    logger.debug(LogCategory::ESSENCE, "Found EssenceData via enumeration but size is 0");
                }
                pEssenceData->Release();
                break;
            }
        }
        pEssenceData->Release();
    }
    
    pEssenceEnum->Release();
    
    if (!hasEmbeddedData) {
        logger.debug(LogCategory::ESSENCE, "No embedded EssenceData found - this is EXTERNAL audio");
    }
    
    logger.debug(LogCategory::ESSENCE, "Final embedded status: " + std::string(hasEmbeddedData ? "EMBEDDED" : "EXTERNAL"));
    return hasEmbeddedData;
}

bool AAFEssenceExtractor::extractEmbeddedEssenceByMobID(const aafMobID_t& mobID, const std::string& outputPath) {
    logger.debug(LogCategory::EXTRACTION, "Extracting essence using LookupEssenceData");
    
    // Используем более эффективный метод LookupEssenceData через ContentStorage
    IAAFContentStorage* pStorage = nullptr;
    if (FAILED(m_pHeader->GetContentStorage(&pStorage))) {
        logger.warn(LogCategory::EXTRACTION, "Failed to get ContentStorage");
        return false;
    }
    
    IAAFEssenceData* pEssence = nullptr;
    HRESULT hr = pStorage->LookupEssenceData(mobID, &pEssence);
    if (FAILED(hr)) {
        logger.debug(LogCategory::EXTRACTION, "No EssenceData found via LookupEssenceData, falling back to enumeration");
        pStorage->Release();
        
        // Fallback к старому методу через перечисление
        return extractEmbeddedEssenceByMobID_Fallback(mobID, outputPath);
    }
    
    // Открываем файл для записи
    std::ofstream outFile(outputPath, std::ios::binary);
    if (!outFile.is_open()) {
        logger.error(LogCategory::EXTRACTION, "Failed to create output file: " + outputPath);
        pEssence->Release();
        pStorage->Release();
        return false;
    }
    
    // Читаем данные блоками для эффективности
    const size_t bufSize = 8192; // 8KB буфер
    std::vector<aafUInt8> buffer(bufSize);
    aafUInt32 readBytes = 0;
    size_t totalBytes = 0;
    
    logger.info(LogCategory::EXTRACTION, "Reading essence data in 8KB chunks...");
    
    while (SUCCEEDED(pEssence->Read(static_cast<aafUInt32>(bufSize), buffer.data(), &readBytes)) && readBytes > 0) {
        outFile.write(reinterpret_cast<const char*>(buffer.data()), readBytes);
        totalBytes += readBytes;
        
        // Логируем прогресс каждые 1MB
        if (totalBytes % (1024 * 1024) == 0) {
            logger.debug(LogCategory::EXTRACTION, "Read " + std::to_string(totalBytes / (1024 * 1024)) + " MB...");
        }
    }
    
    outFile.close();
    logger.info(LogCategory::EXTRACTION, "Successfully extracted " + std::to_string(totalBytes) + " bytes");
    
    pEssence->Release();
    pStorage->Release();
    return true;
}

bool AAFEssenceExtractor::extractEmbeddedEssenceByMobID_Fallback(const aafMobID_t& mobID, const std::string& outputPath) {
    logger.debug(LogCategory::EXTRACTION, "Using fallback enumeration method");
    
    IEnumAAFEssenceData* pEnum = nullptr;
    if (FAILED(m_pHeader->EnumEssenceData(&pEnum))) {
        logger.warn(LogCategory::EXTRACTION, "Failed to get EssenceData enumeration");
        return false;
    }
    
    IAAFEssenceData* pEssence = nullptr;
    bool found = false;
    
    while (SUCCEEDED(pEnum->NextOne(&pEssence))) {
        aafMobID_t essenceMobID;
        if (SUCCEEDED(pEssence->GetFileMobID(&essenceMobID)) && isSameMobID(essenceMobID, mobID)) {
            found = true;
            
            std::ofstream outFile(outputPath, std::ios::binary);
            if (!outFile.is_open()) {
                logger.error(LogCategory::EXTRACTION, "Failed to create output file: " + outputPath);
                pEssence->Release();
                pEnum->Release();
                return false;
            }
            
            const size_t bufSize = 8192;
            std::vector<aafUInt8> buffer(bufSize);
            aafUInt32 readBytes = 0;
            size_t totalBytes = 0;
            
            while (SUCCEEDED(pEssence->Read(static_cast<aafUInt32>(bufSize), buffer.data(), &readBytes)) && readBytes > 0) {
                outFile.write(reinterpret_cast<const char*>(buffer.data()), readBytes);
                totalBytes += readBytes;
            }
            
            outFile.close();
            logger.info(LogCategory::EXTRACTION, "Successfully extracted " + std::to_string(totalBytes) + " bytes via fallback");
            
            pEssence->Release();
            break;
        }
        pEssence->Release();
    }
    
    pEnum->Release();
    return found;
}

std::string AAFEssenceExtractor::getFileNameFromLocator(IAAFSourceMob* pSourceMob) {
    if (!pSourceMob) return "";
    
    logger.debug(LogCategory::EXTRACTION, "Getting filename from EssenceDescriptor Locator...");
    
    IAAFEssenceDescriptor* pDesc = nullptr;
    if (FAILED(pSourceMob->GetEssenceDescriptor(&pDesc))) {
        logger.debug(LogCategory::EXTRACTION, "No EssenceDescriptor found");
        return "";
    }
    
    // Получаем Locators напрямую из EssenceDescriptor
    IEnumAAFLocators* pLocEnum = nullptr;
    if (FAILED(pDesc->GetLocators(&pLocEnum))) {
        logger.debug(LogCategory::EXTRACTION, "No Locators found in EssenceDescriptor");
        pDesc->Release();
        return "";
    }
    
    IAAFLocator* pLoc = nullptr;
    std::string fileName;
    
    while (SUCCEEDED(pLocEnum->NextOne(&pLoc))) {
        // Получаем путь из Locator
        aafWChar filePath[1024] = {0};
        if (SUCCEEDED(pLoc->GetPath(filePath, sizeof(filePath)))) {
            std::wstring wPath(filePath);
            std::string path = wideToUtf8(wPath);
            
            // Извлекаем только имя файла из полного пути
            size_t pos = path.find_last_of("/\\");
            fileName = (pos != std::string::npos) ? path.substr(pos + 1) : path;
            
            logger.debug(LogCategory::EXTRACTION, "Found filename in Locator: " + fileName);
            pLoc->Release();
            break;
        }
        pLoc->Release();
    }
    
    pLocEnum->Release();
    pDesc->Release();
    
    return fileName;
}

std::string AAFEssenceExtractor::getFileNameFallbackFromSourceMob(IAAFSourceMob* pSourceMob, const aafMobID_t& mobID) {
    if (!pSourceMob) {
        std::string extension = getEmbeddedFileExtension(mobID);
        return "essence_" + formatMobID(mobID).substr(0, 8) + extension;
    }
    
    logger.debug(LogCategory::EXTRACTION, "Using fallback filename from SourceMob name...");
    
    IAAFMob* pMob = nullptr;
    if (SUCCEEDED(pSourceMob->QueryInterface(IID_IAAFMob, (void**)&pMob))) {
        aafWChar name[256] = {0};
        if (SUCCEEDED(pMob->GetName(name, sizeof(name)))) {
            std::wstring wName(name);
            std::string mobName = wideToUtf8(wName);
            pMob->Release();
            
            if (!mobName.empty()) {
                // Если имя не содержит расширение, добавляем правильное расширение
                if (mobName.find('.') == std::string::npos) {
                    std::string extension = getEmbeddedFileExtension(mobID);
                    mobName += extension;
                }
                logger.debug(LogCategory::EXTRACTION, "Using SourceMob name as filename: " + mobName);
                return mobName;
            }
        }
        pMob->Release();
    }
    
    // Последний fallback - используем MobID с правильным расширением
    std::string extension = getEmbeddedFileExtension(mobID);
    std::string fallbackName = "essence_" + formatMobID(mobID).substr(0, 8) + extension;
    logger.debug(LogCategory::EXTRACTION, "Using MobID-based fallback: " + fallbackName);
    return fallbackName;
}

std::string AAFEssenceExtractor::detectFileExtension(IAAFEssenceData* pEssenceData) {
    if (!pEssenceData) {
        logger.debug(LogCategory::EXTRACTION, "No EssenceData provided, using default .aif");
        return ".aif";
    }
    
    // Попробуем определить формат по данным
    aafMobID_t fileMobID;
    if (SUCCEEDED(pEssenceData->GetFileMobID(&fileMobID))) {
        return getEmbeddedFileExtension(fileMobID);
    }
    
    logger.debug(LogCategory::EXTRACTION, "Cannot determine format from EssenceData, using default .aif");
    return ".aif";
}

std::string AAFEssenceExtractor::getEmbeddedFileExtension(const aafMobID_t& mobID) {
    // Ищем SourceMob по MobID
    IAAFMob* pMob = nullptr;
    if (FAILED(m_pHeader->LookupMob(mobID, &pMob))) {
        logger.debug(LogCategory::EXTRACTION, "Cannot find Mob for format detection, using default .aif");
        return ".aif";
    }
    
    IAAFSourceMob* pSourceMob = nullptr;
    if (SUCCEEDED(pMob->QueryInterface(IID_IAAFSourceMob, (void**)&pSourceMob))) {
        IAAFEssenceDescriptor* pEssDesc = nullptr;
        if (SUCCEEDED(pSourceMob->GetEssenceDescriptor(&pEssDesc))) {
            std::string extension = determineFormatFromEssenceDescriptor(pEssDesc);
            pEssDesc->Release();
            pSourceMob->Release();
            pMob->Release();
            return extension;
        }
        pSourceMob->Release();
    }
    
    pMob->Release();
    logger.debug(LogCategory::EXTRACTION, "EssenceDescriptor not found, using default .aif for embedded");
    return ".aif";
}

std::string AAFEssenceExtractor::determineFormatFromEssenceDescriptor(IAAFEssenceDescriptor* pEssDesc) {
    if (!pEssDesc) {
        logger.debug(LogCategory::EXTRACTION, "No EssenceDescriptor, using default .aif");
        return ".aif";
    }
    
    // Пытаемся получить DataEssenceDescriptor для информации о кодировке
    IAAFDataEssenceDescriptor* pDataEssDesc = nullptr;
    if (SUCCEEDED(pEssDesc->QueryInterface(IID_IAAFDataEssenceDescriptor, (void**)&pDataEssDesc))) {
        aafUID_t codingUID;
        if (SUCCEEDED(pDataEssDesc->GetDataEssenceCoding(&codingUID))) {
            std::string extension = getExtensionFromCodingUID(codingUID);
            pDataEssDesc->Release();
            return extension;
        }
        pDataEssDesc->Release();
    }
    
    // Пытаемся получить SoundDescriptor для аудио информации
    IAAFSoundDescriptor* pSoundDesc = nullptr;
    if (SUCCEEDED(pEssDesc->QueryInterface(IID_IAAFSoundDescriptor, (void**)&pSoundDesc))) {
        // Получаем информацию о сжатии
        aafUID_t compressionUID;
        if (SUCCEEDED(pSoundDesc->GetCompression(&compressionUID))) {
            std::string extension = getExtensionFromCodingUID(compressionUID);
            pSoundDesc->Release();
            return extension;
        }
        
        // Логируем дополнительную информацию для диагностики
        aafUInt32 channelCount = 0;
        aafRational_t sampleRate = {0, 1};
        aafUInt32 quantizationBits = 0;
        
        if (SUCCEEDED(pSoundDesc->GetChannelCount(&channelCount))) {
            logger.debug(LogCategory::EXTRACTION, "Audio channels: " + std::to_string(channelCount));
        }
        if (SUCCEEDED(pSoundDesc->GetAudioSamplingRate(&sampleRate))) {
            double rate = (double)sampleRate.numerator / (double)sampleRate.denominator;
            logger.debug(LogCategory::EXTRACTION, "Sample rate: " + std::to_string(rate) + " Hz");
        }
        if (SUCCEEDED(pSoundDesc->GetQuantizationBits(&quantizationBits))) {
            logger.debug(LogCategory::EXTRACTION, "Bit depth: " + std::to_string(quantizationBits) + " bits");
        }
        
        pSoundDesc->Release();
    }
    
    // По умолчанию для embedded аудио возвращаем AIFF
    logger.debug(LogCategory::EXTRACTION, "No specific coding found, using default .aif for embedded audio");
    return ".aif";
}

std::string AAFEssenceExtractor::getExtensionFromCodingUID(const aafUID_t& codingUID) {
    // Определяем известные кодировки AAF
    const aafUID_t kAAFCodecDef_WAVE = {0x820f09b1, 0xeb9b, 0x11d2, {0x80, 0x9f, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f}};
    const aafUID_t kAAFCodecDef_AIFC = {0x4b1c1a45, 0x03f2, 0x11d4, {0x80, 0xfb, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f}};
    const aafUID_t kAAFCodecDef_PCM = {0x90ac17c8, 0xe3e2, 0x4596, {0x9e, 0x9e, 0xa6, 0xdd, 0x1c, 0x70, 0xc8, 0x92}};
    
    // Сравниваем UID
    if (memcmp(&codingUID, &kAAFCodecDef_WAVE, sizeof(aafUID_t)) == 0) {
        logger.debug(LogCategory::EXTRACTION, "Detected WAVE codec -> .wav");
        return ".wav";
    }
    else if (memcmp(&codingUID, &kAAFCodecDef_AIFC, sizeof(aafUID_t)) == 0) {
        logger.debug(LogCategory::EXTRACTION, "Detected AIFC codec -> .aif");
        return ".aif";
    }
    else if (memcmp(&codingUID, &kAAFCodecDef_PCM, sizeof(aafUID_t)) == 0) {
        logger.debug(LogCategory::EXTRACTION, "Detected PCM codec -> .aif (default for uncompressed)");
        return ".aif";
    }
    else {
        // Логируем UID для отладки
        char uidStr[256];
        sprintf(uidStr, "Unknown codec UID: {%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x}",
            codingUID.Data1, codingUID.Data2, codingUID.Data3,
            codingUID.Data4[0], codingUID.Data4[1], codingUID.Data4[2], codingUID.Data4[3],
            codingUID.Data4[4], codingUID.Data4[5], codingUID.Data4[6], codingUID.Data4[7]);
        logger.debug(LogCategory::EXTRACTION, std::string(uidStr) + " -> .aif (default)");
        return ".aif";
    }
}
