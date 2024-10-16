
proc 2

	strvar hp
	GET_MON_INFO 1 1 10 hp 0 0 0 0
	if hp = 0
		add_sys_msg_start
		add_sys_msg   勇敢的玩家$_player$与他的伙伴,慧眼如炬,在流金沙漠找到了魔物真身,并将之击杀,获得了大量的宝物!
		set_sys_msg_mode 1 0 0
		svr_msg_info
		
	endif
endproc 
proc 12
	strvar hp
	GET_MON_INFO 1 1 10 hp 0 0 20 30
	if hp != 0
		
		effect 0 0 playEffct($@20$,$@30$,"effect\\entity\\204011_18.ent")
		MON_SPEECH @10 1 1 1 哼!居然到了18:00,兄弟喊我回副本了,你们给我等着,下次我还会回来的……
	endif

endproc 