#include "PlayerSpecialUI.h"
#include "../Character/Player/Player.h"
#include "Singleton/CameraSetter.h"

namespace
{
	constexpr float kGraphScale = 0.5f;

	constexpr int kGraphMinX = 60;
	constexpr int kGraphMaxX = 739;

	enum class Graph
	{
		Frame,
		Main,

		Num
	};

	constexpr const wchar_t* kFileNames[] = {
		L"data/Graphs/Game/SpecialGauge_Frame.png",
		L"data/Graphs/Game/SpecialGauge.png"
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
	for (int i = 0; i < static_cast<int>(Graph::Num); i++)
	{
		m_handles.push_back(LoadGraph(kFileNames[i]));
		assert(m_handles.back() != -1);
	}
}

void PlayerSpecialUI::Update()
{
}

void PlayerSpecialUI::Draw()
{
	int w, h;
	GetGraphSize(m_handles[static_cast<int>(Graph::Frame)], &w, &h);
	// 枠を描画
	int x = 30 + w / 2 * kGraphScale;
	int y = 80 + h / 2 * kGraphScale;
	DrawRotaGraph(x, y, kGraphScale, 0.0, m_handles[static_cast<int>(Graph::Frame)], true);
	// 本体を描画
	// ゲージの中身の長さを計算
	int gaugeLen = w - ((w - kGraphMaxX) + kGraphMinX);
	// プレイヤーのHPの割合を計算
	auto player = m_pPlayer.lock();
	float rate = static_cast<float>(player->GetSpecialCharge()) / static_cast<float>(player->kMaxSpecialCharge);
	// 現在のHPバーの長さを計算
	int width = rate * gaugeLen;
	int handle = m_handles[static_cast<int>(Graph::Main)];
	// HPバーを描画
	DrawRectRotaGraph(x - (gaugeLen - width) * kGraphScale * 0.5f, y, kGraphMinX, 0, width, h, kGraphScale, 0.0, handle, true);
	

}
