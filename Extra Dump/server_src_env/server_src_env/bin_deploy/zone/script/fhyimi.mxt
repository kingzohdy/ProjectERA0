;����

proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b��������һ���˼Ҿ�Ҫ��ҥ�ԣ�����������֦�ӽ���֩���Ҫ������
	
		task_add_option 1910 1 100
		task_add_option 1911 1 101
		task_add_option 1912 1 102
		task_add_option 1913 1 103
		task_add_option 1914 1 104
		task_add_option 1915 1 105
		task_add_option 1916 1 106
		task_add_option 1917 1 107
		task_add_option 1918 1 108
		strvar Ret
		task_stat 1919 0 Ret
		if Ret = 2
			option 130 ѯ������
		endif
		
		strvar Ret num1
		GET_ITEM_NUM_INPAK 5300933 1 num1
		if num1 > 0
			TASK_STAT 3305 1 Ret
			if Ret = 2
				option 700 �����뻻����
			endif	
		endif
	
	talk
endproc
proc 130
	dialog 1
	add_talk $_npc$:
	add_talk #b#b��Ҳ��֪������˵ɶ���ѵ���������һƿ�����������������ƺ����Ǿƹ�Ҳ�������϶�ͯ��
	add_talk #b#b�Ҿ����������е��Ժ�������ͷһ�������š��ҿ��һ������������չ����ɡ�
	add_talk #b#b������������һ�ˣ�����Щ������߽�㣬�������а취��
		task_fini 1919 1
		option 150 #��֪����
	talk 
endproc
proc 150
	return
endproc 

PROC 100
	task_do_option 1910 1
ENDPROC
PROC 101
	task_do_option 1911 1
ENDPROC
PROC 102
	task_do_option 1912 1
ENDPROC
PROC 103
	task_do_option 1913 1
ENDPROC
PROC 104
	task_do_option 1914 1
ENDPROC
PROC 105
	task_do_option 1915 1
ENDPROC
PROC 106
	task_do_option 1916 1
ENDPROC
PROC 107
	task_do_option 1917 1
ENDPROC
PROC 108
	task_do_option 1918 1
ENDPROC
;proc 2
;	get_trig_task 1 2
;	if 2 = 1910
;		task_fini 1910 1
;	endif
;
;endproc


proc 700
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b�������������ģ��������������ģ������������������������������ˣ���Ŷ~~ʲô�����ˣ������Ǹ�ֵ���о��Ŀ��⡣
		option 701 #����

	talk
	
endproc
proc 701
	
	strvar ItmNum Num Rad
	GET_ITEM_NUM_INPAK 5300933 1 ItmNum
	if ItmNum > 0
			
		GET_PACK_SPACE 0 Num
		if Num > 0
			DEC_ITEM_NUM_INPAK 5300933 1 1
			
			ADD_ITEM_NUM_INPAK 5300931 1 0 0

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


