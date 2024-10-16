;;;¶ñÄ§Ö®Ó°


proc 2
	ADD_XP 10000 1
;	CIRCLE_NEW_MON 202006 10 7779 8542 500 1 10 0 0 0 0 0 0
;	NEW_TMP_ITEM 1440240 7779 8542 0 0 0 0


	strvar Mid X Y Sot
	GET_MON_INFO 1 1 Mid 1 1 1 X Y

	get_pworld_var 11 8 1 Sot
	if Sot != 10	
		
		NEW_LIFE_MON 202007 1 @X @Y 1 1 1 1 12 1 1

		strvar NumT1
		get_pworld_var 9 8 1 NumT1
		var NumT1 1
		set_pworld_var 9 8 @NumT1 1
	endif
endproc 