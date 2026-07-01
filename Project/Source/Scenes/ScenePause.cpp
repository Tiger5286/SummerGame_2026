#include "ScenePause.h"
#include "DxLib.h"
#include "Game.h"
#include "Singleton/Input.h"
#include "SceneManager.h"

ScenePause::ScenePause(SceneManager& sceneManager) :
	SceneBase(sceneManager)
{

}

void ScenePause::Init()
{

}

void ScenePause::End()
{

}

void ScenePause::Update()
{
	auto& input = Input::GetInstance();
	if (input.IsTriggerd(XINPUT_BUTTON_START))
	{
		m_sceneManager.PopScene();
		return;
	}
}

void ScenePause::Draw()
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

#ifdef _DEBUG
	DrawString(0, 0, L"ScenePause", 0xffffff);
#endif
}
