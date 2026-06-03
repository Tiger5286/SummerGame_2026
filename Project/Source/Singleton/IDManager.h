#pragma once

// ゲーム内で被ることのないIDを作成して配るクラス
class IDManager
{
public:
	static IDManager& GetInstance();
	virtual ~IDManager() = default;

	IDManager(const IDManager&) = delete;
	IDManager& operator=(const IDManager&) = delete;
private:
	IDManager() = default;
public:
	int GetNewId();

private:
	int m_nextId = 0;
};

