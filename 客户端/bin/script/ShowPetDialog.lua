--GameWizard:setNpcTalk( nMemID, szTalk, "ChatPop", false, 0, 0, "PopBackArrowTexture", true,0,0,0,8 );
--showpos	PETPHOTO ����ͷ�� PETHEAD ����ͷ�� PETBUTTON ���ﰴť
--showtype	CHATPOP �������� CHATTIP ��ʾ
local t_PetDialog = 
	{
		-- �����ͼ��ʱ�� 1000 ��ʾ��ͼID interval��ʾÿ�η��Եļ��ʱ��  rate ��ʾ���Եĸ��� showtype ��ʾ���Ե����� ���ݻ� TIPS showpos��ʾ���Ե�λ�� 
		["EnterMap"]		=	{	startclock=0, interval=0, 
									[1000] =	{ rate=100, showpos="PETHEAD", showtype="CHATPOP", text=
																												{ 
																													"�ۣ�ʱ���Ү����˵�����г���ѵ��ʦ��",
																												}
												};
									[10200] =	{ rate=100, showpos="PETHEAD", showtype="CHATPOP", text=
																												{ 
																													"��������������ѽ��",
																												}
												}
								}; 
		-- �ڵ�ͼ��ʱ�� 1000��ʾ��ͼID
		["InMap"]			=	{  startclock=0, interval=900,
									[1000] =	{ rate=30, showpos="PETPHOTO", showtype="CHATTIP", text=
																										{ 
																													"���ˣ��ѵ���һ��ʱ��ǣ�����ȥ�ҳ���ѵ��ʦ�ɣ�",
																													"ʱ���̫���ˣ���ͷ�����ˣ�",
																										}
												}
								}; 
		-- ���������ʱ��
		["PetCamp"]			=	{ startclock=0, interval=0, rate=60, showpos="PETHEAD", showtype="CHATPOP", text=
																			{ 
																				"�����ڿ����ҳ���͸͸����!",
																				"�ͳ�����ˣ���Ҫ�����ˣ�",
																			}
								};
		-- ����δ������ʱ��
		["PetNotCamp"]		=	{ startclock=0, rate=70, showpos="PETBUTTON", showtype="CHATTIP", interval=120, text=
																								{ 
																									"#petname#�������Ѿ���ֹ�������ˣ�����ҳ�����",
																									"#petname#������ҳ���ѽ���ٲ����ң���ҧ��ѽ��",
																								} 
								};
		-- ����������ʱ��
		["PetDead"]			=	{ startclock=0, interval=0, rate=100, showpos="PETHEAD", showtype="CHATPOP",  text=
																			{ 
																				"�ҡ����ܡ����ˡ�����",
																				"ʮ�����Ӻ�����һ���ú�������㻹�Ϸ��ҳ����Ļ���",
																				"��ѽ���������ۣ���������Ѫ���Ҳ����ˣ�",
																			}
								};
		-- ���ﳤ���ض��ȼ���ʱ�� 20��ʾ����ȼ�
		["PetLv"]			=	{ startclock=0, interval=0, 
									[20] = { rate=100, showpos="PETHEAD", showtype="CHATPOP", text=
																				{ 
																					"��20���ˣ������ˣ�����Ҫȥ��ʰ˭�أ�",
																				}
											}
								};
		-- ����ÿ��������ʱ��
		["PetLevelUp"]		=	{ startclock=0, interval=0, rate=30, showpos="PETHEAD", showtype="CHATPOP",text=
																			{ 
																				"���Ѿ����������ˣ�����",
																			}
								};
		-- �����ҳ϶ȴﵽĳ����Χ��ʱ��  minloveval=3000 ��ʾ��С�ҳ϶�, maxloveval=5000 ��ʾ����ҳ϶�
		["PetLoveval"]		=	{ startclock=0, interval=600, rate=100, showpos="PETPHOTO", showtype="CHATTIP", text=
																			{ 
																				{ minloveval=3000, maxloveval=5000, text ="�㲻�����ˣ�����ι�ҳԵ��ˣ�" },
																			};
								};
		-- ��ҵ�Ѫ������ĳ����Χ��ʱ�� minhp=10 ��ʾ��СѪ��, maxhp=50 ��ʾ���Ѫ��
		["PlayerHpLow"]		=	{ 
									startclock=0, interval=30, rate=100, showpos="PETPHOTO", showtype="CHATTIP",  text =
																				{ 
																					{ minhp=10, maxhp=50, text ="�ϴ���ûѪ�ˣ������ҩ���ѵ�Ҫ�������ι��ô����" },
																					
																				}
								};
		
		-- ��ҵ�װ������ĳ����Χ��ʱ�� arm_unuse (�;ö�Ϊ0 ��) װ�������� arm_brea���;ö�Ϊ0.09��װ����
		["PlayerArmDur"]	=	{	startclock=0, interval=1000,
									["arm_unuse"] =	{ rate=100, showpos="PETPHOTO", showtype="CHATTIP", text =
																		{ 
																			"���ˣ�װ�����ˣ���ȥ��ѽ�������㱼ô��",
																		}
															};
									["arm_break"] =	{  rate=100, showpos="PETHEAD", showtype="CHATPOP", text =
																{
																	"���ˣ�װ�����ˣ���ȥ�ޣ������������ã���Ӱ�������󰡣�",
																}
													};
								};
		-- ��ҵȼ��ﵽ�ض��ȼ���ʱ�� 30��ʾ��ҵȼ�
		["PlayerLv"]		=	{   startclock=0, interval=0, 
									[30]	= { text="��ϲ��������30����", rate=100, showpos="PETHEAD", showtype="CHATPOP"};
								};
		-- ��Ҿ�ֹ������ duration ���ʱ�� 
		["PlayerStand"]		=	{
									 startclock=0, interval=600, rate=50, showpos="PETPHOTO", showtype="CHATTIP", duration=60, text =
																							{ 
																								"���ˣ�����ȥ�ʺ絺��",
																								"���ˣ���˵����������һ���ܹ��ó�����������Ķ�������֪����ʲô��",
																							}
								};
		-- ϵͳ���ʼ��ʱ�� 1 ��ʾ�ID
		["Promotion"]		=	{
									startclock=0, interval=0, [1] = { rate=100, showpos="PETHEAD", showtype="CHATPOP", text="ιιι��˫�������˰����������ɻɱ��ȥѽ��" };
								};
	}

function GetPetDialog( dialogType )
	if t_PetDialog[dialogType].interval ~= 0 and ( os.clock() - t_PetDialog[dialogType].startclock ) < t_PetDialog[dialogType].interval then
		return nil;
	end
	return t_PetDialog[dialogType]; 
end