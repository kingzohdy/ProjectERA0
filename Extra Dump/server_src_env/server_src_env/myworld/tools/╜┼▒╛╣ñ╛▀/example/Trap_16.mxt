;��6��ͨ��7�㣬
;
proc 10
	get_task_status 265 100
	get_task_var 265 0 101
	if 100 = 2 and 101 >= 7
		GET_MAP_MONSTER_NUM 12
		if 12 = 0
		goto_map 30207 3305 2912 10 995
		if 995 = 0
			msg_player 3 2 �����������߲�
		else
			msg_player 3 2 �����������߲�ʧ�ܣ�
		endif
		else
			msg_player 3 2 �����ͼ�Ĺ�����δ���꣬���Ƚ�����ȫ������
		endif
	else
		msg_player 3 2 ����δ���ǰ��ؿ�����ս���޷�ǰ�����߲㣡
	endif
endproc 