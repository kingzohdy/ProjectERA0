;��ţԤ��,�����¼�
proc 1
	call 100
;	MAP_NEW_NPC #���ر�־ #NpcID #x #y #dir #map $�ű� 
	MAP_NEW_NPC 1 306036 20600 31400 80 4400 npccowtryoutstart.mac 
;	NEW_MAP_LIFE_NPC 1 306036 20600 31400 80 4400 48600 npccowtryoutstart.mac 
endproc
proc 2
	call 100
endproc
proc 100
	get_map_inst 4400 1
	if 1 = 0
		del_npc 306036 1
	endif
endproc
