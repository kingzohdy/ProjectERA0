;ѱ���� 
; Я������\��������

proc 1

	dialog 1
	add_talk $_npc$:
	add_talk #b#b����Ҫѧ����ϧ���������ÿһ�죬��Ϊ����ÿһ��Ŀ�ʼ���������������������еĵ�һ�졣�������Ǽ�����ȥ����Զ��Ҫ���������������족��������Ϊ��˭Ҳû��ȷ�������죬�������ᷢ��ʲô���㣬�ң������Ƿ���������㰲Ȼ����
	task_add_option 1060 1 101	

	task_add_option 1065 1 104
	task_add_option 1064 1 105




	TASK_STAT 1060 1 20
	if 20 = 2 
		OPTION 110 #���������
	endif
	if 20 = 3
		option 111 ���������
	
	endif

	talk
endproc
proc 111
	dialog 1
	add_talk $_npc$:
	add_talk #b#b������ĸо��ܺ�Ŷ��
		option 112 #���������
		option 113 #ȡ��
	talk
endproc

proc 101
    task_do_option 1060 1
endproc

proc 104
    task_do_option 1065 1
endproc
proc 105
    task_do_option 1064 1
endproc

proc 110
	autofly 1 1
	task_fini 1060 41
endproc
proc 112
    	autofly 1 1
endproc

proc 2
	get_trig_task 1 2
	if 2 = 1060
		dialog 1
		add_talk $_npc$:
		add_talk #b#b����Ҫѧ����ϧ���������ÿһ�죬��Ϊ����ÿһ��Ŀ�ʼ���������������������еĵ�һ�졣�������Ǽ�����ȥ����Զ��Ҫ���������������족��������Ϊ��˭Ҳû��ȷ�������죬�������ᷢ��ʲô���㣬�ң������Ƿ���������㰲Ȼ����
	
		OPTION 110 #���������
		talk
	endif
	if 2 = 1065
		
	;	NEW_OWNER_TMP_ITEM 1460064 16300 16900 0 0 0 305 1095 1 3600 1
	endif
endproc

