


proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b��Ҫȥ���˺������ϳ���������̫���ˣ��ҿ�������ǰȥ������Ҫ�������Ӵ��
;		strvar Ret1
;		task_stat 1373 1 Ret1
;		if Ret1 = 2 
			option 150 #����С��
;		endif
	talk
endproc 

proc 150
	AUTOFLY 122 1
;	transmit 2202 12100 36300 1
endproc
