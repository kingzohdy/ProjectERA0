

proc 1
	
	strvar Ret
	IS_WORLD_CITY_OWNER Ret
	if Ret = 1
		call 100
	else
		call 200
	endif
endproc 

proc 100
	DIALOG 1

	GET_PLAYER_BASE_ATT 1 0 2 0
	get_world_city_keep_num 1 3
	clan_pay 0 1 4

	add 3 1

	add_talk $_npc$:
	add_talk #b#b尊敬的时间城霸主成员，很愿意为您效劳。
	add_talk #b#b
	add_talk #b#b您所在公会已经持续占领时间城$@3$次；
;	add_talk #b#b您现在是$@2$等级；
;	add_talk #b#b可领取占领资金：$@4$银币。

	; option 101 #领取占领资金	
	option 99 #关闭
	
	talk
endproc

proc 101
	clan_pay 0 1 0
endproc

proc 200
	DIALOG 1

	add_talk $_npc$:
	add_talk #b#b做为一个城市的负责人，我可是拥有很多特权，给予你们这帮人什么样的帮助完全看我的心情。
	add_talk #b#b小子！离我远一点，别以为谁都能来指使我干着干那的。我！沃格大人！只负责为尊敬时间城霸主们提供服务。不过等你什么时候成为霸主的一员，我还是愿意为您效劳的。
	
	option 99 #关闭
	
	talk
endproc


