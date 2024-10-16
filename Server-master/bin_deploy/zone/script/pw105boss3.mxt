;70副本,3号boss
proc 1
	GET_PWORLD_VAR 40 32 1 20
        If 20 = 0
		dialog 1
		ADD_TALK $_npc$:
		ADD_TALK #b#b啊，陌生人，不管你们是误打误撞来到亚瑟的地盘，还是真的有点什么能耐，现在，你的命运由亚瑟安排！
		ADD_TALK #b#b让我想想，是把你们大卸八块再拼装好呢？还是用你们来制作玩具好呢？啊，用你们来检验我最新研制的终结者LW-1186再合适不过了……这是今天亚瑟做的最明智的决定！
		ADD_TALK #b#b我的地盘，我做主，你们没的选择，啊不，你们可以选择亚瑟为你们准备的测试工具。
		option 100 挑战
		talk
		
	endif
endproc
proc 100
	;boss3对话标记,0未触发过,1触发过
	GET_PWORLD_VAR 40 32 1 20
        If 20 = 0
                var 20 1
                set_pworld_var 40 32 @20 1
        endif
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b可别怪我下手不留情。
	talk
	DEL_NPC 317013 1
endproc
