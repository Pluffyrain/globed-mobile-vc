#pragma once
#include <Geode/Geode.hpp>

// Hand-reconstructed declarations for a few of Globed2's internal
// (non-public) functions, recovered from the exported symbol table of
// dankmeme.globed2's own compiled binary -- the same file you sent me.
// Parameter types are extracted directly from the binary and certain;
// return types are inferred (the standard pointer-accessor pattern) and
// not confirmed against Globed's real source. No compatibility guarantee
// -- this can break if Globed's internals change, unlike their real
// public API.
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
