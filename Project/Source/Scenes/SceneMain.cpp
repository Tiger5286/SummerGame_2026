#include "SceneMain.h"
#include "DxLib.h"
#include "../GameObject/Player.h"
#include "../System/Camera.h"
#include "../GameObject/Zombie.h"
#include "../System/SkyBox.h"

SceneMain::SceneMain()
{
}

SceneMain::~SceneMain()
{
}

void SceneMain::Init()
{
	// モデルの読み込み
	m_mapModelHandle = MV1LoadModel(L"data/models/Stage.mv1");
	m_mapColliderHandle = MV1LoadModel(L"data/models/Collision.mv1");

	m_playerModelHandle = MV1LoadModel(L"data/models/Player.mv1");
	m_zombieModelHandle = MV1LoadModel(L"data/models/Zombie.mv1");

	// プレイヤーの生成
	m_pPlayer = std::make_shared<Player>(m_input);
	m_pPlayer->SetHandle(m_playerModelHandle);
	m_pPlayer->SetMapHandle(m_mapColliderHandle);
	m_pPlayer->Init();
	// カメラの生成
	m_pCamera = std::make_shared<Camera>(m_input);
	m_pCamera->SetMapHandle(m_mapColliderHandle);
	m_pCamera->Init();
	// ゾンビの生成
	m_pZombie = std::make_shared<Zombie>();
	m_pZombie->SetHandle(m_zombieModelHandle);
	m_pZombie->Init();

	// スカイボックスの生成
	m_pSkyBox = std::make_shared<SkyBox>();
	m_pSkyBox->Init();
}

void SceneMain::End()
{
	MV1DeleteModel(m_playerModelHandle);
	MV1DeleteModel(m_mapModelHandle);
	MV1DeleteModel(m_mapColliderHandle);
	MV1DeleteModel(m_zombieModelHandle);

	m_pPlayer->End();
	m_pZombie->End();
	m_pSkyBox->End();
}

void SceneMain::Update()
{
	m_frameCount++;

	m_input.Update();

	// カメラの更新
	m_pCamera->SetPlayerPos(m_pPlayer->GetPos());
	m_pPlayer->SetCameraAngleY(m_pCamera->GetAngleY());

	// 各オブジェクトの更新
	m_pPlayer->Update();
	m_pCamera->Update();

	m_pZombie->SetPlayerPos(m_pPlayer->GetPos());
	m_pZombie->Update();

	m_pSkyBox->SetCameraPos(m_pCamera->GetPos());
	m_pSkyBox->Update();
}

void SceneMain::Draw()
{
	m_pSkyBox->Draw();

	MV1DrawModel(m_mapModelHandle);

#ifdef _DEBUG
	DrawGrid();
#endif

	m_pZombie->Draw();
	m_pPlayer->Draw();

#ifdef _DEBUG
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
