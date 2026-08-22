#include "SceneStageSelect.h"
#include "Singleton/Input.h"
#include "Game.h"
#include <string_view>
#include <array>
#include "Singleton/SoundManager.h"

#include "SceneManager.h"
#include "SceneMain.h"
#include "SceneTitle.h"

namespace
{
	// ステージ名
	constexpr std::array<std::wstring_view, static_cast<int>(MyLib::Stage::Num)> kStageNames = {
		L"ステージ 1",
		L"ステージ 2",
	};
	constexpr std::wstring_view kStartStageString = L"ステージ開始";
	constexpr int kStartStageStringY = Game::kScreenHeight / 2 + 200;
	constexpr float kStartStageStringSinScale = 20.0f;
	constexpr float kStartStageStringScaleWidth = 0.1f;

	// ステージごとの固有のデータ
	const SceneMain::UniqueDatas kUniqueFiles[static_cast<int>(MyLib::Stage::Num)] = {
		{
			.stage = MyLib::Stage::First,
			.collisionFilePath = L"data/Stages/First/Collision.mv1",
			.stageFilePath = L"data/Stages/First/Stage.mv1",
			.spawnerDataFilePath = L"data/Stages/First/SpawnerData.dat",
			.bossDirectionData = {
				.lightDir = Vector3(0, 0, 1),
				.cameraPos = Vector3(-5800, 400, 14800),
				.cameraTargetPos = Vector3(-5800, 400, 14800) + Vector3::Front()
			}
#ifdef _DEBUG
		,.bossRoomEntrancePos = Vector3(-5800, 200, 13000)
#endif
		},
		{
			.stage = MyLib::Stage::Second,
			.collisionFilePath = L"data/Stages/Second/Collision.mv1",
			.stageFilePath = L"data/Stages/Second/Stage.mv1",
			.spawnerDataFilePath = L"data/Stages/Second/SpawnerData.dat",
			.bossDirectionData = {
				.lightDir = Vector3(0, 0, -1),
				.cameraPos = Vector3(3100, 130, -6300),
				.cameraTargetPos = Vector3(3100, 0, -6800)
			}
#ifdef _DEBUG
		,.bossRoomEntrancePos = Vector3(3100, -130, -4700)
#endif
		}
	};

	constexpr const wchar_t* kBGMFilePath = L"data/Sounds/BGM/StageSelect.ogg";

	constexpr const wchar_t* kBackGraphs[static_cast<int>(MyLib::Stage::Num)] = {
		L"data/Graphs/FirstStage.png",
		L"data/Graphs/SecondStage.png"
	};
	constexpr float kBackAlphaLerpT = 0.2f;

	constexpr int kFontSize = 50;

	constexpr int kSideStringOffsetX = 100;

	constexpr int kSelectArrowOffsetX = 430;
	constexpr float kSelectArrowScale = 0.5f;
	constexpr float kSelectArrowSinScale = 10.0f;
	constexpr int kSelectArrowMoveWidth = 5;
}

SceneStageSelect::SceneStageSelect(SceneManager& sceneManager):
	SceneBase(sceneManager)
{
}

SceneStageSelect::~SceneStageSelect()
{
}

void SceneStageSelect::Init()
{
	auto& soundManager = SoundManager::GetInstance();
	soundManager.LoadSound(L"StageSelectBGM", kBGMFilePath, SoundManager::SoundType::BGM);
	soundManager.PlaySoundGame(L"StageSelectBGM", true, true);

	for (int i = 0; i < static_cast<int>(MyLib::Stage::Num); i++)
	{
		m_backHandles[i] = LoadGraph(kBackGraphs[i]);
	}
	m_backAlpha[static_cast<int>(MyLib::Stage::First)] = 1.0f;
	m_buttonHandle = LoadGraph(L"data/Graphs/Buttons/A.png");
	m_arrowHandle = LoadGraph(L"data/Graphs/SelectArrow.png");

	m_fontHandle = CreateFontToHandle(Game::kMainFontName, kFontSize, -1);
}

void SceneStageSelect::End()
{
	auto& soundManager = SoundManager::GetInstance();
	soundManager.StopSound(L"StageSelectBGM", true);
	soundManager.DeleteSound(L"StageSelectBGM");

	for (auto& handle : m_backHandles)
	{
		DeleteGraph(handle);
	}
	DeleteGraph(m_buttonHandle);
	DeleteGraph(m_arrowHandle);

	DeleteFontToHandle(m_fontHandle);
}

void SceneStageSelect::Update()
{
	auto& input = Input::GetInstance();
	m_frame++;

	// 左右入力で選択しているステージを切り替え
	if (input.IsTriggerd(XINPUT_BUTTON_DPAD_RIGHT,true))
	{
		// 次のステージの番号
		int nextSelect = static_cast<int>(m_selectStage) + 1;
		// 番号がステージの数を上回っていたら戻す
		if (nextSelect >= static_cast<int>(MyLib::Stage::Num))
		{
			nextSelect = static_cast<int>(MyLib::Stage::Num) - 1;
		}
		// 実際の選択しているステージに適用
		m_selectStage = static_cast<MyLib::Stage>(nextSelect);
		SoundManager::GetInstance().PlaySoundGame(L"Select");
	}

	if (input.IsTriggerd(XINPUT_BUTTON_DPAD_LEFT, true))
	{
		// 次のステージの番号
		int nextSelect = static_cast<int>(m_selectStage) - 1;
		// 番号がステージの数を下回っていたら戻す
		if (nextSelect < 0)
		{
			nextSelect = 0;
		}
		// 実際の選択しているステージに適用
		m_selectStage = static_cast<MyLib::Stage>(nextSelect);
		SoundManager::GetInstance().PlaySoundGame(L"Select");
	}

	// 決定入力でステージ開始
	if (input.IsTriggerd(XINPUT_BUTTON_A))
	{
		auto sceneMain = std::make_shared<SceneMain>(m_sceneManager);
		sceneMain->SetData(kUniqueFiles[static_cast<int>(m_selectStage)]);
		m_sceneManager.ChangeSceneWithFade(sceneMain, true);
		SoundManager::GetInstance().PlaySoundGame(L"Decision");
		return;
	}
	
	// キャンセル入力でタイトルに戻る
	if (input.IsTriggerd(XINPUT_BUTTON_B))
	{
		m_sceneManager.ChangeSceneWithFade(std::make_shared<SceneTitle>(m_sceneManager), false);
		SoundManager::GetInstance().PlaySoundGame(L"Cancel");
		return;
	}

	// 背景のアルファ値を選択中のステージに合わせて変化させる
	for (int i = 0; i < static_cast<int>(MyLib::Stage::Num); i++)
	{
		if (i == static_cast<int>(m_selectStage))
		{
			m_backAlpha[i] = std::lerp(m_backAlpha[i], 1.0f, kBackAlphaLerpT);
		}
		else
		{
			m_backAlpha[i] = std::lerp(m_backAlpha[i], 0.0f, kBackAlphaLerpT);
		}
	}
}

void SceneStageSelect::Draw()
{
	// 背景描画
	for (int i = 0; i < static_cast<int>(MyLib::Stage::Num); i++)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(m_backAlpha[i] * 255));
		DrawGraph(0, 0, m_backHandles[i], true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	// 選択中のステージ(真ん中)
	int strW = GetDrawStringWidthToHandle(kStageNames[static_cast<int>(m_selectStage)].data(), kStageNames[static_cast<int>(m_selectStage)].size(), m_fontHandle);
	DrawStringToHandle(Game::kScreenWidth / 2 - strW / 2,Game::kScreenHeight / 2 - kFontSize / 2,kStageNames[static_cast<int>(m_selectStage)].data(), 0xffffff, m_fontHandle);

	// 選択より一つ前のステージ(左)
	int prevStage = static_cast<int>(m_selectStage) - 1;
	// ひとつ前のステージが存在するステージの範囲内なら描画
	float arrowOffsetX = sinf(m_frame / kSelectArrowSinScale) * kSelectArrowMoveWidth;
	if (prevStage >= 0)
	{
		DrawStringToHandle(kSideStringOffsetX, Game::kScreenHeight / 2 - kFontSize / 2, kStageNames[prevStage].data(), 0x888888,m_fontHandle);
		DrawRotaGraph(kSelectArrowOffsetX + arrowOffsetX, Game::kScreenHeight / 2, kSelectArrowScale, DX_PI_F, m_arrowHandle, true);
	}

	// 選択より一つ後のステージ(右)
	int nextStage = static_cast<int>(m_selectStage) + 1;
	// ひとつ後のステージが存在するステージの範囲内なら描画
	if (nextStage < static_cast<int>(MyLib::Stage::Num))
	{
		strW = GetDrawStringWidthToHandle(kStageNames[nextStage].data(), kStageNames[nextStage].size(), m_fontHandle);
		DrawStringToHandle(Game::kScreenWidth - kSideStringOffsetX - strW, Game::kScreenHeight / 2 - kFontSize / 2, kStageNames[nextStage].data(), 0x888888,m_fontHandle);
		DrawRotaGraph(Game::kScreenWidth - kSelectArrowOffsetX + arrowOffsetX, Game::kScreenHeight / 2, kSelectArrowScale, 0.0f, m_arrowHandle, true);
	}

	strW = GetDrawStringWidthToHandle(kStartStageString.data(), kStartStageString.size(), m_fontHandle);
	DrawStringToHandle(Game::kScreenWidth / 2 - strW / 2, kStartStageStringY - kFontSize / 2, kStartStageString.data(), 0xffffff, m_fontHandle);
	int w, h;
	GetGraphSize(m_buttonHandle, &w, &h);
	float scale = sinf(m_frame / kStartStageStringSinScale) * kStartStageStringScaleWidth;
	DrawRotaGraph(Game::kScreenWidth / 2 - strW / 2 - w / 2, kStartStageStringY, 1.0f + scale, 0.0, m_buttonHandle, true);

#ifdef _DEBUG
	DrawString(0, 0, L"SceneStageSelect", 0xffffff);
#endif
}