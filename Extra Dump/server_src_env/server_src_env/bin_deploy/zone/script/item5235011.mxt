;高级技能书残卷

proc 1

	GET_ITEM_NUM_INPAK 5235011 1 11
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
				ADD_ITEM_NUM_INPAK 5231101 1 1 12
			endcase
			case 101
				ADD_ITEM_NUM_INPAK 5231103 1 1 12
			endcase
			case 102
				ADD_ITEM_NUM_INPAK 5231105 1 1 12
			endcase
			case 103
				ADD_ITEM_NUM_INPAK 5231107 1 1 12
			endcase
			case 104
				ADD_ITEM_NUM_INPAK 5231109 1 1 12
			endcase
			case 105
				ADD_ITEM_NUM_INPAK 5231111 1 1 12
			endcase
			case 106
				ADD_ITEM_NUM_INPAK 5231113 1 1 12
			endcase
			case 107
				ADD_ITEM_NUM_INPAK 5231115 1 1 12
			endcase
			case 108
				ADD_ITEM_NUM_INPAK 5231117 1 1 12
			endcase
			case 109
				ADD_ITEM_NUM_INPAK 5231119 1 1 12
			endcase
			case 110
				ADD_ITEM_NUM_INPAK 5231121 1 1 12
			endcase
			case 111
				ADD_ITEM_NUM_INPAK 5231123 1 1 12
			endcase

			case 112
				ADD_ITEM_NUM_INPAK 5231301 1 1 12
			endcase
			case 113
				ADD_ITEM_NUM_INPAK 5231303 1 1 12
			endcase
			case 114
				ADD_ITEM_NUM_INPAK 5231305 1 1 12
			endcase
			case 115
				ADD_ITEM_NUM_INPAK 5231308 1 1 12
			endcase
			case 116
				ADD_ITEM_NUM_INPAK 5231310 1 1 12
			endcase
			case 117
				ADD_ITEM_NUM_INPAK 5231312 1 1 12
			endcase
			case 118
				ADD_ITEM_NUM_INPAK 5231314 1 1 12
			endcase
			case 119
				ADD_ITEM_NUM_INPAK 5231316 1 1 12
			endcase
			case 120
				ADD_ITEM_NUM_INPAK 5231318 1 1 12
			endcase
			case 121
				ADD_ITEM_NUM_INPAK 5231320 1 1 12
			endcase
			case 122
				ADD_ITEM_NUM_INPAK 5231322 1 1 12
			endcase

			case 123
				ADD_ITEM_NUM_INPAK 5231501 1 1 12
			endcase
			case 124
				ADD_ITEM_NUM_INPAK 5231503 1 1 12
			endcase
			case 125
				ADD_ITEM_NUM_INPAK 5231505 1 1 12
			endcase
			case 126
				ADD_ITEM_NUM_INPAK 5231507 1 1 12
			endcase
			case 127
				ADD_ITEM_NUM_INPAK 5231509 1 1 12
			endcase
			case 128
				ADD_ITEM_NUM_INPAK 5231511 1 1 12
			endcase
			case 129
				ADD_ITEM_NUM_INPAK 5231513 1 1 12
			endcase
			case 130
				ADD_ITEM_NUM_INPAK 5231515 1 1 12
			endcase
			case 131
				ADD_ITEM_NUM_INPAK 5231517 1 1 12
			endcase
			case 132
				ADD_ITEM_NUM_INPAK 5231519 1 1 12
			endcase	
		endswitch

		IF 12 = 0
			DEC_ITEM_NUM_INPAK 5235011 5 1
		Else
			add_sys_msg_start
			add_sys_msg 背包已满，请预留1个空位。
			set_sys_msg_mode 4 0 0
			msg_info
		ENDIF
	ENDIF
endproc	