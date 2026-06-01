#pragma once
#include <memory>
#include <list>

class Character;
class ColliderBase;
class SphereCollider;
class CapsuleCollider;

class CollisionManager
{
public:
	CollisionManager();
	virtual ~CollisionManager();

	/// <summary>
	/// 当たり判定を更新する
	/// </summary>
	void Update();

	/// <summary>
	/// 当たり判定を持つオブジェクトを登録する
	/// </summary>
	/// <param name="object">当たり判定を持つゲームオブジェクト</param>
	void Register(std::shared_ptr<Character> object);

	/// <summary>
	/// 当たり判定を持つオブジェクトの登録を解除する
	/// </summary>
	/// <param name="object">当たり判定を持つゲームオブジェクト</param>
	void Unregister(std::shared_ptr<Character> object);

private:
	/// <summary>
	/// 当たり判定を行う
	/// </summary>
	/// <param name="col1">col2と当たり判定を行うオブジェクト</param>
	/// <param name="col2">col1と当たり判定を行うオブジェクト</param>
	/// <returns>true:当たった / false:当たっていない</returns>
	bool CheckCollision(std::shared_ptr<ColliderBase> col1, std::shared_ptr<ColliderBase> col2);

	/// <summary>
	/// 球同士の当たり判定
	/// </summary>
	/// <param name="sphere1">当たり判定を行う球</param>
	/// <param name="sphere2">当たり判定を行う球</param>
	/// <returns>true:当たった / false:当たっていない</returns>
	bool CheckHitSS(std::shared_ptr<SphereCollider> sphere1, std::shared_ptr<SphereCollider> sphere2);

	/// <summary>
	/// 球とカプセルの当たり判定
	/// </summary>
	/// <param name="sphere">当たり判定を行う球</param>
	/// <param name="capsule">当たり判定を行うカプセル</param>
	/// <returns>true:当たった / false:当たっていない</returns>
	bool CheckHitSC(std::shared_ptr<SphereCollider> sphere, std::shared_ptr<CapsuleCollider> capsule);

	/// <summary>
	/// カプセル同士の当たり判定
	/// </summary>
	/// <param name="capsule1">当たり判定を行うカプセル</param>
	/// <param name="capsule2">当たり判定を行うカプセル</param>
	/// <returns>true:当たった / false:当たっていない</returns>
	bool CheckHitCC(std::shared_ptr<CapsuleCollider> capsule1, std::shared_ptr<CapsuleCollider> capsule2);

private:
	// 当たり判定を行うオブジェクトのリスト
	std::list<std::shared_ptr<Character>> m_objects;
};

