#include "Animation.h"
#include "DxLib.h"
#include <cassert>

void Animation::Init(int modelHandle, std::wstring animName)
{
	m_modelHandle = modelHandle;
	m_animIndex = MV1GetAnimIndex(m_modelHandle, animName.c_str());
	assert(m_animIndex != -1 && "アニメーション名が不正です");
}

void Animation::Update()
{
	// 初期化、再生チェック
	assert(m_modelHandle != -1 && "アニメーションが初期化されていません");
	assert(m_attachIndex != -1 && "アニメーションが再生されていません");

	// アニメーションを進行させる
	MV1SetAttachAnimTime(m_modelHandle, m_attachIndex, m_animTime);
	m_animTime += m_animSpeed;

	// アニメーションの最大時間を取得
	const float totalTime = MV1GetAttachAnimTotalTime(m_modelHandle, m_attachIndex);

	// ループする場合の処理
	if (m_isLoop)
	{
		// 最大時間を越したら戻す
		while (totalTime > m_animTime)
		{
			m_animTime -= totalTime;
		}
	}
	else	// ループしない場合
	{
		// 最大時間を越したら固定
		if (totalTime > m_animTime)
		{
			m_animTime = totalTime;
		}
	}
}

void Animation::Start(bool isLoop, float animSpeed)
{
	// 初期化チェック
	assert(m_modelHandle != -1 && "アニメーションが初期化されていません");

	// ループ設定を適用
	m_isLoop = isLoop;
	// アニメーション速度を適用
	m_animSpeed = animSpeed;

	// アタッチされていたアニメーションをデタッチする
	MV1DetachAnim(m_modelHandle, m_animIndex);
	// アニメーションをアタッチ
	MV1AttachAnim(m_modelHandle, m_animIndex);
}

void Animation::SetBlendRate(float rate)
{
	// アニメーションのブレンド率を設定
	MV1SetAttachAnimBlendRate(m_modelHandle, m_attachIndex, rate);
}

bool Animation::IsEnd()
{
	// ループする場合はfalse
	if (m_isLoop) return false;

	// アニメーションの最大時間を取得
	const float totalTime = MV1GetAttachAnimTotalTime(m_modelHandle, m_attachIndex);
	// アニメーションが最大まで到達しているならtrue
	if (m_animTime >= totalTime)
	{
		return true;
	}
	// アニメーションが最大まで到達していないならfalse
	return false;
}