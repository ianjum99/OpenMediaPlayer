#include <nfd.h>
#include "MediaPlayer.h"
#include <iostream>

std::string openFileDialog() {
    nfdchar_t *outPath = nullptr;
    nfdresult_t result = NFD_OpenDialog(nullptr, nullptr, &outPath);
    
    if (result == NFD_OKAY) {
        std::string file_path(outPath);
        free(outPath);
        return file_path;
    } else if (result == NFD_CANCEL) {
        std::cerr << "User pressed cancel." << std::endl;
    } else {
        std::cerr << "Error: " << NFD_GetError() << std::endl;
    }

    return "";
}

int main(int argc, char* argv[]) {
    std::string media_file;

    // Allow user to choose a media file using a file dialog
    media_file = openFileDialog();

    // Check if the user selected a valid file
    if (media_file.empty()) {
        std::cerr << "No file selected." << std::endl;
        return -1;
    }

    MediaPlayer player(media_file);
    player.initialize();
    player.play();
    player.cleanup();

    return 0;
}
