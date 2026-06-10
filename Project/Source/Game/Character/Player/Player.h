#pragma once
#include "Game/Character/Character.h"
#include "System/Animator.h"
#include <memory>

class PlayerAttackCollider;
class Camera;

// プレイヤーのステートクラスをプロトタイプ宣言しておく
class PlayerStateBase;
class PlayerStateIdle;
class PlayerStateMove;
class PlayerStateFall;
class PlayerStateDodge;
class PlayerStateAttack;
class PlayerStateHit;
class PlayerStateShift;
class PlayerStateBurning;
class PlayerStateSpin;

class Player : public Character, public std::enable_shared_from_this<Player>
{
public:
    Player() = default;
    virtual ~Player() = default;

    void Init() override;
    void End() override;
    void Update() override;
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

    void OnHitAttack(int damage) override;

private:

    void Move();    // 移動処理
    void Jump();    // ジャンプ処理

    void RotateInputDir();  // 入力方向を向く

    void RotateToTarget(float activeDist);  // ターゲットの方向を向く

    // 接地判定
    void CheckGround();

    /// <summary>
    /// ステートの切り替え処理
    /// </summary>
    void CheckChangeState();

private:
	Animator m_anim;    // アニメーションクラス
	int m_mapHandle = -1;	// マップのモデルのハンドル

    std::shared_ptr<Character> m_target;   // ロックオンしているターゲット

    std::weak_ptr<Camera> m_pCamera;

    // プレイヤーのステート
    std::shared_ptr<PlayerStateBase> m_pState = nullptr;

    // 見えなくなるかどうか
    bool m_isInvisible = false;

	// 自身のモデルの回転角度
    float m_angle = 0.0f;
	// 描画用の回転角度(Lerpで滑らかに回転する)
	float m_drawAngle = 0.0f;

    // ボタン配置
    const int kJump = XINPUT_BUTTON_A;
    const int kAttack = XINPUT_BUTTON_X;
    const int kDodge = XINPUT_BUTTON_RIGHT_SHOULDER;
    const int kShift = XINPUT_BUTTON_B;
    const int kBurning = XINPUT_BUTTON_LEFT_SHOULDER;
    const int kSpin = XINPUT_BUTTON_Y;

    // プレイヤーのステートクラスがプレイヤーにアクセスできるようにする
    friend PlayerStateBase;
    friend PlayerStateIdle;
    friend PlayerStateMove;
    friend PlayerStateFall;
    friend PlayerStateDodge;
    friend PlayerStateAttack;
    friend PlayerStateHit;
    friend PlayerStateShift;
    friend PlayerStateBurning;
    friend PlayerStateSpin;
};