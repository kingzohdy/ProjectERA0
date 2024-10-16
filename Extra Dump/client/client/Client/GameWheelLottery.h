#include <list>
#include "proto_comm.h"
#include "GameEngineTypes.h"

using namespace std;

//tolua_begin
struct RandomItemVector
{
	int Type[RAND_ITEM_NUM];                            	/*   奖励类型0-金钱 1-布尔 2-道具 */
	int ItemID[RAND_ITEM_NUM];                          	/*   如果Type为0或1,这个字段不需要填 */
	int Num[RAND_ITEM_NUM];                             	/*   表示金钱数量或者道具数量 */
};

#define GOLD_RAFFLE_MAX_NUM_TOLUA GOLD_RAFFLE_MAX_NUM

//tolua_end

class GameWheelLottery//tolua_export
	:public NetMsgHandler
{//tolua_export
private:
	list<int>				m_lstIndex;
	int						m_resultIdx;

	//	商城vip的抽奖，
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
	// 开始vip 抽奖
	void		vipStart( int type );
	void		vipGet( int type );
	int			getVipTimes();
	int			getVipIdx( int type );
	//tolua_end
};//tolua_export