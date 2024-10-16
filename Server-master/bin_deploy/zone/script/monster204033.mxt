proc 1

;	GET_MON_INFO 1 2 30 31 32 33 0 0
;	TARGET_ADD_STATUS 3 @30 1006 2 4 1
	
	GET_MON_VAR 0 8 1 10
	var 10 0
	SET_MON_VAR 0 8 @10 1
	
	GET_MON_VAR 1 8 1 11
	var 11 0
	SET_MON_VAR 1 8 @11 1
	strvar Mid Rad
	GET_MON_INFO 1 2 Mid 31 32 33 0 0
	rand Rad 3
	if Rad = 0
		MON_SPEECH @Mid 3 1 1 从来没挑战者能赶上我的速度！	
	endif
	if Rad = 1
		MON_SPEECH @Mid 3 1 1 终点是我等待失败者的舞台！
	endif
	if Rad = 2
		MON_SPEECH @Mid 3 1 1 我渴望体验赛马失败的心情！
	endif	
	
endproc
proc 2

	MON_SPEECH @30 3 1 1 居然输给了你，以后我会再找你比赛的……	
	add_sys_msg_start
	add_sys_msg 你赢得了比赛的胜利！
	set_sys_msg_mode 3 0 0
	msg_info


endproc
proc 3 
;巡逻结束

;	GET_MON_VAR 0 8 1 10
;	var 10 1
;	SET_MON_VAR 0 8 @10 1
	
;	GET_MON_INFO 1 2 30 31 32 33 0 0
;	TARGET_ADD_STATUS 3 @30 1006 2 10 1	

;			MON_SPEECH @30 3 1 1 居然输给了你，以后我会再找你比赛的……		
;			DESTROY_MON 1	
			del_mon 1
endproc 
;PROC 4
;
;	GET_MON_VAR 0 8 1 10	
;	IF 10 = 1
;		GET_MON_VAR 1 8 1 11
;		ADD 11 1
;		SET_MON_VAR 1 8 @11 1
;		if 11 = 2
;			GET_MON_INFO 1 2 30 31 32 33 0 0	
;			
;		endif
;		if 11 = 3
;	
;		endif
;	ENDIF
;ENDPROC 
