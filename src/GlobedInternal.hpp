#pragma once
#include <Geode/Geode.hpp>

// how bleood reacts to death sentence
namespace globed {
    class GlobedGJBGL {
    public:
        static GlobedGJBGL* get(GJBaseGameLayer* layer);
        static GlobedGJBGL* getActive(GJBaseGameLayer* layer);
        void pauseVoiceRecording();
        void resumeVoiceRecording();
        bool isUnableToSpeak();
    };
}
