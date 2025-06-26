#include "csv_export.h"
#include <fstream>
#include <iomanip>

void exportToCSV(const ProjectData& projectData, const std::string& filename) {
    std::ofstream csv(filename);
    
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
                csv << track.trackIndex << ","
                    << "\"" << track.trackName << "\","
                    << "\"" << track.trackType << "\","
                    << std::fixed << std::setprecision(3) << track.volume << ","
                    << std::fixed << std::setprecision(3) << track.pan << ","
                    << (track.mute ? "true" : "false") << ","
                    << (track.solo ? "true" : "false") << ","
                    << "\"" << clip.fileName << "\","
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
