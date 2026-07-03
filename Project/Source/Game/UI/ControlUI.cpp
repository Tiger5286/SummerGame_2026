#include "ControlUI.h"
#include <string>
#include "Game.h"
#include "Singleton/Input.h"
#include "../Character/Player/Player.h"

namespace
{
	enum class Graph
	{
		Attack,
		Jump,
		Spin,
		Shift,

		Num
	};

	const std::vector<std::wstring> kFilePath = {
		L"data/Graphs/AttackIcon.png",
		L"data/Graphs/JumpIcon.png",
		L"data/Graphs/SpinIcon.png",
		L"data/Graphs/ShiftIcon.png"
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
		m_scale.push_back(1.0f);
	}
	m_cooltimeHandle = LoadGraph(L"data/Graphs/Cooltime.png");
}

void ControlUI::Update()
{
	auto& input = Input::GetInstance();
	for (int i = 0; i < 4; i++)
	{
		if (input.IsPressed(kButton[i]))
		{
			m_scale[i] = std::lerp(m_scale[i], kMaxScale, 0.1f);
		}
		else
		{
			m_scale[i] = std::lerp(m_scale[i], 1.0f, 0.7f);
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
		if (i == static_cast<int>(Graph::Spin))
		{
			auto player = m_pPlayer.lock();
			float rate = static_cast<float>(player->GetSkillCooltime()) / static_cast<float>(player->kSkillCooltime);
			if (rate < 1.0f)
			{
				DrawCircleGauge(x[i], y[i], rate * 100.0f, m_cooltimeHandle, 0.0, exRate * m_scale[i] * 1.2f);
			}
		}

		float scale = exRate * m_scale[i];
		DrawRotaGraph(x[i], y[i], scale, 0.0, m_handles[i], true);
	}
}
