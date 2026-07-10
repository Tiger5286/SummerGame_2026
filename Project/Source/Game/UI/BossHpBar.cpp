#include "BossHpBar.h"
#include "../Character/Enemy/EnemyBase.h"
#include "Singleton/CameraSetter.h"
#include "Game.h"

namespace
{
	constexpr float kGraphScale = 0.8f;
	constexpr int kOffsetX = -30;
	constexpr int kOffsetY = 40;

	constexpr int kGraphMinX = 21;
	constexpr int kGraphMaxX = 780;

	enum class Graph
	{
		Frame,
		Main,

		Num
	};

	constexpr const wchar_t* kFileNames[] = {
		L"data/Graphs/Game/BossHPBar_Frame.png",
		L"data/Graphs/Game/BossHPBar.png"
	};

	// 画像の種類とファイルパスの数が一致していないとエラー
	static_assert(static_cast<int>(Graph::Num) == sizeof(kFileNames) / sizeof(wchar_t*));
}

BossHpBar::BossHpBar() :
	UIBase(2)
{
}

BossHpBar::~BossHpBar()
{
	for (auto handle : m_handles)
	{
		DeleteGraph(handle);
	}
}

void BossHpBar::Init()
{
	for (int i = 0; i < static_cast<int>(Graph::Num); i++)
	{
		m_handles.push_back(LoadGraph(kFileNames[i]));
		assert(m_handles.back() != -1);
	}
}

void BossHpBar::Update()
{
	// 持ち主が死んでいたら自分も死ぬ
	if (m_pOwner.lock()->GetHP() <= 0)
	{
		m_isAlive = false;
	}
}

void BossHpBar::Draw()
{
	int w, h;
	GetGraphSize(m_handles[static_cast<int>(Graph::Frame)], &w, &h);
	// 枠を描画
	int x = Game::kScreenWidth + kOffsetX - w / 2 * kGraphScale;
	int y = kOffsetY + h / 2 * kGraphScale;
	DrawRotaGraph(x, y, kGraphScale, 0.0, m_handles[static_cast<int>(Graph::Frame)], true);
	// 本体を描画
	// ゲージの中身の長さを計算
	int gaugeLen = w - ((w - kGraphMaxX) + kGraphMinX);
	// 必殺技チャージの割合を計算
	auto boss = m_pOwner.lock();
	float rate = static_cast<float>(boss->GetHP()) / static_cast<float>(boss->GetMaxHP());
	// 現在のゲージの長さを計算
	int width = rate * gaugeLen;
	int handle = m_handles[static_cast<int>(Graph::Main)];
	// ゲージを描画
	DrawRectRotaGraph(x - (gaugeLen - width) * kGraphScale * 0.5f, y, kGraphMinX, 0, width, h, kGraphScale, 0.0, handle, true);
}
