;��������ݸ�������ģʽ

;���������¼�
PROC 1
	;11��¼ʱ��
	SET_PWORLD_VAR 0 32 0 1
	;13�����ٻ����� ��8��
	SET_PWORLD_VAR 4 32 0 1
	;npc��ʱ�԰�
	SET_PWORLD_VAR 8 32 0 1
	;npc�԰�   4��˵һ��
	SET_PWORLD_VAR 12 32 0 1
	;�԰׽�����־
	SET_PWORLD_VAR 16 32 0 1
	
	add_sys_msg_start
	add_sys_msg ���һ�����죬�����ʯͷ�����������������ǳ���
	set_sys_msg_mode 4 0 0
	MAP_MSG_INFO 
ENDPROC

;�رո����¼�
proc 2
endproc

;������ʱ�¼���ÿ2�봥��һ�Σ�
proc 3
   IS_PWORLD_OVER 30
	if @30 != 1
		GET_PWORLD_VAR 4 32 1 10
		if @10 >= 8
			PWORLD_PLAYER_EXP 0 2
		else
			GET_PWORLD_VAR 0 32 1 20
			if @20 >= 12
				SET_PWORLD_VAR 0 32 0 1
				rand 11 6
				strvar x1 y1 x2 y2 x3 y3
				strvar x4 y4 x5 y5 x6 y6
				switch 11
					case 0
						var x1 4404
						var y1 2609
						var x2 4401
						var y2 3543
						var x3 4300
						var y3 4709
						var x4 4414
						var y4 5736
						var x5 5416
						var y5 4543
						var x6 5335
						var y6 5775
					endcase

					case 1
						var x1 6699
						var y1 5762
						var x2 7608
						var y2 5710
						var x3 8205
						var y3 4881
						var x4 8267
						var y4 3736
						var x5 8368
						var y5 2262
						var x6 7369
						var y6 2363
					endcase

					case 2
						var x1 7380
						var y1 3145
						var x2 6561
						var y2 3109
						var x3 5827
						var y3 2560
						var x4 6493
						var y4 4099
						var x5 7290
						var y5 4303
						var x6 7034
						var y6 5116
					endcase
					
					case 3
						var x1 8882
						var y1 3783
						var x2 4960
						var y2 3288
						var x3 4537
						var y3 4198
						var x4 4915
						var y4 5287
						var x5 4404
						var y5 2609
						var x6 5416
						var y6 4543
					endcase
					
					case 4
						var x1 7034
						var y1 5116
						var x2 7369
						var y2 2363
						var x3 4537
						var y3 4198
						var x4 4915
						var y4 5287
						var x5 4404
						var y5 2609
						var x6 8882
						var y6 3783
					endcase
					
					case 5
						var x1 8882
						var y1 3783
						var x2 4960
						var y2 3288
						var x3 4537
						var y3 4198
						var x4 4915
						var y4 5287
						var x5 4404
						var y5 2609
						var x6 4300
						var y6 4709
					endcase
				endswitch
				NEW_MON 230052 1 @x1 @y1 0 0 0 0 0
				NEW_MON 230053 1 @x2 @y2 0 0 0 0 0
				NEW_MON 230054 1 @x3 @y3 0 0 0 0 0
				NEW_MON 230052 1 @x4 @y4 0 0 0 0 0
				NEW_MON 230053 1 @x5 @y5 0 0 0 0 0
				NEW_MON 230054 1 @x6 @y6 0 0 0 0 0
				add 10 1
				SET_PWORLD_VAR 4 32 @10 1
				add_sys_msg_start
				add_sys_msg ���һ�����죬�����ʯͷ�����������������ǳ���
				set_sys_msg_mode 4 0 0
				MAP_MSG_INFO 
			else
				add 20 1
				SET_PWORLD_VAR 0 32 @20 1
			endif
		endif
		
		call 101
		
	endif
endproc

;�����й��������¼�
proc 4

endproc

;�����н�ɫ�����¼�
proc 5

endproc

;�����н�ɫ�����¼�
proc 6

endproc

;�����е���ʱ�����¼�
proc 7
endproc 

;�������ʼ�¼�
proc 8
endproc 


proc 101
	;��ʱ�԰�
		GET_PWORLD_VAR 12 32 1 60
		if @60 < 1
			add 60 1
			SET_PWORLD_VAR 12 32 @60 1
		else
			GET_PWORLD_VAR 16 32 1 70
			if @70 = 0
				GET_PWORLD_VAR 8 32 1 40
				if @40 < 6 
					add 40 1
					SET_PWORLD_VAR 8 32 @40 1
					call 100
				endif
			
				SET_PWORLD_VAR 12 32 0 1
			else
				return
			endif
		endif
endproc

proc 100
	strvar npcmid
	GET_MAP_NPC_ID  302161 npcmid 1
	GET_PWORLD_VAR 8 32 1 50
	if @50 = 1
		MON_SPEECH  @npcmid 0 2 1 ��ӭ�������������ݣ��������Ӯ������ϸ����˵��
	elseif @50 = 2
		MON_SPEECH  @npcmid 0 2 1 ÿ��һ��ʱ������ͻ����µ����ݳ��֣���Ҫ���ľ��Ǵ�������
	elseif @50 = 3
		MON_SPEECH  @npcmid 0 2 1 �ü��ܻ�ɱ���ݣ�ʹ���Ǳ��ͬһ����ɫ��ͬһ�����ݣ�
	elseif @50 = 4
		MON_SPEECH  @npcmid 0 2 1 ����������ʱ�������ڵ���������ͬһ�֣�����ʤ���ˣ�
	elseif @50 = 5
		MON_SPEECH  @npcmid 0 2 1 ����������60ֻ��ͬ������ʱ��������ǰ��������Ҳ��ʤ��Ӵ��
	elseif @50 = 6
		MON_SPEECH  @npcmid 0 2 1 ���ڣ����ʼ�ˣ���סӴ��ÿ��һ��ʱ��ͻ����µ����ݳ��֡�
	else
		SET_PWORLD_VAR 16 32 0 1
	endif
endproc
