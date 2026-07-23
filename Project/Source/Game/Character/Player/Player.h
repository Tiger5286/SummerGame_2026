#pragma once
#include "Game/Character/Character.h"
#include "System/Animator.h"
#include <memory>

class Camera;
class PlayerHPUI;
class PlayerSpecialUI;
class PlayerSkillUI;
class ControlUI;

class Player : public Character
{
public:
    Player() = default;
    virtual ~Player() = default;

    void Init() override;
    void End() override;
    void OnUpdate() override;
    void Draw() override;
    

    // カメラのポインタを設定する
    void SetCamera(std::weak_ptr<Camera> pCamera) { m_pCamera = pCamera; }

	// マップモデルのハンドルを設定する(Initの前に実行する)
	void SetMapHandle(int mapHandle) { m_mapHandle = mapHandle; }

    void SetTarget(std::shared_ptr<Character> target) { m_target = target; }

    // 角度を取得する
    float GetAngle() const { return m_angle; }
    // 自身の向きを表すベクトルを取得する
    Vector3 GetDir() const;

    int GetSpecialCharge() const { return m_specialCharge; }
    int GetSkillCooltime() const { return m_skillCooltime; }

    bool IsDead() const { return m_isDead; }

    void OnHitAttack(const MyLib::AttackData& atkData) override;

    void SetCanControl(bool canControl) { m_isCanControl = canControl; }

    // 最大HP
    const int kMaxHp = 1000;
    int GetMaxHP() const override { return kMaxHp; }
    // 最大必殺技チャージ
    const int kMaxSpecialCharge = 1000;
    // スキルクールタイム
    const int kSkillCooltime = 10 * 60;

private:

    void Move();    // 移動処理
    void Jump();    // ジャンプ処理

    void RotateInputDir();  // 入力方向を向く

    void RotateToTarget(float activeDist);  // ターゲットの方向を向く

    // 接地判定
    void CheckGround();

private:
	Animator m_anim;    // アニメーションクラス
	int m_mapHandle = -1;	// マップのモデルのハンドル

    std::shared_ptr<Character> m_target = nullptr;   // ロックオンしているターゲット

    std::weak_ptr<Camera> m_pCamera;

    std::shared_ptr<PlayerHPUI> m_pHPUI = nullptr;
    std::shared_ptr<PlayerSpecialUI> m_pSpecialUI = nullptr;
    std::shared_ptr<PlayerSkillUI> m_pSkillUI = nullptr;
    std::shared_ptr<ControlUI> m_pControlUI = nullptr;

    // 見えなくなるかどうか
    bool m_isInvisible = false;

    bool m_isDead = false;

    // 空中でテレポートが発動可能かどうか
    bool m_isCanAirShift = true;
    // 空中で通常攻撃が発動可能か
    bool m_isCanAirAttack = true;
    // 操作可能かどうか
    bool m_isCanControl = true;

    // 必殺技ゲージ
    int m_specialCharge = 0;
    // スキルクールタイム
    int m_skillCooltime = kSkillCooltime;

    // ボタン配置
    const int kJump = XINPUT_BUTTON_A;
    const int kAttack = XINPUT_BUTTON_X;
    const int kDodge = XINPUT_BUTTON_RIGHT_SHOULDER;
    const int kShift = XINPUT_BUTTON_B;
    const int kBurning = XINPUT_BUTTON_LEFT_SHOULDER;
    const int kSpin = XINPUT_BUTTON_Y;

    // プレイヤーのステートクラスがプレイヤーにアクセスできるようにする
    friend class PlayerStateBase;
    friend class PlayerStateIdle;
    friend class PlayerStateMove;
    friend class PlayerStateFall;
    friend class PlayerStateDodge;
    friend class PlayerStateAttack;
    friend class PlayerStateHit;
    friend class PlayerStateShift;
    friend class PlayerStateShiftAttack;
    friend class PlayerStateBurning;
    friend class PlayerStateSpin;
    friend class PlayerStateDeath;
};