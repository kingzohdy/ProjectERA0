;�ݽߵ����ʯ	

proc 1

;	strvar Mp MaxMp
;	GET_ID_PLAYER_INFO 0 10 MaxMp
;	GET_ID_PLAYER_INFO 0 11 Mp
;	if @Mp > 30
;		add_sys_msg_start
;		add_sys_msg ��������30ħ��ֵ��
;		set_sys_msg_mode 3 0 0
;		msg_info
;		effect 2 0 playEffct(20817,34100,"effect\\entity\\9568_16.ent")			
		
	get_player_mapinfo 0 1 2 3 4
	get_point_dist @3 @4 9000 25900 10
	if 2 >= 2000 or 2 <= 2007
		if 10 <= 500  
			dec_item_num_inpak 5300445 1 0
			ADD_PLAYER_MP 0 30 0
		
			ADD_ITEM_NUM_INPAK 5300446 1 1 0

			GET_PLAYER_ID 1 30
			effect_id 0 @30 playMontion(%d,"40100_5")
			
			add_sys_msg_start
			add_sys_msg ��е���һ��ԴԴ������ħ�����������е����ʯ��ɢ����������ʱ������ˬ
			set_sys_msg_mode 3 0 0
			msg_info
		else
			add_sys_msg_start
			add_sys_msg ������ȷ������ʹ�ã�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	  else
			add_sys_msg_start
			add_sys_msg ����Ʒֻ������յ�ʹ�ã�
			set_sys_msg_mode 3 0 0
			msg_info
	  endif
endproc
