#include "MediaPlayer.h"
#include "GtkFileDialog.h"
#include <iostream>

int main(int argc, char* argv[]) {
    std::string media_file;

    // Create an instance of GtkFileDialog and allow the user to choose a media file
    GtkFileDialog fileDialog;
    media_file = fileDialog.openFileDialog();

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
