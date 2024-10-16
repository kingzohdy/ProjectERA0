;屈圆npc

proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b你知道端午节的由来吗？
	add_talk #b#b你知道端午节为什么吃粽子吗？
	add_talk #b#b你知道粽子是谁发明的吗？

	
	TASK_ADD_OPTION 4486 0 103
	;TASKCYCLE_ADD_OPTION 139 1 120

	option 150 #(买卖)粽子材料商店
	;option 151 #返回
	
	talk
	
endproc 
proc 103
	TASK_DO_OPTION 4486 0
endproc
PROC 120	
	TASKCYCLE_DO_OPTION 139 1
ENDPROC

proc 150
	OPEN_NPC_SHOP 
endproc


