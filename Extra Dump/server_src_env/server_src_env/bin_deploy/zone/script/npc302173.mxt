;npc �ſ�

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b�ڴ�����ѽ�֮�ʣ�����ʲô�ܹ����±����ܴ���������أ���Ͷ֮���±����ұ�֮�����ƣ�����ʲô��������������أ�
	add_talk #b#b
	add_talk #b#b#c186506������ʾ����ڼ䣬��ÿ������±���ʹ����ȡ�±���С�#n
		
	strvar res
	GET_TASK_VAR 4631 4 32 12 res
	
	strvar er1 er2 er3 er4
	TASK_STAT 4631 er1 er2
	if @er1 = 0 and @er2 = 2
		if @res = 0 and @12 = 0
			option 206 ��ʼƴ��
		else
			option 200 #�����±�����������һ�Σ�
		endif
	endif
	
	task_stat 4632 er3 er4
	;δ������
	if @er3 = 0 and @er4 = 0 and @er1 = 0 and @er2 = 0
		GET_PLAYER_BASE_ATT 13 0 19 0
		if @13 = 0 and @19 >= 40
			option 200 #�����±�
		endif
	endif
		
	strvar num
	TASK_STAT 4632 13 14
	if @13 = 0 and @14 = 2
		rand num 14
		switch num
			case 0
				option 400 #��ʼ��ʫ
			endcase
			case 1
				option 401 #��ʼ��ʫ
			endcase
			case 2
				option 402 #��ʼ��ʫ
			endcase
			case 3
				option 403 #��ʼ��ʫ
			endcase
			case 4
				option 404 #��ʼ��ʫ
			endcase
			case 5
				option 405 #��ʼ��ʫ
			endcase
			case 6
				option 406 #��ʼ��ʫ
			endcase
			case 7
				option 407 #��ʼ��ʫ
			endcase
			case 8
				option 408 #��ʼ��ʫ
			endcase
			case 9
				option 409 #��ʼ��ʫ
			endcase
			case 10
				option 410 #��ʼ��ʫ
			endcase
			case 11
				option 411 #��ʼ��ʫ
			endcase
			case 12
				option 412 #��ʼ��ʫ
			endcase
			case 13
				option 413 #��ʼ��ʫ
			endcase
			case 14
				option 414 #��ʼ��ʫ
			endcase
		endswitch
	endif
	
		task_stat 4631 60 70
		if @60 = 0 and @70 = 1
			TASK_ADD_OPTION 4631 0 305
		endif
		
		task_stat 4632 80 90
		if @80 = 0 and @90 = 1
			TASK_ADD_OPTION 4632 0 306
		endif
		
	talk
endproc 

proc 305
	TASK_DO_OPTION 4631 0
endproc

proc 306
	TASK_DO_OPTION 4632 0
endproc

proc 200	
	dialog 3
	add_talk $_npc$:
	add_talk #b#b�ܸ�����Ը�����ҷ��������ζ���±����Ҳ���ʳŶ�������ζ���±������ԣ��������Ҳ������㽫�����±������ң���Ϊ�����±�������һ�Ųر�ͼ����������������ı�ʯ��Ƭ�����ҡ�
	add_talk #b#b
	add_talk #b#b����Ҫ�ύ����Ʒ�����·��Ŀ���
	
	NPC_COMMIT_OPT 300
	talk
endproc

proc 300
	strvar ret1
	GET_PACK_SPACE 0 ret1
	if @ret1 = 0
		add_sys_msg_start
		add_sys_msg �����ռ䲻�㣬�޷���ȡ
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	
	strvar itemId pos index ret ret2
	strvar iflag
	var iflag 0
	GET_COMMIT_ITEM_INFO 1 itemId
	if itemId = -1
		add_sys_msg_start
		add_sys_msg ����û���ύ��Ʒ
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	
	if @itemId = 5030109 or @itemId = 5012130 or @itemId = 5012131 or @itemId = 5012132
		var iflag 1
	endif
	
	if @itemId = 5012134 or @itemId = 5012135 or @itemId = 5012133
		var iflag 1
	endif
	
	if @iflag = 0
		add_sys_msg_start
		add_sys_msg ����ѽڲ�����ֻ����Լ�����ζ�±�����
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	
	GET_COMMIT_ITEM_INFO 2 pos
	GET_COMMIT_ITEM_INFO 3 index

	DEC_ITEM_NUM_BYIDX @itemId 1 @pos @index ret
	if ret = 0		
		TASK_STAT 4631 15 70
		if @70 = 2 and @15 = 0
			strvar res
			GET_TASK_VAR 4631 4 32 12 res
			if @res = 1 and @12 = 0
				SET_TASK_VAR 4631 4 32 0 16
			endif
			;����ǳ���Ҫ����Ϊ�ύ�Ի����ύ��ϡ��ֻ������DIALOG_STAT_COMMIT_OPT��ֵ��ȥ��
			;�����stSEnv.pstPlayer->stOnline.cDialogStatOld = DIALOG_STAT_OPT;  Ϊ3ǿ�Ƹ�ֵ
			SET_DLG_OLD_STAT 3
			call 1
		elseif @70 = 0 and @15 = 0
			add_sys_msg_start
			add_sys_msg �ſ��ѽ�������������ı�����
			set_sys_msg_mode 3 0 0
			msg_info
			
			strvar rd
			rand rd 100
			if @rd >= 0 and @rd < 50
				ADD_BIND_ITEM_NUM_INPAK 5030111 1 1 13
				strvar npcmid
				GET_MAP_NPC_ID  302173 npcmid 1
				MON_SPEECH  @npcmid 0 0 1 ���������±����һ�����ʫǩ��ϣ�������ܹ���ң���һ����
			else
				ADD_BIND_ITEM_NUM_INPAK 5030112 1 1 23
				strvar npcmid
				GET_MAP_NPC_ID  302173 npcmid 1
				MON_SPEECH  @npcmid 0 0 1 ���������±����һ����Ծ��ף�ϣ�������ܹ���ң���һ����
			endif
			SET_DLG_OLD_STAT 3
			call 1
		else
		endif
	endif
endproc

;ƴ��
;4631 ����ID
proc 100
	add_sys_msg_start
	add_sys_msg ���Ѿ�������
	set_sys_msg_mode 4 0 0
	msg_info
	
	;֪ͨ�ͻ��˽�����Ļģ��
	pinjiu_screen_vague 1
	
	PLAYER_CLOSE_DIALOG1 0 1
	
	strvar npcmid
	GET_MAP_NPC_ID  302173 npcmid 1
	MON_SPEECH  @npcmid 0 2 1 С��������ƴ�ƣ���Ű�أ�
	
	SET_TASK_VAR 4631 0 32 60 10
	SET_TASK_VAR 4631 4 32 1 16
	SET_TASK_VAR 4631 8 32 0 26
	SET_TASK_VAR 4631 12 32 0 27
	SET_TASK_VAR 4631 16 32 0 28
endproc

;���Ӯ��
proc 101
	add_sys_msg_start
	add_sys_msg $_npc$�Ѿ���Ĳ�ʡ�����ˣ���Ӯ�ˣ�
	set_sys_msg_mode 4 0 0
	msg_info
	
	task_fini 4631 1
endproc

;�������
proc 102
	add_sys_msg_start
	add_sys_msg $_npc$���ǳ�������ǧ�������أ���źȵ��İ���������̫С�������ˣ�
	set_sys_msg_mode 4 0 0
	msg_info
	
	SET_TASK_VAR 4631 0 32 60 10
	SET_TASK_VAR 4631 4 32 1 16
	SET_TASK_VAR 4631 8 32 0 26
	SET_TASK_VAR 4631 12 32 0 27
	SET_TASK_VAR 4631 16 32 0 28
	
	PLAYER_CLOSE_DIALOG1 0 1
endproc

;����һ��
proc 201
	;ÿ�ε��������һ����ֻ�� x% ��䣬ÿ�ε���ֵ��� 1-12�������60%��
	rand 20 12
	add 20 1
	
	GET_TASK_VAR 4631 0 32 11 num
	add num @20
	SET_TASK_VAR 4631 0 32 @num 10
	
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
	GET_TASK_VAR 4631 16 32 25 jiuliangval
	GET_TASK_VAR 4631 0 32 11 num	
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
	GET_TASK_VAR 4631 0 32 11 num
	if @num = 0
		SET_TASK_VAR 4631 0 32 60 10
		var num 60
	endif
	
	;idx 12 ��¼npc���������־   flag Ϊ 1ʱ����ʾ�Ѿ�ȡ��  ,0ʱ��������ȡ
	GET_TASK_VAR 4631 12 32 21 flag 
	if @flag = 0
		;npc����  90-97
		rand 14 8
		add 14 90
		
		if @14 < 95 and @14 >= 90
			;�ſ����������ߣ��������ƺ������׾ͻ����
			SET_TASK_VAR 4631 8 32 1 10
		elseif @14 >= 95 and @14 <= 97
			;�ſ����˴�����Խ��Խ�������������ƺ��ܹ�ǧ������
			SET_TASK_VAR 4631 8 32 2 10
		else
			SET_TASK_VAR 4631 8 32 0 10
		endif
		
		SET_TASK_VAR 4631 12 32 1 22
		;idx Ϊ 16 ���þ���ֵ
		SET_TASK_VAR 4631 16 32 @14 23
	endif
	
	
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#bһ�����¶ǣ����Ѿ���$@num$%�����⣬��Ҫ��������
	
	;idx 8 ����ʾ�ľ�   
	GET_TASK_VAR 4631 8 32 15 jiuliang
	if @jiuliang = 1
		add_talk #b#b�ſ����������ߣ��������ƺ������׾ͻ����
	elseif @jiuliang = 2
		add_talk #b#b�ſ����˴�����Խ��Խ�������������ƺ��ܹ�ǧ������
	else
	endif
	
	option 201 ����һ��
	option 202 #�����ˣ�����������ɣ�
	
	talk
endproc



proc 400
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b���¼�ʱ��
	add_talk #b#b������¾䣩
	
		option 500 #�Ѿ�������
		option 501 #�Ѿ�������
		option 501 #�Ѿ���Ů��
		option 501 #�Ѿ��ʻ���
		
	talk
endproc

proc 401
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b��Ը�˳���
	add_talk #b#b������¾䣩
	
		option 501 #���������
		option 500 #ǧ�ﹲ濾�
		option 501 #ǧ��žžž
		option 501 #����һ���� 
		
	talk
endproc

proc 402
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b����һ���ƣ�����������
	add_talk #b#b������¾䣩
	
		option 501 #������ɽ�����ƺ��뺣��
		option 501 #�¼Ȳ�������Ӱͽ������
		option 500 #�ٱ������£���Ӱ������
		option 501 #��ɽ�ڲ�ס���Ͼ�����ȥ
		
	talk
endproc

proc 403
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b���������£����Ĺ���ʱ
	add_talk #b#b������¾䣩
	
		option 501 #Ƭ���칲Զ����ҹ��ͬ��
		option 501 #���������������¾�¶��
		option 501 #�����ɼ��գ���Ȫʯ����
		option 500 #����Թңҹ����Ϧ����˼
		
	talk
endproc

proc 404
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b���³���ɽ����ã�ƺ���
	add_talk #b#b������¾䣩
	
		option 501 #ǧɽ��ɾ�����������
		option 501 #���°׵ǵ��������ຣ��
		option 500 #���缸����������Ź�
		option 501 #Ը��һ���ģ����ײ�����
		
	talk
endproc

proc 405
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b��ǰ���¹⣬���ǵ���˪
	add_talk #b#b������¾䣩
	
		option 501 #��ͷ�����£�����Ь��˫
		option 500 #��ͷ�����£���ͷ˼����
		option 501 #��ͷ�����£��������˫
		option 501 #��ͷ�����£���ͷ�ű���
		
	talk
endproc

proc 406
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#bĺ���վ����庮����������ת����
	add_talk #b#b������¾䣩
	
		option 500 #������ҹ�����ã���������δ���
		option 501 #����ɴ���׷�䣬ֻ�ǵ�ʱ���Ȼ
		option 501 #���ʱ�ѱ����ѣ����������ٻ�Ǯ
		option 501 #Ǽ�Ѻ���Թ����,���粻�����Ź�
		
	talk
endproc

proc 407
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b�������ʮ��ҹ������������԰��
	add_talk #b#b������¾䣩
	
		option 501 #�������ʮ��ҹ��ɢ�����Ū����
		option 500 #�������ʮ��ҹ������ɳͷˮ��ǰ
		option 501 #�������ʮ��ҹ��ֱ���Ʒ��ò׺�
		option 501 #�������ʮ��ҹ����ʱ����ʤ����
		
	talk
endproc

proc 408
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b��������δ��ǣ����ϼ��¼���Բ
	add_talk #b#b������¾䣩
	
		option 501 #�����Ҳı����ã�ǧ��ɢ��������
		option 501 #���ǰ׵۲��Ƽ䣬ǧ�ｭ��һ�ջ�
		option 500 #���һ�����˻ᣬ��ҹ���������
		option 501 #���̨�Ϸ���Σ���ȥ̨�ս�����
		
	talk
endproc

proc 409
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b��ͥ�ذ�����ѻ����¶����ʪ��
	add_talk #b#b������¾䣩
	
		option 501 #�������������ƣ����ͬ����ų�
		option 501 #��ҹ�ٱ������£���֪˭�˰��һ�
		option 501 #��Ů��֪�����ޣ������̳���ͥ��
		option 500 #��ҹ�����˾�������֪��˼��˭��
		
	talk
endproc

proc 410
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b������������
	add_talk #b#b������¾䣩
	
		option 501 #��Ů��Ů��Ů
		option 501 #˧��˧��˧��
		option 500 #����էԲ��ȱ
		option 501 #�����������
		
	talk
endproc

proc 411
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b¶�ӽ�ҹ��
	add_talk #b#b������¾䣩
	
		option 501 #���ǹ�����
		option 500 #���ǹ�����
		option 501 #����������
		option 501 #���ǹ�ʱ��
		
	talk
endproc

proc 412
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b����δ��Ⱥɽ�ߣ����ǧ�����׺�
	add_talk #b#b������¾䣩
	
		option 500 #һ��δ������ӿ�������ѻ������
		option 501 #˭Ϊ�칫ϴ���ӣ�Ӧ������ǧ��ˮ
		option 501 #�����俴�����ˣ�����տȻ�Ĳ���
		option 501 #����������������Ұ���������Ժ�
		
	talk
endproc

proc 413
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b�Ѿ�������
	add_talk #b#b������Ͼ䣩
	
		option 501 #���ż�ʱ��
		option 500 #���¼�ʱ��
		option 501 #���Ҽ�ʱ��
		option 501 #��Ʒ��ʱ��
		
	talk
endproc

proc 414
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b��������Բȱ
	add_talk #b#b������Ͼ䣩
	
		option 501 #Ů�л�������
		option 501 #���и߰����
		option 500 #���б������
		option 501 #�������ϲ���
		
	talk
endproc

proc 500
	TASK_FINI 4632 0
	
	strvar npcID
	GET_MAP_NPC_ID 302173 npcID 0
	mon_speech @npcID 2 0 1 ���˾�������Ҳ����������
	
endproc

proc 501
	;COMMIT_TASK 4632 0
	TASK_FINI 4632 0
	
	;add_sys_msg_start 
	;add_sys_msg δ�ܶԳ�ʫ�䣬�޷���ý�����
	;set_sys_msg_mode 4 0 0
	;msg_info
	
	strvar npcID
	GET_MAP_NPC_ID 302173 npcID 0
	mon_speech @npcID 2 0 1 ����̡���Ҳ���ǹ�����ֻ�ǡ�������Ҫ�ú����������ˣ�
endproc
