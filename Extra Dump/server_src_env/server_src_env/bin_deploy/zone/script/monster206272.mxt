;70副本,boss1脚本
;70副本1号boss,办熊办机械
proc 1
	strvar memid
	GET_MON_INFO 1 0 memid 0 0 0 0 0
	TARGET_ADD_STATUS 3 @memid 1095 1 25920000 0 
		
endproc
;死亡脚本
proc 2
	;删阻挡
	delete_mon_by_defid 261039 1
	;清怪
	delete_mon_by_defid 206264 1
	delete_mon_by_defid 206266 1
	delete_mon_by_defid 206268 1
	;boss6flagspeech39,boss6在无双死亡时说话
	var 39 1
	set_pworld_var 116 32 @39 1
endproc
