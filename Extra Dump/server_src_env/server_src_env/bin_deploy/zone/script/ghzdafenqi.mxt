

proc 1
	DIALOG 1

	add_talk $_npc$:
	add_talk #b#b�Ҿ���ΰ��Ļ��ң���ѧ�ң������ң���ѧ�ң����ּң�ҽѧ�ң�����ѧ�ң�����ѧ�ң����ܼң�����ǧ�谮��һ���......���ܴ�ʦ�����.....��������
	add_talk #b#b�����ô��������ô�����Ұɣ��ҿ��԰���ΰ����������Զ���������ΰ��ĳ��У���������Ľ������ߡ�����������������֮���԰��������⣡���������Ҹ��������ÿ���������Σ�Ҫ��Ҫ�����°ѱ�������˻����������
	;strvar Ret
	;IS_WORLD_CITY_OWNER Ret
	;if Ret = 1
		option 199 #�������
	;endif
	option 201 #�ر�
	
;	option 400 #����ˢ��
	
	talk
endproc 
proc 199
	PKG_IS_UNLOCK 10 20
	if 10 = 0 and 20 = 1
		choose 2 200 201 ������Ҫ֧��500000���ң��Ƿ������
	endif
endproc
proc 200
	PKG_IS_UNLOCK 10 20
	if 10 = 0 and 20 = 0
		return
	endif
	DEC_MONEY_TEST 0 500000 20 10
	if 10 = -1
		add_sys_msg_start
		add_sys_msg ���Ҳ��㣬����ʧ��
		set_sys_msg_mode 3 0 0
		msg_info
	else
		strvar Ret
		SET_WORLD_CITY_EFFIGY Ret 1000 26420 29945
		if Ret = 0
		
			DEC_MONEY 0 500000 20 1
			add_sys_msg_start
			add_sys_msg ΰ���ʱ��ǳ��������ĵ����Ѿ�������ɡ�
			set_sys_msg_mode 4 0 0
			msg_info
			
			strvar mon day
			GET_NOW_TIME mon day 0 0 0 0 0
			
			add_sys_msg_start
			add_sys_msg ΰ���[ʱ��ǳ���]$_player$�ĸ��˵�������$@mon$��$@day$����ʽ������ʱ�������λ��
			set_sys_msg_mode 1 0 0
			svr_msg_info
			
		else
		
			add_sys_msg_start
			add_sys_msg �����Ҹ�����ܣ������Ϊʱ��ǵĳ���֮���������Ұɡ�
			set_sys_msg_mode 4 0 0
			msg_info
		endif
	endif
endproc 
proc 201

endproc
proc 400
	strvar num x y ba monid
	set_call_var monid 283001
	set_call_var num 10
	set_call_var x 26500
	set_call_var y 29600
	set_call_var ba 100
	call 401
	set_call_var num 15
	call 401
endproc 
proc 401
	NEW_MON @monid @num @x @y @ba 0 0 0 0
endproc 





