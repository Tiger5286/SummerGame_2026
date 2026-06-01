#include "EnemyManager.h"
#include "../Character/EnemyBase.h"
#include <cassert>

EnemyManager::EnemyManager()
{}

EnemyManager::~EnemyManager()
{}

void EnemyManager::Init(std::shared_ptr<Player> pPlayer)
{
	assert(pPlayer != nullptr && "EnemyManager::Init() : プレイヤーのポインタがnullptrです");
	
	m_pPlayer = pPlayer;

	for (auto& enemy : m_enemyList)
	{
		enemy->SetPlayer(m_pPlayer);
		enemy->Init();
	}
}

void EnemyManager::End()
{
	for (auto& enemy : m_enemyList)
	{
		enemy->End();
	}
}

void EnemyManager::Update()
{
	// 死んでいる敵を一時的に保存するリスト
	std::list<std::shared_ptr<EnemyBase>> m_deadEnemyList;
	// 敵を更新する
	for (auto& enemy : m_enemyList)
	{
		enemy->Update();
		if (enemy->IsDead())
		{
			m_deadEnemyList.push_back(enemy);
		}
	}
	// 死んでいる敵をリストから削除する
	for (auto& enemy : m_deadEnemyList)
	{
		m_enemyList.remove(enemy);
	}
}

void EnemyManager::Draw()
{
	for (auto& enemy : m_enemyList)
	{
		enemy->Draw();
	}
}

void EnemyManager::AddEnemy(std::shared_ptr<EnemyBase> enemy, const Vector3& pos)
{
	enemy->SetPos(pos);
	m_enemyList.push_back(enemy);

	enemy->SetPlayer(m_pPlayer);
	enemy->Init();
}
