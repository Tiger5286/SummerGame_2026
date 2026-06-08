#include "SceneMain.h"
#include "DxLib.h"
#include <string>
#include <vector>

#include "../Singleton/ModelManager.h"
#include "Singleton/EffectManager.h"

#include "Game/Character/Player/Player.h"
#include "Game/Camera/Camera.h"
#include "Game/Character/Enemy/Zombie/Zombie.h"

#include "Game/Managers/EnemyManager.h"
#include "Game/Managers/TargetManager.h"

#include "Game/SkyBox.h"

namespace
{
	// ロードするモデルのファイル名と登録名
	const std::vector<std::pair<std::wstring, std::wstring>> kModelFileNames = {
		{ L"Stage", L"data/models/Stage.mv1" },
		{ L"Collision", L"data/models/Collision.mv1" },
		{ L"Player", L"data/models/Player.mv1" },
		{ L"Zombie", L"data/models/Zombie.mv1" }
	};
}

SceneMain::SceneMain()
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
	effManager.LoadEffect(L"data/effects/ShiftEffect.efk", L"Shift",100.0f);

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

	// 仮の敵を生成
	m_pEnemyManager->AddEnemy(EnemyType::Zombie, Vector3(0, 0, 800));
	m_pEnemyManager->AddEnemy(EnemyType::Zombie, Vector3(300, 0, 800));
	m_pEnemyManager->AddEnemy(EnemyType::Zombie, Vector3(-300, 0, 800));

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

	EffectManager::GetInstance().Update();
}

void SceneMain::Draw()
{
	// 空の描画
	m_pSkyBox->Draw();

	// ステージの描画
	auto& modelManager = ModelManager::GetInstance();
	MV1DrawModel(modelManager.GetModelHandle(L"Stage"));

#ifdef _DEBUG
	// グリッドの描画
	DrawGrid();
#endif

	// 各クラスの描画
	m_pPlayer->Draw();

	m_pEnemyManager->Draw();

	EffectManager::GetInstance().Draw();

	// ターゲットUIの描画
	m_pTargetManager->Draw();

#ifdef _DEBUG
	// デバッグ表示
	DrawString(0,0,L"SceneMain",0xffffff);
	DrawFormatString(0, 16, 0xffffff, L"FRAME:%d", m_frameCount);
#endif
}

void SceneMain::DrawGrid()
{
	// 直線の始点と終点
	VECTOR startPos;
	VECTOR endPos;

	for (int z = -300; z <= 300; z += 100)
	{
		startPos = VGet(-300.0f, 0.0f, static_cast<float>(z));
		endPos = VGet(300.0f, 0.0f, static_cast<float>(z));
		DrawLine3D(startPos, endPos, 0xff0000);
	}
	for (int x = -300; x <= 300; x += 100)
	{
		startPos = VGet(static_cast<float>(x), 0.0f, -300.0f);
		endPos = VGet(static_cast<float>(x), 0.0f, 300.0f);
		DrawLine3D(startPos, endPos, 0x0000ff);
	}
}
