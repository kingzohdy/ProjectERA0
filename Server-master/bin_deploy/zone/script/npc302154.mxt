;公会条约

proc 1
	DIALOG 1
	add_talk #b#b#b#b#b#b#b#b《公会条约》
	add_talk #b#b
	add_talk #b#b《#c005ebe退会条约#n》——退出公会的相关说明
	add_talk #b#b《#c005ebe散会条约#n》——解散公会的相关说明
	add_talk #b#b《#c005ebe让位条约#n》——会长让位的相关说明
	
	option 100 #《退会条约》
	option 200 #《散会条约》
	option 300 #《让位条约》
	
	talk
endproc

proc 100
	DIALOG 1
	add_talk #b#b#b#b#b#b#b#b《退会条约》
	add_talk #b#b#b 退会后全数返还当前剩余的#c186506贡献度#n
	add_talk #b 退会后返还#c18650680％#n学习公会技能花费的#c186506贡献度#n
	add_talk #b#b
	add_talk #b#b#b#b#b#b#b#b#b自主退会
	add_talk #b#b#b 退会#c18650624小时#n后可申请加入任何公会
	add_talk #b#b
	add_talk #b#b#b#b#b#b#b#b#b踢除退会
	add_talk #b#b#b#b退会后随时可申请加入任何公会
	add_talk #b#b#b #c005ebe（公会解散后视为自动踢除退会）#n
	
	option 1 #返回
	
	talk
endproc

proc 200
	DIALOG 1
	add_talk #b#b#b#b#b#b#b#b《散会条约》
	add_talk #b#b#b#b#b#b#b#b#b自主散会
	add_talk #b#b 会长自主申请解散，经确认后解散公会
	add_talk #b#b#b #c005ebe（需在申请后第48-96小时内确认）#n
	add_talk #b#b
	add_talk #b#b#b#b#b#b#b#b#b自动散会
	add_talk #b 公会基本评议未达标，将自动回收解散公会
	add_talk #b#b#b[1-5]级公会的人数#c186506连续7天#n少于10人
	add_talk #b[1]级公会的资金#c186506连续15天#n不足结算维护费
	add_talk #b#b[1-5]级公会的成员#c186506连续60天#n无任何人在线
	add_talk #b#b
	add_talk #b#b#b#b#b#b#b#b#b公会降级
	add_talk #b [2-5]级公会的资金#c186506连续3天#n不足结算维护费
	
	option 201 #“维护费”说明
	option 1 #返回
	
	talk
endproc

proc 201
	DIALOG 1
	add_talk #b#b#b#b#b#b#b “维护费”说明
	add_talk #b#b#b公会资金提供公会运作所需的维护费
	add_talk #b#b#b#b#b#b#b维护费每天结算1次
	add_talk #b#b#b#b 公会等级越高，维护费用越高
	add_talk #b#b#b#b 若安定度越高，维护费用越低
	add_talk #b#b# #c005ebe（未结算的维护费，累积到下次结算）#n
	add_talk #b#b
	add_talk ☆维护费用☆
	add_talk #b#b1级公会：≥#c18650630000#n公会资金
	add_talk #b#b2级公会：≥#c18650650000#n公会资金
	add_talk #b#b3级公会：≥#c186506100000#n公会资金
	add_talk #b#b4级公会：≥#c186506500000#n公会资金
	add_talk #b#b5级公会：≥#c186506800000#n公会资金
	
	option 200 #返回
	
	talk
endproc

proc 300
	DIALOG 1
	add_talk #b#b#b#b#b#b#b#b《让位条约》
	add_talk #b#b#b#b#b#b#b#b#b自主让位
	add_talk #b#b会长可自主将职位转让给公会的任何成员
	add_talk #b#b
	add_talk #b#b#b#b#b#b#b#b#b自动让位
	add_talk #b 会长#c186506连续10天#n不在线，自动让位给公会成员
	add_talk #b#b 贡献度最高的成员，将接替新会长职位
	add_talk #b#b#b #c005ebe（接位成员需离线时间小于15天）#n
	
	option 1 #返回
	
	talk
endproc
