
proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b�����������������������죬�ҽ���ÿ�ܶ������ġ������ٰ�һ��������ʿ���ܹ�����ġ����������������ǲ�����˶��ܻ�ý����������ܹ��������ǰʮ�������ܻ�÷ǳ����Ľ�����
    add_talk #b#b
	add_talk #b#b�������������ߵģ�
	add_talk #b#b
	add_talk #b#b#c186506�����������ճ̱�
	add_talk #b#b#c186506�ܶ������ġ����� 8��00-20��00
	
	option 201 #�鿴��һ�캽���������
			
	talk
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
