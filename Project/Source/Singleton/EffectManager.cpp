#include "EffectManager.h"
#include "EffekseerForDXLib.h"

EffectManager& EffectManager::GetInstance()
{
	static EffectManager instance;
	return instance;
}

EffectManager::~EffectManager()
{
	StopEffectAll();
}

void EffectManager::Update()
{
	UpdateEffekseer3D();
	// 再生されていないエフェクトハンドルを除外する
	std::list<int> deleteHandles;
	for (auto& handle : m_effectPlayingHandles)
	{
		if (IsEffekseer3DEffectPlaying(handle) == -1)
		{
			deleteHandles.push_back(handle);
		}
	}
	for (auto& handle : deleteHandles)
	{
		m_effectPlayingHandles.remove(handle);
	}
}

void EffectManager::Draw()
{
	if (m_effectPlayingHandles.empty())
	{
		return;	// 再生中のエフェクトがなければ描画しない
	}
	DrawEffekseer3D();
}

void EffectManager::LoadEffect(const std::wstring& filePath, const std::wstring& name, float scale)
{
	if (m_effectResourceHandles.contains(name))
	{
		assert(false && "登録するエフェクトの名前が重複しています");
		return;
	}
	auto handle = LoadEffekseerEffect(filePath.c_str(), scale);
	assert(handle != -1 && "エフェクトが正しくロードされませんでした");
	m_effectResourceHandles[name] = handle;
}

int EffectManager::PlayEffect(const std::wstring& name, const Vector3& pos)
{
	// 指定されたエフェクトが存在するかどうかチェック
	if (!(m_effectResourceHandles.contains(name)))
	{
		assert(false && "存在しないエフェクトを再生しようとしています");
		return -1;
	}
	// エフェクトを再生し正しく再生されたかチェック
	auto handle = PlayEffekseer3DEffect(m_effectResourceHandles[name]);
	assert(handle != -1 && "エフェクトが正しく再生されませんでした");
	// 位置を設定
	SetPosPlayingEffekseer3DEffect(handle, pos.x, pos.y, pos.z);
	// リストに登録
	m_effectPlayingHandles.push_back(handle);
	return handle;
}

void EffectManager::StopEffectAll()
{
	// 再生中のエフェクトをすべて停止する
	for (auto& effect : m_effectPlayingHandles)
	{
		StopEffekseer3DEffect(effect);
		effect = -1;	// ハンドルを無効にする
	}
	// 再生中のエフェクトのリストを空にする
	m_effectPlayingHandles.clear();
}
