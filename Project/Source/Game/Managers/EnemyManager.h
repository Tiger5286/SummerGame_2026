#pragma once
#include <list>
#include <memory>
#include "../../Utility/Vector3.h"

class EnemyBase;

class EnemyManager
{
public:
	EnemyManager();
	virtual ~EnemyManager();

	void Init();
	void End();
	void Update();
	void Draw();

	/// <summary>
	/// 新しい敵を追加する
	/// </summary>
	/// <param name="enemy">新しい敵のポインタ</param>
	/// <param name="pos">敵の位置</param>
	void AddEnemy(std::shared_ptr<EnemyBase> enemy, const Vector3& pos);

private:
	std::list<std::shared_ptr<EnemyBase>> m_enemyList;
};