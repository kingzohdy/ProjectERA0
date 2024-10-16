class AutoTalkNpc
{
protected:
	AutoTalkNpc();
	static AutoTalkNpc *m_instant;

public:
	static AutoTalkNpc * getInstant();
	virtual int	 talkByResId( int npcResId ) = 0;
	virtual int	 talkByMemId( int npcMemId ) = 0;
};
