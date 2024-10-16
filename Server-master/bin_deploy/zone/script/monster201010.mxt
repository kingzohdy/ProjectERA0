;1014003	精金刀	1
;1104003	黑木杖	2
;1204003	铁膛III	3
;1504003	修士杖	4


proc 2
	strvar map
	GET_PLAYER_MAPINFO 0 map 0 0 0
	if map != 10400
		return
	endif
	strvar pRet pNum rad rac
	GET_TEAM_PLAYER_ID pRet 0 pNum 10 20 30 40 50 60

	if pRet = 0
		if pNum = 2
			rand rad 2
			if rad = 0
				GET_ID_PLAYER_INFO @10 7 rac
			endif
			if rad = 1
				GET_ID_PLAYER_INFO @20 7 rac
			endif
		endif
		if pNum = 3
			rand rad 3
			if rad = 0
				GET_ID_PLAYER_INFO @10 7 rac
			endif
			if rad = 1
				GET_ID_PLAYER_INFO @20 7 rac
			endif
			if rad = 2
				GET_ID_PLAYER_INFO @30 7 rac
			endif
		endif
		if pNum = 4
			rand rad 4
			if rad = 0
				GET_ID_PLAYER_INFO @10 7 rac
			endif
			if rad = 1
				GET_ID_PLAYER_INFO @20 7 rac
			endif
			if rad = 2
				GET_ID_PLAYER_INFO @30 7 rac
			endif
			if rad = 3
				GET_ID_PLAYER_INFO @40 7 rac
			endif
		endif
		if pNum = 5
			rand rad 5
			if rad = 0
				GET_ID_PLAYER_INFO @10 7 rac
			endif
			if rad = 1
				GET_ID_PLAYER_INFO @20 7 rac
			endif
			if rad = 2
				GET_ID_PLAYER_INFO @30 7 rac
			endif
			if rad = 3
				GET_ID_PLAYER_INFO @40 7 rac
			endif
			if rad = 4
				GET_ID_PLAYER_INFO @50 7 rac
			endif
		endif
		if pNum = 6
			rand rad 6
			if rad = 0
				GET_ID_PLAYER_INFO @10 7 rac
			endif
			if rad = 1
				GET_ID_PLAYER_INFO @20 7 rac
			endif
			if rad = 2
				GET_ID_PLAYER_INFO @30 7 rac
			endif
			if rad = 3
				GET_ID_PLAYER_INFO @40 7 rac
			endif
			if rad = 4
				GET_ID_PLAYER_INFO @50 7 rac
			endif
			if rad = 5
				GET_ID_PLAYER_INFO @60 7 rac
			endif
		endif
		;取得职业的值之后，按随机到的职业掉落物品
		if rac = 1
			ADD_ITEM_ON_MAP 1 0 1014003 1 0 0 0 0
		endif
		if rac = 2
			ADD_ITEM_ON_MAP 1 0 1104003 1 0 0 0 0
		endif
		if rac = 3
			ADD_ITEM_ON_MAP 1 0 1204003 1 0 0 0 0
		endif
		if rac = 4
			ADD_ITEM_ON_MAP 1 0 1504003 1 0 0 0 0
		endif
	else
	;单人
		GET_ID_PLAYER_INFO 0 7 rac				
		if rac = 1
			ADD_ITEM_ON_MAP 1 0 1014003 1 0 0 0 0
		endif
		if rac = 2
			ADD_ITEM_ON_MAP 1 0 1104003 1 0 0 0 0
		endif
		if rac = 3
			ADD_ITEM_ON_MAP 1 0 1204003 1 0 0 0 0
		endif
		if rac = 4
			ADD_ITEM_ON_MAP 1 0 1504003 1 0 0 0 0
		endif
	endif
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	strvar Ret1
	MON_OWNER_TASK_CHECK 1327 Ret1
	
	if Ret1 = 0
	
		strvar Ret mid1 mid2 mid3 mid4 mid5 mid6 Map1 Map2 Map3 Map4 Map5 Map6
		strvar num
		GET_TEAM_PLAYER_ID Ret 0 num mid1 mid2 mid3 mid4 mid5 mid6
		GET_ID_PLAYER_INFO @mid1 5 Map1
		GET_ID_PLAYER_INFO @mid2 5 Map2
		GET_ID_PLAYER_INFO @mid3 5 Map3
		GET_ID_PLAYER_INFO @mid4 5 Map4
		GET_ID_PLAYER_INFO @mid5 5 Map5
		GET_ID_PLAYER_INFO @mid6 5 Map6
		
		
		if Ret = 0
		
			if num = 2
				if Map1 = 10400
					SET_PLAYER_CURR @mid1 10
					ADD_ITEM_NUM_INPAK 5300395 1 1 0
				endif
				if Map2 = 10400
					SET_PLAYER_CURR @mid2 20
					ADD_ITEM_NUM_INPAK 5300395 1 1 0
				endif
			endif
			if num = 3
				if Map1 = 10400
					SET_PLAYER_CURR @mid1 10
					ADD_ITEM_NUM_INPAK 5300395 1 1 0
				endif
				if Map2 = 10400
					SET_PLAYER_CURR @mid2 20
					ADD_ITEM_NUM_INPAK 5300395 1 1 0
				endif
				if Map3 = 10400
					SET_PLAYER_CURR @mid3 1
					ADD_ITEM_NUM_INPAK 5300395 1 1 0
				endif
			endif
			if num = 4
				if Map1 = 10400
					SET_PLAYER_CURR @mid1 10
					ADD_ITEM_NUM_INPAK 5300395 1 1 0
				endif
				if Map2 = 10400
					SET_PLAYER_CURR @mid2 20
					ADD_ITEM_NUM_INPAK 5300395 1 1 0
				endif
				if Map3 = 10400
					SET_PLAYER_CURR @mid3 1
					ADD_ITEM_NUM_INPAK 5300395 1 1 0
				endif
				if Map4 = 10400
					SET_PLAYER_CURR @mid4 1
					ADD_ITEM_NUM_INPAK 5300395 1 1 0
					
				endif
			endif
			if num = 5
				if Map1 = 10400
					SET_PLAYER_CURR @mid1 10
					ADD_ITEM_NUM_INPAK 5300395 1 1 0
				endif
				if Map2 = 10400
					SET_PLAYER_CURR @mid2 20
					ADD_ITEM_NUM_INPAK 5300395 1 1 0
				endif
				if Map3 = 10400
					SET_PLAYER_CURR @mid3 1
					ADD_ITEM_NUM_INPAK 5300395 1 1 0
				endif
				if Map4 = 10400
					SET_PLAYER_CURR @mid4 1
					ADD_ITEM_NUM_INPAK 5300395 1 1 0
				endif
				if Map5 = 10400
					SET_PLAYER_CURR @mid5 1
					ADD_ITEM_NUM_INPAK 5300395 1 1 0
					
				endif
			endif
			if num = 6
				if Map1 = 10400
					SET_PLAYER_CURR @mid1 10
					ADD_ITEM_NUM_INPAK 5300395 1 1 0
				endif
				if Map2 = 10400
					SET_PLAYER_CURR @mid2 20
					ADD_ITEM_NUM_INPAK 5300395 1 1 0
				endif
				if Map3 = 10400
					SET_PLAYER_CURR @mid3 1
					ADD_ITEM_NUM_INPAK 5300395 1 1 0
				endif
				if Map4 = 10400
					SET_PLAYER_CURR @mid4 1
					ADD_ITEM_NUM_INPAK 5300395 1 1 0
				endif
				if Map5 = 10400
					SET_PLAYER_CURR @mid5 1
					ADD_ITEM_NUM_INPAK 5300395 1 1 0
				endif
				if Map6 = 10400
					SET_PLAYER_CURR @mid6 1
					ADD_ITEM_NUM_INPAK 5300395 1 1 0
				
				endif
			endif
		else
			
			ADD_ITEM_NUM_INPAK 5300395 1 1 0
		endif
	
	endif
	
endproc  









