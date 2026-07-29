#pragma once
#include "SceneBase.h"
#include "System/Menu.h"
#include <functional>
#include <string>
#include <array>

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
	void Resume();
	void Option();
	void BackToTitle();

private:
	int m_titleFontHandle = -1;
	int m_titleStringWidth = -1;
	Menu m_menu;
};

