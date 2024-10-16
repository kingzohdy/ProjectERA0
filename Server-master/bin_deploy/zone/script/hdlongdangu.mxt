

proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b一整天都在闲晃你都不会觉得无聊吗？要不要跟朋友来参加活动啊？
	add_talk #b#b
	add_talk #c186506特别提示：
	add_talk #b#b[45-64]级参加龙蛋谷，可获得大量经验值
	add_talk #b#b队伍人数越多，你所获得的经验越多
	add_talk #b#b队友等级越低，副本难度越低
	add_talk #b#b经验获得只与个人等级有关，跟队友等级无关#n
	add_talk #b#b
	add_talk #b#b#cff0080[65-80]级只能在龙蛋谷中获得10%经验
	add_talk #b#b完成[龙蛋谷-合作]任务可获得[晶石提炼器]#n


	
	task_add_option 3313 1 100
	task_add_option 3314 1 101
	task_add_option 3311 1 102
	task_add_option 3312 1 103
	task_add_option 3102 1 105
	
	
	task_add_option 3315 1 104
	
	OPTION 201 #进入龙蛋谷
	
;	option 202 #(买卖)声望商店
;	option 230 #进入大地之心
;	option 240 #播放
;    option 241 #提示
	
	talk
endproc 

proc 202
	OPEN_NPC_SHOP 
endproc


proc 230
	OPEN_UI 36
endproc
proc 240
	strvar X Y
	GET_PLAYER_MAPINFO 0 0 0 X Y
	effect 2 0 playUiEffect(0,0,0,"effect\\entity\\9577_16.ent")
	effect 2 0 playEffct($@X$,$@Y$,"effect\\entity\\9577_18.ent")	

;	effect 0 0 playEffct(31000,31800,"effect\\entity\\9544_16.ent")	

endproc

PROC 100
	task_do_option 3313 1
ENDPROC
PROC 101
	task_do_option 3314 1
ENDPROC
PROC 102
	task_do_option 3311 1
ENDPROC
PROC 103
	task_do_option 3312 1
ENDPROC
PROC 104
	task_do_option 3315 1
ENDPROC
PROC 105
	task_do_option 3102 1
ENDPROC


proc 201

;	ENTER_PWORLD 211 0 0
;		OPEN_PWORLD_UI 211
		OPEN_PWORLD_UI 13
endproc









