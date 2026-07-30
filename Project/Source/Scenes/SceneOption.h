#pragma once
#include "SceneBase.h"
#include <functional>
#include <string>
#include <array>
#include "System/Menu.h"

class SceneOption :
    public SceneBase
{
public:
    SceneOption(SceneManager& sceneManager);
    ~SceneOption() = default;

    void Init() override;
    void End() override;
    void Update() override;
    void Draw() override;

private:
    void SeVolume();
    void BgmVolume();
    void WindowMode();

private:
    Menu m_menu;

    float m_volume[2];
    bool m_isFullScreen = true;
};

