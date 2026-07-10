#include "PlayerHPUI.h"
#include "../Character/Player/Player.h"
#include "Singleton/CameraSetter.h"
#include <string>
#include "Game.h"

namespace
{
	constexpr float kGraphScale = 0.5f;
	constexpr int kOffset = 30;

	constexpr int kHPGraphMinX = 21;
	constexpr int kHPGraphMaxX = 780;

	constexpr float kWarningHPRate = 0.4f;
	constexpr float kDangerHPRate = 0.2f;

	enum class Graph
	{
		Frame,
		Green,
		Yellow,
		Red,

		Num
	};

	constexpr const wchar_t* kFileNames[] = {
		L"data/Graphs/Game/HPBar_Frame.png",
		L"data/Graphs/Game/HPBar_Green.png",
		L"data/Graphs/Game/HPBar_Yellow.png",
		L"data/Graphs/Game/HPBar_Red.png",
	};

	// 画像の種類とファイルパスの数が一致していないとエラー
	static_assert(static_cast<int>(Graph::Num) == sizeof(kFileNames) / sizeof(wchar_t*));
}

PlayerHPUI::~PlayerHPUI()
{
	for (auto& handle : m_handles)
	{
		DeleteGraph(handle);
	}
}

void PlayerHPUI::Init()
{
	for (int i = 0; i < static_cast<int>(Graph::Num); i++)
	{
		m_handles.push_back(LoadGraph(kFileNames[i]));
		assert(m_handles.back() != -1);
	}
}

void PlayerHPUI::Update()
{

}

void PlayerHPUI::Draw()
{
	// 画像サイズを取得
	int w, h;
	GetGraphSize(m_handles[static_cast<int>(Graph::Frame)], &w, &h);
	// 枠を描画
	int x, y;
	x = kOffset + w / 2 * kGraphScale;
	y = kOffset + h / 2 * kGraphScale;
	DrawRotaGraph(x, y, kGraphScale, 0.0, m_handles[static_cast<int>(Graph::Frame)], true);
	// 本体を描画
	// HPバーの中身の長さを計算
	int hpLen = w - ((w - kHPGraphMaxX) + kHPGraphMinX);
	// プレイヤーのHPの割合を計算
	auto player = m_pPlayer.lock();
	float rate = static_cast<float>(player->GetHP()) / static_cast<float>(player->kMaxHp);
	// 現在のHPバーの長さを計算
	int width = rate * hpLen;
	// HPの割合によって色を変える
	int handle = m_handles[static_cast<int>(Graph::Green)];
	if (rate < kDangerHPRate)
	{
		handle = m_handles[static_cast<int>(Graph::Red)];
	}
	else if (rate < kWarningHPRate)
	{
		handle = m_handles[static_cast<int>(Graph::Yellow)];
	}
	// HPバーを描画
	DrawRectRotaGraph(x - (hpLen - width) * kGraphScale * 0.5f, y, kHPGraphMinX, 0, width, h, kGraphScale, 0.0, handle, true);
}