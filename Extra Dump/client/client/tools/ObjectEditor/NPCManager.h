
#pragma once
#include "OgrePrerequisites.h"
#include "GameResDBTypes.h"

class CNPCOperator
{
public:
	CNPCOperator(void);
	~CNPCOperator(void);

	//static Ogre::DBFIELD_DESC_T m_descNpc[];
	static Ogre::ResTable* m_pNpcTable;
	static void InitDB();
	static void ReleaseDB();
	static int GetAllNpc(NPCDISPLAYINFO* pBuff, int size);
	static size_t GetNpcCount();
	//static CNPCInfo GetNpcByIndex(size_t index);
	static bool GetNpcById(NPCDISPLAYINFO* pBuff, unsigned int id);
	static bool AddNpc(NPCDISPLAYINFO* pNpc);
	static unsigned int CreateNpcId();
	//static CNPCInfo NewNpc();
	//static void SaveDB();

};

//class CNPCInfo
//{
//public:
//	enum
//	{
//		ID = 0,
//		Name,
//		ModAddr,
//		Scale,
//		Transparent,
//		SkinTex1,
//		SkinTex2,
//		SkinTex3,
//		SoundID,
//
//		Race,
//		Gender,
//
//		SkinColor,
//		FaceType,
//		HairStyle,
//		HairColor,
//		FacialHair,
//		FacialColor,
//
//		EqiupStart,
//		Head = EqiupStart,
//		Neck,
//		Shoulder,
//		Boots,
//		Belt,
//		Shirt,
//		Pants,
//		Chest,
//		Bracers,
//		Gloves,
//		HandRight,
//		HandLeft,
//		Cape,
//		Tabard,
//		Quiver,
//		EqiupEnd = Quiver
//
//	};
//
//	CNPCInfo(Ogre::DBRecord& record);
//	CNPCInfo(const CNPCInfo& npc);
//	CNPCInfo();
//
//	Ogre::DBRecord m_Record;
//
//	bool IsValid() { return m_Record.IsValid(); };
//	void FormatRecord(char* buf);
//	CNPCInfo& operator = (const CNPCInfo& npc);
//
//	unsigned int	GetID();
//	const char*		GetName();
//	const char*		GetModelAddr();
//	float			GetScale();
//	float			GetTransparent();
//	const char*		GetSkinTex1();
//	const char*		GetSkinTex2();
//	const char*		GetSkinTex3();
//	//void			GetAvatar(CAvatar* pOutAvatar);
//	bool			GetAttrib(int* attrib);
//
//	void SetID(unsigned int id);
//	void SetName(char* name);
//	void SetModelAddr(char* addr);
//	void SetScale(float scale);
//	void SetTransparent(float trans);
//	void SetSkinTex1(char* tex1);
//	void SetSkinTex2(char* tex2);
//	void SetSkinTex3(char* tex3);
//	bool SetAttrib(int* attrib);
//};
