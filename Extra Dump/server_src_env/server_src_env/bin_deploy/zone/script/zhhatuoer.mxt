proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b年轻的#Race#，你是否正为这片迷雾而困扰，是否正为未知的危险而忧心？别担心，我可以帮助你渡过一切灾厄！只要让我占据你的灵魂，你将变得无比强大！把你的灵魂交予我，$_palyer$，你将获得无与伦比的战斗力！
		
		strvar Loc3 Loc4
		get_pworld_var 11 8 1 Loc3 
		get_pworld_var 13 8 1 Loc4 
		if Loc3 = 0 and Loc4 = 0
			option 130 #签订灵魂契约
			option 131 #休想！我的灵魂是无价之宝！
		endif
	talk
				
endproc

proc 130
	
	strvar Loc3 Mid
	get_pworld_var 0 8 1 50
	var 50 1
	set_pworld_var 0 8 @50 1
	
	GET_MAP_NPC_ID 282019 Mid 1
	mon_speech @Mid 2 0 1 你做了个明智的选择，年轻人
	
	get_pworld_var 11 8 1 Loc3 
	var Loc3 1
	set_pworld_var 11 8 @Loc3 1
	
	effect 0 0 playEffct(5551,9508,"effect\\entity\\282013_18.ent")
	EFFECT_ID 0 0 setOverlayRegion("区域_4")
	
endproc 

proc 131
;	del_npc 282019 1
;	NEW_MON 282026 1 5551 9508 500 0 0 0 0
	
	strvar Loc4 Mid
	get_pworld_var 13 8 1 Loc4 
	var Loc4 1
	set_pworld_var 13 8 @Loc4 1
	
	
	GET_MAP_NPC_ID 282019 Mid 1
	mon_speech @Mid 2 0 1 愚蠢的#Race#，你将付出惨痛的代价！
	effect 0 0 playEffct(5551,9508,"effect\\entity\\9512_16.ent")
	
	
endproc 











