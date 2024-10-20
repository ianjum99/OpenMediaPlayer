#include "MediaPlayer.h"
#include <iostream>
extern "C" {
    #include <libavutil/log.h>
}

int main(int argc, char* argv[]) {
    av_log_set_level(AV_LOG_DEBUG);

    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <media file path>" << std::endl;
        return -1;
    }

    MediaPlayer player(argv[1]);

    // Call initialize without checking the return value (if it's still void)
    player.initialize();

    player.play();
    player.cleanup();

    return 0;
}
