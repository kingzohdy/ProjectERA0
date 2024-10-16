


;;;;±¦Îï¹Ö


proc 2

	strvar ret lv
	get_mon_level ret lv
	if ret = 0
		
		if lv >= 30 and lv < 40
			MON_DROP_BOOTYGROUP 2060 2069 2078 2087 0
		endif
		if lv >= 40 and lv < 50
			MON_DROP_BOOTYGROUP 2061 2070 2079 2088 0
		endif
		if lv >= 50 and lv < 60
			MON_DROP_BOOTYGROUP 2062 2071 2080 2089 0
		endif
		if lv >= 60 and lv < 70
			MON_DROP_BOOTYGROUP 2063 2072 2081 2090 0
		endif
		if lv >= 70 and lv < 80
			MON_DROP_BOOTYGROUP 2064 2073 2082 2091 0
		endif
		if lv >= 80 and lv < 90
			MON_DROP_BOOTYGROUP 2065 2074 2083 2092 0
		endif
		if lv >= 90 and lv < 100
			MON_DROP_BOOTYGROUP 2066 2075 2084 2093 0
		endif
		if lv >= 100 and lv < 110
			MON_DROP_BOOTYGROUP 2067 2076 2085 2094 0
		endif
		if lv >= 110 and lv <= 120
			MON_DROP_BOOTYGROUP 2068 2077 2076 2095 0
		endif
	endif


endproc  

