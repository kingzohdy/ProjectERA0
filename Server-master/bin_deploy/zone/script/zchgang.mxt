

proc 1

	dialog 1
	add_talk $_npc$:
	add_talk #b#b��������,����Ϊ��ɱ�˵��ֶθ���,���ǽ�������,����Ϊ���ǻ�����ǿ!
	add_talk #b#b���ҵĵ���,�͵����ҵ�! 
	add_talk #b#bɱ��ֵ�ﵽ30��,����200��,�������,��ôһ��ɱ��ֵ��û�ʸ���������.��Ȼ,����СǮ,��Ҳ���������￪���۽�.����״̬,�����ǲ�����侭���,���ǿ�������,�������������һ�������ܰ�!
	add_talk #b#bɱ��ֵ�ﵽ200����,��ϲ��,���Ѿ���ķ�����.��ĸ���������ǿ�ߺ������ļ���Ӫ,ɱ�˶�ħ�ͳ嶯�ߵľۼ���.ɱ��ֵ���ߵ�ʱ��,�����������ɱ¾��״̬,��ʽ����,���Ŵ�,����������,��ͻ����������Һ�����в!

	add_talk #b#bɱ��ֵ�ﵽ200����,����������ļ�,ʹ�ûسǾ���,������д��;������,ͳͳ���ᴫ�͵�������!

	;��һ�
	task_stat 4435 1 51
	if 51 = 2
		option 510 ����ο�ʡ�����������
	endif

	option 100 #���̵�
		
	option 101 #�ص�ʱ���
		
	talk
endproc
proc 100
	OPEN_NPC_SHOP 
	
endproc
proc 101
	strvar Ret
	GET_PLAYER_PKVAL_TYPE 0 Ret
	if Ret != 3
		TRANSMIT 1000 26100 28800 0
	else
		add_sys_msg_start
		add_sys_msg ���ɱ��̫���ˣ���ʱ��������ص�ʱ��ǡ�
		set_sys_msg_mode 1 0 0
		msg_info
	endif
endproc 

;��һ�
proc 510
	GET_ITEM_NUM_INPAK 5301004 1 10
	STRVAR flag
	var flag 0
	if 10 > 0
		DEC_ITEM_NUM_INPAK 5301004 1 11
		if 11 = 0
			var flag 1
			task_fini 4435 1
			
			DIALOG 1
			add_talk $_npc$:
			add_talk #b#bлл�㣬$_player$���뽫�ҵ�ף���������ͣ�˳������лл����
			add_talk #b#b�������ҡ�����������˷���֮ǰ���������黻���Ҹ�Ƶ�����������ʱ���������ʱ��ͣ�������మ���װ��ģ���һ���֣�
			option 511 #û����
			talk
		endif
	endif
	
	if flag = 0
		add_sys_msg_start
		add_sys_msg ���ı�����û�С���Ʒ�С���
		set_sys_msg_mode 1 0 0
		msg_info
		PLAYER_CLOSE_DIALOG1 0 0
	endif
endproc
