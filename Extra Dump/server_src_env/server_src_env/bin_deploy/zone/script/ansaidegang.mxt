

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b阿拜多斯有什么可怕？他只拥有那小小的弹丸之地，廖廖追随者，而我们拥有黄沙万里，国民千千万，却被逼入绝境，这让人情何以堪！
	task_add_option 1553 1 101
	task_add_option 1554 1 102



	talk
endproc 


PROC 101
	task_do_option 1553 1
ENDPROC
PROC 102
	task_do_option 1554 1
ENDPROC



;proc 2
;	get_trig_task 1 2
;	if 2 = 1550
;		task_fini 1550 1
;	endif 
;endproc
