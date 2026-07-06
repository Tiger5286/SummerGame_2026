#pragma once
class FadeManager
{
public:
	static FadeManager& GetInstance();
	~FadeManager() = default;

	FadeManager(const FadeManager&) = delete;
	FadeManager& operator=(const FadeManager&) = delete;
private:
	FadeManager() = default;
public:
	enum class State
	{
		None,
		FadeIn,
		FadeOut
	};

	void Update();
	void Draw();

	void StartFadeIn();

	void StartFadeOut();

	bool IsNotFading() const { return m_fadeState == State::None; }
	float GetFadeRate() const { return m_fadeRate; }
	
private:
	State m_fadeState = State::None;
	float m_fadeRate = 1.0f;	// 0:画面が見える / 1:真っ黒
};

