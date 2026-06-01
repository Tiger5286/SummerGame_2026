#include "CollisionManager.h"
#include "Game/Character/Character.h"
#include "ColliderBase.h"
#include "SphereCollider.h"
#include "CapsuleCollider.h"

void CollisionManager::Update()
{
	// 二つのオブジェクトの当たり判定を全部回す
	for (auto& obj1 : m_objects)
	{
		for (auto& obj2 : m_objects)
		{
			// 同じオブジェクトなら当たり判定しない
			if (obj1 == obj2) continue;
			// 当たり判定をチェックし、当たっていたらお互いを渡す
			if (CheckCollision(obj1->GetCollider(), obj2->GetCollider()))
			{
				obj1->OnCollision(*obj2);
				obj2->OnCollision(*obj1);
			}
		}
	}
}

void CollisionManager::Register(std::shared_ptr<Character> object)
{
	m_objects.push_back(object);
}

void CollisionManager::Unregister(std::shared_ptr<Character> object)
{
	m_objects.remove(object);
}

bool CollisionManager::CheckCollision(std::shared_ptr<ColliderBase> col1, std::shared_ptr<ColliderBase> col2)
{
	bool result = false;
	// オブジェクトの当たり判定タイプに応じて別の処理を実行する
	if (col1->GetType() == ColliderType::Sphere && col2->GetType() == ColliderType::Sphere)
	{	// 球と球
		std::shared_ptr<SphereCollider> sphere1 = std::dynamic_pointer_cast<SphereCollider>(col1);
		std::shared_ptr<SphereCollider> sphere2 = std::dynamic_pointer_cast<SphereCollider>(col2);
		result = CheckHitSS(sphere1, sphere2);
	}
	else if (col1->GetType() == ColliderType::Sphere && col2->GetType() == ColliderType::Capsule)
	{	// 球とカプセル
		std::shared_ptr<SphereCollider> sphere = std::dynamic_pointer_cast<SphereCollider>(col1);
		std::shared_ptr<CapsuleCollider> capsule = std::dynamic_pointer_cast<CapsuleCollider>(col2);
		result = CheckHitSC(sphere, capsule);
	}
	else if (col1->GetType() == ColliderType::Capsule && col2->GetType() == ColliderType::Sphere)
	{	// カプセルと球
		std::shared_ptr<SphereCollider> sphere = std::dynamic_pointer_cast<SphereCollider>(col2);
		std::shared_ptr<CapsuleCollider> capsule = std::dynamic_pointer_cast<CapsuleCollider>(col1);
		result = CheckHitSC(sphere, capsule);
	}
	else if (col1->GetType() == ColliderType::Capsule && col2->GetType() == ColliderType::Capsule)
	{	// カプセルとカプセル
		std::shared_ptr<CapsuleCollider> capsule1 = std::dynamic_pointer_cast<CapsuleCollider>(col1);
		std::shared_ptr<CapsuleCollider> capsule2 = std::dynamic_pointer_cast<CapsuleCollider>(col2);
		result = CheckHitCC(capsule1, capsule2);
	}
	return result;
}

bool CollisionManager::CheckHitSS(std::shared_ptr<SphereCollider> sphere1, std::shared_ptr<SphereCollider> sphere2)
{
	bool result = false;
	// 球同士の距離の2乗を計算
	float squareDist = (sphere1->GetPos() - sphere2->GetPos()).SquaredLength();
	// 球同士が当たる距離を計算
	float hitDist = sphere1->GetRadius() + sphere2->GetRadius();
	// 計算した距離で当たっているかどうか判定
	if (squareDist < hitDist * hitDist)
	{
		result = true;
	}
	return result;
}

bool CollisionManager::CheckHitSC(std::shared_ptr<SphereCollider> sphere, std::shared_ptr<CapsuleCollider> capsule)
{
	bool result = false;

	// 球とカプセルの最も短い距離を計算
	float minSquareDist = Segment_Point_MinLength_Square(capsule->GetPos().ToDxLib(),
														 (capsule->GetPos() + Vector3::Up() * capsule->GetHeight()).ToDxLib(),
														 sphere->GetPos().ToDxLib());
	// 球とカプセルが当たる距離を計算
	float hitDist = sphere->GetRadius() + capsule->GetRadius();
	// 計算した距離で当たっているかどうか判定
	if (minSquareDist < hitDist * hitDist)
	{
		result = true;
	}
	return result;
}

bool CollisionManager::CheckHitCC(std::shared_ptr<CapsuleCollider> capsule1, std::shared_ptr<CapsuleCollider> capsule2)
{
	bool result = false;
	// カプセル同士の最も短い距離を計算
	float minDistSquare = Segment_Segment_MinLength_Square(capsule1->GetPos().ToDxLib(),
														   (capsule1->GetPos() + Vector3::Up() * capsule1->GetHeight()).ToDxLib(),
														   capsule2->GetPos().ToDxLib(),
														   (capsule2->GetPos() + Vector3::Up() * capsule2->GetHeight()).ToDxLib());
	// カプセル同士が当たる距離を計算
	float hitDist = capsule1->GetRadius() + capsule2->GetRadius();
	// 計算した距離で当たっているかどうか判定
	if (minDistSquare < hitDist * hitDist)
	{
		result = true;
	}
	return result;
}
