;�þ�NPC��
proc 1
	is_captain 10 0
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b��һ��δ֪�����������ǰ,��׼��������?
	;�жϽ�ɫ�Ƿ��Ƕӳ�
	if @10 = 0
		get_unreal_layer 1 20
		if @20 < 110
		option 100 #ǰ����һ��
		endif
		var 30 @20
		MOD @30 10
		if @30 = 0 AND @20 >= 20
			DEV @20 10
			SUB @20 1
			SWITCH @20
				CASE 1
			;		option 200 #ǰ��BOSS��ͼ - ����֮��
				endcase
				CASE 2
			;		option 200 #ǰ��BOSS��ͼ - ̰��֮��
				endcase
				CASE 3
			;		option 200 #ǰ��BOSS��ͼ - ����֮��
				endcase
				CASE 4
			;		option 200 #ǰ��BOSS��ͼ - ��ŭ֮��
				endcase
				CASE 5
			;		option 200 #ǰ��BOSS��ͼ - ����֮��
				endcase
				CASE 6
			;		option 200 #ǰ��BOSS��ͼ - �ʼ�֮��
				endcase
				CASE 7
			;		option 200 #ǰ��BOSS��ͼ - ����֮��
				endcase
				CASE 8
			;		option 200 #ǰ��BOSS��ͼ - ��թ֮��
				endcase
				CASE 9
			;		option 200 #ǰ��BOSS��ͼ - ����֮��
				endcase
				CASE 10
			;		option 200 #ǰ��BOSS��ͼ - ԭ��֮��
				endcase
			ENDSWITCH
		endif
	endif
	
	option 300 #����ʱ���
	talk	
endproc 

proc 100
	strvar Maxlv
	var Maxlv 0
	
	strvar Layer
	var Layer 0
	
	strvar Rflag
	var Rflag 0

	is_captain 10 0
	if @10 = 0
		GET_CUR_MAX_LEVEL Rflag Maxlv
		get_unreal_layer 1 Layer
		
		if Maxlv >= 80
			SUB Maxlv 5
			
			if @Rflag = 0 and @Maxlv > @Layer
				UNREAL_ENTER 0 1
			else
				add_sys_msg_start
				add_sys_msg	��һ��þ����ڵȼ���ӡ����󿪷�
				set_sys_msg_mode 4 0 0
				msg_info
			endif
		elseif Maxlv < 80
			SUB Maxlv 10
			
			if @Rflag = 0 and @Maxlv > @Layer
				UNREAL_ENTER 0 1
			else
				add_sys_msg_start
				add_sys_msg	��һ��þ����ڵȼ���ӡ����󿪷�
				set_sys_msg_mode 4 0 0
				msg_info
			endif
		else
		endif
	else
		add_sys_msg_start
		add_sys_msg	��ϧ�ˣ��������Ѳ��Ƕӳ����޷������鴫����һ���ˡ�
		set_sys_msg_mode 4 0 0
		msg_info
	endif
endproc

proc 200
	UNREAL_ENTER_BOSS_CHECK 1
	if @1 = -1
		return
	endif
	choose 0 210 220 �����˴�ħ��Я���������أ������������ð�յ���Ҷ��飬PK�޳ͷ�����������룡
endproc

proc 210
	UNREAL_ENTER_BOSS_CHECK 1
	if @1 = -1
		return
	endif
	get_unreal_layer 1 10
		
	DEV @10 10
	SUB @10 1
	
	SWITCH @10
		CASE 1
			TEAM_TRANS_INSAMEMAPINST 2600 4732	3620 0
		endcase
		CASE 2
			TEAM_TRANS_INSAMEMAPINST 2601 4801	3716 0
		endcase
		CASE 3
			TEAM_TRANS_INSAMEMAPINST 2602 4732	3620 0
		endcase
		CASE 4
			TEAM_TRANS_INSAMEMAPINST 2603 4801	3716 0
		endcase
		CASE 5
			TEAM_TRANS_INSAMEMAPINST 2604 4732	3620 0
		endcase
		CASE 6
			TEAM_TRANS_INSAMEMAPINST 2605 4801	3716 0
		endcase
		CASE 7
			TEAM_TRANS_INSAMEMAPINST 2606 4732	3620 0
		endcase
		CASE 8
			TEAM_TRANS_INSAMEMAPINST 2607 4801	3716 0
		endcase
		CASE 9
			TEAM_TRANS_INSAMEMAPINST 2608 4732	3620 0
		endcase
		CASE 10
			TEAM_TRANS_INSAMEMAPINST 2609 4801	3716 0
		endcase
	ENDSWITCH
endproc

proc 220
	return
endproc

proc 300
	choose 0 301 302 ȷ������ʱ�����?
endproc

proc 301
	is_captain 20 0
	if @20 = 0
		TEAM_TRANS_INSAMEMAPINST 1000 40765 35339 0
	else
		TRANSMIT 1000 40765 35339 0
	endif
endproc

proc 302

endproc


