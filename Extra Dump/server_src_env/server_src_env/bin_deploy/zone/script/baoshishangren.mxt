

proc 1
	DIALOG 1
	add_talk $_npc$:
;	add_talk #b#b自治联盟把我们这些以发掘开采宝石的人称作“宝石猎人”，在这里希望能让你找到心仪的宝石。
;	add_talk #b#b在我身后的石阵被称作“宝石鸣聚阵”，它有一个非常特殊的用途，只有在它附近才能够进行宝石升级，希望这些信息对你有用。
	add_talk #b#b据说危险的失落之域能寻找到污染的高能凝晶,净化后的高能凝晶,能够给三级或以上品质的宝石进行充能,提升宝石的星级.
	add_talk #b#b如果有冒险者找到这种污染的高能凝晶,可以来我这里,利用宝石魔阵净化它,每净化一份污染的高能凝晶,需要消耗一颗宝石碎片,受限于精力,每人每天最多只能净化50份.
	add_talk #b#b#c0a6400玩家第一次成功净化污染的高能凝晶后,每天可净化的份额数量会累积,比如您今天只净化了10份,那么第二天可以净化的份额为:第一天剩余的40份,加上第二天的50份,等于90份!最多累积500份,超过部分不会再累积
	
	; exchg_item_get 5213411 40
	; if 40 != -1
		; add_talk #b#b高能凝晶可净化次数：$@40$
	; endif

	
	strvar time
	get_role_var 41 32 1 time
	is_same_day @time ret
	if ret = -1
		var 15 0
		set_role_var 40 8 @15 1
		
	endif
	task_add_option 3403 1 100
	; task_add_option 4160 1 102
	; task_add_option 4161 1 103
	; task_add_option 4162 1 104
	; task_add_option 4163 1 105
	; task_add_option 4164 1 106
	; task_add_option 4165 1 107
	; task_add_option 4166 1 108
	; task_add_option 4167 1 109
	; task_add_option 4168 1 110
	; task_add_option 4169 1 111
	; task_add_option 4170 1 112
	; task_add_option 4171 1 113
	; task_add_option 4172 1 114
	; task_add_option 4173 1 115
	; task_add_option 4174 1 116
	; task_add_option 4175 1 117
	; task_add_option 4176 1 118
	; task_add_option 4177 1 119
	; task_add_option 4178 1 120
	; task_add_option 4179 1 121
	
	
	TASKCYCLE_ADD_OPTION 140 1 101



	OPTION 200 #(买卖)打开商店

;	OPTION 201 #宝石升级

;	OPTION 202 #宝石镶嵌

;	OPTION 203 #宝石摘取

	strvar Ret
	task_stat 3677 1 Ret
	if Ret = 2
		option 300 我替公会送来了你需要的制作工具

	endif
	option 500 查询可净化次数
	option 400 净化10份污染的高能凝晶	
	
	task_stat 4492 1 20
	if 20 = 2
		option 250 #向宝石商人希伯利赠送一个粽子
	endif
	;option 410 净化20份污染的高能凝晶
	;option 420 净化50份污染的高能凝晶

	talk
endproc 

PROC 100
	task_do_option 3403 1
ENDPROC

proc 200
	OPEN_NPC_SHOP
ENDPROC 
proc 201
	open_ui 14
ENDPROC 
proc 202
	open_ui 15
ENDPROC 
proc 203
	open_ui 16
ENDPROC 
proc 204
	open_npc_shop
endproc 
 PROC 101	
	TASKCYCLE_DO_OPTION 140 1
 ENDPROC

PROC 102
	task_do_option 4160 1
ENDPROC
PROC 103
	task_do_option 4161 1
ENDPROC
PROC 104
	task_do_option 4162 1
ENDPROC
PROC 105
	task_do_option 4163 1
ENDPROC
PROC 106
	task_do_option 4164 1
ENDPROC
PROC 107
	task_do_option 4165 1
ENDPROC
PROC 108
	task_do_option 4166 1
ENDPROC
PROC 109
	task_do_option 4167 1
ENDPROC
PROC 110
	task_do_option 4168 1
ENDPROC
PROC 111
	task_do_option 4169 1
ENDPROC
PROC 112
	task_do_option 4170 1
ENDPROC
PROC 113
	task_do_option 4171 1
ENDPROC
PROC 114
	task_do_option 4172 1
ENDPROC
PROC 115
	task_do_option 4173 1
ENDPROC
PROC 116
	task_do_option 4174 1
ENDPROC
PROC 117
	task_do_option 4175 1
ENDPROC
PROC 118
	task_do_option 4176 1
ENDPROC
PROC 119
	task_do_option 4177 1
ENDPROC
PROC 120
	task_do_option 4178 1
ENDPROC
PROC 121
	task_do_option 4179 1
ENDPROC



proc 250
	dialog 3
	add_talk $_npc$:
	add_talk #b#b请在下面的格子中放入#c186506相思粽#n或#c186506情义粽#n
	add_talk #b#b如果你没有粽子的话，就去粽老那儿制作吧！
	
	NPC_COMMIT_OPT 251
	
	talk
endproc 
proc 251
	strvar m_ID m_Pos1 m_Pos2 m_Col Ret RetTask
	GET_COMMIT_ITEM_INFO 1 m_ID
	if m_ID = -1
		add_sys_msg_start
		add_sys_msg 你还没有提交物品！
		set_sys_msg_mode 1 0 0
		msg_info		
		return 
	endif
	
	GET_COMMIT_ITEM_INFO 2 m_Pos1
	GET_COMMIT_ITEM_INFO 3 m_Pos2
	GET_COMMIT_ITEM_INFO 4 m_Col
	task_stat 4492 1 100
	if 100 = 2
		if m_ID = 5012123 or m_ID = 5012124
			switch m_ID
				case 5012123
					DEC_ITEM_NUM_BYIDX 5012123 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4492 1
					call 502
				endcase
				case 5012124
					DEC_ITEM_NUM_BYIDX 5012124 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4492 1
					call 502
				endcase
			endswitch
		else
			add_sys_msg_start
			add_sys_msg 物品不符任务要求，未能成功上交
			set_sys_msg_mode 1 0 0
			msg_info
		endif
	
	endif
endproc 
proc 502
	add_sys_msg_start
	add_sys_msg 成功上交物品，任务完成
	set_sys_msg_mode 3 0 0
	msg_info
	PLAYER_CLOSE_DIALOG 0 0
endproc 



proc 300
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b太感谢了，我正愁工具又磨损，没法磨制宝石呢。
		option 301 #交出制作工具
	talk
endproc 
proc 301 
	GET_ITEM_NUM_INPAK 5300874 0 11
	if 11 < 1
		add_sys_msg_start
		add_sys_msg 你身上没有这件物品！
		set_sys_msg_mode 1 0 0
		msg_info
	ENDIF
	if 11 >= 1
			
		dec_item_num_inpak 5300874 1 30
		if 30 != -1
			task_fini 3677 1
		endif
	endif
endproc 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
proc 400
	choose 1 402 403 是否消耗1个宝石碎片,净化10个污染的高能凝晶?
endproc 

proc 402
	exchg_item 1 1
endproc
proc 500
	DIALOG 1
	add_talk 可净化次数：
	
	exchg_item_get 1 40
	if 40 != -1
		add_talk #b#b污染的高能凝晶：剩余#c186506$@40$#n次		
	else
		add_talk #b#b污染的高能凝晶：剩余#c1865060#n次
	endif	
	add_talk #b
	add_talk #b#b怎么获得污染的高能凝晶:失落之域大量掉落
	add_talk #b#b怎么净化污染的高能凝晶:在宝石商人处,每次净化需要消耗10个污染的高能凝晶和1个宝石碎片,可以得到10个高能凝晶
	add_talk #b#b高能凝晶:可以给三级或以上品质的宝石充能,提升宝石的属性
		
	option 1 返回
	talk
endproc
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; proc 888
;;;备份
;;;备份
;;;备份

	; get_role_var 40 8 1 15 
	
	; if 15 >= 50
		
		; add_sys_msg_start
		; add_sys_msg 每人每天最多只能净化50份,您今天净化数量已经达到50份!
		; set_sys_msg_mode 3 0 0
		; msg_info
		; return
	; endif

	; strvar num
	; GET_PACK_SPACE 0 num
	; if num = 0
		; call 555
		; return
	; endif
	
	; GET_ITEM_NUM_INPAK 5060014 1 30	
	; GET_ITEM_NUM_INPAK 5213410 1 40	
	; if 30 >= 10 
		; if 40 >= 10
			
			; dec_item_num_inpak 5060014 10 50
			; dec_item_num_inpak 5213410 10 55
			; if 50 = 0 and 55 = 0
				; ADD_BIND_ITEM_NUM_INPAK 5213411 10 1 0
				
				; strvar time
				; GET_TIME_SEC time
				; set_role_var 41 32 @time 1
				
				; add 15 10
				; set_role_var 40 8 @15 1
			; endif
		; else
			; add_sys_msg_start
			; add_sys_msg 污染的高能凝晶不足！
			; set_sys_msg_mode 3 0 0
			; msg_info
		; endif
	; else
		; add_sys_msg_start
		; add_sys_msg 宝石碎片不足！
		; set_sys_msg_mode 3 0 0
		; msg_info
	; endif
; endproc
proc 403
endproc 


; proc 410
	
	; choose 1 412 403 净化20份污染的高能凝晶需要消耗20份宝石碎片,是否确认?
; endproc
; proc 412
	; get_role_var 40 8 1 15 
	; if 15 >= 50
		
		; add_sys_msg_start
		; add_sys_msg 每人每天最多只能净化50份,您今天净化数量已经达到50份!
		; set_sys_msg_mode 3 0 0
		; msg_info
		; return
	; endif

	; strvar num
	; GET_PACK_SPACE 0 num
	; if num = 0
		; call 555
		; return
	; endif
	
	; GET_ITEM_NUM_INPAK 5060014 1 30	
	; GET_ITEM_NUM_INPAK 5213410 1 40	
	; if 30 >= 20 
		; if 40 >= 20
			
			; dec_item_num_inpak 5060014 20 50
			; dec_item_num_inpak 5213410 20 55
			; if 50 = 0 and 55 = 0
				; ADD_BIND_ITEM_NUM_INPAK 5213411 20 1 0
				
				; strvar time
				; GET_TIME_SEC time
				; set_role_var 41 32 @time 1
				
				; add 15 20
				; set_role_var 40 8 @15 1
			; endif
		; else
			; add_sys_msg_start
			; add_sys_msg 污染的高能凝晶不足！
			; set_sys_msg_mode 3 0 0
			; msg_info
		; endif
	; else
		; add_sys_msg_start
		; add_sys_msg 宝石碎片不足！
		; set_sys_msg_mode 3 0 0
		; msg_info
	; endif


; endproc 
; proc 420

	; choose 1 422 403 净化50份污染的高能凝晶需要消耗50份宝石碎片,是否确认?

; endproc 
; proc 422
	; get_role_var 40 8 1 15 
	; if 15 >= 50
		
		; add_sys_msg_start
		; add_sys_msg 每人每天最多只能净化50份,您今天净化数量已经达到50份!
		; set_sys_msg_mode 3 0 0
		; msg_info
		; return
	; endif

	; strvar num
	; GET_PACK_SPACE 0 num
	; if num = 0
		; call 555
		; return
	; endif
	
	; GET_ITEM_NUM_INPAK 5060014 1 30	
	; GET_ITEM_NUM_INPAK 5213410 1 40	
	; if 30 >= 50 
		; if 40 >= 50
			
			; dec_item_num_inpak 5060014 50 50
			; dec_item_num_inpak 5213410 50 55
			; if 50 = 0 and 55 = 0
				; ADD_BIND_ITEM_NUM_INPAK 5213411 50 1 0
				
				; strvar time
				; GET_TIME_SEC time
				; set_role_var 41 32 @time 1
				
				; add 15 50
				; set_role_var 40 8 @15 1
			; endif
		; else
			; add_sys_msg_start
			; add_sys_msg 污染的高能凝晶不足！
			; set_sys_msg_mode 3 0 0
			; msg_info
		; endif
	; else
		; add_sys_msg_start
		; add_sys_msg 宝石碎片不足！
		; set_sys_msg_mode 3 0 0
		; msg_info
	; endif

; endproc 

; proc 555
	; add_sys_msg_start
	; add_sys_msg 背包已满，无法获得物品
	; set_sys_msg_mode 3 0 0
	; msg_info
; endproc 



















