;��ţԤ��,����
;����
proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#bţ�к�æ���������󣬾Ϳ����������ѯ�����ɼ������ڲ��������ڶ࣬��ֻ������ʱ�����ٶ�����ǰ10��ѡ�ֵĳɼ���
	add_talk #b#b����·��ġ���ѯ�����ɼ�����ȡ������ѡ��Ϳɿ���ǰ10���ĳɼ��񵥣����ǰ�����������ң�����������������ȡһ������������
	add_talk #b#b��ܰ��ʾ�����ھ���������24Сʱ����ȡ�������������������ǳ����ǧ��Ҫ���Ӵ��
	
	option 110 ��ѯ�����ɼ�����ȡ����
	
		strvar Ret num1
	GET_ITEM_NUM_INPAK 5300926 1 num1
	if num1 > 0
		TASK_STAT 3305 1 Ret
		if Ret = 2
			option 700 �����뻻����
		endif
	endif
	
	
	talk
endproc 
proc 110
	OPEN_COW_FINAL_RANGE_UI 1
endproc
proc 700
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b������水����������ݴ߶����ֻ���������ѣ���֪����˭��������ƪ�����ӵܵĶ�������
	add_talk #b#b���ҽ����ɣ���Ҳ��ϣ�������������Ϊ���ܵ��˺����԰ɣ�

		option 701 #����

	talk
	
endproc
proc 701
	
	strvar ItmNum Num Rad
	GET_ITEM_NUM_INPAK 5300926 1 ItmNum
	if ItmNum > 0
			
		GET_PACK_SPACE 0 Num
		if Num > 0
			DEC_ITEM_NUM_INPAK 5300926 1 1
			
			rand Rad 100
			if @Rad < 80
				ADD_ITEM_NUM_INPAK 5300927 1 0 0
			endif
			if @Rad >= 80 and @Rad < 95
				ADD_ITEM_NUM_INPAK 5300928 1 0 0
			endif
			if @Rad >= 95 and @Rad < 99
				ADD_ITEM_NUM_INPAK 5300929 1 0 0
			endif
			if @Rad = 99
				ADD_ITEM_NUM_INPAK 5300930 1 0 0
			endif
		else
			add_sys_msg_start
			add_sys_msg �����ռ䲻�㣡
			set_sys_msg_mode 1 0 0
			msg_info
		endif
	else
		add_sys_msg_start
		add_sys_msg ������û�п��Խ�������Ʒ��
		set_sys_msg_mode 1 0 0
		msg_info
	endif
endproc 
