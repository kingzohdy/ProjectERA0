; ���İ�

proc 1
	strvar num gezi job ret
	GET_PACK_SPACE 0 gezi
	
	if @gezi < 1
		ADD_SYS_MSG_START
		ADD_SYS_MSG ������Ҫ1�����Ӳ�װ����Ŷ�������������ٿ���
		SET_SYS_MSG_MODE 3 0 0
		MSG_INFO
	else
		GET_PLAYER_BASE_ATT 0 0 0 job
		
			DEC_ITEM_NUM_INPAK 5053044 1 ret
			if ret = 0
				if job = 17 ;�ػ�
					rand num 6
						if num = 0
							ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5251600 1 1 0
						endif
						if num = 1
							ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5251601 1 1 0
						endif
						if num = 2
							ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5251605 1 1 0
						endif
						if num = 3
							ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5251606 1 1 0
						endif
						if num = 4
							ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5251610 1 1 0
						endif
						if num = 5
							ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5251611 1 1 0
						endif
				endif
				
				if job = 18 ;����
					rand num 6
						if num = 0
							ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5251630 1 1 0
						endif
						if num = 1
							ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5251631 1 1 0
						endif
						if num = 2
							ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5251635 1 1 0
						endif
						if num = 3
							ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5251636 1 1 0
						endif
						if num = 4
							ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5251640 1 1 0
						endif
						if num = 5
							ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5251641 1 1 0
						endif
				endif
				
				if job = 19 ;��ɱ
					rand num 6
						if num = 0
							ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5251660 1 1 0
						endif
						if num = 1
							ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5251661 1 1 0
						endif
						if num = 2
							ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5251665 1 1 0
						endif
						if num = 3
							ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5251666 1 1 0
						endif
						if num = 4
							ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5251670 1 1 0
						endif
						if num = 5
							ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5251671 1 1 0
						endif
				endif
				
				if job = 20 ;ʥ��
					rand num 6
						if num = 0
							ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5251690 1 1 0
						endif
						if num = 1
							ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5251691 1 1 0
						endif
						if num = 2
							ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5251695 1 1 0
						endif
						if num = 3
							ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5251696 1 1 0
						endif
						if num = 4
							ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5251700 1 1 0
						endif
						if num = 5
							ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5251701 1 1 0
						endif
				endif
				
				if job = 33 ;����
					rand num 6
						if num = 0
							ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5251615 1 1 0
						endif
						if num = 1
							ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5251616 1 1 0
						endif
						if num = 2
							ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5251620 1 1 0
						endif
						if num = 3
							ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5251621 1 1 0
						endif
						if num = 4
							ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5251625 1 1 0
						endif
						if num = 5
							ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5251626 1 1 0
						endif
				endif
				
				if job = 34 ;����
					rand num 6
						if num = 0
							ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5251645 1 1 0
						endif
						if num = 1
							ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5251647 1 1 0
						endif
						if num = 2
							ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5251650 1 1 0
						endif
						if num = 3
							ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5251652 1 1 0
						endif
						if num = 4
							ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5251655 1 1 0
						endif
						if num = 5
							ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5251657 1 1 0
						endif
				endif
				
				if job = 35 ;����
					rand num 6
						if num = 0
							ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5251675 1 1 0
						endif
						if num = 1
							ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5251676 1 1 0
						endif
						if num = 2
							ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5251680 1 1 0
						endif
						if num = 3
							ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5251681 1 1 0
						endif
						if num = 4
							ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5251685 1 1 0
						endif
						if num = 5
							ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5251686 1 1 0
						endif
				endif
				
				if job = 36 ;Ԧ��
					rand num 6
						if num = 0
							ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5251705 1 1 0
						endif
						if num = 1
							ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5251706 1 1 0
						endif
						if num = 2
							ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5251710 1 1 0
						endif
						if num = 3
							ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5251711 1 1 0
						endif
						if num = 4
							ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5251715 1 1 0
						endif
						if num = 5
							ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5251716 1 1 0
						endif
				endif
			endif
		
	endif
endproc

		