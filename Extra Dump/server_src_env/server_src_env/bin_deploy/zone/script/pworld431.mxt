;�������ս����    ,������ս���ٵ��Ǹ�,������Ϸ


proc 1
	;������ʼ��
	;monmemid10,��Ŧ��memid 1
	var 10 0
	set_pworld_var 0 32 @10 1
	;monmemid11,��Ŧ��memid 2
	var 11 0
	set_pworld_var 4 32 @11 1
	;monmemid12,��Ŧ��memid 3
	var 12 0
	set_pworld_var 8 32 @12 1
	;camp1buffcool12,��Ӫ1buff��ȴʱ��
	var 12 0
	set_pworld_var 8 32 @12 1
	;camp2buffcool13,��Ӫ2buff��ȴʱ��
	var 13 0
	set_pworld_var 12 32 @13 1
	;camp3buffcool14,��Ӫ3buff��ȴʱ��
	var 14 0
	set_pworld_var 16 32 @14 1
	;camp1fly1buy15,��Ӫ1�ɻ�1������
	var 15 1
	set_pworld_var 20 32 @15 1
	;camp1fly2buy16,��Ӫ1�ɻ�2������
	var 16 0
	set_pworld_var 24 32 @16 1
	;camp1fly3buy17,��Ӫ1�ɻ�3������
	var 17 0
	set_pworld_var 28 32 @17 1
	;camp1fly4buy18,��Ӫ1�ɻ�4������
	var 18 0
	set_pworld_var 32 32 @18 1
	;camp1fly5buy19,��Ӫ1�ɻ�5������
	var 19 0
	set_pworld_var 36 32 @19 1
	;camp2fly1buy20,��Ӫ2�ɻ�1������
	var 20 1
	set_pworld_var 40 32 @20 1
	;camp2fly2buy21,��Ӫ2�ɻ�2������
	var 21 0
	set_pworld_var 44 32 @21 1
	;camp2fly3buy22,��Ӫ2�ɻ�3������
	var 22 0
	set_pworld_var 48 32 @22 1
	;camp2fly4buy23,��Ӫ2�ɻ�4������
	var 23 0
	set_pworld_var 52 32 @23 1
	;camp2fly5buy24,��Ӫ2�ɻ�5������
	var 24 0
	set_pworld_var 56 32 @24 1
	;camp3fly1buy25,��Ӫ3�ɻ�1������
	var 25 1
	set_pworld_var 60 32 @25 1
	;camp3fly2buy26,��Ӫ3�ɻ�2������
	var 26 0
	set_pworld_var 64 32 @26 1
	;camp3fly3buy27,��Ӫ3�ɻ�3������
	var 27 0
	set_pworld_var 68 32 @27 1
	;camp3fly4buy28,��Ӫ3�ɻ�4������
	var 28 0
	set_pworld_var 72 32 @28 1
	;camp3fly5buy29,��Ӫ3�ɻ�5������
	var 29 0
	set_pworld_var 76 32 @29 1
	;battlestat30,ս���׶�|0��ʼ�׶� 1׼���׶� 2��ʼ�׶�
	var 30 1
	set_pworld_var 80 32 @30 1
	;camp1buffcoolreduce31,��Ӫ1buff������ȴʱ��ʹ�ô���
	var 31 0
	set_pworld_var 84 32 @31 1
	;camp2buffcoolreduce32,��Ӫ2buff������ȴʱ��ʹ�ô���
	var 32 0
	set_pworld_var 88 32 @32 1
	;camp3buffcoolreduce33,��Ӫ3buff������ȴʱ��ʹ�ô���
	var 33 0
	set_pworld_var 92 32 @33 1
	;camp1die34,��Ӫ1��Ŧ�������
	var 34 0
	set_pworld_var 96 32 @34 1
	;camp2die35,��Ӫ2��Ŧ�������
	var 35 0
	set_pworld_var 100 32 @35 1
	;camp3die36,��Ӫ3��Ŧ�������
	var 36 0
	set_pworld_var 104 32 @36 1
	;camp1num37,��Ӫ1��ҽ������
	var 37 0
	set_pworld_var 108 32 @37 1
	;camp2num38,��Ӫ2��ҽ������
	var 38 0
	set_pworld_var 112 32 @38 1
	;camp3num39,��Ӫ3��ҽ������
	var 39 0
	set_pworld_var 116 32 @39 1

	;����Ӫ���е�
	;NEW_CAMP_NPC 1 350103 10089 13864 0 1 npc350103.mac
	NEW_NPC 1 350103 10089 13864 0 npc350103.mac
	if @1 > 0
		set_mon_curr @1 2
		;monfly10,�������ѹ�����|������������
		var 10 16
		set_mon_var 0 32 @10 1
	endif
	
	;NEW_CAMP_NPC 1 350103 10412 13694 0 1 npc350103.mac
	NEW_NPC 1 350103 10412 13694 0 npc350103.mac
	if @1 > 0
		set_mon_curr @1 2
		;monfly10,�������ѹ�����|������������
		var 10 17
		set_mon_var 0 32 @10 1
	endif
	
	;NEW_CAMP_NPC 1 350103 9361 14010 0 1 npc350103.mac
	NEW_NPC 1 350103 9361 14010 0 npc350103.mac
	if @1 > 0
		set_mon_curr @1 2
		;monfly10,�������ѹ�����|������������
		var 10 18
		set_mon_var 0 32 @10 1
	endif
	
	;NEW_CAMP_NPC 1 350103 9019 13978 0 1 npc350103.mac
	NEW_NPC 1 350103 9019 13978 0 npc350103.mac
	if @1 > 0
		set_mon_curr @1 2
		;monfly10,�������ѹ�����|������������
		var 10 19
		set_mon_var 0 32 @10 1
	endif
	
	;NEW_CAMP_NPC 1 350104 17413 22613 0 2 npc350104.mac
	NEW_NPC 1 350104 17413 22613 0 npc350104.mac
	if @1 > 0
		set_mon_curr @1 2
		;monfly10,�������ѹ�����|������������
		var 10 21
		set_mon_var 0 32 @10 1
	endif
	
	;NEW_CAMP_NPC 1 350104 17727 23315 0 2 npc350104.mac
	NEW_NPC 1 350104 17727 23315 0 npc350104.mac
	if @1 > 0
		set_mon_curr @1 2
		;monfly10,�������ѹ�����|������������
		var 10 22
		set_mon_var 0 32 @10 1
	endif
	
	;NEW_CAMP_NPC 1 350104 17705 23763 0 2 npc350104.mac
	NEW_NPC 1 350104 17705 23763 0 npc350104.mac
	if @1 > 0
		set_mon_curr @1 2
		;monfly10,�������ѹ�����|������������
		var 10 23
		set_mon_var 0 32 @10 1
	endif
	
	;NEW_CAMP_NPC 1 350104 17167 22373 0 2 npc350104.mac
	NEW_NPC 1 350104 17167 22373 0 npc350104.mac
	if @1 > 0
		set_mon_curr @1 2
		;monfly10,�������ѹ�����|������������
		var 10 24
		set_mon_var 0 32 @10 1
	endif
	
	;NEW_CAMP_NPC 1 350105 21288 11852 0 3 npc350105.mac
	NEW_NPC 1 350105 21288 11852 0 npc350105.mac
	if @1 > 0
		set_mon_curr @1 2
		;monfly10,�������ѹ�����|������������
		var 10 26
		set_mon_var 0 32 @10 1
	endif
	
	;NEW_CAMP_NPC 1 350105 21766 11250 0 3 npc350105.mac
	NEW_NPC 1 350105 21766 11250 0 npc350105.mac
	if @1 > 0
		set_mon_curr @1 2
		;monfly10,�������ѹ�����|������������
		var 10 27
		set_mon_var 0 32 @10 1
	endif
	
	;NEW_CAMP_NPC 1 350105 22050 11033 0 3 npc350105.mac
	NEW_NPC 1 350105 22050 11033 0 npc350105.mac
	if @1 > 0
		set_mon_curr @1 2
		;monfly10,�������ѹ�����|������������
		var 10 28
		set_mon_var 0 32 @10 1
	endif
	
	;NEW_CAMP_NPC 1 350105 21150 12301 0 3 npc350105.mac
	NEW_NPC 1 350105 21150 12301 0 npc350105.mac
	if @1 > 0
		set_mon_curr @1 2
		;monfly10,�������ѹ�����|������������
		var 10 29
		set_mon_var 0 32 @10 1
	endif
	

	;����Ӫ��Ŧ�ػ���
	;NEW_DYN_MON 1 303070 1 8106 12167 0 0 0 2 1 0 0
	NEW_CAMP_NPC 1 303070 8106 12167 135 1 npc303070.mac
	;SET_CAMP_MON_MAP_SHOW #���ر�־ #����MemID #��Ӫ #����
;	if @1 > 0
;		SET_CAMP_MON_MAP_SHOW 2 @1 1 0
;	endif
	;NEW_DYN_MON 1 303071 1 15919 25655 0 0 0 2 2 0 0
	NEW_CAMP_NPC 1 303071 15919 25655 0 2 npc303071.mac
	;SET_CAMP_MON_MAP_SHOW #���ر�־ #����MemID #��Ӫ #����
;	if @1 > 0
;		SET_CAMP_MON_MAP_SHOW 2 @1 2 0
;	endif
	;NEW_DYN_MON 1 303072 1 24000 10950 0 0 0 2 3 0 0
	NEW_CAMP_NPC 1 303072 24000 10950 60 3 npc303072.mac
	;SET_CAMP_MON_MAP_SHOW #���ر�־ #����MemID #��Ӫ #����
;	if @1 > 0
;		SET_CAMP_MON_MAP_SHOW 2 @1 3 0
;	endif

	;����Ӫ��Ŧ
	;NEW_MON #����id #�������� #����x #����y #����뾶 #Ѳ��ID #�Ƿ�������� #���ر�־ #��һֻ�����ʵ��ID
	NEW_MON 261196 1 9285 12319 0 0 0 1 3 
	;SET_CAMP_MON_MAP_SHOW #���ر�־ #����MemID #��Ӫ #����
	;SET_MON_CAMP_IDX #����ֵ #ʵ��ID #��Ӫ����
	if 1 = 0
		SET_MON_CAMP_IDX 2 @3 1
		SET_CAMP_MON_MAP_SHOW 2 @3 1 0
	endif
	;NEW_MON #����id #�������� #����x #����y #����뾶 #Ѳ��ID #�Ƿ�������� #���ر�־ #��һֻ�����ʵ��ID
	NEW_MON 261197 1 16041 23527 0 0 0 1 3 
	;SET_CAMP_MON_MAP_SHOW #���ر�־ #����MemID #��Ӫ #����
	;SET_MON_CAMP_IDX #����ֵ #ʵ��ID #��Ӫ����
	if 1 = 0
		SET_MON_CAMP_IDX 2 @3 2
		SET_CAMP_MON_MAP_SHOW 2 @3 2 0
	endif
	;NEW_MON #����id #�������� #����x #����y #����뾶 #Ѳ��ID #�Ƿ�������� #���ر�־ #��һֻ�����ʵ��ID
	NEW_MON 261198 1 22734 12442 0 0 0 1 3 
	;SET_CAMP_MON_MAP_SHOW #���ر�־ #����MemID #��Ӫ #����
	;SET_MON_CAMP_IDX #����ֵ #ʵ��ID #��Ӫ����
	if 1 = 0
		SET_MON_CAMP_IDX 2 @3 3
		SET_CAMP_MON_MAP_SHOW 2 @3 3 0
	endif

	add_sys_msg_start
	add_sys_msg ս��30���ʼ 
	set_sys_msg_mode 3 0 0
	map_msg_info

	SET_PWORLD_TIMER 1 30
	;����Ӫ�赲
	call 100

	
endproc

;
proc 3
;	strvar campid num
;	battle_min_camp_player_num campid num
;	if num <= 0 and campid > 0
;	endif
endproc

; ���������¼�
proc 4
	strvar defid livenum livecamp
	var livenum 0
;	var diecamp 0
	var livecamp 0
	;GET_NPC_NUM #NpcID #���ر�־ #����
	;GET_MON_INFO #���ر�־ #����id #ʵ��id #Ѫ�� #��ͼid #��ͼʵ�� #����X #Y 
	get_mon_info 1 defid 0 0 0 0 0 0


	;��Ŧ�� 1
	;GET_MON_NUM_BY_DEFID #����Ķ���ID #���ر�־
;	GET_MON_NUM_BY_DEFID 261196 3
;	if defid = 261196 or 3 < 1
;		add dienum 1
;		add diecamp 1
;	endif 
	if defid = 261196 
		;camp1die34,��Ӫ1��Ŧ�������
		var 34 0
		get_pworld_var 96 32 1 34
		add 34 1
		;camp1die34,��Ӫ1��Ŧ�������
		var 34 @34;
		set_pworld_var 96 32 @34 1
		
		DEL_NPC 303070 1
	endif 

	;��Ŧ�� 2
	;GET_MON_NUM_BY_DEFID #����Ķ���ID #���ر�־
;	GET_MON_NUM_BY_DEFID 261197 3
;	if defid = 261197 or 3 < 1
;		add dienum 1
;		add diecamp 2
;	endif 
	if defid = 261197 
		;camp2die35,��Ӫ2��Ŧ�������
		var 35 0
		get_pworld_var 100 32 1 35
		add 35 1
		;camp2die35,��Ӫ2��Ŧ�������
		var 35 @35;
		set_pworld_var 100 32 @35 1
		
		DEL_NPC 303071 1
	endif 

	;��Ŧ�� 3
	;GET_MON_NUM_BY_DEFID #����Ķ���ID #���ر�־
;	GET_MON_NUM_BY_DEFID 261198 3
;	if defid = 261198 or 3 < 1
;		add dienum 1
;		add diecamp 3
;	endif 
	if defid = 261198 
		;camp3die36,��Ӫ3��Ŧ�������
		var 36 0
		get_pworld_var 104 32 1 36
		add 36 1
		;camp3die36,��Ӫ3��Ŧ�������
		var 36 @36;
		set_pworld_var 104 32 @36 1
		
		DEL_NPC 303072 1
	endif 
	
	;camp1die34,��Ӫ1��Ŧ�������
	var 34 0
	get_pworld_var 96 32 1 34
	if 34 = 0
		add livenum 1
		var livecamp 1
	endif
	
	;camp2die35,��Ӫ2��Ŧ�������
	var 35 0
	get_pworld_var 100 32 1 35
	if 35 = 0
		add livenum 1
		var livecamp 2
	endif
	
	;camp3die36,��Ӫ3��Ŧ�������
	var 36 0
	get_pworld_var 104 32 1 36
	if 36 = 0
		add livenum 1
		var livecamp 3
	endif
	if livenum = 1

		set_battle_win 1 @livecamp
		PWORLD_OVER
;		add_sys_msg_start
;		add_sys_msg win:$@livecamp$
;		set_sys_msg_mode 1 0 0
;		map_msg_info
	endif
	
	
	
;	add_sys_msg_start
;	add_sys_msg die:$@defid$ live:$@livenum$
;	set_sys_msg_mode 1 0 0
;	map_msg_info

endproc

;��ҽ����¼�
proc 5

	
	;��ʾ
	strvar begintime
	get_pworld_timer begintime
	if begintime > 0

		add_sys_msg_start
		add_sys_msg ս��$@begintime$���ʼ  
		set_sys_msg_mode 3 0 0
		msg_info
	endif

	;����
	EFFECT 2 1 SetMessageTips(320)

	;��ŦС��ͼ��ʾ
	;CAMP_MON_MAP_SHOW_ACTION #���ر�־ #��Ӫ #���� #�㲥����
	CAMP_MON_MAP_SHOW_ACTION 1 0 1 0

	strvar playercamp
	GET_ATTACKER_CAMP 1 playercamp
	;��ҽ������
	if playercamp = 1 
		;camp1num37,��Ӫ1��ҽ������
		var 37 0
		get_pworld_var 108 32 1 37
		add 37 1
		;camp1num37,��Ӫ1��ҽ������
		
		set_pworld_var 108 32 @37 1
	endif 
	;��ҽ������
	if playercamp = 2 
		;camp2num38,��Ӫ2��ҽ������
		var 38 0
		get_pworld_var 112 32 1 38
		add 38 1
		;camp2num38,��Ӫ2��ҽ������
		
		set_pworld_var 112 32 @38 1
	endif 
	;��ҽ������
	if playercamp = 3 
		;camp3num39,��Ӫ3��ҽ������
		var 39 0
		get_pworld_var 116 32 1 39
		add 39 1
		;camp3num39,��Ӫ3��ҽ������
		
		set_pworld_var 116 32 @39 1
	endif 
endproc

;����ʱ�¼�
proc 7
	;ɾ�赲
	delete_mon_by_defid 294008 1	

	add_sys_msg_start
	add_sys_msg ս����ʼ 
	set_sys_msg_mode 3 0 0
	map_msg_info
	;����ս��״̬
	;battlestat30,ս���׶�|0��ʼ�׶� 1׼���׶� 2��ʼ�׶�
	var 30 2
	set_pworld_var 80 32 @30 1
endproc

proc 100
	new_mon_by_dir 294008 1 11131 13490 0 0 0 30 1 2
	new_mon_by_dir 294008 1 11252 13340 0 0 0 30 1 2
	new_mon_by_dir 294008 1 11353 13201 0 0 0 30 1 2
	new_mon_by_dir 294008 1 11423 13062 0 0 0 30 1 2
	new_mon_by_dir 294008 1 11505 12938 0 0 0 30 1 2
	new_mon_by_dir 294008 1 11557 12834 0 0 0 30 1 2

	new_mon_by_dir 294008 1 16506 21514 0 0 0 0 1 2
	new_mon_by_dir 294008 1 16351 21502 0 0 0 0 1 2
	new_mon_by_dir 294008 1 16175 21512 0 0 0 0 1 2
	new_mon_by_dir 294008 1 15999 21522 0 0 0 0 1 2
	new_mon_by_dir 294008 1 15823 21532 0 0 0 0 1 2
	new_mon_by_dir 294008 1 15647 21552 0 0 0 0 1 2
	new_mon_by_dir 294008 1 15295 21562 0 0 0 0 1 2

	new_mon_by_dir 294008 1 20701 13107 0 0 0 55 1 2
	new_mon_by_dir 294008 1 20755 13333 0 0 0 55 1 2
	new_mon_by_dir 294008 1 20852 13524 0 0 0 55 1 2
	new_mon_by_dir 294008 1 20949 13715 0 0 0 55 1 2
	new_mon_by_dir 294008 1 21063 13910 0 0 0 55 1 2
endproc
