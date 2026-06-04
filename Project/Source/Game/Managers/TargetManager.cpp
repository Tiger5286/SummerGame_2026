#include "TargetManager.h"
#include "EnemyManager.h"
#include "../Character/Player/Player.h"
#include "../Camera.h"
#include "../Character/Enemy/EnemyBase.h"
#include <limits>

namespace
{
	constexpr float kFloatMax = (std::numeric_limits<float>::max)();
}

void TargetManager::Init(std::shared_ptr<Player> pPlayer, std::shared_ptr<Camera> pCamera, std::shared_ptr<EnemyManager> pEnemyManager)
{
	m_pPlayer = pPlayer;
	m_pCamera = pCamera;
	m_pEnemyManager = pEnemyManager;
}

void TargetManager::Update()
{
	// ターゲットがいない場合
	if (m_pTarget == nullptr)
	{
		// 敵がいる場合、一番近い敵をターゲットにする
		if(!m_pEnemyManager->GetEnemies().empty())
		{
			// 敵とプレイヤーの最短距離を記録し、ターゲットをその敵にする
			auto playerPos = m_pPlayer->GetPos();
			float minDist = kFloatMax;
			for (const auto& enemy : m_pEnemyManager->GetEnemies())
			{
				// 死んでいるモーション中なら無視する
				if (enemy->IsDying())
				{
					continue;
				}

				auto enemyPos = enemy->GetPos();
				float dist = (playerPos - enemyPos).SquaredLength();
				if (dist < minDist)
				{
					minDist = dist;
					m_pTarget = enemy;
					m_pPlayer->SetTarget(enemy);
					m_pCamera->SetTarget(enemy);
				}
			}
		}
	}
	else	// ターゲットがいる場合
	{
		// ターゲットが生きているかどうかチェックする
		bool isFindEnemy = false;
		for (const auto& enemy : m_pEnemyManager->GetEnemies())
		{
			if (m_pTarget == enemy)
			{
				isFindEnemy = true;
				// ターゲットが死ぬモーション中なら無視する
				if (enemy->IsDying())
				{
					isFindEnemy = false;
				}
			}
		}
		// ターゲットが死んでいたらnullptrにする
		if (!isFindEnemy)
		{
			m_pTarget = nullptr;
		}
	}
	// ターゲットを設定
	m_pPlayer->SetTarget(m_pTarget);
	m_pCamera->SetTarget(m_pTarget);
}
