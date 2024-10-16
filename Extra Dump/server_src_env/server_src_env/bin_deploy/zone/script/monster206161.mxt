;70副本,70副本boss5
proc 1
	strvar memid
	GET_MON_INFO 1 0 memid 0 0 0 0 0
	TARGET_ADD_STATUS 3 @memid 1095 1 25920000 0 
		
endproc
proc 4
	;boss6flagspeech39,boss6在无双死亡时说话
	var 39 0
	get_pworld_var 116 32 1 39
	if 39 = 1
		var 39 2
		strvar ret defid memid hp mapdef map x y
		GET_MON_INFO ret defid memid hp mapdef map x y
		
		MON_SPEECH @memid 1 2 3 啊——你们竟然杀死了我的无双，我要让你们付出代价！
		;boss6flagspeech39,boss6在无双死亡时说话
		
		set_pworld_var 116 32 @39 1
	endif
endproc
