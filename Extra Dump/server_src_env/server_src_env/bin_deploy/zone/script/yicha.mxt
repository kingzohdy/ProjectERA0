

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b˭�����ҵ�������������ص���Դ��˭���ܹ��ɾ�һ����ҵ��
	add_talk #b#b
	add_talk #c186506ע�⣺��������������������һ�Ρ�
	add_talk #c186506ע�⣺��ȡ����󣬽�����ʹ�á��Һ�æ������������ɡ�#n
	
	task_add_option 1747 1 11
	task_add_option 1770 1 13
	TASKCYCLE_ADD_OPTION 107 1 12


	TASK_STAT 12067 1 20
	if 1 = 0 and 20 = 2
	  	option 150 (����)���Ǳ���������壬����û�з����������ֵ������壿
	endif

	TASK_STAT 12267 1 20
	if 1 = 0 and 20 = 2
	  	option 160 (����)����������Ҫ��һЩ���ϣ���Ը���æ�ռ�һЩô��
	endif
	
	strvar lv
	GET_PLAYER_BASE_ATT 0 0 lv 1
	
	if lv >= 55 and lv < 65
		option 230 ��Ҫ������ɡ����顿Խ������
	endif
	
	talk
endproc 
PROC 11
	task_do_option 1747 1
ENDPROC
PROC 12
	TASKCYCLE_DO_OPTION 107 1
ENDPROC
PROC 13
	task_do_option 1770 1
ENDPROC

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
	task_fini 12067 1
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
	task_fini 12267 1
	endif
ENDPROC
;proc 2
;	get_trig_task 1 2
;	if 2 = 1770
;		task_fini 1770 1
;	endif    


;endproc
proc 230
	strvar lv
	GET_PLAYER_BASE_ATT 0 0 lv 1
	if lv >= 55 and lv < 65
		PLAYER_BUSY_UI 18 1
	endif
endproc