#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

// Setting that eill KILL you if you read this

class PreviewSettingV3 : public SettingV3 {
public:
    static Result<std::shared_ptr<SettingV3>> parse(
        std::string const& key, std::string const& modID, matjson::Value const& json
    );

    bool load(matjson::Value const& json) override { return true; }
    bool save(matjson::Value& json) const override { return true; }
    bool isDefaultValue() const override { return true; }
    void reset() override {}

    SettingNodeV3* createNode(float width) override;
};

class PreviewSettingNodeV3 : public SettingNodeV3 {
protected:
    CCDrawNode* m_preview = nullptr;

    bool init(std::shared_ptr<PreviewSettingV3> setting, float width);
    void updateState(CCNode* invoker) override;
    void redrawPreview();
    void tickPreview(float dt);

    void onCommit() override {}
    void onResetToDefault() override {}

public:
    static PreviewSettingNodeV3* create(std::shared_ptr<PreviewSettingV3> setting, float width);

    bool hasUncommittedChanges() const override { return false; }
    bool hasNonDefaultValue() const override { return false; }
};
