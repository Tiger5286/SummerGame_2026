#include "ModelManager.h"
#include "Dxlib.h"
#include <cassert>

ModelManager& ModelManager::GetInstance()
{
	static ModelManager instance;
	return instance;
}

ModelManager::~ModelManager()
{
	// すべてのロードしたモデルを削除
	for (auto& handle : m_modelHandles)
	{
		MV1DeleteModel(handle.second);
	}
	m_modelHandles.clear();
}

void ModelManager::LoadModel(std::wstring fileName, std::wstring key)
{
	// キー重複チェック
	if (m_modelHandles.contains(key))
	{
		assert(false && "同じ名前のモデルをロードしようとしています。");
		return;
	}
	// モデルをロードして登録
	auto handle = MV1LoadModel(fileName.c_str());
	assert(handle != -1 && "モデルが正しくロードされませんでした");
	m_modelHandles[key] = handle;
}

void ModelManager::DeleteModel(std::wstring key)
{
	// 存在しないキーをチェック
	if (!m_modelHandles.contains(key))
	{
		assert(false && "存在しないモデルを削除しようとしています");
		return;
	}
	// モデルを削除してキーを除外
	auto check = MV1DeleteModel(m_modelHandles[key]);
	assert(check != -1 && "モデルが正しく削除されませんでした");
	m_modelHandles.erase(key);
}

int ModelManager::DuplicateModel(std::wstring key) const
{
	// 存在しないキーをチェック
	if (!m_modelHandles.contains(key))
	{
		assert(false && "存在しないモデルを複製しようとしています");
		return -1;
	}
	// モデルを複製してハンドルを返す
	auto handle = MV1DuplicateModel(m_modelHandles.at(key));
	assert(handle != -1 && "モデルが正しく複製されませんでした");
	return handle;
}

int ModelManager::GetModelHandle(std::wstring key) const
{
	// 存在しないキーをチェック
	if (!m_modelHandles.contains(key))
	{
		assert(false && "存在しないモデルを指定しています");
		return -1;
	}
	return m_modelHandles.at(key);
}
