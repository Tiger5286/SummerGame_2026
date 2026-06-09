#pragma once
#include "SceneBase.h"
class SceneTitle :
    public SceneBase
{
public:
    SceneTitle(SceneManager& sceneManager);
    ~SceneTitle() = default;

    void Init() override;
    void End() override;
    void Update() override;
    void Draw() override;
private:

};

