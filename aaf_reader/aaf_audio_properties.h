#pragma once

#include "data_structures.h"
#include "debug_logger.h"
#include <AAF.h>

class AAFAudioProperties {
public:
    AAFAudioProperties(DebugLogger& logger);
    ~AAFAudioProperties();

    // Получение аудио свойств из EssenceDescriptor
    bool getAudioProperties(IAAFSourceMob* pSourceMob, AAFAudioClipInfo& clipInfo);

    // Определение типа сжатия из EssenceDescriptor
    std::string getCompressionTypeFromDescriptor(IAAFEssenceDescriptor* pEssDesc);

    // Проверка DataDef на соответствие аудио
    bool isAudioDataDef(IAAFDataDef* pDataDef);

private:
    DebugLogger& logger;
};
