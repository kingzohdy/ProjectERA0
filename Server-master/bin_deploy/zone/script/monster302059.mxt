
proc 1
	var 10 1
	set_mon_var 0 8 @10 1
	strvar Mid
	GET_MON_INFO 1 1 Mid 1 1 1 1 1
	MON_SPEECH @Mid 1 2 1 跟紧，你的任务是护送我前往悬空台，小心途中的试炼突袭兵，若我被击败则意味着试炼失败。
endproc
proc 2
		TASK_FAIL 3143 1
endproc 
proc 3
;巡逻结束
	DESTROY_MON 0
	new_npc 1 302055 26300 46400 0 zyzqzhanshi.mac
	strvar Mid
	GET_MON_INFO 1 1 Mid 1 1 1 1 1
	MON_SPEECH @Mid 1 2 1 我们到达目的地了，表现不错，但接下来你要面对更强大的阻碍了，准备好后就来找我吧。

endproc
proc 4
	strvar HP
	GET_MON_INFO 1 1 1 HP 1 1 1 1
	if HP < 500
		strvar Mid
		GET_MON_INFO 1 1 Mid 1 1 1 1 1
		MON_SPEECH @Mid 1 2 1 我快要坚持不住了！
	endif
endproc
	
proc 5
;巡逻点回调
;	get_mon_var 0 8 1 10
;	add 10 1
;	set_mon_var 0 8 @10 1
	
	strvar Num Mid
	GET_MON_INFO 0 0 Mid 0 0 0 0 0
	GET_MON_PATROLIDX @Mid Num 0
	
	if Num = 2
		MON_CALL_ENEMY_MON 205019 1 50 4 16300 43000 0 0 0 0 0 0 0
		rand 90 3
		switch 90
			case 0
				MON_SPEECH @Mid 1 2 1 保护我！
			endcase
			case 1
				MON_SPEECH @Mid 1 2 1 试炼突袭兵来了！
			endcase
			case 2
				MON_SPEECH @Mid 1 2 1 作好准备！
			endcase
								
		endswitch
	endif
	if Num = 3
		MON_CALL_ENEMY_MON 205019 1 50 4 20200 43600 0 0 0 0 0 0 0
		MON_CALL_ENEMY_MON 205019 1 50 4 20200 42900 0 0 0 0 0 0 0
		rand 90 3
		switch 90
			case 0
				MON_SPEECH @Mid 1 2 1 保护我！
			endcase
			case 1
				MON_SPEECH @Mid 1 2 1 试炼突袭兵来了！
			endcase
			case 2
				MON_SPEECH @Mid 1 2 1 作好准备！
			endcase
								
		endswitch		
		
	endif
	if Num = 4
		MON_CALL_ENEMY_MON 205019 1 50 4 26200 43800 0 0 0 0 0 0 0
		MON_CALL_ENEMY_MON 205019 1 50 4 26200 43000 0 0 0 0 0 0 0
		MON_CALL_ENEMY_MON 205019 1 50 4 26700 43500 0 0 0 0 0 0 0
		rand 90 3
		switch 90
			case 0
				MON_SPEECH @Mid 1 2 1 保护我！
			endcase
			case 1
				MON_SPEECH @Mid 1 2 1 试炼突袭兵来了！
			endcase
			case 2
				MON_SPEECH @Mid 1 2 1 作好准备！
			endcase
								
		endswitch
	endif
endproc
