

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b����ʲô����ˣ�������Щ�ֲ��Ĺ�����ʧ�ɣ���ֻ��ϣ�����Ժúõĸ����ҵĵذ���
	

;20~30
	
	task_add_option 1494 1 110
	task_add_option 1495 1 111
	task_add_option 1496 1 112
	
	

	TASK_STAT 12025 1 20
	if 1 = 0 and 20 = 2
	  	option 150 (����)ѧԺ����������̽һ��ѩԭ�����
	endif

	TASK_STAT 12225 1 20
	if 1 = 0 and 20 = 2
	  	option 160 (����)�Ҵ�����Э���飬������һ��
	endif
	
	talk
endproc 


PROC 110
	task_do_option 1494 1
ENDPROC
PROC 111
	task_do_option 1495 1
ENDPROC
PROC 112
	task_do_option 1496 1
ENDPROC



proc 160
    	DIALOG 1
   	ADD_TALK $_npc$:       
    	ADD_TALK #b#b�����֪�������������ϵ���Ϣ����һ����֪ͨ������ʦ���µġ�

	OPTION 260 #(����)лл���֧�֣�
	TALK
endproc
PROC 260
	
	GET_ITEM_NUM_INPAK 5300513 0 11
	if 11 < 1
		add_sys_msg_start
		add_sys_msg ������û�������Ʒ
		set_sys_msg_mode 4 0 0
		msg_info
	ENDIF
	if 11 >= 1
	dec_item_num_inpak 5300513 1 0
	task_fini 12225 1
	endif
ENDPROC
proc 2
	get_trig_task 1 2

	if 2 = 1495
		task_fini 1495 1
	endif 
	if 2 = 1496
;			add_item_num_inpak 5300335 5 1 0
	endif 

endproc
