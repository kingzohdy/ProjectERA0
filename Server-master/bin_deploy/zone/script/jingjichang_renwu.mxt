proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b我只接待勇士，只有真正的勇士才能得到大家的尊敬，同时我将把你们带到地狱，那里你们唯一能做的就是胜利，只有胜者才能生存下来，当然你们也可以选择逃避，离我远远的!
	
	TASK_ADD_OPTION 3306 0 150
	
	DIANFENG_IF_OPEN 30
	if @30 = 1
		OPTION 201 #末世巅峰争霸赛报名
		OPTION 200 末世巅峰争霸赛奖励领取
		OPTION 203 末世巅峰争霸赛雕像塑造
	endif
	
	talk
endproc 

proc 150
	TASK_DO_OPTION 3306 0
endproc

proc 200
	strvar flag
	;可以领取奖励不
	DIANFENG_AWARD_CHECK  11 flag
	if @11 = 1 and @flag = 3
		add_sys_msg_start
		add_sys_msg 末世巅峰竞技奖励已领取！
		set_sys_msg_mode 4 0 0
		msg_info
		return 
	elseif @11 = 1 and @flag = 1
		GET_ID_PLAYER_INFO 0 7 whichcareer	
		if @whichcareer = 17
		;守护者
			CHOOSE 1 202 1 恭喜您获得【守护】组冠军，奖励：#c00ffff金卷*100、末世奖章*4、10万绑银、独有称号、雕像树立权！#n
		elseif @whichcareer = 33
		;征服者
			CHOOSE 1 202 1 恭喜您获得【征服】组冠军，奖励：#c00ffff金卷*100、末世奖章*4、10万绑银、独有称号、雕像树立权！#n
		elseif @whichcareer = 18
		;聚能者
			CHOOSE 1 202 1 恭喜您获得【聚能】组冠军，奖励：#c00ffff金卷*100、末世奖章*4、10万绑银、独有称号、雕像树立权！#n
		elseif @whichcareer = 34
		;炼魂师
			CHOOSE 1 202 1 恭喜您获得【炼魂】组冠军，奖励：#c00ffff金卷*100、末世奖章*4、10万绑银、独有称号、雕像树立权！#n
		elseif @whichcareer = 19
		;猎杀者
			CHOOSE 1 202 1 恭喜您获得【猎杀】组冠军，奖励：#c00ffff金卷*100、末世奖章*4、10万绑银、独有称号、雕像树立权！#n
		elseif @whichcareer = 35
		;毁灭者
			CHOOSE 1 202 1 恭喜您获得【毁灭】组冠军，奖励：#c00ffff金卷*100、末世奖章*4、10万绑银、独有称号、雕像树立权！#n
		elseif @whichcareer = 20
		;圣佑者
			CHOOSE 1 202 1 恭喜您获得【圣佑】组冠军，奖励：#c00ffff金卷*100、末世奖章*4、10万绑银、独有称号、雕像树立权！#n
		elseif @whichcareer = 36
		;驭雷师
			CHOOSE 1 202 1 恭喜您获得【驭雷】组冠军，奖励：#c00ffff金卷*100、末世奖章*4、10万绑银、独有称号、雕像树立权！#n
		else
			return 
		endif
	elseif @11 = 1 and @flag = 2
		CHOOSE 1 202 1 感谢您对末世巅峰争霸赛的支持，奖励：#c00ffff金卷*100、末世奖章*4、10万绑银！#n
	else
		add_sys_msg_start
		add_sys_msg 非常遗憾您并未参与末世巅峰竞技，没有可领取奖励！
		set_sys_msg_mode 4 0 0
		msg_info
		return 
	endif
endproc

proc 201
	DIANFENG_FIRST_INFO 0
endproc

proc 202
	strvar retflag
	;可以领取奖励不
	DIANFENG_AWARD 11 retflag
	if @11 = 1 and @retflag = 1
	;冠军，程序中直接给了
		return
	elseif @11 = 1 and @retflag = 2
	;参与者,程序中直接给
		return 
	elseif @11 = 1 and @retflag = 3
		add_sys_msg_start
		add_sys_msg 末世巅峰竞技奖励已领取！
		set_sys_msg_mode 4 0 0
		msg_info
		return 
	else
		add_sys_msg_start
		add_sys_msg 非常遗憾您并未参与末世巅峰竞技，没有可领取奖励！
		set_sys_msg_mode 4 0 0
		msg_info
		return 
	endif
endproc

proc 203
	strvar retflag
	;检查可雕像的条件
	DIANFENG_FRONT_CHAMPION 10 retflag
	if @10 = 1 and @retflag = 1
		CHOOSE 1 204 1 雕塑需要支付10000银币，是否继续？
	else
		add_sys_msg_start
		add_sys_msg 只有末世巅峰各职业组冠军才能进行雕像塑造，你还没有让我帮你塑造的权利，证明了后再来找我吧！
		set_sys_msg_mode 4 0 0
		msg_info
		return 
	endif
endproc

proc 204
	strvar whichcareer PosX PosY
	DEC_MONEY_TEST 0 10000 20 30
	if @30 = 0
		DEC_MONEY 0 10000 20 40
		if @40 = 0
			GET_ID_PLAYER_INFO 0 7 whichcareer	
			
			if @whichcareer = 17
			;守护者
				var PosX 25482
				var PosY 20909
			elseif @whichcareer = 33
			;征服者
				var PosX 27660
				var PosY 20909
			elseif @whichcareer = 18
			;聚能者
				var PosX 25482
				var PosY 18867
			elseif @whichcareer = 34
			;炼魂师
				var PosX 27660
				var PosY 18868
			elseif @whichcareer = 19
			;猎杀者
				var PosX 27658
				var PosY 19859
			elseif @whichcareer = 35
			;毁灭者
				var PosX 25482
				var PosY 19858
			elseif @whichcareer = 20
			;圣佑者
				var PosX 25482
				var PosY 17813
			elseif @whichcareer = 36
			;驭雷师
				var PosX 27662
				var PosY 17818
			else
				return 
			endif
			
			;设置雕像
			DIANFENG_SET_EFFIGY 1 1000 @PosX @PosY @whichcareer
		endif
	else
		add_sys_msg_start
		add_sys_msg 你的银币不足，无法雕塑
		set_sys_msg_mode 4 0 0
		msg_info
		return 
	endif
endproc



