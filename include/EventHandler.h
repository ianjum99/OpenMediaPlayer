#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

class MediaPlayer;

class EventHandler {
public:
    EventHandler();
    ~EventHandler();

    bool handleEvents(MediaPlayer& mediaPlayer);
    void togglePause();
    void handleSeek();

private:
    bool is_paused;
};

#endif // EVENT_HANDLER_H
