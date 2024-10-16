
;


proc 1
	strvar Mid
	GET_MON_INFO 1 1 Mid 1 1 1 1 1

	MON_SPEECH @Mid 1 1 1 打我啊，打我啊！按Q键上方的数字键“1”“2”用技能打我啊！

endproc
proc 2
	GET_MON_INFO 1 1 Mid 1 1 1 1 1
	MON_SPEECH @Mid 1 1 1 我竟然败了，你把我俩都打败之后地上就会出现一把火焰刀！
endproc


