#include "PTTButton.hpp"

// Everything in this file only matters on a touchscreen -- on desktop
// builds it compiles to nothing, since there's a physical keybind already.
#if defined(GEODE_IS_ANDROID) || defined(GEODE_IS_IOS)

#include <Geode/utils/permission.hpp>
#include <Geode/loader/SettingV3.hpp>
#include <globed/core/net/NetworkManager.hpp>

using namespace geode::utils::permission;

static constexpr float kRadius = 40.f;

PTTButton* PTTButton::create() {
    auto ret = new PTTButton();
    if (ret->init()) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool PTTButton::init() {
    if (!CCNode::init()) return false;

    this->setContentSize({ kRadius * 2.f, kRadius * 2.f });
    this->setAnchorPoint({ 0.5f, 0.5f });

    m_circle = CCDrawNode::create();
    this->addChild(m_circle);

    m_label = CCLabelBMFont::create("VC", "bigFont.fnt");
    m_label->setScale(0.4f);
    m_label->setPosition({ kRadius, kRadius });
    this->addChild(m_label);

    this->loadSavedPosition();
    this->updateVisual();

    // Periodically refresh the idle color in case connection state changes
    // while the button isn't being touched.
    this->schedule(schedule_selector(PTTButton::checkConnection), 1.0f);

    return true;
}

void PTTButton::onEnter() {
    CCNode::onEnter();
    CCTouchDispatcher::get()->addTargetedDelegate(this, -900, true);
}

void PTTButton::onExit() {
    CCTouchDispatcher::get()->removeDelegate(this);
    CCNode::onExit();
}

void PTTButton::loadSavedPosition() {
    // Defaults land it a comfortable thumb's reach from the bottom-left
    // corner. Mod::get()->getSavedValue is Geode's free-form persistent
    // storage, separate from the declared mod.json settings.
    float x = Mod::get()->getSavedValue<float>("ptt-pos-x", 70.f);
    float y = Mod::get()->getSavedValue<float>("ptt-pos-y", 90.f);
    this->setPosition({ x, y });
}

void PTTButton::savePosition() {
    auto pos = this->getPosition();
    Mod::get()->setSavedValue("ptt-pos-x", pos.x);
    Mod::get()->setSavedValue("ptt-pos-y", pos.y);
}

void PTTButton::updateVisual() {
    m_circle->clear();

    bool connected = false;
    if (auto nm = globed::NetworkManager::get()) {
        connected = nm->isConnected();
    }

    ccColor4F color;
    if (m_holding) {
        color = { 0.9f, 0.2f, 0.2f, 0.9f };       // red: actively talking
    } else if (Mod::get()->getSettingValue<bool>("edit-position")) {
        color = { 0.9f, 0.7f, 0.1f, 0.85f };      // amber: repositioning
    } else if (connected) {
        color = { 0.2f, 0.8f, 0.3f, 0.75f };      // green: ready
    } else {
        color = { 0.5f, 0.5f, 0.5f, 0.6f };       // grey: not in a session
    }

    m_circle->drawDot({ kRadius, kRadius }, kRadius, color);
}

void PTTButton::checkConnection(float) {
    if (!m_holding) this->updateVisual();
}

bool PTTButton::ccTouchBegan(CCTouch* touch, CCEvent*) {
    auto local = this->convertTouchToNodeSpace(touch);
    float dx = local.x - kRadius;
    float dy = local.y - kRadius;
    if ((dx * dx + dy * dy) > (kRadius * kRadius)) return false; // outside the circle

    if (Mod::get()->getSettingValue<bool>("edit-position")) {
        m_dragging = true;
    } else {
        m_holding = true;
        this->startTalking();
    }
    this->updateVisual();
    return true;
}

void PTTButton::ccTouchMoved(CCTouch* touch, CCEvent*) {
    if (!m_dragging || !this->getParent()) return;
    this->setPosition(this->getParent()->convertTouchToNodeSpace(touch));
}

void PTTButton::ccTouchEnded(CCTouch*, CCEvent*) {
    if (m_dragging) {
        m_dragging = false;
        this->savePosition();
    } else if (m_holding) {
        m_holding = false;
        this->stopTalking();
    }
    this->updateVisual();
}

void PTTButton::ccTouchCancelled(CCTouch* touch, CCEvent* event) {
    this->ccTouchEnded(touch, event);
}

void PTTButton::startTalking() {
    // NOTE: verify `Permission::RecordAudio` is the real enum member name
    // in the <Geode/utils/permission.hpp> shipped with your installed SDK
    // -- your editor's autocomplete will tell you instantly. Only
    // `Permission::ReadAllFiles` is confirmed in the public docs I could
    // check; microphone may be named differently (or not exposed yet, in
    // which case this call won't compile and you'd need to request it be
    // added upstream, or fall back to raw JNI / AVAudioSession).
    auto status = getPermissionStatus(Permission::RecordAudio);
    if (status != PermissionStatus::Granted) {
        requestPermission(Permission::RecordAudio, [this](bool granted) {
            if (granted) {
                this->triggerVoiceKey(true);
            } else {
                Notification::create("Microphone permission denied", NotificationIcon::Error)->show();
            }
        });
        return;
    }
    this->triggerVoiceKey(true);
}

void PTTButton::stopTalking() {
    this->triggerVoiceKey(false);
}

void PTTButton::triggerVoiceKey(bool down) {
    // This re-emits the same global event Geode fires when a physical key
    // bound to a keybind SETTING is pressed or released -- targeting
    // Globed's own "keybind-voice-chat" setting, the same one "V" is
    // bound to on desktop. That's the intended public integration point
    // (see listenForKeybindSettingPresses / KeybindSettingPressedEventV3
    // in <Geode/loader/SettingV3.hpp>).
    //
    // IMPORTANT: double-check this constructor against the actual header
    // in your SDK before trusting it -- I pieced this signature together
    // from Geode's documentation site, not a live header, and it can
    // shift between Geode versions. Also remember: as of writing, Globed
    // only actually transmits captured audio on Windows, so even a
    // perfectly-fired event may currently be a no-op on Android/iOS until
    // GlobedGD ships mobile mic capture. shut the fuck up claude haha i love vibecoding im such a goy
    KeybindSettingPressedEventV3(
        "dankmeme.globed2",
        "keybind-voice-chat",
        Keybind::create(KEY_None),
        down,
        false,
        0.0
    ).post();
}

#endif // GEODE_IS_ANDROID || GEODE_IS_IOS
