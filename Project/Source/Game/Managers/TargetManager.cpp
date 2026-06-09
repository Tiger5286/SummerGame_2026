#include "TargetManager.h"
#include "EnemyManager.h"
#include "../Character/Player/Player.h"
#include "../Camera/Camera.h"
#include "../Character/Enemy/EnemyBase.h"
#include <limits>

namespace
{
	constexpr float kNoTargetDist = 1000.0f;
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
			float minDist = FLT_MAX;
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
				}
			}
			if (minDist > kNoTargetDist * kNoTargetDist)
			{
				m_pTarget = nullptr;
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
		// 一定の距離離れていたらnullptrにする
		Vector3 playerToEnemyVec = m_pPlayer->GetPos() - m_pTarget->GetPos();
		float squareDist = playerToEnemyVec.SquaredLength();
		if (squareDist > kNoTargetDist * kNoTargetDist)
		{
			m_pTarget = nullptr;
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

void TargetManager::Draw()
{
	// ターゲットがいないなら処理しない
	if (m_pTarget == nullptr)
	{
		return;
	}

	auto pos = m_pTarget->GetPos() + Vector3::Up() * 200.0f;
	auto screenPos = ConvWorldPosToScreenPos(pos.ToDxLib());
	DrawCircle(screenPos.x, screenPos.y, 10,0xff0000, true);
}
