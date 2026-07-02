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

	enum class PauseMenu
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
	int m_selectIndex = 0;	// 選択中のメニューのインデックス
	// メニューのアクション(関数)を格納する配列
	std::array<MenuItem, static_cast<int>(PauseMenu::Num)> m_menuActions;

};

