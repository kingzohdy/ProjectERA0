

proc 1


	DIALOG 1
	strvar Ret
	ENTER_WORLD_CITY_CHECK Ret
	if Ret = -1
		add_talk $_npc$:
		add_talk #b#b成就霸主的时刻即将到来，带上你的兄弟姐妹们征战沙场吧。将时间城的中控中心占领，你们就将是人类最后都城的霸主！
		talk
		return
	endif

		add_talk $_npc$:
		add_talk #b#b成就霸主的时刻即将到来，带上你的兄弟姐妹们征战沙场吧。将时间城的中控中心占领，你们就将是人类最后都城的霸主！

			option 200 战争规则
			option 201 占领奖励
			option 202 #进入中控中心地图
			option 203 #关闭
				
	talk
endproc 
proc 200
	DIALOG 1
	add_talk #b#b#b#b#b#b#b#b#b战争规则
	add_talk 战争时间：
	add_talk #b#b城主争霸战将于每周六晚20：00开始，21：30分结束。
	add_talk 战争规则：
	add_talk #b#b1.只有据点占领公会可在中控中心地图中争夺建筑控制权；
	add_talk #b#b2.在战争地图中，争夺占领、摧毁或成功守护城防建筑都可获得战争积分。
	add_talk 胜负条件：
	add_talk #b#b在战争结束时，根据各公会战争积分判定胜负，积分排名第1公会，获得最终胜利，其他情况：
	add_talk #b#b 1.如防守方公会与其他公会分数相同，则防守方胜利；
	add_talk #b#b 2.如进攻方公会之间出现平分，则按照分数达到时间，靠前者获胜。
	add_talk #b#b重要提示：在中控中心中，争夺3座终端控制器控制权可获得大量战争积分！
	
	option 1 返回
	
	talk
endproc
proc 201
	DIALOG 1
	add_talk #b#b#b#b#b占领时间城奖励
	add_talk 1.当前占领公会，每天会长可领取一次占领资金奖励（银币）。
	add_talk 2.占领后在整点时系统自动奖励公会一定量的资源。
	add_talk 3.所有当前公会成员，在时间城NPC处购买物品、维修装备，可享受一定折扣优惠。
	add_talk 4.所有当前公会成员，在[急你所急航空]世界航线传送中，拥有更多的地图传送点
	add_talk 5.时间城城主（占领公会会长）个人雕像。
	add_talk 6.当前占领公会的公会名特殊化。
	add_talk 7.注：以上为主要特权与奖励。（连续占领时间城的时间越久，拥有的特权与奖励越多）。

	option 1 返回
	
	talk
endproc
proc 203
	return
endproc



proc 202

	strvar Player
	IS_WORLD_CITY_OWNER Player
	;城市所有者随时都可以进入
	if Player = 1
		call 204
		return
	endif
	
	
	strvar Ret
	GET_EVENT_STAT Ret 45
	;非城市所有者开战一小时前不可进入
	if Ret = 1
		add_sys_msg_start
		add_sys_msg 战争即将开始，当前时间段内禁止入内，战争开始后才可进入
		set_sys_msg_mode 4 0 0
		msg_info
		return		
	else
		call 204
	endif

endproc 

proc 204
	strvar Rad
	rand Rad 21
	switch Rad
		case 0
			TRANSMIT 4404 21616	7372 0
		endcase
		case 1
			TRANSMIT 4404 21544	7135 0
		endcase
		case 2
			TRANSMIT 4404 21234	6717 0
		endcase
		case 3
			TRANSMIT 4404 21063	6398 0
		endcase
		case 4
			TRANSMIT 4404 21179	6227 0
		endcase
		case 5
			TRANSMIT 4404 21465	5915 0
		endcase
		case 6
			TRANSMIT 4404 22142	5839 0
		endcase
		case 7
			TRANSMIT 4404 22484	6122 0
		endcase
		case 8
			TRANSMIT 4404 22514	6967 0
		endcase
		case 9
			TRANSMIT 4404 22527	6582 0
		endcase
		case 10
			TRANSMIT 4404 21938	6549 0
		endcase
		case 11
			TRANSMIT 4404 21629	6316 0
		endcase
		case 12
			TRANSMIT 4404 21777	6234 0
		endcase
		case 13
			TRANSMIT 4404 22139	7076 0
		endcase
		case 14
			TRANSMIT 4404 22103	7352 0
		endcase
		case 15
			TRANSMIT 4404 21678	6757 0
		endcase
		case 16
			TRANSMIT 4404 21149	6983 0
		endcase
		case 17
			TRANSMIT 4404 20929	6701 0
		endcase
		case 18
			TRANSMIT 4404 21797	5800 0
		endcase
		case 19
			TRANSMIT 4404 22774	6447 0
		endcase
		case 20
			TRANSMIT 4404 22234	6257 0
		endcase
	endswitch
endproc 










