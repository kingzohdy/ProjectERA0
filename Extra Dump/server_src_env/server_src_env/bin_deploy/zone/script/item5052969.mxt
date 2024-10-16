;天天畅玩包

proc 1
	strvar ret
	GET_PACK_SPACE 0 ret
	if ret > 0

		DEC_ITEM_NUM_INPAK 5052969 1 10
		if 10 = 0
			ADD_BIND_ITEM_NUM_INPAK 5200020 1 1 0
		endif
	else
		MAIL_TEXT #r 由于您的背包格数不足，您天天畅玩包中的物品通过邮寄的方式发放给您，请注意接收物品。
		sys_mail_bind 1 5200020 1 1 0 0 0 天天畅玩包$零纪元客服中心
		DEC_ITEM_NUM_INPAK 5052969 1 11
	endif
endproc 

