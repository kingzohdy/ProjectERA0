;���Ƚ̹�
; 

proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#bÿ���˶��ᾭ������׶Σ�����һ��ɽ������֪��ɽ������ʲô���󲿷��˻���Ϊɽ���վ�����ʧ�������С��Һ���������������֪��ɽ�Ǳ���ʲô��������ȥ�����ɣ�Ҳ����������ջ�
	
	task_add_option 1083 1 106
;	task_add_option 1084 1 107
;	task_add_option 1085 1 108
;	task_add_option 1086 1 109
	
;	task_add_option 1088 1 111
;	task_add_option 1089 1 112

	
	talk
endproc

proc 106
    task_do_option 1083 1
endproc
proc 107
    task_do_option 1084 1
endproc
proc 108
    task_do_option 1085 1
endproc
proc 109
    task_do_option 1086 1
endproc
proc 111
    task_do_option 1088 1
endproc
proc 112
    task_do_option 1089 1
endproc


proc 2
	strvar Retask
	get_trig_task 1 Retask
;	if @Retask = 1086
;		task_fini 1086 1
;	endif

endproc

