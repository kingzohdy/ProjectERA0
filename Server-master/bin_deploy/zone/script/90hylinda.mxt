; �մ�


proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b����ΪʲôҪ�������أ�Ϊʲô�������ˮ���̵��أ���Ȼ��ˣ�Ϊʲô������������������أ�
	
		task_add_option 2125 1 100
		task_add_option 2126 1 101
		task_add_option 2127 1 102
		task_add_option 2128 1 103
		
		strvar Ret
		task_stat 2128 1 Ret
		if Ret = 2
			option 150 ����ѡ�����֣�
		endif
		
		strvar Ret1
		task_stat 2109 1 Ret1
		if Ret1 = 2
			option 180 ��������˵����������

		endif
		strvar ret2 num
		task_stat 2126 1 ret2
		GET_ITEM_NUM_INPAK 5300901 1 num
		if ret2 = 2 and num = 0
			option 190 #�ո���ı����������ˣ��ٸ���һ���
		endif
	talk
endproc

PROC 100
	task_do_option 2125 1
ENDPROC
PROC 101
	task_do_option 2126 1
ENDPROC
PROC 102
	task_do_option 2127 1
ENDPROC
PROC 103
	task_do_option 2128 1
ENDPROC


proc 2
	get_trig_task 1 2
	if 2 = 2128
		dialog 1
		add_talk $_npc$:
		add_talk #b#b����ΪʲôҪ�������أ�Ϊʲô�������ˮ���̵��أ���Ȼ��ˣ�Ϊʲô������������������أ�
			option 150 ����ѡ�����֣�
		talk
	endif
endproc

proc 150
	dialog 1
	add_talk $_npc$:
	add_talk #b#b�԰�������ѡ���ԡ���ɫ���������ǡ���ɫ������$_player$���ҿɾͰ�С��������������ˣ���ǧ��Ҫ����ѡ��Ӵ��

		option 151 ��ɫ����
		option 152 ��ɫ����

	talk

endproc 
proc 151
	dialog 1
	add_talk $_npc$:
	add_talk #b#bҲ�ã��ܹ�����ɫʳƷ�Ӧ���������޺��Ĺ����Ǿ����ɡ�

		option 153 #��˵ʲô����ʲô�ɡ�

	talk
endproc
proc 152
	dialog 1
	add_talk $_npc$:
	add_talk #b#b���̲��̵�����ˮˮ��������ҩ��������ɫ���Ҳ�����ģ���ˮҲ�����ģ�Ӧ�ò�����ʲô�������

		option 153 #��˵ʲô����ʲô�ɡ�

	talk
endproc
proc 153
;	ADD_ITEM_NUM_INPAK 5300903 1 1 0
	task_fini 2128 1
	return
endproc 

proc 180
	dialog 1
	add_talk $_npc$:
	add_talk #b#b��Ȼ��˵�������������Ұ������Ϣ�����˶�����أ��ҿ������ϵ�һ��֪�������Ϣ���ˣ��һ��صذ���ص����Ŷ��ռ���������һ�ݼ�������

		option 181 �ۣ�������ѽ�����ܿ�����

	talk
endproc
proc 181
	dialog 1
	add_talk $_npc$:
	add_talk #b#bҲ���Ǻ���������һ�����������裬��Ȼ���ԣ�����������ǧ���Ū����Ӵ��

		option 182 #���İɣ���һ����úñ�������
	talk
endproc
proc 182
	
	ADD_ITEM_NUM_INPAK 5300895 1 1 0
endproc 

proc 190
	ADD_ITEM_NUM_INPAK 5300901 1 1 0
endproc 









