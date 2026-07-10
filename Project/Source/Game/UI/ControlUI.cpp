#include "ControlUI.h"
#include <string>
#include "Game.h"
#include "Singleton/Input.h"
#include "../Character/Player/Player.h"
//#include <cassert>

namespace
{
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

	const std::vector<std::wstring> kFilePath = {
		L"data/Graphs/Game/AttackIcon.png",
		L"data/Graphs/Game/JumpIcon.png",
		L"data/Graphs/Game/SpinIcon.png",
		L"data/Graphs/Game/ShiftIcon.png",
		L"data/Graphs/Game/SpinIcon_black.png",
		L"data/Graphs/Game/Cooltime.png",
		L"data/Graphs/Game/Cooltime_black.png"
	};

	constexpr int kButton[4] = { XINPUT_BUTTON_X,XINPUT_BUTTON_A,XINPUT_BUTTON_Y,XINPUT_BUTTON_B };
	constexpr float kMaxScale = 1.2f;
}

ControlUI::ControlUI() :
	UIBase(2)
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
		m_handles.push_back(LoadGraph(path.c_str()));
		assert(m_handles.back() != -1);
		m_scale.push_back(1.0f);
	}
}

void ControlUI::Update()
{
	auto& input = Input::GetInstance();
	for (int i = 0; i < 4; i++)
	{
		if (input.IsPressed(kButton[i]))
		{
			m_scale[i] = std::lerp(m_scale[i], kMaxScale, 0.5f);
		}
		else
		{
			m_scale[i] = std::lerp(m_scale[i], 1.0f, 0.5f);
		}
	}
}

void ControlUI::Draw()
{
	int baseX = Game::kScreenWidth - 250;
	int baseY = Game::kScreenHeight - 120;
	int dist = 60;
	int r = 30;

	int x[4] = { baseX - dist,baseX,baseX,baseX + dist };
	int y[4] = { baseY,baseY + dist,baseY - dist,baseY };

	int w, h;
	GetGraphSize(m_handles[static_cast<int>(Graph::Attack)], &w, &h);
	float exRate = static_cast<float>(r * 2) / static_cast<float>(w);

	for (int i = 0; i < static_cast<int>(Graph::Num); i++)
	{
		float scale = exRate * m_scale[i];
		DrawRotaGraph(x[i], y[i], scale, 0.0, m_handles[i], true);

		if (i == static_cast<int>(Graph::Spin))
		{
			auto player = m_pPlayer.lock();
			float rate = static_cast<float>(player->GetSkillCooltime()) / static_cast<float>(player->kSkillCooltime);
			if (rate < 1.0f)
			{
				DrawRotaGraph(x[i], y[i], scale * 1.2f, 0.0, m_handles[static_cast<int>(Graph::CooltimeBlack)], true);
				DrawCircleGauge(x[i], y[i], rate * 100.0f, m_handles[static_cast<int>(Graph::Cooltime)], 0.0, scale * 1.2f);

				auto a = DrawRotaGraph(x[i], y[i], scale, 0.0, m_handles[static_cast<int>(Graph::SpinBlack)], true);
			}
		}
	}
}
