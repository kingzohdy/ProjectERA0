;����Ůboss
proc 2

;	strvar x y
;	GET_MON_INFO 0 0 0 0 0 0 x y
;	effect 0 0 playEffct($@x$,$@y$,"effect\\entity\\9535_16.ent")

	range_set 5 6
	pworld_over 
	var 10 -1
	set_pworld_var 0 32 @10 1

	add_sys_msg_start
	add_sys_msg ɳĮ��ʤ����
	set_sys_msg_mode 1 0 0
	MAP_MSG_INFO 
	strvar camp
	GET_ATTACKER_CAMP 1 camp
	;����ʤ������Ӫ
	SET_BATTLE_WIN 1 2

endproc
	
;��ʱ,3����
proc 4
	get_pworld_var 8 32 1 12
	
	;����˿��ʱ
	add 12 1
	set_pworld_var 8 32 @12 1
	var 24 @12
	mod 12 10
	is_mon_fighting 1 2
	if 12 = 0 and 2 = 1
		add_sys_msg_start
		add_sys_msg ���޷�ָ�ӹٿ���˿���ֵ��ˣ���Ҫ��������
		set_sys_msg_mode 4 0 0
		MAP_MSG_INFO 
	endif	
	strvar Lock
	get_pworld_var 16 32 1 Lock 1
	if Lock = 10	
		mod 24 3
		if 24 = 0
			get_pworld_var 4 32 1 11
			strvar mid
			GET_MON_INFO 1 0 mid 0 0 0 0 0

			if 11 != 1
				strvar id X Y
				GET_MON_INFO 0 0 id 0 0 0 X Y
;				EFFECT_ID 0 @id playMontion(%d,"350047")
				effect 0 0 playEffct($@X$,$@Y$,"effect\\entity\\350049.ent")
				;; ��̬��Ѫ
				strvar pworld_lv
				get_pworld_var 32 32 1 pworld_lv 
				switch pworld_lv
					case 401
						mon_set_hp -2889 @mid 1
					endcase
					case 402
						mon_set_hp -6009 @mid 1
					endcase
					case 403
						mon_set_hp -11559 @mid 1
					endcase
					case 404
						mon_set_hp -20229 @mid 1
					endcase
					case 405
						mon_set_hp -33060 @mid 1
					endcase
					case 406
						mon_set_hp -50862 @mid 1
					endcase
					case 407
						mon_set_hp -73983 @mid 1
					endcase 
					case 408
						mon_set_hp -106005 @mid 1
					endcase 
					case 409
						mon_set_hp -151896 @mid 1
					endcase 
				endswitch
			endif
			
		endif
	endif
endproc

