;�ƽ���


proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b����$_player$����˵������ڽ���һ�����ܵĹ������㿴�������������ӣ�������û�д��������������ѷ�����Ĺ������鰡��
	add_talk #b#b���ˣ������ҵĽ�����лл���͵Ļ�����ʲô����û�������ҵĽ������㲻Ҫ����������û��ʲô�ɺ��ߵģ����ĳ�Ҳ������Ĵ��������飬�����㣬���绹�Ǻ����õģ�
	
	task_add_option 2001 1 100
	task_add_option 2002 1 101
	task_add_option 2003 1 102
	task_add_option 2004 1 103
	task_add_option 2009 1 104
	task_add_option 2010 1 105
	task_add_option 2017 1 106
	
	task_add_option 2040 1 107
	task_add_option 2050 1 108
	
	task_add_option 2005 1 109
	task_add_option 2101 1 110
	
	
	
	talk
	
endproc 

PROC 100
	task_do_option 2001 1
ENDPROC
PROC 101
	task_do_option 2002 1
ENDPROC
PROC 102
	task_do_option 2003 1
ENDPROC
PROC 103
	task_do_option 2004 1
ENDPROC
PROC 104
	task_do_option 2009 1
ENDPROC
PROC 105
	task_do_option 2010 1
ENDPROC
PROC 106
	task_do_option 2017 1
ENDPROC
PROC 107
	task_do_option 2040 1
ENDPROC
PROC 108
	task_do_option 2050 1
ENDPROC
PROC 109
	task_do_option 2005 1
ENDPROC
PROC 110
	task_do_option 2101 1
ENDPROC

proc 2
	
	get_trig_task 1 2 
	if 2 = 2101
		task_fini 2101 1
	endif
	if 2 = 2005
		task_fini 2005 1
	endif
	if 2 = 2010
		task_fini 2010 1
	endif
	if 2 = 2040
		task_fini 2040 1
	endif
endproc

