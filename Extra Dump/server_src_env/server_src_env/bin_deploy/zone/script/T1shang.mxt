;303036	
proc 1

	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b�ţ���������Щ���ᣬ�㻹��ҪŬ������
	add_talk #b#bս������̵���Ʒ�ϼܣ����� #c186506���ֱ�ʯ�����ó��֡����׹�ʵ������#n ���ϵ��εĶ���Ŷ~~
	add_talk #b#b���ڵ�ŵ���Ѿ����ұƵĸ������أ�������״������Ӵ�Һ����������ͷ��~~
	
	;��һ�
	task_stat 4432 1 51
	if 51 = 2
		option 510 ����ο�ʡ�����������
	endif
	
	strvar Ret
	
	strvar rum
	GET_ITEM_NUM_INPAK 5300967 1 rum
	task_stat 3025 1 Ret
	if Ret = 2 and rum = 0
		option 500 #�����溢�������ú����


	endif
	; OPTION 110 #(����)���̵� 
	
	strvar LV
	GET_PLAYER_BASE_ATT 1 1 LV 1
	
	if @LV < 50
		option 201 #(����)��ս����¡��̵�
	endif
	if @LV >= 50 and @LV < 60
		option 202 #(����)��ս����¡��̵�
	endif
	if @LV >= 60 and @LV < 70
		option 203 #(����)��ս����¡��̵�
	endif
	if @LV >= 70 and @LV < 80
		option 205 #(����)��ս����¡��̵�
	endif	
	if @LV >= 80
		option 204 #(����)��ս����¡��̵�
	endif
	
	talk


endproc

proc 110
	 OPEN_NPC_SHOP
endproc

proc 500
	task_stat 3025 1 Ret
	if Ret = 2
		strvar rum
		GET_ITEM_NUM_INPAK 5300967 1 rum
		if rum = 0
			ADD_ITEM_NUM_INPAK 5300967 1 1 20
		endif
	endif
endproc 

;��һ�
proc 510
	GET_ITEM_NUM_INPAK 5301004 1 10
	STRVAR flag
	var flag 0
	if 10 > 0
		DEC_ITEM_NUM_INPAK 5301004 1 11
		if 11 = 0
			var flag 1
			task_fini 4432 1
			
			DIALOG 1
			add_talk $_npc$:
			add_talk #b#bлл�㣬$_player$���뽫�ҵ�ף���������ͣ�˳������лл����
			add_talk #b#bԸ�Ͷ��ڸ��������������һ�㣬�������һ�㣬������˳һ�㣬�������һ�㡣
			option 511 #û����
			talk
		endif
	endif
	
	if flag = 0
		add_sys_msg_start
		add_sys_msg ���ı�����û�С���Ʒ�С���
		set_sys_msg_mode 1 0 0
		msg_info
		PLAYER_CLOSE_DIALOG1 0 0
	endif
endproc

proc 201
	OPEN_SHOP_BY_ID 10030
	
	task_fini 2268 1
endproc
proc 202
	OPEN_SHOP_BY_ID 10031
	
	task_fini 2268 1
endproc
proc 203
	OPEN_SHOP_BY_ID 10032
	
	task_fini 2268 1
endproc
proc 204
	OPEN_SHOP_BY_ID 10033
	
	task_fini 2268 1
endproc
proc 205
	OPEN_SHOP_BY_ID 10034
	
	task_fini 2268 1
endproc
