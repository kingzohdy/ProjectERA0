

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b���Աߵľ�����ҹ����ڣ���˹̹����ʦ���Ǵ����������ҹ�ģ����Ѿ��Ǻܾ���ǰ�����ˣ��¸ҵ�ð���ߣ����ټ�����ֵܺ������ǽ�ȥЭ����˹̹����ʦ�Կ��ڰ��ɡ�
	add_talk #b#b���������ǿ���˹֮����������ȥ����ɽ����������ȥ���棬�����кܶ�ܶ�ϡ�п�ʯ�����и�ʽ������������⣬�����Ը�⣬�����Գ�ס���ǡ�
	

;20~30

	task_add_option 1420 1 104
	task_add_option 1421 1 105
	task_add_option 1499 1 106
	task_add_option 1418 1 107
	task_add_option 1416 1 108
	task_add_option 2241 1 109
	task_add_option 1419 1 110


	option 300 #������ҹ��
;	option 301 #������ҹ��

;	task_stat 1416 1 20
;	if 1 = 0 and 20 = 2
;		GET_ITEM_NUM_INPAK 5300291 1 30
;		if 30 < 1
;			option 400 ��ȡ����֮��
;		endif
;	endif
	
	
	talk
endproc 


PROC 104
	task_do_option 1420 1
ENDPROC
PROC 105
	task_do_option 1421 1
ENDPROC
PROC 106
	task_do_option 1499 1
ENDPROC
PROC 107
	task_do_option 1418 1
ENDPROC
PROC 108
	task_do_option 1416 1
ENDPROC
PROC 109
	task_do_option 2241 1
	TASK_FINI 2241 1

ENDPROC
PROC 110
	task_do_option 1419 1
ENDPROC


proc 300
;	enter_pworld 102 0 1
;		OPEN_PWORLD_UI 102
		OPEN_PWORLD_UI 1
	
endproc
proc 301

	RESET_PWORLD 102
		add_sys_msg_start
		add_sys_msg �������óɹ�
		set_sys_msg_mode 4 0 0
		msg_info
endproc
proc 400
	
    DIALOG 1
   	ADD_TALK $_npc$:       
    ADD_TALK #b#b������֮���ǿ�˹̹����ʦ�������������׽���㣿

	option 401 ���ǿ�˹̹����ʦ�������ġ�
	TALK
endproc
proc 401
    DIALOG 1
   	ADD_TALK $_npc$:       
    ADD_TALK ��˹̹����ʦ���ڵ������Σ�

	option 402 ��������Ϊ��ħ��������š�
	TALK
endproc
proc 402
    DIALOG 1
   	ADD_TALK $_npc$:       
    ADD_TALK ���ǿ�˹̹����ʦ���������ң����۷���ʲô�¶����ܽ�����֮��ժ��!

	option 403 ����֮�����������Ѷ�������š�
	TALK
endproc
proc 403
    DIALOG 1
   	ADD_TALK $_npc$:       
    ADD_TALK #c676464<��������Ȼժ�¾��ϵ�����֮�����ݵ�����ǰ>#n
	ADD_TALK ���պã���һ��Ҫ��ȿ�˹̹����ʦ���գ��һ���Զ�м��㣬���ѡ�


	option 404 #��������֮����˹̹����ʦһ����ƽ������
	TALK
endproc

proc 404

;	add_item_num_inpak 5300291 1 1 0
	add_sys_msg_start
	add_sys_msg ���������֮����
	set_sys_msg_mode 4 0 0
	msg_info
endproc

proc 2

	get_trig_task 1 2

	if 2 = 2241
		task_fini 2241 1
	endif
	if 2 = 1419
		task_fini 1419 1
	endif
endproc
