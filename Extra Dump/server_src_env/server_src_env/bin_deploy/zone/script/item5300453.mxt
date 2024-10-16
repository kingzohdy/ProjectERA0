;相机


Proc 1
		
		strvar X Y
		GET_PLAYER_MAPINFO 0 0 0 X Y
		GET_PACK_SPACE 1 1
			if @1 > 0
				ADD_ITEM_NUM_INPAK 5300454 1 1 1
					if @1 = -1
						return
					else
						effect 2 0 playUiEffect(0,0,2,"effect\\entity\\9588_16.ent")	
	
						;effect 2 0 playEffct($@X$,$@Y$,"effect\\entity\\9588_16.ent")
						add_sys_msg_start
						add_sys_msg 咔嚓一声响，照片拍好了！
						set_sys_msg_mode 1 0 0
						msg_info
					endif
			endif
endproc			
			
				
				