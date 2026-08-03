#include "SceneStageSelect.h"
#include "Singleton/Input.h"
#include "Game.h"
#include <string_view>
#include <array>

#include "SceneManager.h"
#include "SceneMain.h"
#include "SceneTitle.h"

namespace
{
	// ステージ名
	constexpr std::array<std::wstring_view, static_cast<int>(SceneStageSelect::Stage::Num)> kStageNames = {
		L"Stage 1",
		L"Stage 2",
		L"Stage 3"
	};
	// ステージごとの固有のデータ
	const SceneMain::UniqueFiles kUniqueFiles[static_cast<int>(SceneStageSelect::Stage::Num)] = {
		{
			.collisionFilePath = L"data/Stages/First/Collision.mv1",
			.stageFilePath = L"data/Stages/First/Stage.mv1",
			.spawnerDataFilePath = L"data/Stages/First/SpawnerData.dat"
		},
		{
			.collisionFilePath = L"data/Stages/Second/Collision.mv1",
			.stageFilePath = L"data/Stages/Second/Stage.mv1",
			.spawnerDataFilePath = L"data/Stages/Second/SpawnerData.dat"
		},
		{
			.collisionFilePath = L"data/Stages/First/Collision.mv1",
			.stageFilePath = L"data/Stages/First/Stage.mv1",
			.spawnerDataFilePath = L"data/Stages/First/SpawnerData.dat"
		}
	};
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
}

void SceneStageSelect::End()
{
}

void SceneStageSelect::Update()
{
	auto& input = Input::GetInstance();

	// 左右入力で選択しているステージを切り替え
	if (input.IsTriggerd(XINPUT_BUTTON_DPAD_RIGHT,true))
	{
		// 次のステージの番号
		int nextSelect = static_cast<int>(m_selectStage) + 1;
		// 番号がステージの数を上回っていたら戻す
		if (nextSelect >= static_cast<int>(Stage::Num))
		{
			nextSelect = static_cast<int>(Stage::Num) - 1;
		}
		// 実際の選択しているステージに適用
		m_selectStage = static_cast<Stage>(nextSelect);
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
		m_selectStage = static_cast<Stage>(nextSelect);
	}

	// 決定入力でステージ開始
	if (input.IsTriggerd(XINPUT_BUTTON_A))
	{
		auto sceneMain = std::make_shared<SceneMain>(m_sceneManager);
		sceneMain->SetData(kUniqueFiles[static_cast<int>(m_selectStage)]);
		m_sceneManager.ChangeSceneWithFade(sceneMain, true);
		return;
	}
	
	// キャンセル入力でタイトルに戻る
	if (input.IsTriggerd(XINPUT_BUTTON_B))
	{
		m_sceneManager.ChangeSceneWithFade(std::make_shared<SceneTitle>(m_sceneManager), false);
		return;
	}
}

void SceneStageSelect::Draw()
{
	// 選択中のステージ(真ん中)
	DrawString(Game::kScreenWidth / 2, Game::kScreenHeight / 2, kStageNames[static_cast<int>(m_selectStage)].data(), 0xffffff);

	// 選択より一つ前のステージ(左)
	int prevStage = static_cast<int>(m_selectStage) - 1;
	// ひとつ前のステージが存在するステージの範囲内なら描画
	if (prevStage >= 0)
	{
		DrawString(Game::kScreenWidth / 4, Game::kScreenHeight / 2, kStageNames[prevStage].data(), 0x888888);
	}

	// 選択より一つ後のステージ(右)
	int nextStage = static_cast<int>(m_selectStage) + 1;
	// ひとつ後のステージが存在するステージの範囲内なら描画
	if (nextStage < static_cast<int>(Stage::Num))
	{
		DrawString(Game::kScreenWidth / 4 * 3, Game::kScreenHeight / 2, kStageNames[nextStage].data(), 0x888888);
	}

#ifdef _DEBUG
	DrawString(0, 0, L"SceneStageSelect", 0xffffff);
#endif
}