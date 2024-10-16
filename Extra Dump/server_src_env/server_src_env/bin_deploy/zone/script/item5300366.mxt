;伊利亚斯的疗伤药膏



proc 1
	get_player_mapinfo 0 1 2 3 4
	if 2 = 2300
;			effect 0 0 playEffct(6800,6001,"effect\\entity\\9544_16.ent")
			GET_MON_INFO 0 20 21 22 23 24 25 26
			dec_item_num_inpak 5300366 1 0
			add_item_num_inpak 5300345 1 1 0

			rand 100 5
			add 100 100
			switch 100
				case 100
					MON_SPEECH @21 11 1 4 感觉好多了，谢谢你，#Name#。
				endcase
				case 101
					MON_SPEECH @21 11 1 4 我应该很快就会康复了吧？
				endcase
				case 102
					MON_SPEECH @21 11 1 4 多谢你，#Name#，此时此刻见到你是唯一一件值得高兴的事了。
				endcase
				
				case 103
					MON_SPEECH @21 11 1 4 真是麻烦您了，这个药膏冰凉的触感让人心神宁静，能再多来点吗？
				endcase
				
				case 104
					MON_SPEECH @21 11 1 4 很久没有见到像你这样有耐心的年轻人了，真希望和你做好朋友。
				endcase
			endswitch
			del_mon 10			
			
	  else
			add_sys_msg_start
			add_sys_msg 目标不在本地图中！
			set_sys_msg_mode 4 0 0
			msg_info
	  endif

endproc


