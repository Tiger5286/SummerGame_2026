#pragma once
#include "Game/Character/Character.h"
#include "System/Animator.h"
#include <memory>

// プレイヤーのステートクラスをプロトタイプ宣言しておく
class PlayerStateBase;
class PlayerStateIdle;
class PlayerStateMove;
class PlayerStateFall;
class PlayerStateDodge;
class PlayerStateAttack;

class Player : public Character, public std::enable_shared_from_this<Player>
{
public:
    Player() = default;
    virtual ~Player() = default;

    void Init() override;
    void End() override;
    void Update() override;
    void Draw() override;
    
    // カメラの角度を設定する(毎フレームUpdateの前に実行する)
    void SetCameraAngleY(float cameraAngleY) { m_cameraAngleY = cameraAngleY; }

	// マップモデルのハンドルを設定する(Initの前に実行する)
	void SetMapHandle(int mapHandle) { m_mapHandle = mapHandle; }

    void SetTarget(std::shared_ptr<Character> target) { m_target = target; }

    // 角度を取得する
    float GetAngle() const { return m_angle; }
    // 自身の向きを表すベクトルを取得する
    Vector3 GetDir() const;

private:

    void Move();    // 移動処理
    void Jump();    // ジャンプ処理

    void RotateInputDir();  // 入力方向を向く

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

    // 操作可能かどうかフラグ
    bool m_isCanControll = true;

    // 回避のフレーム数
    int m_dodgeFrame = 0;

    // コンボのフレーム数
    int m_comboFrame = 0;
    // 次のコンボに移行するかどうか
    bool m_isTransferNextCombo = false;

    // カメラの角度
    float m_cameraAngleY = 0.0f;
	// 自身のモデルの回転角度
    float m_angle = 0.0f;
	// 描画用の回転角度(Lerpで滑らかに回転する)
	float m_drawAngle = 0.0f;
    
    // プレイヤーのステート
    std::shared_ptr<PlayerStateBase> m_pState = nullptr;

    // プレイヤーのステートクラスがプレイヤーにアクセスできるようにする
    friend PlayerStateBase;
    friend PlayerStateIdle;
    friend PlayerStateMove;
    friend PlayerStateFall;
    friend PlayerStateDodge;
    friend PlayerStateAttack;
};

