#include "SceneLoad.h"
#include "DxLib.h"
#include "SceneManager.h"
#include "Game.h"

namespace
{
	constexpr int kLoadBarWidth = 800;
	constexpr int kLoadBarHeight = 100;

	constexpr int kMinLoadFrame = 60;
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
	m_firstLoadNum = GetASyncLoadNum();
}

void SceneLoad::End()
{

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
	if (m_loadEndCount > kMinLoadFrame)
	{
		m_sceneManager.PopScene();
		return;
	}
}

void SceneLoad::Draw()
{
	// 背景を真っ黒で描画
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);

	auto loadNum = GetASyncLoadNum();
	// ロードのバーを描画
	float progress = 1.0f - (static_cast<float>(loadNum) / static_cast<float>(m_firstLoadNum));
	int x1 = Game::kScreenWidth / 2 - kLoadBarWidth / 2;
	int y1 = Game::kScreenHeight / 2 - kLoadBarHeight / 2;
	int x2 = x1 + kLoadBarWidth * progress;
	int y2 = Game::kScreenHeight / 2 + kLoadBarHeight / 2;
	DrawBox(x1, y1, x2, y2, 0xffff00, true);
	// ロードのバーの枠を描画
	x2 = Game::kScreenWidth / 2 + kLoadBarWidth / 2;
	DrawBox(x1, y1, x2, y2, 0xffffff, false, 5);
	// 割合を表示
	y1 -= 20;
	DrawFormatString(x1, y1, 0xffffff, L"%.0f%%", progress * 100.0f);

#ifdef _DEBUG
	DrawString(0, 0, L"SceneLoad", 0xffffff);
	DrawFormatString(0, 16, 0xffffff, L"LoadNum:%d", loadNum);
#endif
}
