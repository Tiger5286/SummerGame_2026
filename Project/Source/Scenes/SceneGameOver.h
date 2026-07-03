#pragma once
#include "SceneBase.h"
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
private:

};

