#include "Character.h"
#include "../Collider/CapsuleCollider.h"
#include "../Collider/SphereCollider.h"
#include "Singleton/IDManager.h"

namespace
{
	constexpr float kResistancePower = 1.0f;
}

Character::Character() :
	m_id(IDManager::GetInstance().GetNewId())
{
}

Character::~Character()
{
}

void Character::Resistance()
{
	// 速度と逆向きのベクトルを生成
	auto resistVec = -m_vel;
	resistVec.y = 0.0f;
	// 長さを固定
	resistVec.Normalize();
	resistVec *= kResistancePower;
	// 速度に足す
	m_vel += resistVec;
	// 速度が一定以下になったら止まる
	auto velXZ = Vector3(m_vel.x, 0.0f, m_vel.z);
	if (velXZ.SquaredLength() < kResistancePower)
	{
		m_vel.x = 0.0f;
		m_vel.z = 0.0f;
	}
}

void Character::Gravity(float power)
{
	m_vel.y -= power;
}

void Character::OnCollision(Character& other)
{}

void Character::CheckHitMapCapsule(MV1_COLL_RESULT_POLY_DIM coll)
{
	auto capsule = std::dynamic_pointer_cast<CapsuleCollider>(m_pCollider);
	for (int i = 0; i < coll.HitNum; i++)
	{
		// 当たったポリゴンの法線を取得
		auto normal = Vector3::FromDxLib(coll.Dim[i].Normal);

		// 法線のy要素で床壁判定をする
		bool isFloor = normal.y > 0.2f;	// true:床 / false:壁
		// 床判定なら法線を真上向きにする
		if (isFloor) normal = Vector3::Up();

		// カプセルの位置を取得
		Vector3 capsuleStart = capsule->GetPos();
		Vector3 capsuleEnd = capsuleStart + Vector3::Up() * capsule->GetHeight();

		// ポリゴンとカプセルの線との最短距離を計算
		auto minDist = Segment_Triangle_MinLength(capsuleStart.ToDxLib(), capsuleEnd.ToDxLib(),
			coll.Dim[i].Position[0], coll.Dim[i].Position[1], coll.Dim[i].Position[2]);
		// 押し戻し量を計算
		auto pushDist = capsule->GetRadius() - minDist;

		// 法線方向に押し戻す
		m_pos += normal * pushDist;
		if (isFloor)
		{
			m_vel.y = 0.0f;
			m_isGround = true;
		}
	}
}

void Character::CheckHitMapSphere(MV1_COLL_RESULT_POLY_DIM coll)
{
	auto sphere = std::dynamic_pointer_cast<SphereCollider>(m_pCollider);
	for (int i = 0; i < coll.HitNum; i++)
	{
		// 当たったポリゴンの法線を取得
		auto normal = Vector3::FromDxLib(coll.Dim[i].Normal);

		// 法線のy要素で床壁判定をする
		bool isFloor = normal.y > 0.2f;	// true:床 / false:壁
		// 床判定なら法線を真上向きにする
		if (isFloor) normal = Vector3::Up();

		// 球の中心とポリゴンの最短距離を取得
		auto minDist = Triangle_Point_MinLength(coll.Dim[i].Position[0], coll.Dim[i].Position[1], coll.Dim[i].Position[2],
			sphere->GetPos().ToDxLib());
		// 押し戻し量を計算
		auto pushDist = sphere->GetRadius() - minDist;

		// 法線方向に押し戻す
		m_pos += normal * pushDist;
		if (isFloor)
		{
			m_vel.y = 0.0f;
			m_isGround = true;
		}
	}
}
