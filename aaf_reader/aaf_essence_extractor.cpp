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
#include <unordered_map>

AAFEssenceExtractor::AAFEssenceExtractor(IAAFHeader* pHeader, DebugLogger& logger)
    : m_pHeader(pHeader), logger(logger) {
}

AAFEssenceExtractor::~AAFEssenceExtractor() {
}

bool AAFEssenceExtractor::extractEmbeddedAudioClip(const AAFAudioClipInfo& clip, 
                                                  const std::string& outputDir, 
                                                  std::string& outPath) {
    logger.debug(LogCategory::EXTRACTION, "Attempting to extract clip. Provided MobID string: " + clip.mobID);
    
    // 1. Parse the MobID string using the new robust utility function
    aafMobID_t mobID;
    if (!aaf_utils::parseMobIDFull(clip.mobID, mobID)) {
        logger.warn(LogCategory::EXTRACTION, "Failed to parse MobID string: " + clip.mobID);
        return false;
    }
    
    // Log the parsed MobID for verification
    logger.debug(LogCategory::EXTRACTION, "Successfully parsed MobID: " + aaf_utils::formatMobIDFull(mobID));

    // 2. Look up the Mob by its ID
    IAAFMob* pMob = nullptr;
    if (FAILED(m_pHeader->LookupMob(mobID, &pMob))) {
        logger.warn(LogCategory::EXTRACTION, "Failed to find Mob with ID: " + clip.mobID);
        return false;
    }
    
    // 3. Проверяем, что это MasterMob
    IAAFMasterMob* pMasterMob = nullptr;
    HRESULT hr = pMob->QueryInterface(IID_IAAFMasterMob, (void**)&pMasterMob);
    if (FAILED(hr)) {
        logger.debug(LogCategory::EXTRACTION, "Mob is not a MasterMob, trying as SourceMob directly");
        
        // Если это не MasterMob, попробуем как SourceMob
        IAAFSourceMob* pSourceMob = nullptr;
        if (SUCCEEDED(pMob->QueryInterface(IID_IAAFSourceMob, (void**)&pSourceMob))) {
            bool result = extractFromSourceMob(pSourceMob, clip.originalFileName, outputDir, outPath);
            pSourceMob->Release();
            pMob->Release();
            return result;
        }
        
        logger.warn(LogCategory::EXTRACTION, "Mob is neither MasterMob nor SourceMob");
        pMob->Release();
        return false;
    }
    
    logger.debug(LogCategory::EXTRACTION, "Found MasterMob, looking for embedded essence");
    
    // 4. Получаем слоты MasterMob
    IEnumAAFMobSlots* pSlotEnum = nullptr;
    if (FAILED(pMob->GetSlots(&pSlotEnum))) {
        logger.warn(LogCategory::EXTRACTION, "Failed to get slots from MasterMob");
        pMasterMob->Release();
        pMob->Release();
        return false;
    }
    
    bool extracted = false;
    IAAFMobSlot* pSlot = nullptr;
    
    // 5. Проходим по всем слотам
    while (!extracted && SUCCEEDED(pSlotEnum->NextOne(&pSlot))) {
        aafSlotID_t slotID = 0;
        if (SUCCEEDED(pSlot->GetSlotID(&slotID))) {
            logger.debug(LogCategory::EXTRACTION, "Checking slot ID: " + std::to_string(slotID));
            
            // 6. Для каждого слота получаем связанный SourceClip
            IAAFSegment* pSegment = nullptr;
            if (SUCCEEDED(pSlot->GetSegment(&pSegment))) {
                IAAFSourceClip* pSourceClip = nullptr;
                
                // Проверяем, является ли сегмент SourceClip напрямую
                if (SUCCEEDED(pSegment->QueryInterface(IID_IAAFSourceClip, (void**)&pSourceClip))) {
                    // 7. Получаем SourceRef (ссылку на FileSourceMob)
                    aafSourceRef_t sourceRef;
                    if (SUCCEEDED(pSourceClip->GetSourceReference(&sourceRef))) {
                        // 8. Ищем FileSourceMob по SourceRef
                        IAAFMob* pSourceMobBase = nullptr;
                        if (SUCCEEDED(m_pHeader->LookupMob(sourceRef.sourceID, &pSourceMobBase))) {
                            IAAFSourceMob* pSourceMob = nullptr;
                            if (SUCCEEDED(pSourceMobBase->QueryInterface(IID_IAAFSourceMob, (void**)&pSourceMob))) {
                                // 9. Извлекаем данные из FileSourceMob
                                extracted = extractFromSourceMob(pSourceMob, clip.originalFileName, outputDir, outPath);
                                pSourceMob->Release();
                            }
                            pSourceMobBase->Release();
                        }
                    }
                    pSourceClip->Release();
                } 
                // Проверка на Sequence (часто MasterMob содержит последовательности)
                else {
                    IAAFSequence* pSequence = nullptr;
                    if (SUCCEEDED(pSegment->QueryInterface(IID_IAAFSequence, (void**)&pSequence))) {
                        logger.debug(LogCategory::EXTRACTION, "Found Sequence in slot, looking for SourceClips inside");
                        
                        // Проходим по компонентам последовательности
                        IEnumAAFComponents* pCompEnum = nullptr;
                        if (SUCCEEDED(pSequence->GetComponents(&pCompEnum))) {
                            IAAFComponent* pComp = nullptr;
                            while (!extracted && SUCCEEDED(pCompEnum->NextOne(&pComp))) {
                                IAAFSourceClip* pSeqSourceClip = nullptr;
                                if (SUCCEEDED(pComp->QueryInterface(IID_IAAFSourceClip, (void**)&pSeqSourceClip))) {
                                    // Получаем SourceRef
                                    aafSourceRef_t sourceRef;
                                    if (SUCCEEDED(pSeqSourceClip->GetSourceReference(&sourceRef))) {
                                        // Ищем FileSourceMob
                                        IAAFMob* pSourceMobBase = nullptr;
                                        if (SUCCEEDED(m_pHeader->LookupMob(sourceRef.sourceID, &pSourceMobBase))) {
                                            IAAFSourceMob* pSourceMob = nullptr;
                                            if (SUCCEEDED(pSourceMobBase->QueryInterface(IID_IAAFSourceMob, (void**)&pSourceMob))) {
                                                // Извлекаем данные
                                                extracted = extractFromSourceMob(pSourceMob, clip.originalFileName, outputDir, outPath);
                                                pSourceMob->Release();
                                            }
                                            pSourceMobBase->Release();
                                        }
                                    }
                                    pSeqSourceClip->Release();
                                }
                                pComp->Release();
                            }
                            pCompEnum->Release();
                        }
                        pSequence->Release();
                    }
                }
                pSegment->Release();
            }
        }
        pSlot->Release();
    }
    
    pSlotEnum->Release();
    pMasterMob->Release();
    pMob->Release();
    
    if (!extracted) {
        logger.warn(LogCategory::EXTRACTION, "No embedded essence found in MasterMob");
    }
    
    return extracted;
}

// Новая вспомогательная функция для извлечения из SourceMob
bool AAFEssenceExtractor::extractFromSourceMob(IAAFSourceMob* pSourceMob, 
                                              const std::string& originalFileName,
                                              const std::string& outputDir, 
                                              std::string& outPath) {
    if (!pSourceMob) {
        logger.error(LogCategory::EXTRACTION, "Null SourceMob provided");
        return false;
    }
    
    // Get the MobID
    IAAFMob* pMob = nullptr;
    if (FAILED(pSourceMob->QueryInterface(IID_IAAFMob, (void**)&pMob))) {
        logger.error(LogCategory::EXTRACTION, "Failed to get IAAFMob interface from SourceMob");
        return false;
    }
    
    aafMobID_t mobID;
    if (FAILED(pMob->GetMobID(&mobID))) {
        logger.error(LogCategory::EXTRACTION, "Failed to get MobID from SourceMob");
        pMob->Release();
        return false;
    }
    
    std::string mobIDStr = aaf_utils::formatMobIDFull(mobID);
    logger.debug(LogCategory::EXTRACTION, "Looking for EssenceData for MobID: " + mobIDStr);
    
    // Get ContentStorage to find EssenceData
    IAAFContentStorage* pStorage = nullptr;
    if (FAILED(m_pHeader->GetContentStorage(&pStorage))) {
        logger.error(LogCategory::EXTRACTION, "Failed to get ContentStorage");
        pMob->Release();
        return false;
    }
    
    // Look up essence data by mob ID
    IAAFEssenceData* pEssenceData = nullptr;
    HRESULT hr = pStorage->LookupEssenceData(mobID, &pEssenceData);
    if (FAILED(hr) || !pEssenceData) {
        logger.warn(LogCategory::EXTRACTION, "Failed to find EssenceData for MobID: " + mobIDStr + ". This is normal for non-embedded clips.");
        pStorage->Release();
        pMob->Release();
        return false;
    }
    
    // Get the size
    aafLength_t dataSize = 0;
    if (FAILED(pEssenceData->GetSize(&dataSize)) || dataSize == 0) {
        logger.error(LogCategory::EXTRACTION, "EssenceData has zero size");
        pEssenceData->Release();
        pStorage->Release();
        pMob->Release();
        return false;
    }
    
    logger.info(LogCategory::EXTRACTION, "Found EssenceData with size: " + std::to_string(dataSize));
    
    // Create output path
    std::string fileName = generateProperFileName(originalFileName, mobIDStr);
    std::filesystem::path outputPath = std::filesystem::path(outputDir) / fileName;
    outPath = outputPath.string();
    
    // Create the output directory if needed
    std::filesystem::create_directories(std::filesystem::path(outputDir));
    
    // Check if file already exists
    if (std::filesystem::exists(outputPath)) {
        logger.info(LogCategory::EXTRACTION, "File already exists: " + outPath);
        // Still return success since we have the file
        pEssenceData->Release();
        pStorage->Release();
        pMob->Release();
        return true;
    }
    
    // Extract essence data to file
    bool result = extractEssenceDataToFile(pEssenceData, outPath);
    
    pEssenceData->Release();
    pStorage->Release();
    pMob->Release();
    
    return result;
}

// Новая вспомогательная функция для записи EssenceData в файл
bool AAFEssenceExtractor::extractEssenceDataToFile(IAAFEssenceData* pEssenceData, const std::string& filePath) {
    if (!pEssenceData) {
        logger.error(LogCategory::EXTRACTION, "Null EssenceData provided");
        return false;
    }
    
    // Open the output file
    std::ofstream outFile(filePath, std::ios::binary);
    if (!outFile.is_open()) {
        logger.error(LogCategory::EXTRACTION, "Failed to create output file: " + filePath);
        return false;
    }
    
    // Rewind to start of essence data
    if (FAILED(pEssenceData->SetPosition(0))) {
        logger.error(LogCategory::EXTRACTION, "Failed to set position to start of EssenceData");
        outFile.close();
        return false;
    }
    
    // Read in chunks
    const size_t BUFFER_SIZE = 8192;  // 8KB buffer
    std::vector<aafUInt8> buffer(BUFFER_SIZE);
    aafUInt32 bytesRead = 0;
    size_t totalBytes = 0;
    
    logger.info(LogCategory::EXTRACTION, "Extracting audio data to: " + filePath);
    
    while (true) {
        // Read a chunk
        HRESULT hr = pEssenceData->Read(static_cast<aafUInt32>(BUFFER_SIZE), buffer.data(), &bytesRead);
        
        if (FAILED(hr)) {
            logger.error(LogCategory::EXTRACTION, "Error reading EssenceData");
            outFile.close();
            return false;
        }
        
        if (bytesRead == 0) {
            // End of data
            break;
        }
        
        // Write to file
        outFile.write(reinterpret_cast<const char*>(buffer.data()), bytesRead);
        totalBytes += bytesRead;
        
        // Log progress for large files
        if (totalBytes % (1024 * 1024) == 0) {  // Every 1MB
            logger.debug(LogCategory::EXTRACTION, "Extracted " + std::to_string(totalBytes / (1024 * 1024)) + " MB");
        }
    }
    
    outFile.close();
    logger.info(LogCategory::EXTRACTION, "Successfully extracted " + std::to_string(totalBytes) + " bytes");
    return true;
}

bool AAFEssenceExtractor::extractAllEmbeddedAudio(const std::vector<AAFAudioTrackInfo>& audioTracks, 
                                                const std::string& outputDir) {
    logger.info(LogCategory::EXTRACTION, "Starting extraction of all embedded audio files");
    
    // Ensure output directory exists
    std::filesystem::create_directories(outputDir);
    
    int totalClips = 0;
    int extractedClips = 0;
    
    // Clear file counters for proper naming
    fileCounters.clear();
    
    // Process all tracks
    for (const auto& track : audioTracks) {
        logger.debug(LogCategory::EXTRACTION, "Processing track: " + track.trackName);
        
        // Process all clips in track
        for (const auto& clip : track.clips) {
            totalClips++;
            
            // Attempt to extract this clip
            std::string outPath;
            if (extractEmbeddedAudioClip(clip, outputDir, outPath)) {
                extractedClips++;
                logger.info(LogCategory::EXTRACTION, "Extracted clip " + std::to_string(extractedClips) + 
                            " of " + std::to_string(totalClips) + ": " + outPath);
            }
        }
    }
    
    logger.info(LogCategory::EXTRACTION, "Extraction complete. Successfully extracted " + 
                std::to_string(extractedClips) + " of " + std::to_string(totalClips) + 
                " audio clips");
                
    return extractedClips > 0;
}

std::string AAFEssenceExtractor::generateProperFileName(const std::string& originalFileName, const std::string& mobID) {
    // Base name is the original file name without any extension
    std::string baseName = originalFileName;
    
    // Remove any directory paths
    size_t lastSlash = baseName.find_last_of("/\\");
    if (lastSlash != std::string::npos) {
        baseName = baseName.substr(lastSlash + 1);
    }
    
    // Remove any file extension
    size_t lastDot = baseName.find_last_of('.');
    if (lastDot != std::string::npos) {
        baseName = baseName.substr(0, lastDot);
    }
    
    // If baseName is empty or invalid, generate a name from MobID
    if (baseName.empty()) {
        baseName = "Audio_" + mobID.substr(0, 8);
    }
    
    // Default extension for AAF embedded audio (.aif or .wav is common)
    std::string extension = ".aif";
    
    // Check if we've already extracted a file with this basename
    int counter = 0;
    auto it = fileCounters.find(baseName);
    if (it != fileCounters.end()) {
        counter = ++it->second;
    } else {
        counter = 1;
        fileCounters[baseName] = counter;
    }
    
    // Format the filename as "BaseName-XX.extension"
    std::string result;
    if (counter == 1) {
        // First file with this name doesn't need a counter
        result = baseName + extension;
    } else {
        // Format the counter with leading zeros (01, 02, etc.)
        char counterStr[8];
        snprintf(counterStr, sizeof(counterStr), "-%02d", counter);
        result = baseName + std::string(counterStr) + extension;
    }
    
    // Replace any invalid filename characters
    std::replace(result.begin(), result.end(), ':', '_');
    std::replace(result.begin(), result.end(), '?', '_');
    std::replace(result.begin(), result.end(), '*', '_');
    std::replace(result.begin(), result.end(), '\"', '_');
    std::replace(result.begin(), result.end(), '<', '_');
    std::replace(result.begin(), result.end(), '>', '_');
    std::replace(result.begin(), result.end(), '|', '_');
    
    logger.debug(LogCategory::EXTRACTION, "Generated filename: " + result);
    return result;
}

bool AAFEssenceExtractor::checkIfEmbedded(IAAFSourceMob* pSourceMob) {
    if (!pSourceMob) {
        return false;
    }
    
    // Get the essence descriptor from the SourceMob
    IAAFEssenceDescriptor* pEssenceDesc = nullptr;
    if (FAILED(pSourceMob->GetEssenceDescriptor(&pEssenceDesc))) {
        return false;
    }
    
    // Check if this is a FileDescriptor (external file) or embedded
    IAAFFileDescriptor* pFileDesc = nullptr;
    bool isFile = SUCCEEDED(pEssenceDesc->QueryInterface(IID_IAAFFileDescriptor, (void**)&pFileDesc));
    
    if (pFileDesc) {
        pFileDesc->Release();
    }
    pEssenceDesc->Release();
    
    // If it's a FileDescriptor, it could be external or embedded
    // We need to check if there's corresponding EssenceData
    aafMobID_t mobID;
    IAAFMob* pMob = nullptr;
    if (FAILED(pSourceMob->QueryInterface(IID_IAAFMob, (void**)&pMob))) {
        return false;
    }
    
    HRESULT hr = pMob->GetMobID(&mobID);
    pMob->Release();
    if (FAILED(hr)) {
        return false;
    }
    
    // Try to find EssenceData with the same MobID
    IAAFEssenceData* pEssenceData = nullptr;
    bool hasEssenceData = SUCCEEDED(m_pHeader->LookupEssenceData(mobID, &pEssenceData));
    if (pEssenceData) {
        pEssenceData->Release();
    }
    
    return hasEssenceData;
}

std::string AAFEssenceExtractor::getFileNameFromLocator(IAAFSourceMob* pSourceMob) {
    if (!pSourceMob) {
        return "";
    }
    
    // For now, return empty string - this is a complex operation that requires
    // proper AAF SDK interface knowledge
    // TODO: Implement proper locator parsing
    return "";
}

std::string AAFEssenceExtractor::getFileNameFallbackFromSourceMob(IAAFSourceMob* pSourceMob, const aafMobID_t& mobID) {
    if (!pSourceMob) {
        return "";
    }
    
    // Try to get the mob name through the IAAFMob interface
    IAAFMob* pMob = nullptr;
    if (SUCCEEDED(pSourceMob->QueryInterface(IID_IAAFMob, (void**)&pMob))) {
        aafUInt32 nameBufSize = 0;
        pMob->GetNameBufLen(&nameBufSize);
        if (nameBufSize > 0) {
            std::vector<aafCharacter> nameBuffer(nameBufSize / sizeof(aafCharacter));
            if (SUCCEEDED(pMob->GetName(nameBuffer.data(), nameBufSize))) {
                std::string mobName = wideToUtf8(nameBuffer.data());
                if (!mobName.empty()) {
                    pMob->Release();
                    return mobName;
                }
            }
        }
        pMob->Release();
    }
    
    // If no name, create a fallback name from MobID
    std::string mobIdStr = aaf_utils::formatMobIDFull(mobID);
    // Take first 8 characters for a shorter name
    if (mobIdStr.length() >= 8) {
        return "Audio_" + mobIdStr.substr(0, 8);
    }
    
    return "Audio_Unknown";
}

bool AAFEssenceExtractor::parseMobIDString(const std::string& mobIdStr, aafMobID_t& mobID) {
    // Use the new utility function
    return aaf_utils::parseMobIDFull(mobIdStr, mobID);
}
