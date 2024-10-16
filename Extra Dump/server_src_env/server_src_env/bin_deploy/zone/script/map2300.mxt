;地图脚本, 流金沙漠，1/500刷新宝物怪

proc 4
	;4为死亡怪物id，5为随机数，6、7为坐标,8为死亡怪物等级
	get_mon_info 1 4 1 1 1 1 6 7
	get_mon_level 1 8
	if @4 != 227009
	rand 5 1000
	if @5 < 0
	NEW_LIFE_DYN_MON 1 227009 1 @6 @7 0 0 0 1 0 @8 0 60
	endif
	endif
endproc
















