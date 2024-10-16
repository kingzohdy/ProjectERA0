
#include "stdafx.h"
#include "NPCManager.h"
#include "DBManager.h"

#include "OgreResDB.h"

CNPCOperator::CNPCOperator(void)
{
}

CNPCOperator::~CNPCOperator(void)
{
}
//#define MAX_ATTRIB 23  //这个定义可能需要移到其他相关的地方
//#define AVATAR_START 9
//Ogre::DBFIELD_DESC_T CNPCOperator::m_descNpc[] = 
//{
//	{0,		Ogre::DBFIELD_UINT32,	0,true},//ID
//	{1,		 Ogre::DBFIELD_STRING,	4,false},//名字
//	{2,		 Ogre::DBFIELD_STRING,	8,false},//模型路径
//	{3,		Ogre::DBFIELD_FLOAT,		12,false},//放缩
//	{4,		Ogre::DBFIELD_FLOAT,		16,false},//透明度
//	{5,		 Ogre::DBFIELD_STRING,	20,false},//皮肤贴图1
//	{6,		 Ogre::DBFIELD_STRING,	24,false},//皮肤贴图2
//	{7,		 Ogre::DBFIELD_STRING,	28,false},//皮肤贴图3
//	{8,		Ogre::DBFIELD_UINT32,	32,false},//声音ID
//	
//	{9,		Ogre::DBFIELD_UINT32,	36,false},//种族
//	{10,	Ogre::DBFIELD_UINT32,	40,false},//性别
//	
//	{11,	Ogre::DBFIELD_UINT32,	44,false},//皮肤颜色
//	{12,	Ogre::DBFIELD_UINT32,	48,false},//脸型
//	{13,	Ogre::DBFIELD_UINT32,	52,false},//发型
//	{14,	Ogre::DBFIELD_UINT32,	56,false},//发色
//	{15,	Ogre::DBFIELD_UINT32,	60,false},//胡子
//	{16,	Ogre::DBFIELD_UINT32,	64,false},//胡子颜色
//
//	{17,	Ogre::DBFIELD_UINT32,	68,false},//头部
//	{18,	Ogre::DBFIELD_UINT32,	72,false},//颈部
//	{19,	Ogre::DBFIELD_UINT32,	76,false},//肩部
//	{20,	Ogre::DBFIELD_UINT32,	80,false},//脚
//	{21,	Ogre::DBFIELD_UINT32,	84,false},//腰部
//	{22,	Ogre::DBFIELD_UINT32,	88,false},//内衣
//	{23,	Ogre::DBFIELD_UINT32,	92,false},//腿部
//	{24,	Ogre::DBFIELD_UINT32,	96,false},//胸部
//	{25,	Ogre::DBFIELD_UINT32,	100,false},//背部
//	{26,	Ogre::DBFIELD_UINT32,	104,false},//手套
//	{27,	Ogre::DBFIELD_UINT32,	108,false},//右手
//	{28,	Ogre::DBFIELD_UINT32,	112,false},//左手
//	{29,	Ogre::DBFIELD_UINT32,	116,false},//披风
//	{30,	Ogre::DBFIELD_UINT32,	120,false},//袍子
//	{31,	Ogre::DBFIELD_UINT32,	124,false},//箭袋
//	{0,		Ogre::DBFIELD_UNKNOWN,	0,false},//箭袋
//};


//
//"HEAD", "NECK", "SHOULDER", "BOOTS", "BELT", 
//"SHIRT", "PANTS", "CHEST", "BRACERS", "GLOVES", "HAND RIGHT",
//"HAND LEFT", "CAPE", "TABARD", "QUIVER"

Ogre::ResTable* CNPCOperator::m_pNpcTable = NULL;

void CNPCOperator::InitDB()
{
	m_pNpcTable = CDataBase::GetInstance()->GetTable("NpcDisplayInfo"); //new Ogre::DBFile("data\\db\\client\\res_npc.csv", m_descNpc);

}

void CNPCOperator::ReleaseDB()
{
	//SAFE_DELETE(m_pNpcDB);
	m_pNpcTable = NULL;
}

size_t CNPCOperator::GetNpcCount()
{
	if(m_pNpcTable != NULL)
		return m_pNpcTable->FindRecord(NULL, 0, NULL);
	return 0;
}

//CNPCInfo CNPCOperator::GetNpcByIndex(size_t index)
//{
//	CNPCInfo npc(m_pNpcDB->GetIthRecord(index));
//	return npc;
//}

int CNPCOperator::GetAllNpc(NPCDISPLAYINFO* pBuff, int size)
{
	if(m_pNpcTable == NULL)
		return 0;

	if(m_pNpcTable->FindRecord(NULL, 0, NULL) <= 0)
		return 0;

	return m_pNpcTable->FindRecord(pBuff, size, NULL);
}

bool CNPCOperator::GetNpcById(NPCDISPLAYINFO* pbuff, unsigned int id)
{
	NPCDISPLAYINFO npc;
	npc.id = id;
	if(pbuff == NULL)
		return false;

	if(m_pNpcTable == NULL)
		return false;

	if(m_pNpcTable->FindRecord(pbuff, 1, &npc) <= 0)
		return false;

	return true;;
}

bool CNPCOperator::AddNpc(NPCDISPLAYINFO* pNpc)
{
	if(m_pNpcTable == NULL || pNpc == NULL)
		return false;

	if(m_pNpcTable->InsertRecord(pNpc) > 0)
		return true;

	if(m_pNpcTable->UpdateRecord(pNpc) > 0)
		return true;

	MessageBox(NULL, "物品信息写入失败", "错误", MB_OK);
	return false;
}

unsigned int CNPCOperator::CreateNpcId()
{
	if(m_pNpcTable == NULL)
		return 0;

	int count = m_pNpcTable->FindRecord(NULL, 0, NULL);
	NPCDISPLAYINFO* pNpcs = new NPCDISPLAYINFO[count];
	m_pNpcTable->FindRecord(pNpcs, count, NULL);

	int maxid = 0;
	for(int i = 0; i < count; i++)
	{
		if((int)pNpcs[i].id > maxid)
			maxid = pNpcs[i].id;
	}
	maxid++;
	delete pNpcs;
	return maxid;
}

//CNPCInfo CNPCOperator::NewNpc()
//{
//	CNPCInfo npc(m_pNpcDB->AddRecord());
//	return npc;
//}
//void CNPCOperator::SaveDB()
//{
//	//m_pNpcDB->Save();
//	FILE* file = fopen("data\\db\\client\\res_npc.csv","wt");
//	char buf[512] = {0};
//	for(int i = 0; i < (int)m_pNpcDB->GetNumRecord(); i++)
//	{
//		CNPCInfo npc(m_pNpcDB->GetIthRecord(i));
//		npc.FormatRecord(buf);
//		fputs(buf, file);
//		//fwrite(buf, 1, strlen(buf)+1, file);
//		fputs("\r\n", file);
//	}
//
//	fclose(file);
//
//}


// -------------------------- [9/19/2007]
//	CNPCInfo
// -------------------------- [9/19/2007]

//CNPCInfo::CNPCInfo(Ogre::DBRecord& record)
//: m_Record(record)
//{
//}
//
//CNPCInfo::CNPCInfo(const CNPCInfo& npc)
//: m_Record(npc.m_Record)
//{
//}
//
//CNPCInfo::CNPCInfo()
//: m_Record(NULL, NULL)
//{
//}
//
//void CNPCInfo::FormatRecord(char* buf)
//{
//	int iFieldCount = m_Record.pdb->GetNumField();
//	Ogre::DBFIELD_DESC_T* pDesc = CNPCOperator::m_descNpc;
//	buf[0] = 0;
//	char tmp[256] = {0};
//	for(int i = 0; i < iFieldCount; i++)
//	{
//		switch(pDesc[i].type)
//		{
//		case Ogre::DBFIELD_STRING:
//			sprintf(tmp, "%s", m_Record.GetString(i));
//			break;
//		case Ogre::DBFIELD_UINT32:
//			sprintf(tmp, "%d", m_Record.GetUInt(i));
//			break;
//		case Ogre::DBFIELD_FLOAT:
//			sprintf(tmp, "%f", m_Record.GetFloat(i));
//			break;
//
//		}
//		if(i != iFieldCount - 1)
//			strcat(tmp, ",");
//		strcat(buf, tmp);
//		
//	}
//
//}
//
//CNPCInfo& CNPCInfo::operator = (const CNPCInfo& npc)
//{
//	m_Record.pdb = npc.m_Record.pdb;
//	m_Record.precord = npc.m_Record.precord;
//	return *this;
//}
//
//unsigned int CNPCInfo::GetID()
//{
//	return m_Record.GetUInt(ID);
//}
//
//const char* CNPCInfo::GetName()
//{
//	return m_Record.GetString(Name);
//}
//
//const char* CNPCInfo::GetModelAddr()
//{
//	return m_Record.GetString(ModAddr);
//}
//
//float CNPCInfo::GetScale()
//{
//	return m_Record.GetFloat(Scale);
//}
//
//float CNPCInfo::GetTransparent()
//{
//	return m_Record.GetFloat(Transparent);
//}
//
//const char* CNPCInfo::GetSkinTex1()
//{
//	return m_Record.GetString(SkinTex1);
//}
//
//const char* CNPCInfo::GetSkinTex2()
//{
//	return m_Record.GetString(SkinTex2);
//}
//
//const char* CNPCInfo::GetSkinTex3()
//{
//	return m_Record.GetString(SkinTex3);
//}
//
////void CNPCInfo::GetAvatar(CAvatar* pOutAvatar)
////{
////	if(pOutAvatar == NULL)
////		return;
////
////	if(m_Record.GetString(ModAddr) != NULL && m_Record.GetString(ModAddr)[0] != 0)
////	{
////		pOutAvatar->SetIsAvatar(false);
////		pOutAvatar->SetResPath((char*)m_Record.GetString(ModAddr));
////	}
////	else
////	{
////		pOutAvatar->SetIsAvatar(true);
////		pOutAvatar->SetRace			(m_Record.GetUInt(Race));
////		pOutAvatar->SetGender		(m_Record.GetUInt(Gender));
////		pOutAvatar->SetSkinColor	(m_Record.GetUInt(SkinColor));
////		pOutAvatar->SetFaceType		(m_Record.GetUInt(FaceType));
////		pOutAvatar->SetHairStyle	(m_Record.GetUInt(HairStyle));
////		pOutAvatar->SetHairColor	(m_Record.GetUInt(HairColor));
////		pOutAvatar->SetFacialHair	(m_Record.GetUInt(FacialHair));
////		pOutAvatar->SetFacialColor	(m_Record.GetUInt(FacialColor));
////
////		for(int i = 0; i < EqiupEnd - EqiupStart; i++)
////		{
////			pOutAvatar->SetEquipment(i + 1, m_Record.GetUInt(i + EqiupStart));
////		}
////
////	}
////}
//
//bool CNPCInfo::GetAttrib(int* attrib)
//{
//	if(attrib == NULL)
//		return false;
//
//	if(m_Record.GetString(ModAddr) != NULL && m_Record.GetString(ModAddr)[0] != 0)
//		return false;
//
//	for(int i = 0; i < MAX_ATTRIB; i++)
//	{
//		attrib[i] = (int)m_Record.GetUInt(AVATAR_START + i);
//	}
//	return true;
//}
//
//void CNPCInfo::SetID(unsigned int id)
//{
//	m_Record.Set<unsigned int>(ID, id);
//}
//
//void CNPCInfo::SetName(char* name)
//{
//	m_Record.SetString(Name, name);
//}
//
//void CNPCInfo::SetModelAddr(char* addr)
//{
//	m_Record.SetString(ModAddr, addr);
//}
//
//void CNPCInfo::SetScale(float scale)
//{
//	m_Record.Set<float>(Scale, scale);
//}
//
//void CNPCInfo::SetTransparent(float trans)
//{
//	m_Record.Set<float>(Transparent, trans);
//}
//
//void CNPCInfo::SetSkinTex1(char* tex1)
//{
//	m_Record.SetString(SkinTex1, tex1);
//}
//
//void CNPCInfo::SetSkinTex2(char* tex2)
//{
//	m_Record.SetString(SkinTex2, tex2);
//}
//
//void CNPCInfo::SetSkinTex3(char* tex3)
//{
//	m_Record.SetString(SkinTex3, tex3);
//}
//
////void CNPCInfo::SetAvatar(CAvatar* pAvatar)
////{
////	if(pAvatar == NULL)
////		return;
////	if(pAvatar->GetIsAvatar() == false)
////	{
////		SetModelAddr(pAvatar->GetResPath());
////	}
////	else
////	{
////		SetModelAddr("");
////		m_Record.Set<unsigned int>(Race, pAvatar->GetRace());
////		m_Record.Set<unsigned int>(Gender, pAvatar->GetGender());
////		m_Record.Set<unsigned int>(SkinColor, pAvatar->GetSkinColor());
////		m_Record.Set<unsigned int>(FaceType, pAvatar->GetFaceType());
////		m_Record.Set<unsigned int>(HairStyle, pAvatar->GetHairStyle());
////		m_Record.Set<unsigned int>(HairColor, pAvatar->GetHairColor());
////		m_Record.Set<unsigned int>(FacialHair, pAvatar->GetFacialHair());
////		m_Record.Set<unsigned int>(FacialColor, pAvatar->GetFacialColor());
////
////		for(int i = 0; i < EqiupEnd - EqiupStart; i++)
////		{
////			m_Record.Set<unsigned int>(i + EqiupStart, pAvatar->GetEquipment(i + 1));
////		}
////	}
////}
//
//bool CNPCInfo::SetAttrib(int* attrib)
//{
//	if(attrib == NULL)
//		return true;
//
//	m_Record.SetString(ModAddr, "");
//
//	for(int i = 0; i < MAX_ATTRIB; i++)
//	{
//		m_Record.Set<unsigned int>(AVATAR_START + i, attrib[i]);
//	}
//	return true;
//
//}