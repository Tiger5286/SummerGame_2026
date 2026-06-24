#include "SceneMain.h"
#include "DxLib.h"
#include <string>
#include <vector>

#include "../Singleton/ModelManager.h"
#include "Singleton/EffectManager.h"

#include "SceneManager.h"

#include "Game/Character/Player/Player.h"
#include "Game/Camera/Camera.h"
#include "Game/Character/Enemy/Zombie/Zombie.h"

#include "Game/Managers/EnemyManager.h"
#include "Game/Managers/TargetManager.h"

#include "Game/SkyBox.h"

#include "../Game/Managers/SpawnerManager.h"

namespace
{
	// ロードするモデルのファイル名と登録名
	const std::vector<std::pair<std::wstring, std::wstring>> kModelFileNames = {
		{ L"Stage", L"data/models/Stage/Stage.mv1" },
		{ L"Collision", L"data/models/Stage/NewMapCollision.mv1" },
		{ L"Player", L"data/models/Player/Player.mv1" },
		{ L"Wing" , L"data/models/Player/Wing.mv1" },
		{ L"Zombie", L"data/models/Enemy/Zombie.mv1" },
		{ L"Vulture", L"data/models/Enemy/Vulture.mv1" },
		{L"General",L"data/models/Enemy/General.mv1"},
		{L"GeneralSword",L"data/models/Enemy/sword.mv1"}
	};
	// ロードするエフェクトのファイル名と登録名
	struct EffectData
	{
		std::wstring path;
		std::wstring name;
		float scale;
	};

	const std::vector<EffectData> kEffectFileDatas =
	{
		{
			.path = L"data/effects/ShiftEffect.efkefc",
			.name = L"Shift",
			.scale = 100.0f
		},
		{
			.path = L"data/effects/Burning.efkefc",
			.name = L"Burning",
			.scale = 50.0f
		},
		{
			.path = L"data/effects/ShiftEndEffect.efkefc",
			.name = L"ShiftEnd",
			.scale = 100.0f
		},
		{
			.path = L"data/effects/SpinEffect.efkefc",
			.name = L"Spin",
			.scale = 60.0f
		}
	};
}

SceneMain::SceneMain(SceneManager& sceneManager) :
	SceneBase(sceneManager)
{
}

SceneMain::~SceneMain()
{
}

void SceneMain::Init()
{
	// モデルのロードと登録
	auto& modelManager = ModelManager::GetInstance();
	for (const auto& modelFileName : kModelFileNames)
	{
		modelManager.LoadModel(modelFileName.second, modelFileName.first);
	}
	// エフェクトのロードと登録
	auto& effManager = EffectManager::GetInstance();
	for (const auto& effData : kEffectFileDatas)
	{
		effManager.LoadEffect(effData.path, effData.name, effData.scale);
	}

	// プレイヤーの生成
	m_pPlayer = std::make_shared<Player>();
	m_pPlayer->SetHandle(modelManager.GetModelHandle(L"Player"));
	m_pPlayer->SetMapHandle(modelManager.GetModelHandle(L"Collision"));
	m_pPlayer->Init();
	// カメラの生成
	m_pCamera = std::make_shared<Camera>();
	m_pCamera->SetMapHandle(modelManager.GetModelHandle(L"Collision"));
	m_pCamera->Init();

	m_pCamera->SetPlayer(m_pPlayer);
	m_pPlayer->SetCamera(m_pCamera);

	// 敵管理クラスの生成
	m_pEnemyManager = std::make_shared<EnemyManager>();
	m_pEnemyManager->Init(m_pPlayer);

	// スポナークラスの生成(スポナーのデータをロード)
	m_pSpawnerManager = std::make_shared<SpawnerManager>();
	m_pSpawnerManager->Init(m_pEnemyManager, m_pPlayer);
	m_pSpawnerManager->Load();

	// ターゲットマネージャーの生成
	m_pTargetManager = std::make_shared<TargetManager>();
	m_pTargetManager->Init(m_pPlayer, m_pCamera, m_pEnemyManager);

	// スカイボックスの生成
	m_pSkyBox = std::make_shared<SkyBox>();
	m_pSkyBox->Init();
}

void SceneMain::End()
{
	// 各クラスの終了処理
	m_pPlayer->End();
	m_pSkyBox->End();

	m_pEnemyManager->End();
}

void SceneMain::Update()
{
	m_frameCount++;

	// 各クラスの更新
	m_pPlayer->Update();
	m_pCamera->Update();
	
	m_pEnemyManager->Update();
	m_pTargetManager->Update();

	m_pSkyBox->SetCameraPos(m_pCamera->GetPos());
	m_pSkyBox->Update();

	m_pSpawnerManager->Update();

	EffectManager::GetInstance().Update();
}

void SceneMain::Draw()
{
	// 空の描画
	m_pSkyBox->Draw();

	// ステージ用のライトの向きを設定する
	Vector3 lightVec = m_pPlayer->GetPos() - m_pCamera->GetPos();
	lightVec.Normalize();
	lightVec.y = -0.5f;
	SetLightDirection(lightVec.ToDxLib());
	// ステージの描画
	auto& modelManager = ModelManager::GetInstance();
	//MV1DrawModel(modelManager.GetModelHandle(L"Stage"));
	MV1DrawModel(modelManager.GetModelHandle(L"Collision"));

#ifdef _DEBUG
	// グリッドの描画
	DrawGrid();
#endif

	// ライトの向きを設定する
	lightVec = m_pPlayer->GetPos() - m_pCamera->GetPos();
	lightVec.Normalize();
	SetLightDirection(lightVec.ToDxLib());
	// 各クラスの描画
	m_pPlayer->Draw();

	m_pEnemyManager->Draw();

	m_pSpawnerManager->Draw();

	EffectManager::GetInstance().Draw();

	// ターゲットUIの描画
	m_pTargetManager->Draw();

#ifdef _DEBUG
	// デバッグ表示
	DrawString(0,0,L"SceneMain",0xffffff);
	DrawFormatString(0, 16, 0xffffff, L"FRAME:%d", m_frameCount);
#endif
}
