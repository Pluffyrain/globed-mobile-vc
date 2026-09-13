#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

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
    bool m_toggleActive = false;

    bool isToggleMode();
    bool isSpeaking();
    void updateVisual();
    void refreshVisibility(float dt);
    void startTalking();
    void stopTalking();
    void triggerVoiceKey(bool down);
    void loadSavedPosition();
    void savePosition();
};
