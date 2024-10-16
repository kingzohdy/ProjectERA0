;;;;石头也怕锤 土灵之锤



proc 1

			
	get_player_mapinfo 0 1 2 3 4
	if 2 = 2201
		
			strvar X Y Rad
;			dec_item_num_inpak 5300592 1 0
			
			GET_TARGET_INFO 3 3 X
			GET_TARGET_INFO 3 4 Y
			DESTROY_MON 1
;			NEW_LIFE_MON 205037 2 @X @Y 300 0 0 0 30 0 0	

				rand Rad 2
				if Rad = 0
					NEW_LIFE_MON 205037 3 @X @Y 300 0 0 0 60 0 0			
				endif
				if Rad = 1
					NEW_LIFE_MON 205037 4 @X @Y 300 0 0 0 60 0 0		
				endif
				
		else
			add_sys_msg_start
			add_sys_msg 物品只可在风徊裂谷地图使用！
			set_sys_msg_mode 3 0 0
			msg_info
		
		endif
		

endproc  