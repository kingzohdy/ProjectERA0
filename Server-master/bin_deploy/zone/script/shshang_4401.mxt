
;生活商人
proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b您好，我是随船而来的供应商。
	add_talk #b#b相信我的小店肯定有你想要的工具跟材料，充足的准备能让您的寻宝更事办工倍。
	add_talk #b#b祝您能在寻宝秘境中满载而归。
	add_talk #b#b#c186506出售与“科技生产”相关的工具与材料#n

	
		OPTION 110 #打开商店


	talk
	
endproc 

proc 110

	OPEN_NPC_SHOP 
endproc 