
proc 1

	DIALOG 1
	add_talk $_npc$:
	add_talk #b#bս��������ս����������
	add_talk #b#bʲô����Ҫ��������С�������ڣ� ������������Ҫ����ҽ�ƣ������ҵ����ֻش������ܱ�֤ʿ���ǿ���Ѹ��Ͷ��ս������������������
	
		
		OPTION 150 #��ʥ��ף����50���ң�
	talk
endproc



proc 150
	strvar HP MaxHP MP MaxMP
	GET_ID_PLAYER_INFO 0 2 HP
	GET_ID_PLAYER_INFO 0 9 MaxHP
	GET_ID_PLAYER_INFO 0 11 MP
	GET_ID_PLAYER_INFO 0 10 MaxMP
	if @HP < @MaxHP or @MP < @MaxMP 
		strvar ret
		DEC_MONEY_TEST 0 50 20 ret
		if ret = -1
			add_sys_msg_start
			add_sys_msg �������е����Ҳ��㣡
			set_sys_msg_mode 4 0 0
			msg_info	
			
			BUY_HETONG 1
				;���Ҳ����򿪿�ݹ���ʽ
			
		else
			strvar re
			DEC_MONEY 0 50 20 re
			if re = 0
				ADD_PLAYER_HP 0 999999 0
				ADD_PLAYER_MP 0 999999 0
			endif
		endif
	else
		add_sys_msg_start
		add_sys_msg ���״̬���ã�����Ҫ�ҵ�Ԯ����
		set_sys_msg_mode 4 0 0
		msg_info
	
	endif
endproc


 