#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING

#include <AAF.h>
#include <AAFResult.h>
#include <AAFTypes.h>
#include <AAFTypeDefUIDs.h>
#include <AAFDataDefs.h>
#include <AAFFileKinds.h>

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <memory>
#include <iomanip>
#include <chrono>

// Наши модули
#include "data_structures.h"
#include "aaf_utils.h"
#include "aaf_proper_parser.h"
#include "media_utils.h"
#include "csv_export.h"

int main(int argc, char* argv[]) {
    // Проверяем на помощь в первую очередь
    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "--help") {
            std::cout << "Usage: aaf_reader <file.aaf> [options]" << std::endl;
            std::cout << "Options:" << std::endl;
            std::cout << "  --log-level=LEVEL    Set log level (ERROR, WARN, INFO, DEBUG, TRACE)" << std::endl;
            std::cout << "  --help               Show this help message" << std::endl;
            std::cout << "\nLevels explanation:" << std::endl;
            std::cout << "  ERROR: Only critical errors" << std::endl;
            std::cout << "  WARN:  Errors and warnings" << std::endl;
            std::cout << "  INFO:  General information (default)" << std::endl;
            std::cout << "  DEBUG: Detailed debugging information" << std::endl;
            std::cout << "  TRACE: Maximum verbosity" << std::endl;
            return 0;
        }
    }
    
    if (argc < 2) {
        std::cerr << "ERROR: Usage: aaf_reader <file.aaf> [--log-level=LEVEL]" << std::endl;
        std::cerr << "  LEVEL can be: ERROR, WARN, INFO, DEBUG, TRACE" << std::endl;
        std::cerr << "Use --help for more information." << std::endl;
        return 1;
    }
    
    std::string aafFilePath = argv[1];
    LogLevel logLevel = LogLevel::LOG_INFO; // По умолчанию
    
    // Парсим аргументы командной строки
    for (int i = 2; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg.substr(0, 12) == "--log-level=") {
            std::string levelStr = arg.substr(12);
            if (levelStr == "ERROR") logLevel = LogLevel::LOG_ERROR;
            else if (levelStr == "WARN") logLevel = LogLevel::LOG_WARN;
            else if (levelStr == "INFO") logLevel = LogLevel::LOG_INFO;
            else if (levelStr == "DEBUG") logLevel = LogLevel::LOG_DEBUG;
            else if (levelStr == "TRACE") logLevel = LogLevel::LOG_TRACE;
            else {
                std::cerr << "WARNING: Unknown log level: " << levelStr << std::endl;
            }
        }
    }

    ProjectData projectData;
    
    // Переменные для статистики
    int audioTrackCount = 0;
    int audioClipCount = 0;
    int audioFadeCount = 0;
    int audioEffectCount = 0;

    // Создаем папки для медиафайлов
    MediaUtils::createExtractedMediaFolder();

    std::ofstream out("output.txt");
    out << "[*] Opening AAF file..." << std::endl;
    out << "[*] Created extracted_media and audio_files folders" << std::endl;
    out << "[*] Log level: " << static_cast<int>(logLevel) << std::endl;

    // Конвертируем путь в wide string  
    size_t len = strlen(aafFilePath.c_str()) + 1;
    std::wstring widePath(len, L'\0');
    size_t convertedChars = 0;
    mbstowcs_s(&convertedChars, &widePath[0], len, aafFilePath.c_str(), _TRUNCATE);

    // Открываем AAF файл
    IAAFFile* pFile = nullptr;
    if (FAILED(AAFFileOpenExistingRead(widePath.c_str(), 0, &pFile)) || !pFile) {
        out << "ERROR: Failed to open file." << std::endl;
        return 1;
    }

    IAAFHeader* pHeader = nullptr;
    if (FAILED(pFile->GetHeader(&pHeader))) {
        out << "ERROR: Failed to get header." << std::endl;
        pFile->Close(); 
        pFile->Release();
        return 1;
    }

    std::map<std::string, std::string> mobIdToName;

    // Инициализируем новый парсер
    out << "[*] Initializing AAF Proper Parser..." << std::endl;
    AAFProperParser properParser(pHeader, out, logLevel);
    
    // Парсим AAF файл согласно спецификации
    out << "[*] Parsing AAF file using Edit Protocol chain..." << std::endl;
    auto startTime = std::chrono::high_resolution_clock::now();
    
    if (!properParser.parseAAFFile()) {
        out << "ERROR: Failed to parse AAF file properly." << std::endl;
        pHeader->Release();
        pFile->Close(); 
        pFile->Release();
        return 1;
    }
    
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    properParser.getLogger().logPerformanceMetric("AAF Parsing", static_cast<double>(duration.count()));
    
    // Получаем результаты парсинга
    std::vector<AAFAudioTrackInfo> parsedTracks = properParser.getAudioTracks();
    out << "[*] Found " << parsedTracks.size() << " audio tracks" << std::endl;
    
    // Извлекаем все embedded аудио файлы
    out << "[*] Extracting embedded audio files..." << std::endl;
    startTime = std::chrono::high_resolution_clock::now();
    
    if (!properParser.extractAllEmbeddedAudio("extracted_media")) {
        out << "WARNING: Some embedded audio files could not be extracted." << std::endl;
    }
    
    endTime = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    properParser.getLogger().logPerformanceMetric("Audio Extraction", static_cast<double>(duration.count()));
    
    // Преобразуем данные в формат ProjectData для CSV экспорта
    projectData.projectName = "AAF_Import_Project";
    projectData.sessionStartTimecode = 0.0;
    
    int totalClips = 0;
    for (size_t trackIdx = 0; trackIdx < parsedTracks.size(); ++trackIdx) {
        const AAFAudioTrackInfo& parsedTrack = parsedTracks[trackIdx];
        
        AudioTrackData trackData;
        trackData.trackIndex = static_cast<int>(trackIdx);
        trackData.trackName = parsedTrack.trackName;
        trackData.trackType = "Audio";
        
        for (const AAFAudioClipInfo& parsedClip : parsedTrack.clips) {
            AudioClipData clipData;
            clipData.fileName = parsedClip.originalFileName;
            if (!parsedClip.extractedFilePath.empty()) {
                clipData.fileName = parsedClip.extractedFilePath;
            }
            clipData.timelineStart = (double)parsedClip.timelineStart / parsedTrack.editRate.numerator * parsedTrack.editRate.denominator;
            clipData.timelineEnd = (double)parsedClip.timelineEnd / parsedTrack.editRate.numerator * parsedTrack.editRate.denominator;
            clipData.sourceStart = (double)parsedClip.sourceStart / parsedTrack.editRate.numerator * parsedTrack.editRate.denominator;
            clipData.length = (double)parsedClip.duration / parsedTrack.editRate.numerator * parsedTrack.editRate.denominator;
            clipData.mobID = parsedClip.mobID;
            
            // Фейды
            if (parsedClip.hasFadeIn) {
                clipData.fadeInLength = (double)parsedClip.fadeInLength / parsedTrack.editRate.numerator * parsedTrack.editRate.denominator;
            }
            if (parsedClip.hasFadeOut) {
                clipData.fadeOutLength = (double)parsedClip.fadeOutLength / parsedTrack.editRate.numerator * parsedTrack.editRate.denominator;
            }
            
            // Эффекты
            clipData.effects = parsedClip.effects;
            
            trackData.clips.push_back(clipData);
            totalClips++;
        }
        
        projectData.tracks.push_back(trackData);
        audioTrackCount++;
        audioClipCount += static_cast<int>(trackData.clips.size());
    }
    
    out << "[*] Processed " << audioTrackCount << " tracks with " << totalClips << " clips total" << std::endl;

    // Вычисляем общую длительность проекта
    for (const auto& track : projectData.tracks) {
        for (const auto& clip : track.clips) {
            if (clip.timelineEnd > projectData.totalLength) {
                projectData.totalLength = clip.timelineEnd;
            }
        }
    }
    
    // Экспортируем в CSV
    exportToCSV(projectData, "aaf_export.csv");
    
    // Извлечение embedded аудио через новый API
    out << "\n🎵 === EXTRACTING EMBEDDED AUDIO (NEW API) ===" << std::endl;
    auto audioStartTime = std::chrono::high_resolution_clock::now();
    
    bool extractionResult = properParser.extractAllEmbeddedAudio("extracted_media");
    
    auto audioEndTime = std::chrono::high_resolution_clock::now();
    auto audioDuration = std::chrono::duration_cast<std::chrono::milliseconds>(audioEndTime - audioStartTime);
    properParser.getLogger().logPerformanceMetric("Embedded Audio Extraction", static_cast<double>(audioDuration.count()));
    
    if (extractionResult) {
        out << "✅ Embedded audio extraction completed successfully!" << std::endl;
    } else {
        out << "⚠️ No embedded audio found or extraction failed" << std::endl;
    }
    
    // Статистика
    out << "\n📊 === AUDIO TRACKS SUMMARY ===" << std::endl;
    out << "Total audio tracks processed: " << audioTrackCount << std::endl;
    out << "Total audio clips: " << totalClips << std::endl;
    out << "Total audio fades/crossfades: " << audioFadeCount << std::endl;
    out << "Total audio effects: " << audioEffectCount << std::endl;
    
    out << "\n✅ CSV Export: aaf_export.csv" << std::endl;
    out << "Total tracks exported: " << projectData.tracks.size() << std::endl;
    int totalClipsExported = 0;
    for (const auto& track : projectData.tracks) {
        totalClipsExported += static_cast<int>(track.clips.size());
    }
    out << "Total clips exported: " << totalClipsExported << std::endl;
    out << "Project length: " << std::fixed << std::setprecision(3) << projectData.totalLength << "s" << std::endl;

    // Освобождаем ресурсы
    pHeader->Release();
    pFile->Close();
    pFile->Release();
    out.close();

    // Успешное завершение
    std::cout << "✅ AAF processed successfully!" << std::endl;
    std::cout << "📊 CSV exported: aaf_export.csv" << std::endl;
    std::cout << "📋 Report: output.txt" << std::endl;

    return 0;
}
