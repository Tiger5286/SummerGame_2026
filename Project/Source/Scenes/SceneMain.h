#pragma once
#include <memory>

class Player;

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

	int m_playerModelHandle = -1;

	std::shared_ptr<Player> m_pPlayer = nullptr;
};