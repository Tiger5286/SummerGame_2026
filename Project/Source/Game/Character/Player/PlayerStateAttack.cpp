#include "PlayerStateAttack.h"
#include "Player.h"
#include "Singleton/Input.h"
#include "Utility/Matrix4x4.h"

#include "PlayerStateIdle.h"
#include "PlayerStateDodge.h"

namespace
{
	// コンボ一つ一つの情報
	struct ComboData
	{
		std::wstring animName;	// アニメーション名
		float minTimeRate = -1;		// 次のコンボに行くまでの最低時間
		float minInputTimeRate = -1;	// 次のコンボに行く入力の最低受付時間
		float maxInputTimeRate = -1;	// 次のコンボに行く入力の最大受付時間
		float moveTimeRate = -1;		// 移動する時間
		float moveSpeed = -1;		// 移動する速度
		float startColTimeRate = -1;	// 当たり判定を開始する時間
		float endColTimeRate = -1;	// 当たり判定を終了する時間
	};
	// コンボ一つ一つの情報
	std::vector<ComboData> kComboDatas =
	{
		{
			L"Player|Combo1",
			0.28f,
			0.1f,
			0.9f,
			0.1f,
			10.0f,
			0.3f,
			0.7f
		},
		{
			L"Player|Combo2",
			0.3f,
			0.1f,
			0.9f,
			0.1f,
			10.0f,
			0.3f,
			0.7f
		},
		{
			L"Player|Combo3",
			0.0f,
			0.0f,
			0.0f,
			0.15f,
			10.0f,
			0.3f,
			0.7f
		}
	};
}

void PlayerStateAttack::Enter(std::shared_ptr<Player> pPlayer)
{
	m_pPlayer = pPlayer;
	m_pPlayer->m_anim.ChangeAnim(kComboDatas[0].animName, 0.5f, false);
	m_comboIndex = 0;
}

void PlayerStateAttack::Update()
{
	// 入力を取得
	auto& input = Input::GetInstance();
	// 回避を入力したら回避
	if (input.IsTriggerd(XINPUT_BUTTON_B))
	{
		ChangeState(std::make_shared<PlayerStateDodge>());
		return;
	}
	// アニメーションが終わったらIdleに移行
	if (m_pPlayer->m_anim.IsEnd())
	{
		ChangeState(std::make_shared<PlayerStateIdle>());
		return;
	}
	// 効果時間内に攻撃ボタンを押したら移行フラグを立てる
	float animRate = m_pPlayer->m_anim.GetAnimRate();
	bool isCanTransTime = animRate > kComboDatas[m_comboIndex].minInputTimeRate && animRate < kComboDatas[m_comboIndex].maxInputTimeRate;
	if (isCanTransTime && input.IsTriggerd(XINPUT_BUTTON_X))
	{
		m_isCanTransNextCombo = true;
	}
	// 移動処理
	if (animRate < kComboDatas[m_comboIndex].moveTimeRate)
	{
		Vector3 moveVec = Vector3(0, m_pPlayer->m_vel.y, -kComboDatas[m_comboIndex].moveSpeed);
		moveVec *= Matrix4x4::GetRotY(m_pPlayer->m_angle);
		m_pPlayer->m_vel = moveVec;
	}
	// 移行フラグが立っている、かつ次の攻撃までの最低時間を越していたら次の攻撃へ
	if (m_isCanTransNextCombo && animRate > kComboDatas[m_comboIndex].minTimeRate)
	{
		m_pPlayer->RotateInputDir();
		m_pPlayer->m_anim.ChangeAnim(kComboDatas[m_comboIndex + 1].animName, 0.5f, false);
		m_comboIndex++;
		m_isCanTransNextCombo = false;
	}
}

void PlayerStateAttack::Exit()
{}