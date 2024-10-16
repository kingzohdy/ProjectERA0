;肉香扑鼻任务物品（5300479），不知道叫什么，使用后删除，获得5300475，也不知道叫什么，不过没关系啦。


Proc 1
		DEC_ITEM_NUM_INPAK 5300479 1 1
		ADD_ITEM_NUM_INPAK 5300475 1 1 1
					if 1 = -1
						return
					else
					TASK_FINI 1730 1
					ENDIF
endproc			
			
				
				