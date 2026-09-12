#include <Geode/modify/PlayLayer.hpp>
#include "PTTButton.hpp"
#include "PreviewSetting.hpp"

using namespace geode::prelude;

#if defined(GEODE_IS_ANDROID) || defined(GEODE_IS_IOS)

class $modify(PlayLayer) {
    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects)) return false;

        auto btn = PTTButton::create();
        this->addChild(btn, 1000);

        return true;
    }
};

#endif

$on_mod(Loaded) {
    (void)Mod::get()->registerCustomSettingType("ptt-preview", &PreviewSettingV3::parse);
    log::info("Globed Mobile Voice Chat loaded");
}
