

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b�޴�ĲƸ������������о޴�İ�ȫ�У�Ȼ��ͻ����������޴�ĵ��������žͻ����и���ĲƸ���
;	task_add_option 1576 1 101
;	task_add_option 1577 1 102
;	task_add_option 1578 1 103
 ;	task_add_option 1579 1 104
	TASKCYCLE_ADD_OPTION 103 1 140
	TASK_STAT 12044 1 20
	if 1 = 0 and 20 = 2
	  	option 150 (����)ѧԺ��������ѯ��һ�¹��ڵع�����Ϣ
	endif
	TASK_STAT 12244 1 20
	if 1 = 0 and 20 = 2
	  	option 160 (����)����һ�����ϵ�ͼ�飬ϣ��������Ȿ���ܸ���һЩ��Ϣ
	endif
	talk
endproc 
proc 140
	TASKCYCLE_DO_OPTION 103 1
endproc

PROC 101
	task_do_option 4250 1
ENDPROC
PROC 102
	task_do_option 4241 1
ENDPROC
PROC 103
	task_do_option 4244 1
ENDPROC
PROC 104
	task_do_option 4245 1
ENDPROC

PROC 150
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b���ϵĵع����������ص�ѶϢ��һ�ж������������ǵ�̽Ѱ�ҵ���ʵ����Ϣ���һὫ���Ͻ���ѧԺ���ġ�
	option 250 #(����)�õ�
	TALK
ENDPROC
PROC 250
	GET_ITEM_NUM_INPAK 5300505 0 11
	if 11 < 1
		add_sys_msg_start
		add_sys_msg ������û�������Ʒ
		set_sys_msg_mode 4 0 0
		msg_info
	ENDIF
	if 11 >= 1
	dec_item_num_inpak 5300505 1 0
	task_fini 12044 1
	endif
ENDPROC

;proc 2
;	get_trig_task 1 2
;	if 2 = 4250
;		task_fini 4250 1
;	endif  
; 
;endproc
