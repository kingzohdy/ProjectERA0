proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b���������֮��,û��������׷Ѱ�Լ����ڵ�����,����Ǽ����Ը����
	option 11 #����ȥ�����!
;	option 13 #�ټ�
	talk
endproc

proc 11
	GET_PWORLD_VAR 0 32 1 10
	if @10 >= 5
	autofly 105 1
	;get_pworld_var 16 32 1 14
	;var 14 5
	;set_pworld_var 16 32 @14 1
	else
		ADD_SYS_MSG_START
		ADD_SYS_MSG ��Χ���кܶ�����ߣ���ս�粼����֮ǰ�������������ְ�
		set_sys_msg_mode 4 0 0
		MAP_MSG_INFO
	endif
endproc 

;proc 13
;	return
;endproc
