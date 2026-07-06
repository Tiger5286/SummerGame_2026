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
    int m_loadEndCount = 0;
    int m_firstLoadNum = 0;
    bool m_isEndScene = false;
};

