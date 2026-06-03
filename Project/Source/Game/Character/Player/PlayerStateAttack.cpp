#include "PlayerStateAttack.h"
#include "Player.h"
#include "Singleton/Input.h"
#include "Utility/Matrix4x4.h"
#include <vector>
#include "Game/Character/Attack.h"
#include "Singleton/CollisionManager.h"

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
	const std::vector<ComboData> kComboDatas =
	{
		{
			L"Player|Combo1",
			0.28f,
			0.1f,
			0.9f,
			0.1f,
			10.0f,
			0.1f,
			0.28f
		},
		{
			L"Player|Combo2",
			0.3f,
			0.1f,
			0.9f,
			0.1f,
			10.0f,
			0.16f,
			0.3f
		},
		{
			L"Player|Combo3",
			0.0f,
			0.0f,
			0.0f,
			0.15f,
			10.0f,
			0.25f,
			0.44f
		}
	};

	// 攻撃がホーミングする距離
	constexpr float kTrackingAttackDist = 500.0f;
	// 攻撃の前進をやめる距離
	constexpr float kStopTrackingDist = 120.0f;

	// 攻撃の位置オフセット
	const Vector3 kAttackColliderOffset = Vector3(0, 100, -100);
	// 攻撃のデータ
	const Attack::Data kAttackData = {
	100.0f,
	100,
	Character::Type::Enemy
	};
}

void PlayerStateAttack::Enter(std::weak_ptr<Player> pPlayer)
{
	m_pPlayer = pPlayer;
	m_pPlayer.lock()->m_anim.ChangeAnim(kComboDatas[0].animName, 0.5f, false);
	m_comboIndex = 0;
	Tracking();
}

void PlayerStateAttack::Update()
{
	// 入力を取得
	auto& input = Input::GetInstance();
	// プレイヤーのshared_ptrを取得
	auto player = m_pPlayer.lock();
	// 回避を入力したら回避
	if (input.IsTriggerd(XINPUT_BUTTON_B))
	{
		ChangeState(std::make_shared<PlayerStateDodge>());
		return;
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
	if (isCanTransTime && input.IsTriggerd(XINPUT_BUTTON_X))	// 入力受付時間内かつ入力があったら
	{	// 移行フラグを立てる
		m_isCanTransNextCombo = true;
	}

	// 移動処理
	if (animRate < kComboDatas[m_comboIndex].moveTimeRate)
	{
		// 移動ベクトルを生成
		Vector3 moveVec = Vector3(0, m_pPlayer.lock()->m_vel.y, -kComboDatas[m_comboIndex].moveSpeed);

		// ホーミングするターゲットへのベクトルを生成
		Vector3 toTargetVec = m_pPlayer.lock()->m_target->GetPos() - m_pPlayer.lock()->m_pos;
		// 一定の距離より近かったら進まない
		if (toTargetVec.SquaredLength() < kStopTrackingDist * kStopTrackingDist)
		{
			moveVec = Vector3::Zero();
		}
		// 移動ベクトルをプレイヤーの向きに回転
		moveVec *= Matrix4x4::GetRotY(m_pPlayer.lock()->m_angle);
		// 速度に適用
		m_pPlayer.lock()->m_vel = moveVec;
	}

	// 当たり判定処理
	// 当たり判定開始	当たり判定開始の時間、かつまだ当たり判定をonにしていないなら
	if (animRate > kComboDatas[m_comboIndex].startColTimeRate && !m_isOnCollider)
	{
		// 当たり判定を生成し、当たり判定をonにする
		m_pAtk = std::make_shared<Attack>();
		m_pAtk->SetData(kAttackData);
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

	// 移行フラグが立っている、かつ次の攻撃までの最低時間を越していたら次の攻撃へ
	if (m_isCanTransNextCombo && animRate > kComboDatas[m_comboIndex].minTimeRate)
	{
		m_pPlayer.lock()->RotateInputDir();
		Tracking();
		m_pPlayer.lock()->m_anim.ChangeAnim(kComboDatas[m_comboIndex + 1].animName, 0.5f, false);
		m_comboIndex++;
		m_isCanTransNextCombo = false;
		m_isOnCollider = false;
		m_isOffCollider = false;
	}
}

void PlayerStateAttack::Exit()
{}

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

void PlayerStateAttack::Tracking()
{
	// ホーミングするターゲットへのベクトルを生成
	Vector3 toTargetVec = m_pPlayer.lock()->m_target->GetPos() - m_pPlayer.lock()->m_pos;
	// 条件を満たしたらプレイヤーの向きをターゲットのほうへ向ける
	bool isTrackingDist = toTargetVec.SquaredLength() < kTrackingAttackDist * kTrackingAttackDist;	// ホーミングする距離内
	if (isTrackingDist)	// 距離内、かつ一定の距離外
	{
		m_pPlayer.lock()->m_angle = toTargetVec.Angle();
	}
}
