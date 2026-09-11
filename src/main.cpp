#include <Geode/modify/PlayLayer.hpp>
#include "PTTButton.hpp"

using namespace geode::prelude;

#if defined(GEODE_IS_ANDROID) || defined(GEODE_IS_IOS)

class $modify(PlayLayer) {
    // NOTE: verify this matches PlayLayer::init's real signature for your
    // targeted GD version (check Geode's codegenned Bindings.hpp) --
    // this is the commonly-seen signature but bindings do shift.
    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects)) return false;

        auto btn = PTTButton::create();
        this->addChild(btn, 1000);

        return true;
    }
};

#endif

$on_mod(Loaded) {
    log::info("Globed Mobile Voice Chat loaded");
}
