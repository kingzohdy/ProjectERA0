;����˹

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b��ӭ�������Ѿƹݣ������ˮ��ۣ�С���ⵥ��$_player$��Ҫ��Ҫ������Ϣһ����أ�

	TASKCYCLE_ADD_OPTION 105 1 140

	TASK_STAT 12051 1 20
	if 1 = 0 and 20 = 2
	  	option 150 �͸�������������ѫ��
	endif
	TASK_STAT 12251 1 20
	if 1 = 0 and 20 = 2
	  	option 160 �㿴һ�£�����������⣬��Ͷ��ô��
	endif
	talk
endproc 
proc 140
	TASKCYCLE_DO_OPTION 105 1
endproc 

PROC 150
    	DIALOG 1
   	ADD_TALK $_npc$:       
    	ADD_TALK  #b#b��ֻ��������Щ�����Ķ��ﲻҪ��Ϊ����Ĵ������ȥ��

	OPTION 250 #֪���ˣ�
	TALK
ENDPROC

PROC 250
	GET_ITEM_NUM_INPAK 5300507 0 11
	if 11 < 1
		add_sys_msg_start
		add_sys_msg ������û�������Ʒ
		set_sys_msg_mode 4 0 0
		msg_info
	ENDIF
	if 11 >= 1
	dec_item_num_inpak 5300507 1 0
	task_fini 12051 1
	endif
ENDPROC

;proc 2
;	get_trig_task 1 2 
;	if 2 = 4345
;		task_fini 4345 1
;	endif
;
;endproc

