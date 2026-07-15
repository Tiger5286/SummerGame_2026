#include "SceneMain.h"
#include "DxLib.h"
#include <string>
#include <vector>

#include "Singleton/ModelManager.h"
#include "Singleton/EffectManager.h"
#include "Singleton/CameraSetter.h"
#include "Singleton/Input.h"
#include "Singleton/UIManager.h"
#include "Singleton/EventManager.h"
#include "Singleton/FadeManager.h"

#include "SceneManager.h"
#include "ScenePause.h"
#include "SceneGameOver.h"
#include "SceneClear.h"

#include "Game/Character/Player/Player.h"
#include "Game/Camera/Camera.h"
#include "Game/Camera/CameraStateAppearBoss.h"
#include "Game/Camera/CameraStateFree.h"

#include "Game/Managers/EnemyManager.h"
#include "Game/Managers/TargetManager.h"

#include "Game/SkyBox.h"

#include "../Game/Managers/SpawnerManager.h"

namespace
{
	// ロードするモデルのファイル名と登録名
	const std::vector<std::pair<std::wstring, std::wstring>> kModelFileNames = {
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
		},
		{
			.path = L"data/effects/BattleArea.efkefc",
			.name = L"BattleArea",
			.scale = 100.0f
		},
		{
			.path = L"data/effects/SwordRush.efkefc",
			.name = L"SwordRush",
			.scale = 50.0f
		}
	};

	constexpr float kStageLightY = -0.5f;
	const Vector3 kBossDirectionLight = Vector3(0, 0, -1);
	constexpr int kBossDirectionWaitFrame = 60;
	constexpr int kBossDirectionEndFrame = 330;

#ifdef _DEBUG
	const Vector3 kBossRoomPos = Vector3(3100, -130, -4700);
#endif
}

SceneMain::SceneMain(SceneManager& sceneManager) :
	SceneBase(sceneManager)
{
}

SceneMain::~SceneMain()
{
	// イベントマネージャーに登録した関数を削除
	EventManager::GetInstance().UnRegister(m_onSpawnBossHandle);
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

	m_onSpawnBossHandle = EventManager::GetInstance().Register("SpawnBoss", [this]() {OnSpawnBoss(); });

	// UIの初期化
	UIManager::GetInstance().Init();

	// ステージの初期化
	auto stage = modelManager.GetModelHandle(L"Collision");
	MV1SetMatrix(stage, Matrix4x4::GetRotY(DX_PI_F).ToDxLib());

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

	// カメラ設定クラスの初期化
	CameraSetter::GetInstance().Init(m_pCamera);

	// 敵管理クラスの生成
	m_pEnemyManager = std::make_shared<EnemyManager>();
	m_pEnemyManager->Init(m_pPlayer);

	// スポナークラスの生成(スポナーのデータをロード)
	m_pSpawnerManager = std::make_shared<SpawnerManager>();
	m_pSpawnerManager->Init(m_pEnemyManager, m_pPlayer);
	//m_pSpawnerManager->Loadcsv();
	m_pSpawnerManager->LoadBinaly();

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

	// モデルの削除		// ほんとは使わなくなるやつだけ削除すべきだけど、バグ防止のためにとりあえず全部消してる　あとで直したい
	auto& modelManager = ModelManager::GetInstance();
	for (auto& model : kModelFileNames)
	{
		modelManager.DeleteModel(model.first);
	}

	EffectManager::GetInstance().StopEffectAll();
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

	// UIの更新
	UIManager::GetInstance().Update();

	// ボス戦前演出
	if (m_isDirectionSpawnBoss)
	{
		SpawnBossUpdate();
	}

	// ボスがいたらボス戦フラグをつける
	if (m_pEnemyManager->IsAliveBoss())
	{
		m_isBossBattle = true;
	}
	else if (m_isBossBattle == true && !m_isEndScene)	// ボスがいない、かつボス戦中ならボスが死んだと判断し、クリアに遷移する
	{
		m_isEndScene = true;
		m_sceneManager.ChangeSceneWithFade(std::make_shared<SceneClear>(m_sceneManager), false);
		return;
	}

	// ポーズが押されたらポーズシーンに遷移する
	auto& input = Input::GetInstance();
	if (input.IsTriggerd(XINPUT_BUTTON_START))
	{
		m_sceneManager.PushScene(std::make_shared<ScenePause>(m_sceneManager));
		return;
	}
	// プレイヤーが死んだらゲームオーバー
	if (m_pPlayer->IsDead() && !m_isEndScene)
	{
		m_sceneManager.ChangeSceneWithFade(std::make_shared<SceneGameOver>(m_sceneManager),false);
		m_isEndScene = true;
		return;
	}

#ifdef _DEBUG
	// デバッグ機能
	char key[256];
	GetHitKeyStateAll(key);
	// 1キーでボス部屋にテレポート
	if (key[KEY_INPUT_1])
	{
		m_pPlayer->SetPos(kBossRoomPos);
	}
#endif
}

void SceneMain::Draw()
{
	// 空の描画
	m_pSkyBox->Draw();

	// ステージ用のライトの向きを設定する
	Vector3 lightVec = m_pPlayer->GetPos() - m_pCamera->GetPos();
	lightVec.Normalize();
	lightVec.y = kStageLightY;
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
	// ボス戦演出中ならライトの向きを固定する
	if (m_isDirectionSpawnBoss)
	{
		lightVec = kBossDirectionLight;
		SetLightDirection(lightVec);
	}
	// 各クラスの描画
	m_pPlayer->Draw();

	m_pEnemyManager->Draw();

	m_pSpawnerManager->Draw();

	EffectManager::GetInstance().Draw();

	// ボス出現演出中はUIを描画しない
	if (!m_isDirectionSpawnBoss)
	{
		// UIの描画
		UIManager::GetInstance().Draw();
	}

#ifdef _DEBUG
	// デバッグ表示
	DrawString(0,0,L"SceneMain",0xffffff);
	DrawFormatString(0, 16, 0xffffff, L"FRAME:%d", m_frameCount);
#endif
}

void SceneMain::OnSpawnBoss()
{
	m_isDirectionSpawnBoss = true;
	FadeManager::GetInstance().StartFadeOut();
	m_pPlayer->SetCanControl(false);
}

void SceneMain::SpawnBossUpdate()
{
	m_directionBossFrameCount++;
	auto& fadeManager = FadeManager::GetInstance();
	if (m_directionBossFrameCount == kBossDirectionWaitFrame)
	{
		fadeManager.StartFadeIn();
		m_pCamera->ChangeState(std::make_shared<CameraStateAppearBoss>());
		m_pCamera->SetBossBattle(true);
	}
	if (m_directionBossFrameCount > kBossDirectionEndFrame)
	{
		m_isDirectionSpawnBoss = false;
		m_pPlayer->SetCanControl(true);
	}
}
