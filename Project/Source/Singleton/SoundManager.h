#pragma once
#include <map>
#include <string>

class SoundManager
{
private:
	enum class SoundFadeState
	{
		None,
		FadeIn,
		FadeOut
	};

public:
	enum class SoundType
	{
		BGM,
		SE
	};

	struct SoundInfo
	{
		int handle = -1;
		SoundType type = SoundType::SE;
		SoundFadeState fadeState = SoundFadeState::None;
		int fadeFrame = 0;
		bool m_isRequestDelete = false;
	};

public:
	static SoundManager& GetInstance();
	~SoundManager();

	void Init();
	void Update();

	/// <summary>
	/// 読み込んだ音声ファイルを再生する
	/// </summary>
	/// <param name="soundName">登録した名前</param>
	/// <param name="isLoop">ループするかどうか</param>
	/// <param name="isFade">再生開始時に音がフェードインするかどうか</param>
	void PlaySoundGame(const std::wstring& soundName, bool isLoop = false, bool isFade = false);	// PlaySoundにしたかったけど何かと被ってエラー吐くので仕方なくPlaySoundGameに

	/// <summary>
	/// 再生中の音を停止する
	/// </summary>
	/// <param name="soundName">停止する音</param>
	/// <param name="isFade">音がフェードアウトするかどうか</param>
	void StopSound(const std::wstring& soundName, bool isFade = false);

	/// <summary>
	/// すべての再生中の音を停止する
	/// </summary>
	void StopSoundAll(bool isFade = false);

	/// <summary>
	/// 音量を調整する
	/// </summary>
	/// <param name="type">調整する音の種類</param>
	/// <param name="volume">音量</param>
	void ChangeVolume(SoundType type, int volume);

	// 音量取得関数
	int GetBGMVolume() const { return m_bgmVolume; }
	int GetSEVolume() const { return m_seVolume; }

	/// <summary>
	/// 音声ファイルを読み込み、登録する
	/// </summary>
	/// <param name="soundName">登録する名前</param>
	///	<param name="filePath">ファイル名</param>
	/// <param name="type">音の種類</param>
	void LoadSound(const std::wstring& soundName, const std::wstring& filePath, SoundType type);

	/// <summary>
	/// 読み込んだ音声ファイルを全て開放し、登録を解除する
	/// </summary>
	void DeleteSoundAll();

	/// <summary>
	/// 読み込んだ音声ファイルを開放し、登録を解除する
	/// </summary>
	/// <param name="soundName">解放する音の登録名</param>
	void DeleteSound(const std::wstring& soundName);

private:
	SoundManager();
	SoundManager(const SoundManager& sm) = delete;
	void operator=(const SoundManager& sm) = delete;

	// 音声ファイルマップ
	std::map<std::wstring, SoundInfo> m_soundMap;

	// 音量(0~255)
	int m_bgmVolume = 255 / 10 * 5;
	int m_seVolume = 255 / 10 * 7;
};