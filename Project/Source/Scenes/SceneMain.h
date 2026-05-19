#pragma once
#include <memory>

#include "../System/Input.h"

class Player;
class Camera;
class SkyBox;

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

	int m_mapModelHandle = -1;
	int m_playerModelHandle = -1;

	Input m_input;

	std::shared_ptr<Player> m_pPlayer = nullptr;
	std::shared_ptr<Camera> m_pCamera = nullptr;

	std::shared_ptr<SkyBox> m_pSkyBox = nullptr;
};