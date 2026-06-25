#include "TargetManager.h"
#include "EnemyManager.h"
#include "../Character/Player/Player.h"
#include "../Camera/Camera.h"
#include "../Character/Enemy/EnemyBase.h"
#include "Singleton/Input.h"
#include <limits>
#include "Game.h"

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
	auto& input = Input::GetInstance();
	// 右スティック押し込みでターゲット
	if (input.IsTriggerd(XINPUT_BUTTON_RIGHT_THUMB))
	{
		// ターゲットがいなければ近い敵をターゲット
		if (m_pTarget == nullptr)
		{
			m_pTarget = SearchTarget();
		}
		else	// ターゲットがいれば解除
		{
			m_pTarget = nullptr;
		}
	}

	// 十字キー左右でターゲット切り替え

#if false

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

#endif

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
	// スクリーン座標のzが0.0~1.0の範囲でなければ無効
	if (screenPos.z > 0.0f && screenPos.z < 1.0f)
	{
		DrawCircle(screenPos.x, screenPos.y, 10, 0xff0000, true);
	}
}

std::shared_ptr<EnemyBase> TargetManager::SearchTarget()
{
	// 敵リスト取得
	auto enemies = m_pEnemyManager->GetEnemies();
	std::list<std::shared_ptr<EnemyBase>> noTargetEnemies;
	// 画面外の敵を除外
	for (auto& enemy : enemies)
	{
		auto pos = enemy->GetPos();
		// スクリーン座標に変換
		pos = Vector3::FromDxLib(ConvWorldPosToScreenPos(pos.ToDxLib()));
		// NearFarの範囲内かどうか
		bool isInNearFar = pos.z > 0.0f && pos.z < 1.0f;
		// 画面内かどうか
		bool isInScreen = pos.x > 0 && pos.y > 0 && pos.x < Game::kScreenWidth && pos.y < Game::kScreenHeight;
		// どちらかの条件を満たさなければ除外
		if (!isInNearFar || !isInScreen)
		{
			noTargetEnemies.push_back(enemy);
		}
	}
	// ターゲット対象外の敵を除外
	for (auto& enemy : noTargetEnemies)
	{
		enemies.remove(enemy);
	}
	// 対象の敵がいなければreturn
	if (enemies.empty())
	{
		return nullptr;
	}

	// 最も近い敵を抽出
	float minDist = FLT_MAX;	// 最も近い距離
	std::shared_ptr<EnemyBase> minDistEnemy = nullptr;	// 最も近い距離の敵
	for (auto& enemy : enemies)
	{
		auto enemyPos = enemy->GetPos();
		auto playerPos = m_pPlayer->GetPos();
		// 敵とプレイヤーとの距離を算出
		auto squaredDist = (enemyPos - playerPos).SquaredLength();
		// 最も短い距離とその敵を記録
		if (squaredDist < minDist)
		{
			minDist = squaredDist;
			minDistEnemy = enemy;
		}
	}
	// ターゲットになる敵を返す
	return minDistEnemy;
}
