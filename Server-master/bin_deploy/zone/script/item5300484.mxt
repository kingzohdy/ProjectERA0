;源血之柱 删怪 刷友好怪


Proc 1
	STRVAR id1 id2 x y hp a

		GET_MON_INFO 1 1 id2 0 0 0 x y
		GET_MON_HP_PERCENT @id2 hp
		if hp < 40
			DESTROY_MON 1
				if 1 = -1
					return
				endif
				NEW_LIFE_MON 317012 1 @x @y 50 0 0 0 6 1 id1
				if 1 = -1
					return
				else
					RAND a 2
					switch a
						case 0
							MON_SPEECH @id1 1 0 1 感觉好像做了一个长长的梦……
						endcase	
						case 1
							MON_SPEECH @id1 1 0 1 你是谁？我这是怎么了？
						endcase
						case 2
							MON_SPEECH @id1 1 0 1 好累啊，脑袋空空的，酋长大人呢？
						endcase	
					endswitch
					ADD_ITEM_NUM_INPAK 5300470 1 0 1
					if 1 = -1
						return
					ENDIF
				endif
		else
			add_sys_msg_start
			add_sys_msg 哎呀现在还用不了，你再多打它几下！
			set_sys_msg_mode 1 0 0
			MSG_INFO			
		endif
endproc							