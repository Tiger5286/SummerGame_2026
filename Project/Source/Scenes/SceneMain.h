#pragma once
#include <memory>
#include <vector>
#include <string>
#include "SceneBase.h"
#include "Utility/Vector3.h"

class Player;
class Camera;
class SkyBox;

class TargetManager;

class EnemyManager;

class SpawnerManager;

class SceneMain : public SceneBase
{
public:
	// ロードするエフェクトのファイル名と登録名
	struct EffectData
	{
		std::wstring path;
		std::wstring name;
		float scale;
	};
	// ボス演出用のデータ
	struct BossDirectionData
	{
		Vector3 lightDir;
		Vector3 cameraPos;
		Vector3 cameraTargetPos;
	};
	enum class Stage
	{
		First,
		Second,
		Num
	};
	// ステージごとの異なるデータ
	struct UniqueDatas
	{
		Stage stage;						// ステージの種類
		std::wstring collisionFilePath;		// ステージの当たり判定モデルのファイルパス
		std::wstring stageFilePath;			// ステージの見た目モデルのファイルパス
		std::wstring spawnerDataFilePath;	// スポナーデータのファイルパス
		BossDirectionData bossDirectionData;// ボス演出用のデータ
#ifdef _DEBUG
		Vector3 bossRoomEntrancePos;		// デバッグ用:ボス部屋の入り口の座標
#endif
	};

public:
	SceneMain(SceneManager& sceneManager);
	~SceneMain();

	void Init() override;
	void End() override;
	void Update() override;
	void Draw() override;

	/// <summary>
	/// ステージごとの固有のデータをセットする(Initの前に実行する)
	/// </summary>
	/// <param name="filePaths">ステージごとの固有データのファイルパス</param>
	void SetData(UniqueDatas filePaths);

private:
	void OnSpawnBoss();

	void SpawnBossUpdate();

private:
	int m_frameCount = 0;

	int m_shadowMapHandle = -1;

	bool m_isBossBattle = false;
	bool m_isEndScene = false;

	int m_onSpawnBossHandle = -1;
	// ボス出現演出中かどうかのフラグ
	bool m_isDirectionSpawnBoss = false;
	int m_directionBossFrameCount = 0;

	std::shared_ptr<Player> m_pPlayer = nullptr;
	std::shared_ptr<Camera> m_pCamera = nullptr;

	std::shared_ptr<EnemyManager> m_pEnemyManager = nullptr;
	std::shared_ptr<TargetManager> m_pTargetManager = nullptr;

	std::shared_ptr<SkyBox> m_pSkyBox = nullptr;

	std::shared_ptr<SpawnerManager> m_pSpawnerManager = nullptr;

	UniqueDatas m_uniqueDatas;
};