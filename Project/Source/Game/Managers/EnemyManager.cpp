#include "EnemyManager.h"
#include "../Character/Enemy/EnemyBase.h"
#include <cassert>
#include "../Character/Enemy/Zombie/Zombie.h"
#include "Singleton/ModelManager.h"

EnemyManager::EnemyManager()
{}

EnemyManager::~EnemyManager()
{}

void EnemyManager::Init(std::shared_ptr<Player> pPlayer, std::shared_ptr<CollisionManager> pColManager)
{
	assert(pPlayer != nullptr && "EnemyManager::Init() : プレイヤーのポインタがnullptrです");
	assert(pColManager != nullptr && "EnemyManager::Init() : コリジョンマネージャーのポインタがnullptrです");
	
	m_pPlayer = pPlayer;
	m_pColManager = pColManager;

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

void EnemyManager::AddEnemy(EnemyType type, const Vector3& pos)
{
	std::shared_ptr<EnemyBase> pEnemy = nullptr;
	switch (type)
	{
	case EnemyType::Zombie:
		pEnemy = std::make_shared<Zombie>();
		pEnemy->SetHandle(ModelManager::GetInstance().DuplicateModel(L"Zombie"));
		pEnemy->SetColManager(m_pColManager);
		pEnemy->SetPlayer(m_pPlayer);
		pEnemy->SetMapHandle(ModelManager::GetInstance().GetModelHandle(L"Collision"));
		pEnemy->SetPos(pos);
		pEnemy->Init();
		m_enemyList.push_back(pEnemy);
		break;
	default:
		assert(false && "EnemyManager::AddEnemy() : 未知のEnemyTypeが渡されました");
	}
}

std::shared_ptr<EnemyBase> EnemyManager::GetLastEnemy()
{
	return m_enemyList.back();
}
