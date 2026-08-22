#pragma once
#include "SceneBase.h"
#include "Utility/MyLib.h"

class SceneStageSelect :
    public SceneBase
{
public:
    SceneStageSelect(SceneManager& sceneManager);
    virtual ~SceneStageSelect();

    void Init() override;
    void End() override;
    void Update() override;
    void Draw() override;

private:
    int m_frame = 0;
    MyLib::Stage m_selectStage = MyLib::Stage::First;
    int m_fontHandle = -1;
    int m_buttonHandle = -1;
    int m_arrowHandle = -1;
    int m_backHandles[static_cast<int>(MyLib::Stage::Num)] = { -1,-1 };
	float m_backAlpha[static_cast<int>(MyLib::Stage::Num)] = {};
};

