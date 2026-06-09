#pragma once
#include <list>
#include <memory>

class SceneBase;

class SceneManager
{
public:
	SceneManager() = default;
	~SceneManager() = default;

	void ChangeScene(std::shared_ptr<SceneBase> pNewScene);
	void PushScene(std::shared_ptr<SceneBase> pNewScene);
	void PopScene();
	void ResetScene(std::shared_ptr<SceneBase> pNewScene);

	void Update();
	void Draw();
	void End();

private:
	std::list<std::shared_ptr<SceneBase>> m_pScenes;
};

