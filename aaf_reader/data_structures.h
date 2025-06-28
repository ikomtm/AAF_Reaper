#pragma once

#include <string>
#include <vector>
#include <AAFTypes.h>

// Простые структуры для экспорта в CSV
struct AudioClipData {
    std::string fileName;
    std::string mobID;
    double timelineStart;
    double timelineEnd;
    double sourceStart;
    double length;
    double gain;
    double volume;
    double pan;
    std::vector<std::string> effects;
    
    // Fade параметры для Reaper
    double fadeInLength;      // Длительность fade-in в секундах
    double fadeOutLength;     // Длительность fade-out в секундах
    double crossfadeLength;   // Длительность crossfade в секундах
    std::string fadeInType;   // Тип fade-in кривой
    std::string fadeOutType;  // Тип fade-out кривой
    
    // Конструктор с инициализацией по умолчанию
    AudioClipData() : fileName(""), mobID(""), timelineStart(0.0), timelineEnd(0.0), 
                     sourceStart(0.0), length(0.0), gain(0.0), volume(1.0), pan(0.0),
                     fadeInLength(0.0), fadeOutLength(0.0), crossfadeLength(0.0),
                     fadeInType("Linear"), fadeOutType("Linear") {}
};

struct AudioTrackData {
    int trackIndex;
    std::string trackName;
    std::string trackType;
    double volume;
    double pan;
    bool mute;
    bool solo;
    std::vector<AudioClipData> clips;
    
    // Конструктор с инициализацией по умолчанию
    AudioTrackData() : trackIndex(0), trackName(""), trackType("Audio"), 
                      volume(1.0), pan(0.0), mute(false), solo(false) {}
};

struct ProjectData {
    std::string projectName;
    double sampleRate;
    int channelCount;
    double sessionStartTimecode;
    std::string timecodeFormat;
    double totalLength;
    std::vector<AudioTrackData> tracks;
    
    // Конструктор с инициализацией по умолчанию
    ProjectData() : projectName("AAF_Import_Project"), sampleRate(48000.0), 
                   channelCount(2), sessionStartTimecode(0.0), 
                   timecodeFormat("25fps"), totalLength(0.0) {}
};

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
