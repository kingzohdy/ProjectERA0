
;proc 1
;	var 10 0
;	set_task_var 1323 0 8 @10 1
;	var 11 0
;	set_task_var 1323 1 32 @11 1

;endproc
;proc 2
;		del_npc 313000 0
;		new_mon 293090 1 8834 17779 0 0 0 0 0

;endproc
proc 4
		dec_item_num_inpak 5300394 1 0
				
endproc 
proc 5

		dec_item_num_inpak 5300394 1 0
endproc
;proc 6
;		get_task_var 1323 0 8 1 10
;		if 10 = 1
;			get_task_var 1323 1 32 1 11
;			add 11 1
;			set_task_var 1323 1 32 @11 1
;			if 11 = 1 
;				new_npc 0 313000 8834 17779 101 tgjisi.mac
;			endif
;			
;			if 11 = 60 
;			;
;				del_npc 313000 0
;				new_mon 293090 1 8834 17779 0 0 0 0 0
;				var 10 0
;				set_task_var 1323 0 8 @10 1
;			endif
;			if 11 = 120
;				dec_item_num_inpak 5300394 1 0
;			endif
;
;		endif
;endproc






