
;С־
proc 1

	dialog 1
	add_talk $_npc$:
	add_talk #b#b�����С�����Ƕ��裬��Ȼ���ˣ�
	add_talk #b#b#046#046#046#046#046
	add_talk #b#b������һ��Ҫ�ҵ�һֻ�뺷�ĳ��Ӯ�ñ�������Ը�������
	add_talk #b#b��Ȼ�Ҳ������㵱�ҵĳ��ȥ�μӱ�������ֻ��ϣ�������͸����ﵰ���ң�������
		
	strvar ret
	task_stat 30015 1 ret
	if ret = 2
		option 150 #����Ҫʲô���ﵰ�أ�
	endif
	talk
endproc

proc 150
	dialog 3
	add_talk $_npc$:
	get_task_var 30015 0 8 1 10	
	if 10 = 0
		add_talk #b#b����Ҫһֻ#c186506������ﵰ#n�����Բ���
	endif
	if 10 = 1
		add_talk #b#b����Ҫһֻ#c186506Ұ����ﵰ#n�����Բ���
	endif
	if 10 = 2
		add_talk #b#b����Ҫһֻ#c186506�����˳��ﵰ#n�����Բ���
	endif
	if 10 = 3
		add_talk #b#b����Ҫһֻ#c186506����������ﵰ#n�����Բ���
	endif
	if 10 = 4
		add_talk #b#b����Ҫһֻ#c186506ͺ�ճ��ﵰ#n�����Բ���
	endif
	if 10 = 5
		add_talk #b#b����Ҫһֻ#c186506�Ĺ��������ﵰ#n�����Բ���
	endif
	if 10 = 6
		add_talk #b#b����Ҫһֻ#c186506���ݻ����ﵰ#n�����Բ���
	endif
	
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
	
	get_task_var 30015 0 8 1 10 
	if 10 = 0
		if itid = 5610100
			DEC_ITEM_NUM_BYIDX 5610100 1 @pos @ge 1
			task_fini 30015 1
			call 300
		else
			add_sys_msg_start
			add_sys_msg ��ȷ�������������ﵰ��������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if 10 = 1
		if itid = 5610101
			DEC_ITEM_NUM_BYIDX 5610101 1 @pos @ge 1
			task_fini 30015 1
			call 300
		else
			add_sys_msg_start
			add_sys_msg ��ȷ�������Ұ����ﵰ��������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if 10 = 2
		if itid = 5610102
			DEC_ITEM_NUM_BYIDX 5610102 1 @pos @ge 1
			task_fini 30015 1
			call 300
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������������˳��ﵰ��������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if 10 = 3
		if itid = 5611116
			DEC_ITEM_NUM_BYIDX 5611116 1 @pos @ge 1
			task_fini 30015 1
			call 300
		else
			add_sys_msg_start
			add_sys_msg ��ȷ�����������������ﵰ��������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if 10 = 4
		if itid = 5610104
			DEC_ITEM_NUM_BYIDX 5610104 1 @pos @ge 1
			task_fini 30015 1
			call 300
		else
			add_sys_msg_start
			add_sys_msg ��ȷ�������ͺ�ճ��ﵰ��������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if 10 = 5
		if itid = 5610106
			DEC_ITEM_NUM_BYIDX 5610106 1 @pos @ge 1
			task_fini 30015 1
			call 300
		else
			add_sys_msg_start
			add_sys_msg ��ȷ��������Ĺ��������ﵰ��������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if 10 = 6
		if itid = 5610107
			DEC_ITEM_NUM_BYIDX 5610107 1 @pos @ge 1
			task_fini 30015 1
			call 300
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǽ��ݻ����ﵰ��������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
endproc  

proc 300
	add_sys_msg_start
;	add_sys_msg �ɹ��Ͻ���Ʒ���������
	set_sys_msg_mode 1 0 0
	msg_info
	PLAYER_CLOSE_DIALOG 0 0
endproc 












