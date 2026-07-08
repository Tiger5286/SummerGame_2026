#pragma once
#include <string>
#include <list>
#include <map>
#include "../Utility/Vector3.h"
#include "EffekseerForDXLib.h"

class EffectManager
{
public:
	static EffectManager& GetInstance();
	virtual ~EffectManager();

	EffectManager(const EffectManager&) = delete;
	EffectManager& operator=(const EffectManager&) = delete;
private:
	EffectManager();
public:

	void Update();
	void Draw();

	/// <summary>
	/// エフェクトをロードし登録する
	/// </summary>
	/// <param name="filePath">ロードするエフェクトのファイルパス</param>
	/// <param name="name">登録する名前</param>
	/// <param name="scale">エフェクトの拡大率</param>
	void LoadEffect(const std::wstring& filePath, const std::wstring& name,float scale = 1.0f);

	/// <summary>
	/// エフェクトを再生する
	/// </summary>
	/// <param name="name">再生するエフェクトの登録名</param>
	/// <param name="pos">再生するエフェクトの位置</param>
	/// return : 再生したエフェクトのハンドル
	int PlayEffect(const std::wstring& name, const Vector3& pos);

	/// <summary>
	/// 再生中のエフェクトを停止する
	/// </summary>
	/// <param name="handle">再生中のエフェクトのハンドル</param>
	void StopEffect(int handle);

	/// <summary>
	/// すべての再生中のエフェクトを停止する
	/// </summary>
	void StopEffectAll();

	Effekseer::ManagerRef GetManager() { return m_efkManager; }

private:
	std::map<std::wstring, int> m_effectResourceHandles;
	std::list<int> m_effectPlayingHandles;
	Effekseer::ManagerRef m_efkManager = {};
};