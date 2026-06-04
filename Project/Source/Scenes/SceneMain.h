#pragma once
#include <memory>

class Player;
class Camera;
class SkyBox;

class TargetManager;

class EnemyManager;

class SceneMain
{
public:
	SceneMain();
	~SceneMain();

	void Init();
	void End();
	void Update();
	void Draw();
private:
	void DrawGrid();

private:
	int m_frameCount = 0;

	std::shared_ptr<Player> m_pPlayer = nullptr;
	std::shared_ptr<Camera> m_pCamera = nullptr;

	std::shared_ptr<EnemyManager> m_pEnemyManager = nullptr;
	std::shared_ptr<TargetManager> m_pTargetManager = nullptr;

	std::shared_ptr<SkyBox> m_pSkyBox = nullptr;
};