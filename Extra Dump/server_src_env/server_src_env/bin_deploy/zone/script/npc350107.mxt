;�������ս����    ,������
proc 1
	;battlestat30,ս���׶�|0��ʼ�׶� 1׼���׶� 2��ʼ�׶�
	var 30 0
	get_pworld_var 80 32 1 30
	if 30 != 2
		return
	endif
	autofly 126 1
	DEL_NPC 0 1
	
endproc
