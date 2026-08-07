#pragma once
#include "SceneBase.h"
class SceneStageSelect :
    public SceneBase
{
public:
    enum class Stage
    {
        First,
        Second,

        Num
    };

public:
    SceneStageSelect(SceneManager& sceneManager);
    virtual ~SceneStageSelect();

    void Init() override;
    void End() override;
    void Update() override;
    void Draw() override;

private:
    int m_frame = 0;
    Stage m_selectStage = Stage::First;
    int m_fontHandle = -1;
    int m_buttonHandle = -1;
    int m_arrowHandle = -1;
    int m_backHandles[static_cast<int>(Stage::Num)] = { -1,-1 };
	float m_backAlpha[static_cast<int>(Stage::Num)] = {};
};

