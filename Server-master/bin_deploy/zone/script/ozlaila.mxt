;����˹����

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b��ų�����˵���꣬ϣ����������������ɡ�ǿ���Ծȣ�ʥ�߾��ˣ�����������
	

;20~30�޸�

	task_add_option 1410 1 113
	task_add_option 1411 1 114
	task_add_option 1412 1 115
	task_add_option 1413 1 116
	task_add_option 1414 1 117
	task_add_option 1415 1 118
;	task_add_option 1418 1 119
;	task_add_option 1419 1 120
	task_add_option 1437 1 121
	task_add_option 1427 1 122


	talk
endproc 

PROC 113
	task_do_option 1410 1
ENDPROC
PROC 114
	task_do_option 1411 1
ENDPROC
PROC 115
	task_do_option 1412 1
ENDPROC
PROC 116
	task_do_option 1413 1
ENDPROC
PROC 117
	task_do_option 1414 1
ENDPROC
PROC 118
	task_do_option 1415 1
ENDPROC
;PROC 119
;	task_do_option 1418 1
;ENDPROC
;PROC 120
;	task_do_option 1419 1
;ENDPROC
PROC 121
	task_do_option 1437 1
ENDPROC
PROC 122
	task_do_option 1427 1
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
	task_fini 12227 1
	endif
ENDPROC
proc 2
	get_trig_task 1 2

	if 2 = 1415 
		task_fini 1415 1
	endif 
	if 2 = 1427
		task_fini 1427 1
	endif

endproc
