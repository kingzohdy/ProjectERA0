;
proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b��ӭ���ټ����������գ��ҹ�˾���С��ͻ����ϡ��ĳ�ŵ��ͨ���ṩ���ɿ���׼�㡢���㡢��ݡ������ʷ����������㲢��Խ�ͻ�������������һ�Ȼ�����ְܾ䷢�ķ��а�ȫ��߽���
;	add_talk #b#b���Ϊ��л���ͻ���֧����񰮣��ҹ�˾�ؾٰ�����Żݻ������ʱ����ڴ���ҹ�˾�ĺ��࣬���۾���Զ����ֻ��Ҫ90���ң�
;	add_talk #b#bֻ��90���ң����Ϳ������̴�����·�̣���û�п�������9ǧ������9�٣�ֻҪ90���ң�
	
	strvar LV
	GET_PLAYER_BASE_ATT 1 1 LV 1
	if LV >= 10
		OPTION 100 #���纽��
	endif
	talk
	
endproc
proc 100
	strvar ret
	task_stat 3131 1 ret
	if ret = 2
		task_fini 3131 1
	endif
	MAP_TRANS 1

endproc

