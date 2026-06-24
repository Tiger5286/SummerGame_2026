#pragma once
#include <memory>
#include "SceneBase.h"

class Player;
class Camera;
class SkyBox;

class TargetManager;

class EnemyManager;

class SpawnerManager;

class SceneMain : public SceneBase
{
public:
	SceneMain(SceneManager& sceneManager);
	~SceneMain();

	void Init() override;
	void End() override;
	void Update() override;
	void Draw() override;
private:
	int m_frameCount = 0;

	std::shared_ptr<Player> m_pPlayer = nullptr;
	std::shared_ptr<Camera> m_pCamera = nullptr;

	std::shared_ptr<EnemyManager> m_pEnemyManager = nullptr;
	std::shared_ptr<TargetManager> m_pTargetManager = nullptr;

	std::shared_ptr<SkyBox> m_pSkyBox = nullptr;

	std::shared_ptr<SpawnerManager> m_pSpawnerManager = nullptr;
};