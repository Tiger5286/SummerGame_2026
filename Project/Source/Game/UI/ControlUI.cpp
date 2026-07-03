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
	DeleteGraph(m_spinBlackHandle);
	DeleteGraph(m_cooltimeHandle);
	DeleteGraph(m_cooltimeBlackHandle);
}

void ControlUI::Init()
{
	for (auto& path : kFilePath)
	{
		m_handles.push_back(LoadGraph(path.c_str()));
		m_scale.push_back(1.0f);
	}
	m_spinBlackHandle = LoadGraph(L"data/Graphs/SpinIcon_black.png");
	m_cooltimeHandle = LoadGraph(L"data/Graphs/Cooltime.png");
	m_cooltimeBlackHandle = LoadGraph(L"data/Graphs/Cooltime_black.png");
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
				DrawRotaGraph(x[i], y[i], scale * 1.2f, 0.0, m_cooltimeBlackHandle, true);
				DrawCircleGauge(x[i], y[i], rate * 100.0f, m_cooltimeHandle, 0.0, scale * 1.2f);

				auto a = DrawRotaGraph(x[i], y[i], scale, 0.0, m_spinBlackHandle, true);
			}
		}
	}
}
