;��˹��


proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b�������������̫����Ŀ�����ˣ�ֻ���ĵ���ǰ�����棬ȴ���������������Щ������Ϊ����ʱ����������Ҳ����Ҳû�л����ʶ��Щ�漣��
	
	
	task_add_option 2025 1 100
	task_add_option 2026 1 101
	task_add_option 2027 1 102
	task_add_option 2028 1 103
	
	strvar Ret
	task_stat 2026 1 Ret
	get_task_var 2026 4 8 1 Loc3 
	if Ret = 2 and Loc3 = 0
		option 150 ��ʼ����
	endif	
	
	get_task_var 2028 4 8 1 Loc3 
	task_stat 2028 1 Ret
	if Ret = 2 and Loc3 = 0
		option 160 ��ʼ����
	endif	
	talk
	
endproc 

PROC 100
	task_do_option 2025 1
ENDPROC
PROC 101
	task_do_option 2026 1
ENDPROC
PROC 102
	task_do_option 2027 1
ENDPROC
PROC 103
	task_do_option 2028 1
ENDPROC
proc 150
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b���ݼ�����ʼ��$_player$�����׼�����ˣ�
		option 151 #׼������
		option 152 #���е���ţ���������Ϣһ����
	talk
endproc 
proc 151
	strvar Loc3
	get_task_var 2026 4 8 1 Loc3 
	var Loc3 1
	set_task_var 2026 4 8 @Loc3 1
	
	;��ʱ������
	strvar Tim
	get_task_var 2026 0 8 1 Tim
	var Tim 0
	set_task_var 2026 0 8 @Tim 1	
endproc 
 
proc 160
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b���ݼ�����ʼ��$_player$�����׼�����ˣ�
		option 161 #׼������
		option 152 #���е���ţ���������Ϣһ����
	talk
endproc 
proc 161
	strvar Loc3
	get_task_var 2028 4 8 1 Loc3 
	var Loc3 1
	set_task_var 2028 4 8 @Loc3 1
	
	;��ʱ������
	strvar Tim
	get_task_var 2028 0 8 1 Tim
	var Tim 0
	set_task_var 2028 0 8 @Tim 1	
endproc 
 
proc 152
	return
endproc

proc 2
	
	get_trig_task 1 2 
;	if 2 = 2001
;		task_fini 2001 1
;	endif
	if 2 = 2026
		DIALOG 1
		add_talk $_npc$:
		add_talk #b#b�������������̫����Ŀ�����ˣ�ֻ���ĵ���ǰ�����棬ȴ���������������Щ������Ϊ����ʱ����������Ҳ����Ҳû�л����ʶ��Щ�漣��
		option 150 ��ʼ����
		talk
	endif
	if 2 = 2028
		DIALOG 1
		add_talk $_npc$:
		add_talk #b#b�������������̫����Ŀ�����ˣ�ֻ���ĵ���ǰ�����棬ȴ���������������Щ������Ϊ����ʱ����������Ҳ����Ҳû�л����ʶ��Щ�漣��
		option 160 ��ʼ����
		talk
	endif
endproc

