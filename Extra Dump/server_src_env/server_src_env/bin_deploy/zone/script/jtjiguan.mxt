;;;;;;;;;;;���ػ���

proc 1

	strvar Ret
	task_stat 1583 1 Ret
	if Ret = 2
		DIALOG 1
		add_talk $_npc$:
		add_talk #b#b#c676464<������صĶ�����������һ�����أ�������Ѱ��һЩ�ƽ���صķ�ʽ�����������и��ֱ�������ת>#n	
			option 151 ��ת
			option 300 #��ת
		talk
	endif
endproc 
proc 300
	add_sys_msg_start
	add_sys_msg ����û��˿���仯�������Ǵ򿪵ķ�ʽ���ԣ�	
	set_sys_msg_mode 3 0 0
	msg_info
endproc 
proc 151
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b#c676464<����������΢�ľ�ʯת�������죬��Ȼת˲���ţ�����ȷʵ������>#n
		option 152 ��ת
		option 301 #��ת
	talk
endproc
proc 301
	add_sys_msg_start
	add_sys_msg ���ػָ���ԭò�������Ǵ򿪵ķ�ʽ���ԣ�	
	set_sys_msg_mode 3 0 0
	msg_info
endproc
proc 152
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b#c676464<�����ﴫ����������������>#n
		option 302 #��ת
		option 153 ��ת
	talk

endproc
proc 302
	add_sys_msg_start
	add_sys_msg ���ػָ���ԭò�������Ǵ򿪵ķ�ʽ���ԣ�		
	set_sys_msg_mode 3 0 0
	msg_info
endproc 
proc 153
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b#c676464<����һ��ϸ΢�����촫�����·��Ǿ޴�ĳ���ת���ĳ���>#n
		option 302 #��ת
		option 154 #��ת
	talk
endproc

proc 154
;	task_fini 1583 1
	add_item_num_inpak 5300807 1 1 0
endproc











