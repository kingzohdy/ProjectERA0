;���� 
; Я������\���̵�

proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b���ǵĿں��ǣ�������󣬵�����ã�

	
;	strvar Ret1053 
;	TASK_STAT 1053 1 Ret1053
;	if Ret1053 = 2
;		option 150 ������ȡ�������
;	endif
	
	OPTION 110 #(����)���̵� 
	
	
	
	strvar Ret num1
	GET_ITEM_NUM_INPAK 5300762 1 num1
	if num1 > 0
		TASK_STAT 3304 1 Ret
		if Ret = 2
			option 500 �����뻻����
		endif
	endif
	talk
endproc
proc 110
	OPEN_NPC_SHOP
endproc 

proc 150
	DIALOG 1
	add_talk $_npc$:
	strvar Gend
	GET_PLAYER_BASE_ATT 0 Gend 0 0
	if Gend = 1
		add_talk #b#bѽ��˧�磬��ǰû�����㰡���ǽ���ż����������˵�ô��Ҫ��Ҫ���ʳ�ﰡ���Ҹ������ۣ�ʲô�������ɶ��������켱������ģ���ô����˵�أ��������Ǹ�˧��������ϣ��������ˣ���ҪǮ�������´������Ҫ��ʳ�һ���ǵ�������Ӵ��
	else
		add_talk #b#bѽ����Ů����ǰû�����㰡���ǽ���ż����������˵�ô��Ҫ��Ҫ���ʳ�ﰡ���Ҹ������ۣ�ʲô�������ɶ��������켱������ģ���ô����˵�أ��������Ǹ���Ů�������ϣ��������ˣ���ҪǮ�������´������Ҫ��ʳ�һ���ǵ�������Ӵ��

	endif
		option 151 #��л�ˣ�����ȥ���������ͷ��

	talk

endproc 
proc 151
	
	ADD_ITEM_NUM_INPAK 5300004 1 1 0

endproc 

proc 500
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b��ֳ�д����ţ��˶�����������ע����
	add_talk #b#b���ҽ����ɣ������������ó���һ����ֵ����顣
	
		option 501 #����

	talk
endproc

proc 501
	
	strvar ItmNum Num Rad
	GET_ITEM_NUM_INPAK 5300762 1 ItmNum
	if ItmNum > 0
			
		GET_PACK_SPACE 0 Num
		if Num > 0
			DEC_ITEM_NUM_INPAK 5300762 1 1
			
		;	rand Rad 100
		;	if @Rad < 80
				ADD_ITEM_NUM_INPAK 5300763 1 0 0
		;	endif
		;	if @Rad >= 80 and @Rad < 95
		;		ADD_ITEM_NUM_INPAK 5300764 1 0 0
		;	endif
		;	if @Rad >= 95 and @Rad < 99
		;		ADD_ITEM_NUM_INPAK 5300765 1 0 0
		;	endif
		;	if @Rad = 99
		;		ADD_ITEM_NUM_INPAK 5300766 1 0 0
		;	endif
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





