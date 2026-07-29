#include "Menu.h"
#include "Singleton/Input.h"
#include "Game.h"

namespace
{
	constexpr int kFontSize = 30;
	constexpr int kMargin = 50;

	constexpr float kMaxScale = 1.1f;
	constexpr float kLerpT = 0.2f;

	constexpr unsigned int kTextColor = 0xffffff;
}

Menu::~Menu()
{
	DeleteGraph(m_uiHandle);
	DeleteGraph(m_selectUIHandle);

	DeleteFontToHandle(m_fontHandle);
}

void Menu::Init(std::vector<Funcs> actions, int x, int y,float scale)
{
	// 関数とそのアクション名を初期化
	m_items.resize(actions.size());
	for (int i = 0; i < actions.size(); i++)
	{
		m_items[i].name = actions[i].name;
		m_items[i].action = actions[i].action;
		// scaleとcolorRateは0.0で初期化
		m_items[i].type = actions[i].type;
		m_items[i].pBarRate = actions[i].pBarRate;
	}
	// 位置を初期化
	m_x = x;
	m_y = y;
	// 大きさを初期化
	m_scale = scale;

	m_uiHandle = LoadGraph(L"data/Graphs/Game/MenuUI.png");
	m_selectUIHandle = LoadGraph(L"data/Graphs/Game/MenuUI_Select.png");

	m_fontHandle = CreateFontToHandle(Game::kMainFontName, kFontSize, -1);
}

void Menu::Update()
{
	auto& input = Input::GetInstance();

	// 上下キーでメニューの選択を変更する
	if (input.IsTriggerd(XINPUT_BUTTON_DPAD_DOWN, true))
	{
		m_selectIndex++;
		if (m_selectIndex >= m_items.size())
		{
			m_selectIndex = 0;
		}
	}
	if (input.IsTriggerd(XINPUT_BUTTON_DPAD_UP, true))
	{
		m_selectIndex--;
		if (m_selectIndex < 0)
		{
			m_selectIndex = m_items.size() - 1;
		}
	}
	// 決定ボタンで選択中のメニューのアクションを実行する
	if (input.IsTriggerd(XINPUT_BUTTON_A))
	{
		m_items[m_selectIndex].action();
		return;
	}

	// UIの大きさをLerpで動かす
	for (int i = 0; i < m_items.size(); i++)
	{
		// 大きさをスムーズに変える
		if (m_selectIndex == i)
		{
			m_items[i].scale = std::lerp(m_items[i].scale, kMaxScale, kLerpT);
		}
		else
		{
			m_items[i].scale = std::lerp(m_items[i].scale, 1.0f, kLerpT);
		}
		// 色をスムーズに変える
		if (m_selectIndex == i)
		{
			m_items[i].colorRate = std::lerp(m_items[i].colorRate, 1.0f, kLerpT);
		}
		else
		{
			m_items[i].colorRate = std::lerp(m_items[i].colorRate, 0.0f, kLerpT);
		}
	}
}

void Menu::Draw()
{
	for (int i = 0; i < m_items.size(); i++)
	{
		int handle = m_uiHandle;
		if (i == m_selectIndex) handle = m_selectUIHandle;
		auto actionName = m_items[i].name;

		int origBlendMode, origBlendParam;
		GetDrawBlendMode(&origBlendMode, &origBlendParam);

		// 文字の背景の描画
		int black = (1.0f - m_items[i].colorRate) * 255;
		int red = m_items[i].colorRate * 255;
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, black);
		DrawRotaGraph(m_x, m_y + (kFontSize / 2 + (kFontSize + kMargin) * i) * m_scale, m_items[i].scale * m_scale, 0.0, m_uiHandle, true);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, red);
		DrawRotaGraph(m_x, m_y + (kFontSize / 2 + (kFontSize + kMargin) * i) * m_scale, m_items[i].scale * m_scale, 0.0, m_selectUIHandle, true);
		SetDrawBlendMode(origBlendMode, origBlendParam);

		// 文字の描画
		int width = GetDrawExtendStringWidthToHandle(m_items[i].scale, actionName.c_str(), actionName.size(), m_fontHandle);
		if (m_items[i].type == Type::Normal)
		{
			DrawExtendStringToHandle(m_x - (width / 2) * m_scale,
						   m_y + ((kFontSize + kMargin) * i) * m_scale,
						   m_items[i].scale * m_scale, m_items[i].scale * m_scale, actionName.c_str(), kTextColor, m_fontHandle);
		}
		else if (m_items[i].type == Type::Bar)
		{
			DrawExtendStringToHandle(m_x - (width / 2) * m_scale,
						   m_y + ((kFontSize + kMargin) * i) * m_scale,
						   m_items[i].scale * m_scale, m_items[i].scale * m_scale, L"バーの描画", kTextColor, m_fontHandle);
		}
	}
}