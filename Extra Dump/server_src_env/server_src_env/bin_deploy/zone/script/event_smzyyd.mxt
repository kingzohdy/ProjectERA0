;���������˶�   �¼�ID:54     �����ID:96

PROC 1
	;ʱ��ڼ��롣�ó����ľ仰
	SET_EVENT_VAR 0 32 0 1
	;Ҫð�ݻ���ܻ��ǰ䲼ָ��    1Ϊ�䲼ָ��   0��ִ��ð�ݻ����
	SET_EVENT_VAR 4 32 0 1
	;proc 202 �У�0 ִ��rand   1ִ����һЩ����...����proc 300 ~ proc 313 �� ��ִ���ľ�mon_speech 0��һ�䣬1�ڶ���
	SET_EVENT_VAR 8 32 0 1
	;��¼��ʲô����   0 ~ 13  �ֱ��Ӧ��ͬ�Ķ���
	strvar Loc
	var Loc 0
	SET_EVENT_VAR 12 32 @Loc 1
	
	;�ں��߼��������鶯������25��  
	SET_EVENT_VAR 16 32 0 1
	
	;������100���������ˣ���ˢ����
	SET_EVENT_VAR 20 32 0 1
	
	strvar time 
	GET_TIME_SEC time
	SET_EVENT_VAR 24 32 @time 1
	
	;��¼��־�ֵ�����X,Y
	SET_EVENT_VAR 28 32 0 1
	SET_EVENT_VAR 32 32 0 1
		
	add_sys_msg_start
	add_sys_msg ���������˶����װ��������ǣ����������⺣̲�ȴ�������һ�����˶�Ӵ��
	set_sys_msg_mode 6 0 0
	svr_msg_info
ENDPROC

PROC 2		
	add_sys_msg_start
	add_sys_msg �ʵ����˶��ܹ�ǿ���壬����������ģ������˶����˽�����������л��λ����飡
	set_sys_msg_mode 6 0 0
	svr_msg_info
ENDPROC

;10�봥��һ��
PROC 3	
	GET_EVENT_VAR 16 32 1 30
	if @30 > 25
		return
	endif

	GET_EVENT_VAR 4 32 1 20
	if @20 = 0
		call 201
	else 
	   call 202
	endif
ENDPROC

proc 201
	GET_MAP_INST 4403 10
	strvar npcmid
	GET_MAP_NPC_ID  304125 npcmid 1
	GET_EVENT_VAR 0 32 1 50
	if @50 = 0
		MON_SPEECH  @npcmid 0 2 1 ��ӭ��λͬѧ�����˼�һ�����˶������ȷ������ģ����˼ҽ����˶����ɡ�#010#010#003#003
	elseif @50 = 1
		MON_SPEECH  @npcmid 0 2 1 ֻҪ���#G���촰�����½ǵ�Ц��ͼ��#n���Ϳ��Դ򿪶���ָ����塣#037#022
	elseif @50 = 2
		MON_SPEECH  @npcmid 0 2 1 #G�����������еĶ���ָ��ͼ��#n���Ϳ���������Ӧ�Ķ����ˣ�#003#023
	elseif @50 = 3
		MON_SPEECH  @npcmid 0 2 1 �Ժ��˼Ҹ�������������ĵƹ⣬�˼�ֻ�ῴ���ڵƹ��µ�ͬѧ��#028#003
	elseif @50 = 4
		MON_SPEECH  @npcmid 0 2 1 �ڵƹ���ÿ����һ�������������ý���������Ҫ����ע����Ŷ��#022#022
	elseif @50 = 5
		MON_SPEECH  @npcmid 0 2 1 ������ϣ������������ף����Կ�����鿴�˼ҵġ��˶����ɡ����ܡ�#003#003#003
	elseif @50 = 6
		MON_SPEECH  @npcmid 0 2 1 ׼���������װ��ģ��˼��Ѿ���ʼ�Ȳ���������һ�����˶��ˣ�#022#037#010
	elseif @50 = 7
		MON_SPEECH  @npcmid 0 2 1 Ԥ�����˼�Ҫ��ʼ��ʾ�����ˡ�#056#056#056
		SET_EVENT_VAR 4 32 1 1
	else
		return
	endif
	
	add 50 1
	SET_EVENT_VAR 0 32 @50 1
endproc


proc 202	
	GET_EVENT_VAR 16 32 1 30
	if @30 > 25
		return
	endif
	
	GET_EVENT_VAR 8 32 1 21
	if @21 = 0			
		rand 12 14
		add 12 1
		switch 12
			case 1
				SET_EVENT_VAR 12 32 1 1
				call 301
			endcase

			case 2
				SET_EVENT_VAR 12 32 2 1
				call 302
			endcase
					
			case 3
				SET_EVENT_VAR 12 32 3 1
				call 303
			endcase
					
			case 4
				SET_EVENT_VAR 12 32 4 1
				call 304
			endcase
					
			case 5
				SET_EVENT_VAR 12 32 5 1
				call 305
			endcase
				
			case 6
				SET_EVENT_VAR 12 32 6 1
				call 306
			endcase
					
			case 7
				SET_EVENT_VAR 12 32 7 1
				call 307
			endcase
					
			case 8
				SET_EVENT_VAR 12 32 8 1
				call 308
			endcase
					
			case 9
				SET_EVENT_VAR 12 32 9 1
				call 309
			endcase

			case 10
				SET_EVENT_VAR 12 32 10 1
				call 310
			endcase
					
			case 11
				SET_EVENT_VAR 12 32 11 1
				call 311
			endcase
					
			case 12
	    		SET_EVENT_VAR 12 32 12 1
				call 312
			endcase
					
			case 13
	    		SET_EVENT_VAR 12 32 13 1
				call 313
	    	endcase
			
			case 14
				SET_EVENT_VAR 12 32 14 1
				call 300
			endcase
		endswitch
		
		SET_EVENT_VAR 8 32 1 1
		
		;�ڼ�������
		GET_EVENT_VAR 16 32 1 30
		add 30 1
		SET_EVENT_VAR 16 32 @30 1
	else		
		strvar Loc
		GET_EVENT_VAR 12 32 1 Loc
		if @Loc = 1
			call 301
		elseif @Loc = 2
			call 302
		elseif @Loc = 3
			call 303
		elseif @Loc = 4
			call 304
		elseif @Loc = 5
			call 305
		elseif @Loc = 6
			call 306
		elseif @Loc = 7
			call 307
		elseif @Loc = 8
			call 308
		elseif @Loc = 9
			call 309
		elseif @Loc = 10
			call 310
		elseif @Loc = 11
			call 311
		elseif @Loc = 12
			call 312
		elseif @Loc = 13
			call 313
		elseif @Loc = 14
			call 300
		else
			return
		endif
		
		SET_EVENT_VAR 8 32 0 1
		
		;��¼��־�ֵ�����X,Y   ��0
		SET_EVENT_VAR 28 32 0 1
		SET_EVENT_VAR 32 32 0 1
		
		;���25�ζ�������
		GET_EVENT_VAR 16 32 1 30
		
		if @30 >= 25
			GET_MAP_INST 4403 10
			strvar npcmid
			GET_MAP_NPC_ID  304125 npcmid 1
			MON_SPEECH  @npcmid 0 2 1 �����˶����˽������װ��ģ�ϣ���´��㻹�������������˶���#006#006#006
			
			add 30 1
			SET_EVENT_VAR 16 32 @30 1
			return
		endif
	endif
endproc

proc 300 
	GET_MAP_INST 4403 10
	strvar npcmid
	GET_MAP_NPC_ID  304125 npcmid 1
	
	strvar Loc2 Loc3
	GET_EVENT_VAR 8 32 1 Loc2
	if @Loc2 = 0
		mon_speech @npcmid 0 2 1 #G����#n����ʼ��#002#002#002#002
		
		strvar outx outy
		GET_MAP_INST 4403 10
		;��������22475 15738   ��12��֮�ڣ�ˢ��־��
		GET_MAP_RANDPOINT  22475 15738 1200 56 outx outy 
		;����: ��ȡĳ����ͼ�����ĳ���뾶��Χ�ڵ��������
		if @56 = 0
			NEW_LIFE_MON 203803 1 @outx @outy 0 0 0 0 10 0 0
			SET_EVENT_VAR 28 32 @outx 1
			SET_EVENT_VAR 32 32 @outy 1
		endif
		
		SET_EVENT_VAR 8 32 1 1
	else
		mon_speech @npcmid 0 2 1 ����������������һ������������ʼ��#002#002#002#002
		SET_EVENT_VAR 8 32 0 1
		;�ۻ���ͬʱ���Ե��������¸�������ʼǰ��Ӧ����0
		SET_EVENT_VAR 20 32 0 1
	endif
endproc

proc 301 
	GET_MAP_INST 4403 10
	strvar npcmid
	GET_MAP_NPC_ID  304125 npcmid 1
	
	strvar Loc2 Loc3
	GET_EVENT_VAR 8 32 1 Loc2
	if @Loc2 = 0
		mon_speech @npcmid 0 2 1 #G����#n����ʼ��#022#022#022#022
		
		strvar outx outy
		GET_MAP_INST 4403 10
		;��������22475 15738   ��12��֮�ڣ�ˢ��־��
		GET_MAP_RANDPOINT  22475 15738 1200 56 outx outy 
		;����: ��ȡĳ����ͼ�����ĳ���뾶��Χ�ڵ��������
		if @56 = 0
			NEW_LIFE_MON 203803 1 @outx @outy 0 0 0 0 10 0 0
			SET_EVENT_VAR 28 32 @outx 1
			SET_EVENT_VAR 32 32 @outy 1
		endif
		
		SET_EVENT_VAR 8 32 1 1
	else
		mon_speech @npcmid 0 2 1 �����ǡ���������һ������������ʼ��#022#022#022#022
		SET_EVENT_VAR 8 32 0 1
		;�ۻ���ͬʱ���Ե��������¸�������ʼǰ��Ӧ����0
		SET_EVENT_VAR 20 32 0 1
	endif
endproc

proc 302 
	GET_MAP_INST 4403 10
	strvar npcmid
	GET_MAP_NPC_ID  304125 npcmid 1
	
	strvar Loc2 Loc3
	GET_EVENT_VAR 8 32 1 Loc2
	if @Loc2 = 0
		mon_speech @npcmid 0 2 1 #G����#n����ʼ��#009#009#009#009
		
		strvar outx outy
		GET_MAP_INST 4403 10
		;��������22475 15738   ��12��֮�ڣ�ˢ��־��
		GET_MAP_RANDPOINT  22475 15738 1200 56 outx outy 
		;����: ��ȡĳ����ͼ�����ĳ���뾶��Χ�ڵ��������
		if @56 = 0
			NEW_LIFE_MON 203803 1 @outx @outy 0 0 0 0 10 0 0
			SET_EVENT_VAR 28 32 @outx 1
			SET_EVENT_VAR 32 32 @outy 1
		endif
		
		SET_EVENT_VAR 8 32 1 1
	else
		mon_speech @npcmid 0 2 1 �����衱��������һ������������ʼ��#009#009#009#009
		SET_EVENT_VAR 8 32 0 1
		;�ۻ���ͬʱ���Ե��������¸�������ʼǰ��Ӧ����0
		SET_EVENT_VAR 20 32 0 1
	endif
endproc

proc 303 
	GET_MAP_INST 4403 10
	strvar npcmid
	GET_MAP_NPC_ID  304125 npcmid 1
	
	strvar Loc2 Loc3
	GET_EVENT_VAR 8 32 1 Loc2
	if @Loc2 = 0
		mon_speech @npcmid 0 2 1 #G����#n����ʼ��#023#023#023#023
		
		strvar outx outy
		GET_MAP_INST 4403 10
		;��������22475 15738   ��12��֮�ڣ�ˢ��־��
		GET_MAP_RANDPOINT  22475 15738 1200 56 outx outy 
		;����: ��ȡĳ����ͼ�����ĳ���뾶��Χ�ڵ��������
		if @56 = 0
			NEW_LIFE_MON 203803 1 @outx @outy 0 0 0 0 10 0 0
			SET_EVENT_VAR 28 32 @outx 1
			SET_EVENT_VAR 32 32 @outy 1
		endif
		
		SET_EVENT_VAR 8 32 1 1
	else
		mon_speech @npcmid 0 2 1 �����衱��������һ������������ʼ��#023#023#023#023
		SET_EVENT_VAR 8 32 0 1
		;�ۻ���ͬʱ���Ե��������¸�������ʼǰ��Ӧ����0
		SET_EVENT_VAR 20 32 0 1
	endif
endproc

proc 304 
	GET_MAP_INST 4403 10
	strvar npcmid
	GET_MAP_NPC_ID  304125 npcmid 1
	
	strvar Loc2 Loc3
	GET_EVENT_VAR 8 32 1 Loc2
	if @Loc2 = 0
		mon_speech @npcmid 0 2 1 #Gװ��#n����ʼ��#031#031#031#031
		
		strvar outx outy
		GET_MAP_INST 4403 10
		;��������22475 15738   ��12��֮�ڣ�ˢ��־��
		GET_MAP_RANDPOINT  22475 15738 1200 56 outx outy 
		;����: ��ȡĳ����ͼ�����ĳ���뾶��Χ�ڵ��������
		if @56 = 0
			NEW_LIFE_MON 203803 1 @outx @outy 0 0 0 0 10 0 0
			SET_EVENT_VAR 28 32 @outx 1
			SET_EVENT_VAR 32 32 @outy 1
		endif
		
		SET_EVENT_VAR 8 32 1 1
	else
		mon_speech @npcmid 0 2 1 ��װ������������һ������������ʼ��#031#031#031#031
		SET_EVENT_VAR 8 32 0 1
		;�ۻ���ͬʱ���Ե��������¸�������ʼǰ��Ӧ����0
		SET_EVENT_VAR 20 32 0 1
	endif
endproc

proc 305 
	GET_MAP_INST 4403 10
	strvar npcmid
	GET_MAP_NPC_ID  304125 npcmid 1
	
	strvar Loc2 Loc3
	GET_EVENT_VAR 8 32 1 Loc2
	if @Loc2 = 0
		mon_speech @npcmid 0 2 1 #G����#n����ʼ��#037#037#037#037
		
		strvar outx outy
		GET_MAP_INST 4403 10
		;��������22475 15738   ��12��֮�ڣ�ˢ��־��
		GET_MAP_RANDPOINT  22475 15738 1200 56 outx outy 
		;����: ��ȡĳ����ͼ�����ĳ���뾶��Χ�ڵ��������
		if @56 = 0
			NEW_LIFE_MON 203803 1 @outx @outy 0 0 0 0 10 0 0
			SET_EVENT_VAR 28 32 @outx 1
			SET_EVENT_VAR 32 32 @outy 1
		endif
		
		SET_EVENT_VAR 8 32 1 1
	else
		mon_speech @npcmid 0 2 1 ����������������һ������������ʼ��#037#037#037#037
		SET_EVENT_VAR 8 32 0 1
		;�ۻ���ͬʱ���Ե��������¸�������ʼǰ��Ӧ����0
		SET_EVENT_VAR 20 32 0 1
	endif
endproc

proc 306 
	GET_MAP_INST 4403 10
	strvar npcmid
	GET_MAP_NPC_ID  304125 npcmid 1
	
	strvar Loc2 Loc3
	GET_EVENT_VAR 8 32 1 Loc2
	if @Loc2 = 0
		mon_speech @npcmid 0 2 1 #G����#n����ʼ��#002#002#002#002
		
		strvar outx outy
		GET_MAP_INST 4403 10
		;��������22475 15738   ��12��֮�ڣ�ˢ��־��
		GET_MAP_RANDPOINT  22475 15738 1200 56 outx outy 
		;����: ��ȡĳ����ͼ�����ĳ���뾶��Χ�ڵ��������
		if @56 = 0
			NEW_LIFE_MON 203803 1 @outx @outy 0 0 0 0 10 0 0
			SET_EVENT_VAR 28 32 @outx 1
			SET_EVENT_VAR 32 32 @outy 1
		endif
		
		SET_EVENT_VAR 8 32 1 1
	else	
		mon_speech @npcmid 0 2 1 �����ơ���������һ������������ʼ��#002#002#002#002
		SET_EVENT_VAR 8 32 0 1
		;�ۻ���ͬʱ���Ե��������¸�������ʼǰ��Ӧ����0
		SET_EVENT_VAR 20 32 0 1
	endif
endproc

proc 307 
	GET_MAP_INST 4403 10
	strvar npcmid
	GET_MAP_NPC_ID  304125 npcmid 1
	
	strvar Loc2 Loc3
	GET_EVENT_VAR 8 32 1 Loc2
	if @Loc2 = 0
		mon_speech @npcmid 0 2 1 #Gˣ��#n����ʼ��#003#003#003#003
		
		strvar outx outy
		GET_MAP_INST 4403 10
		;��������22475 15738   ��12��֮�ڣ�ˢ��־��
		GET_MAP_RANDPOINT  22475 15738 1200 56 outx outy 
		;����: ��ȡĳ����ͼ�����ĳ���뾶��Χ�ڵ��������
		if @56 = 0
			NEW_LIFE_MON 203803 1 @outx @outy 0 0 0 0 10 0 0
			SET_EVENT_VAR 28 32 @outx 1
			SET_EVENT_VAR 32 32 @outy 1
		endif
		
		SET_EVENT_VAR 8 32 1 1
	else
		mon_speech @npcmid 0 2 1 ��ˣ�ᡱ��������һ������������ʼ��#003#003#003#003
		SET_EVENT_VAR 8 32 0 1
		;�ۻ���ͬʱ���Ե��������¸�������ʼǰ��Ӧ����0
		SET_EVENT_VAR 20 32 0 1
	endif
endproc

proc 308 
	GET_MAP_INST 4403 10
	strvar npcmid
	GET_MAP_NPC_ID  304125 npcmid 1
	
	strvar Loc2 Loc3
	GET_EVENT_VAR 8 32 1 Loc2
	if @Loc2 = 0
		mon_speech @npcmid 0 2 1 #G����#n����ʼ��#009#009#009#009
		
		strvar outx outy
		GET_MAP_INST 4403 10
		;��������22475 15738   ��12��֮�ڣ�ˢ��־��
		GET_MAP_RANDPOINT  22475 15738 1200 56 outx outy 
		;����: ��ȡĳ����ͼ�����ĳ���뾶��Χ�ڵ��������
		if @56 = 0
			NEW_LIFE_MON 203803 1 @outx @outy 0 0 0 0 10 0 0
			SET_EVENT_VAR 28 32 @outx 1
			SET_EVENT_VAR 32 32 @outy 1
		endif
		
		SET_EVENT_VAR 8 32 1 1
	else
		mon_speech @npcmid 0 2 1 �����֡���������һ������������ʼ��#009#009#009#009
		SET_EVENT_VAR 8 32 0 1
		;�ۻ���ͬʱ���Ե��������¸�������ʼǰ��Ӧ����0
		SET_EVENT_VAR 20 32 0 1
	endif
endproc

proc 309 
	GET_MAP_INST 4403 10
	strvar npcmid
	GET_MAP_NPC_ID  304125 npcmid 1
	
	strvar Loc2 Loc3
	GET_EVENT_VAR 8 32 1 Loc2
	if @Loc2 = 0
		mon_speech @npcmid 0 2 1 #G��Ҿ#n����ʼ��#004#004#004#004
		
		strvar outx outy
		GET_MAP_INST 4403 10
		;��������22475 15738   ��12��֮�ڣ�ˢ��־��
		GET_MAP_RANDPOINT  22475 15738 1200 56 outx outy 
		;����: ��ȡĳ����ͼ�����ĳ���뾶��Χ�ڵ��������
		if @56 = 0
			NEW_LIFE_MON 203803 1 @outx @outy 0 0 0 0 10 0 0
			SET_EVENT_VAR 28 32 @outx 1
			SET_EVENT_VAR 32 32 @outy 1
		endif
		
		SET_EVENT_VAR 8 32 1 1
	else
		mon_speech @npcmid 0 2 1 ����Ҿ����������һ������������ʼ��#004#004#004#004
		SET_EVENT_VAR 8 32 0 1
		;�ۻ���ͬʱ���Ե��������¸�������ʼǰ��Ӧ����0
		SET_EVENT_VAR 20 32 0 1
	endif
endproc

proc 310 
	GET_MAP_INST 4403 10
	strvar npcmid
	GET_MAP_NPC_ID  304125 npcmid 1
	
	strvar Loc2 Loc3
	GET_EVENT_VAR 8 32 1 Loc2
	if @Loc2 = 0
		mon_speech @npcmid 0 2 1 #G����#n����ʼ��#056#056#056#056
		
		strvar outx outy
		GET_MAP_INST 4403 10
		;��������22475 15738   ��12��֮�ڣ�ˢ��־��
		GET_MAP_RANDPOINT  22475 15738 1200 56 outx outy 
		;����: ��ȡĳ����ͼ�����ĳ���뾶��Χ�ڵ��������
		if @56 = 0
			NEW_LIFE_MON 203803 1 @outx @outy 0 0 0 0 10 0 0
			SET_EVENT_VAR 28 32 @outx 1
			SET_EVENT_VAR 32 32 @outy 1
		endif
		
		SET_EVENT_VAR 8 32 1 1
	else
		mon_speech @npcmid 0 2 1 �����Ρ���������һ������������ʼ��#056#056#056#056
		SET_EVENT_VAR 8 32 0 1
		;�ۻ���ͬʱ���Ե��������¸�������ʼǰ��Ӧ����0
		SET_EVENT_VAR 20 32 0 1
	endif
endproc

proc 311 
	GET_MAP_INST 4403 10
	strvar npcmid
	GET_MAP_NPC_ID  304125 npcmid 1
	
	strvar Loc2 Loc3
	GET_EVENT_VAR 8 32 1 Loc2
	if @Loc2 = 0
		mon_speech @npcmid 0 2 1 #G����#n����ʼ��#022#022#022#022
		
		strvar outx outy
		GET_MAP_INST 4403 10
		;��������22475 15738   ��12��֮�ڣ�ˢ��־��
		GET_MAP_RANDPOINT  22475 15738 1200 56 outx outy 
		;����: ��ȡĳ����ͼ�����ĳ���뾶��Χ�ڵ��������
		if @56 = 0
			NEW_LIFE_MON 203803 1 @outx @outy 0 0 0 0 10 0 0
			SET_EVENT_VAR 28 32 @outx 1
			SET_EVENT_VAR 32 32 @outy 1
		endif
		
		SET_EVENT_VAR 8 32 1 1
	else
		mon_speech @npcmid 0 2 1 �����֡���������һ������������ʼ��#022#022#022#022
		SET_EVENT_VAR 8 32 0 1
		;�ۻ���ͬʱ���Ե��������¸�������ʼǰ��Ӧ����0
		SET_EVENT_VAR 20 32 0 1
	endif
endproc

proc 312 
	GET_MAP_INST 4403 10
	strvar npcmid
	GET_MAP_NPC_ID  304125 npcmid 1
	
	strvar Loc2
	GET_EVENT_VAR 8 32 1 Loc2
	if @Loc2 = 0
		mon_speech @npcmid 0 2 1 #G��˼#n����ʼ��#002#002#002#002
		
		strvar outx outy
		GET_MAP_INST 4403 10
		;��������22475 15738   ��12��֮�ڣ�ˢ��־��
		GET_MAP_RANDPOINT  22475 15738 1200 56 outx outy 
		;����: ��ȡĳ����ͼ�����ĳ���뾶��Χ�ڵ��������
		if @56 = 0
			NEW_LIFE_MON 203803 1 @outx @outy 0 0 0 0 10 0 0
			SET_EVENT_VAR 28 32 @outx 1
			SET_EVENT_VAR 32 32 @outy 1
		endif
		
		SET_EVENT_VAR 8 32 1 1
	else
		mon_speech @npcmid 0 2 1 ����˼����������һ������������ʼ��#002#002#002#002
		SET_EVENT_VAR 8 32 0 1
		;�ۻ���ͬʱ���Ե��������¸�������ʼǰ��Ӧ����0
		SET_EVENT_VAR 20 32 0 1
	endif
endproc

proc 313 
	GET_MAP_INST 4403 10
	strvar npcmid
	GET_MAP_NPC_ID  304125 npcmid 1
	
	strvar Loc2
	GET_EVENT_VAR 8 32 1 Loc2
	if @Loc2 = 0
		mon_speech @npcmid 0 2 1 #Gʹ��#n����ʼ��#012#012#012#012
		
		strvar outx outy
		GET_MAP_INST 4403 10
		;��������22475 15738   ��12��֮�ڣ�ˢ��־��
		GET_MAP_RANDPOINT  22475 15738 1200 56 outx outy 
		;����: ��ȡĳ����ͼ�����ĳ���뾶��Χ�ڵ��������
		if @56 = 0
			NEW_LIFE_MON 203803 1 @outx @outy 0 0 0 0 10 0 0
			SET_EVENT_VAR 28 32 @outx 1
			SET_EVENT_VAR 32 32 @outy 1
		endif
		
		SET_EVENT_VAR 8 32 1 1
	else
		mon_speech @npcmid 0 2 1 ��ʹ�ࡱ��������һ������������ʼ��#012#012#012#012
		SET_EVENT_VAR 8 32 0 1
		;�ۻ���ͬʱ���Ե��������¸�������ʼǰ��Ӧ����0
		SET_EVENT_VAR 20 32 0 1
	endif
endproc

;�����ж�
proc 5
	GET_EVENT_STAT 10 96
	if @10 = 0
	;�δ��ʼ
		return
	endif
	
	strvar roletime begintime tmptime
	GET_ROLE_VAR 52 32 1 roletime
	
	;�¼���ʼ��ʱ��
	GET_EVENT_VAR 24 32 1 begintime
	var tmptime @begintime
	;15����
	add tmptime 900
	if @roletime < @begintime or @roletime > @tmptime
		set_role_var 52 32 @begintime 1
		;��Ϊ0
		set_role_var 51 8 0 1
		;56λ�����¼�ý�ɫ�ܹ����ԵĴ���
		set_role_var 56 8 0 1
		;57λ����ý�ɫ������25�ζ�����������Ƿ�ȥ����������ȡ������1��ʾ�����죬0����
		;set_role_var 57 8 0 1
	endif
	
	GET_EVENT_VAR 4 32 1 26
	;0�Ļ�����ʾ��û�䲼��������
	if @26 = 0
		return
	endif
	
	;��ǰ����
	GET_EVENT_VAR 16 32 1 30

	strvar Ret
	;��ɫ��¼����
	get_role_var 51 8 Ret 31
	if @Ret < 0
		return
	endif
	
	if @30 = @31
		return
	endif
	
	GET_EVENT_VAR 28 32 1 87
	GET_EVENT_VAR 32 32 1 88
	strvar x y m
	;��ȡ�������
    GET_PLAYER_MAPINFO 1 0 0 x y
	;����������3���ڶ������
	GET_POINT_DIST @x @y @87 @88 m
	if m > 300
		add_sys_msg_start
		add_sys_msg ֻ������ȷ��ʱ�䣨�����Ŀ����ȷ�ĵص㣨þ������£�������ȷ�Ķ������н���Ӵ��
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	
	strvar MotIon Loc 
	ID_PLAYER_MOTION 0 0 MotIon          

	GET_EVENT_VAR 12 32 1 Loc
	if @Loc = 1
		if MotIon = 10701
		;����		
			;call 402
			
			;��ǰ����
			GET_EVENT_VAR 16 32 1 40
			if @40 < 0 or @40 > 26
				return
			else
			;�����ظ�������ǰ����
				set_role_var 51 8 @40 1
			endif
			
			;��¼����ҽ�ɫ���ԵĴ���
			strvar rrRet
			get_role_var 56 8 rrRet 99
			if @rrRet = 0 
				add 99 1
				set_role_var 56 8 @99 1
				if @99 >= 25
					;����25�Σ�����Ϊ1����ʾ������ȡ��Ʒ
					set_role_var 57 8 1 1
					add_sys_msg_start
					add_sys_msg �������ɫ�ı��ֻ��������ƺţ��뾡��ǰ����������ȡ
					set_sys_msg_mode 3 0 0
					msg_info
				endif
			endif
			
			call 401
		else
			;��ǰ����
			GET_EVENT_VAR 16 32 1 40
			if @40 < 0 or @40 > 26
				return
			else
			;�����ظ�������ǰ����
				set_role_var 51 8 @40 1
			endif
			
			call 400
		endif
	endif
	
	if @Loc = 2
		if MotIon = 10702
		;����
			;call 402
			
			;��ǰ����
			GET_EVENT_VAR 16 32 1 40
			if @40 < 0 or @40 > 26
				return
			else
			;�����ظ�������ǰ����
				set_role_var 51 8 @40 1
			endif
			
			;��¼����ҽ�ɫ���ԵĴ���
			strvar rrRet
			get_role_var 56 8 rrRet 99
			if @rrRet = 0 
				add 99 1
				set_role_var 56 8 @99 1
				if @99 >= 25
					;����25�Σ�����Ϊ1����ʾ������ȡ��Ʒ
					set_role_var 57 8 1 1
					add_sys_msg_start
					add_sys_msg �������ɫ�ı��ֻ��������ƺţ��뾡��ǰ����������ȡ
					set_sys_msg_mode 3 0 0
					msg_info
				endif
			endif
			
			call 401
		else	
			;��ǰ����
			GET_EVENT_VAR 16 32 1 40
			if @40 < 0 or @40 > 26
				return
			else
			;�����ظ�������ǰ����
				set_role_var 51 8 @40 1
			endif
			
			call 400
		endif
	endif
	
	if @Loc = 3
		if MotIon = 10703
		;����
			;call 402
			
			;��ǰ����
			GET_EVENT_VAR 16 32 1 40
			if @40 < 0 or @40 > 26
				return
			else
			;�����ظ�������ǰ����
				set_role_var 51 8 @40 1
			endif
			
			;��¼����ҽ�ɫ���ԵĴ���
			strvar rrRet
			get_role_var 56 8 rrRet 99
			if @rrRet = 0 
				add 99 1
				set_role_var 56 8 @99 1
				if @99 >= 25
					;����25�Σ�����Ϊ1����ʾ������ȡ��Ʒ
					set_role_var 57 8 1 1
					add_sys_msg_start
					add_sys_msg �������ɫ�ı��ֻ��������ƺţ��뾡��ǰ����������ȡ
					set_sys_msg_mode 3 0 0
					msg_info
				endif
			endif
			
			call 401
		else
			;��ǰ����
			GET_EVENT_VAR 16 32 1 40
			if @40 < 0 or @40 > 26
				return
			else
			;�����ظ�������ǰ����
				set_role_var 51 8 @40 1
			endif
			
			call 400
		endif
	endif
	
	if @Loc = 4
		if MotIon = 10704
		;װ��
			;call 402
			
			;��ǰ����
			GET_EVENT_VAR 16 32 1 40
			if @40 < 0 or @40 > 26
				return
			else
			;�����ظ�������ǰ����
				set_role_var 51 8 @40 1
			endif
			
			;��¼����ҽ�ɫ���ԵĴ���
			strvar rrRet
			get_role_var 56 8 rrRet 99
			if @rrRet = 0 
				add 99 1
				set_role_var 56 8 @99 1
				if @99 >= 25
					;����25�Σ�����Ϊ1����ʾ������ȡ��Ʒ
					set_role_var 57 8 1 1
					add_sys_msg_start
					add_sys_msg �������ɫ�ı��ֻ��������ƺţ��뾡��ǰ����������ȡ
					set_sys_msg_mode 3 0 0
					msg_info
				endif
			endif
			
			call 401
		else
			;��ǰ����
			GET_EVENT_VAR 16 32 1 40
			if @40 < 0 or @40 > 26
				return
			else
			;�����ظ�������ǰ����
				set_role_var 51 8 @40 1
			endif
			
			call 400
		endif
	endif
	
	if @Loc = 5
		if MotIon = 10705
		;����
			;call 402
			
			;��ǰ����
			GET_EVENT_VAR 16 32 1 40
			if @40 < 0 or @40 > 26
				return
			else
			;�����ظ�������ǰ����
				set_role_var 51 8 @40 1
			endif
			
			;��¼����ҽ�ɫ���ԵĴ���
			strvar rrRet
			get_role_var 56 8 rrRet 99
			if @rrRet = 0 
				add 99 1
				set_role_var 56 8 @99 1
				if @99 >= 25
					;����25�Σ�����Ϊ1����ʾ������ȡ��Ʒ
					set_role_var 57 8 1 1
					add_sys_msg_start
					add_sys_msg �������ɫ�ı��ֻ��������ƺţ��뾡��ǰ����������ȡ
					set_sys_msg_mode 3 0 0
					msg_info
				endif
			endif
			
			call 401
		else
			;��ǰ����
			GET_EVENT_VAR 16 32 1 40
			if @40 < 0 or @40 > 26
				return
			else
			;�����ظ�������ǰ����
				set_role_var 51 8 @40 1
			endif
			
			call 400
		endif
	endif
	
	if @Loc = 6
		if MotIon = 10706
		;����
			;call 402
			
			;��ǰ����
			GET_EVENT_VAR 16 32 1 40
			if @40 < 0 or @40 > 26
				return
			else
			;�����ظ�������ǰ����
				set_role_var 51 8 @40 1
			endif
			
			;��¼����ҽ�ɫ���ԵĴ���
			strvar rrRet
			get_role_var 56 8 rrRet 99
			if @rrRet = 0 
				add 99 1
				set_role_var 56 8 @99 1
				if @99 >= 25
					;����25�Σ�����Ϊ1����ʾ������ȡ��Ʒ
					set_role_var 57 8 1 1
					add_sys_msg_start
					add_sys_msg �������ɫ�ı��ֻ��������ƺţ��뾡��ǰ����������ȡ
					set_sys_msg_mode 3 0 0
					msg_info
				endif
			endif
			
			call 401
		else
			;��ǰ����
			GET_EVENT_VAR 16 32 1 40
			if @40 < 0 or @40 > 26
				return
			else
			;�����ظ�������ǰ����
				set_role_var 51 8 @40 1
			endif
			
			call 400
		endif
	endif
	
	if @Loc = 7
		if MotIon = 10707
		;ˣ��
			;call 402
			
			;��ǰ����
			GET_EVENT_VAR 16 32 1 40
			if @40 < 0 or @40 > 26
				return
			else
			;�����ظ�������ǰ����
				set_role_var 51 8 @40 1
			endif
			
			;��¼����ҽ�ɫ���ԵĴ���
			strvar rrRet
			get_role_var 56 8 rrRet 99
			if @rrRet = 0 
				add 99 1
				set_role_var 56 8 @99 1
				if @99 >= 25
					;����25�Σ�����Ϊ1����ʾ������ȡ��Ʒ
					set_role_var 57 8 1 1
					add_sys_msg_start
					add_sys_msg �������ɫ�ı��ֻ��������ƺţ��뾡��ǰ����������ȡ
					set_sys_msg_mode 3 0 0
					msg_info
				endif
			endif
			
			call 401
		else
			;��ǰ����
			GET_EVENT_VAR 16 32 1 40
			if @40 < 0 or @40 > 26
				return
			else
			;�����ظ�������ǰ����
				set_role_var 51 8 @40 1
			endif
			
			call 400
		endif
	endif
	
	if @Loc = 8
		if MotIon = 10708
		;����
			;call 402
			
			;��ǰ����
			GET_EVENT_VAR 16 32 1 40
			if @40 < 0 or @40 > 26
				return
			else
			;�����ظ�������ǰ����
				set_role_var 51 8 @40 1
			endif
			
			;��¼����ҽ�ɫ���ԵĴ���
			strvar rrRet
			get_role_var 56 8 rrRet 99
			if @rrRet = 0 
				add 99 1
				set_role_var 56 8 @99 1
				if @99 >= 25
					;����25�Σ�����Ϊ1����ʾ������ȡ��Ʒ
					set_role_var 57 8 1 1
					add_sys_msg_start
					add_sys_msg �������ɫ�ı��ֻ��������ƺţ��뾡��ǰ����������ȡ
					set_sys_msg_mode 3 0 0
					msg_info
				endif
			endif
			
			call 401
		else
			;��ǰ����
			GET_EVENT_VAR 16 32 1 40
			if @40 < 0 or @40 > 26
				return
			else
			;�����ظ�������ǰ����
				set_role_var 51 8 @40 1
			endif
			
			call 400
		endif
	endif
		
	if @Loc = 9
		if MotIon = 10709
		;��Ҿ
			;call 402
			
			;��ǰ����
			GET_EVENT_VAR 16 32 1 40
			if @40 < 0 or @40 > 26
				return
			else
			;�����ظ�������ǰ����
				set_role_var 51 8 @40 1
			endif
			
			;��¼����ҽ�ɫ���ԵĴ���
			strvar rrRet
			get_role_var 56 8 rrRet 99
			if @rrRet = 0 
				add 99 1
				set_role_var 56 8 @99 1
				if @99 >= 25
					;����25�Σ�����Ϊ1����ʾ������ȡ��Ʒ
					set_role_var 57 8 1 1
					add_sys_msg_start
					add_sys_msg �������ɫ�ı��ֻ��������ƺţ��뾡��ǰ����������ȡ
					set_sys_msg_mode 3 0 0
					msg_info
				endif
			endif
			
			call 401
		else
			;��ǰ����
			GET_EVENT_VAR 16 32 1 40
			if @40 < 0 or @40 > 26
				return
			else
			;�����ظ�������ǰ����
				set_role_var 51 8 @40 1
			endif
			
			call 400
		endif
	endif
	
	if @Loc = 10
		if MotIon = 10710
		;����
			;call 402
			
			;��ǰ����
			GET_EVENT_VAR 16 32 1 40
			if @40 < 0 or @40 > 26
				return
			else
			;�����ظ�������ǰ����
				set_role_var 51 8 @40 1
			endif
			
			;��¼����ҽ�ɫ���ԵĴ���
			strvar rrRet
			get_role_var 56 8 rrRet 99
			if @rrRet = 0 
				add 99 1
				set_role_var 56 8 @99 1
				if @99 >= 25
					;����25�Σ�����Ϊ1����ʾ������ȡ��Ʒ
					set_role_var 57 8 1 1
					add_sys_msg_start
					add_sys_msg �������ɫ�ı��ֻ��������ƺţ��뾡��ǰ����������ȡ
					set_sys_msg_mode 3 0 0
					msg_info
				endif
			endif
			
			call 401
		else
			;��ǰ����
			GET_EVENT_VAR 16 32 1 40
			if @40 < 0 or @40 > 26
				return
			else
			;�����ظ�������ǰ����
				set_role_var 51 8 @40 1
			endif
			
			call 400
		endif
	endif
	
	if @Loc = 11
		if MotIon = 10711
		;����
			;call 402
			
			;��ǰ����
			GET_EVENT_VAR 16 32 1 40
			if @40 < 0 or @40 > 26
				return
			else
			;�����ظ�������ǰ����
				set_role_var 51 8 @40 1
			endif
			
			;��¼����ҽ�ɫ���ԵĴ���
			strvar rrRet
			get_role_var 56 8 rrRet 99
			if @rrRet = 0 
				add 99 1
				set_role_var 56 8 @99 1
				if @99 >= 25
					;����25�Σ�����Ϊ1����ʾ������ȡ��Ʒ
					set_role_var 57 8 1 1
					add_sys_msg_start
					add_sys_msg �������ɫ�ı��ֻ��������ƺţ��뾡��ǰ����������ȡ
					set_sys_msg_mode 3 0 0
					msg_info
				endif
			endif
			
			call 401
		else	
			;��ǰ����
			GET_EVENT_VAR 16 32 1 40
			if @40 < 0 or @40 > 26
				return
			else
			;�����ظ�������ǰ����
				set_role_var 51 8 @40 1
			endif
			
			call 400
		endif
	endif
	
	if @Loc = 12
		if MotIon = 10712
		;��˼
			;call 402
			
			;��ǰ����
			GET_EVENT_VAR 16 32 1 40
			if @40 < 0 or @40 > 26
				return
			else
			;�����ظ�������ǰ����
				set_role_var 51 8 @40 1
			endif
			
			;��¼����ҽ�ɫ���ԵĴ���
			strvar rrRet
			get_role_var 56 8 rrRet 99
			if @rrRet = 0 
				add 99 1
				set_role_var 56 8 @99 1
				if @99 >= 25
					;����25�Σ�����Ϊ1����ʾ������ȡ��Ʒ
					set_role_var 57 8 1 1
					add_sys_msg_start
					add_sys_msg �������ɫ�ı��ֻ��������ƺţ��뾡��ǰ����������ȡ
					set_sys_msg_mode 3 0 0
					msg_info
				endif
			endif
			
			call 401
		else
			;��ǰ����
			GET_EVENT_VAR 16 32 1 40
			if @40 < 0 or @40 > 26
				return
			else
			;�����ظ�������ǰ����
				set_role_var 51 8 @40 1
			endif
			
			call 400
		endif
	endif
	
	if @Loc = 13
		if MotIon = 10713
		;ʹ��
			;call 402
			
			;��ǰ����
			GET_EVENT_VAR 16 32 1 40
			if @40 < 0 or @40 > 26
				return
			else
			;�����ظ�������ǰ����
				set_role_var 51 8 @40 1
			endif
			
			;��¼����ҽ�ɫ���ԵĴ���
			strvar rrRet
			get_role_var 56 8 rrRet 99
			if @rrRet = 0 
				add 99 1
				set_role_var 56 8 @99 1
				if @99 >= 25
					;����25�Σ�����Ϊ1����ʾ������ȡ��Ʒ
					set_role_var 57 8 1 1
					add_sys_msg_start
					add_sys_msg �������ɫ�ı��ֻ��������ƺţ��뾡��ǰ����������ȡ
					set_sys_msg_mode 3 0 0
					msg_info
				endif
			endif
			
			call 401
		else
			;��ǰ����
			GET_EVENT_VAR 16 32 1 40
			if @40 < 0 or @40 > 26
				return
			else
			;�����ظ�������ǰ����
				set_role_var 51 8 @40 1
			endif
			
			call 400
		endif
	endif
	
	if @Loc = 14
		if MotIon = 10700
		;����			
			;call 402
			
			;��ǰ����
			GET_EVENT_VAR 16 32 1 40
			if @40 < 0 or @40 > 26
				return
			else
			;�����ظ�������ǰ����
				set_role_var 51 8 @40 1
			endif
			
			;��¼����ҽ�ɫ���ԵĴ���
			strvar rrRet
			get_role_var 56 8 rrRet 99
			if @rrRet = 0 
				add 99 1
				set_role_var 56 8 @99 1
				if @99 >= 25
					;����25�Σ�����Ϊ1����ʾ������ȡ��Ʒ
					set_role_var 57 8 1 1
					add_sys_msg_start
					add_sys_msg �������ɫ�ı��ֻ��������ƺţ��뾡��ǰ����������ȡ
					set_sys_msg_mode 3 0 0
					msg_info
				endif
			endif
			
			call 401
		else	
			;��ǰ����
			GET_EVENT_VAR 16 32 1 40
			if @40 < 0 or @40 > 26
				return
			else
			;�����ظ�������ǰ����
				set_role_var 51 8 @40 1
			endif
			
			call 400
		endif
	endif
endproc

proc 400
	add_sys_msg_start
	add_sys_msg ����û��һ�ֺ�����Ⱥ�ĸо��أ�ͬѧ����Ķ���������Ӵ��
	set_sys_msg_mode 3 0 0
	msg_info
	
	strvar aflag
	GET_PLAYER_ID 1 aflag
	if @aflag = -1
		return
	else
		;��Ŀ�����״̬
		TARGET_ADD_STATUS 1 @aflag 1057 1 2 90
	endif
endproc


;ÿ����һ�Σ���5060050 
proc 401
	add_sys_msg_start
	add_sys_msg �ǳ����������⶯�����Ե����Ƿ���
	set_sys_msg_mode 3 0 0
	msg_info
	
	strvar ret1
	GET_PACK_SPACE 0 ret1
	if @ret1 = 0
		add_sys_msg_start
		add_sys_msg ���ı����Ѿ�װ���¸��ණ����
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	
	rand 55 100
	if @55 <= 77 and @55 >= 0
		ADD_BIND_ITEM_NUM_INPAK 5060050 1 1 14 
	else
		ADD_BIND_ITEM_NUM_INPAK 5060050 10 1 14 
		add_sys_msg_start
		add_sys_msg  $_player$�����⺣̲������һ���˶��Ĺ����б��ֳ�ɫ�������Ů���������
		set_sys_msg_mode 18 0 0
		SVR_MSG_INFO 
	endif
endproc

proc 402
	strvar x1 y1 PeoNum
	var PeoNum 2
	rand 15 8
	switch 15
		case 0
			var x1 14509
			var y1 29289
		endcase

		case 1
			var x1 15078
			var y1 29395
		endcase

		case 2
			var x1 15482
			var y1 29055
		endcase
				
		case 3
			var x1 15625
			var y1 28610
		endcase
				
		case 4
			var x1 15413
			var y1 28247
		endcase
		
		case 5
			var x1 14995
			var y1 28114
		endcase
		
		case 6
			var x1 14562
			var y1 28285
		endcase
		
		case 7
			var x1 14285
			var y1 28742
		endcase
	endswitch
	
	GET_EVENT_VAR 20 32 1 60
	add 60 1
	SET_EVENT_VAR 20 32 @60 1
	
	if @60 >= @PeoNum
		NEW_LIFE_MON 293524 10 @x1 @y1 800 0 0 0 20 0 0
		add_sys_msg_start
		add_sys_msg Ϊ�˸�л��ҵĻ�����ϣ�������������Ů��������������г������˽�ƷӴ��
		set_sys_msg_mode 4 0 0
		SVR_MSG_INFO
	endif
endproc
