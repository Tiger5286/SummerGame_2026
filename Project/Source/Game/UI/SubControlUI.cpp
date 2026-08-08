#include "SubControlUI.h"
#include <string_view>
#include "Game.h"
#include <cassert>

namespace
{
	constexpr int kLayer = 2;

	enum class Graph
	{
		RB,
		RStickPush,

		Num
	};

	constexpr std::wstring_view kFileNames[] = {
		L"data/Graphs/Buttons/RB.png",
		L"data/Graphs/Buttons/RStickPush.png"
	};

	constexpr std::wstring_view kTexts[] = {
		L"回避",
		L"ロックオン"
	};

	constexpr int kUINum = 2;

	static_assert(static_cast<int>(Graph::Num) == std::size(kFileNames));

	constexpr int kDrawPosX = Game::kScreenWidth - 50;
	constexpr int kDrawPosY = Game::kScreenHeight / 2 + 100;

	constexpr float kGraphScale = 0.5f;
}

SubControlUI::SubControlUI() : 
	UIBase(kLayer)
{
}

SubControlUI::~SubControlUI()
{
	for (int handle : m_handles)
	{
		DeleteGraph(handle);
	}
}

void SubControlUI::Init()
{
	m_handles.reserve(static_cast<int>(Graph::Num));
	for (int i = 0; i < static_cast<int>(Graph::Num); i++)
	{
		m_handles.push_back(LoadGraph(kFileNames[i].data()));
		assert(m_handles.back() != -1);
	}
}

void SubControlUI::Update()
{
}

void SubControlUI::Draw()
{
	int w, h;
	GetGraphSize(m_handles[0], &w, &h);
	for (int i = 0; i < kUINum; i++)
	{
		int x = kDrawPosX;
		int y = kDrawPosY - h * kGraphScale * i;
		DrawRotaGraph(x, y, kGraphScale, 0.0, m_handles[i], true);
		int strW = GetDrawStringWidthToHandle(kTexts[i].data(), kTexts[i].size(), m_fontHandle);
		x = kDrawPosX - w / 2 * kGraphScale - strW;
		DrawStringToHandle(x, y, kTexts[i].data(), 0xffffff, m_fontHandle);
	}
}
