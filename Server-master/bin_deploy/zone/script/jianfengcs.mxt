

proc 1
	strvar npcid
	GET_NPC_DEFID 1 npcid
	if npcid = 333110
		player_transmit_rand 20000 6600 14600 1000 1
	endif                          
	if npcid = 333111              
		player_transmit_rand 20000 6600 11500 1000 1
	endif                          
	if npcid = 333112              
		player_transmit_rand 20000 3800 11400 1000 1
	endif                        
	
	
	if npcid = 333113
		player_transmit_rand 20000 11900 14800 1000 1
	endif                          
	if npcid = 333114              
		player_transmit_rand 20000 12500 11300 1000 1
	endif                          
	if npcid = 333115              
		player_transmit_rand 20000 15500 11400 1000 1
	endif                        
	
	
	if npcid = 333116
		player_transmit_rand 20000 3900 6800 1000 1
	endif                          
	if npcid = 333117              
		player_transmit_rand 20000 7600 6600 1000 1
	endif                          
	if npcid = 333118              
		player_transmit_rand 20000 7100 4000 1000 1
	endif                        
	
	
	
	if npcid = 333119
		player_transmit_rand 20000 12300 6700 1000 1
	endif                          
	if npcid = 333120              
		player_transmit_rand 20000 15000 7000 1000 1
	endif                          
	if npcid = 333121              
		player_transmit_rand 20000 12300 3700 1000 1
	endif                        


endproc 

