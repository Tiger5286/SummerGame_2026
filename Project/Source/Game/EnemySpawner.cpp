#include "EnemySpawner.h"
#include "Character/Player/Player.h"
#include "Managers/EnemyManager.h"
#include "Character/Enemy/EnemyBase.h"
#include "Singleton/EffectManager.h"

void EnemySpawner::Init(std::shared_ptr<EnemyManager> pEnemyManager, std::shared_ptr<Player> pPlayer, EnemySpawner::Data spawnerData)
{
	m_pPlayer = pPlayer;
	m_pEnemyManager = pEnemyManager;
	m_pos = spawnerData.pos;
	m_radius = spawnerData.radius;
	m_enemyDatas = spawnerData.enemyDatas;
	m_tag = spawnerData.tag;
}

void EnemySpawner::Update()
{
	// 敵を掃討した場合は処理しない
	if (m_isDefeatedEnemies)
	{
		return;
	}

	// 敵がいない場合、かつすでに出現させた場合掃討した判定
	bool isEnemiesEmpty = m_pEnemyManager->GetEnemies().empty();
	if (isEnemiesEmpty && m_isSpawned)
	{
		m_isDefeatedEnemies = true;
		//EffectManager::GetInstance().GetManager()->SendTrigger(m_effHandle, 0);
		EffectManager::GetInstance().StopEffect(m_effHandle);
	}

	// 敵がいる場合、かつすでに出現させた場合はプレイヤー、敵を範囲内にとどめる
	if (!isEnemiesEmpty && m_isSpawned)
	{
		KeepCharacterInArea();
	}

	// すでに出現させた場合は処理しない
	if (m_isSpawned)
	{
		return;
	}

	// プレイヤーが敵を出現させる範囲に入ったら出現させる
	Vector3 toPlayer = m_pPlayer->GetPos() - m_pos;
	if (toPlayer.SquaredLength() < m_radius * m_radius)
	{
		Spawn();
		m_isSpawned = true;
	}
}

void EnemySpawner::Draw()
{
#ifdef _DEBUG
	unsigned int color = 0x00ff00;
	if (m_isSpawned) color = 0xff0000;
	DrawSphere3D(m_pos.ToDxLib(), m_radius, 8, color, 0xffffff, false);
#endif
}

void EnemySpawner::Spawn()
{
	for (auto& data : m_enemyDatas)
	{
		m_pEnemyManager->AddEnemy(data.type, m_pos + data.localPos);
	}
	m_effHandle = EffectManager::GetInstance().PlayEffect(L"BattleArea", m_pos);
	float scale = m_radius / 1000 * 2;
	SetScalePlayingEffekseer3DEffect(m_effHandle, scale, scale, scale);
}

void EnemySpawner::KeepCharacterInArea()
{
	Vector3 spawnerPosXZ = m_pos;
	spawnerPosXZ.y = 0.0f;

	// プレイヤー
	Vector3 playerPosXZ = m_pPlayer->GetPos();
	playerPosXZ.y = 0.0f;
	float squaredDist = (playerPosXZ - spawnerPosXZ).SquaredLength();
	if (squaredDist > m_radius * m_radius)
	{
		float pushDist = sqrtf(squaredDist) - m_radius;
		Vector3 pushVec = (spawnerPosXZ - playerPosXZ).Normalized();
		pushVec *= pushDist;
		m_pPlayer->SetPos(m_pPlayer->GetPos() + pushVec);
	}
	// 敵
	for (auto& enemy : m_pEnemyManager->GetEnemies())
	{
		Vector3 enemyPosXZ = enemy->GetPos();
		enemyPosXZ.y = 0.0f;

		squaredDist = (enemyPosXZ - spawnerPosXZ).SquaredLength();
		if (squaredDist > m_radius * m_radius)
		{
			float pushDist = sqrtf(squaredDist) - m_radius;
			Vector3 pushVec = (spawnerPosXZ - playerPosXZ).Normalized();
			pushVec *= pushDist;
			enemy->SetPos(enemy->GetPos() + pushVec);
		}
	}
}
