#include "ShaderManager.h"
#include "DxLib.h"
#include <cassert>

ShaderManager& ShaderManager::GetInstance()
{
    static ShaderManager instance;
    return instance;
}

ShaderManager::~ShaderManager()
{
    InitShader();
}

void ShaderManager::Init()
{
    InitShader();
}

void ShaderManager::LoadShader(const std::wstring & fileName, const std::wstring & key, ShaderType type)
{
    // キー重複チェック
    if (m_shaderHandles.contains(key))
    {
        assert(false && "同じ名前のシェーダーをロードしようとしています");
        return;
    }
    // シェーダーをロードして登録
    int handle = -1;
    switch (type)
    {
    case ShaderType::Pixel:
        handle = LoadPixelShader(fileName.c_str());
        assert(handle != -1 && "シェーダーが正しくロードできませんでした");
        break;
        
    case ShaderType::Vertex:
        handle = LoadVertexShader(fileName.c_str());
        assert(handle != -1 && "シェーダーが正しくロードできませんでした");
        break;

    default:
        assert(false && "未知のシェーダータイプです");
    }
}

int ShaderManager::GetHandle(const std::wstring & key)
{
    return m_shaderHandles[key];
}
