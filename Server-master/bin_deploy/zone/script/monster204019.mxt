;¿ó¾®ÏÝÚå±¦Ïä

proc 2
	get_mon_info 1 1 1 1 168 1 1 1
	switch 168
		case 10300
			call_mon 1 204026 100 0 23230 10572 2
		endcase
		case 10700
			call_mon 1 206114 100 0 23230 10572 2
			call_mon 1 206112 100 0 23230 10572 2
		endcase
		case 11000
			call_mon 1 206231 100 0 23230 10572 2
			call_mon 2 206229 100 0 23230 10572 2
		endcase
		case 11300
			call_mon 1 206349 100 0 23230 10572 2
			call_mon 3 206351 100 0 23230 10572 2
		endcase
		case 12800
			call_mon 1 206459 100 0 23230 10572 2
			call_mon 3 206461 100 0 23230 10572 2
		endcase
	endswitch
endproc

