#include "SceneLoad.h"
#include "DxLib.h"
#include "SceneManager.h"
#include "Game.h"
#include "Singleton/FadeManager.h"

namespace
{
	constexpr int kLoadBarWidth = 800;
	constexpr int kLoadBarHeight = 30;

	constexpr int kMinLoadFrame = 30;

	constexpr int kLoadBarY = 650;

	constexpr float kBackScale = 1.3f;
	constexpr float kGearRotateSpeed = 0.05f;
	constexpr int kGearOffset = 20;
}

SceneLoad::SceneLoad(SceneManager& sceneManager) : 
	SceneBase(sceneManager)
{

}

SceneLoad::~SceneLoad()
{
	// ロードシーンが終わるとき非同期読み込みを終了する
	SetUseASyncLoadFlag(false);
}

void SceneLoad::Init()
{
	m_backHandle = LoadGraph(L"data/Graphs/StoneBrick.png");
	m_gearHandle = LoadGraph(L"data/Graphs/gear.png");
	m_firstLoadNum = GetASyncLoadNum();
	FadeManager::GetInstance().SetFadeState(false);
}

void SceneLoad::End()
{
	DeleteGraph(m_backHandle);
	DeleteGraph(m_gearHandle);
}

void SceneLoad::Update()
{
	// ロードしている数の取得
	auto loadNum = GetASyncLoadNum();
	// ロードが終わったらフレームをカウント
	if (loadNum == 0)
	{
		m_loadEndCount++;
	}
	// ロードが終わって一定時間たったらロードシーンをポップ
	if (m_loadEndCount > kMinLoadFrame && !m_isEndScene)
	{
		m_isEndScene = true;
		m_sceneManager.PopSceneWithFade();
		return;
	}

	// 歯車を回す
	m_gearAngle += kGearRotateSpeed;
}

void SceneLoad::Draw()
{
	DrawRotaGraph(Game::kScreenWidth / 2, Game::kScreenHeight / 2, kBackScale, 0.0, m_backHandle, false);

	auto loadNum = GetASyncLoadNum();
	// ロードのバーを描画
	float progress = 1.0f - (static_cast<float>(loadNum) / static_cast<float>(m_firstLoadNum));
	int x1 = Game::kScreenWidth / 2 - kLoadBarWidth / 2;
	int y1 = kLoadBarY - kLoadBarHeight / 2;
	int x2 = x1 + kLoadBarWidth * progress;
	int y2 = kLoadBarY + kLoadBarHeight / 2;
	DrawBox(x1, y1, x2, y2, 0xffff00, true);
	DrawBox(x2, y1, Game::kScreenWidth / 2 + kLoadBarWidth / 2, y2, 0x000000, true);
	// ロードのバーの枠を描画
	x2 = Game::kScreenWidth / 2 + kLoadBarWidth / 2;
	DrawBox(x1, y1, x2, y2, 0x888888, false, 3);
	// 割合を表示
	y1 -= 20;
	DrawFormatString(x1, y1, 0xffffff, L"%.0f%%", progress * 100.0f);

	// 右下に歯車を描画
	int w, h;
	GetGraphSize(m_gearHandle, &w, &h);
	DrawRotaGraph(Game::kScreenWidth - w / 2 - kGearOffset, Game::kScreenHeight - h / 2 - kGearOffset, 1.0, m_gearAngle, m_gearHandle, true);

#ifdef _DEBUG
	DrawString(0, 0, L"SceneLoad", 0xffffff);
	DrawFormatString(0, 16, 0xffffff, L"LoadNum:%d", loadNum);
#endif
}