#include "ShaderManager.h"
#include "DxLib.h"
#include <cassert>
#include <d3dcompiler.h>
#include <vector>

ShaderManager& ShaderManager::GetInstance()
{
    static ShaderManager instance;
    return instance;
}

ShaderManager::~ShaderManager()
{
    InitShader();
    LoadMV1DefaultShader();
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

void ShaderManager::LoadMV1DefaultShader()
{
    // シェーダーをコンパイルするのに必要なマクロを準備
    std::vector<D3D_SHADER_MACRO> macros = {
        {"SKINMESH",""},
        {nullptr,nullptr}
    };

    ID3DBlob* pVSShader = nullptr;
    ID3DBlob* pMsg = nullptr;
    HRESULT result = D3DCompileFromFile(L"Shader/Default/MV1VertexShader.hlsl",
        macros.data(),
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "main",
        "vs_5_0",
        0, 0, &pVSShader, &pMsg);

    //失敗したらエラー出力
    if (result != S_OK) {
        auto size = pMsg->GetBufferSize();
        std::string strMsg;
        strMsg.resize(size);
        std::copy_n((char*)pMsg->GetBufferPointer(), size, strMsg.data());
        OutputDebugStringA(strMsg.c_str());
        assert(0);
    }

    int handle = LoadVertexShaderFromMem(pVSShader->GetBufferPointer(), pVSShader->GetBufferSize());
    assert(handle != -1);

    m_shaderHandles[L"DefaultVS"] = handle;
}
