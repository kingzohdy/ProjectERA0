; ��ķ��


proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b���뵱�����ĳ��ӾͲ��Ǻò÷죡
	add_talk #b#b����ƾʲô��˶�һ�仰���Ҿ�Ҫ���������ƾʲô���ٵľ��ܴ���С�硢����С�ơ�����С�⣬���һ�ñ�˺����Ƭ��
	add_talk #b#b�������˹��������ǵģ����˴����ҵģ�����ƾʲô��������˼�̤������Ҳ�������ĺ��ӣ�

	task_add_option 2115 1 100
		task_add_option 2116 1 101
		task_add_option 2117 1 102
		task_add_option 2118 1 103
		
		strvar Ret
		task_stat 2117 1 Ret
		if Ret = 2
			option 150 #��ܸ�ķ��

		endif
	talk
endproc

PROC 100
	task_do_option 2115 1
ENDPROC
PROC 101
	task_do_option 2116 1
ENDPROC
PROC 102
	task_do_option 2117 1
ENDPROC
PROC 103
	task_do_option 2118 1
ENDPROC


proc 2
	get_trig_task 1 2
	if 2 = 2117
		dialog 1
		add_talk $_npc$:
		add_talk #b#b���뵱�����ĳ��ӾͲ��Ǻò÷죡
		add_talk #b#b����ƾʲô��˶�һ�仰���Ҿ�Ҫ���������ƾʲô���ٵľ��ܴ���С�硢����С�ơ�����С�⣬���һ�ñ�˺����Ƭ��
		add_talk #b#b�������˹��������ǵģ����˴����ҵģ�����ƾʲô��������˼�̤������Ҳ�������ĺ��ӣ�
		
			option 150 #��ܸ�ķ��
		talk
	endif
	if 2 = 2118
		task_fini 2118 1
	endif

endproc
proc 150
	strvar x y
	GET_PLAYER_MAPINFO 0 0 0 x y
	CALL_MON 1 254021 500 120 @x @y 1
	return
endproc 





