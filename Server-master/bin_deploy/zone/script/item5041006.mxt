;���鴫���� ,����ʥ�ش�����
proc 1
	new_npc_private 1 1 333071 30 0 0 0 npcteamtransgate.mac
endproc
;���ɷ�ʹ�ô˵���
proc 2
	IS_PLAYER_IN_JAIL 1 0
	if 1 = 1
		set_script_int_ret -1
		add_sys_msg_start
		add_sys_msg ��ǰ״̬��,�޷�ʹ�ô˵���
		set_sys_msg_mode 3 0 0
		msg_info	
	endif
endproc
