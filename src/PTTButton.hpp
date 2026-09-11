#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

// A small circular touch button, shown bottom-left by default, that:
//  - can be dragged to a new spot while "Move Button" is on in settings
//  - starts/stops Globed voice chat while held (mirrors the desktop
//    "Voice Chat Activate" keybind)
//  - requests microphone permission the first time it's pressed
//
// Only compiled on touchscreen platforms -- see the #if guard in the .cpp.
class PTTButton : public CCNode, public CCTargetedTouchDelegate {
public:
    static PTTButton* create();

protected:
    bool init() override;
    void onEnter() override;
    void onExit() override;

    bool ccTouchBegan(CCTouch* touch, CCEvent* event) override;
    void ccTouchMoved(CCTouch* touch, CCEvent* event) override;
    void ccTouchEnded(CCTouch* touch, CCEvent* event) override;
    void ccTouchCancelled(CCTouch* touch, CCEvent* event) override;

private:
    CCDrawNode* m_circle = nullptr;
    CCLabelBMFont* m_label = nullptr;
    bool m_holding = false;
    bool m_dragging = false;

    void updateVisual();
    void startTalking();
    void stopTalking();
    void triggerVoiceKey(bool down);
    void loadSavedPosition();
    void savePosition();
};
