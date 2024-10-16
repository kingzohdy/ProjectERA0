

proc 1


	DIALOG 1
	add_talk #b#b公会旗帜无疑是象征荣耀与宿命的徽记。
	add_talk #b#b只有独据一方的公会将权握仅有的六面旗帜。
	add_talk #b#b这最强的六个被称作【六域军阀】。
	add_talk #b#b
	add_talk #c186506占领该据点的公会，旗帜可开启【战旗荣耀】活动

		
		option 400 【战旗荣耀】活动
		option 220 自定义旗帜
		option 221 编辑公会宣言
		option 222 公会传奇
	

	talk
endproc 

proc 220
	open_ui 70
endproc 

proc 221 

	open_ui 71
endproc 

proc 222
	DIALOG 1
	add_talk #b#b在零纪元元年前的那次旷世大战中，出现了六个公会，他们带领着天行者一次一次从战火中凯旋而归，从此便流下一个传承。
	add_talk #b#b时至今日，经历了历史的交替，能够登上这六个宝座的公会，已然成为最强的六个象征，成为每位勇士心中的“瓦尔哈拉”！
	add_talk #b#b但强者的世界容不下二者，为了成为最强中的最强，逐渐演化出了军阀，领主，城主的实力划分，而最后只能有一个公会登上霸主之位。
	add_talk #b#b
	add_talk #b#b#c186506注释：瓦尔哈拉为北欧神话中为了迎接“诸神的黄昏”之战，奥丁为阵亡英雄安置的宫殿
	
		option 300 我想了解“军阀”公会
		option 310 我想了解“领主”公会
		option 320 我想了解“城主”公会
		option 330 我想了解“霸主”公会
	
	talk
endproc

proc 300
	DIALOG 1
	add_talk #b#b对于那些占领了据点的公会，人们将其称之为“军阀”，由于世界上最多只能同时存在六个，所以也被称为“最强的六个”
	add_talk #b#b	
	add_talk #b#b★ 占领冰岛据点：#c186506冰岛·军阀#n
	add_talk #b#b★ 占领银风雪原据点：#c186506雪原·军阀#n
	add_talk #b#b★ 占领流金沙漠据点：#c186506沙漠·军阀#n
	add_talk #b#b★ 占领落日荒野据点：#c186506荒野·军阀#n
	add_talk #b#b★ 占领越雾雨林据点：#c186506雨林·军阀#n
	add_talk #b#b★ 占领鸣沙陷地据点：#c186506鸣沙·军阀
	
	;	option 300 我想了解“军阀”公会
		option 310 我想了解“领主”公会
		option 320 我想了解“城主”公会
		option 330 我想了解“霸主”公会
	
	talk
endproc
proc 310
	DIALOG 1
	add_talk #b#b只有占领了一个据点以上的公会才有资格被称为“领主”，显然他们是比“军阀”更强的公会
	add_talk #b#b	
	add_talk #b#b★ 占领2个据点：#c005ebe双域·领主#n
	add_talk #b#b★ 占领3个据点：#c005ebe三域·领主#n
	add_talk #b#b★ 占领4个据点：#c005ebe四域·领主#n
	add_talk #b#b★ 占领5个据点：#c005ebe五域·领主#n
	add_talk #b#b★ 占领6个据点：#c005ebe全域·领主

		option 300 我想了解“军阀”公会
	;	option 310 我想了解“领主”公会
		option 320 我想了解“城主”公会
		option 330 我想了解“霸主”公会
	
	talk
endproc 

proc 320
	DIALOG 1
	add_talk #b#b成为“城主”只有一个要求，谁能占领时间城谁就是城主，但这也意味着城主将面临所剩那五个最强公会的竞争
	add_talk #b#b
	add_talk #b#b★ 占领时间城：#c6d00d2时间·城主#n
	add_talk #b#b★ 占领时间城和1个据点：#c6d00d2主境·城主#n
	add_talk #b#b★ 占领时间城和2个据点：#c6d00d2双境·城主#n
	add_talk #b#b★ 占领时间城和3个据点：#c6d00d2三境·城主#n
	add_talk #b#b★ 占领时间城和4个据点：#c6d00d2四境·城主#n
	add_talk #b#b★ 占领时间城和5个据点：#c6d00d2五境·城主

		option 300 我想了解“军阀”公会
		option 310 我想了解“领主”公会
	;	option 320 我想了解“城主”公会
		option 330 我想了解“霸主”公会
	
	
	talk
endproc 
proc 330
	DIALOG 1
	add_talk #b#b世界只有一个霸主，主宰时间城与所有据点的公会，才有资格登上霸主的宝座！
	add_talk #b#b
	add_talk #b#b★ 占领时间城和6个据点：#cff4e00世界·霸主

		option 300 我想了解“军阀”公会
		option 310 我想了解“领主”公会
		option 320 我想了解“城主”公会
	;	option 330 我想了解“霸主”公会
	
	talk
endproc 


proc 400
	DIALOG 1
	add_talk #b#b每天公会旗帜将被英灵所祝福，散发出被称为奇观的荣耀辉芒。
	add_talk #b#b#c005ebe当辉芒普照时，所有驻守在公会旗帜下的人将获得荣耀的回馈！
	add_talk #b#b
	add_talk #c186506开启要求：占领据点第2天起
	add_talk #b#b#b#b#b解除59级等级封印
	add_talk 活动时间：每个【公会旗帜】每天可开启1次
	
	strvar map
	GET_PLAYER_MAPINFO 1 map 0 0 0
	if map = 2202
		add_talk 开启权限：【冰岛】据点公会的会长或副会长
	elseif map = 2200
		add_talk 开启权限：【银风雪原】据点公会的会长或副会长
	elseif map = 2300
		add_talk 开启权限：【流金沙漠】据点公会的会长或副会长
	elseif map = 2400
		add_talk 开启权限：【落日荒野】据点公会的会长或副会长
	elseif map = 2500
		add_talk 开启权限：【越雾雨林】据点公会的会长或副会长
	elseif map = 3404
		add_talk 开启权限：【鸣沙陷地】据点公会的会长或副会长
	else
	endif
	add_talk 持续时间：每个【公会旗帜】每次持续10分钟#n
	
	strvar pos
	player_clan_pos_info 1 0 0 0 Pos
	if Pos = 7 or Pos = 8
	
		is_strong_member 1 2202 50
		is_strong_member 1 2200 51
		is_strong_member 1 2300 52
		is_strong_member 1 2400 53
		is_strong_member 1 2500 54
		is_strong_member 1 3404 55
		
		strvar mapid
		has_strong_glory 1 mapid
		
		strvar Rflag Maxlv
		GET_CUR_MAX_LEVEL Rflag Maxlv
		if Rflag = 0 and Maxlv > 59
			strvar mapid
			has_strong_glory 1 mapid
				
			if map = 2202 and 50 = 1 and mapid != 2202
				option 410 #我已召集会员，开启【战旗荣耀】活动	
			endif
			if map = 2200 and 51 = 1 and mapid != 2200
				option 410 #我已召集会员，开启【战旗荣耀】活动
			endif
			if map = 2300 and 52 = 1 and mapid != 2300
				option 410 #我已召集会员，开启【战旗荣耀】活动
			endif
			if map = 2400 and 53 = 1 and mapid != 2400
				option 410 #我已召集会员，开启【战旗荣耀】活动
			endif
			if map = 2500 and 54 = 1 and mapid != 2500
				option 410 #我已召集会员，开启【战旗荣耀】活动
			endif
			if map = 3404 and 55 = 1 and mapid != 3404
				option 410 #我已召集会员，开启【战旗荣耀】活动
			endif
		endif
	endif
	
	talk
endproc
proc 402
endproc
proc 410
	
	choose 2 411 402 是否确认开启【战旗荣耀】活动？
endproc
proc 411
	strvar pos
	player_clan_pos_info 1 0 0 0 Pos
	if Pos = 7 or Pos = 8
	
		
		strvar map
		GET_PLAYER_MAPINFO 1 map 0 0 0
		if map = 2202
			get_map_npc_id 302140 50 1
		;	EFFECT_ID 0 @50 playMontion(%d,"3195001")
			STRONG_OPEN_GLORY 1 2202
			if 1 = 0
			;	effect 0 0 playEffct(23000,8700,0,"effect\\entity\\3195001.ent",600)
			endif
		endif
		if map = 2200
			get_map_npc_id 302141 50 1
		;	EFFECT_ID 0 @50 playMontion(%d,"3195001")
			STRONG_OPEN_GLORY 1 2200
			if 1 = 0
				;effect 0 0 playEffct(6500,18000,0,"effect\\entity\\3195001.ent",600)
			endif
		endif
		if map = 2300
			get_map_npc_id 302142 50 1
		;	EFFECT_ID 0 @50 playMontion(%d,"3195001")
			STRONG_OPEN_GLORY 1 2300
			if 1 = 0
				;effect 0 0 playEffct(8700,12400,0,"effect\\entity\\3195001.ent",600)
			endif
		endif
		if map = 2400
			get_map_npc_id 302143 50 1
		;	EFFECT_ID 0 @50 playMontion(%d,"3195001")
			STRONG_OPEN_GLORY 1 2400
			if 1 = 0
				;effect 0 0 playEffct(14000,15400,0,"effect\\entity\\3195001.ent",600)
			endif
		endif
		if map = 2500
			get_map_npc_id 302144 50 1
		;	EFFECT_ID 0 @50 playMontion(%d,"3195001")
			STRONG_OPEN_GLORY 1 2500
			if 1 = 0
				;effect 0 0 playEffct(25700,25700,0,"effect\\entity\\3195001.ent",600)
			endif
		endif
		if map = 3404
			get_map_npc_id 302145 50 1
		;	EFFECT_ID 0 @50 playMontion(%d,"3195001")
			STRONG_OPEN_GLORY 1 3404
			if 1 = 0
				;effect 0 0 playEffct(27400,13200,0,"effect\\entity\\3195001.ent",600)
			endif
		endif
	else
		ADD_SYS_MSG_START
		ADD_SYS_MSG 只有会长、副会长才可开启【战旗荣耀】活动
		SET_SYS_MSG_MODE 3 0 0
		MSG_INFO
	
	endif
endproc 






