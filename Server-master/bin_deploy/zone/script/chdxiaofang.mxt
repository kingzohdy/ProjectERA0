
;С��
proc 1

	dialog 1
	add_talk $_npc$:
	add_talk #b#b5555��#060#060#060����λ#Gender3#��Ϊʲô��ֻ���ô��ѽ��Ϊʲô��������ѽ��Ϊʲôû��Ը������ѽ��Ϊʲô#060#060#060��Ϊʲô#060#060#060��Ϊʲôѽ #060#060#060��
	add_talk #b#b�������Ԫ���ˣ������������������ӣ���Ը������ô��
	
	strvar ret
	task_stat 30014 1 ret
	if ret = 2
		option 150 #����֮�ͣ�
	endif
	talk
endproc
proc 150
	dialog 3
	add_talk $_npc$:
	
	get_task_var 30014 0 8 1 10 
	if 10 = 0
		add_talk #b#b��˵���������������ܹ�˲��ظ�������ҩ������ʲô#c186506��ͨ��������#n�����ܸ���һ����
	endif
	if 10 = 1
		add_talk #b#b��˵�������������ܹ�˲��ظ�ħ����ҩ������ʲô#c186506��ͨħ��ҩƬ#n�����ܸ���һ����
	endif
	if 10 = 2
		add_talk #b#b��˵������������������#c186506��ʱ��ǵĴ��;���#n�����ܸ���һ����
	endif
	
	add_talk #b#b
	add_talk #b#b����Ҫ�ύ����Ʒ�����·��Ŀ���
		
		NPC_COMMIT_OPT 160
	talk
endproc 

proc 160
	strvar itid pos ge
	GET_COMMIT_ITEM_INFO 1 itid
	GET_COMMIT_ITEM_INFO 2 pos
	;λ��
	GET_COMMIT_ITEM_INFO 3 ge
	;����
	
	get_task_var 30014 0 8 1 10 
	if 10 = 0
		if itid = 5010101
			DEC_ITEM_NUM_BYIDX 5010101 1 @pos @ge 1
			task_fini 30014 1
			call 300
		else
			add_sys_msg_start
			add_sys_msg ǰ�������������ģ��˼���Ҫ��ͨ��������
			set_sys_msg_mode 3 0 0
			msg_info
			
		endif
	endif
	if 10 = 1
		if itid = 5011101
			DEC_ITEM_NUM_BYIDX 5011101 1 @pos @ge 1
			task_fini 30014 1
			call 300
		else
			add_sys_msg_start
			add_sys_msg ǰ�������������ģ��˼���Ҫ��ͨħ��ҩƬ
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if 10 = 2
		if itid = 5041000
			DEC_ITEM_NUM_BYIDX 5041000 1 @pos @ge 1
			task_fini 30014 1
			call 300
		else
			add_sys_msg_start
			add_sys_msg ǰ�������������ģ��˼���Ҫ��ʱ��ǵĴ��;���
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif

endproc 

proc 300
	add_sys_msg_start
;	add_sys_msg �ɹ��Ͻ���Ʒ���������
	set_sys_msg_mode 3 0 0
	msg_info
	PLAYER_CLOSE_DIALOG 0 0
endproc 












