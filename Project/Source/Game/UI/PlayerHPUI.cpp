#include "PlayerHPUI.h"
#include "../Character/Player/Player.h"
#include "Singleton/CameraSetter.h"
#include <string>
#include "Game.h"

namespace
{
	constexpr float kGraphScale = 0.5f;
	constexpr int kOffsetX = 30;
	constexpr int kOffsetY = 50;

	constexpr int kHPGraphMinX = 21;
	constexpr int kHPGraphMaxX = 780;
	constexpr int kHPGraphMinY = 28;

	constexpr float kWarningHPRate = 0.4f;
	constexpr float kDangerHPRate = 0.2f;

	enum class Graph
	{
		Frame,
		Green,
		Yellow,
		Red,
		TextBack,

		Num
	};

	constexpr const wchar_t* kFileNames[] = {
		L"data/Graphs/Game/HPBar_Frame.png",
		L"data/Graphs/Game/HPBar_Green.png",
		L"data/Graphs/Game/HPBar_Yellow.png",
		L"data/Graphs/Game/HPBar_Red.png",
		L"data/Graphs/Game/hpBack.png"
	};

	// 画像の種類とファイルパスの数が一致していないとエラー
	static_assert(static_cast<int>(Graph::Num) == sizeof(kFileNames) / sizeof(wchar_t*));

	constexpr int kFontSize = 20;
	constexpr int kTextOffsetY = -5;
}

PlayerHPUI::~PlayerHPUI()
{
	for (auto& handle : m_handles)
	{
		DeleteGraph(handle);
	}
	DeleteFontToHandle(m_fontHandle);
}

void PlayerHPUI::Init()
{
	for (int i = 0; i < static_cast<int>(Graph::Num); i++)
	{
		m_handles.push_back(LoadGraph(kFileNames[i]));
		assert(m_handles.back() != -1);
	}
	m_fontHandle = CreateFontToHandle(Game::kMainFontName, kFontSize, -1, DX_FONTTYPE_ANTIALIASING);
	assert(m_fontHandle != -1);
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
	x = kOffsetX + w / 2 * kGraphScale;
	y = kOffsetY + h / 2 * kGraphScale;
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

	// HPテキストを描画
	x = kOffsetX + kHPGraphMinX * kGraphScale;
	y = kOffsetY - kFontSize + kHPGraphMinY * kGraphScale + kTextOffsetY;

	DrawGraph(x - 17, y-2, m_handles[static_cast<int>(Graph::TextBack)], true);
	DrawFormatStringToHandle(x, y, 0xffffff, m_fontHandle, L"HP : %4d / %4d", player->GetHP(), player->kMaxHp);
}