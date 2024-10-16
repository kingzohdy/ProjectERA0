

proc 3
;提交任务
		GET_ITEM_NUM_INPAK 5300351 0 50
		GET_ITEM_NUM_INPAK 5300352 0 51
		GET_ITEM_NUM_INPAK 5300371 0 52

		dec_item_num_inpak 5300351 @50 0
		dec_item_num_inpak 5300352 @51 0
		dec_item_num_inpak 5300371 @52 0
endproc 


proc 5
;放弃任务
		GET_ITEM_NUM_INPAK 5300351 0 50
		GET_ITEM_NUM_INPAK 5300352 0 51
		GET_ITEM_NUM_INPAK 5300371 0 52

		dec_item_num_inpak 5300351 @50 0
		dec_item_num_inpak 5300352 @51 0
		dec_item_num_inpak 5300371 @52 0

endproc







