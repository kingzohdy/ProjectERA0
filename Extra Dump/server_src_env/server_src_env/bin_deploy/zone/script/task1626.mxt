
proc 1
	var 10 0
	set_task_var 1626 0 8 @10 1



endproc
proc 3
;提交删除临时机械
	DEL_ROLE_TMP_ITEM 0 1
	DEL_ROLE_UNUSED_TMP_ITEM 0 1460005 1
	
	dec_item_num_inpak 5300436 1 0
endproc 

proc 5
;放弃任务删除临时机械
	DEL_ROLE_TMP_ITEM 0 1
	DEL_ROLE_UNUSED_TMP_ITEM 0 1460005 1
	dec_item_num_inpak 5300436 1 0

endproc



