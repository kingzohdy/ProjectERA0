proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b�μ��������ˡ��Ҹո�������ɣ�Ĳҽ�����ԭ�����������ˣ�����ô����
	add_talk #b#b������л�������˾�����
	TASK_STAT 1323 0 20
	if 20 = 2 
		option 100 ����ΪʲôҪ°���㣿
	endif
	talk
endproc
proc 100
	dialog 1
	add_talk $_npc$:
	add_talk #b#b�������̽��������ʽ�����ܣ��Լ��ɰ͵����䡣�������������˷��ģ��Ҳ�û�и��������ɰͱ�������ս�Ĵ����ܾݵ�����顣

	option 102 ���Ȼ�ս�Ĵ崫������ϼӷ�����
	
	talk
endproc
proc 101
	dialog 1
	add_talk $_npc$:
	add_talk #b#b�������̽��������ʽ�����ܣ��Լ��ɰ͵����䡣�������������˷��ģ��Ҳ�û�и��������ɰͱ�������ս�Ĵ����ܾݵ�����顣

	option 102 �ܺã����Ȼ�ս�Ĵ壬��������ϼӿ����ɰͣ�ǧ����������ˣ�
	
	talk
endproc
proc 102
	dialog 1
	add_talk $_npc$:
	add_talk #b#b����������
	
	 option 103 #�ţ�
	task_fini 1323 0
	talk

endproc
proc 103
	return
endproc
;proc 2
;	get_trig_task 1 2
;	if 2 = 1308
;		task_fini 1308 1
;	endif
;
;endproc

