;70����,70������
;�����¼�
PROC 1	
	;���ø�������
	call 100
	;���ֹ�
	call 98
	;�����赲
	call 701
	add_sys_msg_start
	add_sys_msg ������ʼ
	set_sys_msg_mode 4 0 0
	map_msg_info	
ENDPROC
;�ر��¼�
proc 2
	add_sys_msg_start
	add_sys_msg ��������
	set_sys_msg_mode 4 0 0
	map_msg_info
endproc
;2��һ��
PROC 3
	;�����׶�,��Ҫ����tickʱҪ������proc
	get_pworld_var 12 32 1 13
	;��ʼ������
	if 13 = 1
		add 13 1
		set_pworld_var 12 32 @13 1
		;10��ʱ��
		var 10 3
		sub 10 1
		set_pworld_var 0 32 @10 1
	endif
	;����²���
	if 13 = 2
		;����⻷վ�Դ���
		VAR 14 0
		get_pworld_var 16 32 1 14
		;վ��6��
		if 14 >= 6
			
			
			
			;10��ʱ��
			get_pworld_var 0 32 1 10
			add 10 1
			;���⻷������n��tick
			if 10 = 3
				;��ռ�ʱ
				var 10 0
				set_pworld_var 0 32 @10 1
				add 13 1
				set_pworld_var 12 32 @13 1
				;�������
				call 203
			endif
			set_pworld_var 0 32 @10 1
		else
			;10��ʱ��
			get_pworld_var 0 32 1 10
			add 10 1
			;���⻷������n��tick
			if 10 = 3
				call 202
			endif
			set_pworld_var 0 32 @10 1
			strvar num
			var num @14
			if 10 = 1 and num > 0
				add_sys_msg_start
				add_sys_msg ��$@num$����������ɹ�����һ�鼴����ʼ��
				set_sys_msg_mode 3 0 0
				map_msg_info
			endif
		endif
	endif
	;�߼�����1���,0δ������,1������
        GET_PWORLD_VAR 28 32 1 17
        If 17 = 1		
                var 17 2
                set_pworld_var 28 32 @17 1
		call 402
        endif
	;�߼�����2���,0δ������,1������
        GET_PWORLD_VAR 32 32 1 18
        If 18 = 1
                var 18 2
                set_pworld_var 32 32 @18 1
		call 403
        endif
	;�߼�����3���,0δ������,1������
        GET_PWORLD_VAR 36 32 1 19
        If 19 = 1
                var 19 2
                set_pworld_var 36 32 @19 1
		call 404
        endif
	;boss3�Ի����,0δ������,1������
	GET_PWORLD_VAR 40 32 1 20
        If 20 = 1
                var 20 2
                set_pworld_var 40 32 @20 1
		call 405
        endif
	if 20 = 2
		;boss3flag34,boss3npc�Ի����ʱ
		var 34 0
		get_pworld_var 96 32 1 34
		add 34 1
		;boss3flag34,boss3npc�Ի����ʱ
		
		set_pworld_var 96 32 @34 1
		if 34 > 7
			call 411
			var 20 3
			set_pworld_var 40 32 @20 1
		endif
	endif
	if 20 = 3
		get_mon_num_by_defid 206270 1
		if 1 < 1
			var 20 4
			set_pworld_var 40 32 @20 1
			call 406
		endif
	endif
	;boss3flagclose38,boss3����ս������
	var 38 0
	get_pworld_var 112 32 1 38
	if 38 = 1
		call 412
		var 38 2
		;boss3flagclose38,boss3����ս������
		
		set_pworld_var 112 32 @38 1
	endif
	;�߼�����4���,0δ������,1������
        GET_PWORLD_VAR 68 32 1 27
        If 27 = 1
		var 27 2
                set_pworld_var 68 32 @27 1
		call 408
	endif
	;�߼�����5���,0δ������,1������
        GET_PWORLD_VAR 72 32 1 28
        If 28 = 1
		var 28 2
		set_pworld_var 72 32 @28 1
		call 409
	endif
	;�߼�����6���,0δ������,1������
        GET_PWORLD_VAR 76 32 1 29
        If 29 = 1
	        var 29 2
                set_pworld_var 76 32 @29 1
		call 410
	endif
	;���ع�npc�Ի����,0δ������,1����������ͨ��
	get_pworld_var 44 32 1 21
	if 21 = 1
		call 600
		var 21 2
		set_pworld_var 44 32 @21 1
	endif	
	;�߼�����boss1������,0δ������,1��һ�ű���,2��һ�ű���,3�����ű���
	get_pworld_var 80 32 1 30
	if 30 > 0
		;11��boss���������ļ�ʱ
		get_pworld_var 4 32 1 11
		add 11 1
		set_pworld_var 4 32 @11 1
		mod 11 10
		if 11 = 0
			add 30 1
			if 30 > 3
				var 30 1
			endif
			set_pworld_var 80 32 @30 1  
		endif
	endif
	;boss1flag32,boss1�ٻ����,0δ����,1�Ժ�Ϊ��ʱ
	var 32 0
	get_pworld_var 88 32 1 32
	if 32 = 1
		strvar ret num
		var ret 0
		GET_MON_NUM_BY_DEFID 206297 num
		add ret @num
		GET_MON_NUM_BY_DEFID 206298 num
		add ret @num
		GET_MON_NUM_BY_DEFID 206299 num
		add ret @num
		GET_MON_NUM_BY_DEFID 206300 num
		add ret @num
		GET_MON_NUM_BY_DEFID 206301 num
		add ret @num
		GET_MON_NUM_BY_DEFID 206302 num
		add ret @num
		GET_MON_NUM_BY_DEFID 206303 num
		add ret @num
		GET_MON_NUM_BY_DEFID 206304 num
		add ret @num
		GET_MON_NUM_BY_DEFID 206305 num
		add ret @num
		GET_MON_NUM_BY_DEFID 206306 num
		add ret @num
		GET_MON_NUM_BY_DEFID 206307 num
		add ret @num
		GET_MON_NUM_BY_DEFID 206308 num
		add ret @num
		
;		add_sys_msg_start
;		add_sys_msg num$@ret$
;		set_sys_msg_mode 3 0 0
;		map_msg_info
		if ret < 1
			add 32 1
			;boss1flag32,boss1�ٻ����,0δ����,1�Ժ�Ϊ��ʱ
			
			set_pworld_var 88 32 @32 1
		endif
	endif
	if 32 = 2
		add_sys_msg_start
		add_sys_msg һ��������������������˫���ȱ���Щ��֪����ļһ
		set_sys_msg_mode 3 0 0
		map_msg_info
	endif
	if 32 > 1
;		sub 32 1
		if 32 > 20
			;��boss
			call 103
			var 32 0
			;boss1flag32,boss1�ٻ����,0δ����,1�Ժ�Ϊ��ʱ
			
			set_pworld_var 88 32 @32 1
		else
			add 32 1
			;boss1flag32,boss1�ٻ����,0δ����,1�Ժ�Ϊ��ʱ
			
			set_pworld_var 88 32 @32 1
		endif
	endif
	;boss5flag35,boss5ս�������
	var 35 0
	get_pworld_var 100 32 1 35
	if 35 = 1
		call 503
		;boss5flag35,boss5ս�������
		var 35 2
		set_pworld_var 100 32 @35 1
	endif
	;boss5flag36,boss5����ս������
	var 36 0
	get_pworld_var 104 32 1 36
	if 36 > 0
		call 504
		;boss5flag36,boss5����ս������
		var 36 0
		set_pworld_var 104 32 @36 1
	endif
	;boss3stage42,boss3ս���׶�
	var 42 0
	get_pworld_var 128 32 1 42
	if 42 = 1
		GET_PLAYER_NUM_RANGE 10 34553 29488 2800
		if 10 < 1
			var 42 2
			;boss3stage42,boss3ս���׶�
			
			set_pworld_var 128 32 @42 1
			;boss3flagclose38,boss3����ս������
			var 38 0
			get_pworld_var 112 32 1 38
			if 38 = 0 
				var 38 1
				;boss3flagclose38,boss3����ս������
				
				set_pworld_var 112 32 @38 1
			;	add_sys_msg_start
			;	add_sys_msg 123
			;	set_sys_msg_mode 3 0 0
			;	map_msg_info
			endif
			
			
			
		endif
	endif
	
	;�����õ�tick
;	call 99
ENDPROC
PROC 4
	strvar x
	strvar y
	strvar id
	GET_MON_INFO 0 id 0 0 0 0 x y
	
	;����������,�ж��Ƿ����������
	If id = 206268
;		get_pworld_var 4 32 1 11
;		get_pworld_var 8 32 1 12
;		if x = ::x8:: and 12 = ::y8::
			;boss1����
;			new_mon 206272 1 19344 10978 0 0 0 1 0
;		endif
	endif
	;��һ��boss����,���
	If id = 206272
		call 102
	endif
	;������������
	if id = 206246
		delete_mon_by_defid 206245 1
	endif
	if id = 206251
		delete_mon_by_defid 206250 1
	endif
	if id = 206253
		delete_mon_by_defid 206252 1
	endif
	if id = 206255
		delete_mon_by_defid 206254 1
	endif
	if id = 206257
		delete_mon_by_defid 206256 1
	endif
	if id = 206311
		delete_mon_by_defid 206309 1
		;��ͨ��
		call 302
	endif
	
	;������boss����
	if id = 206274
		call 303
	endif
	;���Ĳ�boss����
	if id = 206276
		call 407
	endif
	;�����˫�����
	if id = 206280
		call 502
	endif
	;����������ʱ����
	if id = 206281
		strvar x y
		GET_MON_INFO 1 0 0 0 0 0 x y
		var 10 0
		new_mon_by_dir 206283 1 @x @y 0 0 0 @10 1 2	
	endif
ENDPROC
proc 98
	;��һ��
	call 101 
	;����tick�õ�,��ֵΪ0�رմ˹���
	get_pworld_var 20 32 1 15
	if 15 > 0
		return 
	endif
	;�ڶ���,����
	call 201
	;������,���boss,��ͨ��
	call 301
	;���Ĳ�,�ұ�boss,��ͨ��
	call 401
	;�����,����boss,��ͨ��
	call 501
	;������,����boss
	call 601
endproc
proc 99
	;����tick�õ�,��ֵΪ0�رմ˹���
	get_pworld_var 20 32 1 15
	if 15 < 1
		return 
	endif
	;����tick�õ�,�׶�
	get_pworld_var 24 32 1 16
	if 16 = 0
		return
	elseif 16 = 1
		call 201
	elseif 16 = 2
		call 301
	elseif 16 = 3
		call 401
	elseif 16 = 4
		call 501
	elseif 16 = 5
		call 601
	else
	endif
	add_sys_msg_start
	add_sys_msg ������Խ׶�$@16$
	set_sys_msg_mode 4 0 0
	map_msg_info
	var 16 0
	set_pworld_var 24 32 @16 1
endproc
proc 100
	;���ø���������
	;10��ʱ��
	var 10 0
	set_pworld_var 0 32 @10 1
	;11��boss���������ļ�ʱ
	var 11 0
	set_pworld_var 4 32 @11 1
	;12��boss�����ֵ�λ��y
;	VAR 12 11026
;	set_pworld_var 8 32 @12 1
	;�����׶�,��Ҫ����tickʱҪ������proc
	VAR 13 0
	set_pworld_var 12 32 @13 1
	;����⻷վ�Դ���
	VAR 14 0
	set_pworld_var 16 32 @14 1
	;����tick�õ�,��ֵΪ0�رմ˹���
	VAR 15 0
	set_pworld_var 20 32 @15 1
	;����tick�õ�,�׶�
	VAR 16 0
	set_pworld_var 24 32 @16 1
	;�߼�����1���,0δ������,1������
	VAR 17 0
	set_pworld_var 28 32 @17 1
	;�߼�����2���,0δ������,1������
	VAR 18 0
	set_pworld_var 32 32 @18 1
	;�߼�����3���,0δ������,1������
	VAR 19 0
	set_pworld_var 36 32 @19 1
	;boss3�Ի����,0δ������,1������
	VAR 20 0
	set_pworld_var 40 32 @20 1
	;���ع�npc�Ի����,0δ������,1����������ͨ��
	VAR 21 0
	set_pworld_var 44 32 @21 1
	;�߼�����road1���,0δ������,1������
	VAR 22 0
	set_pworld_var 48 32 @22 1
	;�߼�����road2���,0δ������,1������
	VAR 23 0
	set_pworld_var 52 32 @23 1
	;�߼�����road3���,0δ������,1������
	VAR 24 0
	set_pworld_var 56 32 @24 1
	;�߼�����road4���,0δ������,1������
	VAR 25 0
	set_pworld_var 60 32 @25 1
	;�߼�����road5���,0δ������,1������
	VAR 26 0
	set_pworld_var 64 32 @26 1
	;�߼�����4���,0δ������,1������
	VAR 27 0
	set_pworld_var 68 32 @27 1
	;�߼�����5���,0δ������,1������
	VAR 28 0
	set_pworld_var 72 32 @28 1
	;�߼�����6���,0δ������,1������
	VAR 29 0
	set_pworld_var 76 32 @29 1
	;�߼�����boss1������,0δ������,1��һ�ű���,2��һ�ű���,3�����ű���
	VAR 30 0
	set_pworld_var 80 32 @30 1
	;roadtipflag31,������ʾ�߼������,0δ������,1����
	var 31 0
	set_pworld_var 84 32 @31 1
	;boss1flag32,boss1�ٻ����,0δ����,1�Ժ�Ϊ��ʱ
	var 32 0
	set_pworld_var 88 32 @32 1
	
	;ufoflag33,ufo������־,0����,1������
	var 33 0
	set_pworld_var 92 32 @33 1
	
	;boss3flag34,boss3npc�Ի����ʱ
	var 34 0
	set_pworld_var 96 32 @34 1
	;boss5flag35,boss5ս�������
	var 35 0
	set_pworld_var 100 32 @35 1
	;boss5flag36,boss5����ս������
	var 36 0
	set_pworld_var 104 32 @36 1
	;boss3flagopen37,boss3ս�������
	var 37 0
	set_pworld_var 108 32 @37 1
	;boss3flagclose38,boss3����ս������
	var 38 0
	set_pworld_var 112 32 @38 1
	;boss6flagspeech39,boss6����˫����ʱ˵��
	var 39 0
	set_pworld_var 116 32 @39 1
	;boss3flagspeech40,boss3��������ʱ˵��
	var 40 0
	set_pworld_var 120 32 @40 1
	;boss3npcmemid41,boss3npc��mid
	var 41 0
	set_pworld_var 124 32 @41 1
	;boss3stage42,boss3ս���׶�
	var 42 0
	set_pworld_var 128 32 @42 1
endproc
;ˢ��
;��һ��
proc 101
	;������
	new_mon 206303 1 18716 9953 0 0 0 1 0
	new_mon 206304 1 20023 10008 0 0 0 1 0
	new_mon 206305 1 18188 10955 0 0 0 1 0
	new_mon 206306 1 20553 11026 0 0 0 1 0
	new_mon 206307 1 18721 12025 0 0 0 1 0
	new_mon 206308 1 19997 12049 0 0 0 1 0
;	new_mon 206268 1 19364 12428 0 0 0 1 0
	;������
	NEW_NPC 1 206240 19422 15163 90 pw105lift1.mac
	NEW_NPC 1 206241 19346 20771 0 pw105lift2.mac
;	NEW_NPC 1 350064 19628 30660 0 pw105lift3.mac	
	NEW_NPC 1 206242 19499 33267 0 pw105lift4.mac
endproc
;һ��boss����
proc 102
	;������
;	delete_mon_by_defid 206268 1
;	;�赲
;	delete_mon_by_defid 261039 1
	;����tick�õ�,�׶�
	VAR 16 1
	set_pworld_var 24 32 @16 1
endproc
;һ����boss
proc 103
	
	new_mon 206272 1 19344 10978 0 0 0 1 0
endproc
;ˢ��
;�ڶ���
proc 201
	;һ�����������npc
	NEW_NPC 1 206238 18800 29200 0 pw105pwd.mac
	;һ�����عؿ���npc	
;	NEW_NPC 1 284052 20104 30534 0 pw105hidedoor.mac
endproc
;�ڶ���ˢ���뷽��
proc 202
	;ɾ�컷
	;delete_mon_by_defid 284049 1
	add_sys_msg_start
	add_sys_msg �������뿪ʼ��ע�⿴�Ǹ����ڲ�ͬ�ģ�Ȼ��վ��ȥ��
	set_sys_msg_mode 3 0 0
	map_msg_info
	strvar r
	rand r 9
	strvar x
	strvar y
	strvar posX
	strvar posY
	strvar offset
	;��ɫ284050
	;��ɫ284049
	;NEW_LIFE_MON #����id #�������� #����x #����y #����뾶 #Ѳ��ID #�Ƿ�������� #���� #����ʱ�� #���ر�־ #��һֻ�����ʵ��ID
	strvar id 
	strvar t
	strvar i
	var i 0
	var t 10
	var x -1
	
	while x < 2
		var y -1
		while y < 2
			var posX 19620
			var posY 29736
			var offset 350
			mul offset @x
			add posX @offset
			var offset 350 
			mul offset @y
			add posY @offset
			var id 284049
			if i = @r
				var id 284050
				if i != 0
					var id 284096
					add id @i
					sub id 1
				endif
				NEW_LIFE_MON @id 1 @posX @posY 0 0 0 0 @t 1 0
			else
				if i != 0
					var id 284088
					add id @i
					sub id 1
				endif
				new_mon @id 1 @posX @posY 0 0 0 1 0
			endif	
			add i 1	
			add y 1
		endwhile
		
		add x 1
		
		;
	endwhile
endproc
;�ڶ����������
proc 203
	;�ɵ�
	NEW_MON_MACHINE 1 1460058 0 1 19620 29736 0
	;ɾ�赲
	delete_mon_by_defid 261040 1
	add_sys_msg_start
	add_sys_msg ��ϲ���ǣ����Ŵ��ˣ����Ǿ���ά����ڵ�ʵ�����ֽ���һ����
	set_sys_msg_mode 3 0 0
	map_msg_info
	;����tick�õ�,�׶�
	VAR 16 2
	set_pworld_var 24 32 @16 1
	
	del_npc 206238 1
endproc
;ˢ��
;������
proc 301
;	call 304
;	;5������
;	strvar i onex oney twox twoy
;	var i 0
;	var onex 16112
;	var oney 30125
;	var twox 16090
;	var twoy 29306
;	while i < 1
;		
;		;��һ��
;		new_mon 206247 1 @onex @oney 0 0 0 1 0
;		add onex -1044
;		add oney -3
;
;		new_mon 206249 1 @onex @oney 0 0 0 1 0
;		add onex -1044
;		add oney -3
;
;		new_mon 206248 1 @onex @oney 0 0 0 1 0
;		
;
;		add onex 0
;		add oney -443
;		if i = 0
;			new_mon 206246 1 @onex @oney 0 0 0 1 0
;		endif
;		if i = 1
;			new_mon 206251 1 @onex @oney 0 0 0 1 0
;		endif
;		if i = 2
;			new_mon 206253 1 @onex @oney 0 0 0 1 0
;		endif
;		if i = 3
;			new_mon 206255 1 @onex @oney 0 0 0 1 0
;		endif
;		if i = 4
;			new_mon 206257 1 @onex @oney 0 0 0 1 0
;		endif		
;		
;		add onex -1044
;		add oney -3
;
;		add onex -1044
;		add oney -3
;
;		if i = 0
;			new_mon 206245 1 @onex @oney 0 0 0 1 0
;		endif
;		if i = 1
;			new_mon 206250 1 @onex @oney 0 0 0 1 0
;		endif
;		if i = 2
;			new_mon 206252 1 @onex @oney 0 0 0 1 0
;		endif
;		if i = 3
;			new_mon 206254 1 @onex @oney 0 0 0 1 0
;		endif
;		if i = 4
;			new_mon 206256 1 @onex @oney 0 0 0 1 0
;		endif
;		
;		sub onex 0
;		sub oney -443
;
;		add onex -1044
;		add oney -3
;
;		;�ڶ���
;		new_mon 206247 1 @twox @twoy 0 0 0 1 0
;		add twox -1044
;		add twoy -3
;
;		new_mon 206249 1 @twox @twoy 0 0 0 1 0
;		add twox -1044
;		add twoy -3
;
;		new_mon 206248 1 @twox @twoy 0 0 0 1 0
;		add twox -1044
;		add twoy -3
;
;		add twox -1044
;		add twoy -3
;
;		add twox -1044
;		add twoy -3
;
;		add i 1
;	endwhile
;
;	var onex 12106
;	var oney 30144
;	var twox 9020
;	var twoy 26527
;	
;	while i < 5
;		
;		;��һ��
;		new_mon 206247 1 @onex @oney 0 0 0 1 0
;		add onex 4
;		add oney -952
;
;		new_mon 206249 1 @onex @oney 0 0 0 1 0
;		add onex 4
;		add oney -952
;		add onex -1
;
;
;		new_mon 206248 1 @onex @oney 0 0 0 1 0
;		
;
;		add onex 457
;		add oney -1
;		if i = 0
;			new_mon 206246 1 @onex @oney 0 0 0 1 0
;		endif
;		if i = 1
;			new_mon 206251 1 @onex @oney 0 0 0 1 0
;		endif
;		if i = 2
;			new_mon 206253 1 @onex @oney 0 0 0 1 0
;		endif
;		if i = 3
;			new_mon 206255 1 @onex @oney 0 0 0 1 0
;		endif
;		if i = 4
;			new_mon 206257 1 @onex @oney 0 0 0 1 0
;		endif		
;		
;		add onex 4
;		add oney -952
;
;		add onex 4
;		add oney -952
;
;		if i = 0
;			new_mon 206245 1 @onex @oney 0 0 0 1 0
;		endif
;		if i = 1
;			new_mon 206250 1 @onex @oney 0 0 0 1 0
;		endif
;		if i = 2
;			new_mon 206252 1 @onex @oney 0 0 0 1 0
;		endif
;		if i = 3
;			new_mon 206254 1 @onex @oney 0 0 0 1 0
;		endif
;		if i = 4
;			new_mon 206256 1 @onex @oney 0 0 0 1 0
;		endif
;		
;		sub onex 457
;		sub oney -1
;
;		add onex 4
;		add oney -952
;
;		;�ڶ���
;		new_mon 206247 1 @twox @twoy 0 0 0 1 0
;		add twox 4
;		add twoy -952
;
;		new_mon 206249 1 @twox @twoy 0 0 0 1 0
;		add twox 4
;		add twoy -952
;
;		new_mon 206248 1 @twox @twoy 0 0 0 1 0
;		add twox 4
;		add twoy -952
;
;		add twox 4
;		add twoy -952
;
;		add twox 4
;		add twoy -952
;
;		add i 1
;	endwhile
	
	;link��
;	new_mon 206324 1 8700 10700 0 0 0 1 2
;
;	SET_MON_GROUP 1 @2 31
;	new_mon 206324 1 7700 10200 0 0 0 1 2
;
;	SET_MON_GROUP 1 @2 31
;	new_mon 206234 1 10000 9500 0 0 0 1 2
;
;	SET_MON_GROUP 1 @2 31
;	LINK_MON 1 31
;
;	new_mon 206324 1 7200 9500 0 0 0 1 2
;	SET_MON_GROUP 1 @2 32
;	new_mon 206324 1 9600 10100 0 0 0 1 2
;	SET_MON_GROUP 1 @2 32
;	new_mon 206234 1 8608 14420 0 0 0 1 2
;	SET_MON_GROUP 1 @2 32
;	LINK_MON 1 32
;
;
;	new_mon 206324 1 9060 10947 0 0 0 1 2
;	SET_MON_GROUP 1 @2 33
;
;	new_mon 206324 1 8200 10789 0 0 0 1 2
;	SET_MON_GROUP 1 @2 33
;
;	new_mon 206234 1 8618 11678 0 0 0 1 2
;	SET_MON_GROUP 1 @2 33
;	LINK_MON 1 33
	;boss
	new_mon 206274 1 8651 9133 0 0 0 1 0
;::substr206322С�ɻ��ֵ�id::284051::206322
endproc
;��ͨ��
proc 302
	;ufoflag33,ufo������־,0����,1������
	var 33 1
	set_pworld_var 92 32 @33 1
	delete_mon_by_defid 206247 1	
	delete_mon_by_defid 206249 1
	delete_mon_by_defid 206248 1
	delete_mon_by_defid 206322 1
	delete_mon_by_defid 1460058 1		
endproc
;����ͨ�����赲
proc 303
	;���Ŷ���
	;ɾ�赲
	delete_mon_by_defid 261041 1
	;�ִ�����
	NEW_NPC 1 360056 8641 7584 0 pw105trans.mac
	
	;����tick�õ�,�׶�
	VAR 16 3
	set_pworld_var 24 32 @16 1
;	add_sys_msg_start
;	add_sys_msg ��ɪ:�Ⲣ����ʲô������ɪ�����У����Ǿ��������ֽ���һ���� 
;	set_sys_msg_mode 1 0 0
;	map_msg_info
	;boss3npcmemid41,boss3npc��mid
	var 41 0
	get_pworld_var 124 32 1 41
	if 41 > 0
		mon_speech @41 1 2 3 �Ⲣ����ʲô������ɪ�����У����Ǿ��������ֽ���һ����
	endif
endproc
proc 304
	;
	;��һ��
	new_mon 206247 1 16112 30125 0 0 0 1 0
	new_mon 206247 1 8202 20617 0 0 0 1 0
	new_mon 206247 1 8204 24538 0 0 0 1 0
	new_mon 206247 1 8658 28503 0 0 0 1 0
	new_mon 206247 1 12106 30144 0 0 0 1 0
	new_mon 206249 1 15072 30139 0 0 0 1 0
	new_mon 206249 1 8191 19634 0 0 0 1 0
	new_mon 206249 1 8175 23578 0 0 0 1 0
	new_mon 206249 1 8257 27490 0 0 0 1 0
	new_mon 206249 1 11227 30138 0 0 0 1 0
	new_mon 206248 1 14113 30141 0 0 0 1 0
	new_mon 206248 1 8185 18707 0 0 0 1 0
	new_mon 206248 1 8187 22536 0 0 0 1 0
	new_mon 206248 1 8186 26541 0 0 0 1 0
	new_mon 206248 1 10161 29841 0 0 0 1 0
	;�ڶ���
	new_mon 206247 1 16112 29261 0 0 0 1 0
	new_mon 206247 1 9080 20588 0 0 0 1 0
	new_mon 206247 1 9069 24516 0 0 0 1 0
	new_mon 206247 1 9404 28071 0 0 0 1 0
	new_mon 206247 1 12140 29289 0 0 0 1 0
	new_mon 206249 1 15089 29256 0 0 0 1 0
	new_mon 206249 1 9076 19625 0 0 0 1 0
	new_mon 206249 1 9075 23559 0 0 0 1 0
	new_mon 206249 1 9111 27303 0 0 0 1 0
	new_mon 206249 1 11275 29254 0 0 0 1 0
	new_mon 206248 1 14131 29288 0 0 0 1 0
	new_mon 206248 1 9063 18674 0 0 0 1 0
	new_mon 206248 1 9055 22530 0 0 0 1 0
	new_mon 206248 1 9059 26532 0 0 0 1 0
	new_mon 206248 1 10494 29027 0 0 0 1 0
	var 10 45
	new_mon_by_dir 206246 1 13380 29701 0 0 0 @10 1 2
	new_mon_by_dir 206245 1 13152 29697 0 0 0 @10 1 2
	new_mon_by_dir 206245 1 13146 29274 0 0 0 @10 1 2
	new_mon_by_dir 206245 1 13165 30109 0 0 0 @10 1 2
	var 10 22
	new_mon_by_dir 206251 1 9810 29123 0 0 0 @10 1 2
	new_mon_by_dir 206250 1 9590 28991 0 0 0 @10 1 2
	new_mon_by_dir 206250 1 9752 28780 0 0 0 @10 1 2
	new_mon_by_dir 206250 1 9372 29204 0 0 0 @10 1 2
	var 10 0
	new_mon_by_dir 206253 1 8632 25673 0 0 0 @10 1 2
	new_mon_by_dir 206252 1 8637 25494 0 0 0 @10 1 2
	new_mon_by_dir 206252 1 8346 25483 0 0 0 @10 1 2
	new_mon_by_dir 206252 1 8940 25500 0 0 0 @10 1 2
	var 10 0
	new_mon_by_dir 206255 1 8631 21805 0 0 0 @10 1 2
	new_mon_by_dir 206254 1 8640 21630 0 0 0 @10 1 2
	new_mon_by_dir 206254 1 8328 21641 0 0 0 @10 1 2
	new_mon_by_dir 206254 1 8931 21644 0 0 0 @10 1 2
	var 10 0
	new_mon_by_dir 206257 1 8642 17830 0 0 0 @10 1 2
	new_mon_by_dir 206256 1 8640 17640 0 0 0 @10 1 2
	new_mon_by_dir 206256 1 8327 17659 0 0 0 @10 1 2
	new_mon_by_dir 206256 1 8933 17634 0 0 0 @10 1 2
endproc
;ˢ��
;���Ĳ�
proc 401
	
	;boss npc
	NEW_NPC 1 317013 34553 29488 45 pw105boss3.mac
	;boss3npcmemid41,boss3npc��mid
	var 41 @1
	set_pworld_var 124 32 @41 1
endproc
;����1�й�
proc 402
;	new_mon 206323 1 21967 29700 0 0 0 1 0
	destroy_mon_by_defid 1 206282
endproc
;����2�й�
proc 403
;	new_mon 206323 1 24400 29716 0 0 0 1 0
	destroy_mon_by_defid 1 206284
endproc
;����3�й�
proc 404
;	new_mon 206323 1 26672 29678 0 0 0 1 0
	destroy_mon_by_defid 1 206285
endproc
;boss3 �Ի���,����
proc 405
	;ɾufo
	delete_mon_by_defid 1460058 1	
	;ˢ��ʱ��е
	;ȱ�ӿ�
;	::substr14600563�ŷ������ʱ��е. ������::1460056::1460056
;	::substr14600573�ŷ������ʱ��е. ������::1460057::1460057
	var 10 160
	new_mon_machine_by_dir 1 1460056 0 1 33324 30219 0 @10
	
	var 10 164
	new_mon_machine_by_dir 1 1460057 0 1 33589 30458 0 @10
	
	var 10 168
	new_mon_machine_by_dir 1 1460057 0 1 33817 30657 0 @10
	var 10 172
	new_mon_machine_by_dir 1 1460057 0 1 34096 30813 0 @10
	var 10 176
	new_mon_machine_by_dir 1 1460057 0 1 34401 30900 0 @10
	var 10 180
	new_mon_machine_by_dir 1 1460056 0 1 34704 30864 0 @10
	;����
	var 10 45
;	new_mon_by_dir 261039 1 31544 29058 0 0 0 @10 1 2
;	new_mon_by_dir 261039 1 31544 28758 0 0 0 @10 1 2
;	new_mon_by_dir 261039 1 31489 30074 0 0 0 @10 1 2
;	new_mon_by_dir 261039 1 31510 29844 0 0 0 @10 1 2
;	new_mon_by_dir 261039 1 31521 29618 0 0 0 @10 1 2
;	new_mon_by_dir 261039 1 31515 29435 0 0 0 @10 1 2
;	new_mon_by_dir 261039 1 31544 29058 0 0 0 @10 1 2
	;boss3flagclose38,boss3����ս������
	var 38 0
	set_pworld_var 112 32 @38 1
	;boss3flag34,boss3npc�Ի����ʱ
	var 34 0
	set_pworld_var 96 32 @34 1
	add_sys_msg_start
	add_sys_msg ѡ��һ����ʱ��е��
	set_sys_msg_mode 3 0 0
	map_msg_info
;	::substr206270�����޹���::206270::206270
endproc
;��һ����
proc 406
	;ɾnpc
	;��boss
	;EFFECT_ID 0 @19 playMontion(%d,"282013_18")
	;ɾnpc
	DEL_NPC 317013 1	
	;��boss
	var 10 0
	NEW_MON_BY_DIR 206278 1 34553 29488 0 0 0 10 1 2        ;����ʦ/���BOSS��
	delete_mon_by_defid 1460057 1
	delete_mon_by_defid 1460056 1
	;boss3stage42,boss3ս���׶�
	var 42 2
	set_pworld_var 128 32 @42 1
endproc
;�Ĳ�boss������
proc 407
	;ɾ�赲
	delete_mon_by_defid 261042 1
	delete_mon_by_defid 261039 1
	
	;����tick�õ�,�׶�
	VAR 16 4
	set_pworld_var 24 32 @16 1
endproc
;����4�й�
proc 408
;	new_mon 206323 1 26672 29678 0 0 0 1 0
	destroy_mon_by_defid 1 206286
endproc
;����5�й�
proc 409
;	new_mon 206323 1 26672 29678 0 0 0 1 0
	destroy_mon_by_defid 1 206287
endproc
;����6�й�
proc 410
;	new_mon 206323 1 26672 29678 0 0 0 1 0
	destroy_mon_by_defid 1 206289
endproc
;��boss3�ԱߵĹ�
proc 411
	;�Ź�
	;����һ��
	new_mon 206270 1 35500 29200 0 0 0 1 0
	new_mon 206270 1 35300 28900 0 0 0 1 0
	new_mon 206270 1 34900 28600 0 0 0 1 0
	new_mon 206270 1 35600 29600 0 0 0 1 0
	new_mon 206270 1 34600 28600 0 0 0 1 0	
	add_sys_msg_start
	add_sys_msg �ս��߳����ˣ�
	set_sys_msg_mode 3 0 0
	map_msg_info
	;boss3stage42,boss3ս���׶�
	var 42 1
	set_pworld_var 128 32 @42 1
endproc
;��սboss3ʧ�ܺ���
proc 412
	DEL_NPC 317013 1	
	destroy_mon_by_defid 1 206278
	destroy_mon_by_defid 1 1460057
	destroy_mon_by_defid 1 1460056
	destroy_mon_by_defid 1 206270
	destroy_mon_by_defid 1 261039
	destroy_mon_by_defid 1 206276
	;boss3�Ի����,0δ������,1������
	VAR 20 0
	SET_PWORLD_VAR 40 32 @20 1
	;��npc
	CALL 401
endproc
;ˢ��
;�����
proc 501
;	new_mon 206243 1 34628 23065 0 0 0 1 0
;	new_mon 206243 1 34587 15626 0 0 0 1 0
;	new_mon 206243 1 34537 16573 0 0 0 1 0
;	new_mon 206243 1 34610 17646 0 0 0 1 0
;	new_mon 206243 1 34570 18796 0 0 0 1 0
;	new_mon 206243 1 34525 19952 0 0 0 1 0
;	new_mon 206243 1 34517 21130 0 0 0 1 0
;	new_mon 206243 1 34537 22174 0 0 0 1 0
	strvar memid
	new_mon 206280 1 34610 14087 0 0 0 1 memid
	TARGET_ADD_STATUS 3 @memid 3006 1 30 0 
	new_mon 206258 1 34610 14087 0 0 0 1 0
endproc
;ɾ��
;�����
proc 502
	;ɾ������
	DELETE_MON_BY_DEFID 206258 1 
	;�����еĹ�
	delete_mon_by_defid 261035 1
	delete_mon_by_defid 261036 1
	delete_mon_by_defid 261037 1
	delete_mon_by_defid 261038 1
	delete_mon_by_defid 282037 1
	delete_mon_by_defid 282038 1
	delete_mon_by_defid 282039 1
	delete_mon_by_defid 282040 1
	
	;�ִ�����
	NEW_NPC 1 333048 35000 13400 0 pw105out.mac
	;����
	delete_mon_by_defid 206317 1
	PWORLD_BOX_START 0
	pworld_over
	OPEN_PWORLD_AWARD_UI
	
	
endproc
;����
;�����
proc 503
	return
	;ɾufo
	delete_mon_by_defid 1460058 1	
	var 10 0
	new_mon_by_dir 206317 1 34665 15413 0 0 0 @10 1 2
	new_mon_by_dir 206317 1 34254 15407 0 0 0 @10 1 2
	new_mon_by_dir 206317 1 34864 15438 0 0 0 @10 1 2
	;boss5flag36,boss5����ս������
	var 36 0
	set_pworld_var 104 32 @36 1
endproc
;����
;�����
proc 504
	DELETE_MON_BY_DEFID 206317 1 
	;boss5flag35,boss5ս�������
	var 35 0
	set_pworld_var 100 32 @35 1
endproc
;6��ɾ�赲
proc 600
	;ɾ�赲
	delete_mon_by_defid 261043 1
	;����tick�õ�,�׶�
	VAR 16 5
	set_pworld_var 24 32 @16 1
endproc
;ˢ��
;������
proc 601
	new_mon 284066 1 19526 36649 0 0 0 1 0
	new_mon 206281 1 19526 36649 0 0 0 1 0
endproc
;�����赲 
proc 701
	var 10 0
	new_mon_by_dir 261039 1 19430 12754 0 0 0 @10 1 2
	new_mon_by_dir 261039 1 18814 12785 0 0 0 @10 1 2
	new_mon_by_dir 261039 1 19623 12762 0 0 0 @10 1 2
	new_mon_by_dir 261039 1 18608 12761 0 0 0 @10 1 2
	new_mon_by_dir 261039 1 20167 12768 0 0 0 @10 1 2
	new_mon_by_dir 261039 1 19830 12770 0 0 0 @10 1 2
	new_mon_by_dir 261039 1 19060 12771 0 0 0 @10 1 2
	var 10 45
	new_mon_by_dir 261040 1 18599 29710 0 0 0 @10 1 2
;	new_mon_by_dir 261040 1 18550 29959 0 0 0 @10 1 2
;	new_mon_by_dir 261040 1 18543 29481 0 0 0 @10 1 2
	var 10 45
;	new_mon_by_dir 261041 1 20638 29712 0 0 0 @10 1 2
;	new_mon_by_dir 261041 1 20674 29991 0 0 0 @10 1 2
;	new_mon_by_dir 261041 1 20687 29414 0 0 0 @10 1 2
	var 10 0
	new_mon_by_dir 261042 1 34611 26447 0 0 0 @10 1 2
	new_mon_by_dir 261042 1 33980 26399 0 0 0 @10 1 2
	new_mon_by_dir 261042 1 34261 26404 0 0 0 @10 1 2
	new_mon_by_dir 261042 1 34925 26434 0 0 0 @10 1 2
	new_mon_by_dir 261042 1 35179 26465 0 0 0 @10 1 2
;	new_mon_by_dir 261042 1 31449 29750 0 0 0 @10 1 2
;	var 10 0
;	new_mon_by_dir 261043 1 19624 30486 0 0 0 @10 1 2
;	var 10 45
;	new_mon_by_dir 261043 1 19804 30602 0 0 0 @10 1 2
;	new_mon_by_dir 261043 1 19419 30592 0 0 0 @10 1 2
endproc
;����,������
PROC 10
	pworld_again
ENDPROC
proc 11
        OUT_PWORLD_BY_SINGLE
endproc
PROC 12
	pworld_over
ENDPROC
PROC 13
	delete_mon_by_defid 261039 1
	delete_mon_by_defid 261040 1
	delete_mon_by_defid 261041 1
	delete_mon_by_defid 261042 1
	delete_mon_by_defid 261043 1
	delete_mon_by_defid 206245 1
	delete_mon_by_defid 206250 1
	delete_mon_by_defid 206252 1
	delete_mon_by_defid 206254 1
	delete_mon_by_defid 206256 1
ENDPROC
proc 14
	;����⻷վ�Դ���
	VAR 14 6
	set_pworld_var 16 32 @14 1
endproc
