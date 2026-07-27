#pragma once
#include <string>

class Animation
{
public:
	static constexpr float kDefaultAnimSpeed = 0.5f;

	Animation() = default;
	~Animation() = default;

	/// <summary>
	/// アニメーションの初期化処理
	/// </summary>
	/// <param name="modelHandle">モデルのハンドル</param>
	/// <param name="animName">アニメーション名</param>
	void Init(int modelHandle, std::wstring animName);

	/// <summary>
	/// アニメーションの更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// アニメーションを再生開始する
	/// </summary>
	/// <param name="isLoop">ループするかどうか</param>
	/// <param name="animSpeed">アニメーションの再生速度</param>
	void Start(bool isLoop, float animSpeed = kDefaultAnimSpeed);

	/// <summary>
	/// アニメーションのブレンド率を設定する
	/// </summary>
	/// <param name="rate">ブレンド率(0.0 ~ 1.0)</param>
	void SetBlendRate(float rate);

	/// <summary>
	/// アニメーションが終わったかどうか取得する　ループしている場合は常にfalseを返す
	/// </summary>
	/// <returns>true : アニメーションが終わった / false : アニメーションが終わっていない</returns>
	bool IsEnd();

private:
	int m_modelHandle = -1;					// モデルのハンドル
	int m_animIndex   = -1;					// アニメーションのインデックス
	int m_attachIndex  = -1;					// 再生中のアニメーションのハンドル
	float m_animSpeed = kDefaultAnimSpeed;	// アニメーションの再生速度
	float m_animTime  = 0.0f;				// アニメーションの現在の時間

	bool m_isLoop = false;	// アニメーションがループするかどうか
};

