#pragma once
#include <string>
#include <functional>
#include <vector>

class Menu
{
private:
	struct Item
	{
		std::wstring name;
		std::function<void()> action;
		float scale = 0.0f;
		float colorRate = 0.0f;
	};

public:
	struct Funcs
	{
		std::wstring name;
		std::function<void()> action;
	};

	Menu() = default;
	~Menu();

	/// <summary>
	/// メニューの初期化
	/// </summary>
	/// <param name="actions">メニューのアクションの配列</param>
	/// <param name="x">メニューの一番上の項目の真ん中座標X</param>
	/// <param name="y">メニューの真ん中座標Y</param>
	void Init(std::vector<Funcs> actions,int x,int y);
	void Update();
	void Draw();

private:
	std::vector<Item> m_items;	// メニューのアクション(関数)を格納する配列
	int m_selectIndex = 0;			// 選択中のメニューの番号

	int m_uiHandle = -1;
	int m_selectUIHandle = -1;

	int m_fontHandle = -1;

	int m_x = 0;
	int m_y = 0;
};