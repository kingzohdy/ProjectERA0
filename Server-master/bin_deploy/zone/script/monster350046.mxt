;���޶�̬������
proc 2

	strvar x y
	GET_MON_INFO 0 0 0 0 0 0 x y
	effect 0 0 playEffct($@x$,$@y$,"effect\\entity\\9535_16.ent")



	add_sys_msg_start
	strvar camp
	GET_ATTACKER_CAMP 1 camp
	if camp = 1
		add_sys_msg ���޷����$_Player$ͻϮռ������ɫԭҰ����㡣
	elseif camp = 2
		add_sys_msg ɳĮ�����$_Player$ͻϮռ������ɫԭҰ����㡣
	else
		add_sys_msg δ֪�����$_Player$ͻϮռ������ɫԭҰ����㡣
	endif
	set_sys_msg_mode 3 0 0
	MAP_MSG_INFO
	
	SET_REVIVE_POS_CAMP 1 0 @camp
	NEW_DYN_MON 1 350046 1 16011 12511 0 0 0 2 @camp 0 0

	;SET_REVIVE_POS_CAMP #����ֵ #��������� #��Ӫ����
	



endproc
	