#include "aaf_audio_properties.h"
#include "aaf_utils.h"
#include <AAFTypeDefUIDs.h>
#include <AAFClassDefUIDs.h>
#include <AAFDataDefs.h>
#include <cstring>

AAFAudioProperties::AAFAudioProperties(DebugLogger& logger)
    : logger(logger) {
}

AAFAudioProperties::~AAFAudioProperties() {
}

bool AAFAudioProperties::getAudioProperties(IAAFSourceMob* pSourceMob, AAFAudioClipInfo& clipInfo) {
    if (!pSourceMob) return false;
    
    logger.debug(LogCategory::ESSENCE, "Getting audio properties from EssenceDescriptor...");
    
    IAAFEssenceDescriptor* pEssenceDesc = nullptr;
    if (FAILED(pSourceMob->GetEssenceDescriptor(&pEssenceDesc))) {
        logger.error(LogCategory::ESSENCE, "Failed to get EssenceDescriptor");
        return false;
    }
    
    // Пытаемся получить SoundDescriptor
    IAAFSoundDescriptor* pSoundDesc = nullptr;
    if (SUCCEEDED(pEssenceDesc->QueryInterface(IID_IAAFSoundDescriptor, (void**)&pSoundDesc))) {
        // Sample Rate
        if (SUCCEEDED(pSoundDesc->GetAudioSamplingRate(&clipInfo.sampleRate))) {
            double rate = (double)clipInfo.sampleRate.numerator / (double)clipInfo.sampleRate.denominator;
            logger.debug(LogCategory::ESSENCE, "Sample Rate: " + std::to_string(rate) + " Hz");
        }
        
        // Channel Count
        if (SUCCEEDED(pSoundDesc->GetChannelCount(&clipInfo.channelCount))) {
            logger.debug(LogCategory::ESSENCE, "Channels: " + std::to_string(clipInfo.channelCount));
        }
        
        // Bits Per Sample
        if (SUCCEEDED(pSoundDesc->GetQuantizationBits(&clipInfo.bitsPerSample))) {
            logger.debug(LogCategory::ESSENCE, "Bit Depth: " + std::to_string(clipInfo.bitsPerSample) + " bits");
        }
        
        pSoundDesc->Release();
    }
    
    // Получаем тип сжатия
    clipInfo.compressionType = getCompressionTypeFromDescriptor(pEssenceDesc);
    
    // Пытаемся получить имя файла из FileDescriptor
    IAAFFileDescriptor* pFileDesc = nullptr;
    if (SUCCEEDED(pEssenceDesc->QueryInterface(IID_IAAFFileDescriptor, (void**)&pFileDesc))) {
        // Здесь можно попытаться получить имя файла, если есть такой метод
        // В AAF SDK нет прямого GetName для FileDescriptor, но есть другие способы
        pFileDesc->Release();
    }
    
    pEssenceDesc->Release();
    return true;
}

std::string AAFAudioProperties::getCompressionTypeFromDescriptor(IAAFEssenceDescriptor* pEssDesc) {
    if (!pEssDesc) return "Unknown";
    
    // Проверяем различные типы дескрипторов
    IAAFWAVEDescriptor* pWaveDesc = nullptr;
    IAAFAIFCDescriptor* pAifcDesc = nullptr;
    
    if (SUCCEEDED(pEssDesc->QueryInterface(IID_IAAFWAVEDescriptor, (void**)&pWaveDesc))) {
        pWaveDesc->Release();
        return "WAVE";
    } else if (SUCCEEDED(pEssDesc->QueryInterface(IID_IAAFAIFCDescriptor, (void**)&pAifcDesc))) {
        pAifcDesc->Release();
        return "AIFC";
    }
    
    // Проверяем через SoundDescriptor compression
    IAAFSoundDescriptor* pSoundDesc = nullptr;
    if (SUCCEEDED(pEssDesc->QueryInterface(IID_IAAFSoundDescriptor, (void**)&pSoundDesc))) {
        aafUID_t compressionUID;
        if (SUCCEEDED(pSoundDesc->GetCompression(&compressionUID))) {
            // Здесь можно сравнить с известными UID сжатия
            pSoundDesc->Release();
            return "Compressed";
        }
        pSoundDesc->Release();
    }
    
    return "PCM";
}

bool AAFAudioProperties::isAudioDataDef(IAAFDataDef* pDataDef) {
    if (!pDataDef) return false;
    
    // Получаем DefObject для доступа к AUID
    IAAFDefObject* pDefObj = nullptr;
    if (FAILED(pDataDef->QueryInterface(IID_IAAFDefObject, (void**)&pDefObj))) {
        return false;
    }
    
    aafUID_t dataDefID;
    bool isAudio = false;
    if (SUCCEEDED(pDefObj->GetAUID(&dataDefID))) {
        // Сравниваем с kAAFDataDef_Sound
        isAudio = (memcmp(&dataDefID, &kAAFDataDef_Sound, sizeof(aafUID_t)) == 0);
    }
    
    pDefObj->Release();
    return isAudio;
}
