#pragma once
#include <memory>
#include <vector>
#include "Utility/Vector3.h"
#include "Utility/MyLib.h"

class Player;
class EnemyManager;

class EnemySpawner
{
public:
	// 出現させる敵の情報
	struct EnemyData
	{
		MyLib::EnemyType type = MyLib::EnemyType::None;
		Vector3 localPos;
	};
	// EnemySpawner自身が持つ情報
	struct Data
	{
		Vector3 pos;
		float radius = 0.0f;
		std::vector<EnemyData> enemyDatas;
	};
public:
	EnemySpawner() = default;
	~EnemySpawner() = default;

	void Init(std::shared_ptr<EnemyManager> pEnemyManager,std::shared_ptr<Player> pPlayer, EnemySpawner::Data spawnerData);
	void Update();
	void Draw();

private:
	void Spawn();

private:
	std::shared_ptr<Player> m_pPlayer = nullptr;
	std::shared_ptr<EnemyManager> m_pEnemyManager = nullptr;

	Vector3 m_pos;
	float m_radius = 0.0f;
	std::vector<EnemyData> m_enemyDatas;

	bool m_isSpawned = false;

};