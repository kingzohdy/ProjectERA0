

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b������ʹ��һ�ѷ����Ľ�һ�������������ʹ�����ԡ���Щ�޷��������ƻ����¼���ͨ����������Ծ��ܽ��������ǧ��ǰ����ӡ�İ��ݶ�˹����������������������ѡ���

	task_add_option 1509 1 101
	task_add_option 1510 1 102
	task_add_option 1511 1 103
	task_add_option 1512 1 104

	task_add_option 1503 1 105
	task_add_option 1585 1 106

	
	
;	task_add_option 1528 1 107
;	task_add_option 1529 1 108
;	task_add_option 1530 1 109
	task_add_option 1531 1 110
;	task_add_option 1532 1 111
;	task_add_option 1533 1 112
;	task_add_option 1534 1 113
	
	
	task_add_option 1601 1 114
	
	strvar Ret
	TASK_STAT 3304 1 Ret
	if Ret = 2
		option 500 �����뻻����
	endif	
	

	talk
endproc 

proc 500
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b�����������Ǹ�����ļ��壬���ǵ�ħ������������˫�����������ʱ��ǽ��������ա������ҽ����ɣ�û���˱��Ҹ��˽Ⱒ���¼����ˡ�
	option 501 #����

	talk
endproc
proc 501
	strvar ItmNum Num Rad
	GET_ITEM_NUM_INPAK 5300770 1 ItmNum
	if ItmNum > 0
			
		GET_PACK_SPACE 0 Num
		if Num > 0
			DEC_ITEM_NUM_INPAK 5300770 1 1
			
		;	rand Rad 100
		;	if @Rad < 80
				ADD_ITEM_NUM_INPAK 5300771 1 0 0
		;	endif
		;	if @Rad >= 80 and @Rad < 95
		;		ADD_ITEM_NUM_INPAK 5300772 1 0 0
		;	endif
		;	if @Rad >= 95 and @Rad < 99
		;		ADD_ITEM_NUM_INPAK 5300773 1 0 0
		;	endif
		;	if @Rad = 99
		;		ADD_ITEM_NUM_INPAK 5300774 1 0 0
		;	endif
		else
			add_sys_msg_start
			add_sys_msg �����ռ䲻�㣡
			set_sys_msg_mode 1 0 0
			msg_info
		endif
	else
		add_sys_msg_start
		add_sys_msg ������û�п��Խ�������Ʒ��
		set_sys_msg_mode 1 0 0
		msg_info
	endif
endproc 



PROC 101
	task_do_option 1509 1
ENDPROC
PROC 102
	task_do_option 1510 1
ENDPROC
PROC 103
	task_do_option 1511 1
ENDPROC
PROC 104
	task_do_option 1512 1
ENDPROC

PROC 105
	task_do_option 1503 1
ENDPROC
PROC 106
	task_do_option 1585 1
ENDPROC
;PROC 107
;	task_do_option 1528 1
;ENDPROC
;PROC 108
;	task_do_option 1529 1
;ENDPROC
;PROC 109
;	task_do_option 1530 1
;ENDPROC
PROC 110
	task_do_option 1531 1
ENDPROC
;PROC 111
;	task_do_option 1532 1
;ENDPROC
;PROC 112
;	task_do_option 1533 1
;ENDPROC
;PROC 113
;	task_do_option 1534 1
;ENDPROC
PROC 114
	task_do_option 1601 1
ENDPROC



proc 2
	get_trig_task 1 2
	if 2 = 1512
		task_fini 1512 1
	endif
;	if 2 = 1522
;		task_fini 1522 1
;	endif
;	if 2 = 1533
;		task_fini 1533 1
;	endif
;	if 2 = 1532
	
;		ADD_ITEM_NUM_INPAK 5300443 1 1 0
;	endif
	if 2 = 1601
		task_fini 1601 1
	endif

endproc
