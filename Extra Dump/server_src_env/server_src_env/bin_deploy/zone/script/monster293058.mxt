;受困的首领

proc 1
	var 10 0
	set_mon_var 0 32 @10 1
	var 11 0
	set_mon_var 0 64 @11 1

endproc

proc 2
	get_mon_var 0 32 1 10
	add 10 1
	set_mon_var 0 32 @10 1

	ADD_SYS_MSG_START
	ADD_SYS_MSG 首领被成功解救。。。
	SET_SYS_MSG_MODE 4 0 0
	MAP_MSG_INFO

	NEW_NPC 1 304004 13913 6970 45 tgyongshi.mac
	;new_tmp_item 1440240 13394 7264 0 0 0 1

endproc
;proc 4
;	get_mon_var 0 32 1 10
;	get_mon_var 0 64 1 11
;	IF 10 = 1
;		ADD 11 1
;			ADD_SYS_MSG_START
;			ADD_SYS_MSG 11=$@11$
;			SET_SYS_MSG_MODE 1 0 0
;			MSG_INFO
;		IF 11 = 1
;
;			NEW_NPC 1 304004 14963 6143 45 tgyongshi.mac	
;		ENDIF
;		set_mon_var 0 64 @11 1
;
;	ENDIF
;	set_mon_var 0 32 @10 1
;
;endproc