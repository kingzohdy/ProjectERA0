;���Բر�ͼ2


proc 1
	strvar X Y
        GET_PLAYER_MAPINFO 0 0 0 X Y

        effect 2 0 playEffct($@X$,$@Y$,"effect\\entity\\9505_16.ent")

	add_sys_msg_start
	add_sys_msg �ѻ���´β����ʸ�
	set_sys_msg_mode 3 0 0
	msg_info
endproc



