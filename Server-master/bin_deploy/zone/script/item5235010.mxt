;高级技能书残卷

proc 1

	GET_ITEM_NUM_INPAK 5235010 1 11
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
				ADD_ITEM_NUM_INPAK 5231100 1 1 12
			endcase
			case 101
				ADD_ITEM_NUM_INPAK 5231102 1 1 12
			endcase
			case 102
				ADD_ITEM_NUM_INPAK 5231104 1 1 12
			endcase
			case 103
				ADD_ITEM_NUM_INPAK 5231106 1 1 12
			endcase
			case 104
				ADD_ITEM_NUM_INPAK 5231108 1 1 12
			endcase
			case 105
				ADD_ITEM_NUM_INPAK 5231110 1 1 12
			endcase
			case 106
				ADD_ITEM_NUM_INPAK 5231112 1 1 12
			endcase
			case 107
				ADD_ITEM_NUM_INPAK 5231114 1 1 12
			endcase
			case 108
				ADD_ITEM_NUM_INPAK 5231116 1 1 12
			endcase
			case 109
				ADD_ITEM_NUM_INPAK 5231118 1 1 12
			endcase
			case 110
				ADD_ITEM_NUM_INPAK 5231120 1 1 12
			endcase
			case 111
				ADD_ITEM_NUM_INPAK 5231122 1 1 12
			endcase

			case 112
				ADD_ITEM_NUM_INPAK 5231300 1 1 12
			endcase
			case 113
				ADD_ITEM_NUM_INPAK 5231302 1 1 12
			endcase
			case 114
				ADD_ITEM_NUM_INPAK 5231304 1 1 12
			endcase
			case 115
				ADD_ITEM_NUM_INPAK 5231306 1 1 12
			endcase
			case 116
				ADD_ITEM_NUM_INPAK 5231309 1 1 12
			endcase
			case 117
				ADD_ITEM_NUM_INPAK 5231311 1 1 12
			endcase
			case 118
				ADD_ITEM_NUM_INPAK 5231313 1 1 12
			endcase
			case 119
				ADD_ITEM_NUM_INPAK 5231315 1 1 12
			endcase
			case 120
				ADD_ITEM_NUM_INPAK 5231317 1 1 12
			endcase
			case 121
				ADD_ITEM_NUM_INPAK 5231319 1 1 12
			endcase
			case 122
				ADD_ITEM_NUM_INPAK 5231321 1 1 12
			endcase

			case 123
				ADD_ITEM_NUM_INPAK 5231500 1 1 12
			endcase
			case 124
				ADD_ITEM_NUM_INPAK 5231502 1 1 12
			endcase
			case 125
				ADD_ITEM_NUM_INPAK 5231504 1 1 12
			endcase
			case 126
				ADD_ITEM_NUM_INPAK 5231506 1 1 12
			endcase
			case 127
				ADD_ITEM_NUM_INPAK 5231508 1 1 12
			endcase
			case 128
				ADD_ITEM_NUM_INPAK 5231510 1 1 12
			endcase
			case 129
				ADD_ITEM_NUM_INPAK 5231512 1 1 12
			endcase
			case 130
				ADD_ITEM_NUM_INPAK 5231514 1 1 12
			endcase
			case 131
				ADD_ITEM_NUM_INPAK 5231516 1 1 12
			endcase
			case 132
				ADD_ITEM_NUM_INPAK 5231518 1 1 12
			endcase	
		endswitch

		IF 12 = 0
			DEC_ITEM_NUM_INPAK 5235010 5 1
		Else
			add_sys_msg_start
			add_sys_msg 背包已满，请预留1个空位。
			set_sys_msg_mode 4 0 0
			msg_info
		ENDIF
	ENDIF
endproc	