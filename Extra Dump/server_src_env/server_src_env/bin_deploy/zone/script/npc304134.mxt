;npcƴ��
;4501 ����ID
;304134 NPCID

proc 1
	GET_TASK_VAR 4501 4 32 12 res
	;���İ���
	if @res = 1 and @12 = 0
		choose 18 207 208 ��������ƺȵİɣ����˵�һ��ƴ�����⣬������ÿ�ζ�Ҫ����2000���������ȷ����ʼƴ�ƣ�
		return
	endif
	
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#bû������Ͳ�ļһｻ���ѣ������ǵ���������Ǿ�����
	
	TASK_STAT 4501 1 70
	if @70 = 2
		option 206 ��ʼƴ��
	endif
	
	TASK_STAT 4573 1 71
	if @71 = 2
		option 210 ��ʼ����
	endif
	
	TASK_STAT 4574 1 72
	if @72 = 2
		option 220 ��ʼ����
	endif
	
	TASK_STAT 4575 1 73
	if @73 = 2
		option 230 ��ʼ����
	endif
	
	TASK_STAT 4576 1 74
	if @74 = 2
		option 240 ��ʼ����
	endif
	
	TASK_STAT 4577 1 75
	if @75 = 2
		option 250 ��ʼ����
	endif
		;TASK_ADD_OPTION 4501 0 103
		;TASK_ADD_OPTION 4517 0 104
		;TASK_ADD_OPTION 4573 0 105
		;TASK_ADD_OPTION 4574 0 106
		;TASK_ADD_OPTION 4575 0 107
		;TASK_ADD_OPTION 4576 0 108
		;TASK_ADD_OPTION 4577 0 109
	talk
endproc


proc 2
	get_trig_task 1 2

	if 2 = 4501
		DIALOG 1
		add_talk $_npc$:
		add_talk #b#bû������Ͳ�ļһｻ���ѣ������ǵ���������Ǿ�����

		option 206 ��ʼƴ��
		talk
	endif
	
	if 2 = 4573
		DIALOG 1
		add_talk $_npc$:
		add_talk #b#bû������Ͳ�ļһｻ���ѣ������ǵ���������Ǿ�����

		option 210 ��ʼ����
		talk
	endif
	
	if 2 = 4574
		DIALOG 1
		add_talk $_npc$:
		add_talk #b#bû������Ͳ�ļһｻ���ѣ������ǵ���������Ǿ�����

		option 220 ��ʼ����
		talk
	endif
	
	if 2 = 4575
		DIALOG 1
		add_talk $_npc$:
		add_talk #b#bû������Ͳ�ļһｻ���ѣ������ǵ���������Ǿ�����

		option 230 ��ʼ����
		talk
	endif
	
	if 2 = 4576
		DIALOG 1
		add_talk $_npc$:
		add_talk #b#bû������Ͳ�ļһｻ���ѣ������ǵ���������Ǿ�����

		option 240 ��ʼ����
		talk
	endif
	
	if 2 = 4577
		DIALOG 1
		add_talk $_npc$:
		add_talk #b#bû������Ͳ�ļһｻ���ѣ������ǵ���������Ǿ�����

		option 250 ��ʼ����
		talk
	endif
endproc

;proc 103
;	TASK_DO_OPTION 4501 0
;endproc

;proc 104
;	TASK_DO_OPTION 4517 0
;endproc

;proc 105
;	TASK_DO_OPTION 4573 0
;endproc

;proc 106
;	TASK_DO_OPTION 4574 0
;endproc

;proc 107
;	TASK_DO_OPTION 4575 0
;endproc

;proc 108
;	TASK_DO_OPTION 4576 0
;endproc

;proc 109
;	TASK_DO_OPTION 4577 0
;endproc

proc 100
	add_sys_msg_start
	add_sys_msg ���Ѿ�������
	set_sys_msg_mode 4 0 0
	msg_info
	
	;֪ͨ�ͻ��˽�����Ļģ��
	pinjiu_screen_vague 1
	
	PLAYER_CLOSE_DIALOG1 0 1
	
	strvar npcmid
	GET_MAP_NPC_ID  304134 npcmid 1
	MON_SPEECH  @npcmid 0 2 1 С��������ƴ�ƣ���Ű�أ�
	
	SET_TASK_VAR 4501 0 32 60 10
	SET_TASK_VAR 4501 4 32 1 16
	SET_TASK_VAR 4501 8 32 0 26
	SET_TASK_VAR 4501 12 32 0 27
	SET_TASK_VAR 4501 16 32 0 28
endproc

;���Ӯ��
proc 101
	add_sys_msg_start
	add_sys_msg $_npc$�Ѿ���Ĳ�ʡ�����ˣ���Ӯ�ˣ�
	set_sys_msg_mode 4 0 0
	msg_info
	
	task_fini 4501 1
endproc

;�������
proc 102
	add_sys_msg_start
	add_sys_msg ��ķ���ǳ�������ǧ�������أ���źȵ��İ���������̫С�������ˣ�
	set_sys_msg_mode 4 0 0
	msg_info
	
	SET_TASK_VAR 4501 0 32 60 10
	SET_TASK_VAR 4501 4 32 1 16
	SET_TASK_VAR 4501 8 32 0 26
	SET_TASK_VAR 4501 12 32 0 27
	SET_TASK_VAR 4501 16 32 0 28
endproc

;����һ��
proc 201
	;ÿ�ε��������һ����ֻ�� x% ��䣬ÿ�ε���ֵ��� 1-12�������60%��
	rand 20 12
	add 20 1
	
	GET_TASK_VAR 4501 0 32 11 num
	add num @20
	SET_TASK_VAR 4501 0 32 @num 10
	
	if @num > 100
		call 100
	else
		call 206
	endif
endproc

;�����ˣ�����������ɣ�
proc 202
	strvar jiuliangval
	;�������ֵ
	GET_TASK_VAR 4501 16 32 25 jiuliangval
	GET_TASK_VAR 4501 0 32 11 num	
	if @jiuliangval < @num and @num <= 100
	;���Ӯ��
		call 101
	else
	;�������
		call 102
	endif
endproc


proc 206

	strvar num res jiuliang flag
	GET_TASK_VAR 4501 0 32 11 num
	if @num = 0
		SET_TASK_VAR 4501 0 32 60 10
		var num 60
	endif
	
	;idx 12 ��¼npc���������־   flag Ϊ 1ʱ����ʾ�Ѿ�ȡ��  ,0ʱ��������ȡ
	GET_TASK_VAR 4501 12 32 21 flag 
	if @flag = 0
		;npc����  90-97
		rand 14 8
		add 14 90
		
		if @14 < 95 and @14 >= 90
			;��ķ���������ߣ��������ƺ������׾ͻ����
			SET_TASK_VAR 4501 8 32 1 10
		elseif @14 >= 95 and @14 <= 97
			;��ķ���˴�����Խ��Խ�������������ƺ��ܹ�ǧ������
			SET_TASK_VAR 4501 8 32 2 10
		else
			SET_TASK_VAR 4501 8 32 0 10
		endif
		
		SET_TASK_VAR 4501 12 32 1 22
		;idx Ϊ 16 ���þ���ֵ
		SET_TASK_VAR 4501 16 32 @14 23
	endif
	
	
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#bһ�����¶ǣ����Ѿ���$@num$%�����⣬��Ҫ��������
	
	;idx 8 ����ʾ�ľ�   
	GET_TASK_VAR 4501 8 32 15 jiuliang
	if @jiuliang = 1
		add_talk #b#b��ķ���������ߣ��������ƺ������׾ͻ����
	elseif @jiuliang = 2
		add_talk #b#b��ķ���˴�����Խ��Խ�������������ƺ��ܹ�ǧ������
	else
	endif
	
	option 201 ����һ��
	option 202 #�����ˣ�����������ɣ�
	
	talk
endproc

proc 207
	DEC_MONEY_TEST 0 2000 21 15
	if 15 = 0
		DEC_MONEY 0 2000 21 12
		if 12 = 0		
			SET_TASK_VAR 4501 4 32 0 16
			
			call 1
		else
			add_sys_msg_start
			add_sys_msg ���İ������㡣
			set_sys_msg_mode 4 0 0
			msg_info
			return
		endif
	endif			
endproc

proc 208
	return
endproc



;ѡ����
proc 200
	TASK_STAT 4573 1 71
	if @71 = 2
		var 430 4573
	endif
	
	TASK_STAT 4574 1 72
	if @72 = 2
		var 430 4574
	endif
	
	TASK_STAT 4575 1 73
	if @73 = 2
		var 430 4575
	endif
	
	TASK_STAT 4576 1 74
	if @74 = 2
		var 430 4576
	endif
	
	TASK_STAT 4577 1 75
	if @75 = 2
		var 430 4577
	endif
	
	;��Դ���
	GET_TASK_VAR @430 0 32 1 80

	if @80 >= 3
		TASK_FINI @430 1
		PLAYER_CLOSE_DIALOG1 0 0
	else
		GET_TASK_VAR @430 4 32 1 90
		if @90 = 1
			;��Դ�����0
			SET_TASK_VAR @430 0 32 0 1
			;�����0
			SET_TASK_VAR @430 4 32 0 1
		endif
	
		;20����
		rand 12 20
		switch 12
			case 0
				call 300
			endcase

			case 1
				call 301
			endcase

			case 2
				call 302
			endcase
					
			case 3
				call 303
			endcase
					
			case 4
				call 304
			endcase
					
			case 5
				call 305
			endcase
					
			case 6
				call 306
			endcase
					
			case 7
				call 307
			endcase
					
			case 8
				call 308
			endcase
					
			case 9
				call 309
			endcase

			case 10
				call 310
			endcase
					
			case 11
				call 311
			endcase
					
			case 12
				call 312
			endcase
					
			case 13
				call 313
			endcase
			
			case 14
				call 314
			endcase
			
			case 15
				call 315
			endcase
			
			case 16
				call 316
			endcase
			
			case 17
				call 317
			endcase
			
			case 18
				call 318
			endcase
			
			case 19
				call 319
			endcase
		endswitch
	endif
endproc

proc 210 
	call 200
endproc

proc 220 
	call 200
endproc

proc 230 
	call 200
endproc

proc 240 
	call 200
endproc

proc 250
	call 200
endproc



;ѡ��ż����Ϊ����ģ�����401Ϊ��ȷ��
proc 300
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b����ϸ�Ķ�������Ŀ������������������һ�⣬�����¿�ʼ���⣬���������������������
	add_talk #b#b
	add_talk #b#b#c186506��������Ƶ�������ڵͼ۽������������һ�Ҫ�����칤�ߵģ�����ô�죿#n
	
	rand 12 4
	if @12 = 0
		option 400 ���ϼ�֮���б��˲�ռ�Ǳ���
		option 401 ���С��ͼ���Ͻǵ�BUG�����ٱ���
		option 400 �Ͻ���ϵ����Ҫ�绰����չ�ɳ��ں�����ϵ
		option 400 �����ܲ����ٱ��˵�
	elseif @12 = 1
		option 401 ���С��ͼ���Ͻǵ�BUG�����ٱ���
		option 400 �Ͻ���ϵ����Ҫ�绰����չ�ɳ��ں�����ϵ
		option 400 �����ܲ����ٱ��˵�
		option 400 ���ϼ�֮���б��˲�ռ�Ǳ���
	elseif @12 = 2
		option 400 �Ͻ���ϵ����Ҫ�绰����չ�ɳ��ں�����ϵ
		option 400 �����ܲ����ٱ��˵�
		option 400 ���ϼ�֮���б��˲�ռ�Ǳ���
		option 401 ���С��ͼ���Ͻǵ�BUG�����ٱ���
	else
		option 400 �����ܲ����ٱ��˵�
		option 400 ���ϼ�֮���б��˲�ռ�Ǳ���
		option 401 ���С��ͼ���Ͻǵ�BUG�����ٱ���
		option 400 �Ͻ���ϵ����Ҫ�绰����չ�ɳ��ں�����ϵ
	endif
	talk
endproc

proc 301
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b����ϸ�Ķ�������Ŀ������������������һ�⣬�����¿�ʼ���⣬���������������������
	add_talk #b#b
	add_talk #b#b#c186506������˸���˵��½ĳ�ǹٷ���վ�ܹ���ȡ������Ϸ���ߡ���ҡ����ҡ����顢��������ô����#n
	
	rand 12 4
	if @12 = 0
	option 402 ���������ܲ��ܶ��һЩ
	option 401 ���С��ͼ���Ͻǵ�BUG�����ٱ���
	option 402 ��ͼ����Ⱥ����֮���ѣ���Ա����һ��ȥ��
	option 402 ����ȥ��
	elseif @12 = 1
	option 401 ���С��ͼ���Ͻǵ�BUG�����ٱ���
	option 402 ��ͼ����Ⱥ����֮���ѣ���Ա����һ��ȥ��
	option 402 ����ȥ��
	option 402 ���������ܲ��ܶ��һЩ
	elseif @12 = 2
	option 402 ��ͼ����Ⱥ����֮���ѣ���Ա����һ��ȥ��
	option 402 ����ȥ��
	option 402 ���������ܲ��ܶ��һЩ
	option 401 ���С��ͼ���Ͻǵ�BUG�����ٱ���
	else
	option 402 ����ȥ��
	option 402 ���������ܲ��ܶ��һЩ
	option 401 ���С��ͼ���Ͻǵ�BUG�����ٱ���
	option 402 ��ͼ����Ⱥ����֮���ѣ���Ա����һ��ȥ��
	endif
	talk
endproc

proc 302
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b����ϸ�Ķ�������Ŀ������������������һ�⣬�����¿�ʼ���⣬���������������������
	add_talk #b#b
	add_talk #b#b#c186506�����˺ű�������ô�죿#n
	
	rand 12 4
	if @12 = 0
	option 404 ���½�һ���ţ����������ϴ��������
	option 404 ���͵��ˣ�����ν
	option 401 �󶨰�ȫ���䣬��ʱ��֤�����ܱ���
	option 404 �跨ȥ�����˵ĺ�
	elseif @12 = 1
	option 404 ���͵��ˣ�����ν
	option 401 �󶨰�ȫ���䣬��ʱ��֤�����ܱ���
	option 404 �跨ȥ�����˵ĺ�
	option 404 ���½�һ���ţ����������ϴ��������
	elseif @12 = 2
	option 401 �󶨰�ȫ���䣬��ʱ��֤�����ܱ���
	option 404 �跨ȥ�����˵ĺ�
	option 404 ���½�һ���ţ����������ϴ��������
	option 404 ���͵��ˣ�����ν
	else
	option 404 �跨ȥ�����˵ĺ�
	option 404 ���½�һ���ţ����������ϴ��������
	option 404 ���͵��ˣ�����ν
	option 401 �󶨰�ȫ���䣬��ʱ��֤�����ܱ���
	endif
	talk
endproc

proc 303
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b����ϸ�Ķ�������Ŀ������������������һ�⣬�����¿�ʼ���⣬���������������������
	add_talk #b#b
	add_talk #b#b#c186506������ֱ������˺������޷�ȡ�ػ򱻶�����ܱ�������ô�죿#n
	
	rand 12 4
	if @12 = 0
	option 406 һ��һ���ԣ�����һ���ǶԵ�
	option 406 ����Ҳ��ֵ����Ǯ����������һ����
	option 406 �����������Ҳû�취
	option 401 ����̳�����ⷴ������������GM����
	elseif @12 = 1	
	option 406 ����Ҳ��ֵ����Ǯ����������һ����
	option 406 �����������Ҳû�취
	option 401 ����̳�����ⷴ������������GM����
	option 406 һ��һ���ԣ�����һ���ǶԵ�
	elseif @12 = 2
	option 406 �����������Ҳû�취
	option 401 ����̳�����ⷴ������������GM����
	option 406 һ��һ���ԣ�����һ���ǶԵ�
	option 406 ����Ҳ��ֵ����Ǯ����������һ����
	else
	option 401 ����̳�����ⷴ������������GM����
	option 406 һ��һ���ԣ�����һ���ǶԵ�
	option 406 ����Ҳ��ֵ����Ǯ����������һ����
	option 406 �����������Ҳû�취
	endif
	talk
endproc

proc 304
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b����ϸ�Ķ�������Ŀ������������������һ�⣬�����¿�ʼ���⣬���������������������
	add_talk #b#b
	add_talk #b#b#c186506���Ԫ�Ĺ������ĸ���#n	
	
	rand 12 4
	if @12 = 0
	option 401 www.ljy0.com
	option 408 www.ljy1.com
	option 408 www.ljy2.com
	option 408 www.ljy3.com
	elseif @12 = 1
	option 408 www.ljy1.com
	option 408 www.ljy2.com
	option 408 www.ljy3.com
	option 401 www.ljy0.com
	elseif @12 = 2
	option 408 www.ljy2.com
	option 408 www.ljy3.com
	option 401 www.ljy0.com
	option 408 www.ljy1.com
	else
	option 408 www.ljy3.com
	option 401 www.ljy0.com
	option 408 www.ljy1.com
	option 408 www.ljy2.com
	endif
	talk
endproc

proc 305
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b����ϸ�Ķ�������Ŀ������������������һ�⣬�����¿�ʼ���⣬���������������������
	add_talk #b#b
	add_talk #b#b#c186506���Ԫ����������Щ���ݣ�#n
	
	rand 12 4
	if @12 = 0
	option 410 ��������
	option 401 ����������������»����Ϸ���ϡ�����
	option 410 ��cctv��ȫ���������Ϣ����������վ�������Ƶ
	option 410 �߻�������
	elseif @12 = 1
	option 401 ����������������»����Ϸ���ϡ�����
	option 410 ��cctv��ȫ���������Ϣ����������վ�������Ƶ
	option 410 �߻�������
	option 410 ��������
	elseif @12 = 2
	option 410 ��cctv��ȫ���������Ϣ����������վ�������Ƶ
	option 410 �߻�������
	option 410 ��������
	option 401 ����������������»����Ϸ���ϡ�����
	else
	option 410 �߻�������
	option 410 ��������
	option 401 ����������������»����Ϸ���ϡ�����
	option 410 ��cctv��ȫ���������Ϣ����������վ�������Ƶ
	endif
	talk
endproc

proc 306
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b����ϸ�Ķ�������Ŀ������������������һ�⣬�����¿�ʼ���⣬���������������������
	add_talk #b#b
	add_talk #b#b#c186506��Ϸ������˵���͵㿨����ң�ֻҪ�ѺŸ������Ϳ��ԣ������ô���أ�#n
	
	rand 12 4
	if @12 = 0
	option 412 ֱ�Ӹ�������ô�õ��°����ѵ�һ��������ȻҪ�Ͻ�
	option 412 �����ѵĺ�һ�����
	option 412 ����һ��
	option 401 ����ID����̳�����ⷴ������ȥ�ٱ�����
	elseif @12 = 1
	option 412 �����ѵĺ�һ�����
	option 412 ����һ��
	option 401 ����ID����̳�����ⷴ������ȥ�ٱ�����
	option 412 ֱ�Ӹ�������ô�õ��°����ѵ�һ��������ȻҪ�Ͻ�
	elseif @12 = 2
	option 412 ����һ��
	option 401 ����ID����̳�����ⷴ������ȥ�ٱ�����
	option 412 ֱ�Ӹ�������ô�õ��°����ѵ�һ��������ȻҪ�Ͻ�
	option 412 �����ѵĺ�һ�����
	else
	option 401 ����ID����̳�����ⷴ������ȥ�ٱ�����
	option 412 ֱ�Ӹ�������ô�õ��°����ѵ�һ��������ȻҪ�Ͻ�
	option 412 �����ѵĺ�һ�����
	option 412 ����һ��
	endif
	talk
endproc

proc 307
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b����ϸ�Ķ�������Ŀ������������������һ�⣬�����¿�ʼ���⣬���������������������
	add_talk #b#b
	add_talk #b#b#c186506���Ԫ���˺Ź����Ĺ�����ַ�ǣ�#n
	
	rand 12 4
	if @12 = 0
	option 401 http://passport.wanyouyl.com
	option 414 http://www.ljy0.com
	option 414 http://bbs.ljy0.com
	option 414 http://pay.wanyouyl.com
	elseif @12 = 1
	option 414 http://www.ljy0.com
	option 414 http://bbs.ljy0.com
	option 414 http://pay.wanyouyl.com
	option 401 http://passport.wanyouyl.com
	elseif @12 = 2
	option 414 http://bbs.ljy0.com
	option 414 http://pay.wanyouyl.com
	option 401 http://passport.wanyouyl.com
	option 414 http://www.ljy0.com
	else
	option 414 http://pay.wanyouyl.com
	option 401 http://passport.wanyouyl.com
	option 414 http://www.ljy0.com
	option 414 http://bbs.ljy0.com
	endif
	talk
endproc

proc 308
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b����ϸ�Ķ�������Ŀ������������������һ�⣬�����¿�ʼ���⣬���������������������
	add_talk #b#b
	add_talk #b#b#c186506�����г��ĸ���;�����������˽⵽���Ԫ���µĻ��̬��#n
	
	rand 12 4
	if @12 = 0
	option 401 ���Ԫ������http://www.ljy0.com
	option 401 ���Ԫ�ٷ���̳��http://bbs.ljy0.com
	option 401 ��������ý����վ����17173�����ˡ����桢���ף�
	option 416 ��Ϸ������Ƶ�������չ�XX�������Ϣ
	elseif @12 = 1
	option 401 ���Ԫ�ٷ���̳��http://bbs.ljy0.com
	option 401 ��������ý����վ����17173�����ˡ����桢���ף�
	option 416 ��Ϸ������Ƶ�������չ�XX�������Ϣ
	option 401 ���Ԫ������http://www.ljy0.com
	elseif @12 = 2
	option 401 ��������ý����վ����17173�����ˡ����桢���ף�
	option 416 ��Ϸ������Ƶ�������չ�XX�������Ϣ
	option 401 ���Ԫ������http://www.ljy0.com
	option 401 ���Ԫ�ٷ���̳��http://bbs.ljy0.com
	else
	option 416 ��Ϸ������Ƶ�������չ�XX�������Ϣ
	option 401 ���Ԫ������http://www.ljy0.com
	option 401 ���Ԫ�ٷ���̳��http://bbs.ljy0.com
	option 401 ��������ý����վ����17173�����ˡ����桢���ף�
	endif
	talk
endproc

proc 309
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b����ϸ�Ķ�������Ŀ������������������һ�⣬�����¿�ʼ���⣬���������������������
	add_talk #b#b
	add_talk #b#b#c186506��Ϸ���Լ�������ͻȻ������Ҫ��ҡ����ҡ�װ���������ô����#n
	
	rand 12 4
	if @12 = 0
	option 418 ֱ�Ӹ���
	option 418 �ѵú��ѿ��У���̫�ò������ӣ�������˵ֱ�Ӹ�
	option 401 ��ϸѯ�ʣ�ͨ��������ʽ���绰����ϵ��ȷ�����
	option 418 ֱ������
	elseif @12 = 1
	option 418 �ѵú��ѿ��У���̫�ò������ӣ�������˵ֱ�Ӹ�
	option 401 ��ϸѯ�ʣ�ͨ��������ʽ���绰����ϵ��ȷ�����
	option 418 ֱ������
	option 418 ֱ�Ӹ���
	elseif @12 = 2
	option 401 ��ϸѯ�ʣ�ͨ��������ʽ���绰����ϵ��ȷ�����
	option 418 ֱ������
	option 418 ֱ�Ӹ���
	option 418 �ѵú��ѿ��У���̫�ò������ӣ�������˵ֱ�Ӹ�
	else
	option 418 ֱ������
	option 418 ֱ�Ӹ���
	option 418 �ѵú��ѿ��У���̫�ò������ӣ�������˵ֱ�Ӹ�
	option 401 ��ϸѯ�ʣ�ͨ��������ʽ���绰����ϵ��ȷ�����
	endif
	talk
endproc

proc 310
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b����ϸ�Ķ�������Ŀ������������������һ�⣬�����¿�ʼ���⣬���������������������
	add_talk #b#b
	add_talk #b#b#c186506��������Ϸ�������ѣ�Ȼ�������ȥXXX��վ��ȡ��ҡ�������������ʿ��������ô����#n
	
	rand 12 4
	if @12 = 0
	option 420 �ۣ���ô�õ��£���Ȼ�ǸϽ�ȥ���ˣ�
	option 420 ���˺������������������æȥ�죡
	option 401 ƭ�ӣ�����IDȥ��̳�����ⷴ�������ٱ���
	option 420 ����ת�������ѣ������ѽй���һ��ȥ��
	elseif @12 = 1
	option 420 ���˺������������������æȥ�죡
	option 401 ƭ�ӣ�����IDȥ��̳�����ⷴ�������ٱ���
	option 420 ����ת�������ѣ������ѽй���һ��ȥ��
	option 420 �ۣ���ô�õ��£���Ȼ�ǸϽ�ȥ���ˣ�
	elseif @12 = 2
	option 401 ƭ�ӣ�����IDȥ��̳�����ⷴ�������ٱ���
	option 420 ����ת�������ѣ������ѽй���һ��ȥ��
	option 420 �ۣ���ô�õ��£���Ȼ�ǸϽ�ȥ���ˣ�
	option 420 ���˺������������������æȥ�죡
	else
	option 420 ����ת�������ѣ������ѽй���һ��ȥ��
	option 420 �ۣ���ô�õ��£���Ȼ�ǸϽ�ȥ���ˣ�
	option 420 ���˺������������������æȥ�죡
	option 401 ƭ�ӣ�����IDȥ��̳�����ⷴ�������ٱ���
	endif
	talk
endproc

proc 311
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b����ϸ�Ķ�������Ŀ������������������һ�⣬�����¿�ʼ���⣬���������������������
	add_talk #b#b
	add_talk #b#b#c186506���������ȣ�����˵������ȷ�ģ�#n
	
	rand 12 4
	if @12 = 0
	option 422 ���Լ�ȥ���������о���
	option 422 ����С�ţ�С�Ż����;���
	option 401 ��̬�����ľ���ֻ���Լ��ĵȼ��й�
	option 422 ʹ�ø����еġ����浶�������ס�����پ���
	elseif @12 = 1
	option 422 ����С�ţ�С�Ż����;���
	option 401 ��̬�����ľ���ֻ���Լ��ĵȼ��й�
	option 422 ʹ�ø����еġ����浶�������ס�����پ���
	option 422 ���Լ�ȥ���������о���
	elseif @12 = 2
	option 401 ��̬�����ľ���ֻ���Լ��ĵȼ��й�
	option 422 ʹ�ø����еġ����浶�������ס�����پ���
	option 422 ���Լ�ȥ���������о���
	option 422 ����С�ţ�С�Ż����;���
	else
	option 422 ʹ�ø����еġ����浶�������ס�����پ���
	option 422 ���Լ�ȥ���������о���
	option 422 ����С�ţ�С�Ż����;���
	option 401 ��̬�����ľ���ֻ���Լ��ĵȼ��й�
	endif
	talk
endproc

proc 312
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b����ϸ�Ķ�������Ŀ������������������һ�⣬�����¿�ʼ���⣬���������������������
	add_talk #b#b
	add_talk #b#b#c18650664�����ν��׵Ĳ��Ͻ�ʲô���֣�#n
	
	rand 12 4
	if @12 = 0
	option 424 ��ͭ΢������ͭ��顢��ͭ�Ͻ�
	option 424 ��ͭ΢������ͭ��顢��ͭ�Ͻ�
	option 401 ������顢����Ͻ�
	option 424 ��ҫ΢������ҫ��顢��ҫ�Ͻ�
	elseif @12 = 1	
	option 424 ��ͭ΢������ͭ��顢��ͭ�Ͻ�
	option 401 ������顢����Ͻ�
	option 424 ��ҫ΢������ҫ��顢��ҫ�Ͻ�
	option 424 ��ͭ΢������ͭ��顢��ͭ�Ͻ�
	elseif @12 = 2	
	option 401 ������顢����Ͻ�
	option 424 ��ҫ΢������ҫ��顢��ҫ�Ͻ�
	option 424 ��ͭ΢������ͭ��顢��ͭ�Ͻ�
	option 424 ��ͭ΢������ͭ��顢��ͭ�Ͻ�
	else
	option 424 ��ҫ΢������ҫ��顢��ҫ�Ͻ�
	option 424 ��ͭ΢������ͭ��顢��ͭ�Ͻ�
	option 424 ��ͭ΢������ͭ��顢��ͭ�Ͻ�
	option 401 ������顢����Ͻ�
	endif
	talk
endproc

proc 313
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b����ϸ�Ķ�������Ŀ������������������һ�⣬�����¿�ʼ���⣬���������������������
	add_talk #b#b
	add_talk #b#b#c18650669�������׵Ĳ��Ͻ�ʲô���֣�#n
	
	rand 12 4
	if @12 = 0
	option 426 ��ͭ΢������ͭ��顢��ͭ�Ͻ�
	option 426 ��ͭ΢������ͭ��顢��ͭ�Ͻ�
	option 401 ������顢����Ͻ�
	option 426 ��ҫ΢������ҫ��顢��ҫ�Ͻ�
	elseif @12 = 1
	option 426 ��ͭ΢������ͭ��顢��ͭ�Ͻ�
	option 401 ������顢����Ͻ�
	option 426 ��ҫ΢������ҫ��顢��ҫ�Ͻ�
	option 426 ��ͭ΢������ͭ��顢��ͭ�Ͻ�
	elseif @12 = 2	
	option 401 ������顢����Ͻ�
	option 426 ��ҫ΢������ҫ��顢��ҫ�Ͻ�
	option 426 ��ͭ΢������ͭ��顢��ͭ�Ͻ�
	option 426 ��ͭ΢������ͭ��顢��ͭ�Ͻ�
	else
	option 426 ��ҫ΢������ҫ��顢��ҫ�Ͻ�
	option 426 ��ͭ΢������ͭ��顢��ͭ�Ͻ�
	option 426 ��ͭ΢������ͭ��顢��ͭ�Ͻ�
	option 401 ������顢����Ͻ�
	endif
	talk
endproc

proc 314
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b����ϸ�Ķ�������Ŀ������������������һ�⣬�����¿�ʼ���⣬���������������������
	add_talk #b#b
	add_talk #b#b#c186506���＼�����á�����ͨ����λNPC���У�#n
	
	rand 12 4
	if @12 = 0
	option 428 ���ﵼʦ-����
	option 401 ����ѵ��Ա-���¶�
	option 428 ����ѵ��ʦ-������
	option 428 �����ʦ-̩��
	elseif @12 = 1
	option 401 ����ѵ��Ա-���¶�
	option 428 ����ѵ��ʦ-������
	option 428 �����ʦ-̩��
	option 428 ���ﵼʦ-����
	elseif @12 = 2
	option 428 ����ѵ��ʦ-������
	option 428 �����ʦ-̩��
	option 428 ���ﵼʦ-����
	option 401 ����ѵ��Ա-���¶�
	else
	option 428 �����ʦ-̩��
	option 428 ���ﵼʦ-����
	option 401 ����ѵ��Ա-���¶�
	option 428 ����ѵ��ʦ-������
	endif
	talk
endproc

proc 315
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b����ϸ�Ķ�������Ŀ������������������һ�⣬�����¿�ʼ���⣬���������������������
	add_talk #b#b
	add_talk #b#b#c186506�ܹ���ñ�ְҵ��װ����װ����¯�ر�������ȡ���ĸ�NPC��#n
	
	rand 12 4
	if @12 = 0
	option 430 ��ѧʿ-����
	option 430 ������·��-������
	option 430 �����ʦ-��ŵ��
	option 401 ����-��������
	elseif @12 = 1
	option 430 ������·��-������
	option 430 �����ʦ-��ŵ��
	option 401 ����-��������
	option 430 ��ѧʿ-����
	elseif @12 = 2
	option 430 �����ʦ-��ŵ��
	option 401 ����-��������
	option 430 ��ѧʿ-����
	option 430 ������·��-������
	else
	option 401 ����-��������
	option 430 ��ѧʿ-����
	option 430 ������·��-������
	option 430 �����ʦ-��ŵ��
	endif
	talk
endproc

proc 316
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b����ϸ�Ķ�������Ŀ������������������һ�⣬�����¿�ʼ���⣬���������������������
	add_talk #b#b
	add_talk #b#b#c186506װ�����ף�����װ����Ϊ��װ�����ĸ�NPC��#n
	
	rand 12 4
	if @12 = 0
	option 401 װ����⡢���״�ʦ-�ݽܷ�
	option 432 װ������ʦ-˹�Ϸ���
	option 432 װ��ǿ����ʦ-�˶���
	option 432 �������Ա-˹�ٷ�˹
	elseif @12 = 1
	option 432 װ������ʦ-˹�Ϸ���
	option 432 װ��ǿ����ʦ-�˶���
	option 432 �������Ա-˹�ٷ�˹
	option 401 װ����⡢���״�ʦ-�ݽܷ�
	elseif @12 = 2
	option 432 װ��ǿ����ʦ-�˶���
	option 432 �������Ա-˹�ٷ�˹
	option 401 װ����⡢���״�ʦ-�ݽܷ�
	option 432 װ������ʦ-˹�Ϸ���
	else
	option 432 �������Ա-˹�ٷ�˹
	option 401 װ����⡢���״�ʦ-�ݽܷ�
	option 432 װ������ʦ-˹�Ϸ���
	option 432 װ��ǿ����ʦ-�˶���
	endif
	talk
endproc

proc 317
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b����ϸ�Ķ�������Ŀ������������������һ�⣬�����¿�ʼ���⣬���������������������
	add_talk #b#b
	add_talk #b#b#c186506װ������ת�ƣ�����ͬ�ȼ�����λ����װ�ĸ�������ת�Ƶ���һ��װ���ϣ����ĸ�NPC��#n
	
	rand 12 4
	if @12 = 0
	option 401 װ������ʦ-˹�Ϸ���
	option 434 װ��ǿ����ʦ-�˶���
	option 434 װ����⡢���״�ʦ-�ݽܷ�
	option 434 �߼���������-÷��
	elseif @12 = 1
	option 434 װ��ǿ����ʦ-�˶���
	option 434 װ����⡢���״�ʦ-�ݽܷ�
	option 434 �߼���������-÷��
	option 401 װ������ʦ-˹�Ϸ���
	elseif @12 = 2
	option 434 װ����⡢���״�ʦ-�ݽܷ�
	option 434 �߼���������-÷��
	option 401 װ������ʦ-˹�Ϸ���
	option 434 װ��ǿ����ʦ-�˶���
	else
	option 434 �߼���������-÷��
	option 401 װ������ʦ-˹�Ϸ���
	option 434 װ��ǿ����ʦ-�˶���
	option 434 װ����⡢���״�ʦ-�ݽܷ�
	endif
	talk
endproc

proc 318
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b����ϸ�Ķ�������Ŀ������������������һ�⣬�����¿�ʼ���⣬���������������������
	add_talk #b#b
	add_talk #b#b#c186506���Ľ��븱���ķ�ʽ�ǣ�#n
	
	rand 12 4
	if @12 = 0
	option 436 �ܵ������ſڣ������˺�ֱ�ӽ�ȥ
	option 436 ���������������Ҫ����������Ҫ������������
	option 401 ͨ��С��ͼ�ϵġ�������ť����ʱ������Ľ�
	option 436 ȥ������
	elseif @12 = 1
	option 436 ���������������Ҫ����������Ҫ������������
	option 401 ͨ��С��ͼ�ϵġ�������ť����ʱ������Ľ�
	option 436 ȥ������
	option 436 �ܵ������ſڣ������˺�ֱ�ӽ�ȥ
	elseif @12 = 2
	option 401 ͨ��С��ͼ�ϵġ�������ť����ʱ������Ľ�
	option 436 ȥ������
	option 436 �ܵ������ſڣ������˺�ֱ�ӽ�ȥ
	option 436 ���������������Ҫ����������Ҫ������������
	else
	option 436 ȥ������
	option 436 �ܵ������ſڣ������˺�ֱ�ӽ�ȥ
	option 436 ���������������Ҫ����������Ҫ������������
	option 401 ͨ��С��ͼ�ϵġ�������ť����ʱ������Ľ�
	endif
	talk
endproc

proc 319
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b����ϸ�Ķ�������Ŀ������������������һ�⣬�����¿�ʼ���⣬���������������������
	add_talk #b#b
	add_talk #b#b#c186506�������ڼ���#n
	
	strvar weekid
	;weekid : 0 ~ 6    0��������    1������һ
	GET_NOW_TIME  0 0 0 weekid 0 0 0 
	
	if @weekid = 1
		rand 12 4
		if @12 = 0
			option 401 ��һ
			option 438 �ܶ�
			option 438 ����
			option 438 ����
			option 438 ����
			option 438 ����
			option 438 ����
		elseif @12 = 1
			option 438 �ܶ�
			option 438 ����
			option 438 ����
			option 438 ����
			option 438 ����
			option 438 ����
			option 401 ��һ
		elseif @12 = 2
			option 438 ����
			option 438 ����
			option 438 ����
			option 438 ����
			option 438 ����
			option 401 ��һ
			option 438 �ܶ�
		else
			option 438 ����
			option 438 ����
			option 438 ����
			option 401 ��һ
			option 438 �ܶ�
			option 438 ����
			option 438 ����
		endif
	elseif @weekid = 2
		rand 12 4
		if @12 = 0
			option 438 ��һ
			option 401 �ܶ�
			option 438 ����
			option 438 ����
			option 438 ����
			option 438 ����
			option 438 ����
		elseif @12 = 1
			option 401 �ܶ�
			option 438 ����
			option 438 ����
			option 438 ����
			option 438 ����
			option 438 ����
			option 438 ��һ
		elseif @12 = 2
			option 438 ����
			option 438 ����
			option 438 ����
			option 438 ����
			option 438 ����
			option 438 ��һ
			option 401 �ܶ�
		else
			option 438 ����
			option 438 ����
			option 438 ����
			option 438 ��һ
			option 401 �ܶ�
			option 438 ����
			option 438 ����
		endif
	elseif @weekid = 3
		rand 12 4
		if @12 = 0
			option 438 ��һ
			option 438 �ܶ�
			option 401 ����
			option 438 ����
			option 438 ����
			option 438 ����
			option 438 ����
		elseif @12 = 1
			option 438 �ܶ�
			option 401 ����
			option 438 ����
			option 438 ����
			option 438 ����
			option 438 ����
			option 438 ��һ
		elseif @12 = 2
			option 401 ����
			option 438 ����
			option 438 ����
			option 438 ����
			option 438 ����
			option 438 ��һ
			option 438 �ܶ�
		else
			option 438 ����
			option 438 ����
			option 438 ����
			option 438 ��һ
			option 438 �ܶ�
			option 401 ����
			option 438 ����
		endif
	elseif @weekid = 4
		rand 12 4
		if @12 = 0
			option 438 ��һ
			option 438 �ܶ�
			option 438 ����
			option 401 ����
			option 438 ����
			option 438 ����
			option 438 ����
		elseif @12 = 1
			option 438 �ܶ�
			option 438 ����
			option 401 ����
			option 438 ����
			option 438 ����
			option 438 ����
			option 438 ��һ
		elseif @12 = 2
			option 438 ����
			option 401 ����
			option 438 ����
			option 438 ����
			option 438 ����
			option 438 ��һ
			option 438 �ܶ�
		else
			option 438 ����
			option 438 ����
			option 438 ����
			option 438 ��һ
			option 438 �ܶ�
			option 438 ����
			option 401 ����
		endif
	elseif @weekid = 5
		rand 12 4
		if @12 = 0
			option 438 ��һ
			option 438 �ܶ�
			option 438 ����
			option 438 ����
			option 401 ����
			option 438 ����
			option 438 ����
		elseif @12 = 1
			option 438 �ܶ�
			option 438 ����
			option 438 ����
			option 401 ����
			option 438 ����
			option 438 ����
			option 438 ��һ
		elseif @12 = 2
			option 438 ����
			option 438 ����
			option 401 ����
			option 438 ����
			option 438 ����
			option 438 ��һ
			option 438 �ܶ�
		else
			option 401 ����
			option 438 ����
			option 438 ����
			option 438 ��һ
			option 438 �ܶ�
			option 438 ����
			option 438 ����
		endif
	elseif @weekid = 6
		rand 12 4
		if @12 = 0
			option 438 ��һ
			option 438 �ܶ�
			option 438 ����
			option 438 ����
			option 438 ����
			option 401 ����
			option 438 ����
		elseif @12 = 1
			option 438 �ܶ�
			option 438 ����
			option 438 ����
			option 438 ����
			option 401 ����
			option 438 ����
			option 438 ��һ
		elseif @12 = 2
			option 438 ����
			option 438 ����
			option 438 ����
			option 401 ����
			option 438 ����
			option 438 ��һ
			option 438 �ܶ�
		else
			option 438 ����
			option 401 ����
			option 438 ����
			option 438 ��һ
			option 438 �ܶ�
			option 438 ����
			option 438 ����
		endif
	elseif @weekid = 0
		rand 12 4
		if @12 = 0
			option 438 ��һ
			option 438 �ܶ�
			option 438 ����
			option 438 ����
			option 438 ����
			option 438 ����
			option 401 ����
		elseif @12 = 1
			option 438 �ܶ�
			option 438 ����
			option 438 ����
			option 438 ����
			option 438 ����
			option 401 ����
			option 438 ��һ
		elseif @12 = 2
			option 438 ����
			option 438 ����
			option 438 ����
			option 438 ����
			option 401 ����
			option 438 ��һ
			option 438 �ܶ�
		else
			option 438 ����
			option 438 ����
			option 401 ����
			option 438 ��һ
			option 438 �ܶ�
			option 438 ����
			option 438 ����
		endif
	else
		return
	endif
	
	
	talk
endproc

;ѡ��ż����Ϊ����ģ�����401Ϊ��ȷ��

proc 400
	TASK_STAT 4573 1 71
	if @71 = 2
		var 430 4573
	endif
	
	TASK_STAT 4574 1 72
	if @72 = 2
		var 430 4574
	endif
	
	TASK_STAT 4575 1 73
	if @73 = 2
		var 430 4575
	endif
	
	TASK_STAT 4576 1 74
	if @74 = 2
		var 430 4576
	endif
	
	TASK_STAT 4577 1 75
	if @75 = 2
		var 430 4577
	endif
	
	;1�Ļ�����ʾһ��Ҫ�Ѵ�Դ�����0
	SET_TASK_VAR @430 4 32 1 1

	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b#c0181f9�ܶ�ƭ�������������ǵ�̰С����������������ʵ��������������������һ������ǧ�������������û����ѵ���ͣ�#n
	
	option 500 ���¿�ʼ����
	
	talk
endproc

proc 401
	TASK_STAT 4573 1 71
	if @71 = 2
		var 430 4573
	endif
	
	TASK_STAT 4574 1 72
	if @72 = 2
		var 430 4574
	endif
	
	TASK_STAT 4575 1 73
	if @73 = 2
		var 430 4575
	endif
	
	TASK_STAT 4576 1 74
	if @74 = 2
		var 430 4576
	endif
	
	TASK_STAT 4577 1 75
	if @75 = 2
		var 430 4577
	endif
	
	GET_TASK_VAR @430 0 32 1 80
	add 80 1
	SET_TASK_VAR @430 0 32 @80 1
	
	if @80 >= 3
		TASK_FINI @430 1
		PLAYER_CLOSE_DIALOG1 0 0
	else
		call 200
	endif
endproc

proc 402
	TASK_STAT 4573 1 71
	if @71 = 2
		var 430 4573
	endif
	
	TASK_STAT 4574 1 72
	if @72 = 2
		var 430 4574
	endif
	
	TASK_STAT 4575 1 73
	if @73 = 2
		var 430 4575
	endif
	
	TASK_STAT 4576 1 74
	if @74 = 2
		var 430 4576
	endif
	
	TASK_STAT 4577 1 75
	if @75 = 2
		var 430 4577
	endif
	
	;1�Ļ�����ʾһ��Ҫ�Ѵ�Դ�����0
	SET_TASK_VAR @430 4 32 1 1

	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b#c0181f9����ƭ������Щ����������һ������ġ�ľ����վ������������ʱ���ͨ��ľ���ȡ�����˺ţ����������棡�Ը�����ƭ��������׼���ԪΨһ������www.ljy0.com�������Թ����ϲ�ѯ�����е����Ԫ�ٷ��Ƴ��Ļ��#n
	
	option 500 ���¿�ʼ����
	
	talk
endproc

proc 404
	TASK_STAT 4573 1 71
	if @71 = 2
		var 430 4573
	endif
	
	TASK_STAT 4574 1 72
	if @72 = 2
		var 430 4574
	endif
	
	TASK_STAT 4575 1 73
	if @73 = 2
		var 430 4575
	endif
	
	TASK_STAT 4576 1 74
	if @74 = 2
		var 430 4576
	endif
	
	TASK_STAT 4577 1 75
	if @75 = 2
		var 430 4577
	endif
	
	;1�Ļ�����ʾһ��Ҫ�Ѵ�Դ�����0
	SET_TASK_VAR @430 4 32 1 1

	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b#c0181f9���ۺ�ʱ�εأ���Ҫ����Լ��ķ�ƭ������ʶ��������δȻ��ͨ�������ġ��˺Ź�����ť����½�������ģ����ݡ���ȫ��ʾ��������ݣ���д��ȫ���䣬��֤��ȫ���䣬���ܱ��������Լ�Զ����ţ�#n
	
	option 500 ���¿�ʼ����
	
	talk
endproc

proc 406
	TASK_STAT 4573 1 71
	if @71 = 2
		var 430 4573
	endif
	
	TASK_STAT 4574 1 72
	if @72 = 2
		var 430 4574
	endif
	
	TASK_STAT 4575 1 73
	if @73 = 2
		var 430 4575
	endif
	
	TASK_STAT 4576 1 74
	if @74 = 2
		var 430 4576
	endif
	
	TASK_STAT 4577 1 75
	if @75 = 2
		var 430 4577
	endif
	
	;1�Ļ�����ʾһ��Ҫ�Ѵ�Դ�����0
	SET_TASK_VAR @430 4 32 1 1

	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b#c0181f9���������������������뵽��̳�����ⷴ������������GM������ϣ������Զ����������������������ǵ�������������һ������ʹ���˺���Ҳ������������ԣ��������˺Ű�ȫ��Ϊ��Ҫ��ͨ�������ġ��˺Ź�����ť����½�������ģ����ܱ�������֤��ȫ����ɣ�#n
	
	option 500 ���¿�ʼ����
	
	talk
endproc

proc 408
	TASK_STAT 4573 1 71
	if @71 = 2
		var 430 4573
	endif
	
	TASK_STAT 4574 1 72
	if @72 = 2
		var 430 4574
	endif
	
	TASK_STAT 4575 1 73
	if @73 = 2
		var 430 4575
	endif
	
	TASK_STAT 4576 1 74
	if @74 = 2
		var 430 4576
	endif
	
	TASK_STAT 4577 1 75
	if @75 = 2
		var 430 4577
	endif
	
	;1�Ļ�����ʾһ��Ҫ�Ѵ�Դ�����0
	SET_TASK_VAR @430 4 32 1 1

	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b#c0181f9�����ƭ�����ţ���ؼ���һ��������׼������www.ljy0.com#n
	
	option 500 ���¿�ʼ����
	
	talk
endproc

proc 410
	TASK_STAT 4573 1 71
	if @71 = 2
		var 430 4573
	endif
	
	TASK_STAT 4574 1 72
	if @72 = 2
		var 430 4574
	endif
	
	TASK_STAT 4575 1 73
	if @73 = 2
		var 430 4575
	endif
	
	TASK_STAT 4576 1 74
	if @74 = 2
		var 430 4576
	endif
	
	TASK_STAT 4577 1 75
	if @75 = 2
		var 430 4577
	endif
	
	;1�Ļ�����ʾһ��Ҫ�Ѵ�Դ�����0
	SET_TASK_VAR @430 4 32 1 1

	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b#c0181f9����ƭ����ĳ���������Ƶ�������ġ���XXX���������ȥ�ٶ�XXX�����������99.99����ƭ�ӣ��벻Ҫ���Ź�����û�г��ֹ��ġ�����������Ҫ�˽�������ȫ��ĸ����������½���Ԫ������www.ljy0.com#n
	
	option 500 ���¿�ʼ����
	
	talk
endproc

proc 412
	TASK_STAT 4573 1 71
	if @71 = 2
		var 430 4573
	endif
	
	TASK_STAT 4574 1 72
	if @72 = 2
		var 430 4574
	endif
	
	TASK_STAT 4575 1 73
	if @73 = 2
		var 430 4575
	endif
	
	TASK_STAT 4576 1 74
	if @74 = 2
		var 430 4576
	endif
	
	TASK_STAT 4577 1 75
	if @75 = 2
		var 430 4577
	endif
	
	;1�Ļ�����ʾһ��Ҫ�Ѵ�Դ�����0
	SET_TASK_VAR @430 4 32 1 1

	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b#c0181f9���ڵ��ż����ǳ������Щ���ü򵥵���������ױ��ƽ⣬���ԣ��벻Ҫ���Լ����˺�͸¶��İ���ˣ�����Ҫ������Щ�ǹٷ��������н��ʼ������Ｐ������Ϣ���ٷ���Ա������������Ҫ������ĸ�����Ϣ��#n
	
	option 500 ���¿�ʼ����
	
	talk
endproc

proc 414
	TASK_STAT 4573 1 71
	if @71 = 2
		var 430 4573
	endif
	
	TASK_STAT 4574 1 72
	if @72 = 2
		var 430 4574
	endif
	
	TASK_STAT 4575 1 73
	if @73 = 2
		var 430 4575
	endif
	
	TASK_STAT 4576 1 74
	if @74 = 2
		var 430 4576
	endif
	
	TASK_STAT 4577 1 75
	if @75 = 2
		var 430 4577
	endif
	
	;1�Ļ�����ʾһ��Ҫ�Ѵ�Դ�����0
	SET_TASK_VAR @430 4 32 1 1

	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b#c0181f9�˺Ź����ܱ�������Ĺ�����ַ��http://passport.wanyouyl.com
	add_talk #b#b
    add_talk #b#b#c0181f9���Ԫ������http://www.ljy0.com
	add_talk #b#b
    add_talk #b#b#c0181f9���Ԫ�ٷ���̳��http://bbs.ljy0.com
	add_talk #b#b
    add_talk #b#b#c0181f9���Ԫ�ٷ���ֵ���ĵ�ַ��http://pay.wanyouyl.com#n
	
	option 500 ���¿�ʼ����
	
	talk
endproc

proc 416
	TASK_STAT 4573 1 71
	if @71 = 2
		var 430 4573
	endif
	
	TASK_STAT 4574 1 72
	if @72 = 2
		var 430 4574
	endif
	
	TASK_STAT 4575 1 73
	if @73 = 2
		var 430 4575
	endif
	
	TASK_STAT 4576 1 74
	if @74 = 2
		var 430 4576
	endif
	
	TASK_STAT 4577 1 75
	if @75 = 2
		var 430 4577
	endif
	
	;1�Ļ�����ʾһ��Ҫ�Ѵ�Դ�����0
	SET_TASK_VAR @430 4 32 1 1

	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b#c0181f9�����Ԫ����еġ����������ȡ����Ʒ���ǰ󶨵ģ����ɽ��׵�Ŷ�����ԣ���Щ����������XX����Ĵ����ƭ��Ӵ���ر�����Щ����ȥ�ٶȶ���ȥ�����������»��Ϣ�ģ�����׼���ԪΨһ������www.ljy0.com#n
	
	option 500 ���¿�ʼ����
	
	talk
endproc


proc 418
	TASK_STAT 4573 1 71
	if @71 = 2
		var 430 4573
	endif
	
	TASK_STAT 4574 1 72
	if @72 = 2
		var 430 4574
	endif
	
	TASK_STAT 4575 1 73
	if @73 = 2
		var 430 4575
	endif
	
	TASK_STAT 4576 1 74
	if @74 = 2
		var 430 4576
	endif
	
	TASK_STAT 4577 1 75
	if @75 = 2
		var 430 4577
	endif
	
	;1�Ļ�����ʾһ��Ҫ�Ѵ�Դ�����0
	SET_TASK_VAR @430 4 32 1 1

	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b#c0181f9����ƭ��������������һ����Ϸ�˺ţ��ھ��������ߺ������вƲ�֮ǰ���ȼٽ�����˵�������������к��ѡ������Ա��Ǯ���Ȼ����ͬ�����˵ĲƲ�һ��ת�ƣ����ԣ���������ʵ�л������Ԫ�������������Ǯ����ǵ�ȷ�϶Է��Ƿ�����Ϊ���ѱ���Ӵ��#n
	
	option 500 ���¿�ʼ����
	
	talk
endproc

proc 420
	TASK_STAT 4573 1 71
	if @71 = 2
		var 430 4573
	endif
	
	TASK_STAT 4574 1 72
	if @72 = 2
		var 430 4574
	endif
	
	TASK_STAT 4575 1 73
	if @73 = 2
		var 430 4575
	endif
	
	TASK_STAT 4576 1 74
	if @74 = 2
		var 430 4576
	endif
	
	TASK_STAT 4577 1 75
	if @75 = 2
		var 430 4577
	endif
	
	;1�Ļ�����ʾһ��Ҫ�Ѵ�Դ�����0
	SET_TASK_VAR @430 4 32 1 1

	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b#c0181f9����ƭ��������Ϸ�з��������Ϣ����ȡ��ҡ������������ϡ����Ʒ����������½һ���ǹٷ���վ����վ�Ϲ���ľ����¼�������˺���Ϣ�������ƭ�����ţ����������н���Ϣ�����ס���ԪΨһ������www.ljy0.com#n
	
	option 500 ���¿�ʼ����
	
	talk
endproc

proc 422
	TASK_STAT 4573 1 71
	if @71 = 2
		var 430 4573
	endif
	
	TASK_STAT 4574 1 72
	if @72 = 2
		var 430 4574
	endif
	
	TASK_STAT 4575 1 73
	if @73 = 2
		var 430 4575
	endif
	
	TASK_STAT 4576 1 74
	if @74 = 2
		var 430 4576
	endif
	
	TASK_STAT 4577 1 75
	if @75 = 2
		var 430 4577
	endif
	
	;1�Ļ�����ʾһ��Ҫ�Ѵ�Դ�����0
	SET_TASK_VAR @430 4 32 1 1

	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b#c0181f9����������ɽ�������οռ�һ�������ڶ�̬��������̬�����ľ���ֻ���Լ��ĵȼ��йأ������ѵĵȼ��޹أ���С���ܹ��ø����еĹ����Ѷȣ��ȼ������ͣ��ø������̸����ס���Ѹ�٣�����������������õľ��飬��Ϊ��ֻȡ����������ĵȼ�������е�������
	add_talk #b#b#c0181f9���⣬ʹ�ø����еġ����浶�������ס�Ҳ��������������õľ�����٣�Ψһ�������õľ�����ٵ�;���ǣ��ж����뿪�ˣ��޹غ����ĵȼ���ְҵ��#n
	
	option 500 ���¿�ʼ����
	
	talk
endproc

proc 424
	TASK_STAT 4573 1 71
	if @71 = 2
		var 430 4573
	endif
	
	TASK_STAT 4574 1 72
	if @72 = 2
		var 430 4574
	endif
	
	TASK_STAT 4575 1 73
	if @73 = 2
		var 430 4575
	endif
	
	TASK_STAT 4576 1 74
	if @74 = 2
		var 430 4576
	endif
	
	TASK_STAT 4577 1 75
	if @75 = 2
		var 430 4577
	endif
	
	;1�Ļ�����ʾһ��Ҫ�Ѵ�Դ�����0
	SET_TASK_VAR @430 4 32 1 1

	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b#c0181f9���Ρ����Σ���������ָ���������Ľ��ײ��϶�������Ͻ���ͨ��ʧ��֮����������������ϳɡ�#n
	
	option 500 ���¿�ʼ����
	
	talk
endproc

proc 426
	TASK_STAT 4573 1 71
	if @71 = 2
		var 430 4573
	endif
	
	TASK_STAT 4574 1 72
	if @72 = 2
		var 430 4574
	endif
	
	TASK_STAT 4575 1 73
	if @73 = 2
		var 430 4575
	endif
	
	TASK_STAT 4576 1 74
	if @74 = 2
		var 430 4576
	endif
	
	TASK_STAT 4577 1 75
	if @75 = 2
		var 430 4577
	endif
	
	;1�Ļ�����ʾһ��Ҫ�Ѵ�Դ�����0
	SET_TASK_VAR @430 4 32 1 1

	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b#c0181f9���Ρ����Σ���������ָ���������Ľ��ײ��϶�������Ͻ���ͨ��ʧ��֮����������������ϳɡ�#n
	
	option 500 ���¿�ʼ����
	
	talk
endproc

proc 428
	TASK_STAT 4573 1 71
	if @71 = 2
		var 430 4573
	endif
	
	TASK_STAT 4574 1 72
	if @72 = 2
		var 430 4574
	endif
	
	TASK_STAT 4575 1 73
	if @73 = 2
		var 430 4575
	endif
	
	TASK_STAT 4576 1 74
	if @74 = 2
		var 430 4576
	endif
	
	TASK_STAT 4577 1 75
	if @75 = 2
		var 430 4577
	endif
	
	;1�Ļ�����ʾһ��Ҫ�Ѵ�Դ�����0
	SET_TASK_VAR @430 4 32 1 1

	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b#c0181f9����ѵ��Ա-���¶࣬�����԰���������м������á�����������������������������ǿ����������ʱ����·����ɸ۸�����#n
	
	option 500 ���¿�ʼ����
	
	talk
endproc

proc 430
	TASK_STAT 4573 1 71
	if @71 = 2
		var 430 4573
	endif
	
	TASK_STAT 4574 1 72
	if @72 = 2
		var 430 4574
	endif
	
	TASK_STAT 4575 1 73
	if @73 = 2
		var 430 4575
	endif
	
	TASK_STAT 4576 1 74
	if @74 = 2
		var 430 4576
	endif
	
	TASK_STAT 4577 1 75
	if @75 = 2
		var 430 4577
	endif
	
	;1�Ļ�����ʾһ��Ҫ�Ѵ�Դ�����0
	SET_TASK_VAR @430 4 32 1 1

	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b#c0181f9����-��������������ʱ����Ϸ����������Ա˹�ٷ�˹��Զ������40����ʼ���Ϳ��������Ƕ����ܡ���¯�ر���������ɫ¯���м��ʻ�ñ�ְҵ��ǰ�ȼ��ε���װ����ɫ¯�ӱ�Ȼ���ñ�ְҵ����װ������ʯ����һ�����ʻ��÷�װ��
	add_talk #b#b#c0181f9����������������¯�ر�������ʱ���ǽ�ɫ��¯�������Ȼ���һ�Ž���¯ƾ֤�����ܵ�һ������ʱ���Ϳ����ڹ������󴦽��ס��̡�������ɫ��¯�����ɽ�ɫ��¯��#n
	
	option 500 ���¿�ʼ����
	
	talk
endproc

proc 432
	TASK_STAT 4573 1 71
	if @71 = 2
		var 430 4573
	endif
	
	TASK_STAT 4574 1 72
	if @72 = 2
		var 430 4574
	endif
	
	TASK_STAT 4575 1 73
	if @73 = 2
		var 430 4575
	endif
	
	TASK_STAT 4576 1 74
	if @74 = 2
		var 430 4576
	endif
	
	TASK_STAT 4577 1 75
	if @75 = 2
		var 430 4577
	endif
	
	;1�Ļ�����ʾһ��Ҫ�Ѵ�Դ�����0
	SET_TASK_VAR @430 4 32 1 1
	
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b#c0181f9������ڡ��ݺϳС����鿴װ�����׵�������⣬�Լ��ϳ�װ������Ҫ�Ĳ��ϳ������ϳɷ�ʽ��
	add_talk #b#b#c0181f9װ���������ڡ�װ����⡢���״�ʦ-�ݽܷᡱ�����С�#n
	
	option 500 ���¿�ʼ����
	
	talk
endproc

proc 434
	TASK_STAT 4573 1 71
	if @71 = 2
		var 430 4573
	endif
	
	TASK_STAT 4574 1 72
	if @72 = 2
		var 430 4574
	endif
	
	TASK_STAT 4575 1 73
	if @73 = 2
		var 430 4575
	endif
	
	TASK_STAT 4576 1 74
	if @74 = 2
		var 430 4576
	endif
	
	TASK_STAT 4577 1 75
	if @75 = 2
		var 430 4577
	endif
	
	;1�Ļ�����ʾһ��Ҫ�Ѵ�Դ�����0
	SET_TASK_VAR @430 4 32 1 1
	
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b#c0181f9װ������ת���ڡ�װ������ʦ-˹�Ϸ��桱�����У�ת������Ҫ��ת��ʯ���ɴӵع���Ĺ������ϻ�á�������װ���Կ���ת�Ƶ���ͬ��λ���ȼ��ķ�װ��Ӵ��ת������Ҫ�ĵ��߿����̳�-��ɫ����-�ɳ�ҳ���й���#n
	
	option 500 ���¿�ʼ����
	
	talk
endproc

proc 436
	TASK_STAT 4573 1 71
	if @71 = 2
		var 430 4573
	endif
	
	TASK_STAT 4574 1 72
	if @72 = 2
		var 430 4574
	endif
	
	TASK_STAT 4575 1 73
	if @73 = 2
		var 430 4575
	endif
	
	TASK_STAT 4576 1 74
	if @74 = 2
		var 430 4576
	endif
	
	TASK_STAT 4577 1 75
	if @75 = 2
		var 430 4577
	endif
	
	;1�Ļ�����ʾһ��Ҫ�Ѵ�Դ�����0
	SET_TASK_VAR @430 4 32 1 1
	
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b#c0181f9��ϸ�о���Ļ���Ͻ�С��ͼ�ϵġ������ְ�ťӴ�����ǽ��븱���Ŀ�ݷ�ʽ������֮�⻹��ս��������ť����ҽ��װ�ť������ְ�ť����������Ϸ��ø����㣡#n
	
	option 500 ���¿�ʼ����
	
	talk
endproc

proc 438
	TASK_STAT 4573 1 71
	if @71 = 2
		var 430 4573
	endif
	
	TASK_STAT 4574 1 72
	if @72 = 2
		var 430 4574
	endif
	
	TASK_STAT 4575 1 73
	if @73 = 2
		var 430 4575
	endif
	
	TASK_STAT 4576 1 74
	if @74 = 2
		var 430 4576
	endif
	
	TASK_STAT 4577 1 75
	if @75 = 2
		var 430 4577
	endif
	
	;1�Ļ�����ʾһ��Ҫ�Ѵ�Դ�����0
	SET_TASK_VAR @430 4 32 1 1
	
	DIALOG 1
	add_talk $_npc$:
	
	strvar weekid
	;weekid : 0 ~ 6    0��������    1������һ
	GET_NOW_TIME  0 0 0 weekid 0 0 0 
	
	if @weekid = 1
		add_talk #b#b#c0181f9��������һ�ޣ�#n
	elseif @weekid = 2
		add_talk #b#b#c0181f9�������ܶ��ޣ�#n
	elseif @weekid = 3
		add_talk #b#b#c0181f9�����������ޣ�#n
	elseif @weekid = 4
		add_talk #b#b#c0181f9�����������ޣ�#n
	elseif @weekid = 5
		add_talk #b#b#c0181f9�����������ޣ�#n
	elseif @weekid = 6
		add_talk #b#b#c0181f9�����������ޣ�#n
	elseif @weekid = 0
		add_talk #b#b#c0181f9�����������ޣ�#n
	else
		return
	endif
	
	option 500 ���¿�ʼ����
	talk
endproc

proc 500
	TASK_STAT 4573 1 71
	if @71 = 2
		var 430 4573
	endif
	
	TASK_STAT 4574 1 72
	if @72 = 2
		var 430 4574
	endif
	
	TASK_STAT 4575 1 73
	if @73 = 2
		var 430 4575
	endif
	
	TASK_STAT 4576 1 74
	if @74 = 2
		var 430 4576
	endif
	
	TASK_STAT 4577 1 75
	if @75 = 2
		var 430 4577
	endif
	
	;��Դ�����0
	SET_TASK_VAR @430 0 32 0 1
	;1�Ļ�����ʾһ��Ҫ�Ѵ�Դ�����0
	SET_TASK_VAR @430 4 32 1 1
	call 200
endproc


