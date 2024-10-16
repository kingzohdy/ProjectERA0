;巡逻守卫


proc 1
	
	strvar Rad1 Ret
	GET_ID_PLAYER_INFO 0 17 Ret
	if Ret = 0 or Ret = 1 or Ret = 3 or Ret = 4 or Ret = 5
		rand Rad1 4
		if Rad1 = 0
			mon_speech 0 0 0 1 城管不好当啊，每天都要巡逻站岗什么的，压力好大啊！
		elseif Rad1 = 1
			mon_speech 0 0 0 1 你看见犯罪份子了么？
		elseif Rad1 = 2
			mon_speech 0 0 0 1 想红名吗？那是自寻死路！
		else
			mon_speech 0 0 0 1 不要以为我在闲逛，其实我是有目的的！
		endif
	endif
		
		
	if Ret = 2
		strvar Rad2
		rand Rad2 4
		if Rad2 = 0
			mon_speech 0 0 0 1 攻击我？你这是犯罪！
		elseif Rad2 = 1
			mon_speech 0 0 0 1 年轻人要沉得住气，攻击城管的后果你懂的！
		elseif Rad2 = 2
			mon_speech 0 0 0 1 可以容忍你窥视我，但我不能容忍你攻击我！
		else
			mon_speech 0 0 0 1 誓与罪恶不两立！
		endif
	endif
	
endproc 	