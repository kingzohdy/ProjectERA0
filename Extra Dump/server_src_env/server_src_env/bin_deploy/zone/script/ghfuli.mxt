

proc 1
	DIALOG 1
	add_talk $_npc$:
	
	strvar Ret Pos Gend
	PLAYER_CLAN_POS_INFO Ret 0 0 0 Pos	
	GET_PLAYER_BASE_ATT 0 Gend 0 0
	
	
	if Ret = 0 and Pos = 0
		add_talk #b#b��λСͬ־���㿴����������������һ�����칫�ḣ��������ô������߽���������С��ͷ�����ƣ�
		add_talk #b#b�ף���Ĺ���ø��԰�����Ȼû�����֣�ɶ���㻹û�м��빫�᣿����Ͻ��Ҹ������������ɣ���һ��ӹ������һ���츣��Ӵ����һ���칫�ḣ���������ʧ����Ӵ��
	
		strvar Ret num1
		GET_ITEM_NUM_INPAK 5300934 1 num1
		if num1 > 0
			TASK_STAT 3305 1 Ret
			if Ret = 2
				option 700 �����뻻����
			endif	
		endif
	else
		if Gend = 1
			add_talk #b#b�ˣ�С���ӣ������Ĺ��ḣ������û��Ӵ����Ȼ�⸣����������һҹ������һϦ������������Ҳ��С��ͷ������Ϊ��׼���ģ��ջ�����Ҳ����һ��С��Ŀ����
		else	
			add_talk #b#b�ˣ�С��������Ĺ��ḣ������û��Ӵ����Ȼ�⸣����������һҹ������һϦ������������Ҳ��С��ͷ������Ϊ��׼���ģ��ջ�����Ҳ����һ��С��Ŀ����
		endif
		
		strvar Ret num1
		GET_ITEM_NUM_INPAK 5300934 1 num1
		if num1 > 0
			TASK_STAT 3305 1 Ret
			if Ret = 2
				option 700 �����뻻����
			endif	
		endif
		option 150 #(����)��ȡ���ḣ��
		option 200 #(����)��ȡ��Ա����
		
	endif
	
	
	
	;	option 160 #����
	
	talk

endproc 
proc 160
	GET_ID_PLAYER_INFO 0 3 10
	GET_ID_PLAYER_INFO 0 4 20
	effect 0 0 playEffct($@10$,$@20$,"effect\\entity\\5300860.ent")
	
;	NEW_LIFE_NPC 1 307015 @10 @20 0 120 xbxiang_1.mac
	
endproc 
proc 200
	open_ui 84
endproc

proc 150
	strvar Ret Pos
	PLAYER_CLAN_POS_INFO Ret 0 0 0 Pos	
	if Ret = 0 and Pos != 0
		open_ui 62
	else
		add_sys_msg_start
		add_sys_msg ���Ѿ����ǹ����Ա�ˣ����ɴ򿪹��ḣ����塣
		set_sys_msg_mode 3 0 0
		msg_info
	endif
endproc 

proc 700
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b�������ҵ�ȷ�У����������ܸ��㣬��Ȼ��ס�İ���
	add_talk #b#bСͬ־��Ҫ�����İ�������ô���׾����٣�����Ȼ�����ñ��������������в�ͼ��ġ����Ԫʷ������������ֻ�������Ŷ��
		option 701 #����

	talk
	
endproc
proc 701
	
	strvar ItmNum Num Rad
	GET_ITEM_NUM_INPAK 5300934 1 ItmNum
	if ItmNum > 0
			
		GET_PACK_SPACE 0 Num
		if Num > 0
			DEC_ITEM_NUM_INPAK 5300934 1 1
			
			ADD_ITEM_NUM_INPAK 5300935 1 0 0

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




