#include "VultureStateIdle.h"
#include "Vulture.h"
#include "Utility/MyLib.h"
#include "../../Player/Player.h"

#include "VultureStateAttack.h"

namespace
{
	// アニメーション名
	const std::wstring kIdleAnimName = L"VultureCinereous_Skelmesh|VultureCinereous_Flying";
	// 移動速度
	constexpr float kMoveSpeed = 5.0f;
	// 回転速度
	constexpr float kRotateSpeed = 0.03f;

	// 攻撃クールタイム
	constexpr int kAttackCooldown = 240;
	// プレイヤーを検知する距離
	constexpr float kFindPlayerDist = 700.0f;
}

void VultureStateIdle::Enter(std::weak_ptr<Vulture> pVulture)
{
	m_pVulture = pVulture;
	auto vulture = m_pVulture.lock();
	vulture->m_anim.ChangeAnim(kIdleAnimName);
	vulture->m_isFlying = true;
}

void VultureStateIdle::Update()
{
	m_frame++;

	auto vulture = m_pVulture.lock();
	
	// 旋回
	vulture->m_angle += kRotateSpeed;
	Vector3 moveVec = MyLib::GetVecAngle(vulture->m_angle);
	moveVec *= kMoveSpeed;
	vulture->m_vel = moveVec;

	// プレイヤー検知→攻撃に遷移
	Vector3 playerPos = vulture->m_pPlayer->GetPos();
	playerPos.y = 0.0f;	// y要素は無視する
	Vector3 myPosXZ = vulture->m_pos;
	myPosXZ.y = 0.0f;
	float toPlayerSquareDist = (playerPos - myPosXZ).SquaredLength();
	// プレイヤーとの距離が攻撃の範囲内
	bool isFindPlayer = toPlayerSquareDist < kFindPlayerDist * kFindPlayerDist;
	// クールタイムが明けている
	bool isEndCooldown = m_frame > kAttackCooldown;
	// 両方の条件を満たしていたら攻撃に遷移
	if (isFindPlayer && isEndCooldown)
	{
		ChangeState(std::make_shared<VultureStateAttack>());
		return;
	}
}

void VultureStateIdle::Exit()
{

}