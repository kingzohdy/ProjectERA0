

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk  ��׬��Ǯô��
;50��

	
	task_add_option 4156 1 108
	task_add_option 4157 1 109
	
	TASK_STAT 12016 1 20
	if 1 = 0 and 20 = 2
	  	option 150 (����)����ѧԺ��������ţ���ϣ���õ���İ���
	endif

	TASK_STAT 12216 1 20
	if 1 = 0 and 20 = 2
	  	option 160 (����)�����Ҹ��������ʱ��ǵ�ģ��
	endif
	talk
endproc 

PROC 104
	task_do_option 2206 1
ENDPROC
PROC 105
	task_do_option 2207 1
ENDPROC

PROC 108
	task_do_option 4156 1
ENDPROC
PROC 109
	task_do_option 4157 1
ENDPROC




PROC 160
    	DIALOG 1
   	ADD_TALK $_npc$       
    	ADD_TALK  ʱ��ǵĽ��裬�����Ӵ�����⣬����Ҫһ�����ϣ������������Ϻú����뿴��

	OPTION 260 #(����)�õģ�
	TALK
ENDPROC
PROC 260
	GET_ITEM_NUM_INPAK 5300512 0 11
	if 11 < 1
		add_sys_msg_start
		add_sys_msg ������û�������Ʒ
		set_sys_msg_mode 4 0 0
		msg_info
	ENDIF
	if 11 >= 1
	dec_item_num_inpak 5300512 1 0
	task_fini 12216 1
	endif
ENDPROC
proc 2
	get_trig_task 1 2
	if 2 = 2007
		task_fini 2007 1
	endif


endproc