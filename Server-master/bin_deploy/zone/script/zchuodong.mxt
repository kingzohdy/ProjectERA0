

proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b��ɽ���Ǹ���λ��ʿ��ϰ��ս������Э�������ĵط�����Ϻ�������и����ջ�
;	add_talk #b#b
;	add_talk #c6d00d2���ڡ�һ���������淨��#n
;	add_talk #b#b#c6d00d2��ɱ#cff0080��ɽ��#n#c6d00d2����BOSS����#cff0080ͬ��Э��#n#c6d00d2״̬��Ч����20������#cff0080ɱ�־�������300����#n#c6d00d2�����ҿ��Ժͻ�Ȼ���ӡ�������ӵĶ౶���鹲�棬���ԣ���õ�ȥ������#cff0080����������οռ�#n
	add_talk #b#b
	add_talk #c186506�ر���ʾ��
	add_talk #b#b#c186506���뿪�����ͬ��Э��״̬����ʧ
	add_talk #b#b#c186506��������Խ�࣬������õľ���Խ��
	add_talk #b#b#c186506#c186506���ѵȼ�Խ�ͣ������Ѷ�Խ��
	add_talk #b#b#c186506������ֻ����˵ȼ��йأ������ѵȼ��޹�


;;;;;;;;;;;;;;;;;;;;;;;;;;

	task_add_option 1111 1 114

	
	task_add_option 3321 1 116
	task_add_option 3322 1 117

	TASK_STAT 12020 1 20
	if 1 = 0 and 20 = 2
	  	option 150 (����)ҩˮ������
	endif
	TASK_STAT 12220 1 20
	if 1 = 0 and 20 = 2
	  	option 160 (����)���������ҩˮ
	endif
;;;;;;;;;;;;;	

;	OPTION 210 #(����)���������̵�
	OPTION 200 #������ɽ������
	OPTION 210 ���ڡ�һ���������淨����
	
	talk
endproc 
proc 201 
	RESET_PWORLD 101
endproc
proc 210
	dialog 1
;	add_talk $_npc$:
	add_talk ���ڡ�һ���������淨��
	add_talk #b#b
	add_talk #b#b#c6d00d2һ������ָ����ɽ�� + ���οռ�#n
	add_talk #b#b
	add_talk #b#b��ɽ����6����ӻ�ɱ����������#cff4e00ͬ��Э��#n״̬����״̬���Ժͻ�Ȼ���ӡ�������ӵĶ౶���鹲�档

	add_talk #b#b
	add_talk #b#b#cff4e00ͬ��Э��Ч����20����ɱ�־�������300��#n
	add_talk #b#b
	add_talk #b#b���οռ䣺����򵥡����еľ��鸱��
	add_talk #b#b
	add_talk #b#b#c186506�ر����ѣ��ڻ��ͬ��Э��״̬���뿪���飬ͬ��Э��״̬����ʧ#n
	
		option 1 ����
	
	talk
endproc

PROC 114
	task_do_option 1111 1
ENDPROC

PROC 116
	task_do_option 3321 1
ENDPROC
PROC 117
	task_do_option 3322 1
ENDPROC

PROC 150
    	DIALOG 1
   	ADD_TALK $_npc$:       
    	ADD_TALK  #b#b���лл���ҩ����������Ӧ�ÿ��Ժܿ�ָ������ˡ�

	OPTION 250 #(����)���ÿ�����
	TALK

ENDPROC
PROC 250
	GET_ITEM_NUM_INPAK 5007000 0 11
	if 11 < 1
		add_sys_msg_start
		add_sys_msg ������û�������Ʒ
		set_sys_msg_mode 4 0 0
		msg_info
	ENDIF
	if 11 >= 1
	dec_item_num_inpak 5007000 1 0
	task_fini 12020 1
	endif

ENDPROC

PROC 160

    	DIALOG 1
   	ADD_TALK $_npc$:       
    	ADD_TALK  #b#b��л������ã���ӭ��һ������ʱ��ǹػ����ˡ�

	OPTION 260 #(����)���ÿ�����
	TALK

ENDPROC
PROC 260
	GET_ITEM_NUM_INPAK 5007001 0 11
	if 11 < 1
		add_sys_msg_start
		add_sys_msg ������û�������Ʒ
		set_sys_msg_mode 4 0 0
		msg_info
	ENDIF
	if 11 >= 1
	dec_item_num_inpak 5007001 1 0
	task_fini 12220 1
	endif
ENDPROC

proc 200

;	GET_EVENT_STAT 411 3	
;	GET_EVENT_STAT 412 5		
;	if @411 = 1 or @412 = 1
;		TEAM_PWORLD_INFO 300 101 0 0
;		if 300 = 0
;		ENTER_PWORLD 101 0 0
;		OPEN_PWORLD_UI 101
		OPEN_PWORLD_UI 12
;		else
;			is_captain 11 0
;			if 11 = 0
;				open_pworld_ui 101 
;			else
;				add_sys_msg_start
;				add_sys_msg �㲻�Ƕӳ����˸�����Ҫ�ӳ��������룡
;				set_sys_msg_mode 4 0 0
;				msg_info 
;			endif
;		endif
;
;	else
;		add_sys_msg_start
;		add_sys_msg ���û�������޷����룡
;		set_sys_msg_mode 4 0 0
;		msg_info	
;	
;	endif
endproc














