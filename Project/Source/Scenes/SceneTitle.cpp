#include "SceneTitle.h"
#include "DxLib.h"
#include "Singleton/Input.h"
#include "SceneManager.h"
#include "SceneMain.h"
#include "Game.h"

SceneTitle::SceneTitle(SceneManager& sceneManager) :
	SceneBase(sceneManager)
{

}

void SceneTitle::Init()
{
	m_titleGraphHandle = LoadGraph(L"data/Graphs/title.png");
}

void SceneTitle::End()
{
	DeleteGraph(m_titleGraphHandle);
}

void SceneTitle::Update()
{
	auto& input = Input::GetInstance();
	if (input.IsTriggerd(XINPUT_BUTTON_A))
	{
		m_sceneManager.ChangeScene(std::make_shared<SceneMain>(m_sceneManager),true);
		return;
	}
}

void SceneTitle::Draw()
{
	DrawRotaGraph(Game::kScreenWidth / 2, Game::kScreenHeight / 2, 0.8, 0.0, m_titleGraphHandle, true);

	DrawString(0, 0, L"SceneTitle Aボタンでスタート", 0xffffff);
}
