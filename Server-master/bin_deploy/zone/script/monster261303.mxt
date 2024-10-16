;ÐÂ¸±±¾

proc 2

	strvar ret lv
	get_mon_level ret lv
	if ret = 0
		if lv < 40
			 MON_DROP_BOOTYGROUP 5610142 2087 2010 2020 0
		endif                         
		if lv >= 40 and lv < 50       
			 MON_DROP_BOOTYGROUP 5610142 2088 2011 2021 0
		endif                         
		if lv >= 50 and lv < 60       
			 MON_DROP_BOOTYGROUP 5610142 2089 2012 2022 0
		endif                         
		if lv >= 60 and lv < 70       
			 MON_DROP_BOOTYGROUP 5610142 2090 2013 2023 0
		endif                         
		if lv >= 70 and lv < 80       
			 MON_DROP_BOOTYGROUP 5610142 2091 2014 2024 0
		endif                         
		if lv >= 80 and lv < 90       
			 MON_DROP_BOOTYGROUP 5610142 2092 2015 2025 0
		endif                         
		if lv >= 90 and lv < 100      
			 MON_DROP_BOOTYGROUP 5610142 2093 2016 2026 0
		endif                         
		if lv >= 100 and lv < 110     
			 MON_DROP_BOOTYGROUP 5610142 2094 2017 2027 0
		endif                         
		if lv >= 110 and lv <= 120    
			 MON_DROP_BOOTYGROUP 5610142 2095 2018 2028 0
		endif       
		MON_DROP_BOOTYGROUP 5060016 0 0 0 0                   
	endif

	
	
	strvar ID1 
	STRVAR ID2 
	STRVAR ID3 
	STRVAR ID4 
	STRVAR ID5 
	STRVAR ID6 
	strvar Num
	
	GET_TEAM_PLAYER_ID 50 0 Num ID1 ID2 ID3 ID4 ID5 ID6

	GET_ID_PLAYER_INFO @ID1 8 30
	GET_ID_PLAYER_INFO @ID2 8 31
	GET_ID_PLAYER_INFO @ID3 8 32
	GET_ID_PLAYER_INFO @ID4 8 33
	GET_ID_PLAYER_INFO @ID5 8 34
	GET_ID_PLAYER_INFO @ID6 8 35
	if 50 = 0
		if 30 >= 70 or 31 >= 70 or 32 >= 70 or 33 >= 70 or 34 >= 70 or 35 >= 70 
			 MON_DROP_BOOTYGROUP 5241500 0 0 0 0
		endif
	else
		strvar lvp
		GET_ID_PLAYER_INFO 0 8 lvp
		if lvp >= 70
			MON_DROP_BOOTYGROUP 5241500 0 0 0 0
		endif
	endif
	
	;;;;BOSS ËÀÍö
	
	get_pworld_var 8 8 1 12 
	var 12 1
	set_pworld_var 8 8 @12 1
	
	
	; add_sys_msg_start
	; add_sys_msg 12=$@12$
	; set_sys_msg_mode 1 0 0
	; map_msg_info
endproc
