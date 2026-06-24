#include "EnemySpawner.h"
#include "Character/Player/Player.h"
#include "Managers/EnemyManager.h"

void EnemySpawner::Init(std::shared_ptr<EnemyManager> pEnemyManager, std::shared_ptr<Player> pPlayer, EnemySpawner::Data spawnerData)
{
	m_pPlayer = pPlayer;
	m_pEnemyManager = pEnemyManager;
	m_pos = spawnerData.pos;
	m_radius = spawnerData.radius;
	m_enemyDatas = spawnerData.enemyDatas;
}

void EnemySpawner::Update()
{
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
}
