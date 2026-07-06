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

};

