;公会战     ,深渊领主修斯佩恩
;邪神活动,邪神,深渊领主修斯佩恩

proc 1
	strvar memid
	GET_MON_INFO 1 0 memid 0 0 0 0 0
	TARGET_ADD_STATUS 3 @memid 1095 1 25920000 0 
endproc
;proc 2
	
	strvar x y id
	GET_MON_INFO 1 0 0 0 0 0 x y
;	NEW_MON 200031 1 @x @y 0 0 0 1 0
	;加称号
	MON_GET_KILLER_ID 1 id
	if 1 = 0
		set_player_curr @id 1		
	endif
	set_desig 1 125 
;
;	;公会战相关
;	strvar time sec
;	get_world_boss_first_kill_time 1 time
;	if time = 0
;		set_world_boss_first_kill_time 1
;		;掉物品
;		ADD_ITEM_ON_MAP 1 0 5043126 1 0 0 0 0
;		;激活时间,10天后的20点
;		get_time_sec sec
;		dev sec 86400
;		add sec 10
;		mul sec 86400
;;		add sec 72000
;
;		set_world_city_enable_time 1 @sec
;	endif
;endproc

proc 2
	
	strvar x y num a b
	GET_MON_INFO 0 200030 0 0 0 0 X Y
	
	rand 10 100
	if 10 < 50
		NEW_MAP_MON 200202 1 3217 @X @Y 0 0 0 0 2 0 0
		NEW_MAP_MON 206427 1 3217 @X @Y 0 0 0 0 600 0 0
	elseif 10 >= 50
		NEW_MAP_MON 200202 1 3217 @X @Y 0 0 0 0 2 0 0
		NEW_MAP_LIFE_NPC 0 3217 304146 @X @Y 0 600 npc304146.mac
	else
	endif
	
	var num 0
	while @num < 5
	rand 20 15
		switch @20
			case 0
				var a 3757
				var b 8804
			endcase
			case 1
				var a 4257
				var b 9009
			endcase
			case 2
				var a 3846
				var b 8346
			endcase
			case 3
				var a 3673
				var b 7854
			endcase
			case 4
				var a 3938
				var b 7379
			endcase
			case 5
				var a 3816
				var b 6779
			endcase
			case 6
				var a 4963
				var b 8821
			endcase
			case 7
				var a 5589
				var b 9102
			endcase
			case 8
				var a 5766
				var b 8598
			endcase
			case 9
				var a 6110
				var b 8997
			endcase
			case 10
				var a 4619
				var b 8938
			endcase
			case 11
				var a 3804
				var b 9186
			endcase
			case 12
				var a 4077
				var b 8745
			endcase
			case 13
				var a 3589
				var b 8491
			endcase
			case 14
				var a 3934
				var b 7917
			endcase
		endswitch
	NEW_MAP_MON 200202 1 3217 @a @b 0 0 0 0 2 0 0
	NEW_MAP_MON 293600 1 3217 @a @b 0 0 0 0 24 0 0
	add num 1
	endwhile
	
	var num 0
	while @num < 5
	rand 21 15
		switch @21
			case 0
				var a 8255
				var b 7889
			endcase
			case 1
				var a 8809
				var b 8011
			endcase
			case 2
				var a 8360
				var b 8517
			endcase
			case 3
				var a 9162
				var b 8556
			endcase
			case 4
				var a 8016
				var b 9097
			endcase
			case 5
				var a 7379
				var b 8674
			endcase
			case 6
				var a 6817
				var b 9101
			endcase
			case 7
				var a 6477
				var b 8778
			endcase
			case 8
				var a 8986
				var b 6658
			endcase
			case 9
				var a 8864
				var b 7220
			endcase
			case 10
				var a 9296
				var b 7643
			endcase
			case 11
				var a 7435
				var b 9326
			endcase
			case 12
				var a 8586
				var b 9064
			endcase
			case 13
				var a 8808
				var b 8425
			endcase
			case 14
				var a 7435
				var b 9005
			endcase
		endswitch
	NEW_MAP_MON 200202 1 3217 @a @b 0 0 0 0 2 0 0
	NEW_MAP_MON 293600 1 3217 @a @b 0 0 0 0 24 0 0
	add num 1
	endwhile
	
	var num 0
	while @num < 5
	rand 22 15
		switch @22
			case 0
				var a 8263
				var b 4815
			endcase
			case 1
				var a 8332
				var b 4244
			endcase
			case 2
				var a 8942
				var b 4719
			endcase
			case 3
				var a 9047
				var b 3917
			endcase
			case 4
				var a 8359
				var b 3639
			endcase
			case 5
				var a 7723
				var b 3997
			endcase
			case 6
				var a 7396
				var b 3452
			endcase
			case 7
				var a 7047
				var b 4284
			endcase
			case 8
				var a 6407
				var b 4062
			endcase
			case 9
				var a 6720
				var b 3478
			endcase
			case 10
				var a 9149
				var b 5324
			endcase
			case 11
				var a 8783
				var b 5864
			endcase
			case 12
				var a 9083
				var b 6326
			endcase
			case 13
				var a 7057
				var b 3818
			endcase
			case 14
				var a 8883
				var b 3447
			endcase
		endswitch
	NEW_MAP_MON 200202 1 3217 @a @b 0 0 0 0 2 0 0
	NEW_MAP_MON 293600 1 3217 @a @b 0 0 0 0 24 0 0
	add num 1
	endwhile
	
	var num 0
	while @num < 5
	rand 23 15
		switch @23
			case 0
				var a 3562
				var b 3698
			endcase
			case 1
				var a 3724
				var b 3984
			endcase
			case 2
				var a 4364
				var b 4069
			endcase
			case 3
				var a 4193
				var b 3694
			endcase
			case 4
				var a 4628
				var b 3365
			endcase
			case 5
				var a 4816
				var b 3732
			endcase
			case 6
				var a 5272
				var b 4095
			endcase
			case 7
				var a 5507
				var b 3574
			endcase
			case 8
				var a 6125
				var b 3681
			endcase
			case 9
				var a 5737
				var b 4299
			endcase
			case 10
				var a 3901
				var b 4506
			endcase
			case 11
				var a 3547
				var b 4928
			endcase
			case 12
				var a 3816
				var b 5342
			endcase
			case 13
				var a 4085
				var b 6007
			endcase
			case 14
				var a 3620
				var b 6366
			endcase
		endswitch
	NEW_MAP_MON 200202 1 3217 @a @b 0 0 0 0 2 0 0
	NEW_MAP_MON 293600 1 3217 @a @b 0 0 0 0 24 0 0
	add num 1
	endwhile
	
	var num 0
	while @num < 4
	rand 30 12
		switch @30
			case 0
				var a 3593
				var b 7917
			endcase
			case 1
				var a 3985
				var b 8962
			endcase
			case 2
				var a 4194
				var b 9232
			endcase
			case 3
				var a 4395
				var b 8849
			endcase
			case 4
				var a 3737
				var b 8518
			endcase
			case 5
				var a 3933
				var b 8130
			endcase
			case 6
				var a 3602
				var b 7563
			endcase
			case 7
				var a 4887
				var b 9089
			endcase
			case 8
				var a 5480
				var b 8840
			endcase
			case 9
				var a 6147
				var b 8596
			endcase
			case 10
				var a 3802
				var b 7062
			endcase
			case 11
				var a 5811
				var b 8953
			endcase
		endswitch
	NEW_MAP_MON 200202 1 3217 @a @b 0 0 0 0 2 0 0
	NEW_MAP_MON 293601 1 3217 @a @b 0 0 0 0 8 0 0
	add num 1
	endwhile
	
	var num 0
	while @num < 4
	rand 31 12
		switch @31
			case 0
				var a 8756
				var b 8778
			endcase
			case 1
				var a 9274
				var b 8137
			endcase
			case 2
				var a 7976
				var b 8730
			endcase
			case 3
				var a 8438
				var b 8120
			endcase
			case 4
				var a 6965
				var b 8695
			endcase
			case 5
				var a 6429
				var b 9127
			endcase
			case 6
				var a 7832
				var b 9332
			endcase
			case 7
				var a 6921
				var b 9319
			endcase
			case 8
				var a 8908
				var b 7610
			endcase
			case 9
				var a 9287
				var b 7231
			endcase
			case 10
				var a 8699
				var b 6808
			endcase
			case 11
				var a 8420
				var b 9301
			endcase
		endswitch
	NEW_MAP_MON 200202 1 3217 @a @b 0 0 0 0 2 0 0
	NEW_MAP_MON 293601 1 3217 @a @b 0 0 0 0 8 0 0
	add num 1
	endwhile
	
	var num 0
	while @num < 4
	rand 32 12
		switch @32
			case 0
				var a 8672
				var b 3809
			endcase
			case 1
				var a 8719
				var b 4449
			endcase
			case 2
				var a 8022
				var b 3883
			endcase
			case 3
				var a 8780
				var b 3883
			endcase
			case 4
				var a 9190
				var b 5700
			endcase
			case 5
				var a 9229
				var b 4998
			endcase
			case 6
				var a 8614
				var b 4941
			endcase
			case 7
				var a 7866
				var b 3373
			endcase
			case 8
				var a 7273
				var b 4058
			endcase
			case 9
				var a 6480
				var b 3665
			endcase
			case 10
				var a 8131
				var b 4450
			endcase
			case 11
				var a 9147
				var b 3591
			endcase
		endswitch
	NEW_MAP_MON 200202 1 3217 @a @b 0 0 0 0 2 0 0
	NEW_MAP_MON 293601 1 3217 @a @b 0 0 0 0 8 0 0
	add num 1
	endwhile
	
	var num 0
	while @num < 4
	rand 33 12
		switch @33
			case 0
				var a 4059
				var b 4027
			endcase
			case 1
				var a 4285
				var b 3396
			endcase
			case 2
				var a 3538
				var b 4364
			endcase
			case 3
				var a 3858
				var b 4931
			endcase
			case 4
				var a 3931
				var b 5691
			endcase
			case 5
				var a 3837
				var b 6391
			endcase
			case 6
				var a 3513
				var b 5093
			endcase
			case 7
				var a 4663
				var b 3978
			endcase
			case 8
				var a 5060
				var b 3453
			endcase
			case 9
				var a 5550
				var b 3837
			endcase
			case 10
				var a 5414
				var b 4331
			endcase
			case 11
				var a 6105
				var b 4101
			endcase
		endswitch
	NEW_MAP_MON 200202 1 3217 @a @b 0 0 0 0 2 0 0
	NEW_MAP_MON 293601 1 3217 @a @b 0 0 0 0 8 0 0
	add num 1
	endwhile
	
	var num 0
	while @num < 3
	rand 40 9
		switch @40
			case 0
				var a 3650
				var b 9041
			endcase
			case 1
				var a 3907
				var b 8609
			endcase
			case 2
				var a 4456
				var b 9158
			endcase
			case 3
				var a 6598
				var b 8182
			endcase
			case 4
				var a 3864
				var b 7607
			endcase
			case 5
				var a 4025
				var b 6631
			endcase
			case 6
				var a 5197
				var b 8967
			endcase
			case 7
				var a 5463
				var b 8553
			endcase
			case 8
				var a 3537
				var b 7241
			endcase
		endswitch
	NEW_MAP_MON 200202 1 3217 @a @b 0 0 0 0 2 0 0
	NEW_MAP_MON 293602 1 3217 @a @b 0 0 0 0 16 0 0
	add num 1
	endwhile
	
	var num 0
	while @num < 3
	rand 41 9
		switch @41
			case 0
				var a 8564
				var b 7806
			endcase
			case 1
				var a 8355
				var b 8822
			endcase
			case 2
				var a 9196
				var b 8796
			endcase
			case 3
				var a 7806
				var b 8874
			endcase
			case 4
				var a 6642
				var b 8504
			endcase
			case 5
				var a 7178
				var b 9157
			endcase
			case 6
				var a 9256
				var b 6466
			endcase
			case 7
				var a 9099
				var b 7241
			endcase
			case 8
				var a 9073
				var b 8253
			endcase
		endswitch
	NEW_MAP_MON 200202 1 3217 @a @b 0 0 0 0 2 0 0
	NEW_MAP_MON 293602 1 3217 @a @b 0 0 0 0 16 0 0
	add num 1
	endwhile
	
	var num 0
	while @num < 3
	rand 42 9
		switch @42
			case 0
				var a 7870
				var b 3613
			endcase
			case 1
				var a 8419
				var b 4550
			endcase
			case 2
				var a 8624
				var b 4114
			endcase
			case 3
				var a 9003
				var b 5064
			endcase
			case 4
				var a 9016
				var b 5940
			endcase
			case 5
				var a 6502
				var b 4341
			endcase
			case 6
				var a 6715
				var b 3835
			endcase
			case 7
				var a 8933
				var b 3617
			endcase
			case 8
				var a 7582
				var b 4180
			endcase
		endswitch
	NEW_MAP_MON 200202 1 3217 @a @b 0 0 0 0 2 0 0
	NEW_MAP_MON 293602 1 3217 @a @b 0 0 0 0 16 0 0
	add num 1
	endwhile
	
	var num 0
	while @num < 3
	rand 43 9
		switch @43
			case 0
				var a 3931
				var b 3726
			endcase
			case 1
				var a 4562
				var b 3722
			endcase
			case 2
				var a 3611
				var b 4396
			endcase
			case 3
				var a 3948
				var b 5125
			endcase
			case 4
				var a 3816
				var b 6090
			endcase
			case 5
				var a 3731
				var b 4666
			endcase
			case 6
				var a 5182
				var b 3765
			endcase
			case 7
				var a 5886
				var b 3556
			endcase
			case 8
				var a 5779
				var b 4063
			endcase
		endswitch
	NEW_MAP_MON 200202 1 3217 @a @b 0 0 0 0 2 0 0
	NEW_MAP_MON 293602 1 3217 @a @b 0 0 0 0 16 0 0
	add num 1
	endwhile
	
	rand 50 6
		switch @50
			case 0
				var a 3816
				var b 9010
			endcase
			case 1
				var a 4731
				var b 8792
			endcase
			case 2
				var a 3715
				var b 8021
			endcase
			case 3
				var a 5263
				var b 8782
			endcase
			case 4
				var a 6504
				var b 8940
			endcase
			case 5
				var a 7779
				var b 9238
			endcase
		endswitch
	NEW_MAP_MON 200202 1 3217 @a @b 0 0 0 0 2 0 0
	NEW_MAP_MON 293603 1 3217 @a @b 0 0 0 0 48 0 0
	
	rand 51 6
		switch @51
			case 0
				var a 8611
				var b 8304
			endcase
			case 1
				var a 9086
				var b 7735
			endcase
			case 2
				var a 8957
				var b 8881
			endcase
			case 3
				var a 9017
				var b 6982
			endcase
			case 4
				var a 8842
				var b 6286
			endcase
			case 5
				var a 8939
				var b 5377
			endcase
		endswitch
	NEW_MAP_MON 200202 1 3217 @a @b 0 0 0 0 2 0 0
	NEW_MAP_MON 293604 1 3217 @a @b 0 0 0 0 48 0 0
	
	rand 52 6
		switch @52
			case 0
				var a 8110
				var b 4097
			endcase
			case 1
				var a 8713
				var b 3430
			endcase
			case 2
				var a 9013
				var b 4424
			endcase
			case 3
				var a 7545
				var b 3649
			endcase
			case 4
				var a 6163
				var b 4344
			endcase
			case 5
				var a 5280
				var b 3563
			endcase
		endswitch
	NEW_MAP_MON 200202 1 3217 @a @b 0 0 0 0 2 0 0
	NEW_MAP_MON 293605 1 3217 @a @b 0 0 0 0 48 0 0
	
	rand 53 6
		switch @53
			case 0
				var a 3495
				var b 3516
			endcase
			case 1
				var a 3892
				var b 4190
			endcase
			case 2
				var a 4489
				var b 3503
			endcase
			case 3
				var a 3566
				var b 4978
			endcase
			case 4
				var a 4014
				var b 6180
			endcase
			case 5
				var a 3511
				var b 7336
			endcase
		endswitch
	NEW_MAP_MON 200202 1 3217 @a @b 0 0 0 0 2 0 0
	NEW_MAP_MON 293606 1 3217 @a @b 0 0 0 0 48 0 0
	
	var num 0
	while @num < 3
	rand 60 24
		switch @60
			case 0
				var a 3816
				var b 9010
			endcase
			case 1
				var a 4731
				var b 8792
			endcase
			case 2
				var a 3715
				var b 8021
			endcase
			case 3
				var a 5263
				var b 8782
			endcase
			case 4
				var a 6504
				var b 8940
			endcase
			case 5
				var a 7779
				var b 9238
			endcase
			case 6
				var a 8611
				var b 8304
			endcase
			case 7
				var a 9086
				var b 7735
			endcase
			case 8
				var a 8957
				var b 8881
			endcase
			case 9
				var a 9017
				var b 6982
			endcase
			case 10
				var a 8842
				var b 6286
			endcase
			case 11
				var a 8939
				var b 5377
			endcase
			case 12
				var a 8110
				var b 4097
			endcase
			case 13
				var a 8713
				var b 3430
			endcase
			case 14
				var a 9013
				var b 4424
			endcase
			case 15
				var a 7545
				var b 3649
			endcase
			case 16
				var a 6163
				var b 4344
			endcase
			case 17
				var a 5280
				var b 3563
			endcase
			case 18
				var a 3495
				var b 3516
			endcase
			case 19
				var a 3892
				var b 4190
			endcase
			case 20
				var a 4489
				var b 3503
			endcase
			case 21
				var a 3566
				var b 4978
			endcase
			case 22
				var a 4014
				var b 6180
			endcase
			case 23
				var a 3511
				var b 7336
			endcase
		endswitch
	NEW_MAP_MON 200202 1 3217 @a @b 0 0 0 0 2 0 0
	NEW_MAP_MON 293607 1 3217 @a @b 0 0 0 0 24 0 0
	add num 1
	endwhile
	
endproc
