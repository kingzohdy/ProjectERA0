
proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#bֻ�л���ǫ�����ģ������������ռ���Ϊ��Ʒ�����ɽ���׼�ʯ�ķ�ӡ����ͨ����̳�ĵ�·��
	;���ˣ�����һ��ǫ�����ģ�������ļ�Ʒ��Ȼ�����ջ�����Ķ���

	option 300 #�׼�����ռ�
	;�����������������ռ�

	talk
endproc


PROC 300
	GET_ITEM_NUM_INPAK 5300584 0 30
	if 30 >= 5
		DEC_ITEM_NUM_INPAK 5300584 5 1
		DEL_NPC 0 1
		get_pworld_var 0 32 1 60
		add 60 1
		add_sys_msg_start
		add_sys_msg ����׼�ʯ��ӡ $@60$/4
		;�׼����� $@60$/4
		set_sys_msg_mode 3 0 0
		msg_info
		set_pworld_var 0 32 @60 1
	else
		add_sys_msg_start
		add_sys_msg �׼���Ҫ5������ռ�
		;û���㹻�Ļռ�
		set_sys_msg_mode 4 0 0
		msg_info
	endif

ENDPROC
