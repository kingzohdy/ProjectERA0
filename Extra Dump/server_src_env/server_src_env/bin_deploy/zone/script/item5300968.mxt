proc 1
	strvar ret num ret2 mapID
	GET_POSITEM_INFO ret mapID X Y
	
	if ret = 0 and mapID = 0
		rand num 16
		switch num
			case 0
				SET_POSITEM_INFO 0 2100 10500 14800			
			endcase
			case 1
				SET_POSITEM_INFO 0 2100 15900 8900
			endcase
			case 2
				SET_POSITEM_INFO 0 2202 32400 15900
			endcase
			case 3
				SET_POSITEM_INFO 0 2202 38400 32500
			endcase
			case 4
				SET_POSITEM_INFO 0 2202 32500 30000
			endcase
			case 5
				SET_POSITEM_INFO 0 2200 17800 20100
			endcase
			case 6
				SET_POSITEM_INFO 0 2200 20600 7800
			endcase
			case 7
				SET_POSITEM_INFO 0 2200 12800 7600
			endcase
			case 8
				SET_POSITEM_INFO 0 2200 12000 13500
			endcase
			case 9
				SET_POSITEM_INFO 0 2300 17300 8000
			endcase
			case 10
				SET_POSITEM_INFO 0 2300 12700 10300
			endcase
			case 11
				SET_POSITEM_INFO 0 2300 11700 13100
			endcase
			case 12
				SET_POSITEM_INFO 0 2300 18400 15700
			endcase
			case 13
				SET_POSITEM_INFO 0 2400 12500 13200
			endcase
			case 14
				SET_POSITEM_INFO 0 2400 6700 16000
			endcase
			case 15
				SET_POSITEM_INFO 0 2400 7900 5700
			endcase
			case 16
				SET_POSITEM_INFO 0 2400 3700 4500
			endcase
		endswitch
	endif
	
	GET_POSITEM_INFO ret mapID X Y
	if ret = 0 and mapID != 0
	
		POSITEM_DIR 500 ret2 ori
		switch ret2
			case 0
				call 200
			endcase
			case 1
				call 201
			endcase
			case 2
				call 202
			endcase
			case 3
				call 203
			endcase
			case 4
				call 204
			endcase
		endswitch
	endif
endproc

proc 200
	strvar ret mapID X Y PlyMid MonMid LV
	GET_POSITEM_INFO ret mapID X Y
	GET_PLAYER_BASE_ATT 0 0 LV 0
	
	if ret = 0
	dec_item_num_inpak 5300968 1 0
		if mapID = 2100 and X = 10500 and Y = 14800
			GET_PLAYER_ID 1 PlyMid 
			del_mon 1		
			
			get_task_var 4612 0 16 0 10
			NEW_LIFE_DYN_MON 1 254025 1 10500 14800 500 0 0 MonMid 0 @LV 0 600
			var 10 @MonMid
			set_task_var 4612 0 16 @10 0
			; CALL_MON 1 254025 500 600 10500 14800 MonMid
						
			SET_MON_OWNER 0 @MonMid
			set_mon_var 0 32 @PlyMid 1
		elseif mapID = 2100 and X = 15900 and Y = 8900 
			GET_PLAYER_ID 1 PlyMid 
			del_mon 1		
			
			get_task_var 4612 0 16 0 10
			NEW_LIFE_DYN_MON 1 254025 1 15900 8900 500 0 0 MonMid 0 @LV 0 600
			var 10 @MonMid
			set_task_var 4612 0 16 @10 0
			; CALL_MON 1 254025 500 600 15900 8900 MonMid
						
			SET_MON_OWNER 0 @MonMid
			set_mon_var 0 32 @PlyMid 1
		elseif mapID = 2202 and X = 32400 and Y = 15900
			GET_PLAYER_ID 1 PlyMid 
			del_mon 1		
			
			get_task_var 4612 0 16 0 10
			NEW_LIFE_DYN_MON 1 254025 1 32400 15900 500 0 0 MonMid 0 @LV 0 600
			var 10 @MonMid
			set_task_var 4612 0 16 @10 0
			; CALL_MON 1 254025 500 600 32400 15900 MonMid
						
			SET_MON_OWNER 0 @MonMid
			set_mon_var 0 32 @PlyMid 1
		elseif mapID = 2202 and X = 38400 and Y = 32500
			GET_PLAYER_ID 1 PlyMid 
			del_mon 1		
			
			get_task_var 4612 0 16 0 10
			NEW_LIFE_DYN_MON 1 254025 1 38400 32500 500 0 0 MonMid 0 @LV 0 600
			var 10 @MonMid
			set_task_var 4612 0 16 @10 0
			; CALL_MON 1 254025 500 600 38400 32500 MonMid
						
			SET_MON_OWNER 0 @MonMid
			set_mon_var 0 32 @PlyMid 1
		elseif mapID = 2202 and X = 32500 and Y = 30000
			GET_PLAYER_ID 1 PlyMid 
			del_mon 1		
			
			get_task_var 4612 0 16 0 10
			NEW_LIFE_DYN_MON 1 254025 1 32500 30000 500 0 0 MonMid 0 @LV 0 600
			var 10 @MonMid
			set_task_var 4612 0 16 @10 0
			; CALL_MON 1 254025 500 600 32500 30000 MonMid
						
			SET_MON_OWNER 0 @MonMid
			set_mon_var 0 32 @PlyMid 1
		elseif mapID = 2200 and X = 17800 and Y = 20100
			GET_PLAYER_ID 1 PlyMid 
			del_mon 1
			
			get_task_var 4612 0 16 0 10
			NEW_LIFE_DYN_MON 1 254025 1 17800 20100 500 0 0 MonMid 0 @LV 0 600
			var 10 @MonMid
			set_task_var 4612 0 16 @10 0
			; CALL_MON 1 254025 500 600 17800 20100 MonMid
						
			SET_MON_OWNER 0 @MonMid
			set_mon_var 0 32 @PlyMid 1
		elseif mapID = 2200 and X = 20600 and Y = 7800
			GET_PLAYER_ID 1 PlyMid 
			del_mon 1
			
			get_task_var 4612 0 16 0 10
			NEW_LIFE_DYN_MON 1 254025 1 20600 7800 500 0 0 MonMid 0 @LV 0 600
			var 10 @MonMid
			set_task_var 4612 0 16 @10 0
			; CALL_MON 1 254025 500 600 20600 7800 MonMid
						
			SET_MON_OWNER 0 @MonMid
			set_mon_var 0 32 @PlyMid 1
		elseif mapID = 2200 and X = 12800 and Y = 7600
			GET_PLAYER_ID 1 PlyMid 
			del_mon 1
			
			get_task_var 4612 0 16 0 10
			NEW_LIFE_DYN_MON 1 254025 1 12800 7600 500 0 0 MonMid 0 @LV 0 600
			var 10 @MonMid
			set_task_var 4612 0 16 @10 0
			; CALL_MON 1 254025 500 600 12800 7600 MonMid
						
			SET_MON_OWNER 0 @MonMid
			set_mon_var 0 32 @PlyMid 1
		elseif mapID = 2200 and X = 12000 and Y = 13500
			GET_PLAYER_ID 1 PlyMid 
			del_mon 1
			
			get_task_var 4612 0 16 0 10
			NEW_LIFE_DYN_MON 1 254025 1 12000 13500 500 0 0 MonMid 0 @LV 0 600
			var 10 @MonMid
			set_task_var 4612 0 16 @10 0
			; CALL_MON 1 254025 500 600 12000 13500 MonMid
						
			SET_MON_OWNER 0 @MonMid
			set_mon_var 0 32 @PlyMid 1
		elseif mapID = 2300 and X = 17300 and Y = 8000
			GET_PLAYER_ID 1 PlyMid 
			del_mon 1
			
			get_task_var 4612 0 16 0 10
			NEW_LIFE_DYN_MON 1 254025 1 17300 8000 500 0 0 MonMid 0 @LV 0 600
			var 10 @MonMid
			set_task_var 4612 0 16 @10 0
			; CALL_MON 1 254025 500 600 17300 8000 MonMid
						
			SET_MON_OWNER 0 @MonMid
			set_mon_var 0 32 @PlyMid 1
		elseif mapID = 2300 and X = 12700 and Y = 10300
			GET_PLAYER_ID 1 PlyMid 
			del_mon 1
			
			get_task_var 4612 0 16 0 10
			NEW_LIFE_DYN_MON 1 254025 1 12700 10300 500 0 0 MonMid 0 @LV 0 600
			var 10 @MonMid
			set_task_var 4612 0 16 @10 0
			; CALL_MON 1 254025 500 600 12700 10300 MonMid
						
			SET_MON_OWNER 0 @MonMid
			set_mon_var 0 32 @PlyMid 1
		elseif mapID = 2300 and X = 11700 and Y = 13100
			GET_PLAYER_ID 1 PlyMid 
			del_mon 1
			
			get_task_var 4612 0 16 0 10
			NEW_LIFE_DYN_MON 1 254025 1 11700 13100 500 0 0 MonMid 0 @LV 0 600
			var 10 @MonMid
			set_task_var 4612 0 16 @10 0
			; CALL_MON 1 254025 500 600 11700 13100 MonMid
						
			SET_MON_OWNER 0 @MonMid
			set_mon_var 0 32 @PlyMid 1
		elseif mapID = 2300 and X = 18400 and Y = 15700
			GET_PLAYER_ID 1 PlyMid 
			del_mon 1
			
			get_task_var 4612 0 16 0 10
			NEW_LIFE_DYN_MON 1 254025 1 18400 15700 500 0 0 MonMid 0 @LV 0 600
			var 10 @MonMid
			set_task_var 4612 0 16 @10 0
			; CALL_MON 1 254025 500 600 18400 15700 MonMid
						
			SET_MON_OWNER 0 @MonMid
			set_mon_var 0 32 @PlyMid 1
		elseif mapID = 2400 and X = 12500 and Y = 13200
			GET_PLAYER_ID 1 PlyMid 
			del_mon 1
			
			get_task_var 4612 0 16 0 10
			NEW_LIFE_DYN_MON 1 254025 1 12500 13200 500 0 0 MonMid 0 @LV 0 600
			var 10 @MonMid
			set_task_var 4612 0 16 @10 0
			; CALL_MON 1 254025 500 600 12500 13200 MonMid
						
			SET_MON_OWNER 0 @MonMid
			set_mon_var 0 32 @PlyMid 1
		elseif mapID = 2400 and X = 6700 and Y = 16000
			GET_PLAYER_ID 1 PlyMid 
			del_mon 1
			
			get_task_var 4612 0 16 0 10
			NEW_LIFE_DYN_MON 1 254025 1 6700 16000 500 0 0 MonMid 0 @LV 0 600
			var 10 @MonMid
			set_task_var 4612 0 16 @10 0
			; CALL_MON 1 254025 500 600 6700 16000 MonMid
						
			SET_MON_OWNER 0 @MonMid
			set_mon_var 0 32 @PlyMid 1
		elseif mapID = 2400 and X = 7900 and Y = 5700
			GET_PLAYER_ID 1 PlyMid 
			del_mon 1
			
			get_task_var 4612 0 16 0 10
			NEW_LIFE_DYN_MON 1 254025 1 7900 5700 500 0 0 MonMid 0 @LV 0 600
			var 10 @MonMid
			set_task_var 4612 0 16 @10 0
			; CALL_MON 1 254025 500 600 7900 5700 MonMid
						
			SET_MON_OWNER 0 @MonMid
			set_mon_var 0 32 @PlyMid 1
		elseif mapID = 2400 and X = 3700 and Y = 4500
			GET_PLAYER_ID 1 PlyMid 
			del_mon 1
			
			get_task_var 4612 0 16 0 10
			NEW_LIFE_DYN_MON 1 254025 1 3700 4500 500 0 0 MonMid 0 @LV 0 600
			var 10 @MonMid
			set_task_var 4612 0 16 @10 0
			; CALL_MON 1 254025 500 600 3700 4500 MonMid
						
			SET_MON_OWNER 0 @MonMid
			set_mon_var 0 32 @PlyMid 1
		else
		endif
			
			add_sys_msg_start
			add_sys_msg 经过不懈的搜寻，终于揪出强盗之王萨尔瓦托！
			set_sys_msg_mode 4 0 0
			msg_info
	endif
endproc

proc 201
	strvar ret mapID
	GET_POSITEM_INFO ret mapID X Y
	
	if ret = 0
		add_sys_msg_start
		if mapID = 2100
			add_sys_msg 红外追踪器感应到目标出现在：细语丘陵
		elseif mapID = 2202
			add_sys_msg 红外追踪器感应到目标出现在：冰岛
		elseif mapID = 2200
			add_sys_msg 红外追踪器感应到目标出现在：银风雪原
		elseif mapID = 2300
			add_sys_msg 红外追踪器感应到目标出现在：流金沙漠
		elseif mapID = 2400
			add_sys_msg 红外追踪器感应到目标出现在：落日荒野
		else
		endif
		set_sys_msg_mode 4 0 0
		msg_info
	endif
	
endproc

proc 202

	strvar ret ori
	GET_POSITEM_INFO ret mapID X Y
	POSITEM_DIR 500 0 ori
	
	if ret = 0
		add_sys_msg_start
		switch ori
			case 1
			add_sys_msg 距离追踪地点只有一步之遥,就在你的北方附近
			endcase
			case 2
			add_sys_msg 距离追踪地点只有一步之遥,就在你的东北方附近
			endcase
			case 3
			add_sys_msg 距离追踪地点只有一步之遥,就在你的东方附近
			endcase
			case 4
			add_sys_msg 距离追踪地点只有一步之遥,就在你的东南方附近
			endcase
			case 5
			add_sys_msg 距离追踪地点只有一步之遥,就在你的南方附近
			endcase
			case 6
			add_sys_msg 距离追踪地点只有一步之遥,就在你的西南方附近
			endcase
			case 7
			add_sys_msg 距离追踪地点只有一步之遥,就在你的西方附近
			endcase
			case 8
			add_sys_msg 距离追踪地点只有一步之遥,就在你的西北方附近
			endcase
		endswitch
		set_sys_msg_mode 4 0 0
		msg_info
	endif
endproc

proc 203

	strvar ret ori
	GET_POSITEM_INFO ret mapID X Y
	
	if ret = 0
		POSITEM_DIR 500 0 ori
		
		add_sys_msg_start
		switch ori
			case 1
			add_sys_msg 追踪地点不远了，就在你的北方
			endcase
			case 2
			add_sys_msg 追踪地点不远了,就在你的东北方
			endcase
			case 3
			add_sys_msg 追踪地点不远了,就在你的东方
			endcase
			case 4
			add_sys_msg 追踪地点不远了,就在你的东南方
			endcase
			case 5
			add_sys_msg 追踪地点不远了,就在你的南方
			endcase
			case 6
			add_sys_msg 追踪地点不远了，就在你的西南方
			endcase
			case 7
			add_sys_msg 追踪地点不远了,就在你的西方
			endcase
			case 8
			add_sys_msg 追踪地点不远了,就在你的西北方
			endcase
		endswitch
		set_sys_msg_mode 4 0 0
		msg_info
	endif
endproc

proc 204

	strvar ret ori
	GET_POSITEM_INFO ret mapID X Y
	
	if ret = 0
		POSITEM_DIR 500 0 ori
		
		add_sys_msg_start
		switch ori
			case 1
			add_sys_msg 追踪地点距此处还有些距离,在你的北方
			endcase
			case 2
			add_sys_msg 追踪地点距此处还有些距离,在你的东北方
			endcase
			case 3
			add_sys_msg 追踪地点距此处还有些距离,在你的东方
			endcase
			case 4
			add_sys_msg 追踪地点距此处还有些距离,在你的东南方
			endcase
			case 5
			add_sys_msg 追踪地点距此处还有些距离,在你的南方
			endcase
			case 6
			add_sys_msg 追踪地点距此处还有些距离,在你的西南方
			endcase
			case 7
			add_sys_msg 追踪地点距此处还有些距离,在你的西方
			endcase
			case 8
			add_sys_msg 追踪地点距此处还有些距离,在你的西北方
			endcase
		endswitch
		set_sys_msg_mode 4 0 0
		msg_info
	endif
endproc
