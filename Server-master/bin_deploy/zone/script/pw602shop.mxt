;公会守护枢纽活动    ,副本里面的商店
proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b我叫$_npc$，我可以提供一些简单的机械道具给你，当然是需要钱的；我也能帮你修理机甲兽，不过要收费的，哈哈！。
	option 300 #修理机甲兽
	option 400 #购买机械道具
	talk
endproc
proc 300
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b希望公会枢纽、炮塔与您驾驶的机械更加强力吗？我这提供各种属性提升的途径。
	add_talk #b#b机甲兽损坏了？别担心，我这也可以帮你修理。
	option 301 修理低级机甲兽；费用：200绑银
	option 302 修理强化机甲兽；费用：1000绑银
	option 303 修理强力机甲兽；费用：5000绑银
	talk
endproc
proc 301
	choose 2 311 312 修理低级机甲兽费用200绑银，确认修理？
endproc
proc 302
	choose 2 321 322 修理强化机甲兽费用1000绑银，确认修理？
endproc
proc 303
	choose 2 331 332 修理强力机甲兽费用5000绑银，确认修理？
endproc


proc 311
	strvar ret
	get_id_player_info 0 21 ret
	get_id_player_info 0 22 30
	if 30 < 200 and @ret >= 200
		choose 1 313 312 绑银消耗已达到每日最大上限，继续操作将直接消耗银币，是否继续操作？
	elseif ret < 200
		choose 1 313 312 绑银不足，继续操作将直接消耗银币，是否继续操作？
	else
		call 313
	endif
endproc 


proc 313
        strvar Num Retm
        GET_ITEM_NUM_INPAK 5300881 1 Num
        if Num > 0
                strvar retmid
                GET_ROLE_RIDE_ON_MACHINE 0 retmid
                if retmid = 0
                        DEC_MONEY_TEST 0 200 21 Retm
                        if Retm = -1
                                ADD_SYS_MSG_START
                                ADD_SYS_MSG 银币不足，修理失败!
                                SET_SYS_MSG_MODE 3 0 0
                                MSG_INFO
                        else
                                strvar Ret
                                DEC_ITEM_NUM_INPAK 5300881 1 Ret
                                if Ret = 0
                                        strvar Retitem
                                        add_item_num_inpak 5300881 1 1 Retitem
                                        if Retitem = 0
                                                DEC_MONEY 0 200 21 10
                                        endif
                                endif
                        endif
                else
                        ADD_SYS_MSG_START
                        ADD_SYS_MSG 机械在骑乘状态，不可修理！
                        SET_SYS_MSG_MODE 3 0 0
                        MSG_INFO
                endif
        else
                ADD_SYS_MSG_START
                ADD_SYS_MSG 修理失败，您背包内没有需要修理的低级机甲兽。
                SET_SYS_MSG_MODE 3 0 0
                MSG_INFO
        endif	
endproc
proc 312
endproc

proc 321
	strvar ret
	get_id_player_info 0 21 ret
	get_id_player_info 0 22 30
	if 30 < 1000 and @ret >= 1000
		choose 1 323 312 绑银消耗已达到每日最大上限，继续操作将直接消耗银币，是否继续操作？
	elseif ret < 1000
		choose 1 323 312 绑银不足，继续操作将直接消耗银币，是否继续操作？
	else
		call 323
	endif
endproc 
proc 323
        strvar Num Retm
        GET_ITEM_NUM_INPAK 5300882 1 Num
        if Num > 0
                strvar retmid
                GET_ROLE_RIDE_ON_MACHINE 0 retmid
                if retmid = 0
                        DEC_MONEY_TEST 0 1000 21 Retm
                        if Retm = -1
                                ADD_SYS_MSG_START
                                ADD_SYS_MSG 银币不足，修理失败!
                                SET_SYS_MSG_MODE 3 0 0
                                MSG_INFO
                        else
                                strvar Ret
                                DEC_ITEM_NUM_INPAK 5300882 1 Ret
                                if Ret = 0
                                        strvar Retitem
                                        add_item_num_inpak 5300882 1 1 Retitem
                                        if Retitem = 0
                                                DEC_MONEY 0 1000 21 10
                                        endif
                                endif
                        endif
                else
                        ADD_SYS_MSG_START
                        ADD_SYS_MSG 机械在骑乘状态，不可修理！
                        SET_SYS_MSG_MODE 3 0 0
                        MSG_INFO
                endif
        else
			ADD_SYS_MSG_START
			ADD_SYS_MSG 修理失败，您背包内没有需要修理的强化机甲兽。
			SET_SYS_MSG_MODE 3 0 0
			MSG_INFO
        endif	
endproc
proc 322
endproc

proc 331
	strvar ret
	get_id_player_info 0 21 ret
	get_id_player_info 0 22 30
	if 30 < 5000 and @ret >= 5000
		choose 1 333 312 绑银消耗已达到每日最大上限，继续操作将直接消耗银币，是否继续操作？
	elseif ret < 5000
		choose 1 333 312 绑银不足，继续操作将直接消耗银币，是否继续操作？
	else
		call 333
	endif
endproc 
proc 333
        strvar Num Retm
        GET_ITEM_NUM_INPAK 5300883 1 Num
        if Num > 0
                strvar retmid
                GET_ROLE_RIDE_ON_MACHINE 0 retmid
                if retmid = 0
                        DEC_MONEY_TEST 0 5000 21 Retm
                        if Retm = -1
                                ADD_SYS_MSG_START
                                ADD_SYS_MSG 银币不足，修理失败!
                                SET_SYS_MSG_MODE 3 0 0
                                MSG_INFO
                        else
                                strvar Ret
                                DEC_ITEM_NUM_INPAK 5300883 1 Ret
                                if Ret = 0
                                        strvar Retitem
                                        add_item_num_inpak 5300883 1 1 Retitem
                                        if Retitem = 0
                                                DEC_MONEY 0 5000 21 10
                                        endif
                                endif
                        endif
                else
                        ADD_SYS_MSG_START
                        ADD_SYS_MSG 机械在骑乘状态，不可修理！
                        SET_SYS_MSG_MODE 3 0 0
                        MSG_INFO
                endif
        else
                ADD_SYS_MSG_START
                ADD_SYS_MSG 修理失败，您背包内没有需要修理的强力机甲兽。
                SET_SYS_MSG_MODE 3 0 0
                MSG_INFO
        endif	
endproc
proc 332
endproc
proc 400
	open_npc_shop
endproc
