;高级技能书残卷

proc 1

	GET_ITEM_NUM_INPAK 5235001 1 11
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
				ADD_ITEM_NUM_INPAK 5231001 1 1 12
			endcase
			case 101
				ADD_ITEM_NUM_INPAK 5231003 1 1 12
			endcase
			case 102
				ADD_ITEM_NUM_INPAK 5231005 1 1 12
			endcase
			case 103
				ADD_ITEM_NUM_INPAK 5231007 1 1 12
			endcase
			case 104
				ADD_ITEM_NUM_INPAK 5231009 1 1 12
			endcase
			case 105
				ADD_ITEM_NUM_INPAK 5231011 1 1 12
			endcase
			case 106
				ADD_ITEM_NUM_INPAK 5231013 1 1 12
			endcase
			case 107
				ADD_ITEM_NUM_INPAK 5231015 1 1 12
			endcase
			case 108
				ADD_ITEM_NUM_INPAK 5231017 1 1 12
			endcase
			case 109
				ADD_ITEM_NUM_INPAK 5231019 1 1 12
			endcase
			case 110
				ADD_ITEM_NUM_INPAK 5231021 1 1 12
			endcase
			case 111
				ADD_ITEM_NUM_INPAK 5231023 1 1 12
			endcase
			
			case 112
				ADD_ITEM_NUM_INPAK 5231201 1 1 12
			endcase
			case 113
				ADD_ITEM_NUM_INPAK 5231203 1 1 12
			endcase
			case 114
				ADD_ITEM_NUM_INPAK 5231205 1 1 12
			endcase
			case 115
				ADD_ITEM_NUM_INPAK 5231208 1 1 12
			endcase
			case 116
				ADD_ITEM_NUM_INPAK 5231210 1 1 12
			endcase
			case 117
				ADD_ITEM_NUM_INPAK 5231212 1 1 12
			endcase
			case 118
				ADD_ITEM_NUM_INPAK 5231214 1 1 12
			endcase
			case 119
				ADD_ITEM_NUM_INPAK 5231216 1 1 12
			endcase
			case 120
				ADD_ITEM_NUM_INPAK 5231218 1 1 12
			endcase
			case 121
				ADD_ITEM_NUM_INPAK 5231220 1 1 12
			endcase
			case 122
				ADD_ITEM_NUM_INPAK 5231222 1 1 12
			endcase

			case 123
				ADD_ITEM_NUM_INPAK 5231401 1 1 12
			endcase
			case 124
				ADD_ITEM_NUM_INPAK 5231403 1 1 12
			endcase
			case 125
				ADD_ITEM_NUM_INPAK 5231405 1 1 12
			endcase
			case 126
				ADD_ITEM_NUM_INPAK 5231407 1 1 12
			endcase
			case 127
				ADD_ITEM_NUM_INPAK 5231409 1 1 12
			endcase
			case 128
				ADD_ITEM_NUM_INPAK 5231411 1 1 12
			endcase
			case 129
				ADD_ITEM_NUM_INPAK 5231413 1 1 12
			endcase
			case 130
				ADD_ITEM_NUM_INPAK 5231415 1 1 12
			endcase
			case 131
				ADD_ITEM_NUM_INPAK 5231417 1 1 12
			endcase
			case 132
				ADD_ITEM_NUM_INPAK 5231419 1 1 12
			endcase
	
		endswitch

		IF 12 = 0
			DEC_ITEM_NUM_INPAK 5235001 5 1
		Else
			add_sys_msg_start
			add_sys_msg 背包已满，请预留1个空位。
			set_sys_msg_mode 4 0 0
			msg_info
		ENDIF
	ENDIF
endproc	