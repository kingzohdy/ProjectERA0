;�������ս����    ,������,1
proc 1
	strvar pvaridx posx posy playercamp
	;�����Ӫ
	;GET_ATTACKER_CAMP #����ֵ #�����ߵ���Ӫ����
	
	GET_ATTACKER_CAMP 1 playercamp
	;����ܲ��ܷ�
	if 1 = 0 and playercamp != 1
		add_sys_msg_start
		add_sys_msg �췽˽�зɻ�,ֻ�к췽�����Ա����  
		set_sys_msg_mode 4 0 0
		msg_info
		return -1
	endif
	var pvaridx -1
	;battlestat30,ս���׶�|0��ʼ�׶� 1׼���׶� 2��ʼ�׶�
	var 30 0
	get_pworld_var 80 32 1 30
	if 30 != 2
		return
	endif
	;GET_MON_INFO #���ر�־ #����id #ʵ��id #Ѫ�� #��ͼid #��ͼʵ�� #����X #Y 
	get_mon_info 1 0 0 0 0 0 posx posy
	if posx = 10089 and posy = 13864
		var pvaridx 16
	endif
	if posx = 10412 and posy = 13694
		var pvaridx 17
	endif
	if posx = 9361 and posy = 14010
		var pvaridx 18
	endif
	if posx = 9019 and posy = 13978
		var pvaridx 19
	endif
	if posx = 17413 and posy = 22613
		var pvaridx 21
	endif
	if posx = 17727 and posy = 23315
		var pvaridx 22
	endif
	if posx = 17705 and posy = 23763
		var pvaridx 23
	endif
	if posx = 17167 and posy = 22373
		var pvaridx 24
	endif
	if posx = 21288 and posy = 11852
		var pvaridx 26
	endif
	if posx = 21766 and posy = 11250
		var pvaridx 27
	endif
	if posx = 22050 and posy = 11033
		var pvaridx 28
	endif
	if posx = 21150 and posy = 12301
		var pvaridx 29
	endif
	if pvaridx < 0
		return
	endif
	;monfly10,�������ѹ�����|������������
	var 10 @pvaridx
	set_mon_var 0 32 @10 1
	sub pvaridx 10
	mul pvaridx 4
	var 90 0
	get_pworld_var @pvaridx 32 1 90
	if 90 = 1
		autofly 127 1
		DEL_NPC 0 1
	else
		DIALOG 1
		add_talk $_npc$:
		add_talk #b#b��ս��˲Ϣ����ս���У��ٶ���������Ҫ�ģ���ʱ���һ������������������ʤ������ʱ���һ���˾��ܹ�����Ϊʤ��˫ɱ��ɱ����ɱ��
		add_talk #b#b
		add_talk #b#b#c186506�ú�����δ��ͨ����ͨ��������ѡ������·�ߣ�������·��һֱ��Чֱ��ս�������������ظ�����
		add_talk #b#b
		add_talk #b#b#c6d00d2С��ʾ���ú���ֻͨ��˳ʱ����һ��
		option 100 #����
		option 102 #��㿴�����ټ�
		talk
	endif
	
endproc
proc 100
	choose 1 101 102 ����һ���µĺ�����Ҫ����50 ��ң�ȷ�Ϲ��򣿹�����·��һֱ��Чֱ��ս�������������ظ�����
endproc
proc 101
	;monfly10,�������ѹ�����|������������
	var 10 0
	get_mon_var 0 32 1 10
	sub 10 10
	mul 10 4
	var 90 0
	get_pworld_var @10 32 1 90
	dec_money_test 0 50 60 2
	if 90 = 0 and 2 = 0
		dec_money 0 50 60 2
		var 90 1
		set_pworld_var @10 32 @90 1
		;��ģ��
		DEL_NPC 0 1
		get_mon_info 1 0 0 0 0 0 posx posy
		;NEW_CAMP_NPC 1 350100 @posx @posy 0 1 npc350100.mac
		NEW_NPC 1 350100 @posx @posy 0 npc350100.mac
		add_sys_msg_start
		add_sys_msg $_ClanName$��$_Player$�����˷�����  
		set_sys_msg_mode 4 0 0
		map_msg_info
		add_pworld_resource 0 1 5 1
	else
		if 2 != 0
			add_sys_msg_start
			add_sys_msg ��Ҳ���,�޷����д˲���   
			set_sys_msg_mode 4 0 0
			msg_info
		endif
	endif
	
;	call 1
endproc
proc 102
endproc
