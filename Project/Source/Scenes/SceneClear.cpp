#include "SceneClear.h"
#include "DxLib.h"
#include "Singleton/Input.h"
#include "SceneManager.h"
#include "SceneTitle.h"
#include "Game.h"
#include "Singleton/SoundManager.h"

namespace
{
	constexpr float kBackGraphScale = 1.3f;
	constexpr int kButtonGraphOffset = 20;

	constexpr float kSinScale = 0.1f;
	constexpr float kSinRate = 0.1f;

	constexpr const wchar_t* kBGMFilePath = L"data/Sounds/BGM/Clear.ogg";
}

SceneClear::SceneClear(SceneManager& sceneManager) :
	SceneBase(sceneManager)
{
}

void SceneClear::Init()
{
	m_backHandle = LoadGraph(L"data/Graphs/StoneBrick.png");
	m_clearLogoHandle = LoadGraph(L"data/Graphs/ClearLogo.png");
	m_buttonHandle = LoadGraph(L"data/Graphs/Buttons/A.png");

	auto& soundManager = SoundManager::GetInstance();
	soundManager.LoadSound(L"ClearBGM", kBGMFilePath, SoundManager::SoundType::BGM);
	soundManager.PlaySoundGame(L"ClearBGM", true, true);
}

void SceneClear::End()
{
	DeleteGraph(m_backHandle);
	DeleteGraph(m_clearLogoHandle);
	DeleteGraph(m_buttonHandle);

	auto& soundManager = SoundManager::GetInstance();
	soundManager.StopSound(L"ClearBGM", true);
	soundManager.DeleteSound(L"ClearBGM");
}

void SceneClear::Update()
{
	m_frame++;
	auto& input = Input::GetInstance();
	if (input.IsTriggerd(XINPUT_BUTTON_A))
	{
		m_sceneManager.ChangeSceneWithFade(std::make_shared<SceneTitle>(m_sceneManager), false);
		SoundManager::GetInstance().PlaySoundGame(L"Decision");
		return;
	}
}

void SceneClear::Draw()
{
	DrawRotaGraph(Game::kScreenWidth / 2, Game::kScreenHeight / 2, kBackGraphScale, 0.0, m_backHandle, false);
	DrawRotaGraph(Game::kScreenWidth / 2, Game::kScreenHeight / 2, 1.0, 0.0, m_clearLogoHandle, true);

	int w, h;
	GetGraphSize(m_buttonHandle, &w, &h);
	int x = Game::kScreenWidth - w / 2 - kButtonGraphOffset;
	int y = Game::kScreenHeight - h / 2 - kButtonGraphOffset;

	float sin = sinf(m_frame * kSinScale) * kSinRate;
	DrawRotaGraph(x, y, 1.0 + sin, 0.0, m_buttonHandle, true);

#ifdef _DEBUG
	DrawString(0, 0, L"SceneClear\n", 0xffffff);
	DrawString(0, 16, L"Aボタンでタイトルに戻る\n", 0xffffff);
#endif
}
