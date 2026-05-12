#pragma once
#include <string>

class Animator
{
public:
	Animator();
	virtual ~Animator();

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="modelHandle">モデルのハンドル</param>
	/// <param name="animName">初期のアニメーション名</param>
	void Init(int modelHandle, std::wstring animName);
	// 更新
	void Update();

	/// <summary>
	/// アニメーションを切り替える
	/// </summary>
	/// <param name="animName">次のアニメーション名</param>
	/// <param name="animSpeed">アニメーションの再生速度</param>
	void ChangeAnim(std::wstring animName, float animSpeed = 0.5f);
private:
	// アニメーションを行うモデルのハンドル
	int m_modelHandle = -1;

	// アニメーションのハンドル
	int m_currentAnimHandle = -1;
	int m_lastAnimHandle = -1;

	// アニメーション管理
	float m_animSpeed = 0.5f;
	float m_currentAnimTime = 0.0f;
	float m_lastAnimTime = 0.0f;

	// アニメーションの切り替えフレーム数
	int m_animChangeFrame = 0;
};

