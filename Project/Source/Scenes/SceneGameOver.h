#pragma once
#include "SceneBase.h"
#include <functional>
#include <array>
#include <string>
#include "System/Menu.h"
#include "SceneMain.h"

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

    void SetData(SceneMain::UniqueDatas uniqueDatas) { m_uniqueDatas = uniqueDatas; m_isSetData = true; }

private:
    void Retry();
    void Title();
private:
    int m_backHandle = -1;
    int m_logoHandle = -1;
    Menu m_menu;
	SceneMain::UniqueDatas m_uniqueDatas;
    bool m_isSetData = false;
};

