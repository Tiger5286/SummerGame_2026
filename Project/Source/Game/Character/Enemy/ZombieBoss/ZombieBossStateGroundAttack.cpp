#include "ZombieBossStateGroundAttack.h"
#include "../../Attack.h"

#include "ZombieBossStateIdle.h"

namespace
{
	constexpr const wchar_t* kAnimName = L"ZombieBoss|GroundAttack";

	constexpr int kStartAttackFrame = 90 * 2;
	constexpr int kEndAttackFrame = 100 * 2;

	MyLib::AttackData kAttackData = {
		500.0f,
		100,
		MyLib::CharacterType::Player
	};
}

void ZombieBossStateGroundAttack::OnEnter()
{
	m_pZombieBoss = std::dynamic_pointer_cast<ZombieBoss>(m_pOwner.lock());
	auto zombieBoss = m_pZombieBoss.lock();
	zombieBoss->m_anim.ChangeAnim(kAnimName, MyLib::kDefaultAnimSpeed, false);
}

void ZombieBossStateGroundAttack::Update()
{
	m_frame++;
	auto zombieBoss = m_pZombieBoss.lock();
	// 当たり判定開始
	if (m_frame == kStartAttackFrame)
	{
		m_pAtk = std::make_shared<Attack>();
		m_pAtk->SetData(kAttackData, shared_from_this());
		m_pAtk->Init();
	}
	// 当たり判定終了
	if (m_frame == kEndAttackFrame)
	{
		m_pAtk = nullptr;
	}
	// 当たり判定の位置設定
	if (m_pAtk != nullptr)
	{
		Vector3 colPos = m_pZombieBoss.lock()->GetPos();
		m_pAtk->SetPos(colPos);
		m_pAtk->Update();
	}
	// 攻撃アニメーションが終わったら待機ステートに遷移する
	if (zombieBoss->m_anim.IsEnd())
	{
		ChangeState(std::make_shared<ZombieBossStateIdle>());
	}
}

void ZombieBossStateGroundAttack::Exit()
{
}

void ZombieBossStateGroundAttack::Draw()
{
#ifdef _DEBUG
	if (m_pAtk != nullptr)
	{
		m_pAtk->Draw();
	}
#endif
}
