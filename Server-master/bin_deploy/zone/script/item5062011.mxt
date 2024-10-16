;低级藏宝图1

proc 1
	strvar d v
	use_cangbaotu_pic 5062011 1000  d v
	switch d
	case 0
			dec_item_num_inpak 5062011 1 0
			add_sys_msg_start
			add_sys_msg 在树根的缝隙中找到一张破旧的羊皮纸
			set_sys_msg_mode 4 0 0
			msg_info
			add_item_num_inpak 5300358 1 1 0	
		
	endcase
	case 1
		add_sys_msg_start
		add_sys_msg 你确定没有看错？宝藏不在这张地图
		set_sys_msg_mode 4 0 0
		msg_info
	endcase
	case 2
		add_sys_msg_start
		switch v
		case 1
		add_sys_msg 距离宝藏只有一步之遥,就在你的北方附近
		endcase
		case 2
		add_sys_msg 距离宝藏只有一步之遥,就在你的东北方附近
		endcase
		case 3
		add_sys_msg 距离宝藏只有一步之遥,就在你的东方附近
		endcase
		case 4
		add_sys_msg 距离宝藏只有一步之遥,就在你的东南方附近
		endcase
		case 5
		add_sys_msg 距离宝藏只有一步之遥,就在你的南方附近
		endcase
		case 6
		add_sys_msg 距离宝藏只有一步之遥,就在你的西南方附近
		endcase
		case 7
		add_sys_msg 距离宝藏只有一步之遥,就在你的西方附近
		endcase
		case 8
		add_sys_msg 距离宝藏只有一步之遥,就在你的西北方附近
		endcase
		endswitch
		set_sys_msg_mode 4 0 0
		msg_info
	endcase
	case 3
		add_sys_msg_start
		switch v
		case 1
		add_sys_msg 宝藏不远了，就在你的北方
		endcase
		case 2
		add_sys_msg 宝藏不远了,就在你的东北方
		endcase
		case 3
		add_sys_msg 宝藏不远了,就在你的东方
		endcase
		case 4
		add_sys_msg 宝藏不远了,就在你的东南方
		endcase
		case 5
		add_sys_msg 宝藏不远了,就在你的南方
		endcase
		case 6
		add_sys_msg 宝藏不远了，就在你的西南方
		endcase
		case 7
		add_sys_msg 宝藏不远了,就在你的西方
		endcase
		case 8
		add_sys_msg 宝藏不远了,就在你的西北方
		endcase
		endswitch
		set_sys_msg_mode 4 0 0
		msg_info
	endcase
	case 4
		add_sys_msg_start
		switch v
		case 1
		add_sys_msg 宝藏距此处还有些距离,在你的北方
		endcase
		case 2
		add_sys_msg 宝藏距此处还有些距离,在你的东北方
		endcase
		case 3
		add_sys_msg 宝藏距此处还有些距离,在你的东方
		endcase
		case 4
		add_sys_msg 宝藏距此处还有些距离,在你的东南方
		endcase
		case 5
		add_sys_msg 宝藏距此处还有些距离,在你的南方
		endcase
		case 6
		add_sys_msg 宝藏距此处还有些距离,在你的西南方
		endcase
		case 7
		add_sys_msg 宝藏距此处还有些距离,在你的西方
		endcase
		case 8
		add_sys_msg 宝藏距此处还有些距离,在你的西北方
		endcase
		endswitch
		set_sys_msg_mode 4 0 0
		msg_info
	endcase
	endswitch

endproc	