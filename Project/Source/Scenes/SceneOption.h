#pragma once
#include "SceneBase.h"
#include <functional>
#include <string>
#include <array>

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

    enum class OptionMenu
    {
        SeVolume,
        BgmVolume,
        WindowMode,

        Num
    };

    struct MenuItem
    {
        std::wstring name;
        std::function<void()> action;
    };

private:
    int m_selectIndex = 0;	// 選択中のメニューのインデックス
    // メニューのアクション(関数)を格納する配列
    std::array<MenuItem, static_cast<int>(OptionMenu::Num)> m_menuActions;

    int m_volume[2];
    bool m_windowMode = true;   // true:ウィンドウ / false:フルスクリーン
};

