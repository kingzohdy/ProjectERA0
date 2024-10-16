

proc 1

	add_sys_msg_start
	add_sys_msg 年兽们出现在银风雪原、流金沙漠、落日荒野、越雾雨林、越雾甬道地图，击杀年兽将会获得奖励，快去消灭它们维持世界的平衡吧！
	set_sys_msg_mode 6 0 0
	svr_msg_info

	
	var 10 0
	set_event_var 0 32 @10 1
	
	call 100
	call 110
	call 120
	call 130
	call 140
endproc 

proc 3
	
	get_event_var 0 32 1 40 
	add 40 1
	set_event_var 0 32 @40 1
	if 40 = 180
	
		call 100
		call 110
		call 120
		call 130
		call 140
	
		add_sys_msg_start
		add_sys_msg 年兽们出现在银风雪原、流金沙漠、落日荒野、越雾雨林、越雾甬道地图，击杀年兽将会获得奖励，快去消灭它们维持世界的平衡吧！
		set_sys_msg_mode 6 0 0
		svr_msg_info
		
		var 40 0
		set_event_var 0 32 @40 1
	endif

endproc 


proc 100
	
	rand 10 4
	switch 10
		case 0
			NEW_MAP_MON 207007 1 2200 3755 18728 0 0 0 0 1800 0 0 
		endcase
		case 1
			NEW_MAP_MON 207007 1 2200 6111 17184 0 0 0 0 1800 0 0
		endcase
		case 2
			NEW_MAP_MON 207007 1 2200 7794 18448 0 0 0 0 1800 0 0
		endcase
		case 3
			NEW_MAP_MON 207007 1 2200 7200 20911 0 0 0 0 1800 0 0
		endcase
	endswitch
	
	rand 11 4
	switch 11
		case 0
			NEW_MAP_MON 207007 1 2200 3625 6474 0 0 0 0 1800 0 0
		endcase
		case 1
			NEW_MAP_MON 207007 1 2200 6014 7167 0 0 0 0 1800 0 0
		endcase
		case 2
			NEW_MAP_MON 207007 1 2200 3500 4950 0 0 0 0 1800 0 0
		endcase
		case 3
			NEW_MAP_MON 207007 1 2200 5454 3977 0 0 0 0 1800 0 0
		endcase
	endswitch

	rand 12 4
	switch 12
		case 0
			NEW_MAP_MON 207007 1 2200 15003 5079 0 0 0 0 1800 0 0
		endcase
		case 1
			NEW_MAP_MON 207007 1 2200 17582 5445 0 0 0 0 1800 0 0
		endcase
		case 2
			NEW_MAP_MON 207007 1 2200 12814 6500 0 0 0 0 1800 0 0
		endcase
		case 3
			NEW_MAP_MON 207007 1 2200 18002 7079 0 0 0 0 1800 0 0
		endcase
	endswitch
	
	rand 13 4
	switch 13
		case 0
			NEW_MAP_MON 207007 1 2200 17075 21032 0 0 0 0 1800 0 0
		endcase
		case 1
			NEW_MAP_MON 207007 1 2200 19792 20423 0 0 0 0 1800 0 0
		endcase
		case 2
			NEW_MAP_MON 207007 1 2200 16417 19474 0 0 0 0 1800 0 0
		endcase
		case 3
			NEW_MAP_MON 207007 1 2200 18393 18814 0 0 0 0 1800 0 0
		endcase
	endswitch
	
	
endproc  

proc 110

	rand 10 4
	switch 10
		case 0
			NEW_MAP_MON 207007 1 2300 2667 18212 0 0 0 0 1800 0 0
		endcase
		case 1
			NEW_MAP_MON 207007 1 2300 4861 16848 0 0 0 0 1800 0 0
		endcase
		case 2
			NEW_MAP_MON 207007 1 2300 6333 18426 0 0 0 0 1800 0 0
		endcase
		case 3
			NEW_MAP_MON 207007 1 2300 4673 19235 0 0 0 0 1800 0 0
		endcase
	endswitch
	
	rand 11 4
	switch 11
		case 0
			NEW_MAP_MON 207007 1 2300 6298 6044 0 0 0 0 1800 0 0
		endcase
		case 1
			NEW_MAP_MON 207007 1 2300 7685 5037 0 0 0 0 1800 0 0
		endcase
		case 2
			NEW_MAP_MON 207007 1 2300 7489 4318 0 0 0 0 1800 0 0
		endcase
		case 3
			NEW_MAP_MON 207007 1 2300 6099 4767 0 0 0 0 1800 0 0
		endcase
	endswitch
	
	rand 12 4
	switch 12
		case 0
			NEW_MAP_MON 207007 1 2300 14512 6735 0 0 0 0 1800 0 0
		endcase
		case 1
			NEW_MAP_MON 207007 1 2300 16298 6609 0 0 0 0 1800 0 0
		endcase
		case 2
			NEW_MAP_MON 207007 1 2300 16126 5285 0 0 0 0 1800 0 0
		endcase
		case 3
			NEW_MAP_MON 207007 1 2300 14660 5316 0 0 0 0 1800 0 0
		endcase
	endswitch

	rand 13 4
	switch 13
		case 0
			NEW_MAP_MON 207007 1 2300 19752 18679 0 0 0 0 1800 0 0
		endcase
		case 1
			NEW_MAP_MON 207007 1 2300 21662 18475 0 0 0 0 1800 0 0
		endcase
		case 2
			NEW_MAP_MON 207007 1 2300 21637 16938 0 0 0 0 1800 0 0
		endcase
		case 3
			NEW_MAP_MON 207007 1 2300 19762 16695 0 0 0 0 1800 0 0
		endcase
	endswitch     
endproc 

proc 120

	rand 10 4
	switch 10
		case 0
			NEW_MAP_MON 207007 1 2400 19646 5960 0 0 0 0 1800 0 0
		endcase
		case 1
			NEW_MAP_MON 207007 1 2400 21210 5906 0 0 0 0 1800 0 0
		endcase
		case 2
			NEW_MAP_MON 207007 1 2400 19612 4177 0 0 0 0 1800 0 0
		endcase
		case 3
			NEW_MAP_MON 207007 1 2400 20746 4010 0 0 0 0 1800 0 0
		endcase
	endswitch
	
	rand 11 4
	switch 11
		case 0
			NEW_MAP_MON 207007 1 2400 17294 18043 0 0 0 0 1800 0 0
		endcase
		case 1
			NEW_MAP_MON 207007 1 2400 18875 18057 0 0 0 0 1800 0 0
		endcase
		case 2
			NEW_MAP_MON 207007 1 2400 18129 18989 0 0 0 0 1800 0 0
		endcase
		case 3
			NEW_MAP_MON 207007 1 2400 17124 17058 0 0 0 0 1800 0 0
		endcase
	endswitch
	
	rand 12 4
	switch 12
		case 0
			NEW_MAP_MON 207007 1 2400 12440 18191 0 0 0 0 1800 0 0
		endcase
		case 1
			NEW_MAP_MON 207007 1 2400 13240 18103 0 0 0 0 1800 0 0
		endcase
		case 2
			NEW_MAP_MON 207007 1 2400 11747 16804 0 0 0 0 1800 0 0
		endcase
		case 3
			NEW_MAP_MON 207007 1 2400 12597 16678 0 0 0 0 1800 0 0
		endcase
	endswitch

	rand 13 4
	switch 13
		case 0
			NEW_MAP_MON 207007 1 2400 6184 6174 0 0 0 0 1800 0 0
		endcase
		case 1
			NEW_MAP_MON 207007 1 2400 7562 5966 0 0 0 0 1800 0 0
		endcase
		case 2
			NEW_MAP_MON 207007 1 2400 5925 4980 0 0 0 0 1800 0 0
		endcase
		case 3
			NEW_MAP_MON 207007 1 2400 6951 4256 0 0 0 0 1800 0 0
		endcase
	endswitch          

endproc  

proc 130

	rand 10 4
	switch 10
		case 0
			NEW_MAP_MON 207007 1 2500 25034 21669 0 0 0 0 1800 0 0
		endcase
		case 1
			NEW_MAP_MON 207007 1 2500 26922 17139 0 0 0 0 1800 0 0
		endcase
		case 2
			NEW_MAP_MON 207007 1 2500 27925 31390 0 0 0 0 1800 0 0
		endcase
		case 3
			NEW_MAP_MON 207007 1 2500 24356 30979 0 0 0 0 1800 0 0
		endcase
	endswitch
	
	rand 11 4
	switch 11
		case 0
			NEW_MAP_MON 207007 1 2500 23868 27299 0 0 0 0 1800 0 0
		endcase
		case 1
			NEW_MAP_MON 207007 1 2500 6685 28498 0 0 0 0 1800 0 0
		endcase
		case 2
			NEW_MAP_MON 207007 1 2500 8767 30139 0 0 0 0 1800 0 0
		endcase
		case 3
			NEW_MAP_MON 207007 1 2500 12507 27566 0 0 0 0 1800 0 0
		endcase
	endswitch
	
	rand 12 4
	switch 12
		case 0
			NEW_MAP_MON 207007 1 2500 9560 26044 0 0 0 0 1800 0 0
		endcase
		case 1
			NEW_MAP_MON 207007 1 2500 9229 17482 0 0 0 0 1800 0 0
		endcase
		case 2
			NEW_MAP_MON 207007 1 2500 10557 16661 0 0 0 0 1800 0 0
		endcase
		case 3
			NEW_MAP_MON 207007 1 2500 10269 15799 0 0 0 0 1800 0 0
		endcase
	endswitch

	rand 13 4
	switch 13
		case 0
			NEW_MAP_MON 207007 1 2500 8308 16257 0 0 0 0 1800 0 0
		endcase
		case 1
			NEW_MAP_MON 207007 1 2500 21944 19342 0 0 0 0 1800 0 0
		endcase
		case 2
			NEW_MAP_MON 207007 1 2500 23613 15848 0 0 0 0 1800 0 0
		endcase
		case 3
			NEW_MAP_MON 207007 1 2500 23688 26703 0 0 0 0 1800 0 0
		endcase
	endswitch          

endproc  

proc 140
	
	rand 10 4
	switch 10
		case 0
			NEW_MAP_MON 207007 1 3500 7341 8620 0 0 0 0 1800 0 0
		endcase
		case 1
			NEW_MAP_MON 207007 1 3500 8482 8505 0 0 0 0 1800 0 0
		endcase
		case 2
			NEW_MAP_MON 207007 1 3500 7176 7164 0 0 0 0 1800 0 0
		endcase
		case 3
			NEW_MAP_MON 207007 1 3500 8498 7222 0 0 0 0 1800 0 0
		endcase
	endswitch
	
	rand 11 4
	switch 11
		case 0
			NEW_MAP_MON 207007 1 3500 16808 10458 0 0 0 0 1800 0 0
		endcase
		case 1
			NEW_MAP_MON 207007 1 3500 15567 10401 0 0 0 0 1800 0 0
		endcase
		case 2
			NEW_MAP_MON 207007 1 3500 15402 12185 0 0 0 0 1800 0 0
		endcase
		case 3
			NEW_MAP_MON 207007 1 3500 16906 11997 0 0 0 0 1800 0 0
		endcase
	endswitch
	
	rand 12 4
	switch 12
		case 0
			NEW_MAP_MON 207007 1 3500 12115 18865 0 0 0 0 1800 0 0
		endcase
		case 1
			NEW_MAP_MON 207007 1 3500 10725 18298 0 0 0 0 1800 0 0
		endcase
		case 2
			NEW_MAP_MON 207007 1 3500 14162 18311 0 0 0 0 1800 0 0
		endcase
		case 3
			NEW_MAP_MON 207007 1 3500 15926 17821 0 0 0 0 1800 0 0
		endcase
	endswitch

	rand 13 4
	switch 13
		case 0
			NEW_MAP_MON 207007 1 3500 7205 18455 0 0 0 0 1800 0 0
		endcase
		case 1
			NEW_MAP_MON 207007 1 3500 8769 18214 0 0 0 0 1800 0 0
		endcase
		case 2
			NEW_MAP_MON 207007 1 3500 7358 16848 0 0 0 0 1800 0 0
		endcase
		case 3
			NEW_MAP_MON 207007 1 3500 8954 16665 0 0 0 0 1800 0 0
		endcase
	endswitch          

endproc 












