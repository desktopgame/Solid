#pragma once
#include <library.hpp>

namespace App::Scenes::Title {
class TitleScene : public IScene {
public:
    explicit TitleScene();
    virtual ~TitleScene();

    void onEnter() override;
    void onExit() override;

    void onUpdate() override;
    void onGui() override;
    void onDraw3D() override;
    void onDraw2D() override;

    bool tryTransition(std::string& outNextScene) override;

private:
    void onClickStartButton();
    void onClickSettingButton();

    std::shared_ptr<Renderer> m_renderer;
    std::shared_ptr<FontMap> m_fontMap;
    std::shared_ptr<RootPane> m_baseUI;
    std::string m_nextScene;
};
}