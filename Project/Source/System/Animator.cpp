#include "Animator.h"
#include "Dxlib.h"
#include <cassert>

namespace
{
	// アニメーションの切り替えフレーム数
	constexpr int kAnimChangeFrame = 8;
}

Animator::Animator()
{
}

Animator::~Animator()
{
}

void Animator::Init(int modelHandle, std::wstring animName)
{
	// モデルを設定
	m_modelHandle = modelHandle;
	//アニメーション名を設定
	m_currentAnimName = animName;

	// アニメーションをアタッチ
	auto animIndex = MV1GetAnimIndex(m_modelHandle, animName.c_str());
	assert(animIndex != -1 && "アニメーションが見つかりませんでした");
	m_currentAnimHandle = MV1AttachAnim(m_modelHandle, animIndex);
	m_currentAnimTime = 0.0f;

	m_lastAnimHandle = -1;
	m_lastAnimTime = 0.0f;

	m_animChangeFrame = 0;
}

void Animator::Update()
{
	// アニメーションのブレンドで滑らかに切り替える
	m_animChangeFrame++;
	if (m_lastAnimHandle == -1)
	{
		// アニメーションが一つしか設定されていない場合
		MV1SetAttachAnimBlendRate(m_modelHandle, m_currentAnimHandle, 1.0f);
	}
	else
	{
		// currentAnimのブレンド率を計算
		float rate = static_cast<float>(m_animChangeFrame) / kAnimChangeFrame;
		if (rate > 1.0f) rate = 1.0f;

		MV1SetAttachAnimBlendRate(m_modelHandle, m_currentAnimHandle, rate);
		MV1SetAttachAnimBlendRate(m_modelHandle, m_lastAnimHandle, 1.0f - rate);
	}

	// アニメーションの進行
	m_currentAnimTime += m_animSpeed;
	// アニメーションのループ処理
	if (m_isLoop)
	{
		float totalCount = MV1GetAttachAnimTotalTime(m_modelHandle, m_currentAnimHandle);
		while (m_currentAnimTime >= totalCount)
		{
			m_currentAnimTime -= totalCount;
		}
	}
	MV1SetAttachAnimTime(m_modelHandle, m_currentAnimHandle, m_currentAnimTime);

	// ひとつ前のアニメーションも進行させる
	if (m_lastAnimHandle != -1)
	{
		m_lastAnimTime += m_animSpeed;
		float lastTotalCount = MV1GetAttachAnimTotalTime(m_modelHandle, m_lastAnimHandle);
		while (m_lastAnimTime >= lastTotalCount)
		{
			m_lastAnimTime -= lastTotalCount;
		}
		MV1SetAttachAnimTime(m_modelHandle, m_lastAnimHandle, m_lastAnimTime);
	}
}

void Animator::ChangeAnim(std::wstring animName, float animSpeed,bool isLoop)
{
	// 新しくアニメーションを設定しようとしたとき
	// 古いアニメーションが残っている場合、それをデタッチ
	if (m_lastAnimHandle != -1)
	{
		MV1DetachAnim(m_modelHandle, m_lastAnimHandle);
		m_lastAnimHandle = -1;
	}

	// 現在再生中のアニメーションを一つ前のアニメーションとする
	m_lastAnimHandle = m_currentAnimHandle;
	m_lastAnimTime = m_currentAnimTime;
	// 次に再生するアニメーションをセット
	auto animIndex = MV1GetAnimIndex(m_modelHandle, animName.c_str());
	assert(animIndex != -1 && "アニメーションが見つかりませんでした");
	m_currentAnimHandle = MV1AttachAnim(m_modelHandle, animIndex);
	m_currentAnimTime = 0.0f;
	// アニメーションの切り替えフレーム数をリセット
	m_animChangeFrame = 0;
	// アニメーションの進行速度を設定
	m_animSpeed = animSpeed;
	// ループするかどうかを設定
	m_isLoop = isLoop;
	// アニメーション名を設定
	m_currentAnimName = animName;
}

bool Animator::IsEnd()
{
	// ループするならfalseを返す
	if (m_isLoop)
	{
		return false;
	}

	// アニメーションの総再生時間を現在の再生時間が下回っていたらfalse
	auto animIndex = MV1GetAnimIndex(m_modelHandle, m_currentAnimName.c_str());
	auto totalTime = MV1GetAnimTotalTime(m_modelHandle, animIndex);
	if (m_currentAnimTime <= totalTime)
	{
		return false;
	}

	// 全てにあてはまらなければtrue
	return true;
}

bool Animator::operator!=(const std::wstring& animName)
{
	return !(*this == animName);
}

bool Animator::operator==(const std::wstring& animName)
{
	if (m_currentAnimName == animName)
	{
		return true;
	}
	return false;
}
