;����³



proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b����ѧ�ᳫ�ھ��Ĺ�Ժ󣬺ö�Ŵ����˵���Ǻ����ﶼ��¶�ˣ�����Ұ�����յ�����Ҳ�𽥼����ˡ�
	
	
	task_add_option 2018 1 100
	task_add_option 2019 1 101
	task_add_option 2020 1 102
	task_add_option 2021 1 103
	strvar Ret Num
	GET_ITEM_NUM_INPAK 5300755 1 Num

	task_stat 2020 1 Ret
	if Ret = 2 and Num = 0
		option 120 #�ҵ��ն��ù���
	endif
	
	talk
	
endproc 

PROC 100
	task_do_option 2018 1
ENDPROC
PROC 101
	task_do_option 2019 1
ENDPROC
PROC 102
	task_do_option 2020 1
ENDPROC
PROC 103
	task_do_option 2021 1
ENDPROC
proc 120
		ADD_ITEM_NUM_INPAK 5300755 1 0 0

endproc
 

proc 2
	
	get_trig_task 1 2 
;	if 2 = 2001
;		task_fini 2001 1
;	endif
;	if 2 = 2017
;	
;	endif
endproc

