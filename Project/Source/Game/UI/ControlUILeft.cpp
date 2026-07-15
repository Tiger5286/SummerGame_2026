#include "ControlUILeft.h"
#include <string_view>
#include "Game.h"
#include <cassert>

namespace
{
	constexpr int kLayer = 2;

	enum class Graph
	{
		LStick,
		RStick,
		RB,

		Num
	};

	constexpr std::wstring_view kFileNames[] = {
		L"data/Graphs/Buttons/LStick.png",
		L"data/Graphs/Buttons/RStick.png",
		L"data/Graphs/Buttons/RB.png"
	};

	constexpr std::wstring_view kTexts[] = {
		L"移動",
		L"カメラ操作",
		L"回避"
	};

	constexpr int kUINum = 3;

	static_assert(static_cast<int>(Graph::Num) == std::size(kFileNames));

	constexpr int kDrawPosX = 50;
	constexpr int kDrawPosY = Game::kScreenHeight - 100;

	constexpr float kGraphScale = 0.5f;
}

ControlUILeft::ControlUILeft() : 
	UIBase(kLayer)
{
}

ControlUILeft::~ControlUILeft()
{
	for (int handle : m_handles)
	{
		DeleteGraph(handle);
	}
}

void ControlUILeft::Init()
{
	m_handles.reserve(static_cast<int>(Graph::Num));
	for (int i = 0; i < static_cast<int>(Graph::Num); i++)
	{
		m_handles.push_back(LoadGraph(kFileNames[i].data()));
		assert(m_handles.back() != -1);
	}
}

void ControlUILeft::Update()
{
}

void ControlUILeft::Draw()
{
	int w, h;
	GetGraphSize(m_handles[0], &w, &h);
	for (int i = 0; i < kUINum; i++)
	{
		int x = kDrawPosX;
		int y = kDrawPosY - h * kGraphScale * i;
		DrawRotaGraph(x, y, kGraphScale, 0.0, m_handles[i], true);
		x += w / 2 * kGraphScale;
		DrawStringToHandle(x, y, kTexts[i].data(), 0xffffff, m_fontHandle);
	}
}
