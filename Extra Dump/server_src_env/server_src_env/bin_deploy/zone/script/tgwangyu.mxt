proc 1

	dialog 1
	add_talk $_npc$:
	add_talk #b#b���µ���Ӱ�ս�����ʧ�������ڰ�Ҳ����ʧ��ո�µ�һ�콫�����٣�̫��Ҳ��ɢ���������Ĺ�â��
;	task_add_option 1311 1 101	
;	task_add_option 1312 1 102	
	task_add_option 1327 1 103
	task_add_option 1401 1 104
	
	task_add_option 3004 1 105
;����

	task_add_option 3030 1 106
	
	strvar Loc
	task_stat 3030 1 30
	get_task_var 3030 1 8 1 Loc 

	if 30 = 2 and Loc = 0
		option 280 �������ǰ��ʱ���
	endif
;	strvar ret
;	task_stat 3004 1 ret
;	if ret = 2
;		option 200 #����Ҫ��������ҹ�ȥ�ɣ�
;	endif
	talk
endproc
;proc 101
;    task_do_option 1311 1
;endproc
;proc 102
;    task_do_option 1312 1
;endproc
proc 103
    task_do_option 1327 1
endproc
proc 104
    task_do_option 1401 1
endproc
proc 105
   task_do_option 3004 1
endproc

proc 106
    task_do_option 3030 1
endproc
proc 300
	NEW_OWNER_TMP_ITEM 1460049 9800 8800 0 0 0 184 1095 1 3600 1
	strvar Loc
	get_task_var 3030 1 8 Loc 1
	var Loc 1
	set_task_var 3030 1 8 @Loc 1
endproc
proc 301
	return
endproc
proc 2
	get_trig_task 1 2
;	if 2 = 1312 
;		task_fini 1312 1
;	endif

	if 2 = 3110
		task_fini 3110 1
	endif
	if 2 = 1401
		task_fini 1401 1
	endif
	if 2 = 3030
	dialog 1
	add_talk $_npc$:
	add_talk #b#b���µ���Ӱ�ս�����ʧ�������ڰ�Ҳ����ʧ��ո�µ�һ�콫�����٣�̫��Ҳ��ɢ���������Ĺ�â��
		
		option 280 �������ǰ��ʱ���
		
	talk
	endif
;	if 2 = 3004
;		dialog 1
;		add_talk $_npc$:
;		add_talk #b#bͭ�ĵĹ�����ħ����µ�����������
;			option 200 #����Ҫ��������ҹ�ȥ�ɣ�
;		talk
;		
;	endif
endproc
proc 200
	AUTO_MOVE 20 1000 24200 13200 1460049 0 3081 1 3600
endproc 
proc 280
	dialog 1
	add_talk $_npc$:
	add_talk #b#b���ػ�������ʱ��ǵ��������˰�ŵ�´�����ɼ����򵥵��������ͻ�ӵ��һƥ�����Լ���#c186506����#n��
	add_talk #b#bͬʱ���㻹������ʱ��ǵġ����򡱴�����#c186506����#nϵ����ص�����
	option 300 #��������������������ȥʱ��ǰ�
	option 301 #����ϸ����������񣬵ȵ���ȥ
		
	talk
endproc 

