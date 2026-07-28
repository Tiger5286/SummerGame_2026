#pragma once
#include "SceneBase.h"
#include <functional>
#include <array>

class SceneDebug :
    public SceneBase
{
public:
    SceneDebug(SceneManager& sceneManager);
    virtual ~SceneDebug();

    void Init() override;
    void End() override;
    void Update() override;
    void Draw() override;

    enum class Scene
    {
        Title,
        StageSelect,
        Main,
        Pause,
        Option,
        Clear,
        GameOver,

        Num
    };

private:
    std::array<std::function<void()>, static_cast<int>(Scene::Num)> m_sceneChangeFuncs;
    Scene m_selectScene = Scene::Title;
};

