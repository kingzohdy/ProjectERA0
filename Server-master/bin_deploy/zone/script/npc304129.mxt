;npc���ײ�

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b�����������������������죬�ҽ���ÿ�ܶ������ġ������ٰ�һ��������ʿ���ܹ�����ġ����������������ǲ�����˶��ܻ�ý����������ܹ��������ǰʮ�������ܻ�÷ǳ����Ľ�����
    add_talk #b#b
	add_talk #b#b�������������ߵģ�

	;�����ȡʱ�����ָ�ѡ��
	task_stat 4500 1 20
	if 20 = 2
		option 101 #����һ�Ҵ�����ҪȥѰ���´�½��
	endif
	
	option 201 #�鿴��һ�캽���������
	option 202 ������ཱ��˵��
		
		TASK_ADD_OPTION 4500 0 100
		
	talk
endproc 


proc 2
	get_trig_task 1 2

	if 2 = 4500 
		DIALOG 1
		add_talk $_npc$:
		add_talk #b#b�����������������������죬�ҽ���ÿ�ܶ������ľٰ�һ��������ʿ���ܹ�����ġ����������������ǲ�����˶��ܻ�ý����������ܹ��������ǰʮ�������ܻ�÷ǳ����Ľ�����
		add_talk #b#b
		add_talk #b#b�������������ߵģ�
		
		option 101 #����һ�Ҵ�����ҪȥѰ���´�½��
			
		talk
	endif
endproc

proc 100
	TASK_DO_OPTION 4500 0
endproc

PROC 201
	GET_EVENT_STAT 10 95
	if @10 = 1
	;����ڽ�����
		add_sys_msg_start
		add_sys_msg ���ڻ���ڽ����У����������������鿴�������
		set_sys_msg_mode 4 0 0
		msg_info
	else
		OPEN_HANG_HAI_RANGE_UI 1
	endif
ENDPROC

proc 101
	DEL_ROLE_TMP_ITEM 0 0
	
	strvar begin_time
	;��¼����ʼʱ��
	GET_TIME_SEC begin_time
	SET_TASK_VAR 4500 0 32 @begin_time 1
	
	;������ʱ��е
	APPLY_TMP_ITEM 1461002 0 1
	
	;����
	TRANSMIT 4403 23200 32600 0
endproc

proc 202
	dialog 1
	add_talk $_npc$:
	add_talk #b#b#c6d00d2��1������������x5
	add_talk #b#b#c6d00d2��2������������x3
	add_talk #b#b#c6d00d2��3������������x2
	add_talk #b#b#c6d00d2��4-10������������x1#n
	add_talk #b#b
	add_talk #b#b�����յ��ѡ�ֶ��ɿ������������أ��м��ʻ�ã��������̡��ͽ��������顢��ͭ��顢�����顢ʧ��Ĳر�ͼ���ǻԱ��ꡭ
	add_talk #b#b
	add_talk #b#b#c186506�����������ı�����Ҫ���������Ķ��ݡ�������Դ�ʢ��������ε����õ�����
	
	option 1 ����
	talk
endproc
