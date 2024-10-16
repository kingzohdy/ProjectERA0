

proc 1

	dialog 1
	add_talk $_npc$:
	add_talk #b#b你们的运气不错,居然能够见到我!
	add_talk #b#b相见即是缘分,你们每人都可以从我这里领取一份随机礼物,包含稀有材料,紫色宠物!能得到什么,就看你们的人品了!

	OPTION 110 #哇咔咔,我来摸一下
	talk
endproc

proc 110
	PWORLD_BOX_AWARD 0
endproc 

















