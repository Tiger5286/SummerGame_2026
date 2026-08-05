#include "SoundManager.h"
#include "Dxlib.h"
#include <cassert>

namespace
{
	// 音がフェードする時間(フレーム)
	constexpr int kFadeFrame = 60;
}

SoundManager::SoundManager()
{
}

SoundManager::~SoundManager()
{
	DeleteSoundAll();
}

void SoundManager::Init()
{
}

void SoundManager::Update()
{
	// 音のフェードイン、アウト処理
	for (auto& pair : m_soundMap)
	{
		// 再生中の音がフェードイン・フェードアウト中かどうかを確認
		if (CheckSoundMem(pair.second.handle))
		{
			// フェードイン中ならフレームカウントを進める
			if (pair.second.fadeState == SoundFadeState::FadeIn)
			{
				// フレームカウントを進める
				if (pair.second.fadeFrame < kFadeFrame)
				{
					pair.second.fadeFrame++;
				}
				else
				{	// フェードイン完了
					pair.second.fadeState = SoundFadeState::None;
				}
			}	// フェードアウト中ならフレームカウントを戻す
			else if (pair.second.fadeState == SoundFadeState::FadeOut)
			{
				// フレームカウントを戻す
				if (pair.second.fadeFrame > 0)
				{
					pair.second.fadeFrame--;
				}
				else
				{	// フェードアウト完了
					pair.second.fadeState = SoundFadeState::None;
					StopSoundMem(pair.second.handle);	// 音を停止
				}
			}
			// フレームカウントに応じて音量を調整
			int targetVolume = pair.second.type == SoundType::BGM ? m_bgmVolume : m_seVolume;
			int newVolume = static_cast<int>(pair.second.fadeFrame / static_cast<float>(kFadeFrame) * targetVolume);
			if (pair.second.fadeState == SoundFadeState::None) newVolume = targetVolume;
			ChangeVolumeSoundMem(newVolume, pair.second.handle);
		}
	}
}

void SoundManager::LoadSound(const std::wstring& soundName, const std::wstring& filePath, SoundType type)
{
	// 同じ名前で登録しようとしたらエラー
	assert(m_soundMap.find(soundName) == m_soundMap.end() && "すでに登録されている音です");
	// 音を読み込み
	int soundHandle = LoadSoundMem(filePath.c_str());
	assert(soundHandle != -1 && "サウンドを正しくロードできませんでした。");
	// mapにハンドルと音の種類を登録
	m_soundMap[soundName].handle = soundHandle;
	m_soundMap[soundName].type = type;
	// 音の種類に応じて音量を調節
	ChangeVolumeSoundMem(type == SoundType::BGM ? m_bgmVolume : m_seVolume, soundHandle);
}

void SoundManager::PlaySoundGame(const std::wstring& soundName, bool isLoop, bool isFade)
{
	// 未登録の名前を渡すとエラー
	assert(m_soundMap.find(soundName) != m_soundMap.end() && "未登録の音です");
	// 登録された名前のハンドルの音を再生
	int handle = m_soundMap[soundName].handle;
	PlaySoundMem(handle, isLoop ? DX_PLAYTYPE_LOOP : DX_PLAYTYPE_BACK);	// ループするかどうかを決める
	// フェードして再生するならフェード状態をフェードインにする
	if (isFade)
	{
		m_soundMap[soundName].fadeState = SoundFadeState::FadeIn;
		m_soundMap[soundName].fadeFrame = 0;
	}
	else	// フェードしないなら即再生
	{
		m_soundMap[soundName].fadeState = SoundFadeState::None;
		m_soundMap[soundName].fadeFrame = kFadeFrame;
	}
}

void SoundManager::StopSound(const std::wstring& soundName, bool isFade)
{
	// 未登録の名前を渡すとエラー
	assert(m_soundMap.find(soundName) != m_soundMap.end() && "未登録の音です");
	// フェードアウト状態にする
	m_soundMap[soundName].fadeState = SoundFadeState::FadeOut;
	if (!isFade)
	{	// フェードしないならフェードアウトを即時終わらせる
		m_soundMap[soundName].fadeState = SoundFadeState::None;
		m_soundMap[soundName].fadeFrame = 0;
		StopSoundMem(m_soundMap[soundName].handle);
	}
}

void SoundManager::StopSoundAll(bool isFade)
{
	// 全ての登録されている音でStopSoundを実行
	for (auto& pair : m_soundMap)
	{
		StopSound(pair.first, isFade);
	}
}

void SoundManager::DeleteSoundAll()
{
	// 全ての音をメモリから解放
	for (const auto& pair : m_soundMap)
	{
		// 再生中なら止める
		if (CheckSoundMem(pair.second.handle))
		{
			StopSoundMem(pair.second.handle);
		}
		DeleteSoundMem(pair.second.handle);
	}
	// mapをクリア
	m_soundMap.clear();
}

void SoundManager::DeleteSound(const std::wstring& soundName)
{
	// 未登録の名前を渡すとエラー
	assert(m_soundMap.find(soundName) != m_soundMap.end() && "未登録の音です");
	// 再生中なら止める
	if (CheckSoundMem(m_soundMap[soundName].handle))
	{
		StopSoundMem(m_soundMap[soundName].handle);
	}
	// 音をメモリから解放
	DeleteSoundMem(m_soundMap[soundName].handle);
	// 登録されている名前を除外
	m_soundMap.erase(soundName);
}

void SoundManager::ChangeVolume(SoundType type, int volume)
{
	// 音を範囲内に丸める
	if (volume < 0) volume = 0;
	if (volume > 255) volume = 255;

	// 音の種類によって調節する音を変える
	switch (type)
	{
	case SoundType::BGM:
		m_bgmVolume = volume;
		break;
	case SoundType::SE:
		m_seVolume = volume;
		break;
	}
	// 音の種類ごとに音量を調節
	for (const auto& pair : m_soundMap)
	{
		if (pair.second.type == type)
		{
			ChangeVolumeSoundMem(volume, pair.second.handle);
		}
	}
}

SoundManager& SoundManager::GetInstance()
{
	static SoundManager instance;
	return instance;
}
