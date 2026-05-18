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
	void ChangeAnim(std::wstring animName, float animSpeed = 0.5f,bool isLoop = true);

	/// <summary>
	/// 現在再生中のアニメーションが終わったかどうか取得する(ループが有効だとtrueを返さない)
	/// </summary>
	/// <returns>true:終わった / false:終わってない</returns>
	bool IsEnd();

	bool operator==(const std::wstring& animName);
	bool operator!=(const std::wstring& animName);

private:
	// アニメーションを行うモデルのハンドル
	int m_modelHandle = -1;

	// アニメーションのハンドル
	int m_currentAnimHandle = -1;
	int m_lastAnimHandle = -1;
	// アニメーション名
	std::wstring m_currentAnimName;

	// アニメーション管理
	float m_animSpeed = 0.5f;
	float m_currentAnimTime = 0.0f;
	float m_lastAnimTime = 0.0f;

	bool m_isCurrentAnimLoop = true;
	bool m_isLastAnimLoop = true;

	// アニメーションの切り替えフレーム数
	int m_animChangeFrame = 0;
};

