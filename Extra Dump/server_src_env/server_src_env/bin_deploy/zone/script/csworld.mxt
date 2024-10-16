;
proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b欢迎光临急你所急航空，我公司禀承“客户至上”的承诺，通过提供“可靠、准点、方便、快捷”的优质服务，致力满足并超越客户的期望，更曾一度获得民航总局颁发的飞行安全最高奖。
;	add_talk #b#b如今，为答谢广大客户的支持与厚爱，我公司特举办飞行优惠活动，凡在时间城内搭乘我公司的航班，无论距离远近，只需要90银币！
;	add_talk #b#b只需90银币！您就可以缩短大量的路程！您没有看错！不是9千！不是9百！只要90银币！
	
	strvar LV
	GET_PLAYER_BASE_ATT 1 1 LV 1
	if LV >= 10
		OPTION 100 #世界航线
	endif
	talk
	
endproc
proc 100
	strvar ret
	task_stat 3131 1 ret
	if ret = 2
		task_fini 3131 1
	endif
	MAP_TRANS 1

endproc

