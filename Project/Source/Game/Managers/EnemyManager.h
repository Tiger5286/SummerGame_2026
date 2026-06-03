#pragma once
#include <list>
#include <memory>
#include "../../Utility/Vector3.h"

class EnemyBase;
class Player;
class CollisionManager;

// 敵の種類を表す列挙体
enum class EnemyType
{
	Zombie,

	Num
};

class EnemyManager
{
public:
	EnemyManager();
	virtual ~EnemyManager();

	void Init(std::shared_ptr<Player> pPlayer);
	void End();
	void Update();
	void Draw();

	/// <summary>
	/// 新しい敵を追加する
	/// </summary>
	/// <param name="enemy">新しい敵のポインタ</param>
	/// <param name="pos">敵の位置</param>
	void AddEnemy(EnemyType type, const Vector3& pos);

	/// <summary>
	/// リストの最後の敵のポインタを取得する
	/// </summary>
	/// <returns>敵のポインタ</returns>
	std::shared_ptr<EnemyBase> GetLastEnemy();


private:
	int m_zombieModelHandle = -1;

	std::shared_ptr<CollisionManager> m_pColManager = nullptr;

	std::list<std::shared_ptr<EnemyBase>> m_enemyList;
	std::shared_ptr<Player> m_pPlayer = nullptr;
};