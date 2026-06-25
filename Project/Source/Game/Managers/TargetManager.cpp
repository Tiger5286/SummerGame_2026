#include "TargetManager.h"
#include "EnemyManager.h"
#include "../Character/Player/Player.h"
#include "../Camera/Camera.h"
#include "../Character/Enemy/EnemyBase.h"
#include "Singleton/Input.h"
#include <limits>
#include "Game.h"
#include <map>

namespace
{
	constexpr float kNoTargetDist = 1500.0f;
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
			auto enemies = m_pEnemyManager->GetEnemies();
			enemies = GetAliveEnemies(enemies);			// 生きている敵
			enemies = GetInSearchAreaEnemies(enemies);	// ターゲット範囲内の敵
			enemies = GetInScreenEnemies(enemies);		// 画面内の敵
			m_pTarget = GetNearestEnemy(enemies);		// 最も近い敵
			if (m_pTarget != nullptr)	// ターゲットできたらisTargetをtrue、できなかったらfalse
			{
				m_isTarget = true;
			}
			else
			{
				m_isTarget = false;
			}
		}
		else	// ターゲットがいれば解除
		{
			m_pTarget = nullptr;
			m_isTarget = false;
		}
	}

	// 十字キー左右でターゲット切り替え
	if (input.IsTriggerd(XINPUT_BUTTON_DPAD_LEFT))
	{
		SelectTarget(MyLib::LR::Left);
	}
	if (input.IsTriggerd(XINPUT_BUTTON_DPAD_RIGHT))
	{
		SelectTarget(MyLib::LR::Right);
	}


	// ターゲットがいる場合の処理
	if (m_isTarget)
	{
		CheckTarget();
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
	// スクリーン座標のzが0.0~1.0の範囲でなければ無効
	if (screenPos.z > 0.0f && screenPos.z < 1.0f)
	{
		DrawCircle(screenPos.x, screenPos.y, 10, 0xff0000, true);
	}
}

void TargetManager::CheckTarget()
{
	// ターゲットすべき敵がいなくなったらターゲットを解除
	auto enemies = m_pEnemyManager->GetEnemies();
	enemies = GetAliveEnemies(enemies);
	enemies = GetInSearchAreaEnemies(enemies);
	if (enemies.empty())
	{
		m_pTarget = nullptr;
		m_isTarget = false;
		return;
	}
	// ターゲットが一定の距離離れたらターゲットを解除
	auto squaredDist = (m_pTarget->GetPos() - m_pPlayer->GetPos()).SquaredLength();
	if (squaredDist > kNoTargetDist * kNoTargetDist)
	{
		m_pTarget = nullptr;
		m_isTarget = false;
		return;
	}
	// isTarget == trueなのにターゲットがnullptrの時 = ターゲットした敵が死んだと判断する → 近い敵にターゲットする
	if (m_pTarget == nullptr || m_pTarget->IsDying())
	{
		// 近い敵にターゲットする
		enemies = m_pEnemyManager->GetEnemies();
		enemies = GetAliveEnemies(enemies);			// 生きている敵
		enemies = GetInSearchAreaEnemies(enemies);	// ターゲット範囲内の敵
		m_pTarget = GetNearestEnemy(enemies);		// 最も近い敵
	}

}

void TargetManager::SelectTarget(MyLib::LR lr)
{
	// ターゲットがいないなら処理しない
	if (m_pTarget == nullptr) return;
	// 選択するターゲットを絞る
	auto enemies = m_pEnemyManager->GetEnemies();
	enemies = GetAliveEnemies(enemies);
	enemies = GetInSearchAreaEnemies(enemies);
	enemies = GetInScreenEnemies(enemies);

	// ターゲットのスクリーン位置
	Vector3 targetScreenPos = Vector3::FromDxLib(ConvWorldPosToScreenPos(m_pTarget->GetPos().ToDxLib()));

	// 敵のスクリーン座標マップ
	std::map<std::shared_ptr<EnemyBase>,Vector3> screenPosEnemies;
	for (auto& enemy : enemies)		// 敵のスクリーン座標を保存
	{
		screenPosEnemies[enemy] = Vector3::FromDxLib(ConvWorldPosToScreenPos(enemy->GetPos().ToDxLib()));
	}

	// 最短距離を記録
	float minDist = FLT_MAX;
	std::shared_ptr<EnemyBase> minDistEnemy = nullptr;
	for (auto& enemy : screenPosEnemies)
	{
		if (enemy.first == m_pTarget) continue;

		auto dist = targetScreenPos.x - enemy.second.x;

		if (lr == MyLib::LR::Left)
		{
			// 距離がマイナスになる = ターゲットより右側なので無視
			if (dist < 0.0f) continue;
		}
		else
		{
			// 距離がプラスになる = ターゲットより左側なので無視
			if (dist > 0.0f) continue;
		}
		dist = abs(dist);
		// 最も短い距離を記録
		if (dist < minDist)
		{
			minDist = dist;
			minDistEnemy = enemy.first;
		}
	}
	// 記録した敵をターゲットにする
	if (minDistEnemy != nullptr)
	{
		m_pTarget = minDistEnemy;
	}
}

std::list<std::shared_ptr<EnemyBase>> TargetManager::GetAliveEnemies(std::list<std::shared_ptr<EnemyBase>> enemies)
{
	// 死んでいる敵を除外
	std::list<std::shared_ptr<EnemyBase>> dyingEnemies;
	for (auto& enemy : enemies)
	{
		if (enemy->IsDying())
		{
			dyingEnemies.push_back(enemy);
		}
	}
	for (auto& enemy : dyingEnemies)
	{
		enemies.remove(enemy);
	}

	return enemies;
}


std::list<std::shared_ptr<EnemyBase>> TargetManager::GetInSearchAreaEnemies(std::list<std::shared_ptr<EnemyBase>> enemies)
{
	// ターゲットする範囲外の敵を除外
	std::list<std::shared_ptr<EnemyBase>> noTargetEnemies;
	for (auto& enemy : enemies)
	{
		auto squaredDist = (enemy->GetPos() - m_pPlayer->GetPos()).SquaredLength();
		if (squaredDist > kNoTargetDist * kNoTargetDist)
		{
			noTargetEnemies.push_back(enemy);
		}
	}
	for (auto& enemy : noTargetEnemies)
	{
		enemies.remove(enemy);
	}
	return enemies;
}

std::list<std::shared_ptr<EnemyBase>> TargetManager::GetInScreenEnemies(std::list<std::shared_ptr<EnemyBase>> enemies)
{
	// 画面外の敵を除外
	std::list<std::shared_ptr<EnemyBase>> noTargetEnemies;
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
	return enemies;
}

std::shared_ptr<EnemyBase> TargetManager::GetNearestEnemy(std::list<std::shared_ptr<EnemyBase>> enemies)
{
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

std::list<std::shared_ptr<EnemyBase>> TargetManager::RemoveTarget(std::list<std::shared_ptr<EnemyBase>> enemies)
{
	if (m_pTarget == nullptr) return enemies;

	bool findTarget = false;
	for (auto& enemy : enemies)
	{
		if (enemy == m_pTarget)
		{
			findTarget = true;
		}
	}
	if (findTarget)
	{
		enemies.remove(m_pTarget);
	}

	return enemies;
}
