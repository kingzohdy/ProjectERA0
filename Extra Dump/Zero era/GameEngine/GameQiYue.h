#pragma once
#include "proto_comm.h"
//tolua_begin
#pragma pack(1)
struct tagQiYueInfo_ToLua
{
	unsigned int CurrQiYueID;                     	/*   ��ǰʹ�õ���Լ */
	unsigned int QiYueNum;                        	/*   �Ѿ�ѧ������Լ */
	unsigned int QiYueMax;                        	/*   �����Լ���� */
	unsigned int QiYueList[QIYUE_MAX_NUM];        	/*   ��ѧ��Լ���б� */
};

#pragma pack()
//tolua_end

struct tagQiYueInfo;
class GameQiYue//tolua_export
{//tolua_export
public:
	GameQiYue(void);
	~GameQiYue(void);
	void	setQiYueList( const tagQiYueInfo& qiYueList );

	//tolua_begin
	const tagQiYueInfo_ToLua&	getQiYueList() const;
	const tagQiYueInfo_ToLua&	getQiYueList();
	//tolua_end

private:
	tagQiYueInfo_ToLua	m_qiYueList;
};//tolua_export
