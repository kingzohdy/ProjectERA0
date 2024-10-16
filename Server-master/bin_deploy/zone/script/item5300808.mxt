;太阳金经



proc 1
	get_player_mapinfo 0 1 2 3 4
	if 2 = 3302
;			effect 0 0 playEffct(4942,14841,"effect\\entity\\9513_16.ent")
;			dec_item_num_inpak 5300808 1 0			
;			add_item_num_inpak 5300339 1 1 0
			strvar Mid
			GET_TARGET_INFO 3 1 Mid
			TARGET_ADD_STATUS 3 @Mid 1261 1 10 1
	  else
	  			add_sys_msg_start
			add_sys_msg 此物品需要在阿努比斯祭坛使用！
			set_sys_msg_mode 3 0 0
			msg_info
	  endif

endproc


