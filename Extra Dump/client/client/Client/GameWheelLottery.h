#include <list>
#include "proto_comm.h"
#include "GameEngineTypes.h"

using namespace std;

//tolua_begin
struct RandomItemVector
{
	int Type[RAND_ITEM_NUM];                            	/*   ��������0-��Ǯ 1-���� 2-���� */
	int ItemID[RAND_ITEM_NUM];                          	/*   ���TypeΪ0��1,����ֶβ���Ҫ�� */
	int Num[RAND_ITEM_NUM];                             	/*   ��ʾ��Ǯ�������ߵ������� */
};

#define GOLD_RAFFLE_MAX_NUM_TOLUA GOLD_RAFFLE_MAX_NUM

//tolua_end

class GameWheelLottery//tolua_export
	:public NetMsgHandler
{//tolua_export
private:
	list<int>				m_lstIndex;
	int						m_resultIdx;

	//	�̳�vip�ĳ齱��
	int						m_vipIndex;
	int						m_vipResultIdx;
	int						m_vipTimes;
protected:
	GameWheelLottery();

public:
	~GameWheelLottery();

public:
	static GameWheelLottery & getSingleton();
	virtual int		HandleNetMsg(int msgid, const tagCSPkgBody& msgbody);

public:
	void		setResult(int result);
	//tolua_begin
	void		add(int time = 1);
	void		reset();
	void		pop();
	int			top();
	int			remain();
	void		start();
	void		get();
	RandomItemVector		getRandomItems(int idx);

	void		clearData();
	// ��ʼvip �齱
	void		vipStart( int type );
	void		vipGet( int type );
	int			getVipTimes();
	int			getVipIdx( int type );
	//tolua_end
};//tolua_export