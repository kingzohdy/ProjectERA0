
proc 1
	strvar Ret Sex Level Job
	GET_PLAYER_BASE_ATT  Ret Sex Level Job

;	if @level = 20
;		call 20
;		call 25
;	endif

;	if @level = 25
;		call 25
;	endif

	if @level = 30
		call 30
	endif


	if @level = 35
		TASK_STAT 4020 1 2
		if 2 = 0
			call 35
		endif		
	endif

;	if @level = 50
;		call 50
;	endif
endproc

proc 20
	mail_text 亲爱的#n：#r#t在你离开的这段时间，莉莉曾经多次来找过你，想与你见上一面。如果你现在没什么特别重要的事情，就去与她谈谈吧。#r#r#t#t#t#t#t#t阿尔弗雷德
	sys_mail 1 5030099 1 5041000 1 一封没贴邮票的信$大行政官阿尔弗雷德
endproc

proc 25
	mail_text 亲爱的#n：#r#r#t我是坐骑商人阿诺德，邀请你参加：百米赛跑。#r#t无论输赢你都会得到非常丰厚的奖励！#r#t参赛证已附上，可直接前往裁判瑞德处参赛。#r#r#t#t#t#t#t#t阿诺德
	sys_mail 1 5030100 1 5041000 1 运动会参赛证$坐骑商人阿诺德
endproc

proc 30
	mail_text 亲爱的#n：#r#r#t我们在茫茫信海中发现了这封邀请函，它并没有详细说明要交给谁，只写着“最优秀的#c亲启”，我首先想到的就是您了，请您于百忙之中查收。#r#r#t#t#t#t#t#t丹尼尔
	sys_mail 1 5030101 1 5041000 1 大地之心邀请函$联邦快递
endproc

proc 35
	mail_text 年轻的#c：#r#r#t天行者如今的辉煌，一半来自上万年的科技技术积累。#r#t或许汝心中仍感困惑，回答老夫的几个提问能也许能帮助汝解开心中的疑惑。#r#r#t#t#t#t#t#t大马丁
	sys_mail 1 5030103 1 0 0 大马丁的邀请函$博学士大马丁
endproc

;proc 50
;	mail_text 尊敬的#n：#r#r#t你或许听过职业坐骑吧？它们是与职业标配的坐骑，拥有更快的速度，但想获取它们必须通过试炼。嘿嘿！放心，你的大名早已传得沸沸扬扬，相信以你的实力只是小菜一碟！#r#t推荐函我已附上，还等什么，赶紧找罗德参加吧！等你的好消息！#r#r#t#t#t#t#t#t阿诺德
;	sys_mail 1 5030102 1 0 0 坐骑试炼推荐函$坐骑商人阿诺德
;
;	mail_text #r#r#t恭喜您已经达到礼包要求。下次《零纪元》测试，您的账号无需再次激活，可直接进入游戏，请妥善保管好自己的账号。同时我们将在下次测试前通过邮件或短信的方式提醒到您（建议您尽可能完善账号的注册信息，特别是常用邮箱或手机号码），最后献上我们的诚挚感谢！
;	sys_mail 1 0 0 0 0 新纪元直通车$九城运营团队
;endproc