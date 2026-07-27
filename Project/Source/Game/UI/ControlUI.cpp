#include "ControlUI.h"
#include <string>
#include "Game.h"
#include "Singleton/Input.h"
#include "../Character/Player/Player.h"
#include <string_view>
#include "ControlUILeft.h"
#include "Singleton/UIManager.h"

namespace
{
	constexpr int kLayer = 2;
	constexpr int kActionNum = 4;
	constexpr float kLerpT = 0.5f;

	constexpr int kDrawPosBaseX = Game::kScreenWidth - 250;
	constexpr int kDrawPosBaseY = Game::kScreenHeight - 120;
	constexpr int kDrawDist = 60;
	constexpr int kDrawRadius = 30;

	constexpr int kTextOffset = 10;

	enum class Graph
	{
		Attack,
		Jump,
		Spin,
		Shift,
		SpinBlack,
		Cooltime,
		CooltimeBlack,

		Num
	};

	constexpr std::wstring_view kFilePath[] = {
		L"data/Graphs/Game/AttackIcon.png",
		L"data/Graphs/Game/JumpIcon.png",
		L"data/Graphs/Game/SpinIcon.png",
		L"data/Graphs/Game/ShiftIcon.png",
		L"data/Graphs/Game/SpinIcon_black.png",
		L"data/Graphs/Game/Cooltime.png",
		L"data/Graphs/Game/Cooltime_black.png"
	};

	static_assert(static_cast<int>(Graph::Num) == std::size(kFilePath));

	constexpr int kButton[kActionNum] = { XINPUT_BUTTON_X,XINPUT_BUTTON_A,XINPUT_BUTTON_Y,XINPUT_BUTTON_B };
	constexpr float kMaxScale = 1.2f;

	constexpr int kFontSize = 20;
	constexpr std::wstring_view kActionNames[kActionNum] = {
		L"スラッシュ",
		L"ジャンプ",
		L"ウィングスピン",
		L"フレイムシフト"
	};

	enum class Buttons
	{
		X,A,Y,B,
		Num
	};
}

ControlUI::ControlUI() :
	UIBase(kLayer)
{
}

ControlUI::~ControlUI()
{
	for (auto& handle : m_handles)
	{
		DeleteGraph(handle);
	}
}

void ControlUI::Init()
{
	for (auto& path : kFilePath)
	{
		m_handles.push_back(LoadGraph(path.data()));
		assert(m_handles.back() != -1);
		m_scale.push_back(1.0f);
	}
	m_fontHandle = CreateFontToHandle(Game::kMainFontName, kFontSize, -1);

	m_pUILeft = std::make_shared<ControlUILeft>();
	m_pUILeft->SetInfo(m_fontHandle);
	UIManager::GetInstance().AddUI(m_pUILeft);
}

void ControlUI::Update()
{
	auto& input = Input::GetInstance();
	// ボタンを押したとき拡縮する
	for (int i = 0; i < kActionNum; i++)
	{
		if (input.IsPressed(kButton[i]))
		{
			m_scale[i] = std::lerp(m_scale[i], kMaxScale, kLerpT);
		}
		else
		{
			m_scale[i] = std::lerp(m_scale[i], 1.0f, kLerpT);
		}
	}
}

void ControlUI::Draw()
{
	// UIの中心位置
	const int baseX = kDrawPosBaseX;
	const int baseY = kDrawPosBaseY;
	// UI同士の距離
	const int dist = kDrawDist;
	// UIの半径
	const int r = kDrawRadius;

	// それぞれのUIの位置を設定
	int x[kActionNum] = { baseX - dist, baseX,        baseX,        baseX + dist };
	int y[kActionNum] = { baseY,        baseY + dist, baseY - dist, baseY };

	// 画像のサイズを取得
	int w, h;
	GetGraphSize(m_handles[static_cast<int>(Graph::Attack)], &w, &h);
	float exRate = static_cast<float>(r * 2) / static_cast<float>(w);	// 画像をUIのサイズにする拡大率を計算

	// それぞれのUIを描画
	for (int i = 0; i < static_cast<int>(Graph::Num); i++)
	{
		// UIを描画
		float scale = exRate * m_scale[i];
		DrawRotaGraph(x[i], y[i], scale, 0.0, m_handles[i], true);

		// スキルUIのときの特別な描画
		if (i == static_cast<int>(Graph::Spin))
		{
			// プレイヤーのスキルクールタイムの割合を計算
			auto player = m_pPlayer.lock();
			float rate = static_cast<float>(player->GetSkillCooltime()) / static_cast<float>(player->kSkillCooltime);
			// クールタイム中ならゲージ等を描画
			if (rate < 1.0f)
			{
				DrawRotaGraph(x[i], y[i], scale * kMaxScale, 0.0, m_handles[static_cast<int>(Graph::CooltimeBlack)], true);	// クールタイムゲージの背景
				DrawCircleGauge(x[i], y[i], rate * 100.0f, m_handles[static_cast<int>(Graph::Cooltime)], 0.0, scale * kMaxScale);	// クールタイムゲージ本体

				DrawRotaGraph(x[i], y[i], scale, 0.0, m_handles[static_cast<int>(Graph::SpinBlack)], true);	// アイコンを暗くする
			}
		}

		// テキストを描画
		if (i == static_cast<int>(Buttons::X) || i == static_cast<int>(Buttons::A))	// X,A
		{
			int width = GetDrawStringWidthToHandle(kActionNames[i].data(), kActionNames[i].size(), m_fontHandle);
			DrawStringToHandle(x[i] - (r + kTextOffset) - width, y[i] - kFontSize / 2, kActionNames[i].data(), 0xffffff, m_fontHandle);
		}
		else if (i == static_cast<int>(Buttons::Y) || i == static_cast<int>(Buttons::B))	// Y,B
		{
			DrawStringToHandle(x[i] + (r + kTextOffset), y[i] - kFontSize / 2, kActionNames[i].data(), 0xffffff, m_fontHandle);
		}
	}
}