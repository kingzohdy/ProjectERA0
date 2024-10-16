

proc 1


	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b嗨，我是这个城市的据点行政官，我可以办理如下业务：
	add_talk #b#b战前准备：是的，你没有看错，竞拍就是由我组织的， 你只有在竞拍中胜出，才能角逐真正的据点占领权。
	add_talk #b#b进入战争：又是我，战争在每周二晚上20 ：30分开启，你可以通过我进入战场，是雄还是雌，就看 这一遭了
	add_talk #b#b胜出奖励：什么部门油水最大？当然是搞后勤的了， 当你占领了这个据点以后，所有的奖励都是由我发放，所 以赶快贿赂贿赂我吧


	strvar map
	GET_PLAYER_MAPINFO 1 map 0 0 0
	strvar op1 op2 op3 op4 op5 op6
	
	STRONG_IS_OPEN 1 2202 op1
	STRONG_IS_OPEN 1 2200 op2
	STRONG_IS_OPEN 1 2300 op3
	STRONG_IS_OPEN 1 2400 op4
	STRONG_IS_OPEN 1 2500 op5
	STRONG_IS_OPEN 1 3404 op6

	strong_is_mine 1 2202 10
	strong_is_mine 1 2200 20
	strong_is_mine 1 2300 30
	strong_is_mine 1 2400 40
	strong_is_mine 1 2500 50
	strong_is_mine 1 3404 60	
		if map = 2202 and op1 = 1
			option 200 竞拍
			option 210 进入据点战场
			; option 220 领取占领奖励
			if 10 = 1
				option 400 放弃据点
			endif
		endif
		if map = 2200 and op2 = 1
			option 200 竞拍
			option 210 进入据点战场
			; option 220 领取占领奖励
			if 20 = 1
				option 400 放弃据点
			endif
		endif
		if map = 2300 and op3 = 1
			option 200 竞拍
			option 210 进入据点战场
			; option 220 领取占领奖励
			if 30 = 1
				option 400 放弃据点
			endif
		endif
		if map = 2400 and op4 = 1
			option 200 竞拍
			option 210 进入据点战场
			; option 220 领取占领奖励
			if 40 = 1
				option 400 放弃据点
			endif
		
		endif
		if map = 2500 and op5 = 1
			option 200 竞拍
			option 210 进入据点战场
			; option 220 领取占领奖励
			if 50 = 1
				option 400 放弃据点
			endif
		
		endif
		if map = 3404 and op6 = 1
			option 200 竞拍
			option 210 进入据点战场
			; option 220 领取占领奖励
			if 60 = 1
				option 400 放弃据点
			endif
		endif

		option 500 公会传奇
	talk
endproc 
proc 200
	strvar map
	GET_PLAYER_MAPINFO 1 map 0 0 0
	if map = 2202
		strong_sale_info 1 2202
	endif
	if map = 2200
		strong_sale_info 1 2200
	endif
	if map = 2300
		strong_sale_info 1 2300
	endif
	if map = 2400
		strong_sale_info 1 2400
	endif
	if map = 2500
		strong_sale_info 1 2500
	endif
	if map = 3404
		strong_sale_info 1 3404
	endif


endproc
proc 210
	strvar map
	GET_PLAYER_MAPINFO 1 map 0 0 0
	if map = 2202
		IS_FIRST_STRONG 20 2202
		if 20 = 1
			choose 2 211 301 进入当前据点战场后，本日不可再进入其他据点战场，是否确认前往当前战场？
		else
			call 211
		endif
	endif
	if map = 2200
		IS_FIRST_STRONG 21 2200
		if 21 = 1
			choose 2 211 301 进入当前据点战场后，本日不可再进入其他据点战场，是否确认前往当前战场？
		else
			call 211
		endif
	endif
	if map = 2300
		IS_FIRST_STRONG 22 2300
		if 22 = 1
			choose 2 211 301 进入当前据点战场后，本日不可再进入其他据点战场，是否确认前往当前战场？
		else
			call 211
		endif
	endif
	if map = 2400
		IS_FIRST_STRONG 23 2400
		if 23 = 1
			choose 2 211 301 进入当前据点战场后，本日不可再进入其他据点战场，是否确认前往当前战场？
		else
			call 211
		endif
	endif
	if map = 2500
		IS_FIRST_STRONG 24 2500
		if 24 = 1
			choose 2 211 301 进入当前据点战场后，本日不可再进入其他据点战场，是否确认前往当前战场？
		else
			call 211
		endif
	endif
	if map = 3404
		IS_FIRST_STRONG 25 3404
		if 25 = 1
			choose 2 211 301 进入当前据点战场后，本日不可再进入其他据点战场，是否确认前往当前战场？
		else
			call 211
		endif
	endif
endproc
proc 301
endproc
proc 211
	strvar map
	GET_PLAYER_MAPINFO 1 map 0 0 0
	if map = 2202
		STRONG_ENTER 1 2202
	endif
	if map = 2200
		strong_enter 1 2200
	endif
	if map = 2300
		strong_enter 1 2300
	endif
	if map = 2400
		strong_enter 1 2400
	endif
	if map = 2500
		strong_enter 1 2500
	endif
	if map = 3404
		strong_enter 1 3404
	endif

endproc


proc 220
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b人生最忌讳的就是贪得无厌，七宗罪之一啊。所以在 我这里，所有的奖励一天只能领取一次，注意！节制啊！ ！

		option 300 领取占领资金
	;	option 310 领取经验
		option 320 领取增益状态
		option 330 领取公会贡献度

	talk
endproc

proc 300
	strvar map
	GET_PLAYER_MAPINFO 1 map 0 0 0
	if map = 2202
		STRONG_PLAYER_AWARD 1 2202 1
	endif
	if map = 2200
		STRONG_PLAYER_AWARD 1 2200 1
	endif
	if map = 2300
		STRONG_PLAYER_AWARD 1 2300 1
	endif
	if map = 2400
		STRONG_PLAYER_AWARD 1 2400 1
	endif
	if map = 2500
		STRONG_PLAYER_AWARD 1 2500 1
	endif
	if map = 3404
		STRONG_PLAYER_AWARD 1 3404 1
	endif

endproc

proc 310
	strvar map
	GET_PLAYER_MAPINFO 1 map 0 0 0
	if map = 2202
		STRONG_PLAYER_AWARD 1 2202 2
	endif
	if map = 2200
		STRONG_PLAYER_AWARD 1 2200 2
	endif
	if map = 2300
		STRONG_PLAYER_AWARD 1 2300 2
	endif
	if map = 2400
		STRONG_PLAYER_AWARD 1 2400 2
	endif
	if map = 2500
		STRONG_PLAYER_AWARD 1 2500 2
	endif
	if map = 3404
		STRONG_PLAYER_AWARD 1 3404 2
	endif

endproc

proc 320
	strvar map
	GET_PLAYER_MAPINFO 1 map 0 0 0
	if map = 2202
		STRONG_PLAYER_AWARD 1 2202 3
	endif
	if map = 2200
		STRONG_PLAYER_AWARD 1 2200 3
	endif
	if map = 2300
		STRONG_PLAYER_AWARD 1 2300 3
	endif
	if map = 2400
		STRONG_PLAYER_AWARD 1 2400 3
	endif
	if map = 2500
		STRONG_PLAYER_AWARD 1 2500 3
	endif
	if map = 3404
		STRONG_PLAYER_AWARD 1 3404 3
	endif

endproc

proc 330
	strvar map
	GET_PLAYER_MAPINFO 1 map 0 0 0
	if map = 2202
		STRONG_PLAYER_AWARD 1 2202 4
	endif
	if map = 2200
		STRONG_PLAYER_AWARD 1 2200 4
	endif
	if map = 2300
		STRONG_PLAYER_AWARD 1 2300 4
	endif
	if map = 2400
		STRONG_PLAYER_AWARD 1 2400 4
	endif
	if map = 2500
		STRONG_PLAYER_AWARD 1 2500 4
	endif
	if map = 3404
		STRONG_PLAYER_AWARD 1 3404 4
	endif

endproc

proc 400
	strvar map
	GET_PLAYER_MAPINFO 1 map 0 0 0
	if map = 2202
		choose 2 401 402 是否确认放弃[冰岛]据点占领权 
	endif
	if map = 2200
		choose 2 401 402 是否确认放弃[银风雪原]据点占领权 
	endif
	if map = 2300
		choose 2 401 402 是否确认放弃[流金沙漠]据点占领权 
	endif
	if map = 2400
		choose 2 401 402 是否确认放弃[落日荒野]据点占领权 
	endif
	if map = 2500
		choose 2 401 402 是否确认放弃[越雾雨林]据点占领权 
	endif
	if map = 3404
		choose 2 401 402 是否确认放弃[鸣沙陷地]据点占领权 
	endif	
endproc 
proc 402
	return
endproc 
proc 401
	strvar map
	GET_PLAYER_MAPINFO 1 map 0 0 0
	if map = 2202
		STRONG_CANCEL 1 2202 
	endif
	if map = 2200
		STRONG_CANCEL 1 2200
	endif
	if map = 2300
		STRONG_CANCEL 1 2300
	endif
	if map = 2400
		STRONG_CANCEL 1 2400
	endif
	if map = 2500
		STRONG_CANCEL 1 2500
	endif
	if map = 3404
		STRONG_CANCEL 1 3404
	endif	
endproc


proc 500
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b在零纪元元年前的那次旷世大战中，出现了六个公会，他们带领着天行者一次一次从战火中凯旋而归，从此便流下一个传承。
	add_talk #b#b时至今日，经历了历史的交替，能够登上这六个宝座的公会，已然成为最强的六个象征，成为每位勇士心中的“瓦尔哈拉”！
	add_talk #b#b但强者的世界容不下二者，为了成为最强中的最强，逐渐演化出了军阀，领主，城主的实力划分，而最后只能有一个公会登上霸主之位。
	add_talk #b#b
	add_talk #b#b#c186506注释：瓦尔哈拉为北欧神话中为了迎接“诸神的黄昏”之战，奥丁为阵亡英雄安置的宫殿
	
		option 501 我想了解“军阀”公会
		option 510 我想了解“领主”公会
		option 520 我想了解“城主”公会
		option 530 我想了解“霸主”公会
	
	talk
endproc


proc 501
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b对于那些占领了据点的公会，人们将其称之为“军阀”，由于世界上最多只能同时存在六个，所以也被称为“最强的六个”
	add_talk #b#b	
	add_talk #b#b★ 占领冰岛据点：#c186506冰岛·军阀#n
	add_talk #b#b★ 占领银风雪原据点：#c186506雪原·军阀#n
	add_talk #b#b★ 占领流金沙漠据点：#c186506沙漠·军阀#n
	add_talk #b#b★ 占领落日荒野据点：#c186506荒野·军阀#n
	add_talk #b#b★ 占领越雾雨林据点：#c186506雨林·军阀#n
	add_talk #b#b★ 占领鸣沙陷地据点：#c186506鸣沙·军阀
	
	;	option 300 我想了解“军阀”公会
		option 510 我想了解“领主”公会
		option 520 我想了解“城主”公会
		option 530 我想了解“霸主”公会
	
	talk
endproc
proc 510
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b只有占领了一个据点以上的公会才有资格被称为“领主”，显然他们是比“军阀”更强的公会
	add_talk #b#b	
	add_talk #b#b★ 占领2个据点：#c005ebe双域·领主#n
	add_talk #b#b★ 占领3个据点：#c005ebe三域·领主#n
	add_talk #b#b★ 占领4个据点：#c005ebe四域·领主#n
	add_talk #b#b★ 占领5个据点：#c005ebe五域·领主#n
	add_talk #b#b★ 占领6个据点：#c005ebe全域·领主

		option 501 我想了解“军阀”公会
	;	option 310 我想了解“领主”公会
		option 520 我想了解“城主”公会
		option 530 我想了解“霸主”公会
	
	talk
endproc 

proc 520
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b成为“城主”只有一个要求，谁能占领时间城谁就是城主，但这也意味着城主将面临所剩那五个最强公会的竞争
	add_talk #b#b
	add_talk #b#b★ 占领时间城：#c6d00d2时间·城主#n
	add_talk #b#b★ 占领时间城和1个据点：#c6d00d2主境·城主#n
	add_talk #b#b★ 占领时间城和2个据点：#c6d00d2双境·城主#n
	add_talk #b#b★ 占领时间城和3个据点：#c6d00d2三境·城主#n
	add_talk #b#b★ 占领时间城和4个据点：#c6d00d2四境·城主#n
	add_talk #b#b★ 占领时间城和5个据点：#c6d00d2五境·城主

		option 501 我想了解“军阀”公会
		option 510 我想了解“领主”公会
	;	option 320 我想了解“城主”公会
		option 530 我想了解“霸主”公会
	
	
	talk
endproc 
proc 530
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b世界只有一个霸主，主宰时间城与所有据点的公会，才有资格登上霸主的宝座！
	add_talk #b#b
	add_talk #b#b★ 占领时间城和6个据点：#cff4e00世界·霸主

		option 501 我想了解“军阀”公会
		option 510 我想了解“领主”公会
		option 520 我想了解“城主”公会
	;	option 330 我想了解“霸主”公会
	
	talk
endproc 






