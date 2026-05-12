#pragma once
#include "GameObject.h"
#include "../System/Animator.h"

class Input;

class Player :
    public GameObject
{
public:
    Player(Input& input);
    virtual ~Player();

    void Init() override;
    void End() override;
    void Update() override;
    void Draw() override;
    
    // カメラの角度を設定する(毎フレームUpdateの前に実行する)
    void SetCameraAngleY(float cameraAngleY) { m_cameraAngleY = cameraAngleY; }

private:

    void Move();    // 移動処理
    void Jump();    // ジャンプ処理

    // 現在の状態を表す列挙体
    enum class State
    {
        Idle,
        Run,
        Fall,

        Num
    };

    /// <summary>
    /// 現在の状態をチェックしてステートを更新する
    /// </summary>
    void UpdateState();

    /// <summary>
    /// ステートが変わった瞬間を取得する
    /// </summary>
    /// <param name="state">取得したいステート</param>
    /// <returns>変わった瞬間かどうか</returns>
    bool TriggerdChangeState(State state);

private:
    Input& m_input; // 入力クラスの参照
	Animator m_anim;    // アニメーションクラス

	bool m_isGround = false;    // 地面にいるかどうか

	State m_state = State::Idle;    // 現在の状態
	State m_prevState = State::Idle;    // 1フレーム前の状態

    // カメラの角度
    float m_cameraAngleY = 0.0f;

	// 自身のモデルの回転角度
    float m_angle = 0.0f;
	// 描画用の回転角度(Lerpで滑らかに回転する)
	float m_drawAngle = 0.0f;
};

