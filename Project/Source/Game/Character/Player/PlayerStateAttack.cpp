#include "PlayerStateAttack.h"
#include "Player.h"
#include "Singleton/Input.h"
#include "Utility/Matrix4x4.h"
#include <vector>
#include "Game/Character/Attack.h"
#include "Singleton/CollisionManager.h"
#include "Utility/MyLib.h"

#include "PlayerStateIdle.h"
#include "PlayerStateMove.h"
#include "PlayerStateDodge.h"
#include "PlayerStateSpin.h"
#include "PlayerStateShift.h"
#include "PlayerStateBurning.h"

namespace
{
	// コンボ最終段のインデックス
	constexpr int kLastComboIndex = 2;
	// コンボの最終段をキャンセルできる時間
	constexpr float kCanComboEndAnimRate = 0.6f;

	// コンボ一つ一つの情報
	struct ComboData
	{
		std::wstring animName;	// アニメーション名
		float minTimeRate		= -1;		// 次のコンボに行くまでの最低時間(攻撃をキャンセルできるようになる時間)	// TimeRateは0.0がアニメーション開始、1.0がアニメーション終了とした割合で表す
		float minInputTimeRate	= -1;	// 次のコンボに行く入力の最低受付時間
		float maxInputTimeRate	= -1;	// 次のコンボに行く入力の最大受付時間
		float moveTimeRate		= -1;		// 移動する時間
		float moveSpeed			= -1;		// 移動する速度
		float startColTimeRate	= -1;	// 当たり判定を開始する時間
		float endColTimeRate	= -1;	// 当たり判定を終了する時間
	};
	// コンボ一つ一つの情報
	const std::vector<ComboData> kComboDatas =
	{
		{
			.animName			= L"Player|Combo1",
			.minTimeRate		= 0.28f,
			.minInputTimeRate	= 0.1f,
			.maxInputTimeRate	= 0.9f,
			.moveTimeRate		= 0.1f,
			.moveSpeed			= 10.0f,
			.startColTimeRate	= 0.1f,
			.endColTimeRate		= 0.28f
		},
		{
			.animName			= L"Player|Combo2",
			.minTimeRate		= 0.3f,
			.minInputTimeRate	= 0.1f,
			.maxInputTimeRate	= 0.9f,
			.moveTimeRate		= 0.1f,
			.moveSpeed			= 10.0f,
			.startColTimeRate	= 0.16f,
			.endColTimeRate		= 0.3f
		},
		{
			.animName			= L"Player|Combo3",
			.minTimeRate		= 0.44f,
			.minInputTimeRate	= 0.0f,
			.maxInputTimeRate	= 0.0f,
			.moveTimeRate		= 0.15f,
			.moveSpeed			= 10.0f,
			.startColTimeRate	= 0.25f,
			.endColTimeRate		= 0.44f
		}
	};

	// 攻撃がホーミングする距離
	constexpr float kTrackingAttackDist = 500.0f;
	// 攻撃の前進をやめる距離
	constexpr float kStopTrackingDist = 120.0f;

	// 攻撃の位置オフセット
	const Vector3 kAttackColliderOffset = Vector3(0, 100, -100);
	// 攻撃のデータ
	const std::vector<MyLib::AttackData> kAttackData = {
		{
			.colliderRadius = 100.0f,
			.damage = 40,
			.hitCharacterType = MyLib::CharacterType::Enemy,
			.isKnockDown = false,
			.isIgnoreInvincible = false,
			.specialCharge = 20,
		},
		{
			.colliderRadius = 100.0f,
			.damage = 60,
			.hitCharacterType = MyLib::CharacterType::Enemy,
			.isKnockDown = false,
			.isIgnoreInvincible = false,
			.specialCharge = 30,
		},
		{
			.colliderRadius = 100.0f,
			.damage = 100,
			.hitCharacterType = MyLib::CharacterType::Enemy,
			.isKnockDown = true,
			.isIgnoreInvincible = false,
			.specialCharge = 50,
		},
	};
}

void PlayerStateAttack::OnEnter()
{
	m_pPlayer = std::dynamic_pointer_cast<Player>(m_pOwner.lock());
	auto player = m_pPlayer.lock();
	player->m_anim.ChangeAnim(kComboDatas[0].animName, MyLib::kDefaultAnimSpeed, false);
	player->RotateToTarget(kTrackingAttackDist);
	// 空中で発動したなら、空中で発動可能フラグを切る
	if (!player->m_isGround)
	{
		player->m_isCanAirAttack = false;
		player->m_anim.SetFloatAnimLowerBody(true);
	}
	m_comboIndex = 0;
}

void PlayerStateAttack::Update()
{
	// 入力を取得
	auto& input = Input::GetInstance();
	// プレイヤーのshared_ptrを取得
	auto player = m_pPlayer.lock();
	// 回避を入力したら回避
	if (input.IsTriggerd(player->kDodge))
	{
		ChangeState(std::make_shared<PlayerStateDodge>());
		return;
	}
	// スピンを入力したらスピンに移行フラグを立てる
	if (input.IsTriggerd(player->kSpin) && player->GetSkillCooltime() >= player->kSkillCooltime)
	{
		m_isActiveSpin = true;
	}
	// シフトを入力したらシフトに移行フラグを立てる
	if (input.IsTriggerd(player->kShift))
	{
		m_isActiveShift = true;
	}
	// 必殺技を入力したら必殺技に移行フラグを立てる
	if (input.IsTriggerd(player->kBurning) && player->GetSpecialCharge() >= player->kMaxSpecialCharge)
	{
		m_isActiveBurning = true;
	}

	// アニメーションが終わったらIdleに移行
	if (m_pPlayer.lock()->m_anim.IsEnd())
	{
		ChangeState(std::make_shared<PlayerStateIdle>());
		return;
	}
	// 効果時間内に攻撃ボタンを押したら移行フラグを立てる
	float animRate = m_pPlayer.lock()->m_anim.GetAnimRate();	// 現在のアニメーションの再生時間の割合を取得
	bool isCanTransTime = animRate > kComboDatas[m_comboIndex].minInputTimeRate && animRate < kComboDatas[m_comboIndex].maxInputTimeRate;	// 入力受付時間内だったらtrue
	if (isCanTransTime && input.IsTriggerd(player->kAttack))	// 入力受付時間内かつ入力があったら
	{	// 移行フラグを立てる
		m_isCanTransNextCombo = true;
	}

	// 移動処理
	if (animRate < kComboDatas[m_comboIndex].moveTimeRate)
	{
		// 移動ベクトルを生成
		Vector3 moveVec = Vector3(0, m_pPlayer.lock()->m_vel.y, -kComboDatas[m_comboIndex].moveSpeed);

		// ターゲットがいる場合のみ
		if (player->m_target != nullptr)
		{
			// ホーミングするターゲットへのベクトルを生成
			Vector3 toTargetVec = m_pPlayer.lock()->m_target->GetPos() - m_pPlayer.lock()->m_pos;
			// 一定の距離より近かったら進まない
			if (toTargetVec.SquaredLength() < kStopTrackingDist * kStopTrackingDist)
			{
				moveVec = Vector3::Zero();
			}
		}
		
		// 移動ベクトルをプレイヤーの向きに回転
		moveVec *= Matrix4x4::GetRotY(m_pPlayer.lock()->m_angle);
		// 速度に適用
		m_pPlayer.lock()->m_vel = moveVec;
	}
	// 落下しないようにする
	player->m_vel.y = 0.0f;

	// 当たり判定処理
	// 当たり判定開始	当たり判定開始の時間、かつまだ当たり判定をonにしていないなら
	if (animRate > kComboDatas[m_comboIndex].startColTimeRate && !m_isOnCollider)
	{
		// 当たり判定を生成し、当たり判定をonにする
		m_pAtk = std::make_shared<Attack>();
		m_pAtk->SetData(kAttackData[m_comboIndex], shared_from_this());
		m_pAtk->Init();
		m_isOnCollider = true;
	}
	// 当たり判定終了	当たり判定終了の時間、かつまだ当たり判定をoffにしていないなら
	if (animRate > kComboDatas[m_comboIndex].endColTimeRate && !m_isOffCollider)
	{
		// 当たり判定を生成し、当たり判定をoffにする
		m_pAtk = nullptr;
		m_isOffCollider = true;
	}
	// 当たり判定の移動
	if (m_pAtk != nullptr)
	{
		Vector3 colPos = player->m_pos + (kAttackColliderOffset * Matrix4x4::GetRotY(player->m_angle));
		m_pAtk->SetPos(colPos);
		m_pAtk->Update();
	}

	// スピンが入力されていた、かつ次の攻撃までの最低時間を越していたらスピンへ
	if (m_isActiveSpin && animRate > kComboDatas[m_comboIndex].minTimeRate)
	{
		ChangeState(std::make_shared<PlayerStateSpin>());
		return;
	}
	// シフトが入力されていた、かつ次の攻撃までの最低時間を越していたらシフトへ
	if (m_isActiveShift && animRate > kComboDatas[m_comboIndex].minTimeRate)
	{
		ChangeState(std::make_shared<PlayerStateShift>());
		return;
	}
	// 必殺技が入力されていた、かつ次の攻撃までの最低時間を越していたら必殺技へ
	if (m_isActiveBurning && animRate > kComboDatas[m_comboIndex].minTimeRate)
	{
		ChangeState(std::make_shared<PlayerStateBurning>());
		return;
	}

	// 移行フラグが立っている、かつ次の攻撃までの最低時間を越していたら次の攻撃へ
	if (m_isCanTransNextCombo && animRate > kComboDatas[m_comboIndex].minTimeRate)
	{
		m_pPlayer.lock()->RotateInputDir();
		m_pPlayer.lock()->RotateToTarget(kTrackingAttackDist);
		m_pPlayer.lock()->m_anim.ChangeAnim(kComboDatas[m_comboIndex + 1].animName, MyLib::kDefaultAnimSpeed, false);
		m_comboIndex++;
		m_isCanTransNextCombo = false;
		m_isOnCollider = false;
		m_isOffCollider = false;
	}

	// コンボ三段目が終わりかけの時
	if (m_comboIndex == kLastComboIndex && animRate > kCanComboEndAnimRate)
	{
		// 攻撃を入力していたらコンボを最初から
		if (input.IsPressed(player->kAttack))
		{
			// 地上にいるなら普通に遷移
			if (player->m_isGround)
			{
				ChangeState(std::make_shared<PlayerStateAttack>());
				return;
			}
			else if (player->m_isCanAirAttack)	// 空中にいる、かつ空中で攻撃が発動可能なら遷移
			{
				ChangeState(std::make_shared<PlayerStateAttack>());
				return;
			}
			// どちらの条件も満たさないなら攻撃に移らない
		}
		// 移動を入力していたら移動ステートへ
		if (input.GetStickInput(MyLib::LR::Left).SquaredLength() > 0.0f)
		{
			ChangeState(std::make_shared<PlayerStateMove>());
			return;
		}
	}
}

void PlayerStateAttack::Exit()
{
	auto player = m_pPlayer.lock();
	player->m_anim.SetFloatAnimLowerBody(false);
}

void PlayerStateAttack::Draw()
{
#ifdef _DEBUG
	// 当たり判定のデバッグ表示
	if (m_pAtk != nullptr)
	{
		m_pAtk->Draw();
	}
#endif
}