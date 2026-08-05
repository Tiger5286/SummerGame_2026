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

    Stage m_selectStage = Stage::First;
};

