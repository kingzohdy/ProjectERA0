;神奇玩具屋副本单人模式

;创建副本事件
PROC 1
	;11记录时间
	SET_PWORLD_VAR 0 32 0 1
	;13召唤次数 最多7次
	SET_PWORLD_VAR 4 32 0 1
	;npc定时对白
	SET_PWORLD_VAR 8 32 0 1
	;npc对白   4秒说一次
	SET_PWORLD_VAR 12 32 0 1
	;对白结束标志
	SET_PWORLD_VAR 16 32 0 1
	
	add_sys_msg_start
	add_sys_msg 天空一声巨响，神奇的石头、剪刀、包袱闪亮登场！
	set_sys_msg_mode 4 0 0
	MAP_MSG_INFO 
ENDPROC

;关闭副本事件
proc 2
endproc

;副本定时事件（每2秒触发一次）
proc 3
	IS_PWORLD_OVER 30
	if @30 != 1
		GET_PWORLD_VAR 4 32 1 10
		if @10 >= 7
			PWORLD_PLAYER_EXP 0 1
		else
			GET_PWORLD_VAR 0 32 1 20
			if @20 >= 15
				SET_PWORLD_VAR 0 32 0 1
				rand 11 6
				strvar x1 y1 x2 y2 x3 y3
				switch 11
					case 0
						var x1 2872
						var y1 8340
						var x2 2699
						var y2 9395
						var x3 4076
						var y3 8860
					endcase

					case 1
						var x1 4751
						var y1 9962
						var x2 4809
						var y2 9171
						var x3 4203
						var y3 9420
					endcase

					case 2
						var x1 4942
						var y1 7991
						var x2 4336
						var y2 7979
						var x3 4643
						var y3 8248
					endcase
					
					case 3
						var x1 3514
						var y1 9850
						var x2 3112 
						var y2 9290
						var x3 3852 
						var y3 9148
					endcase
					
					case 4
						var x1 2872
						var y1 8340
						var x2 4643 
						var y2 8248
						var x3 3852 
						var y3 9148
					endcase
					
					case 5
						var x1 4076
						var y1 8860
						var x2 3112 
						var y2 9290
						var x3 4809 
						var y3 9171
					endcase
				endswitch
				NEW_MON 230052 1 @x1 @y1 0 0 0 0 0
				NEW_MON 230053 1 @x2 @y2 0 0 0 0 0
				NEW_MON 230054 1 @x3 @y3 0 0 0 0 0
				add 10 1
				SET_PWORLD_VAR 4 32 @10 1	
				add_sys_msg_start
				add_sys_msg 天空一声巨响，神奇的石头、剪刀、包袱闪亮登场！
				set_sys_msg_mode 4 0 0
				MAP_MSG_INFO 				
			else
				add 20 1
				SET_PWORLD_VAR 0 32 @20 1
			endif
		endif
		
		call 101
		
	endif	
endproc

;副本中怪物死亡事件
proc 4

endproc

;副本中角色进入事件
proc 5

endproc

;副本中角色死亡事件
proc 6

endproc

;副本中倒计时结束事件
proc 7
	
endproc 

;副本活动开始事件
proc 8
endproc 

proc 101
	;定时对白
		GET_PWORLD_VAR 12 32 1 60
		if @60 < 1
			add 60 1
			SET_PWORLD_VAR 12 32 @60 1
		else
			GET_PWORLD_VAR 16 32 1 70
			if @70 = 0
				GET_PWORLD_VAR 8 32 1 40
				if @40 < 6 
					add 40 1
					SET_PWORLD_VAR 8 32 @40 1
					call 100
				endif
			
				SET_PWORLD_VAR 12 32 0 1
			else
				return
			endif
		endif
endproc

proc 100
	strvar npcmid
	GET_MAP_NPC_ID  302161 npcmid 1
	GET_PWORLD_VAR 8 32 1 50
	if @50 = 1
		MON_SPEECH  @npcmid 0 2 1 欢迎来到神奇泡泡小屋，如果你想赢，就仔细听我说。
	elseif @50 = 2
		MON_SPEECH  @npcmid 0 2 1 每隔一段时间这里就会有新的泡泡出现，你要做的就是打破它！
	elseif @50 = 3
		MON_SPEECH  @npcmid 0 2 1 用技能击杀泡泡，使它们变成同一种颜色（同一种泡泡）
	elseif @50 = 4
		MON_SPEECH  @npcmid 0 2 1 当副本结束时，副本内的泡泡属于同一种，就是胜利了！
	elseif @50 = 5
		MON_SPEECH  @npcmid 0 2 1 当副本内有30只相同的泡泡时，副本提前结束！你也是胜利哟！
	elseif @50 = 6
		MON_SPEECH  @npcmid 0 2 1 现在，活动开始了，记住哟，每隔一段时间就会有新的泡泡出现…
	else
		SET_PWORLD_VAR 16 32 0 1
	endif
endproc
