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
    
    // Добавляем метаинформацию в начало файла в виде комментариев
    csv << "# AAF Project Export" << std::endl;
    csv << "# Project Name: " << projectData.projectName << std::endl;
    csv << "# Session Start Timecode (frames): " << std::fixed << std::setprecision(0) << projectData.sessionStartTimecode << std::endl;
    csv << "# Total Project Length: " << std::fixed << std::setprecision(3) << projectData.totalLength << " seconds" << std::endl;
    csv << "# Sample Rate: " << projectData.sampleRate << " Hz" << std::endl;
    csv << "# Channel Count: " << projectData.channelCount << std::endl;
    csv << "#" << std::endl;
    
    // Заголовок CSV - добавляем поля для fade-событий
    csv << "TrackIndex,TrackName,TrackType,TrackVolume,TrackPan,TrackMute,TrackSolo,"
        << "ClipFileName,ClipMobID,ClipTimelineStart,ClipTimelineEnd,ClipSourceStart,"
        << "ClipLength,ClipGain,ClipVolume,ClipPan,ClipEffects,"
        << "EventType,EventStart,EventLength,EventShape" << std::endl;
    
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
                << ",,,,,,,,,,,,," << std::endl; // Пустые поля для клипа и событий
        } else {
            // Трек с клипами
            for (const auto& clip : track.clips) {
                // Генерируем правильное имя файла с суффиксом
                std::string actualFileName = generateProperFileNameForCSV(clip.fileName, fileCounters);
                
                // Базовая строка с информацией о клипе
                std::string baseClipData = 
                    std::to_string(track.trackIndex) + ","
                    + "\"" + track.trackName + "\","
                    + "\"" + track.trackType + "\","
                    + std::to_string(track.volume) + ","
                    + std::to_string(track.pan) + ","
                    + (track.mute ? "true" : "false") + ","
                    + (track.solo ? "true" : "false") + ","
                    + "\"" + actualFileName + "\","
                    + "\"" + clip.mobID + "\","
                    + std::to_string(clip.timelineStart) + ","
                    + std::to_string(clip.timelineEnd) + ","
                    + std::to_string(clip.sourceStart) + ","
                    + std::to_string(clip.length) + ","
                    + std::to_string(clip.gain) + ","
                    + std::to_string(clip.volume) + ","
                    + std::to_string(clip.pan) + ","
                    + "\""; // Начало поля effects
                
                // Эффекты (объединяем в одну строку)
                std::string effectsString;
                for (size_t i = 0; i < clip.effects.size(); ++i) {
                    if (i > 0) effectsString += "; ";
                    effectsString += clip.effects[i];
                }
                
                bool hasAnyFade = (clip.fadeInLength > 0.0 || clip.fadeOutLength > 0.0);
                
                // Основная запись клипа (без fade-событий)
                csv << std::fixed << std::setprecision(3) << baseClipData << effectsString << "\",";
                if (!hasAnyFade) {
                    csv << "CLIP,,," << std::endl;
                } else {
                    csv << "CLIP,,," << std::endl;
                }
                
                // Fade-in событие (если есть)
                if (clip.fadeInLength > 0.0) {
                    csv << std::fixed << std::setprecision(3) << baseClipData << effectsString << "\",";
                    csv << "FADE_IN," << clip.timelineStart << "," << clip.fadeInLength << ","
                        << "\"" << clip.fadeInType << "\"" << std::endl;
                }
                
                // Fade-out событие (если есть)
                if (clip.fadeOutLength > 0.0) {
                    double fadeOutStart = clip.timelineEnd - clip.fadeOutLength;
                    csv << std::fixed << std::setprecision(3) << baseClipData << effectsString << "\",";
                    csv << "FADE_OUT," << fadeOutStart << "," << clip.fadeOutLength << ","
                        << "\"" << clip.fadeOutType << "\"" << std::endl;
                }
                
                // Crossfade событие (если есть)
                if (clip.crossfadeLength > 0.0) {
                    csv << std::fixed << std::setprecision(3) << baseClipData << effectsString << "\",";
                    csv << "CROSSFADE," << clip.timelineStart << "," << clip.crossfadeLength << ","
                        << "\"Linear\"" << std::endl;
                }
            }
        }
    }
    
    csv.close();
}
