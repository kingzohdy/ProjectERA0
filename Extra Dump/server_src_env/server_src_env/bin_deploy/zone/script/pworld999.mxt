;BOSS����Ӫ

PROC 1
    	add_sys_msg_start
	add_sys_msg ����BOSS����Ӫ��	
	set_sys_msg_mode 4 0 0
	map_msg_info


	var 10 0
	set_pworld_var 0 32 @10 1
	var 11 0
	set_pworld_var 0 32 @11 1
	var 12 0
	set_pworld_var 0 32 @12 1



ENDPROC
PROC 3


;        add_sys_msg_start
;	add_sys_msg 10=$@10$ 11=$@11$ 12=$@12$
;	set_sys_msg_mode 1 0 0
;	map_msg_info


	GET_PWORLD_VAR 8 32 1 12
        GET_PWORLD_VAR 4 32 1 11


	 IF 10 = 0 and 11 = 0
		add 12 1
		IF 12 = 5
			NEW_MON 231001 1 9463 12168 0 0 0 1 2

			add_sys_msg_start
			add_sys_msg  ��һֻBOSS������
			set_sys_msg_mode 4 0 0
			map_msg_info

			VAR 11 1
		 ENDIF

		SET_PWORLD_VAR 4 32 @11 1
		SET_PWORLD_VAR 8 32 @12 1
	ENDIF
	
	IF 10 = 1 and 11 = 1
		add 12 1
		IF 12 = 15
		    NEW_MON 201001 8 9362 17430 500 0 0 1 2

		    add_sys_msg_start
		    add_sys_msg �ڶ�ֻBOSS������
		    set_sys_msg_mode 4 0 0
		    map_msg_info

                VAR 11 2
		ENDIF

		SET_PWORLD_VAR 4 32 @11 1
		SET_PWORLD_VAR 8 32 @12 1
	ENDIF
	
	IF 10 = 2 and 11 = 2
		add 12 1
		IF 12 = 25


		    NEW_MON 201001 2 7885 16466 500 0 0 1 2
		    	add_sys_msg_start
			add_sys_msg ����ֻBOSS������
			set_sys_msg_mode 4 0 0
			map_msg_info


			VAR 11 3
		ENDIF

		SET_PWORLD_VAR 4 32 @11 1
		SET_PWORLD_VAR 8 32 @12 1
         ENDIF
	IF 10 = 3 AND 11 = 3
		add 12 1
		IF 12 = 35

		    NEW_MON 201001 3 10831 16362 500 0 0 1 2

		    add_sys_msg_start
			add_sys_msg ����BOSSս��ʼ�ˡ�������
			set_sys_msg_mode 4 0 0
			map_msg_info

			VAR 11 4
		ENDIF

		SET_PWORLD_VAR 4 32 @11 1
		SET_PWORLD_VAR 8 32 @12 1
         ENDIF
	
	IF 10 = 4 AND 11 = 4
		ADD 12 1
		IF 12 = 40
			add_sys_msg_start
			add_sys_msg ȫ����BOSS�Ѿ��������ҿ����뿪�����ˡ�������
			set_sys_msg_mode 4 0 0
			map_msg_info
			VAR 11 5
	         ENDIF
		SET_PWORLD_VAR 4 32 @11 1
		SET_PWORLD_VAR 8 32 @12 1
	ENDIF


ENDPROC

PROC 4

	GET_PWORLD_VAR 0 32 1 10
	ADD 10 1	
	set_pworld_var 0 32 @10 1

	IF 10 = 4
		add_sys_msg_start
		add_sys_msg ��������ɣ���ͨ���������뿪
		set_sys_msg_mode 4 0 0
		map_msg_info

		;new_npc 1 333021 9362 17430 0 zhu30out.mac

	ENDIF

ENDPROC
