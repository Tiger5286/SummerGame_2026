#include "SpawnerManager.h"
#include "../EnemySpawner.h"
#include <string>
#include <fstream>
#include <sstream>
#include <cassert>

namespace
{
	const std::wstring kfilePath = L"data/csv/SpawnerData.csv";
}

void SpawnerManager::Load()
{
	// ファイルを開く
	std::ifstream file(kfilePath);
	// ファイルが開けなかったらassert
	if (!file.is_open())
	{
		assert(false && "SpawnerData.csv が正しく開けませんでした");
		return;
	}

	std::string line;
	// 行単位で読み込む
	while (std::getline(file, line))
	{
		// 行が空、または行の1文字目が#ならその行は無視する
		if (line.empty() || line[0] == '#') continue;

		// 新しいEnemySpawnerを生成する
		m_pEnemySpawners.push_back(std::make_shared<EnemySpawner>());
		auto spawner = m_pEnemySpawners.back();

		std::stringstream ss(line);
		std::string cell;
		// カンマ区切りでセルごとに分解する
		while (std::getline(ss, cell, ','))
		{
			
		}
		// 行の終わり
	}
}

void SpawnerManager::Update()
{
	for (auto& spawner : m_pEnemySpawners)
	{
		spawner->Update();
	}
}

void SpawnerManager::Draw()
{
	for (auto& spawner : m_pEnemySpawners)
	{
		spawner->Draw();
	}
}
