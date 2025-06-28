#include "media_utils.h"
#include <filesystem>
#include <iostream>

namespace MediaUtils {

void createExtractedMediaFolder() {
    try {
        if (!std::filesystem::exists("extracted_media")) {
            std::filesystem::create_directory("extracted_media");
            std::cout << "Created directory: extracted_media" << std::endl;
        }
        if (!std::filesystem::exists("audio_files")) {
            std::filesystem::create_directory("audio_files");
            std::cout << "Created directory: audio_files" << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error creating media folders: " << e.what() << std::endl;
    }
}

}
