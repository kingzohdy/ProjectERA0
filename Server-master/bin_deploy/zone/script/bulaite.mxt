

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b������Ƭ�����϶�������Щʲô�ɣ���Ƭ������ΪһȺ������ʵ��ũ�������С������һȺ������ë����ũ����ȫ�������ˡ���Ȼ�����������ң�

	
;�޸�
	task_add_option 2021 1 117

;;;;;;;
	task_add_option 1619 1 120

	

	TASK_STAT 12049 1 20
	if 1 = 0 and 20 = 2
	  	option 150 (����)����������ѫ���͸���
	endif

	TASK_STAT 12249 1 20
	if 1 = 0 and 20 = 2
	  	option 160 (����)�㿴һ�£�����������⣬��Ͷ��ô��
	endif
	talk
endproc 

PROC 117
	task_do_option 2021 1
ENDPROC
PROC 120
	task_do_option 1619 1
ENDPROC


PROC 150
    	DIALOG 1
   	ADD_TALK $_npc$:       
    	ADD_TALK  #b#b��ֻ��������Щ�����Ķ��ﲻҪ��Ϊ����Ĵ������ȥ��

	OPTION 250 #(����)��л��������һ�У�
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
	task_fini 12049 1
	endif
ENDPROC


proc 2
	get_trig_task 1 2 

	if 2 = 4246 
		task_fini 4246 1
	endif
	if 2 = 1619 
		task_fini 1619 1
	endif
endproc
