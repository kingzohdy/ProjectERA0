;������
proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b��Ҫ�������ӺͿ��������𣬾������Ұɣ�	
	add_talk #b#bֻ��Ҫ�������ҾͿ���ʹ��Ĳ�԰��ø�����ֲ�����ֲ����
	add_talk #b#b��Ҫ����������أ�����Ĳ�԰��������
	
		option 100 ���Ѳ�԰
	    OPTION 110 #(����)��������
		OPTION 120 #��ѯ��԰��Ϣ
	talk
endproc 

proc 100
	home_kaiken 0
endproc

proc 110
	OPEN_NPC_SHOP
endproc

proc 120
	strvar Nleve Ntudi Nqian leve ret ret1 ret2 ret3
	home_info ret2 1 Nleve
	
	if ret2 = -1
		RETURN
	endif
	
	home_info ret3 2 Ntudi
	home_info ret 3 Nqian
	home_info ret1 5 leve
	
	DIALOG 1
	add_talk #b#b�˵صȼ���$@Nleve$
	add_talk #b#b�ѿ������أ�$@Ntudi$
	add_talk #b#b
	
	if ret = 0
		add_talk #b#b�´ο��ѻ��ѣ�$@Nqian$
	else
		add_talk #b#b#c005ebe�����ѱ�ȫ�����ѣ�#n
	endif
	
	if ret1 = 0
		add_talk #b#b�´ο�����Ҫ�˵صȼ���$@leve$
	else
	endif
	
		option 1 ����
	talk
endproc
