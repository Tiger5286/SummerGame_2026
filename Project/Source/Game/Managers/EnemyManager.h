#pragma once
#include <list>
#include <memory>
#include "Utility/MyLib.h"
#include "../../Utility/Vector3.h"

class EnemyBase;
class Player;
class CollisionManager;

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
	void AddEnemy(MyLib::EnemyType type, const Vector3& pos);

	/// <summary>
	/// 敵のリストを取得する
	/// </summary>
	/// <returns>敵のリスト</returns>
	std::list<std::shared_ptr<EnemyBase>> GetEnemies() const { return m_enemyList; }

	/// <summary>
	/// リストの最後の敵のポインタを取得する
	/// </summary>
	/// <returns>敵のポインタ</returns>
	std::shared_ptr<EnemyBase> GetLastEnemy();

	/// <summary>
	/// ボスが存在するかどうか取得する
	/// </summary>
	/// <returns>true:存在する / false:存在しない</returns>
	bool IsAliveBoss();


private:
	std::list<std::shared_ptr<EnemyBase>> m_enemyList;
	std::shared_ptr<Player> m_pPlayer = nullptr;
};