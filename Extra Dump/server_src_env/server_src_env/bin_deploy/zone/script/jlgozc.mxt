
;精灵副本传送主城

proc 1
	choose 1 100 101 是否要离开当前副本？
endproc
proc 100
	
	COMPEL_CLOSE_PWORLD 1
;	OUT_PWORLD_BY_SINGLE
endproc
proc 101
	return

endproc

