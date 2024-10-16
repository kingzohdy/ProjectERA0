;;;
;l;
proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b铁翼军团击溃了我们，使我们孤立无援！天知道若是没有自治联盟的援手，我们会怎么样！虽然现在我们依旧无法摆脱铁翼军团的侵袭，但生意还得进行，不是吗，$_player$？

		task_add_option 1851 1 100
		task_add_option 1852 1 101
		task_add_option 1853 1 102
		task_add_option 1864 1 103
		
		option 200 #(买卖)打开商店
	talk

endproc

proc 200
	OPEN_NPC_SHOP 
endproc

PROC 100
	task_do_option 1851 1
ENDPROC
PROC 101
	task_do_option 1852 1
ENDPROC
PROC 102
	task_do_option 1853 1
ENDPROC

PROC 103
	task_do_option 1864 1
ENDPROC

