#include "SceneTitle.h"
#include "DxLib.h"
#include "Singleton/Input.h"
#include "SceneManager.h"
#include "Game.h"
#include "Singleton/EffectManager.h"

#include "SceneMain.h"
#include "SceneOption.h"
#include "Application.h"

namespace
{
	const std::vector<std::wstring> kGraphFileNames = {
		L"data/Graphs/title.png",
		L"data/Graphs/StoneBrick.png",
		L"data/Graphs/TitleSelectUI.png"
	};

	enum class Graph
	{
		Title,
		TitleBack,
		Select,

		Num
	};

	constexpr int kFontSize = 30;
	constexpr int kMenuMargin = 20;
}

SceneTitle::SceneTitle(SceneManager& sceneManager) :
	SceneBase(sceneManager)
{
	m_menuActions[static_cast<int>(Menu::Start)] = {
		.name = L"はじめる",
		.action = [this]() {Start(); }
	};
	m_menuActions[static_cast<int>(Menu::Option)] = {
		.name = L"オプション",
		.action = [this]() {Option(); }
	};
	m_menuActions[static_cast<int>(Menu::Exit)] = {
		.name = L"ゲームを終了する",
		.action = [this]() {Exit(); }
	};
}

void SceneTitle::Init()
{
	// 画像を読み込み
	for (const auto& path : kGraphFileNames)
	{
		m_handles.push_back(LoadGraph(path.c_str()));
		assert(m_handles.back() != -1);
	}
	// エフェクトを読み込み
	EffectManager::GetInstance().LoadEffect(L"data/effects/TitleBackFlame.efkefc", L"TitleBackFlame", 100.0f);
	// エフェクトを再生
	m_effHandle = EffectManager::GetInstance().PlayEffect(L"TitleBackFlame", Vector3::Zero());
	// カメラを設定
	SetCameraPositionAndTarget_UpVecY(Vector3(0, 50, -500), Vector3(0, 50, 0));
	Effekseer_Sync3DSetting();
	// フォントを生成
	m_fontHandle = CreateFontToHandle(Game::kMainFontName, kFontSize, -1, DX_FONTTYPE_ANTIALIASING);
	assert(m_fontHandle != -1);
}

void SceneTitle::End()
{
	// 画像を削除
	for (auto& handle : m_handles)
	{
		DeleteGraph(handle);
	}
	// エフェクトを停止
	EffectManager::GetInstance().StopEffect(m_effHandle);
	// フォントを削除
	DeleteFontToHandle(m_fontHandle);
}

void SceneTitle::Update()
{
	auto& input = Input::GetInstance();

	// 上下キーでメニューの選択を変更する
	if (input.IsTriggerd(XINPUT_BUTTON_DPAD_DOWN))
	{
		m_selectIndex++;
		m_selectAlpha = 0.0f;
		if (m_selectIndex >= static_cast<int>(Menu::Num))
		{
			m_selectIndex = 0;
		}
	}
	if (input.IsTriggerd(XINPUT_BUTTON_DPAD_UP))
	{
		m_selectIndex--;
		m_selectAlpha = 0.0f;
		if (m_selectIndex < 0)
		{
			m_selectIndex = static_cast<int>(Menu::Num) - 1;
		}
	}
	// 決定ボタンで選択中のメニューのアクションを実行する
	if (input.IsTriggerd(XINPUT_BUTTON_A))
	{
		m_menuActions[m_selectIndex].action();
		return;
	}

	EffectManager::GetInstance().Update();
}

void SceneTitle::Draw()
{
	DrawRotaGraph(Game::kScreenWidth / 2, Game::kScreenHeight / 2, 1.3, 0.0, m_handles[static_cast<int>(Graph::TitleBack)], false);

	EffectManager::GetInstance().Draw();

	DrawRotaGraph(Game::kScreenWidth / 2, Game::kScreenHeight / 2 - 50, 0.8, 0.0, m_handles[static_cast<int>(Graph::Title)], true);

	int x = Game::kScreenWidth / 2;
	int y = Game::kScreenHeight / 2 + 180;
	m_selectAlpha = std::lerp(m_selectAlpha, 1.0f, 0.2f);
	for (int i = 0; i < static_cast<int>(Menu::Num); i++)
	{
		if (i == m_selectIndex)
		{
			int w, h;
			GetGraphSize(m_handles[static_cast<int>(Graph::Select)], &w, &h);
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_selectAlpha * 255);
			DrawRotaGraph(x, y + i * (kFontSize + kMenuMargin) - h / 2 + kFontSize, 1.0, 0.0, m_handles[static_cast<int>(Graph::Select)], true);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
	}
	for (int i = 0; i < static_cast<int>(Menu::Num); i++)
	{
		unsigned int color = 0x888888;
		if (i == m_selectIndex) color = 0xffffff;
		int strWidth = GetDrawStringWidthToHandle(m_menuActions[i].name.c_str(), m_menuActions[i].name.size(), m_fontHandle);
		DrawStringToHandle(x - strWidth / 2, y + i * (kFontSize + kMenuMargin), m_menuActions[i].name.c_str(), color, m_fontHandle);
	}

#ifdef _DEBUG
	DrawString(0, 0, L"SceneTitle", 0xffffff);
	DrawStringToHandle(0, 16, L"フォントテストtest試験", 0xffffff, m_fontHandle);
	//DrawGrid();
#endif
}

void SceneTitle::Start()
{
	m_sceneManager.ChangeSceneWithFade(std::make_shared<SceneMain>(m_sceneManager), true);
}

void SceneTitle::Option()
{
	m_sceneManager.PushScene(std::make_shared<SceneOption>(m_sceneManager));
}

void SceneTitle::Exit()
{
	Application::GetInstance().RequestExit();
}