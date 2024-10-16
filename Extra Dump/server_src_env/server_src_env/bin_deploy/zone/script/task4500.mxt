;任务4500 

;接任务
proc 1
endproc 

;完成任务
proc 2
	DEL_ROLE_TMP_ITEM 0 0
endproc

;失败
proc 4
	DEL_ROLE_TMP_ITEM 0 0
endproc

;丢弃
proc 5
	DEL_ROLE_TMP_ITEM 0 0
endproc
