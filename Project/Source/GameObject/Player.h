#pragma once
#include "GameObject.h"
#include "../System/Animator.h"
#include "../Collider/CapsuleCollider.h"

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

	// マップモデルのハンドルを設定する(Initの前に実行する)
	void SetMapHandle(int mapHandle) { m_mapHandle = mapHandle; }

private:

    void Move();    // 移動処理
    void Jump();    // ジャンプ処理
    void Dodge();   // 回避処理
    void Attack();  // 攻撃処理
    void CancelAttack();    // 攻撃を中止する

    // マップに当たったときの処理
	void CheckHitMap(MV1_COLL_RESULT_POLY_DIM coll);

    // 設置判定
    void CheckGround();

    // 現在の状態を表す列挙体
    enum class State
    {
        Idle,
        Run,
        Fall,
        Dodge,
        Combo1,
        Combo2,
        Combo3,


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
    CapsuleCollider m_collider;
	int m_mapHandle = -1;	// マップのモデルのハンドル

    // 操作可能かどうかフラグ
    bool m_isCanControll = true;

	State m_state = State::Idle;    // 現在の状態
	State m_prevState = State::Idle;    // 1フレーム前の状態

	bool m_isGround = false;    // 地面にいるかどうか

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
};

