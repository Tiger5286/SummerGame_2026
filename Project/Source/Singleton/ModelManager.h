#pragma once
#include <map>
#include <string>

/// <summary>
/// モデルを管理するシングルトンクラス
/// </summary>
class ModelManager
{
public:
	/// <summary>
	/// このクラスのインスタンスを取得する
	/// </summary>
	/// <returns>このクラスのインスタンス</returns>
	static ModelManager& GetInstance();
	virtual ~ModelManager();

	// コピーコンストラクタと代入演算子を禁止
	ModelManager(const ModelManager&) = delete;
	ModelManager& operator=(const ModelManager&) = delete;

private:
	// シングルトンパターンのためコンストラクタをprivateにする
	ModelManager() = default;
public:

	/// <summary>
	/// モデルをロードし登録する
	/// </summary>
	/// <param name="fileName">ロードするモデルのファイル名</param>
	/// <param name="key">登録するモデルの名前</param>
	void LoadModel(std::wstring fileName,std::wstring key);

	/// <summary>
	/// モデルを削除し登録解除する
	/// </summary>
	/// <param name="key">削除するモデルの名前</param>
	void DeleteModel(std::wstring key);

	/// <summary>
	/// 登録されたモデルを複製しそのハンドルを返す
	/// ※このクラスは複製されたモデルのハンドルを管理しないのでそれぞれで管理すること
	/// </summary>
	/// <param name="key">複製するモデルの名前</param>
	/// <returns>複製されたモデルのハンドル</returns>
	int DuplicateModel(std::wstring key) const;

	/// <summary>
	/// 登録されたモデルのハンドルを返す
	/// </summary>
	/// <param name="key">モデルの名前</param>
	/// <returns>モデルのハンドル</returns>
	int GetModelHandle(std::wstring key) const;

private:
	std::map<std::wstring, int> m_modelHandles;
};

