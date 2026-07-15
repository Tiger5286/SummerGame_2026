#include "PlayerSpecialUI.h"
#include "../Character/Player/Player.h"
#include "Singleton/CameraSetter.h"

namespace
{
	constexpr float kGraphScale = 0.5f;

	constexpr int kGraphMinX = 60;
	constexpr int kGraphMaxX = 739;

	constexpr int kOffsetX = 30;
	constexpr int kOffsetY = 80;

	constexpr int kBrinkFrame = 20;

	enum class Graph
	{
		Frame,
		Main,
		LB,
		LBOutline,

		Num
	};

	constexpr const wchar_t* kFileNames[] = {
		L"data/Graphs/Game/SpecialGauge_Frame.png",
		L"data/Graphs/Game/SpecialGauge.png",
		L"data/Graphs/Buttons/LB.png",
		L"data/Graphs/Buttons/LB_outline.png"
	};

	static_assert(static_cast<int>(Graph::Num) == sizeof(kFileNames) / sizeof(wchar_t*));
}

PlayerSpecialUI::PlayerSpecialUI()
{
}

PlayerSpecialUI::~PlayerSpecialUI()
{
	for (auto handle : m_handles)
	{
		DeleteGraph(handle);
	}
}

void PlayerSpecialUI::Init()
{
	m_handles.reserve(static_cast<int>(Graph::Num));
	for (int i = 0; i < static_cast<int>(Graph::Num); i++)
	{
		m_handles.push_back(LoadGraph(kFileNames[i]));
		assert(m_handles.back() != -1);
	}
}

void PlayerSpecialUI::Update()
{
	m_frame++;
}

void PlayerSpecialUI::Draw()
{
	int w, h;
	GetGraphSize(m_handles[static_cast<int>(Graph::Frame)], &w, &h);
	// 枠を描画
	int x = kOffsetX + w / 2 * kGraphScale;
	int y = kOffsetY + h / 2 * kGraphScale;
	DrawRotaGraph(x, y, kGraphScale, 0.0, m_handles[static_cast<int>(Graph::Frame)], true);
	// 本体を描画
	// ゲージの中身の長さを計算
	int gaugeLen = w - ((w - kGraphMaxX) + kGraphMinX);
	// 必殺技チャージの割合を計算
	auto player = m_pPlayer.lock();
	float rate = static_cast<float>(player->GetSpecialCharge()) / static_cast<float>(player->kMaxSpecialCharge);
	// 現在のゲージの長さを計算
	int width = rate * gaugeLen;
	int handle = m_handles[static_cast<int>(Graph::Main)];
	// ゲージを描画
	DrawRectRotaGraph(x - (gaugeLen - width) * kGraphScale / 2, y, kGraphMinX, 0, width, h, kGraphScale, 0.0, handle, true);

	// ボタンの描画
	// 最大までたまってるときのみ描画
	if (rate >= 1.0f)
	{
		x = kOffsetX + w * kGraphScale;
		y = kOffsetY + h * kGraphScale;
		// ちかちかさせる
		int handle = m_handles[static_cast<int>(Graph::LB)];
		if (m_frame % kBrinkFrame * 2 > kBrinkFrame)
		{
			handle = m_handles[static_cast<int>(Graph::LBOutline)];
		}

		DrawRotaGraph(x, y, kGraphScale, 0.0, handle, true);
	}
}
