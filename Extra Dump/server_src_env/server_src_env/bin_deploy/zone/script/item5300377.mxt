;筛子

proc 1
;	get_player_mapinfo 0 1 2 3 4
;
;	if 1 = 20200
;
;			dec_item_num_inpak 5300377 1 0
;			get_task_var 3003 1 8 1 13
;			var 13 1
;			set_task_var 3003 1 8 @13 1
;		
;			rand 50 8
;			add 50 100
;			switch 50
;
;				case 100
;					effect 0 0 playEffct(6700,5300,"effect\\entity\\9555_16.ent")
;					get_task_var 3003 3 8 1 15
;					var 15 1
;					set_task_var 3003 3 8 @15 1		
;					
;					
;				endcase
;				case 101
;					effect 0 0 playEffct(6700,5300,"effect\\entity\\9556_16.ent")
;					get_task_var 3003 3 8 1 15
;					var 15 2
;					set_task_var 3003 3 8 @15 1	
;					
;				endcase
;				case 102
;					effect 0 0 playEffct(6700,5300,"effect\\entity\\9557_16.ent")
;					get_task_var 3003 3 8 1 15
;					var 15 3
;					set_task_var 3003 3 8 @15 1	
;				endcase
;				case 103
;					effect 0 0 playEffct(6700,5300,"effect\\entity\\9558_16.ent")
;					get_task_var 3003 3 8 1 15
;					var 15 4
;					set_task_var 3003 3 8 @15 1	
;				endcase
;				
;				case 104
;					effect 0 0 playEffct(6700,5300,"effect\\entity\\9559_16.ent")
;					get_task_var 3003 3 8 1 15
;					var 15 5
;					set_task_var 3003 3 8 @15 1	
;				endcase
;				case 105
;					effect 0 0 playEffct(6700,5300,"effect\\entity\\9560_16.ent")
;					get_task_var 3003 3 8 1 15
;					var 15 6
;					set_task_var 3003 3 8 @15 1	
;				endcase
;				case 106
;					effect 0 0 playEffct(6700,5300,"effect\\entity\\9561_16.ent")
;					get_task_var 3003 3 8 1 15
;					var 15 7
;					set_task_var 3003 3 8 @15 1	
;				endcase
;				case 107
;					effect 0 0 playEffct(6700,5300,"effect\\entity\\9562_16.ent")
;					get_task_var 3003 3 8 1 15
;					var 15 8
;					set_task_var 3003 3 8 @15 1	
;				endcase
;
;			endswitch
;		
;	  else
;			add_sys_msg_start
;			add_sys_msg 不可以在本地图使用！
;			set_sys_msg_mode 3 0 0
;			msg_info
;	  endif
;		
;	
endproc

