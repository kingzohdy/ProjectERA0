;npc��ķ��

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b��λ�����ѣ�С���ѣ������ѣ�Ů���ѣ�˧���ѣ������ѣ����������ҵ���������ݰɣ�������������ص�������ߡ��������ǵ�С��Ϸ�����������ڶ�ϡ����ߵ��˶������У�
	add_talk #b#b
	add_talk #b#b#cff00806��7������24��������ķ���ͻ��뿪ʱ��ǣ�����Ѱ�Ҹ�ʽ��������߳�ʵ����ݡ�

	option 105 #(����)����̵�
	
	GET_PLAYER_DAILY_FLAG 0 14 10
	if 10 = 0
	option 200 #1000������һ���˶�������
	endif
	
	option 210 #һ�������ڣ�ʱװ��һ���˶�������

	option 214 ����������
	talk
endproc 

proc 105
	OPEN_NPC_SHOP 
endproc 

proc 200
	strvar ret
	get_id_player_info 0 21 ret
	get_id_player_info 0 22 30
	if 30 < 1000 and @ret >= 1000
		choose 1 201 202 ���������Ѵﵽÿ��������ޣ�����������ֱ���������ң��Ƿ����������
	elseif ret < 1000
		choose 1 201 202 �������㣬����������ֱ���������ң��Ƿ����������
	else
	;��������
		call 201
	endif
endproc

proc 201
	DEC_MONEY_TEST 0 1000 21 10
	if 10 = 0
		GET_PACK_SPACE 0 11
	
		if 11 < 1
			add_sys_msg_start
			add_sys_msg �����ռ䲻�㣡
			set_sys_msg_mode 4 0 0
			msg_info
		else
			strvar Ret 
			SET_PLAYER_DAILY_FLAG Ret 14 1
			if Ret = 0
				DEC_MONEY 0 1000 21 12
				if 12 = 0
					ADD_ITEM_NUM_INPAK 5052116 1 1 13
					add_sys_msg_start
					add_sys_msg ��л���ľ���������һ���˶���ħ����ʾ��л��
					set_sys_msg_mode 3 0 0
					msg_info
				endif
				PLAYER_CLOSE_DIALOG1 0 0
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
	call 1
endproc

proc 210
	dialog 3
	add_talk $_npc$:
	add_talk #b#b�����Ծ���ʱװ�����ڵ�Ҳ���ԡ�
	add_talk #b#b����Ҳ���Ծ���
	add_talk #b#b
	add_talk #b#b����Ҫ�ύ����Ʒ�����·��Ŀ���
		
	NPC_COMMIT_OPT 300
	talk
endproc

proc 300
	strvar itemId pos index ret flag
	GET_COMMIT_ITEM_INFO 1 itemId
	if itemId = -1
		add_sys_msg_start
		add_sys_msg ����û���ύ��Ʒ
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	
	;����
	var flag 0
	if itemId = 3653000 or itemId = 3653001 or itemId = 3653002 or itemId = 3653003 or itemId = 3653004 or itemId = 3653005 or itemId = 3653006 or itemId = 3653007 or itemId = 3650003
		var flag 1
	endif
	
	if itemId = 3650004 or itemId = 3650005 or itemId = 3650006 or itemId = 3650007 or itemId = 3650015 or itemId = 3650017 or itemId = 3651001 
		var flag 1
	endif
	
	if itemId = 3651002 or itemId = 3651003 or itemId = 3651004 or itemId = 3651005 or itemId = 3651006 or itemId = 3651007 or itemId = 3652000
		var flag 1
	endif
	
	if itemId = 3652001 or itemId = 3652003 or itemId = 3652005
		var flag 1
	endif
	
	ARM_IS_FASHION 10 @itemId
	if 10 = -1
		if flag = 0
			add_sys_msg_start
			add_sys_msg �����ʱװ�����ڵ�Ҳ���ԣ�
			set_sys_msg_mode 4 0 0
			msg_info
			return
		endif
	endif
	
	GET_COMMIT_ITEM_INFO 2 pos
	GET_COMMIT_ITEM_INFO 3 index
	
	DEC_ITEM_NUM_BYIDX @itemId 1 @pos @index ret
	if ret = 0
	    ADD_ITEM_NUM_INPAK 5052116 1 1 13
		
		add_sys_msg_start
		add_sys_msg ��л���ľ���������һ���˶���ħ����ʾ��л��
		set_sys_msg_mode 3 0 0
		msg_info
	endif
	PLAYER_CLOSE_DIALOG 0 0
endproc

;������������С�ݣ����˸�����
proc 211
	ENTER_PWORLD 209 0 1
endproc


;������������С�ݣ����鸱����
proc 212
	ENTER_PWORLD 210 0 1
endproc

proc 214
	DIALOG 1
	add_talk $_npc$��
	add_talk �����ݾ��ϣ�
	add_talk #b#b�����ݱ�����(ɱ��)֮�󣬾ͻ�������һ�����ݣ�Ҳ����˵����������������������ǲ������!
	add_talk #b#b
	add_talk �����������α����
	add_talk #b#b#b#b#cff0080ʯͷ#n �� #cff4e00���� #n
	add_talk #b#b#b#b#cff4e00����#n �� #c0181f9����#n
	add_talk #b#b#b#b#c0181f9����#n �� #cff0080ʯͷ#n
	add_talk #b#b
	add_talk ʤ��������
	add_talk #b#b����ʱ����ʱ����������#c186506���е����ݹ���ͬһ����(��ɫ)
	option 215 ������������С�ݣ����˸�����
	option 212 #�������������ݣ����鸱����
	
	talk
endproc

proc 215
	choose 1 211 216 ��������С�ݣ����˸�������ÿ��ֻ�ɽ���1�Σ��Ƿ����̽��룿
endproc

proc 216
	call 214
endproc
