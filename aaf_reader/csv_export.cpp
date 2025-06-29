#include "csv_export.h"
#include <fstream>
#include <iomanip>
#include <unordered_map>
#include <algorithm>
#include <cstdio>

// Helper function to generate proper file name with counter (same logic as extractor)
std::string generateProperFileNameForCSV(const std::string& originalFileName, 
                                         std::unordered_map<std::string, int>& fileCounters) {
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
    
    // If baseName is empty or invalid, use fallback
    if (baseName.empty()) {
        baseName = "Audio_Unknown";
    }
    
    // Default extension for AAF embedded audio
    std::string extension = ".aif";
    
    // Check if we've already used a file with this basename
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
    
    return result;
}

void exportToCSV(const ProjectData& projectData, const std::string& filename) {
    std::ofstream csv(filename);
    
    // Счетчик файлов для генерации правильных имен
    std::unordered_map<std::string, int> fileCounters;
    
    // Заголовок CSV
    csv << "TrackIndex,TrackName,TrackType,TrackVolume,TrackPan,TrackMute,TrackSolo,"
        << "ClipFileName,ClipMobID,ClipTimelineStart,ClipTimelineEnd,ClipSourceStart,"
        << "ClipLength,ClipGain,ClipVolume,ClipPan,ClipEffects,"
        << "FadeInLength,FadeOutLength,CrossfadeLength,FadeInType,FadeOutType" << std::endl;
    
    // Экспортируем данные треков и клипов
    for (const auto& track : projectData.tracks) {
        if (track.clips.empty()) {
            // Пустой трек - записываем только информацию о треке
            csv << track.trackIndex << ","
                << "\"" << track.trackName << "\","
                << "\"" << track.trackType << "\","
                << std::fixed << std::setprecision(3) << track.volume << ","
                << std::fixed << std::setprecision(3) << track.pan << ","
                << (track.mute ? "true" : "false") << ","
                << (track.solo ? "true" : "false") << ","
                << ",,,,,,,,,,,,,,," << std::endl; // Пустые поля для клипа
        } else {
            // Трек с клипами
            for (const auto& clip : track.clips) {
                // Генерируем правильное имя файла с суффиксом
                std::string actualFileName = generateProperFileNameForCSV(clip.fileName, fileCounters);
                
                csv << track.trackIndex << ","
                    << "\"" << track.trackName << "\","
                    << "\"" << track.trackType << "\","
                    << std::fixed << std::setprecision(3) << track.volume << ","
                    << std::fixed << std::setprecision(3) << track.pan << ","
                    << (track.mute ? "true" : "false") << ","
                    << (track.solo ? "true" : "false") << ","
                    << "\"" << actualFileName << "\","  // Используем сгенерированное имя
                    << "\"" << clip.mobID << "\","
                    << std::fixed << std::setprecision(3) << clip.timelineStart << ","
                    << std::fixed << std::setprecision(3) << clip.timelineEnd << ","
                    << std::fixed << std::setprecision(3) << clip.sourceStart << ","
                    << std::fixed << std::setprecision(3) << clip.length << ","
                    << std::fixed << std::setprecision(3) << clip.gain << ","
                    << std::fixed << std::setprecision(3) << clip.volume << ","
                    << std::fixed << std::setprecision(3) << clip.pan << ",";
                
                // Эффекты (объединяем в одну строку)
                csv << "\"";
                for (size_t i = 0; i < clip.effects.size(); ++i) {
                    if (i > 0) csv << "; ";
                    csv << clip.effects[i];
                }
                csv << "\"," 
                    << std::fixed << std::setprecision(3) << clip.fadeInLength << ","
                    << std::fixed << std::setprecision(3) << clip.fadeOutLength << ","
                    << std::fixed << std::setprecision(3) << clip.crossfadeLength << ","
                    << "\"" << clip.fadeInType << "\","
                    << "\"" << clip.fadeOutType << "\""
                    << std::endl;
            }
        }
    }
    
    csv.close();
}
