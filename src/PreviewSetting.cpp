#include "PreviewSetting.hpp"

Result<std::shared_ptr<SettingV3>> PreviewSettingV3::parse(
    std::string const& key, std::string const& modID, matjson::Value const& json
) {
    auto res = std::make_shared<PreviewSettingV3>();
    auto root = checkJson(json, "PreviewSettingV3");

    res->init(key, modID, root);
    res->parseNameAndDescription(root);
    res->parseEnableIf(root);

    root.checkUnknownKeys();
    return root.ok(std::static_pointer_cast<SettingV3>(res));
}

SettingNodeV3* PreviewSettingV3::createNode(float width) {
    return PreviewSettingNodeV3::create(
        std::static_pointer_cast<PreviewSettingV3>(shared_from_this()), width
    );
}

bool PreviewSettingNodeV3::init(std::shared_ptr<PreviewSettingV3> setting, float width) {
    if (!SettingNodeV3::init(setting, width))
        return false;

    m_preview = CCDrawNode::create();
    this->getButtonMenu()->addChildAtPosition(m_preview, Anchor::Center);
    this->getButtonMenu()->setContentWidth(90);
    this->getButtonMenu()->updateLayout();

    this->redrawPreview();
    this->schedule(schedule_selector(PreviewSettingNodeV3::tickPreview), 0.1f);
    this->updateState(nullptr);

    return true;
}

void PreviewSettingNodeV3::tickPreview(float) {
    this->redrawPreview();
}

void PreviewSettingNodeV3::redrawPreview() {
    if (!m_preview) return;
    m_preview->clear();

    double opacity = Mod::get()->getSettingValue<double>("button-opacity");
    double scale = Mod::get()->getSettingValue<double>("button-scale");

    float radius = std::clamp(20.f * static_cast<float>(scale), 8.f, 38.f);

    ccColor4F fill = { 0.85f, 0.2f, 0.2f, static_cast<float>(opacity) };
    ccColor4F border = { 0.f, 0.f, 0.f, 0.55f * static_cast<float>(opacity) };

    m_preview->drawDot({ 0.f, 0.f }, radius, border);
    m_preview->drawDot({ 0.f, 0.f }, radius - 3.f, fill);
}

void PreviewSettingNodeV3::updateState(CCNode* invoker) {
    SettingNodeV3::updateState(invoker);
    this->redrawPreview();
}

PreviewSettingNodeV3* PreviewSettingNodeV3::create(std::shared_ptr<PreviewSettingV3> setting, float width) {
    auto ret = new PreviewSettingNodeV3();
    if (ret->init(setting, width)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}
