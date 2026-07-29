#pragma once
#include "SceneBase.h"
#include <functional>
#include <array>
#include <string>

class SceneGameOver :
    public SceneBase
{
public:
    SceneGameOver(SceneManager& sceneManager);
    ~SceneGameOver() = default;

    void Init() override;
    void End() override;
    void Update() override;
    void Draw() override;
private:
    void Retry();
    void Title();

    enum class Menu
    {
        Retry,
        Title,

        Num
    };

    struct MenuItem
    {
        std::wstring name;
        std::function<void()> action;
    };

private:
    int m_selectIndex = 0;
    std::array<MenuItem, static_cast<int>(Menu::Num)> m_menuActions;

    int m_backHandle = -1;
    int m_logoHandle = -1;

    int m_fontHandle = -1;
};

