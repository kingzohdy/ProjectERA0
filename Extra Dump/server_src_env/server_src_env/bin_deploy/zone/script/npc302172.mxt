;npc �ʲ�

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b���￪���磬����������
	add_talk #b#b���ң�ֵأ�����Զ׺��
	add_talk #b#b�Ӻ�����������¥������
	add_talk #b#b����ǧ��Ц����ӳ��֦ǰ
	add_talk #b#b
	add_talk #b#b��ӭ���μ�����ƻᣬ���ջ����϶��ܼ򵥣�ֻҪ��΢��ĥһ�¾��ܹ��µ��𰸣���ʹ�Ҳ�Ҳ���ķ�֮һ�Ļ���¶ԣ�
	add_talk #b#b
	add_talk #b#b#c186506������ʾ����ڼ䣬��ÿ������±���ʹ����ȡ�±���С�#n
	
	;task_stat 4630 80 90
	;if @80 = 0 and @90 = 0
		GET_PLAYER_BASE_ATT 13 0 12 0
		if @13 = 0 and @12 >= 40
			;option 200 #�ύ�±�����õ���
			option 206 ���ж���ı�Ƥ�±�
		endif
	;endif
	
	strvar ret num
	task_stat 4630 1 ret
	if Ret = 2
		rand num 20
		switch num
			case 0
				option 400 ��ʼ����
			endcase
			case 1
				option 401 ��ʼ����
			endcase
			case 2
				option 402 ��ʼ����
			endcase
			case 3
				option 403 ��ʼ����
			endcase
			case 4
				option 404 ��ʼ����
			endcase
			case 5
				option 405 ��ʼ����
			endcase
			case 6
				option 406 ��ʼ����
			endcase
			case 7
				option 407 ��ʼ����
			endcase
			case 8
				option 408 ��ʼ����
			endcase
			case 9
				option 409 ��ʼ����
			endcase
			case 10
				option 410 ��ʼ����
			endcase
			case 11
				option 411 ��ʼ����
			endcase
			case 12
				option 412 ��ʼ����
			endcase
			case 13
				option 413 ��ʼ����
			endcase
			case 14
				option 414 ��ʼ����
			endcase
			case 15
				option 415 ��ʼ����
			endcase
			case 16
				option 416 ��ʼ����
			endcase
			case 17
				option 417 ��ʼ����
			endcase
			case 18
				option 418 ��ʼ����
			endcase
			case 19
				option 419 ��ʼ����
			endcase
			case 20
				option 420 ��ʼ����
			endcase
		endswitch
	endif
		
		task_stat 4630 60 70
		if @60 = 0 and @70 = 1
			TASK_ADD_OPTION 4630 0 201
		endif
	talk
endproc 

proc 201
	TASK_DO_OPTION 4630 0
endproc

proc 200
	dialog 3
	add_talk $_npc$:
	add_talk #b#b����ֻ����Ƥ�±���
	add_talk #b#b
	add_talk #b#b����Ҫ�ύ����Ʒ�����·��Ŀ���
		
	NPC_COMMIT_OPT 300
	talk
endproc

proc 300
	strvar itemId pos index ret 
	GET_COMMIT_ITEM_INFO 1 itemId
	if itemId = -1
		add_sys_msg_start
		add_sys_msg ����û���ύ��Ʒ
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	
	if @itemId != 5012131
		add_sys_msg_start
		add_sys_msg ��Ȼ�������±��ڶ࣬����ֻȡ��Ƥ�±���һư����
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	
	GET_COMMIT_ITEM_INFO 2 pos
	GET_COMMIT_ITEM_INFO 3 index

	DEC_ITEM_NUM_BYIDX @itemId 1 @pos @index ret
	if ret = 0
		ADD_BIND_ITEM_NUM_INPAK 5030110 1 1 13
		add_sys_msg_start
		add_sys_msg �ʲŽ�һ��д�ŵ��յ�Сֽ�������������С�
		set_sys_msg_mode 3 0 0
		msg_info
	endif
	PLAYER_CLOSE_DIALOG 0 0
endproc

proc 206
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b ��˵���ж���ı�Ƥ�±�������̫���ˣ���Ը����һ����������к���������±����óԵ������±������ֵĻ��±��������Ļ�ϲ�±�������һ�����ϲ���ģ��԰ɣ���Ҫʲô�Լ��ðɣ�
	
	GET_PLAYER_BASE_ATT 13 0 12 0
	if @13 = 0 and @12 >= 40
		option 202 ��Ƥ�±��������±�
		option 203 ��Ƥ�±��������±�
		option 204 ��Ƥ�±������±�
		option 205 ��Ƥ�±�����ϲ�±�
	endif
	
	talk
endproc

;��Ƥ�±��������±�
proc 202
	strvar ret1
	GET_PACK_SPACE 0 ret1
	if @ret1 = 0
		add_sys_msg_start
		add_sys_msg �����ռ䲻�㣬�޷���ȡ
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	
	GET_ITEM_NUM_INPAK 5012131 13 15
	if @13 = 0 and @15 >= 1
		DEC_ITEM_NUM_INPAK 5012131 1 14
		if @14 = 0
			ADD_BIND_ITEM_NUM_INPAK 5012130 1 1 13
		endif
	else
		add_sys_msg_start
		add_sys_msg ������û�б�Ƥ�±���
		set_sys_msg_mode 3 0 0
		msg_info
	endif
	call 206
endproc

;��Ƥ�±��������±�
proc 203
	strvar ret1
	GET_PACK_SPACE 0 ret1
	if @ret1 = 0
		add_sys_msg_start
		add_sys_msg �����ռ䲻�㣬�޷���ȡ
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	
	GET_ITEM_NUM_INPAK 5012131 13 15
	if @13 = 0 and @15 >= 1
		DEC_ITEM_NUM_INPAK 5012131 1 14
		if @14 = 0
			ADD_BIND_ITEM_NUM_INPAK 5012132 1 1 13
		endif
	else
		add_sys_msg_start
		add_sys_msg ������û�б�Ƥ�±���
		set_sys_msg_mode 3 0 0
		msg_info
	endif
	call 206
endproc

;��Ƥ�±������±�
proc 204
	strvar ret1
	GET_PACK_SPACE 0 ret1
	if @ret1 = 0
		add_sys_msg_start
		add_sys_msg �����ռ䲻�㣬�޷���ȡ
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	
	GET_ITEM_NUM_INPAK 5012131 13 15
	if @13 = 0 and @15 >= 1
		DEC_ITEM_NUM_INPAK 5012131 1 14
		if @14 = 0
			ADD_BIND_ITEM_NUM_INPAK 5012134 1 1 13
		endif
	else
		add_sys_msg_start
		add_sys_msg ������û�б�Ƥ�±���
		set_sys_msg_mode 3 0 0
		msg_info
	endif
	call 206
endproc

;��Ƥ�±�����ϲ�±�
proc 205
	strvar ret1
	GET_PACK_SPACE 0 ret1
	if @ret1 = 0
		add_sys_msg_start
		add_sys_msg �����ռ䲻�㣬�޷���ȡ
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	
	GET_ITEM_NUM_INPAK 5012131 13 15
	if @13 = 0 and @15 >= 1
		DEC_ITEM_NUM_INPAK 5012131 1 14
		if @14 = 0
			ADD_BIND_ITEM_NUM_INPAK 5012135 1 1 13
		endif
	else
		add_sys_msg_start
		add_sys_msg ������û�б�Ƥ�±���
		set_sys_msg_mode 3 0 0
		msg_info
	endif
	call 206
endproc

proc 400
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b�϶��·�
	add_talk #b#b����һ������
	
		option 500 #�¼�
		option 501 #ĵ��
		option 501 #��
		option 501 #ܽ��
		
	talk
endproc

proc 401
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b�������һ�
	add_talk #b#b����һ������
	
		option 501 #½����
		option 500 #���й�
		option 501 #������
		option 501 #������
		
	talk
endproc

proc 402
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b�ٱ�������
	add_talk #b#b����һ��ò���
	
		option 501 #����
		option 501 #���
		option 500 #�͹�
		option 501 #����
		
	talk
endproc

proc 403
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#bҹҹ���仨
	add_talk #b#b����һ��ò���
	
		option 501 #�ݴ�
		option 501 #����
		option 501 #�߼�
		option 500 #��л
		
	talk
endproc

proc 404
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#bʮ�������
	add_talk #b#b����һ���
	
		option 501 #��������
		option 501 #�ڰ׷���
		option 500 #�������
		option 501 #���ļ���
		
	talk
endproc

proc 405
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#bŮ��ʮ�˱䣬Խ��Խ�ÿ�
	add_talk #b#b����һ���
	
		option 501 #�仯����
		option 500 #����֮��
		option 501 #ǧ����
		option 501 #������ۼ
		
	talk
endproc

proc 406
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b�ڼ����Ӻù�
	add_talk #b#b����һ�������
	
		option 500 #����
		option 501 #�°�
		option 501 #תҵ
		option 501 #����
		
	talk
endproc

proc 407
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b�粻˵��˵
	add_talk #b#b����һ�֣�
	
		option 501 #��
		option 500 #��
		option 501 #��
		option 501 #��
		
	talk
endproc

proc 408
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b�ط�
	add_talk #b#b����һ�֣�
	
		option 501 #ɣ
		option 501 #��
		option 500 #��
		option 501 #ǧ
		
	talk
endproc

proc 409
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b���Բ�����
	add_talk #b#b����һ�֣�
	
		option 501 #��
		option 501 #��
		option 501 #��
		option 500 #��
		
	talk
endproc

proc 410
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#bͬ�ĸĸ�Ϊ���
	add_talk #b#b����һ�֣�
	
		option 501 #��
		option 501 #��
		option 500 #��
		option 501 #��
		
	talk
endproc

proc 411
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#bһ������֦ͷ������������������
	add_talk #b#b����һ�֣�
	
		option 501 #�
		option 500 #��
		option 501 #��
		option 501 #��
		
	talk
endproc

proc 412
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b���Ѿ��ϲ��˳�
	add_talk #b#b����һ�֣�
	
		option 500 #��
		option 501 #��
		option 501 #��
		option 501 #��
		
	talk
endproc

proc 413
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b�ݰ�����̧̫��
	add_talk #b#b����һ�֣�
	
		option 501 #��
		option 500 #��
		option 501 #��
		option 501 #��
		
	talk
endproc

proc 414
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b����������
	add_talk #b#b����һˮ����
	
		option 501 #����
		option 501 #����
		option 500 #��Բ
		option 501 #����
		
	talk
endproc

proc 415
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b�ձ�����������������Զ�������ˮ����
	add_talk #b#b����һ�
	
		option 501 #����
		option 501 #����
		option 501 #����
		option 500 #��
		
	talk
endproc

proc 416
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#bͷ����컨����ʲ���£����󵱼��ˣ�һ�������
	add_talk #b#b����һ���
	
		option 501 #������
		option 501 #�ܰ�Ƥ
		option 500 #����
		option 501 #ܽ��
		
	talk
endproc

proc 417
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#bС���ɣ��������������룬������
	add_talk #b#b����һ���
	
		option 501 #����
		option 500 #���
		option 501 #Ұ��
		option 501 #����
		
	talk
endproc

proc 418
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b��������һ�̲ˣ�����ˮ���ܵÿ�
	add_talk #b#b����һ���
	
		option 500 #����
		option 501 #����
		option 501 #ˮ��
		option 501 #����
		
	talk
endproc

proc 419
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#bСС������������о��ʣ����°�����ר׽������
	add_talk #b#b����һ���
	
		option 501 #�ڻ�
		option 500 #֩��
		option 501 #���
		option 501 #����
		
	talk
endproc

proc 420
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b��Ҷ���Ķ䣬��ɫ���ֻƣ�һ�꿪һ�Σ����·�����
	add_talk #b#b����һֲ�
	
		option 501 #����
		option 501 #����
		option 500 #��
		option 501 #У��
		
	talk
endproc
proc 500
	TASK_FINI 4630 0
	
	strvar npcID
	GET_MAP_NPC_ID 302172 npcID 0
	mon_speech @npcID 2 0 1 ��ϲ�㣬����ˣ�
	
endproc

proc 501
	COMMIT_TASK 4630 0
	
	add_sys_msg_start 
	add_sys_msg δ�ܲ³��������յף��޷���ý�����
	set_sys_msg_mode 4 0 0
	msg_info
	
	strvar npcID
	GET_MAP_NPC_ID 302172 npcID 0
	mon_speech @npcID 2 0 1 ���Բ��ԣ�
endproc
