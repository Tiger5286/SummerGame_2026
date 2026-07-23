#pragma once
#include <memory>
#include <list>
#include "Utility/MyLib.h"

class Player;
class Camera;
class EnemyManager;
class EnemyBase;
class TargetUI;

class TargetManager : public std::enable_shared_from_this<TargetManager>
{
public:
	TargetManager() = default;
	~TargetManager();

	void Init(std::shared_ptr<Player> pPlayer,std::shared_ptr<Camera> pCamera,std::shared_ptr<EnemyManager> pEnemyManager);
	void Update();

private:

	/// <summary>
	/// ターゲットをするかしないかを判断する
	/// </summary>
	void CheckTarget();

	void SelectTarget(MyLib::LR lr);

	/// <summary>
	/// 生きている敵のリストを取得する
	/// </summary>
	/// <param name="enemies">絞りたい敵のリスト</param>
	/// <returns>生きている敵のみが入ったリスト</returns>
	std::list<std::shared_ptr<EnemyBase>> GetAliveEnemies(std::list<std::shared_ptr<EnemyBase>> enemies);

	/// <summary>
	/// ターゲットできる範囲内の敵リストを取得する
	/// </summary>
	/// <param name="enemies">絞りたい敵のリスト</param>
	/// <returns>ターゲットできる範囲内の敵のみが入ったリスト</returns>
	std::list<std::shared_ptr<EnemyBase>> GetInSearchAreaEnemies(std::list<std::shared_ptr<EnemyBase>> enemies,float dist);

	/// <summary>
	/// 画面内の敵リストを取得する
	/// </summary>
	/// <param name="enemies">絞りたい敵のリスト</param>
	/// <returns>画面内にいる敵のみが入ったリスト</returns>
	std::list<std::shared_ptr<EnemyBase>> GetInScreenEnemies(std::list<std::shared_ptr<EnemyBase>> enemies);

	/// <summary>
	/// 最も近い敵を取得する
	/// </summary>
	/// <param name="enemies">絞りたい敵のリスト</param>
	/// <returns>最も近い敵</returns>
	std::shared_ptr<EnemyBase> GetNearestEnemy(std::list<std::shared_ptr<EnemyBase>> enemies);

	/// <summary>
	/// ターゲットしている敵を除外した敵リストを取得する
	/// </summary>
	/// <param name="enemies">絞りたい敵のリスト</param>
	/// <returns>ターゲットしている敵を除外した敵リスト</returns>
	std::list<std::shared_ptr<EnemyBase>> RemoveTarget(std::list<std::shared_ptr<EnemyBase>> enemies);

private:
	std::shared_ptr<Player> m_pPlayer = nullptr;
	std::shared_ptr<Camera> m_pCamera = nullptr;
	std::shared_ptr<EnemyManager> m_pEnemyManager = nullptr;

	std::shared_ptr<EnemyBase> m_pTarget = nullptr;
	bool m_isTarget = false;

	std::shared_ptr<TargetUI> m_pTargetUI = nullptr;

	// TargetUIはTargetManagerの情報を必要とするため、TargetUIをフレンドにする
	friend TargetUI;
};

