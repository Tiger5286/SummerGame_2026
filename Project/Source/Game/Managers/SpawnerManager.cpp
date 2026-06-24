#include "SpawnerManager.h"
#include "../EnemySpawner.h"
#include <string>
#include <fstream>
#include <sstream>
#include <cassert>
#include <unordered_map>

namespace
{
	const std::wstring kfilePath = L"data/csv/SpawnerData.csv";

	// 敵の種類と文字列の対応表
	const std::unordered_map<std::string, MyLib::EnemyType> kEnemyTypeTable = {
		{ "Zombie",  MyLib::EnemyType::Zombie},
		{ "Vulture", MyLib::EnemyType::Vulture },
		{ "General", MyLib::EnemyType::General }
	};
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
	// 行単位で読み込む(行ごとにループする)
	while (std::getline(file, line))
	{
		//auto wstr = MyLib::StringToWString(line);
		//printfDx(L"%s\n", wstr.c_str());
		//continue;

		// 行が空、または行の1文字目が#ならその行は無視する
		if (line.empty() || line[0] == '#') continue;

		// 新しいEnemySpawnerを生成する
		m_pEnemySpawners.push_back(std::make_shared<EnemySpawner>());
		auto spawner = m_pEnemySpawners.back();

		std::stringstream ss(line);
		std::string cell;

		EnemySpawner::Data data;
		int index = 0;	// 今何番目のデータを処理しているかのカウンター
		
		// 一時保存用の変数
		EnemySpawner::EnemyData enemyData;

		// カンマ区切りでループする
		while (std::getline(ss, cell, ','))
		{
			// 空白だったらループを抜ける
			if (cell == "")
			{
				break;
			}

			// 最初の4データ：スポナー自身の情報
			if (index == 0)		 data.pos.x = std::stof(cell);
			else if (index == 1) data.pos.y = std::stof(cell);
			else if (index == 2) data.pos.z = std::stof(cell);
			else if (index == 3) data.radius = std::stof(cell);

			// 5データ目以降：敵の種類と出現位置の繰り返し
			else
			{
				// 4からの余りで的データの何番目かを判定する
				int enemyDataIndex = (index % 4);

				if (enemyDataIndex == 0)	// 0：敵の種類名
				{
					// 敵の種類と文字列の対応表から探して代入
					auto it = kEnemyTypeTable.find(cell);
					if (it != kEnemyTypeTable.end())
					{
						enemyData.type = it->second;
						// 文字列が出てきた時点で要素を増やす
						data.enemyDatas.push_back(enemyData);
					}
					else	// 文字列が対応していなかったらassert
					{
						assert(false && "未定義の敵の種類がcsvに記載されています");
					}
				}
				else if (enemyDataIndex == 1) enemyData.localPos.x = std::stof(cell);
				else if (enemyDataIndex == 2) enemyData.localPos.y = std::stof(cell);
				else if (enemyDataIndex == 3) enemyData.localPos.z = std::stof(cell);
			}
			// 敵のデータを保存
			if (!data.enemyDatas.empty())
			{
				data.enemyDatas.back() = enemyData;
			}

			// カウンターをカウントアップ
			index++;
		}

		// スポナーの初期化(データを渡す)
		spawner->Init(m_pEnemyManager, m_pPlayer, data);
		// 行の終わり
	}
}

void SpawnerManager::Init(std::shared_ptr<EnemyManager> pEnemyManager, std::shared_ptr<Player> pPlayer)
{
	m_pEnemyManager = pEnemyManager;
	m_pPlayer = pPlayer;
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