proc 1
	dialog 1

	add_talk $_npc$:
	add_talk #b#b���˵�Ѫ���ܰ�������ҪΪ���Ǳ���ѩ�ޣ�
;	task_add_option 1335 1 101
	task_add_option 1336 1 102	
	task_add_option 1312 1 103
	task_add_option 1337 1 104
	
	strvar Ret
	task_stat 1337 1 Ret
	if Ret = 2
;		option 300 �Ƿ���Ҫ������һ�̣�
	endif
	
	TASK_STAT 1318 0 50	
	if 50 = 2
		GET_ITEM_NUM_INPAK 5300415 0 83
	    GET_ITEM_NUM_INPAK 5300416 0 84
		if 83 = 0 and 84 = 0
;			option 199 ��ȡ��Ѷ����
	endif
	endif
	talk
endproc

;proc 101
;    task_do_option 1335 1
;endproc
proc 102
    task_do_option 1336 1
endproc
proc 103
    task_do_option 1312 1
endproc
proc 104
    task_do_option 1337 1
endproc
proc 199
		dialog 1
		add_talk $_npc$:
		add_talk #b#b��������һЩ���ʷ��˿��ܻ��õ��Ķ�������������һ�����ɡ�����һ��Ҫ����������õ��м�ֵ�Ķ��������ԣ�����Ը����������ʹ��ʲô�̾ߡ�
		add_talk #b#b�����������⣬�����Э����������ѡ�ú��ʵ��̾�֮�����ǾͿ�ʼ��Ѷ��
		option 200 #��ȡ����
;		option 201 #��ȡ����
		
		talk
endproc 
proc 200
		GET_ITEM_NUM_INPAK 5300415 0 80
		if 80 < 1
			ADD_ITEM_NUM_INPAK 5300415 1 1 0
			add_sys_msg_start
			add_sys_msg ������һ�����ӣ��ؼ�ʱ��ʹ����Э�������Ѷ
			set_sys_msg_mode 3 0 0
			msg_info
		endif
endproc 
proc 201
		get_task_var 1318 3 8 1 13
		var 13 1
		set_task_var 1318 3 8 @13 1
	    GET_ITEM_NUM_INPAK 5300416 0 81
		if 81 < 1
			ADD_ITEM_NUM_INPAK 5300416 1 1 0
			add_sys_msg_start
			add_sys_msg ������һ�����ƣ��ؼ�ʱ��ʹ����Э�������Ѷ
			set_sys_msg_mode 3 0 0
			msg_info
		endif	
endproc 

proc 300
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b�ҿ���ֱ�ӽ����͵�Ŀ�ĵأ�������;����ͣӴ��
	
		option 301 #��ֱ�ӽ����͵�Ŀ�ĵذɣ�
		option 302 #�����������Լ���ȥ��
	talk

endproc 
proc 301
	NEW_OWNER_TMP_ITEM 1460061 13000 13300 0 0 0 302 1095 1 3600 1
endproc 
proc 302
	return
endproc 

proc 2
	get_trig_task 1 2
	if 2 = 1337
;		dialog 1
;		add_talk $_npc$:
;		add_talk #b#b���˵�Ѫ���ܰ�������ҪΪ���Ǳ���ѩ�ޣ�
		task_fini 1337 1
;		option 300 �Ƿ���Ҫ������һ�̣�
;		talk
		;;;Э��
	endif

endproc 
