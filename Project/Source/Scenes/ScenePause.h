#pragma once
#include "SceneBase.h"
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

	enum class Menu
	{
		Resume,
		Option,
		BackToTitle,

		Num
	};

	struct MenuItem
	{
		std::wstring name;
		std::function<void()> action;
	};

private:
	int m_menuUIHandle = -1;
	int m_selectMenuUIHandle = -1;

	int m_titleFontHandle = -1;
	int m_titleStringWidth = -1;

	int m_menuFontHandle = -1;

	int m_selectIndex = 0;	// 選択中のメニューのインデックス
	// メニューのアクション(関数)を格納する配列
	std::array<MenuItem, static_cast<int>(Menu::Num)> m_menuActions;
	float m_menuScales[static_cast<int>(Menu::Num)] = {};
	float m_menuColorRate[static_cast<int>(Menu::Num)] = {};

};

