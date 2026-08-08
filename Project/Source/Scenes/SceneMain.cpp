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
#include "Singleton/SoundManager.h"

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
#include "Game/TutorialManager.h"

#include "Game/SkyBox.h"

#include "../Game/Managers/SpawnerManager.h"

namespace
{
	// ロードするモデルのファイル名と登録名
	const std::vector<std::pair<std::wstring, std::wstring>> kModelFileNames = {
		{ L"Player", L"data/models/Player/Player.mv1" },
		{ L"Wing" , L"data/models/Player/Wing.mv1" },
		{ L"Zombie", L"data/models/Enemy/Zombie.mv1" },
		{ L"Vulture", L"data/models/Enemy/Vulture.mv1" },
		{ L"General",L"data/models/Enemy/General.mv1"},
		{ L"GeneralSword",L"data/models/Enemy/sword.mv1"},
		{ L"ZombieBoss",L"data/models/Enemy/ZombieBoss.mv1"}
	};

	// ロードするSEのデータ
	struct SEData
	{
		const wchar_t* filePath;
		const wchar_t* name;
	};
	constexpr SEData kSEDatas[] = {
		{ L"data/Sounds/SE/Player/Slash1.mp3", L"LightSlash" },
		{ L"data/Sounds/SE/Player/Dodge.mp3", L"Dodge" },
		{ L"data/Sounds/SE/Player/Shift.mp3", L"Shift" },
		{ L"data/Sounds/SE/Player/Spin.mp3", L"Spin" },
		{ L"data/Sounds/SE/Player/Burning.mp3", L"Burning" },
		{ L"data/Sounds/SE/Damage.mp3", L"Damage" },
		{L"data/Sounds/SE/Enemy/ZombieBossAppear.mp3", L"ZombieBossAppear"},
		{L"data/Sounds/SE/Enemy/GeneralAppear.mp3", L"GeneralAppear"},
		{L"data/Sounds/SE/Enemy/ZombieAppear.mp3",L"ZombieAppear"},
		{L"data/Sounds/SE/Enemy/ZombieDeath.mp3",L"ZombieDeath"},
		{L"data/Sounds/SE/Enemy/VultureAppear.mp3",L"VultureAppear"},
		{L"data/Sounds/SE/Enemy/VultureDeath.mp3",L"VultureDeath"},
	};

	// ロードするエフェクトのデータ
	const std::vector<SceneMain::EffectData> kEffectFileDatas =
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
		},
		{
			.path = L"data/effects/Hit.efkproj",
			.name = L"Hit",
			.scale = 30.0f
		},
		{
			.path = L"data/effects/BossRoar.efkproj",
			.name = L"BossRoar",
			.scale = 50.0f
		},
		{
			.path = L"data/effects/BossAttackProjectileSlash.efkproj",
			.name = L"BossProjectileSlash",
			.scale = 100.0f
		},
		{
			.path = L"data/effects/BossAttackProjectile.efkproj",
			.name = L"BossProjectile",
			.scale = 100.0f
		},
		{
			.path = L"data/effects/EnemyDeath.efkproj",
			.name = L"EnemyDeath",
			.scale = 50.0f
		}
	};

	constexpr float kStageLightY = -0.5f;
	const Vector3 kBossDirectionLight = Vector3(0, 0, -1);
	constexpr int kBossDirectionWaitFrame = 60;
	constexpr int kBossDirectionEndFrame = 330;

	constexpr int kShadowMapSize = 8192;
	const Vector3 kShadowMapMinPos = Vector3(-3000, -1000, -3000);
	const Vector3 kShadowMapMaxPos = Vector3(3000, 1000, 3000);

	constexpr const wchar_t* kBGMFilePaths[static_cast<int>(SceneMain::Stage::Num)] = {
		L"data/Sounds/BGM/FirstStage.ogg",
		L"data/Sounds/BGM/SecondStage.ogg"
	};
	constexpr const wchar_t* kBossBGMFilePath = L"data/Sounds/BGM/BossFight.ogg";

#ifdef _DEBUG
	const Vector3 kBossRoomPosFirst = Vector3(-5800, 200, -11500);
	const Vector3 kBossRoomPosSecond = Vector3(3100, -130, -4700);
#endif
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
	m_shadowMapHandle = MakeShadowMap(kShadowMapSize, kShadowMapSize);

	// モデルのロードと登録
	auto& modelManager = ModelManager::GetInstance();
	for (const auto& modelFileName : kModelFileNames)
	{
		modelManager.LoadModel(modelFileName.second, modelFileName.first);
	}
	modelManager.LoadModel(m_uniqueDatas.collisionFilePath, L"Collision");
	modelManager.LoadModel(m_uniqueDatas.stageFilePath, L"Stage");
	// エフェクトのロードと登録
	auto& effManager = EffectManager::GetInstance();
	for (const auto& effData : kEffectFileDatas)
	{
		effManager.LoadEffect(effData.path, effData.name, effData.scale);
	}

	// 関数をイベントマネージャーに登録
	m_onSpawnBossHandle = EventManager::GetInstance().Register("SpawnBoss", [this]() {OnSpawnBoss(); }, shared_from_this());

	// UIの初期化
	UIManager::GetInstance().Init();

	// ステージの初期化
	auto stage = modelManager.GetModelHandle(L"Collision");
	MV1SetMatrix(stage, Matrix4x4::GetRotY(DX_PI_F).ToDxLib());
	stage = modelManager.GetModelHandle(L"Stage");
	MV1SetMatrix(stage, Matrix4x4::GetRotY(DX_PI_F).ToDxLib());

	// プレイヤーの生成
	m_pPlayer = std::make_shared<Player>();
	m_pPlayer->SetHandle(modelManager.GetModelHandle(L"Player"));
	m_pPlayer->SetMapHandle(modelManager.GetModelHandle(L"Collision"));
	m_pPlayer->Init();
	// カメラの生成
	m_pCamera = std::make_shared<Camera>();
	m_pCamera->SetMapHandle(modelManager.GetModelHandle(L"Collision"));
	m_pCamera->SetBossDirectionData(m_uniqueDatas.bossDirectionData.cameraPos, m_uniqueDatas.bossDirectionData.cameraTargetPos);
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
	m_pSpawnerManager->LoadBinaly(m_uniqueDatas.spawnerDataFilePath);

	// ターゲットマネージャーの生成
	m_pTargetManager = std::make_shared<TargetManager>();
	m_pTargetManager->Init(m_pPlayer, m_pCamera, m_pEnemyManager);

	// スカイボックスの生成
	m_pSkyBox = std::make_shared<SkyBox>();
	m_pSkyBox->Init();

	// BGMのロードと再生
	auto& soundManager = SoundManager::GetInstance();
	soundManager.LoadSound(L"BossBGM", kBossBGMFilePath, SoundManager::SoundType::BGM);
	soundManager.LoadSound(L"StageBGM", kBGMFilePaths[static_cast<int>(m_uniqueDatas.stage)], SoundManager::SoundType::BGM);
	soundManager.PlaySoundGame(L"StageBGM", true, true);
	// 効果音のロード
	for (auto& se : kSEDatas)
	{
		soundManager.LoadSound(se.name, se.filePath, SoundManager::SoundType::SE);
	}

	if (m_uniqueDatas.stage == Stage::First)
	{
		m_pTutorialManager = std::make_shared<TutorialManager>();
		m_pTutorialManager->Init();
	}
}

void SceneMain::End()
{
	// 各クラスの終了処理
	m_pPlayer->End();
	m_pSkyBox->End();

	m_pEnemyManager->End();

	// モデルの削除
	auto& modelManager = ModelManager::GetInstance();
	modelManager.DeleteAllModel();

	EffectManager::GetInstance().StopEffectAll();

	DeleteShadowMap(m_shadowMapHandle);

	// サウンドの停止と削除
	auto& soundManager = SoundManager::GetInstance();
	soundManager.StopSound(L"StageBGM", true);
	soundManager.StopSound(L"BossBGM", true);
	soundManager.DeleteSound(L"StageBGM");
	soundManager.DeleteSound(L"BossBGM");
	// 効果音の削除
	for(auto& se : kSEDatas)
	{
		soundManager.StopSound(se.name, true);
		soundManager.DeleteSound(se.name);
	}
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
		auto sceneGameOver = std::make_shared<SceneGameOver>(m_sceneManager);
		sceneGameOver->SetData(m_uniqueDatas);
		m_sceneManager.ChangeSceneWithFade(sceneGameOver,false);
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
		m_pPlayer->SetPos(m_uniqueDatas.bossRoomEntrancePos);
	}
	// enterで現在位置を出力
	if (key[KEY_INPUT_RETURN])
	{
		Vector3 pos = m_pPlayer->GetPos();
		printfDx(L"PlayerPos:%.2f,%.2f,%.2f\n", pos.x, pos.y, pos.z);
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
	SetUseShadowMap(0, m_shadowMapHandle);
	MV1DrawModel(modelManager.GetModelHandle(L"Stage"));
	SetUseShadowMap(0, -1);
#ifdef _DEBUG
	// ステージの当たり判定モデルの描画
	MV1DrawModel(modelManager.GetModelHandle(L"Collision"));
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
		lightVec = m_uniqueDatas.bossDirectionData.lightDir;
		SetLightDirection(lightVec);
	}

	// シャドウマップへ描画
	Vector3 smMinPos = m_pPlayer->GetPos() + kShadowMapMinPos;
	Vector3 smMaxPos = m_pPlayer->GetPos() + kShadowMapMaxPos;
	SetShadowMapDrawArea(m_shadowMapHandle, smMinPos, smMaxPos);
	ShadowMap_DrawSetup(m_shadowMapHandle);
	m_pPlayer->Draw();
	m_pEnemyManager->Draw();
	ShadowMap_DrawEnd();
	// 通常の描画
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

void SceneMain::SetData(UniqueDatas uniqueDatas)
{
	m_uniqueDatas = uniqueDatas;
}

void SceneMain::OnSpawnBoss()
{
	m_isDirectionSpawnBoss = true;
	FadeManager::GetInstance().StartFadeOut();
	m_pPlayer->SetCanControl(false);
	auto& soundManager = SoundManager::GetInstance();
	soundManager.StopSound(L"StageBGM", true);
	soundManager.PlaySoundGame(L"BossBGM", true, true);
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
