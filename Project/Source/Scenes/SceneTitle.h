#pragma once
#include "SceneBase.h"
#include <string>
#include <functional>
#include <array>

class SceneTitle :
    public SceneBase
{
public:
    SceneTitle(SceneManager& sceneManager);
    ~SceneTitle() = default;

    void Init() override;
    void End() override;
    void Update() override;
    void Draw() override;

private:
    void Start();
    void Option();
    void Exit();

    enum class Menu
    {
        Start,
        Option,
        Exit,

        Num
    };

    struct MenuItem
    {
        std::wstring name;
        std::function<void()> action;
    };

private:
    int m_titleHandle = -1;
    int m_titleBackHandle = -1;
    int m_effHandle = -1;

    int m_selectIndex = 0;
    std::array<MenuItem, static_cast<int>(Menu::Num)> m_menuActions;
};

