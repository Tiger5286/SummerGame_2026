#include "SpawnerManager.h"
#include "../EnemySpawner.h"
#include <string>
#include <fstream>
#include <sstream>
#include <cassert>
#include <unordered_map>
#include "Utility/MyLib.h"
#include "Singleton/EventManager.h"

namespace
{
	// 敵の種類と文字列の対応表
	const std::unordered_map<std::string, MyLib::EnemyType> kEnemyTypeTable = {
		{ "Zombie",  MyLib::EnemyType::Zombie},
		{ "Vulture", MyLib::EnemyType::Vulture },
		{ "General", MyLib::EnemyType::General }
	};
}

//void SpawnerManager::Loadcsv()
//{
//	// ファイルを開く
//	std::ifstream file(kfilePath);
//	// ファイルが開けなかったらassert
//	if (!file.is_open())
//	{
//		assert(false && "SpawnerData.csv が正しく開けませんでした");
//		return;
//	}
//
//	std::string line;
//	// 行単位で読み込む(行ごとにループする)
//	while (std::getline(file, line))
//	{
//		// 行が空、または行の1文字目が#ならその行は無視する
//		if (line.empty() || line[0] == '#') continue;
//
//		// 新しいEnemySpawnerを生成する
//		m_pEnemySpawners.push_back(std::make_shared<EnemySpawner>());
//		auto spawner = m_pEnemySpawners.back();
//
//		std::stringstream ss(line);
//		std::string cell;
//
//		EnemySpawner::Data data;
//		int index = 0;	// 今何番目のデータを処理しているかのカウンター
//		
//		// 一時保存用の変数
//		EnemySpawner::EnemyData enemyData;
//
//		// カンマ区切りでループする
//		while (std::getline(ss, cell, ','))
//		{
//			// 空白だったらループを抜ける
//			if (cell == "")
//			{
//				break;
//			}
//			// 最初の4データ：スポナー自身の情報
//			if (index == 0)		 data.pos.x = std::stof(cell);
//			else if (index == 1) data.pos.y = std::stof(cell);
//			else if (index == 2) data.pos.z = std::stof(cell);
//			else if (index == 3) data.radius = std::stof(cell);
//
//			// 5データ目以降：敵の種類と出現位置の繰り返し
//			else
//			{
//				// 4からの余りで的データの何番目かを判定する
//				int enemyDataIndex = (index % 4);
//
//				if (enemyDataIndex == 0)	// 0：敵の種類名
//				{
//					// 敵の種類と文字列の対応表から探して代入
//					auto it = kEnemyTypeTable.find(cell);
//					if (it != kEnemyTypeTable.end())
//					{
//						enemyData.type = it->second;
//						// 文字列が出てきた時点で要素を増やす
//						data.enemyDatas.push_back(enemyData);
//					}
//					else	// 文字列が対応していなかったらassert
//					{
//						assert(false && "未定義の敵の種類がcsvに記載されています");
//					}
//				}
//				else if (enemyDataIndex == 1) enemyData.localPos.x = std::stof(cell);
//				else if (enemyDataIndex == 2) enemyData.localPos.y = std::stof(cell);
//				else if (enemyDataIndex == 3) enemyData.localPos.z = std::stof(cell);
//			}
//			// 敵のデータを保存
//			if (!data.enemyDatas.empty())
//			{
//				data.enemyDatas.back() = enemyData;
//			}
//			// カウンターをカウントアップ
//			index++;
//		}
//		// スポナーの初期化(データを渡す)
//		spawner->Init(m_pEnemyManager, m_pPlayer, data);
//		// 行の終わり
//	}
//}

void SpawnerManager::LoadBinaly(std::wstring filePath)
{
	bool asyncLoadFlag = GetUseASyncLoadFlag();
	SetUseASyncLoadFlag(false);

	// バイナリを開く
	auto handle = FileRead_open(filePath.c_str());
	assert(handle != -1);

	// スポナーの数を読み込む
	int spawnerNum = 0;
	FileRead_read(&spawnerNum, sizeof(int), handle);
	// スポナーの数だけループ
	for (int i = 0; i < spawnerNum; i++)
	{
		m_pEnemySpawners.push_back(std::make_shared<EnemySpawner>());
		auto spawner = m_pEnemySpawners.back();

		EnemySpawner::Data data;

		char byteNum;	// スポナーのタグのバイト数
		FileRead_read(&byteNum, sizeof(char), handle);
		std::string spawnerTag;	// スポナーのタグ
		spawnerTag.resize(byteNum);
		FileRead_read(spawnerTag.data(), byteNum, handle);
		Vector3 spawnerPos;	// スポナーの位置
		MyLib::ReadVector3(spawnerPos, handle);
		float spawnerRadius = 0.0f;	// スポナーの半径
		FileRead_read(&spawnerRadius, sizeof(float), handle);

		data.tag = spawnerTag;
		data.pos = spawnerPos * 100;
		data.radius = spawnerRadius * 100;

		int enemyNum = 0;	// 敵の数
		FileRead_read(&enemyNum, sizeof(int), handle);
		// 敵の数だけループ
		for (int j = 0; j < enemyNum; j++)
		{
			EnemySpawner::EnemyData enemyData;

			char byteNum;	// 敵の種類の文字列のバイト数(1バイトで出力されるためchar)
			FileRead_read(&byteNum, sizeof(char), handle);
			std::string enemyType;
			enemyType.resize(byteNum);
			FileRead_read(enemyType.data(), byteNum, handle);
			Vector3 enemyPos;	// 敵の位置
			MyLib::ReadVector3(enemyPos, handle);
			enemyPos -= spawnerPos;
			// 文字列から敵タイプに変換して代入
			auto it = kEnemyTypeTable.find(enemyType);
			if (it != kEnemyTypeTable.end())
			{
				enemyData.type = it->second;
				enemyData.localPos = enemyPos * 100;
				data.enemyDatas.push_back(enemyData);
			}
			else
			{
				assert(false && "error SpawnerManager::LoadBinaly()");
			}
		}

		spawner->Init(m_pEnemyManager, m_pPlayer, data);
	}

	FileRead_close(handle);

	SetUseASyncLoadFlag(asyncLoadFlag);
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
		// 前のフレームで出現させたか取得
		bool isPrevSpawned = spawner->IsSpawned();
		// 更新
		spawner->Update();
		// 出現させた瞬間
		if (!isPrevSpawned && spawner->IsSpawned())
		{
			if (spawner->GetTag() == "BossSpawner")
			{
				EventManager::GetInstance().CallEvent("SpawnBoss");
			}
		}
	}
}

void SpawnerManager::Draw()
{
	for (auto& spawner : m_pEnemySpawners)
	{
		spawner->Draw();
	}
}