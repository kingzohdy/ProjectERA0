;BOSS����Ӫ

PROC 1
    	add_sys_msg_start
	add_sys_msg ���ǿ�����Ⱥ��������ͽ���𣿣���	

	set_sys_msg_mode 4 0 0
	map_msg_info

	;	new_npc 1 333021 12797 12760 0 zhu30out.mac


	var 10 0
	set_pworld_var 0 32 @10 1
	var 11 0
	set_pworld_var 4 32 @11 1
	var 12 0
	set_pworld_var 8 32 @12 1



;		NEW_MON 281003 1 9421 16163 0 0 0 1 2
;		NEW_MON 261002 6 9612 16163 0 0 0 1 2

;		NEW_MON 281004 1 16355 16373 0 0 0 1 2
;;		NEW_MON 281005 1 16355 16373 0 0 0 1 2
;		NEW_MON 215000 6 16355 16373 0 0 0 1 2

;		NEW_MON 281006 1 16361 9331 0 0 0 1 2
;		NEW_MON 230002 6 16361 9331 0 0 0 1 2

ENDPROC
PROC 3


;   add_sys_msg_start
;	add_sys_msg 10=$@10$ 11=$@11$ 12=$@12$
;	set_sys_msg_mode 1 0 0
;	map_msg_info

	GET_PWORLD_VAR 0 32 1 10
	GET_PWORLD_VAR 4 32 1 11
        GET_PWORLD_VAR 8 32 1 12


	 IF 10 = 0 and 11 = 0
		add 12 1
		if 12 = 1 
			add_sys_msg_start
			add_sys_msg ��ʿ��׼����ս���ɣ�30���BOSS����������������������ǰ����������	
			set_sys_msg_mode 4 0 0
			map_msg_info
			
		endif
		IF 12 = 12 
			add_sys_msg_start
			add_sys_msg <XXX>��Ҫ�����ڵ�ͼ���࣡��������	
			set_sys_msg_mode 4 0 0
			map_msg_info
		ENDIF
		IF 12 = 15
			EFFECT 0 0 playEffct(12027,13627,"effect\\entity\\9517_16.ent")
		ENDIF
		IF 12 = 16
			rand 200 2
			add 200 100
			switch 200

				case 100
					NEW_MON 281008 1 12027 13627 0 0 0 1 2
				endcase
				case 101
					NEW_MON 281003 1 12027 13627 0 0 0 1 2
				endcase

				add_sys_msg_start
				add_sys_msg  ��һֻBOSS������
				set_sys_msg_mode 4 0 0
				map_msg_info

			endswitch

;			NEW_TMP_ITEM 1400003 12797 12400 0 0 0 1		;����
;			NEW_TMP_ITEM 1400003 13254 12400 0 0 0 1		;����
			NEW_TMP_ITEM 1400003 14512 12400 0 0 0 1		;����
			NEW_TMP_ITEM 1400003 12797 15246 0 0 0 1		;����
			VAR 11 1
		 ENDIF
		set_pworld_var 0 32 @10 1
		SET_PWORLD_VAR 4 32 @11 1
		SET_PWORLD_VAR 8 32 @12 1
	ENDIF
	
	IF 10 = 1 and 11 = 1
		add 12 1
		IF 12 = 17
			add_sys_msg_start
			add_sys_msg ��ʿ��׼����ս���ɣ�20���BOSS����������������������ǰ��������
			set_sys_msg_mode 4 0 0
			map_msg_info		
		ENDIF
		IF 12 = 24
			add_sys_msg_start
			add_sys_msg  <XXX>��Ҫ�����ڵ�ͼ�������򣡣�������
			set_sys_msg_mode 4 0 0
			map_msg_info
		ENDIF
		
		IF 12 = 27
			EFFECT 0 0 playEffct(13473,13673,"effect\\entity\\9517_16.ent")
		ENDIF
		IF 12 = 28
			rand 200 2
			add 200 100
			switch 200
				case 100
					NEW_MON 281004 1 13473 13673 0 0 0 1 2
				endcase
				case 101
					NEW_MON 281006 1 13473 13673 0 0 0 1 2
				endcase

				add_sys_msg_start
				add_sys_msg  �ڶ�ֻBOSS���֣���
				set_sys_msg_mode 4 0 0
				map_msg_info

			endswitch

;			NEW_TMP_ITEM 1400003 12000 12760 0 0 0 1		;����
;			NEW_TMP_ITEM 1400003 13000 12760 0 0 0 1		;����
			NEW_TMP_ITEM 1400003 14000 13500 0 0 0 1		;����
			NEW_TMP_ITEM 1400003 15000 15000 0 0 0 1		;����

			NEW_TMP_ITEM 1440000 13000 12009 0 0 0 1		;����

                VAR 11 2
		ENDIF
		set_pworld_var 0 32 @10 1
		SET_PWORLD_VAR 4 32 @11 1
		SET_PWORLD_VAR 8 32 @12 1
	ENDIF
	
	IF 10 = 2 and 11 = 2
		add 12 1

		IF 12 = 32
			add_sys_msg_start
			add_sys_msg  �������������������ڰ���ʱ�̾�Ҫ�����ˣ�����
			set_sys_msg_mode 4 0 0
			map_msg_info
		ENDIF
		IF 12 = 36
			add_sys_msg_start
			add_sys_msg  <XXX>��Ҫ�����ڵ�ͼ���࣡��������
			set_sys_msg_mode 4 0 0
			map_msg_info		
		ENDIF
		IF 12 = 39
			EFFECT 0 0 playEffct(13653,11912,"effect\\entity\\9517_16.ent")
		ENDIF
		IF 12 = 40
			rand 200 2
			add 200 100
			switch 200
				case 100
		    			NEW_MON 281007 1 13653 11912 0 0 0 1 2
				endcase
				case 101
		    			NEW_MON 281005 1 13653 11912 0 0 0 1 2
				endcase
				
			endswitch

		    add_sys_msg_start
			add_sys_msg ����ֻBOSS���֣�����
			set_sys_msg_mode 4 0 0
			map_msg_info
			NEW_TMP_ITEM 1400003 14949 10648 0 0 0 1		;����
			NEW_TMP_ITEM 1400003 17800 12000 0 0 0 1		;����
			NEW_TMP_ITEM 1400003 10000 17000 0 0 0 1		;����

			NEW_TMP_ITEM 1440000 10000 12354 0 0 0 1		;����
			NEW_TMP_ITEM 1440000 12354 12034 0 0 0 1		;����
			NEW_TMP_ITEM 1440000 11000 10648 0 0 0 1		;����

			VAR 11 3
		ENDIF
		set_pworld_var 0 32 @10 1
		SET_PWORLD_VAR 4 32 @11 1
		SET_PWORLD_VAR 8 32 @12 1
         ENDIF
	IF 10 = 3 AND 11 = 3
		add 12 1

		IF 12 = 42 
			add_sys_msg_start
			add_sys_msg  ���Ǽ�ŭ������<��Ѫ��>,������30��֮�����������ǣ�
			set_sys_msg_mode 4 0 0
			map_msg_info
		ENDIF
		IF 12 = 47
			add_sys_msg_start
			add_sys_msg  <��Ѫ��>����20�����֣�������
			set_sys_msg_mode 4 0 0
			map_msg_info
		ENDIF
		IF 12 = 52
			add_sys_msg_start
			add_sys_msg  <��Ѫ��>����10�����֣�������
			set_sys_msg_mode 4 0 0
			map_msg_info
		ENDIF
		IF 12 = 54
			add_sys_msg_start
			add_sys_msg  ��Ҹо�����صĲ������𣡣�����
			set_sys_msg_mode 4 0 0
			map_msg_info		
		ENDIF
		IF 12 = 56
			EFFECT 0 0 playEffct(12797,12760,"effect\\entity\\281002_18.ent")
		ENDIF
		IF 12 = 58

		    NEW_LIFE_MON 281002 1 12797 12760 0 0 0 180 0 1 1

		    add_sys_msg_start
			add_sys_msg <��Ѫ��>�����ˣ�����
			set_sys_msg_mode 4 0 0
			map_msg_info
			NEW_TMP_ITEM 1400003 15843 12760 0 0 0 1		;����
;			NEW_TMP_ITEM 1400003 12158 12760 0 0 0 1		;����
			NEW_TMP_ITEM 1400003 13687 12760 0 0 0 1		;����

;			NEW_TMP_ITEM 1440000 14151 12009 0 0 0 1		;����
			NEW_TMP_ITEM 1440000 13000 13164 0 0 0 1		;����
			NEW_TMP_ITEM 1440000 13000 11000 0 0 0 1		;����
			VAR 11 4

		ENDIF
		set_pworld_var 0 32 @10 1
		SET_PWORLD_VAR 4 32 @11 1
		SET_PWORLD_VAR 8 32 @12 1
         ENDIF
	
;	IF 10 = 4 AND 11 = 4
;		ADD 12 1
;		IF 12 = 40
;			add_sys_msg_start
;			add_sys_msg ȫ����BOSS�Ѿ��������ҿ����뿪�����ˡ�������
;			set_sys_msg_mode 4 0 0
;			map_msg_info
;			VAR 11 5
;	         ENDIF
;;		set_pworld_var 0 32 @10 1
;		SET_PWORLD_VAR 4 32 @11 1
;		SET_PWORLD_VAR 8 32 @12 1
;	ENDIF


ENDPROC

PROC 4

	GET_PWORLD_VAR 0 32 1 10
;	ADD 10 1	
;	set_pworld_var 0 32 @10 1

	GET_MON_INFO 0 100 0 120 0 0 0 0 
	if 100 = 281008 and 120 = 0
		var 10 1
        	SET_PWORLD_VAR 0 32 @10 1
	endif

	GET_MON_INFO 0 101 0 121 0 0 0 0 
	if 101 = 281003 and 121 = 0
		var 10 1
        	SET_PWORLD_VAR 0 32 @10 1
	endif

	GET_MON_INFO 0 102 0 122 0 0 0 0 
	if 102 = 281004 and 122 = 0
		var 10 2
        	SET_PWORLD_VAR 0 32 @10 1
	endif

	GET_MON_INFO 0 103 0 123 0 0 0 0 
	if 103 = 281006 and 123 = 0
		var 10 2
        	SET_PWORLD_VAR 0 32 @10 1
	endif

	GET_MON_INFO 0 104 0 124 0 0 0 0 
	if 104 = 281005 and 124 = 0
		var 10 3
        	SET_PWORLD_VAR 0 32 @10 1
	endif

	GET_MON_INFO 0 105 0 125 0 0 0 0 
	if 105 = 281007 and 125 = 0
		var 10 3
        	SET_PWORLD_VAR 0 32 @10 1
	endif

	GET_MON_INFO 0 106 0 126 0 0 0 0 
	if 105 = 281002 and 125 = 0
		var 10 4
        	SET_PWORLD_VAR 0 32 @10 1
	endif


	IF 10 = 1
		add_sys_msg_start
		add_sys_msg BOSS<xxx>�������ܻ񣬴�Ҽ��ͣ����滹�кܶ�Σ�յ��Ŵ�ң�����
		set_sys_msg_mode 4 0 0
		map_msg_info
	ENDIF
	IF 10 = 2
		add_sys_msg_start
		add_sys_msg ������Զ���ڵ���������ˣ���ʿ�Ǽ�����ǰ��ɣ�����
		set_sys_msg_mode 4 0 0
		map_msg_info
	ENDIF
	IF 10 = 3 
		add_sys_msg_start
		add_sys_msg ���Ŀ־��Ҫ���ˣ���������ܴ���ȥ���ͽ���Ϊ��������ʿ��
		set_sys_msg_mode 4 0 0
		map_msg_info
	ENDIF
	IF 10 = 4
		add_sys_msg_start
		add_sys_msg �ɹ�������<��Ѫ��>����ҿ�ͨ���������뿪����
		set_sys_msg_mode 4 0 0
		map_msg_info

		new_npc 1 333003 12797 12760 0 zhu30out.mac

	ENDIF

ENDPROC
