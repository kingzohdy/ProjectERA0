;;;
;l;
proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b�ˣ�$_player$����֪����ͦ���ģ������ٻ���ĺ����������Ϊ���ǵĸ��ݵء������ţ�������ļ��룬��������������������������Ÿϳ���Ƭ�ֵء�

		task_add_option 1810 1 100
		task_add_option 1811 1 101
		task_add_option 1812 1 102
		task_add_option 1813 1 103
		task_add_option 1815 1 104
		task_add_option 1816 1 105
		task_add_option 1824 1 106
		task_add_option 1825 1 107
	;	task_add_option 1830 1 108
		
		task_add_option 1850 1 109
		task_add_option 1851 1 110
		task_add_option 1858 1 111
		task_add_option 1859 1 112
		task_add_option 1860 1 113
		
	;	task_add_option 1835 1 114
		
		TASK_STAT 3567 1 20
		TASK_STAT 3613 1 21
		if 20 = 2 or 21 = 2
			option 150 ��������-����Э������
		endif
			
		
		
	talk

endproc
PROC 100
	task_do_option 1810 1
ENDPROC
PROC 101
	task_do_option 1811 1
ENDPROC
PROC 102
	task_do_option 1812 1
ENDPROC
PROC 103
	task_do_option 1813 1
ENDPROC
PROC 104
	task_do_option 1815 1
ENDPROC
PROC 105
	task_do_option 1816 1
ENDPROC
PROC 106
	task_do_option 1824 1
ENDPROC
PROC 107
	task_do_option 1825 1
ENDPROC
PROC 108
	task_do_option 1830 1
ENDPROC
PROC 109
	task_do_option 1850 1
ENDPROC
PROC 110
	task_do_option 1851 1
ENDPROC
PROC 111
	task_do_option 1858 1
ENDPROC
PROC 112
	task_do_option 1859 1
ENDPROC
PROC 113
	task_do_option 1860 1
ENDPROC
PROC 114
	task_do_option 1835 1
ENDPROC
PROC 150
	
	DIALOG 1
	ADD_TALK $_npc$       
	ADD_TALK #b#b������Щ������������ӥԶ���ӷ��ıض���������������ڴˡ�
	OPTION 250 #����Э������
	TALK
ENDPROC

PROC 250
	
	GET_ITEM_NUM_INPAK 5300533 0 11
	if 11 < 1
		add_sys_msg_start
		add_sys_msg ������û�������Ʒ��
		set_sys_msg_mode 1 0 0
		msg_info
	ENDIF
	if 11 >= 1
		dec_item_num_inpak 5300533 1 0
		
		TASK_STAT 3567 1 70
		TASK_STAT 3613 1 71
		if 70 = 2
			task_fini 3567 1
		endif
		if 71 = 2
			task_fini 3613 1
		endif
	
	endif
ENDPROC

proc 2
	get_trig_task 1 2
	if 2 = 1813 
		task_fini 1813 1
	endif
	if 2 = 1816 
		task_fini 1816 1
	endif
	if 2 = 1851 
		task_fini 1851 1
	endif
	if 2 = 1835
		task_fini 1835 1
	endif
	if 2 = 1825
		task_fini 1825 1
	endif
endproc

