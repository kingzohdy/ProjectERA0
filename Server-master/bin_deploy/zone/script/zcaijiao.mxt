proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b#自开创以来，我们皇家学院一直是作战前线的重要战斗力，受到了自治联盟的高度关注。
	add_talk #b#b#只要你#c186506达到60级，即可申请成为学院的导师#n。对于作出贡献的导师，我们将会给以绝对的肯定与奖励。
	add_talk #b#b#如果你#c186506只有30到59级，也可以从我们这找到导师协助成长#n。
	add_talk #b#b#作为皇家学院的教授，我在此衷心的希望每一位勇士能够拥有自己的一片天空。

	strvar Ret Ret_1 MaxLV
	get_id_player_info 0 8 Ret
	GET_CUR_MAX_LEVEL Ret_1 MaxLV
	
	if @Ret > 59
		option 100 #如何成为导师？
	endif
	if @Ret_1 = 0 and @MaxLV > 59
		if @Ret > 29 and @Ret < 60
			option 300 #如何找到导师？
		endif
		option 200 #如何考核毕业？
	endif

	talk
endproc 

proc 100
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b想要成为一名导师，首先你需要具备#c186506达到60级#n这个基本实力。
	add_talk #b#b再来为了保证授学质量，导师同时只能招收#c1865065个#n学生。
	add_talk #b#b
	add_talk #b#b#c186506请确保建立师生关系时双方都在场。#n
	
	option 101 我要招收学生
	option 102 查看学生登记信息
	option 103 导师福利介绍
	option 1 我还想再多了解
	
	talk
endproc 

proc 101
	SCHOOL_INVITE_UI 0
endproc	

proc 102
	GET_REG_INFO 0
endproc

proc 103
	DIALOG 1
	add_talk 导师福利：
	add_talk #b#b只要学生#c186506成长了10级#n后，达到#c18650645,50,54,56,58,60,63,65,70级#n时，导师即可从邮箱收到一份奖励。
	add_talk #b#b当学生#c186506达到65级#n成功毕业后，我们将会邮寄一封毕业奖励给导师和学生。
	add_talk #b#b
	add_talk #b#b而你对学生的付出将获得【#c186506传承点数#n】的肯定，拥有这个点数，你可向我右方的【#c186506皇家学院军需官-奥迪#n】购置皇家学院#c186506专售物品#n。
	
		option 100 我已经了解

	talk
endproc

proc 200
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b只要学生#c186506达到65级#n，即可选择是否毕业。
	add_talk #b#b成功考核毕业后，我们将会邮寄一封毕业奖励给导师和学生。

		option 201 我想考核毕业
		option 1 我还想再多了解

	talk
endproc
	
proc 201
	SCHOOL_FINI 0
endproc

proc 300
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b如果你在#c18650630级到59级#n之间，我们建议你找1位导师，这样将让你的成长获得极大的帮助。
	add_talk #b#b
	add_talk #b#b#c186506你可以通过下方的发布寻求导师信息，更有效快捷的找到导师。#n

	option 301 发布寻求导师信息（学生登记）
	option 102 查看学生登记信息
	option 302 学生福利介绍
	option 1 我还想再多了解

	talk
endproc
	
proc 301
	open_ui 86
endproc

proc 302
	DIALOG 1
	add_talk 学生福利：
	add_talk #b#b通过导师的授学，学生可以获得由导师直接传授的经验值。
	add_talk #b#b只要学生#c186506大于40级，小于最高等级玩家10级#n，每天还能领取“#c186506天天向上#n”状态获得大量经验值。
	add_talk #b#b当学生#c186506达到65级#n成功毕业后，我们将会邮寄一封毕业奖励给导师和学生。
	
	option 300 我已经了解
	
	talk
endproc
