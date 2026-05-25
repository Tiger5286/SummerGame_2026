#include "EnemyManager.h"
#include "../GameObject/EnemyBase.h"

EnemyManager::EnemyManager()
{}

EnemyManager::~EnemyManager()
{}

void EnemyManager::Init()
{}

void EnemyManager::End()
{}

void EnemyManager::Update()
{}

void EnemyManager::Draw()
{}

void EnemyManager::AddEnemy(std::shared_ptr<EnemyBase> enemy, const Vector3& pos)
{
	//enemy->SetPos(pos);
	m_enemyList.push_back(enemy);
}
