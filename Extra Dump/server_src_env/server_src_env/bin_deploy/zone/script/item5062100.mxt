;驯羊鞭子 护送 运镖 任务

proc 1
	STRVAR Overlay Ret MonMemID	fConvoy
	;GET_MON_INFO #返回标志 #定义id #实例id #血量 #地图id #地图实例 #坐标X #Y
	GET_MON_INFO 0 8 MonMemID 0 0 0 0 0

	;IS_MY_CONVOY_MON  #怪物实例ＩＤ #返回标志
	IS_MY_CONVOY_MON  @MonMemID fConvoy

	var 6 0
	var 5 0

	If fConvoy = 0
		If 8 = 264000 Or 8 = 264001 Or 8 = 264002 Or 8 = 264003 Or 8 = 264004 Or 8 = 264005 Or 8 = 264006 Or 8 = 264007 Or 8 = 264008 Or 8 = 264009 Or 8 = 264010 Or 8 = 264011 Or 8 = 264012 Or 8 = 264013 Or 8 = 264014 Or 8 = 264015
		
					;有减速
					GET_ID_MON_STATUS_INFO  @MonMemID 1203 Ret 0 6 0 0
					if Ret = 0
						return
					endif
								
					;获得加速状态信息
					GET_ID_MON_STATUS_INFO  @MonMemID 1202 Ret 0 Overlay 0 0
					;没有这个状态
					if Ret = 0
						;减速
						If Overlay = 3
							TARGET_ADD_STATUS 3 @MonMemID 1203 1 6 0
							add_sys_msg_start
							add_sys_msg 货羊已疲惫不堪，需要休息，移动速度降低！
							set_sys_msg_mode 1 0 0
							MSG_INFO
							
							return
						endif
					endif
					
					;加加速
					TARGET_ADD_STATUS 3 @MonMemID 1202 1 4 0
					
					GET_ID_MON_STATUS_INFO  @MonMemID 1202 Ret 0 Overlay 0 0
					if Ret = -1
						return
					endif


					;4次就满了
					If Overlay = 3
						add_sys_msg_start
						add_sys_msg 这货羊有点疯，速度貌似到极限了！
						set_sys_msg_mode 1 0 0
						MSG_INFO
					endif
			

		Else
			add_sys_msg_start
			add_sys_msg 请在单人护送时，对自己的货羊使用！
			set_sys_msg_mode 1 0 0
			msg_info
		endif
		
	Else
		add_sys_msg_start
		add_sys_msg 请在单人护送时，对自己的货羊使用！
		set_sys_msg_mode 1 0 0
		msg_info
	endif

endproc			