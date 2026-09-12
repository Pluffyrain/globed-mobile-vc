#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

// A small circlebutton. Only appears whhen
//  - the enable Button setting is on
//  - youre connected to globed level dude typing in github is hard ad shit
//.
// NOTE: talking only actually transmits audio on a patched Globed build
// with mobile voice sending enabled -- see this mod's description.
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
    void refreshVisibility(float dt);
    void startTalking();
    void stopTalking();
    void triggerVoiceKey(bool down);
    void loadSavedPosition();
    void savePosition();
};
