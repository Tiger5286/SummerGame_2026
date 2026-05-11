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

    // 現在の状態を表す列挙体
    enum class State
    {
        Idle,
        Run,

        Num
    };

    /// <summary>
    /// 現在の状態をチェックしてステートを更新する
    /// </summary>
    void UpdateState();

private:
    Input& m_input;
    Animator m_anim;
    State m_state = State::Idle;
    State m_prevState = State::Idle;

    float m_cameraAngleY = 0.0f;

    float m_angle = 0.0f;
};

