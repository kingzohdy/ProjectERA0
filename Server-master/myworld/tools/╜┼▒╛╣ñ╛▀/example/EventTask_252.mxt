;���뿪�Ĵǵ����ص��ű�
;3��26�����ӵ��ش���3��������
;----------
;���
proc 1
	skip_check
	get_level_answer_detail 5 21
	if 21 < 20
		playshow 2 6 0
		call 6
		msg_player 3 2 ��ϲ��ԣ���á�$@5$�����顣
	else
		call 6
		finish_task 252
		finish_quiz_round
		get_var 194 16 200
		if 200 >= 0 and 200 < 60000
			add 200 2
			set_var 194 16 @200
		endif
		actlog 12 2 @950 ��ɻ��뿪�Ĵʵ�
		playshow 2 10 0
		get_var 132 0 9
		if 9 = 1
			call 7
			msg_player 3 2 ��ϲ��ɱ��ִ��⣬������һ�ִ��⽫�ܻ��/C05$@5$/C00�ܾ��飬��Ҫ����1������Կ�ס�
		elseif 9 = 2
			call 9
			msg_player 3 2 ��ϲ��ɱ��ִ��⣬������һ�ִ��⽫�ܻ��/C05$@5$/C00�ܾ��飬��Ҫ����1������Կ�ס�
		else
			msg_player 3 2 ��ϲ��ɱ��ִ��⣬�����Ĵ�������Ѿ�ȫ�����꣡
		endif
	endif
endproc 
;----------------
;���
proc 2
	get_level_answer_detail 5 21
	if 21 < 20
		playshow 2 7 0
		msg_player 3 2 ����ˣ�
	else
		finish_task 252
		finish_quiz_round
		get_var 194 16 200
		if 200 >= 0 and 200 < 60000
			add 200 2
			set_var 194 16 @200
		endif
		actlog 12 2 @950 ��ɻ��뿪�Ĵʵ�
		playshow 2 10 0
		get_var 132 0 9
		if 9 = 1
			call 7
			msg_player 3 2 ���ִ��������������һ�ִ��⽫�ܻ��/C05$@5$/C00�ܾ��飬��Ҫ����1������Կ�ס�
		elseif 9 = 2
			call 9
			msg_player 3 2 ���ִ��������������һ�ִ��⽫�ܻ��/C05$@5$/C00�ܾ��飬��Ҫ����1������Կ�ס�
		else
			msg_player 3 2 ��������������Ĵ�������Ѿ�ȫ�����꣡
		endif
	endif
endproc 
;---------------
;---------
proc 6
;ÿ����������齱����ʽ
;20��22	��(228*lv*lv+11080*lv-157950)/120��*0.5			
;23��35	��(228*lv*lv+11080*lv-157950)/80��*0.5			
;36��52	��(228*lv*lv+11080*lv-157950)/60��*0.5			
;53��60	��(228*lv*lv+11080*lv-157950)/40��*0.5			
;61-69	��(228*lv*lv+11080*lv-157950)/35)*0.5			
;70��80	��(228*lv*lv+11080*lv-157950)/30��*0.5					
;ÿ�����Ч��
;����Ч��	1	2	3	4	5	6	7	8	9	10
;��һ�����	1.5	1.6	1.7	1.8	1.9	2	2.1	2.2	2.3	2.4
;�ڶ����շ�	2.5	2.6	2.7	2.8	2.9	3	3.1	3.2	3.3	3.4
;�������շ�	3.5	3.6	3.7	3.8	3.9	4	4.1	4.2	4.3	4.4
			
	get_var 132 0 9
	if 9 = 1
		if 21 = 1 or 21 = 2
			call 8
			mul 5 15
			dev 5 20
			add_exp @5
			return 
		elseif 21 = 3 or 21 = 4
			call 8
			mul 5 16
			dev 5 20
			add_exp @5
			return 
		elseif 21 = 5 or 21 = 6
			call 8
			mul 5 17
			dev 5 20
			add_exp @5
			return 
		elseif 21 = 7 or 21 = 8
			call 8
			mul 5 18
			dev 5 20
			add_exp @5
			return 
		elseif 21 = 9 or 21 = 10
			call 8
			mul 5 19
			dev 5 20
			add_exp @5
			return 
		elseif 21 = 11 or 21 = 12
			call 8
			mul 5 20
			dev 5 20
			add_exp @5
			return 
		elseif 21 = 13 or 21 = 14
			call 8
			mul 5 21
			dev 5 20
			add_exp @5
			return 
		elseif 21 = 15 or 21 = 16
			call 8
			mul 5 22
			dev 5 20
			add_exp @5
			return 
		elseif 21 = 17 or 21 = 18
			call 8
			mul 5 23
			dev 5 20
			add_exp @5
			return 
		else
			call 8
			mul 5 24
			dev 5 20
			add_exp @5
			return 
		endif
	elseif 9 = 2
		if 21 = 1 or 21 = 2
			call 8
			mul 5 25
			dev 5 20
			add_exp @5
			return 
		elseif 21 = 3 or 21 = 4
			call 8
			mul 5 26
			dev 5 20
			add_exp @5
			return 
		elseif 21 = 5 or 21 = 6
			call 8
			mul 5 27
			dev 5 20
			add_exp @5
			return 
		elseif 21 = 7 or 21 = 8
			call 8
			mul 5 28
			dev 5 20
			add_exp @5
			return 
		elseif 21 = 9 or 21 = 10
			call 8
			mul 5 29
			dev 5 20
			add_exp @5
			return 
		elseif 21 = 11 or 21 = 12
			call 8
			mul 5 30
			dev 5 20
			add_exp @5
			return 
		elseif 21 = 13 or 21 = 14
			call 8
			mul 5 31
			dev 5 20
			add_exp @5
			return 
		elseif 21 = 15 or 21 = 16
			call 8
			mul 5 32
			dev 5 20
			add_exp @5
			return 
		elseif 21 = 17 or 21 = 18
			call 8
			mul 5 33
			dev 5 20
			add_exp @5
			return 
		else
			call 8
			mul 5 34
			dev 5 20
			add_exp @5
			return 
		endif
	else
		if 21 = 1 or 21 = 2
			call 8
			mul 5 35
			dev 5 20
			add_exp @5
			return 
		elseif 21 = 3 or 21 = 4
			call 8
			mul 5 36
			dev 5 20
			add_exp @5
			return 
		elseif 21 = 5 or 21 = 6
			call 8
			mul 5 37
			dev 5 20
			add_exp @5
			return 
		elseif 21 = 7 or 21 = 8
			call 8
			mul 5 38
			dev 5 20
			add_exp @5
			return 
		elseif 21 = 9 or 21 = 10
			call 8
			mul 5 39
			dev 5 20
			add_exp @5
			return 
		elseif 21 = 11 or 21 = 12
			call 8
			mul 5 40
			dev 5 20
			add_exp @5
			return 
		elseif 21 = 13 or 21 = 14
			call 8
			mul 5 41
			dev 5 20
			add_exp @5
			return 
		elseif 21 = 15 or 21 = 16
			call 8
			mul 5 42
			dev 5 20
			add_exp @5
			return 
		elseif 21 = 17 or 21 = 18
			call 8
			mul 5 43
			dev 5 20
			add_exp @5
			return 
		else
			call 8
			mul 5 44
			dev 5 20
			add_exp @5
			return 
		endif
	endif
endproc 
;-------
proc 7
	call 8
	mul 5 295
	dev 5 10
	ADD_GOODS 26580 1 1
	IF 1 = 0
		MSG_PLAYER 3 2 ��ϲ��á��á������ƣ�
	ENDIF
	SET_RANK 6009 0 0
	MSG_PLAYER 3 2 ��ϲ��á�����С��ʿ���ƺţ�
	RAND 999 1000
	IF 999 = 0
		GET_SYS_VAR 14 0 998
		IF 998 < 2
			ADD_GOODS 25634 1 1
			ADD @998 1
			SET_SYS_VAR 14 0 @998
			MSG_PLAYER 3 2 ��ϲ��á���������֮�ǡ���
			MSG_WORLD 103 24 ��ϲ$_player$��á���������֮�ǡ���
		ELSE
			ADD_GOODS 10269 1 1
			MSG_PLAYER 3 2 ��ϲ�������ʯ��
		ENDIF
	ELSEIF 999 >= 1 and 999 <= 300
		ADD_GOODS 10269 1 1
		MSG_PLAYER 3 2 ��ϲ�������ʯ��
	ELSEIF 999 >= 301 and 999 < 500
		ADD_GOODS 25152 1 1
		MSG_PLAYER 3 2 ��ϲ�������ƴͼ��
	ELSEIF 999 >= 500 and 999 < 600
		ADD_GOODS 12026 1 1
		MSG_PLAYER 3 2 ��ϲ����ٹⱦʯ��
	ELSEIF 999 >= 600 and 999 < 750
		ADD_GOODS 10671 1 1
		MSG_PLAYER 3 2 ��ϲ��û����裡
	ELSEIF 999 >= 750 and 999 < 900
		ADD_GOODS 10674 1 1
		MSG_PLAYER 3 2 ��ϲ���ˮħ��Ƭ��
	ELSE
		ADD_GOODS 3634 1 1
		MSG_PLAYER 3 2 ��ϲ�����ң�
	ENDIF
	return
endproc 
;--------
proc 8
	get_level 5
		if 5 >= 20 and 5 < 23
			mul 5 @5
			mul 5 228
			
			get_level 6
			mul 6 11080
			
			add 5 @6
			sub 5 157950
			
			dev 5 240
			return
		endif
		;
		if 5 >= 23 and 5 < 36
			mul 5 @5
			mul 5 228
			
			get_level 6
			mul 6 11080
			
			add 5 @6
			sub 5 157950
			
			dev 5 160
			return
		endif
		;
		if 5 >= 36 and 5 < 53
			mul 5 @5
			mul 5 228
			
			get_level 6
			mul 6 11080
			
			add 5 @6
			sub 5 157950
			
			dev 5 120
			return
		endif
		;
		if 5 >= 53 and 5 < 61
			mul 5 @5
			mul 5 228
			
			get_level 6
			mul 6 11080
			
			add 5 @6
			sub 5 157950
			
			dev 5 80
			return
		endif 
		;
		if 5 >= 61 and 5 < 70
				mul 5 @5
			mul 5 228
			
			get_level 6
			mul 6 11080
			
			add 5 @6
			sub 5 157950
			
			dev 5 70
			return
		endif
		;
		if 5 >= 70 and 5 <= 80
			mul 5 @5
			mul 5 228
			
			get_level 6
			mul 6 11080
			
			add 5 @6
			sub 5 157950
			
			dev 5 60
			return
		endif
endproc 
;----------
proc 9
	call 8 
	mul 5 395
	dev 5 10
	return
endproc 