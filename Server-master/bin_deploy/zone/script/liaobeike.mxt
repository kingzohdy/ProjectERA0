

proc 1
	STRVAR playerid
	DIALOG 1
	add_talk $_npc$:
	add_talk  #b#bҲ���������ÿ�춼�����ܶ࿴����������������飬��һЩ���п��޵ľ�������ĳ�����ĳ���������ܸı����һ������Щ����ϢϢ��ص��˵�����Ҳ����֮�ı䡣

	task_add_option 1724 1 11
	task_add_option 1741 1 12
	task_add_option 1742 1 13
	task_add_option 1743 1 14
	task_add_option 1744 1 15
	task_add_option 1745 1 16
	task_add_option 1746 1 17
	task_add_option 1747 1 18
	
;	task_add_option 1801 1 100

	TASK_STAT 12065 1 20
	if 1 = 0 and 20 = 2
	  	option 150 (����)���Ǳ���������壬����û�з����������ֵ������壿
	endif
	TASK_STAT 12265 1 20
	if 1 = 0 and 20 = 2
	  	option 160 (����)����������Ҫ��һЩ���ϣ���Ը���æ�ռ�һЩô��
	endif
	talk
endproc 

PROC 11
	task_do_option 1724 1
ENDPROC
PROC 12
	task_do_option 1741 1
ENDPROC
PROC 13
	task_do_option 1742 1
ENDPROC
PROC 14
	task_do_option 1743 1
ENDPROC
PROC 15
	task_do_option 1744 1
ENDPROC
PROC 16
	task_do_option 1745 1
ENDPROC
PROC 17
	task_do_option 1746 1
ENDPROC
PROC 18
	task_do_option 1747 1
ENDPROC
proc 100
	task_do_option 1801 1
endproc 


PROC 150
    	DIALOG 1
   	ADD_TALK $_npc$       
    	ADD_TALK  �����Ѿ����ٵ���Ƭ���أ��ֲ�������һ������һ��������

	OPTION 250 #(����)֪���ˣ�
	TALK
ENDPROC
PROC 250
	GET_ITEM_NUM_INPAK 5300509 0 11
	if 11 < 1
		add_sys_msg_start
		add_sys_msg ������û�������Ʒ
		set_sys_msg_mode 4 0 0
		msg_info
	ENDIF
	if 11 >= 1
	dec_item_num_inpak 5300509 1 0
	task_fini 12065 1
	endif
ENDPROC

PROC 160
    	DIALOG 1
   	ADD_TALK $_npc$       
    	ADD_TALK  ��Σ�յĴ���֮�����ߣ�һֱ������ȥע����ʲô���������������������ʦҪ��Ļ��������һῼ��һ�¡�

	OPTION 260 #(����)лл���ˣ�
	TALK
ENDPROC
PROC 260
	GET_ITEM_NUM_INPAK 5300519 0 11
	if 11 < 1
		add_sys_msg_start
		add_sys_msg ������û�������Ʒ
		set_sys_msg_mode 4 0 0
		msg_info
	ENDIF
	if 11 >= 1
	dec_item_num_inpak 5300519 1 0
	task_fini 12265 1
	endif
ENDPROC
proc 2
	get_trig_task 1 2
	if 2 = 1747
		task_fini 1747 1
	endif
	if 2 = 1744
		effect 2 0 startGray()
		GET_PLAYER_ID 1 playerid
		TARGET_ADD_STATUS 1 @playerid 1018 1 120 1
	endif
	if 2 = 1801
		task_fini 1801 1
	endif
endproc
