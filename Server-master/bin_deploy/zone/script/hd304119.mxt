proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b�������Ҹ������ͬʱ�����кܶ���ȱ����ʳ���������ȿڸɾ���ˮ�������������ڴ˼ѽ�֮����ʱ������۳���ʮ����ģ�ṩ���ڶ���Ʒ�ڴ�������ϣ�����ø���������뼢������Ӱ��
	add_talk #b#b#cff4e00�����������һ���Ϊ����ļ�裬�������з��װ��ĵ��˶����ü���Ʒһ�ݣ�Ҳ��ֵ����Ǯ��ȴ���ҵ�һ�����⡣
	add_talk #b#b#cFF00805��10�գ����վͻ��뿪ʱ��ǣ����Ｏ���ƿ�����������������

	option 150 #(����)��ģ�����̵�
	
	GET_PLAYER_DAILY_FLAG 0 12 10
	if 10 = 0
	option 200 #��Ҫ����10000����
	endif
	
	option 210 #��Ҫ���������ڣ�ʱװһ��
	talk

endproc 

proc 150
	OPEN_NPC_SHOP 
endproc 

proc 200
	strvar ret
	get_id_player_info 0 21 ret
	get_id_player_info 0 22 30
	if 30 < 10000 and @ret >= 10000
		choose 1 201 202 ���������Ѵﵽÿ��������ޣ�����������ֱ���������ң��Ƿ����������
	elseif ret < 10000
		choose 1 201 202 �������㣬����������ֱ���������ң��Ƿ����������
	else
		call 201
	endif
endproc

proc 201
	DEC_MONEY_TEST 0 10000 21 10
	if 10 = 0
		GET_PACK_SPACE 0 11
	
		if 11 < 1
			add_sys_msg_start
			add_sys_msg �����ռ䲻�㣡
			set_sys_msg_mode 4 0 0
			msg_info
		else
			strvar Ret 
			SET_PLAYER_DAILY_FLAG Ret 12 1
			if Ret = 0
				DEC_MONEY 0 10000 21 12
				if 12 = 0
					ADD_BIND_ITEM_NUM_INPAK 5052306 1 1 13
					add_sys_msg_start
					add_sys_msg ��л���ľ���������һ���Ͷ�˶����ʾ��л��
					set_sys_msg_mode 3 0 0
					msg_info
				endif
			endif
		endif
	else
		add_sys_msg_start
		add_sys_msg �������Ҳ��㡣
		set_sys_msg_mode 4 0 0
		msg_info
	endif
endproc

proc 202

endproc

proc 210
	dialog 3
	add_talk $_npc$:
	add_talk #b#b�����Ծ��׳������⺣̲�Ȼ��������ʱװ�����ڵ�Ҳ���ԡ�
	add_talk #b#b#c6d00d2���β��ɾ���
	add_talk #b#b
	add_talk #b#b����Ҫ�ύ����Ʒ�����·��Ŀ���
		
	NPC_COMMIT_OPT 300
	talk
endproc

proc 300
	strvar itemId pos index ret flag weizhi
	GET_COMMIT_ITEM_INFO 1 itemId
	if itemId = -1
		add_sys_msg_start
		add_sys_msg ����û���ύ��Ʒ
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	ARM_IS_FASHION 10 @itemId
	if 10 = -1
		add_sys_msg_start
		add_sys_msg ����׳����Ρ�ɳ̲�Ȼ��������ʱװ�����ڵ�Ҳ���ԣ�
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	GET_WEAR_POS1 20 @itemID weizhi
	if @20 = 0 and weizhi = 24
		add_sys_msg_start
		add_sys_msg ����׳����Ρ�ɳ̲�Ȼ��������ʱװ�����ڵ�Ҳ���ԣ�
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	
	;�ų�Ӿװ
	var flag 1
	if itemId = 3050010 or itemId = 3050011 or itemId = 3050012 or itemId = 3450010 or itemId = 3450011 or itemId = 3450012 or itemId = 3451010 or itemId = 3451011 or itemId = 3451012 or itemId = 3750010 or itemId = 3750011 or itemId = 3750012 or itemId = 3751010 or itemId = 3751011 or itemId = 3751012
		var flag 0
	endif
	
	; if itemId = 3653000 or itemId = 3653001 or itemId = 3653002 or itemId = 3653003 or itemId = 3653004 or itemId = 3653005 or itemId = 3653006 or itemId = 3653007 or itemId = 3650003
		; var flag 0
	; endif
	
	if flag = 0
		add_sys_msg_start
		add_sys_msg ���ʱװ̫��ˬ�ˣ��������һЩ�Ƚ�ʵ�á���ů��ʱװ�ɣ�
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	GET_COMMIT_ITEM_INFO 2 pos
	GET_COMMIT_ITEM_INFO 3 index
	
	DEC_ITEM_NUM_BYIDX @itemId 1 @pos @index ret
	if ret = 0
		ADD_BIND_ITEM_NUM_INPAK 5052306 1 1 13
		
		add_sys_msg_start
		add_sys_msg ��л���ľ���������һ���Ͷ�˶����ʾ��л��
		set_sys_msg_mode 3 0 0
		msg_info
	endif
	PLAYER_CLOSE_DIALOG 0 0
endproc