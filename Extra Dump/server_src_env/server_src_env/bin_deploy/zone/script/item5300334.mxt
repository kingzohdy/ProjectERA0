;怒火


proc 1
	  get_player_mapinfo 0 1 2 3 4

	  if 2 = 2200

			dec_item_num_inpak 5300334 1 0
;			NPC_DIE 0 1
			DESTROY_MON 1
			
			task_fini 1441 0
;			new_npc 10 215000 10053 14083 170 npc215000.mac	
			strvar X Y	
			get_mon_info 0 0 0 0 0 0 X Y
			NEW_MAP_MON 215000 1 2200 @X @Y 0 0 0 170 20 10 0
;			get_map_npc_id 304002 10 11
;			effect_id 0 @10 playMontion(%d,"40216_5")

	  else
			add_sys_msg_start
			add_sys_msg 目标不在本地图中！
			set_sys_msg_mode 3 0 0
			msg_info
	  endif

endproc


