;高级技能书残卷

proc 1

	GET_ITEM_NUM_INPAK 5235000 1 11
	if 11 < 5
		add_sys_msg_start
		add_sys_msg 材料不足，请凑齐5个。
		set_sys_msg_mode 4 0 0
		msg_info
	else
		rand 20 32
		add 20 100
		switch 20
			case 100
				ADD_ITEM_NUM_INPAK 5231000 1 1 12
			endcase
			case 101
				ADD_ITEM_NUM_INPAK 5231002 1 1 12
			endcase
			case 102
				ADD_ITEM_NUM_INPAK 5231004 1 1 12
			endcase
			case 103
				ADD_ITEM_NUM_INPAK 5231006 1 1 12
			endcase
			case 104
				ADD_ITEM_NUM_INPAK 5231008 1 1 12
			endcase
			case 105
				ADD_ITEM_NUM_INPAK 5231010 1 1 12
			endcase
			case 106
				ADD_ITEM_NUM_INPAK 5231012 1 1 12
			endcase
			case 107
				ADD_ITEM_NUM_INPAK 5231014 1 1 12
			endcase
			case 108
				ADD_ITEM_NUM_INPAK 5231016 1 1 12
			endcase
			case 109
				ADD_ITEM_NUM_INPAK 5231018 1 1 12
			endcase
			case 110
				ADD_ITEM_NUM_INPAK 5231020 1 1 12
			endcase
			case 111
				ADD_ITEM_NUM_INPAK 5231022 1 1 12
			endcase
			
			case 112
				ADD_ITEM_NUM_INPAK 5231200 1 1 12
			endcase
			case 113
				ADD_ITEM_NUM_INPAK 5231202 1 1 12
			endcase
			case 114
				ADD_ITEM_NUM_INPAK 5231204 1 1 12
			endcase
			case 115
				ADD_ITEM_NUM_INPAK 5231207 1 1 12
			endcase
			case 116
				ADD_ITEM_NUM_INPAK 5231209 1 1 12
			endcase
			case 117
				ADD_ITEM_NUM_INPAK 5231211 1 1 12
			endcase
			case 118
				ADD_ITEM_NUM_INPAK 5231213 1 1 12
			endcase
			case 119
				ADD_ITEM_NUM_INPAK 5231215 1 1 12
			endcase
			case 120
				ADD_ITEM_NUM_INPAK 5231217 1 1 12
			endcase
			case 121
				ADD_ITEM_NUM_INPAK 5231219 1 1 12
			endcase
			case 122
				ADD_ITEM_NUM_INPAK 5231221 1 1 12
			endcase

			case 123
				ADD_ITEM_NUM_INPAK 5231400 1 1 12
			endcase
			case 124
				ADD_ITEM_NUM_INPAK 5231402 1 1 12
			endcase
			case 125
				ADD_ITEM_NUM_INPAK 5231404 1 1 12
			endcase
			case 126
				ADD_ITEM_NUM_INPAK 5231406 1 1 12
			endcase
			case 127
				ADD_ITEM_NUM_INPAK 5231408 1 1 12
			endcase
			case 128
				ADD_ITEM_NUM_INPAK 5231410 1 1 12
			endcase
			case 129
				ADD_ITEM_NUM_INPAK 5231412 1 1 12
			endcase
			case 130
				ADD_ITEM_NUM_INPAK 5231414 1 1 12
			endcase
			case 131
				ADD_ITEM_NUM_INPAK 5231416 1 1 12
			endcase
			case 132
				ADD_ITEM_NUM_INPAK 5231418 1 1 12
			endcase
	
		endswitch

		IF 12 = 0
			DEC_ITEM_NUM_INPAK 5235000 5 1
		Else
			add_sys_msg_start
			add_sys_msg 背包已满，请预留1个空位。
			set_sys_msg_mode 4 0 0
			msg_info
		ENDIF
	ENDIF
endproc	