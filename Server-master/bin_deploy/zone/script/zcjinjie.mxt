

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b你是否知道#c6d00d2卓越装备#n可以进阶成更厉害的#cff0080史诗套装#n！
	add_talk #b#b而我将为你提供微粒提炼服务，经过提炼后的微粒可制作成合金，作为装备进阶的材料。
	; add_talk #b#b#c676464（50级、60级每天可以提炼2次）
	add_talk #b#b
	add_talk #b#b每种微粒每天都能提炼#c1865063次#n，每次消耗#c18650620份#n
	add_talk #b#b#b#c186506当天未提炼次数，可累计到今后使用#n
			
		task_add_option 2251 1 101
		task_add_option 2252 1 102
		task_add_option 2253 1 103
		task_add_option 2254 1 104

;		strvar time
;		GET_TIME_SEC time
;		set_role_var 18 32 @time 1
		
		strvar ret time
		get_role_var 18 32 1 time 
		is_same_day @time ret
		if ret = -1
			var 10 0
			set_role_var 23 8 @10 1
			var 11 0
			set_role_var 24 8 @11 1
			var 12 0
			set_role_var 25 8 @12 1
			var 13 0
			set_role_var 26 8 @13 1
		endif
	
		; task_stat 2251 10 30
		; if 10 = 0 and 30 = 3
			; option 151 #请帮我提炼一颗黑耀碎块
		; endif
		; task_stat 2252 10 31
		; if 10 = 0 and 31 = 3
			; option 160 #请帮我提炼一颗炽铜碎块
		; endif
		; task_stat 2253 10 32
		; if 10 = 0 and 32 = 3
			; option 170 #请帮我提炼一颗坚钢碎块
		; endif
		; task_stat 2254 10 33
		; if 10 = 0 and 33 = 3
			; option 180 #请帮我提炼一颗恒钻碎块
		; endif
		
		option 190 我要提炼微粒
		
		option 150 【装备进阶】与【微粒提炼】说明
		
	talk
	
	
endproc 

PROC 101
	task_do_option 2251 1
ENDPROC
PROC 102
	task_do_option 2252 1
ENDPROC
PROC 103
	task_do_option 2253 1
ENDPROC
PROC 104
	task_do_option 2254 1
ENDPROC

proc 151
	exchg_item 2 1
	call 190
endproc
proc 160
	exchg_item 3 1
	call 190
endproc
proc 170
	exchg_item 4 1
	call 190
endproc
proc 180
	exchg_item 5 1
	call 190
endproc

proc 190
	DIALOG 1
	add_talk 可提炼次数：
	
	strvar LV
	exchg_item_get 2 40
	exchg_item_get 3 50
	exchg_item_get 4 60
	exchg_item_get 5 70
	task_stat 2251 10 30
	task_stat 2252 10 31
	task_stat 2253 10 32
	task_stat 2254 10 33
	GET_PLAYER_BASE_ATT 1 1 LV 1
	
	if 30 = 3
		if 40 != -1
			add_talk 黑耀微粒：剩余#c186506$@40$#n/30次
		else
			add_talk 黑耀微粒：剩余#c1865060#n/30次
		endif
	else
		if @LV > 49
		add_talk 完成#c341700装备进阶-黑耀碎块#n任务后，即可提炼#c186506黑耀微粒#n
		endif
	endif
	if 31 = 3
		if 50 != -1
			add_talk 炽铜微粒：剩余#c186506$@50$#n/30次
		else
			add_talk 炽铜微粒：剩余#c1865060#n/30次
		endif
	else
		if @LV > 59
		add_talk 完成#c341700装备进阶-炽铜碎块#n任务后，即可提炼#c186506炽铜微粒#n
		endif
	endif
	if 32 = 3
		if 60 != -1
			add_talk 坚钢微粒：剩余#c186506$@60$#n/30次
		else
			add_talk 坚钢微粒：剩余#c1865060#n/30次
		endif
	else
		if @LV > 69
		add_talk 完成#c341700装备进阶-坚钢碎块#n任务后，即可提炼#c186506坚钢微粒#n
		endif
	endif
	if 33 = 3
		if 70 != -1
			add_talk 恒钻微粒：剩余#c186506$@70$#n/30次
		else
			add_talk 恒钻微粒：剩余#c1865060#n/30次
		endif
	else
		if @LV > 79
			add_talk 完成#c341700装备进阶-恒钻碎块#n任务后，即可提炼#c186506恒钻微粒#n
		endif
	endif
	
	if 10 = 0 and 30 = 3
		option 151 #请帮我提炼1颗 - 黑耀碎块
	endif
	if 10 = 0 and 31 = 3
		option 160 #请帮我提炼1颗 - 炽铜碎块
	endif
	if 10 = 0 and 32 = 3
		option 170 #请帮我提炼1颗 - 坚钢碎块
	endif
	if 10 = 0 and 33 = 3
		option 180 #请帮我提炼1颗 - 恒钻碎块
	endif
	
	
		option 1 返回
	talk
endproc

proc 150
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b如果你对【装备进阶】或【微粒提炼】仍有不明白的地方，我想你应该找对人了。

	option 251 【装备进阶】常见问题解答
	option 252 【微粒提炼】累积次数说明
	option 1 我已经大致了解
	talk
endproc



proc 251
	DIALOG 1
;	add_talk $_npc$:
	add_talk #b#b#c186506问：装备进阶是必然成功的吗？#n
	add_talk #b#b答：Of Course！我以惠家几百年的声誉保证！
	add_talk #b#b
	add_talk #b#b#c186506问：装备进阶后，我之前洗炼的蓝色属性、强化的星级、镶嵌的宝石还在吗？#n
	add_talk #b#b答：当然在！除此之外还有附魔属性、刻印属性都会保留！
	add_talk #b#b
	add_talk #b#b#c186506问：进阶需要的黑耀合金#c676464（50级装备进阶材料）#n、#c186506炽铜合金#n#c676464（60级装备进阶材料）#n、#c186506坚钢合金#n#c676464（70级装备进阶材料）#n、#c186506恒钻合金#n#c676464（80级装备进阶材料）#n#c186506哪里得？#n
	add_talk #b#b答：通过与之名称相对应的#c0181f9“碎块”合成#n！
	add_talk #b#b
	add_talk #b#b#c186506问：合成“合金”需要的“碎块”哪里得？#n
	add_talk #b#b答：通过微粒合成！赏金令5星奖励！世界BOSS掉落！以及在我这接受兑换碎块的任务！
	add_talk #b#b
	add_talk #b#b#c186506问：合成“碎块”的微粒哪里得？#n
	add_talk #b#b答：莫名遗迹-黑耀微粒、祈妙神坛-炽铜微粒、牢城审讯室-坚钢微粒！
	add_talk #b#b
	; add_talk #b#b#c186506问：所有的紫色装备都可以进阶成粉装吗？#n
	; add_talk #b#b答：说来惭愧，我姐妹二人不小心将祖先留下的《装备进阶-首饰篇》遗失了，虽然我们努力补救，潜心钻研，但至今仍未找到首饰进阶必然成功的方法，为了避免您好不容易才到手的紫色装备损坏、报废，哪怕只有万分之一的几率也是我们不愿看到的！
	; add_talk #b#b所以我姐妹二人决定暂时不提供首饰（面饰、项链、戒指、护符）的进阶服务。
	add_talk #b#b#c186506问：面饰、首饰也可以进阶吗？#n
	add_talk #b#b答：面饰、项链、戒指、护符采用另外一种进阶工艺，它们通过#c186506松珀合金#n#c676464（60-69级进阶材料）#n、#c186506苍岩合金#n#c676464（70-79级进阶材料）#n来实现进阶。你可以在失落之域找到这些合金的碎块。
	
	option 150 谢谢你的耐心解答
	talk
endproc 

proc 252
	DIALOG 1
	add_talk $_npc$:
	add_talk ●#b每种微粒只要成功提炼过，即可分别享受#c186506累积提炼次数#n的服务
	add_talk ●#b累积提炼次数服务，可把当天#c186506未提炼次数#n累积到今后使用
	add_talk ●#b每种微粒最多能够累积#c18650630次#n提炼次数
	
	option 150 谢谢你的详细说明
	talk
endproc

;;;-------------------------------------------------------------------------------
; proc 888
; 备份备份备份备份备份备份备份备份备份备份备份备份备份备份备份备份备份备份备份备份备份备份
	; strvar time
	; get_role_var 23 8 1 10
	
	; add_sys_msg_start
	; add_sys_msg $@10$
	; set_sys_msg_mode 1 0 0
	; msg_info
	; strvar num
	; GET_PACK_SPACE 0 num
	; if num = 0
		; call 555
		; return
	; endif
	; if 10 < 3
		; GET_ITEM_NUM_INPAK 5060200 1 30	
		; if 30 >= 20
			; dec_item_num_inpak 5060200 20 15
			; if 15 = 0
				; add_item_num_inpak 5060100 1 1 0
				
				; GET_TIME_SEC time
				; set_role_var 18 32 @time 1			
				; add 10 1
				; set_role_var 23 8 @10 1		
			; endif
		; else
			; add_sys_msg_start
			; add_sys_msg 黑耀微粒不足20颗，无法提炼
			; set_sys_msg_mode 3 0 0
			; msg_info
		; endif
	; else
		; add_sys_msg_start
		; add_sys_msg 每天只可提炼3次黑耀碎块
		; set_sys_msg_mode 3 0 0
		; msg_info
	; endif
; endproc 
; proc 889
;备份备份备份备份备份备份备份备份备份备份备份备份备份备份备份备份备份备份备份备份备份备份
	; strvar num
	; GET_PACK_SPACE 0 num
	; if num = 0
		; call 555
		; return
	; endif
	; strvar time
	; get_role_var 24 8 1 11
	; if 11 < 3
		; GET_ITEM_NUM_INPAK 5060201 1 30	
		; if 30 >= 20
			; dec_item_num_inpak 5060201 20 15
			; if 15 = 0
				; add_item_num_inpak 5060101 1 1 0
				
				; GET_TIME_SEC time
				; set_role_var 18 32 @time 1			
				; add 11 1
				; set_role_var 24 8 @11 1		
			; endif
		; else
			; add_sys_msg_start
			; add_sys_msg 炽铜微粒不足20颗，无法提炼
			; set_sys_msg_mode 3 0 0
			; msg_info
		; endif
	; else
		; add_sys_msg_start
		; add_sys_msg 每天只可提炼3次炽铜碎块
		; set_sys_msg_mode 3 0 0
		; msg_info
	; endif

; endproc 
; proc 890
;备份备份备份备份备份备份备份备份备份备份备份备份备份备份备份备份备份备份备份备份备份备份
	; strvar num
	; GET_PACK_SPACE 0 num
	; if num = 0
		; call 555
		; return
	; endif
	; strvar time
	; get_role_var 25 8 1 12
	; if 12 < 3
		; GET_ITEM_NUM_INPAK 5060202 1 30	
		; if 30 >= 20
			; dec_item_num_inpak 5060202 20 15
			; if 15 = 0
				; add_item_num_inpak 5060102 1 1 0
				
				; GET_TIME_SEC time
				; set_role_var 18 32 @time 1			
				; add 12 1
				; set_role_var 25 8 @12 1		
			; endif
		; else
			; add_sys_msg_start
			; add_sys_msg 坚钢微粒不足20颗，无法提炼
			; set_sys_msg_mode 3 0 0
			; msg_info
		; endif
	; else
		; add_sys_msg_start
		; add_sys_msg 每天只可提炼3次坚钢碎块
		; set_sys_msg_mode 3 0 0
		; msg_info
	; endif
; endproc 
; proc 891
;备份备份备份备份备份备份备份备份备份备份备份备份备份备份备份备份备份备份备份备份备份备份
	; strvar num
	; GET_PACK_SPACE 0 num
	; if num = 0
		; call 555
		; return
	; endif
	; strvar time
	; get_role_var 26 8 1 13
	; if 13 < 3
		; GET_ITEM_NUM_INPAK 5060203 1 30	
		; if 30 >= 20
			; dec_item_num_inpak 5060203 20 15
			; if 15 = 0
				; add_item_num_inpak 5060103 1 1 0
				
				; GET_TIME_SEC time
				; set_role_var 18 32 @time 1			
				; add 13 1
				; set_role_var 26 8 @13 1		
			; endif
		; else
			; add_sys_msg_start
			; add_sys_msg 恒钻微粒不足20颗，无法提炼
			; set_sys_msg_mode 3 0 0
			; msg_info
		; endif
	; else
		; add_sys_msg_start
		; add_sys_msg 每天只可提炼3次恒钻碎块
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






