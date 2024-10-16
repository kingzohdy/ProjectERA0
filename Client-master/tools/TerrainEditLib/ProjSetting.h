
#pragma once

#include <string>

namespace TE
{
//项目配置文件的位置
#define PROJSETTING_PATH     "data\\proj\\proj.ini"

class CProjSetting
{
public:
	CProjSetting(void);
	~CProjSetting(void);

	bool  Init();

	bool  Read();
	bool  Save();

	void  ChangeMayPath();

	const char*  GetTileTexturePath(bool fullpath){return fullpath?m_strTileTextureFullPath.c_str() : m_strTileTexturePath.c_str();};
	const char*  GetMapPath(bool fullpath){return fullpath?m_strMapFullPath.c_str() : m_strMapPath.c_str();};
	const char*  GetModelPath(bool fullpath){return fullpath?m_strModelFullPath.c_str() : m_strModelPath.c_str();};
	const char*  GetTexturePath(bool fullpath){return fullpath?m_strTexturePath.c_str() : m_strTexturePath.c_str();};

	void         SetTileTexturePath( const char* strPath ){ m_strTileTexturePath = strPath; };
	void         SetTileTextureMayPath( const char* strPath ){  }

	void         SetMapPath( const char* strPath ){ m_strMapPath = strPath ;};
	void         SetModelPath( const char* strPath ){ m_strModelPath = strPath; };
	
	void         SetTexturePath( const char* strPath ){ m_strTexturePath = strPath ;};

	void         OpenDirDlg(int mode);
private:
    //地形笔刷纹理路径
	std::string                    m_strTileTexturePath;
	std::string                    m_strTileTextureFullPath;

	//地图存放路径
	std::string                    m_strMapPath;
	std::string                    m_strMapFullPath;

	//模型存放路径
	std::string                    m_strModelPath;
	std::string                    m_strModelFullPath;

	//纹理对应路径
	std::string                    m_strTexturePath;
	std::string                    m_strTextureFullPath;
};


};

extern TE::CProjSetting  g_ProjSetting;