#pragma once
#include <string>
#include <map>

class ShaderManager
{
public:
	static ShaderManager& GetInstance();
	~ShaderManager();

	ShaderManager(const ShaderManager&) = delete;
	ShaderManager& operator=(const ShaderManager&) = delete;
private:
	ShaderManager() = default;
public:

	enum class ShaderType
	{
		Pixel,
		Vertex
	};

	void Init();

	void LoadShader(const std::wstring& fileName, const std::wstring& key, ShaderType type);

	int GetHandle(const std::wstring& key);

private:
	void LoadMV1DefaultShader();

private:
	std::map<std::wstring, int>  m_shaderHandles;
};