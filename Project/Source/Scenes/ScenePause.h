#pragma once
#include "SceneBase.h"
class ScenePause :
    public SceneBase
{
public:
	ScenePause(SceneManager& sceneManager);
	~ScenePause() = default;

	void Init() override;
	void End() override;
	void Update() override;
	void Draw() override;

private:

};

