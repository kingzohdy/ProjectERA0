;������ҩ��
;���񡪡�4006
;���á���ʹ�ú�ɾ�������������
proc 1
	get_player_mapinfo 0 1 2 3 4
	get_point_dist @3 @4 6500 38600 10
	if 2 = 2000 OR 2 = 2001 OR 2 = 2002 OR 2 = 2003 or 2 = 2004 or 2 = 2005 or 2 = 2006 or 2 = 2007 
		if 10 <= 500  
			
			dec_item_num_inpak 5300136 1 0
			task_fini 1072 0
			
			get_map_npc_id 306002 10 11
			effect_id 0 @10 playMontion(%d,"40216_5")

		else
			add_sys_msg_start
			add_sys_msg �������������
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	ELSE
			add_sys_msg_start
			add_sys_msg Ŀ�겻��ָ����ͼ�У�
			set_sys_msg_mode 3 0 0
			msg_info
	ENDIF
endproc

