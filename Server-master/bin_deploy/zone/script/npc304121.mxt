proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b玫尔还是一如既往的固执！无数次，我希望能说服她，却没有一次成功。既然如此，那我们就大开杀戒吧，杀光所有侵略者！
	add_talk #b#b#c6d00d2黑暗议会成员会从科研基地涌出，消灭他们！
	add_talk #b#b#c6d00d2最好在逃生通道上堵截侵略者。
	add_talk #b#b#cff0080保护玫尔，不要让她受到任何伤害！
	
	OPTION 100 #只要有我在谁也别想越雷池半步！为了玫尔！
	
	task_add_option 3317 1 101
	
	talk
endproc 

PROC 101
	task_do_option 3317 1
ENDPROC