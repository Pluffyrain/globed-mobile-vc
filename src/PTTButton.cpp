#include "PTTButton.hpp"

// Everything in this file only matters on a touchscreen -- on desktop
// builds it compiles to nothing, since there's a physical keybind already.
#if defined(GEODE_IS_ANDROID) || defined(GEODE_IS_IOS)

#include <Geode/utils/permission.hpp>
#include <Geode/loader/SettingV3.hpp>

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

    ccColor4F color;
    if (m_holding) {
        color = { 0.9f, 0.2f, 0.2f, 0.9f };       // red: actively talking
    } else if (Mod::get()->getSettingValue<bool>("edit-position")) {
        color = { 0.9f, 0.7f, 0.1f, 0.85f };      // amber: repositioning
    } else {
        color = { 0.2f, 0.8f, 0.3f, 0.75f };      // green: ready
    }

    m_circle->drawDot({ kRadius, kRadius }, kRadius, color);
}

bool PTTButton::ccTouchBegan(CCTouch* touch, CCEvent*) {
    auto local = this->convertTouchToNodeSpace(touch);
    float dx = local.x - kRadius;
    float dy = local.y - kRadius;
    if ((dx * dx + dy * dy) > (kRadius * kRadius)) return false;

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
    bool granted = getPermissionStatus(Permission::RecordAudio);
    if (!granted) {
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
    KeybindSettingPressedEventV3(
        "dankmeme.globed2",
        "keybind-voice-chat",
        Keybind{ KEY_None },
        down,
        false,
        0.0
    ).post();
}

#endif // GEODE_IS_ANDROID || GEODE_IS_IOS
