#pragma once
#include "SceneBase.h"
#include "Utility/MyLib.h"

class SceneClear :
    public SceneBase
{
public:
    SceneClear(SceneManager& sceneManager);
    ~SceneClear() = default;

    void Init() override;
    void End() override;
    void Update() override;
    void Draw() override;

    /// <summary>
    /// どのステージをクリアしたかを設定する(Initの前に実行する)
    /// </summary>
    /// <param name="stage">クリアしたステージ</param>
    void SetClearStage(MyLib::Stage stage) { m_clearStage = stage; }

private:
    int m_frame = 0;

    MyLib::Stage m_clearStage = MyLib::Stage::First;
    int m_backHandle = -1;
    int m_clearLogoHandle = -1;
    int m_buttonHandle = -1;
};

