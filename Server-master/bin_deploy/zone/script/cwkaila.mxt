;����


proc 1


	dialog 1
	add_talk $_npc$:
	add_talk #b#b��ʱ������Ϊ�����治�ò��˺������������ѱ�����ǣ������������Ǵ���ͬһս�ߣ�������ȫ������

	task_add_option 3002 1 101	
	task_add_option 3003 1 102	

	task_add_option 3007 1 106	
	
	TASK_STAT 3003 1 20
	if 20 = 2
	  	option 170 ��ʼ����
;		option 201 #��������
		option 273 ����������������
		
	endif

	talk
endproc

proc 101
	task_do_option 3002 1

endproc
proc 102
	task_do_option 3003 1

endproc

proc 106
	task_do_option 3007 1

endproc
proc 2
	get_trig_task 1 2
	if 2 = 3007
		TASK_FINI 3007 0
	endif
	if 2 = 3003
		dialog 1
		add_talk $_npc$:
		add_talk #b#b����ʺ絺����֪����ѡʲô��������֪�����ʺ����ĳ�����˭��
		add_talk #b#bͨ������Ĳ��ԣ����Ը��������Ը��Ƽ����ʺ����ĳ���Ŷ���������ݰ������������Ը����Ϸ����Ҫ�������ĵ�һ�о�ѡŶ��
		TASK_STAT 3003 1 30
		if 30 = 2
			option 170 ��ʼ����
;			option 201 #��������
			option 273 ����������������
		ENDIF
		talk
	
	endif
	if 2 = 3004
		
	endif
	
endproc 
proc 201 
	return
endproc 

proc 170

	get_pworld_var 8 32 1 30
	if @30 != 0
		del_npc @30 0
	endif
	get_pworld_var 12 32 1 31
	if @31 != 0
		DELETE_MON_BY_DEFID @31 0
	endif
	
	get_pworld_var 16 32 1 32
	if @32 != 0
		DELETE_MON_BY_DEFID @32 0
	endif

	get_task_var 3003 0 8 1 12
	var 12 0
	set_task_var 3003 0 8 @12 1

	dialog 1
	add_talk $_npc$:
	add_talk #b#b��������ǣ�
		
		option 200 ��������3.12-4.19��
		option 200 ��ţ����4.20-5.20��
		option 200 ˫������5.21-6.21��
		option 200 ��з����6.22-7.22��
		option 200 ʨ������7.23-8.23��
		option 200 ��Ů����8.23-9.22��
		option 200 �������9.23-10.23��
		option 200 ��Ы����10.24-11.22��
		option 200 ��������11.23-12.21��
		option 200 Ħ������12.22-1.19��
		option 200 ˮƿ����1.20-2.18��
		option 200 ˫������2.19-3.20��
	talk
	
endproc

proc 200
	dialog 1
	add_talk $_npc$:
	add_talk #b#b�ڲݴ��﷢����һ��������ĵ�һ�뷨�ǣ�
		
		option 210 ������������
		option 211 ������������
	talk
endproc

proc 210
	get_task_var 3003 0 8 1 12
	var 12 1
	set_task_var 3003 0 8 @12 1

	;����
	dialog 1
	add_talk $_npc$:
	add_talk #b#bϲ���ܵ����˵Ĺ���ô��
		
		option 220 ��Ҫ������Ŀ
		option 221 �������ҽԸ���
	talk
	
endproc 
proc 211
	dialog 1
	add_talk $_npc$:
	add_talk #b#bϲ���ܵ����˵Ĺ���ô��
		
		option 220 ��Ҫ������Ŀ
		option 221 �������ҽԸ���
	talk
	
endproc 
proc 220
	get_task_var 3003 0 8 1 12
	if 12 = 0
		var 12 2
		set_task_var 3003 0 8 @12 1
	endif

	;Ӣ��
	dialog 1
	add_talk $_npc$:
	add_talk #b#bϲ��һ�����ж���
		
		option 230 Ӣ�����ǹ¶���
		option 231 ��Ҫ���ҹ¿�����
	talk

endproc 
proc 221
	dialog 1
	add_talk $_npc$:
	add_talk #b#bϲ��һ�����ж���
		
		option 230 Ӣ�����ǹ¶���
		option 231 ��Ҫ���ҹ¿�����
	talk
endproc 

proc 230
	get_task_var 3003 0 8 1 12
	if 12 = 0
		var 12 3
		set_task_var 3003 0 8 @12 1
	endif

	;�侲
	dialog 1
	add_talk $_npc$:
	add_talk #b#b��������ʱ��������ķ����ǣ�
		
		option 240 ����Ѹ�ٽ��
		option 241 ����Ѱ�ҷ���
	talk
	
endproc 
proc 231 
	dialog 1
	add_talk $_npc$:
	add_talk #b#b��������ʱ��������ķ����ǣ�
		
		option 240 ����Ѹ�ٽ��
		option 241 ����Ѱ�ҷ���
	talk
	
endproc

proc 240
	get_task_var 3003 0 8 1 12
	if 12 = 0
		var 12 4
		set_task_var 3003 0 8 @12 1
	endif

	;³ç
	dialog 1
	add_talk $_npc$:
	add_talk #b#b�����������˳�Ϊ���ѣ�
		
		option 250 �ҵ�����������
		option 251 ������һ֪������
	talk
	
endproc
proc 241

	dialog 1
	add_talk $_npc$:
	add_talk #b#b�����������˳�Ϊ���ѣ�
		
		option 250 �ҵ�����������
		option 251 ������һ֪������
	talk
	
endproc

proc 250

	dialog 1
	add_talk $_npc$:
	add_talk #b#b������츳��һ����������ϣ���ǣ�
		
		option 260 ��������
		option 261 ��ǹ����
	talk
endproc
proc 251
	get_task_var 3003 0 8 1 12
	if 12 = 0
		var 12 5
		set_task_var 3003 0 8 @12 1
	endif

	;��Ƨ
	dialog 1
	add_talk $_npc$:
	add_talk #b#b������츳��һ����������ϣ���ǣ�
		
		option 260 ��������
		option 261 ��ǹ����
	talk
endproc
proc 260
	get_task_var 3003 0 8 1 12
	if 12 = 0
		var 12 6
		set_task_var 3003 0 8 @12 1
	endif	

	;��Ѫ
	dialog 1
	add_talk $_npc$:
	add_talk #b#b��Ļش�ܲ���������Ӧ�ø���ʲô�����
		
		option 270 ����
	talk
endproc
proc 261
		get_task_var 3003 0 8 1 12
	if 12 = 0
		var 12 7
		set_task_var 3003 0 8 @12 1
	endif

	;��С
		dialog 1
	add_talk $_npc$:
	add_talk #b#b��Ļش�ܲ�����������Ӧ�ø���ʲô�����
		
		option 270 ����
	talk
endproc
proc 270
	dialog 1
	add_talk $_npc$

	get_task_var 3003 0 8 1 12

	switch 12
		case 1
		;����
			add_talk #b#b����һϵ�в��ԣ����ʺ���Ļ���ǣ�����ǹ�֣�ҫҫ

	;		add_talk #b#b����������������ó�������к�����
		endcase
		case 2

		;³ç
			add_talk #b#b����һϵ�в��ԣ����ʺ���Ļ���ǣ�����ǹ�֣�����

	;		add_talk #b#b³ç�������������ó���߱�����3%	
			
		endcase
		case 3
		;��Ѫ
			add_talk #b#b����һϵ�в��ԣ����ʺ���Ļ���ǣ�����ǹ�֣�����

	;		add_talk #b#b��Ѫ�������������ó�����﹥�ͷ���5%

		endcase
		case 4
		;��С
			add_talk #b#b����һϵ�в��ԣ����ʺ���Ļ���ǣ�����ǹ�֣�����

	;		add_talk #b#b��С�������������ó��������ͷ���5%
		endcase
		case 5
		;�侲
			add_talk #b#b����һϵ�в��ԣ����ʺ���Ļ���ǣ�����ǹ�֣�����

	;		add_talk #b#b�侲�������������ó������������10%	

		endcase
		case 6
		;Ӣ��
			add_talk #b#b����һϵ�в��ԣ����ʺ���Ļ���ǣ�����ǹ�֣�����

	;		add_talk #b#bӢ�µ������������ó����������5%
		endcase
		case 7
		;��Ƨ
			add_talk #b#b����һϵ�в��ԣ����ʺ���Ļ���ǣ�����ǹ�֣��Ѽ�

	;		add_talk #b#b��Ƨ�������������ó���߽���������5%
		endcase
	endswitch
		option 271 #��������
		option 170 �ٲ�һ��
		option 273 ��û�д����������İ취��
	talk
endproc

proc 271

	get_task_var 3003 0 8 1 12
	get_pworld_var 8 32 1 30
	switch 12
		case 1
			new_npc 0 352000 6806 5935 0 cwtiexin1.mac
			var 30 352000
			set_pworld_var 8 32 @30 1
			
			GET_MAP_NPC_ID 352000 50 0 
			mon_speech @50 0 1 0 �޷�����������ȱ�������������Ƕ�ô���£�
		endcase              
		case 2               
			new_npc 0 352001 6806 5935 0 cwtiexin2.mac
			var 30 352001
			set_pworld_var 8 32 @30 1
			
			GET_MAP_NPC_ID 352001 51 0 
			mon_speech @51 0 1 0 �޷�����������ȱ�������������Ƕ�ô���£�
		endcase              
		case 3               
			new_npc 0 352002 6806 5935 0 cwtiexin3.mac
			var 30 352002
			set_pworld_var 8 32 @30 1
			GET_MAP_NPC_ID 352002 52 0 
			mon_speech @52 0 1 0 �޷�����������ȱ�������������Ƕ�ô���£�
		endcase              
		case 4               
			new_npc 0 352003 6806 5935 0 cwtiexin4.mac
			var 30 352003
			set_pworld_var 8 32 @30 1
			GET_MAP_NPC_ID 352003 53 0 
			mon_speech @53 0 1 0 �޷�����������ȱ�������������Ƕ�ô���£�
		endcase              
		case 5               
			new_npc 0 352004 6806 5935 0 cwtiexin5.mac
			var 30 352004
			set_pworld_var 8 32 @30 1
			GET_MAP_NPC_ID 352004 54 0 
			mon_speech @54 0 1 0 �޷�����������ȱ�������������Ƕ�ô���£�
		endcase              
		case 6               
			new_npc 0 352005 6806 5935 0 cwtiexin6.mac
			var 30 352005
			set_pworld_var 8 32 @30 1
			GET_MAP_NPC_ID 352005 55 0 
			mon_speech @55 0 1 0 �޷�����������ȱ�������������Ƕ�ô���£�
		endcase              
		case 7               
			new_npc 0 352006 6806 5935 0 cwtiexin7.mac
			var 30 352006
			set_pworld_var 8 32 @30 1
			GET_MAP_NPC_ID 352006 56 0 
			mon_speech @56 0 1 0 �޷�����������ȱ�������������Ƕ�ô���£�
		endcase             
	endswitch
;   ADD_FINI_TASK 3003 500	

endproc
proc 273
	dialog 1
	add_talk $_npc$:
	add_talk #b#b����$_player$���ԸղŵĽ���������𣿻�����Ҳ��ϲ��ī�سɹ棿��������������������������
	add_talk #b#b<������һ�Ű������ӵݵ�������>
	add_talk #b#b�����������뾪ϲ���������ܹ��ḻ��ʣ��԰ɣ�ת����������˭����������ע���Ļ�飡
;    ADD_FINI_TASK 3003 500	
		option 300 #����������ע���ĳ�����˭
	talk
endproc

proc 300
		
	get_pworld_var 8 32 1 30
	if @30 != 0
		del_npc @30 0
	endif
	get_pworld_var 12 32 1 31
	if @31 != 0
		DELETE_MON_BY_DEFID @31 0
	endif
	get_pworld_var 16 32 1 32
	if @32 != 0
		DELETE_MON_BY_DEFID @32 0
	endif
	
	call 310	
		;�������ֻ����ȡһ������
;		GET_ITEM_N UM_INPAK 5300377 0 430
;		if 430 = 0
;			ADD_ITEM_NUM_INPAK 5300377 1 1 0	
;			add_sys_msg_start
;			add_sys_msg ����������������������Σ�
;			set_sys_msg_mode 3 0 0
;			msg_info
;		else
;			add_sys_msg_start
;			add_sys_msg �����ϻ��и�����û�ù��ء���
;			set_sys_msg_mode 3 0 0
;			msg_info
;		endif

	return
endproc
proc 310

	get_task_var 3003 1 8 1 13
	var 13 1
	set_task_var 3003 1 8 @13 1

	rand 50 8
	add 50 100
	switch 50

		case 100
			effect 0 0 playEffct(6700,5300,"effect\\entity\\9555_16.ent")
			get_task_var 3003 3 8 1 15
			var 15 1
			set_task_var 3003 3 8 @15 1		
			
			
		endcase
		case 101
			effect 0 0 playEffct(6700,5300,"effect\\entity\\9556_16.ent")
			get_task_var 3003 3 8 1 15
			var 15 2
			set_task_var 3003 3 8 @15 1	
			
		endcase
		case 102
			effect 0 0 playEffct(6700,5300,"effect\\entity\\9557_16.ent")
			get_task_var 3003 3 8 1 15
			var 15 3
			set_task_var 3003 3 8 @15 1	
		endcase
		case 103
			effect 0 0 playEffct(6700,5300,"effect\\entity\\9558_16.ent")
			get_task_var 3003 3 8 1 15
			var 15 4
			set_task_var 3003 3 8 @15 1	
		endcase
		
		case 104
			effect 0 0 playEffct(6700,5300,"effect\\entity\\9559_16.ent")
			get_task_var 3003 3 8 1 15
			var 15 5
			set_task_var 3003 3 8 @15 1	
		endcase
		case 105
			effect 0 0 playEffct(6700,5300,"effect\\entity\\9560_16.ent")
			get_task_var 3003 3 8 1 15
			var 15 6
			set_task_var 3003 3 8 @15 1	
		endcase
		case 106
			effect 0 0 playEffct(6700,5300,"effect\\entity\\9561_16.ent")
			get_task_var 3003 3 8 1 15
			var 15 7
			set_task_var 3003 3 8 @15 1	
		endcase
		case 107
			effect 0 0 playEffct(6700,5300,"effect\\entity\\9562_16.ent")
			get_task_var 3003 3 8 1 15
			var 15 8
			set_task_var 3003 3 8 @15 1	
		endcase

	endswitch


endproc 
 

























