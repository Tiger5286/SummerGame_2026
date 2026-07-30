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
    int m_fontHandle = -1;
    Menu m_menu;

    enum class Volumes
    {
        SE,
        BGM,

        Num
    };
    float m_volume[static_cast<int>(Volumes::Num)];

    bool m_isFullScreen = true;
};

