proc 1


	DIALOG 1
	ADD_TALK $_npc$:       
	ADD_TALK #b#b������ĵ��������Ȼ���������ı������ŵĵܵܵ�Ů����������Ҳ���Ź���
	ADD_TALK #b#b��ЩǮ���������ھư�����һ�ܵģ����ȴ���Ҫ�������������ծ�������õ�һ�ܰ�����ʲôʱ����ܻص�����߰���
	task_add_option 3136 1 100
	task_add_option 3135 1 101
;	option 110 #����
	TALK
	
endproc

proc 110

			effect 2 0 playEffct(20900,44900,"effect\\entity\\9572_14.ent")
endproc

proc 100
    task_do_option 3136 1
endproc
proc 101
    task_do_option 3135 1
endproc
proc 2
	get_trig_task 1 2
	if 2 = 3136
		task_fini 3136 1
	endif

endproc
