#pragma once
#include "SceneBase.h"
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

    

private:
    int m_frame = 0;

    int m_backHandle = -1;
    int m_clearLogoHandle = -1;
    int m_buttonHandle = -1;
};

