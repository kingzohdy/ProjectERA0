;npc��ɲ���

proc 1
	;strvar x y m
	;��ȡ�������
    ;GET_PLAYER_MAPINFO 1 0 0 x y
	;����������6���ڶ������
	;GET_POINT_DIST @x @y 24900 30000 m
	;if m < 600
		;�ж��Ƿ�����ʱ��е
	;	GET_ROLE_TMP_ITEM 0 10
	;	if @10 = 1461002
	;		task_stat 4500 1 20
	;		if 20 = 2
	;			call 201
	;			task_fini 4500 1			
	;			call 202
	;			DEL_ROLE_TMP_ITEM 0 0
	;		endif
	;	endif
	;endif
	
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b�㴩Խ����������������������ִﵽ�����������յ㣬�Ҳ�������һ�������Ŷ�δ֪�Ŀ�������İ���ĺ��档������ݺ����������$_player$������ֻ����������ֱǰ�ļ���С�

	;�ж��Ƿ�����ʱ��е1461002 �������Ƿ�������4500  �вų��ָ�ѡ��
    GET_ROLE_TMP_ITEM 0 15
	task_stat 4500 1 16
	if @15 = 1461002 and @16 = 2
		option 205 #�����������յ�ǩ��
	endif
	option 200 #�ص�����
	
	talk
endproc 

PROC 200
	;����
	TRANSMIT 4403 24200 31800 0
	DEL_ROLE_TMP_ITEM 0 0
ENDPROC

;��¼���ʱ��
proc 201
	strvar sec min tmpvar hanghaisec
	;��¼�������ʱ��
	GET_TIME_SEC sec
	GET_TASK_VAR 4500 0 32 1 15
	
	sub sec @15
	;�ȼ�¼�������ͻ���
	var hanghaisec @sec	
	HANG_HAI_RANGE 1 @hanghaisec
	
	if sec > 60
		var tmpvar @sec
		dev tmpvar 60
		var min @tmpvar
		mod sec 60		
	else
		var min 0 
	endif
	add_sys_msg_start
	add_sys_msg �����κ������ٶ���$@min$��$@sec$�룬���ڻ������ǰ�����ײ����鿴�����������
	set_sys_msg_mode 6 0 0
	msg_info
endproc

proc 205
	call 201
	task_fini 4500 1	
	DEL_ROLE_TMP_ITEM 0 0
endproc
