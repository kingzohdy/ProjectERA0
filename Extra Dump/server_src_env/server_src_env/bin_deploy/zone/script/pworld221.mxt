;�����ű� - ��Ӱ֮ս

;���������¼�
PROC 1
	NEW_MON_BY_DIR 201063 1 5729 6990 0 0 0 0 0 0
	NEW_MON_BY_DIR 201063 1 12151 6986 0 0 0 0 0 0
	var 10 0
	;10���û��ʼ��ʶ
	SET_PWORLD_VAR 0 32 @10 1
	;11���õ�һ��ˢ�ֱ�ʶ
	var 11 0
	SET_PWORLD_VAR 4 32 @11 1
ENDPROC

;�رո����¼�
proc 2
	
endproc

;������ʱ�¼���ÿ2�봥��һ�Σ�
proc 3
	GET_PWORLD_VAR 0 32 1 10
	if 10 = 1
	
		GET_PWORLD_VAR 4 32 1 20
		if 20 = 12
			rand 11 3
			strvar x1 y1 x2 y2
			switch 11
				case 0
					var x1 5167
					var y1 10036
					var x2 12227
					var y2 10393
				endcase

				case 1
					var x1 6059
					var y1 11385
					var x2 13941
					var y2 12023
				endcase

				case 2
					var x1 3674
					var y1 8063
					var x2 14166
					var y2 9157
				endcase
			endswitch
			NEW_MON 260100 1 @x1 @y1 800 0 0 0 0
			NEW_MON 260101 1 @x2 @y2 800 0 0 0 0
		else
			add 20 1
			SET_PWORLD_VAR 4 32 @20 1
		endif
		
	endif
endproc

;�����й��������¼�
proc 4

endproc

;�����н�ɫ�����¼�
proc 5

	EFFECT 2 1 SetMessageTips(310)

endproc

;�����н�ɫ�����¼�
proc 6

endproc

;�����е���ʱ�����¼�
proc 7
	DESTROY_MON_BY_DEFID 1 260100
	DESTROY_MON_BY_DEFID 1 260101
endproc 

;�������ʼ�¼�
proc 8
	ADD_SYS_MSG_START
	SET_SYS_MSG_MODE 6 0 0
	ADD_SYS_MSG ���ʼ��
	MAP_MSG_INFO
	var 10 1
	SET_PWORLD_VAR 0 32 @10 1
	DESTROY_MON_BY_DEFID 1 201063
	
	NEW_MON 260100 10 5167 10036 1400 0 0 0 0
	NEW_MON 260101 10 12227 10393 1400 0 0 0 0
endproc 