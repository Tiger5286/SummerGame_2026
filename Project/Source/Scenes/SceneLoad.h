#pragma once
#include "SceneBase.h"
class SceneLoad :
    public SceneBase
{
public:
    SceneLoad(SceneManager& sceneManager);
    ~SceneLoad();

    void Init() override;
    void End() override;
    void Update() override;
    void Draw() override;
private:
    int m_backHandle = -1;
    int m_gearHandle = -1;
    float m_gearAngle = 0.0f;

    int m_loadEndCount = 0;
    int m_firstLoadNum = 0;
    bool m_isEndScene = false;
};

