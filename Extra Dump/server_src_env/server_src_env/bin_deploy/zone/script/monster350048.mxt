;ɳĮ��̬������
proc 2

	strvar x y
	GET_MON_INFO 0 0 0 0 0 0 x y
	effect 0 0 playEffct($@x$,$@y$,"effect\\entity\\9535_16.ent")



	add_sys_msg_start
	strvar camp
	GET_ATTACKER_CAMP 1 camp
	if camp = 1
		add_sys_msg ���޷����$_Player$ͻϮռ����ɳĮ֮�Ǹ���㡣
	elseif camp = 2
		add_sys_msg ɳĮ�����$_Player$ͻϮռ����ɳĮ֮�Ǹ���㡣
	else
		add_sys_msg δ֪�����$_Player$ͻϮռ����ɳĮ֮�Ǹ���㡣
	endif
	set_sys_msg_mode 3 0 0
	MAP_MSG_INFO

	NEW_DYN_MON 1 350048 1 21111 6811 0 0 0 2 @camp 0 0
	SET_REVIVE_POS_CAMP 1 1 @camp



endproc
	