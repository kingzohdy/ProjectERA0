<<<<<<< .mine
-------------------------------------------------
-- g_nCAREER_WARRIOR = 1;  --战士
-- g_nCAREER_MAGICOR = 2;  --法师
-- g_nCAREER_GUNER   = 3;	--枪手
-- g_nCAREER_CHURCH  = 4;	--牧师
-------------------------------------------------

local nTEQid = 
{
	{},		--战士
	{},		--法师
	{},		--枪手
	{},		--牧师

}



local cmdTable =
{
	{
		"//cmdpglv0",
		0,
		1,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//updateskill 9999 1",

	},
	
	{
		"//cmdpg11",
		0,
		24,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//packageclear 3",
		"//repairall",
		"//applystatus 0 1005 1 7200",
		"//additem 5060002 150",
		"//additem 5060001 99",
		"//addattr money=1000000",
		"//addattr bull=1000000",
		"//additem 5041000 10",
		"//additem 5251000 7",
		"//additem 5050210 99",
		"//additem 5050200 99",
		"//updateskill 9999 1",
		"//additem 5011001 60",
		"//additem 5011002 60",
		"//additem 5011003 60",
		"//additem 5010001 60",
		"//additem 5010002 60",
		"//additem 5010003 60",
		"//additem 5000001 60",
		"//additem 5000002 60",
		"//additem 5000003 60",
		"//additem 5001001 60",
		"//additem 5001002 60",
		"//additem 5001003 60",

	},
	
--	{
--		"//cmdpg11",
--		0,
--		23,
--		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
--		"//packageclear 3",
--		"//repairall",
--		"//applystatus 0 1005 1 7200",
--		"//additem 5060002 99",
--		"//additem 5060001 99",
--		"//addattr money=1000000",
--		"//addattr bull=1000000",
--		"//additem 5041000 10",
--		"//additem 5251000 7",
--		"//additem 5050200 99",
--		"//updateskill 9999 1",
--		"//additem 5011001 99",
--		"//additem 5011002 99",
--		"//additem 5011003 99",
--		"//additem 5010001 99",
--		"//additem 5010002 99",
--		"//additem 5010003 99",
--		"//additem 5000001 99",
--		"//additem 5000002 99",
--		"//additem 5000003 99",
--		"//additem 5001001 99",
--		"//additem 5001002 99",
--		"//additem 5001003 99",

--	},
		
	{
		"//cmdpg0",
		0,
		7,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//addattr money=1000000",
		"//addattr bull=1000000",
		"//repairall",
		"//additem 5041000 10",
		"//updateskill 9999 1",
		"//additem 5251000 7",
		"//additem 5050200 99",
		
	},
		
	{
		"//cmdpg1",
		0,
		1,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//applystatus 0 1017 10 7200",
	
	},	
	
	{
		"//cmdpg3",
		0,
		7,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
--		"//applystatus 0 1051 1 7200",  --无敌
		"//applystatus 0 1215 7 7200",   --伤害1000
		"//applystatus 0 1110 7 7200",   --伤害吸收80%
		"//applystatus 0 1134 3 7200",   --暴击提高1000%
		"//applystatus 0 1133 4 7200",   --暴击率提高100%
		"//applystatus 0 1132 3 7200",   --反击
		"//applystatus 0 1104 7 7200",   --命中率提高100%
		"//applystatus 0 1005 1 7200",   --命中率提高100%
		
	
	},	
	
	{
		"//cmdpgbs",   --gm套装
		1,
		15,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//packageclear 3",
		"//additem 5210000 99",
		"//additem 5210100 99",
		"//additem 5211000 99",
		"//additem 5211100 99",
		"//additem 5211202 99",
		"//additem 5212000 99",
		"//additem 5213302 99",
		"//additem 5212402 99",
		"//additem 5213402 99",
		"//additem 5219400 99",
		"//additem 5219401 20",
		"//additem 5219402 20",
		"//additem 5219403 20",
		"//additem 5219404 20",

	},
	
	{
		"//cmdpg2",
		0,
		11,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//applystatus 0 1017 10 3",
		"//applystatus 0 1015 1 3",
		"//applystatus 0 1051 1 3",
		"//applystatus 0 1110 1 2",   --伤害吸收60%
		"//applystatus 0 1215 1 2",   --攻击提高113
		"//applystatus 0 1107 1 2",   --攻击提高113
		"//applystatus 0 1134 1 2",   --攻击提高113
		"//applystatus 0 1133 1 2",   --攻击提高113
		"//applystatus 0 1132 1 2",   --攻击提高113
		"//applystatus 0 1104 1 2",   --攻击提高113
		"//applystatus 0 1005 1 2",   --攻击提高113

	},
	
	{
		"//cmdpg8",   --gm套装
		1,
		17,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//packageclear 3",
		"//additem 1039999 1",
		"//additem 1069999 1",
		"//additem 1109999 1",
		"//additem 1209999 1",
		"//additem 1289999 1",
		"//additem 1509999 1",
		"//additem 2029999 1",
		"//additem 2129999 1",
		"//additem 2229999 1",
		"//additem 2429999 1",
		"//additem 2629999 1",
		"//additem 2719998 1",
		"//additem 2719999 1",
		"//additem 2819999 1",
		"//additem 2919998 1",
		"//additem 2919999 1",

	
	},
	
	{
		"//cmdpgzs1",   --gm套装
		1,
		53,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//packageclear 3",

		"//additem 1013002 1",

		"//additem 2013002 1",

		"//additem 2113002 1",

		"//additem 2213002 1",

		"//additem 2613002 1",

		"//additem 1063002 1",

		"//additem 1011701 1",

		"//additem 1013003 1",

		"//additem 1033003 1",

		"//additem 2011701 1",

		"//additem 2013003 1",

		"//additem 2113003 1",

		"//additem 2111701 1",

		"//additem 2213003 1",

		"//additem 2211701 1",

		"//additem 2611701 1",

		"//additem 2613003 1",

		"//additem 1061701 1",

		"//additem 1063003 1",

		"//additem 2911721 1",

		"//additem 2911701 1",

		"//additem 2811701 1",

		"//additem 2711701 1",

		"//additem 2711721 1",

				

		--10-30级战士蓝装

		"//additem 1014002 1",

		"//additem 1034002 1",

		"//additem 2014002 1",

		"//additem 2114002 1",

		"//additem 2214002 1",

		"//additem 2614002 1",

		"//additem 1064002 1",

		"//additem 1034003 1",

		"//additem 1014003 1",

		"//additem 2014003 1",

		"//additem 2114003 1",

		"//additem 2214003 1",

		"//additem 2614003 1",

		"//additem 1064003 1",



		--10-30级战士紫装


		"//additem 1035002 1",

		"//additem 1015002 1",

		"//additem 2015002 1",

		"//additem 2115002 1",

		"//additem 2215002 1",

		"//additem 2615002 1",

		"//additem 1065002 1",

		"//additem 1035003 1",

		"//additem 1015003 1",

		"//additem 2015003 1",

		"//additem 2115003 1",

		"//additem 2215003 1",

		"//additem 2615003 1",

		"//additem 1065003 1", 
	},

	{
		"//cmdpgzs2",   --gm套装
		1,
		99,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//packageclear 3",
		--31-60级战士绿装

		"//additem 1033004 1",

		"//additem 1013004 1",

		"//additem 2013004 1",

		"//additem 2113004 1",

		"//additem 2213004 1",

		"//additem 2613004 1",

		"//additem 1063004 1",

		"//additem 1013005 1",

		"//additem 1033005 1",

		"//additem 2013005 1",

		"//additem 2113005 1",

		"//additem 2213005 1",

		"//additem 2613005 1",

		"//additem 1063005 1",

		"//additem 1013006 1",

		"//additem 1033006 1",

		"//additem 2013006 1",

		"//additem 2113006 1",

		"//additem 2213006 1",

		"//additem 2613006 1",

		"//additem 1063006 1", 



		--31-60级战士蓝装

		"//additem 1011601 1",

		"//additem 1031601 1",

		"//additem 1034004 1",

		"//additem 1014004 1",

		"//additem 2014004 1",

		"//additem 2011601 1",

		"//additem 2114004 1",

		"//additem 2111601 1",

		"//additem 2214004 1",

		"//additem 2211601 1",

		"//additem 2611601 1",

		"//additem 2614004 1",

		"//additem 1061601 1",

		"//additem 1064004 1",

		"//additem 2911601 1",

		"//additem 2911621 1",

		"//additem 2811601 1",

		"//additem 2711601 1",

		"//additem 2711621 1",

		"//additem 1014005 1",

		"//additem 1011702 1",

		"//additem 1034005 1",

		"//additem 2014005 1",

		"//additem 2011702 1",

		"//additem 2111702 1",

		"//additem 2114005 1",

		"//additem 2214005 1",

		"//additem 2211702 1",

		"//additem 2611702 1",

		"//additem 2614005 1",

		"//additem 1061702 1",

		"//additem 1064005 1",

		"//additem 2911722 1",

		"//additem 2911702 1",

		"//additem 2811702 1",

		"//additem 2711722 1",

		"//additem 2711702 1",

		"//additem 1011602 1",

		"//additem 1031602 1",

		"//additem 1034006 1",

		"//additem 1014006 1",

		"//additem 2011602 1",

		"//additem 2014006 1",

		"//additem 2114006 1",

		"//additem 2111602 1",

		"//additem 2211602 1",

		"//additem 2214006 1",

		"//additem 2611602 1",

		"//additem 2614006 1",

		"//additem 1061602 1",

		"//additem 1064006 1",

		"//additem 2911602 1",

		"//additem 2911622 1",

		"//additem 2811602 1",

		"//additem 2711602 1",

		"//additem 2711622 1",



		--31-60级战士紫装


		"//additem 1035004 1",

		"//additem 1015004 1",

		"//additem 2015004 1",

		"//additem 2115004 1",

		"//additem 2215004 1",

		"//additem 2615004 1",

		"//additem 1065004 1",

		"//additem 1035005 1",

		"//additem 1015005 1",

		"//additem 2015005 1",

		"//additem 2115005 1",

		"//additem 2215005 1",

		"//additem 2615005 1",

		"//additem 1065005 1",

		"//additem 1015006 1",

		"//additem 1035006 1",

		"//additem 2015006 1",

		"//additem 2115006 1",

		"//additem 2215006 1",

		"//additem 2615006 1",

		"//additem 1065006 1",
	
	
	},
	
	{
		"//cmdpgqs1",   --枪手装备
		1,
		61,
		120,
		"//packageclear 3",
		--10-30级枪手绿装

		"//additem 1203002 1",

		"//additem 1253002 1",

		"//additem 2033002 1",

		"//additem 2133002 1",

		"//additem 1273002 1",

		"//additem 2633002 1",

		"//additem 1283002 1",

		"//additem 1253003 1",

		"//additem 1201701 1",

		"//additem 1203003 1",

		"//additem 2031701 1",

		"//additem 2033003 1",

		"//additem 2131701 1",

		"//additem 2133003 1",

		"//additem 1271701 1",

		"//additem 1273003 1",

		"//additem 2631701 1",

		"//additem 2633003 1",

		"//additem 1283003 1",

		"//additem 1281701 1",

		"//additem 2931701 1",

		"//additem 2931721 1",

		"//additem 2831701 1",

		"//additem 2731721 1",

		"//additem 2731701 1",


		--31-60级枪手绿装

		"//additem 1253004 1",

		"//additem 1203004 1",

		"//additem 2033004 1",

		"//additem 2133004 1",

		"//additem 1273004 1",

		"//additem 2633004 1",

		"//additem 1283004 1",

		"//additem 1253005 1",

		"//additem 1203005 1",

		"//additem 2033005 1",

		"//additem 2133005 1",

		"//additem 1273005 1",

		"//additem 2633005 1",

		"//additem 1283005 1",

		"//additem 1203006 1",

		"//additem 1253006 1",

		"//additem 2033006 1",

		"//additem 2133006 1",

		"//additem 1273006 1",

		"//additem 2633006 1",

		"//additem 1283006 1", 


		--10-30级枪手蓝装


		"//additem 1254002 1",

		"//additem 1204002 1",

		"//additem 2034002 1",

		"//additem 2134002 1",

		"//additem 1274002 1",

		"//additem 2634002 1",

		"//additem 1284002 1",

		"//additem 1204003 1",

		"//additem 1254003 1",

		"//additem 2034003 1",

		"//additem 2134003 1",

		"//additem 1274003 1",

		"//additem 2634003 1",

		"//additem 1284003 1",

	},
	
	
	{
		"//cmdpgqs2",   --枪手装备
		1,
		90,
		120,
		"//packageclear 3",
		--31-60级枪手蓝装

		"//additem 1251601 1",

		"//additem 1204004 1",

		"//additem 1201601 1",

		"//additem 1254004 1",

		"//additem 2034004 1",

		"//additem 2031601 1",

		"//additem 2134004 1",

		"//additem 2131601 1",

		"//additem 1271601 1",

		"//additem 1274004 1",

		"//additem 2631601 1",

		"//additem 2634004 1",

		"//additem 1281601 1",

		"//additem 1284004 1",

		"//additem 2931601 1",

		"//additem 2931621 1",

		"//additem 2831601 1",

		"//additem 2731621 1",

		"//additem 2731601 1",

		"//additem 1201702 1",

		"//additem 1204005 1",

		"//additem 1254005 1",

		"//additem 2034005 1",

		"//additem 2031702 1",

		"//additem 2131702 1",

		"//additem 2134005 1",

		"//additem 1274005 1",

		"//additem 1271702 1",

		"//additem 2634005 1",

		"//additem 2631702 1",

		"//additem 1284005 1",

		"//additem 1281702 1",

		"//additem 2931722 1",

		"//additem 2931702 1",

		"//additem 2831702 1",

		"//additem 2731722 1",

		"//additem 2731702 1",

		"//additem 1201602 1",

		"//additem 1251602 1",

		"//additem 1254006 1",

		"//additem 1204006 1",

		"//additem 2034006 1",

		"//additem 2031602 1",

		"//additem 2134006 1",

		"//additem 2131602 1",

		"//additem 1271602 1",

		"//additem 1274006 1",

		"//additem 2631602 1",

		"//additem 2634006 1",

		"//additem 1281602 1",

		"//additem 1284006 1",

		"//additem 2931602 1",

		"//additem 2931622 1",

		"//additem 2831602 1",

		"//additem 2731622 1",

		"//additem 2731602 1", 


		--10-30级枪手紫装

		"//additem 1205002 1",

		"//additem 1255002 1",

		"//additem 2035002 1",

		"//additem 2135002 1",

		"//additem 1275002 1",

		"//additem 2635002 1",

		"//additem 1205003 1",

		"//additem 1255003 1",

		"//additem 2035003 1",

		"//additem 2135003 1",

		"//additem 1275003 1",

		"//additem 2635003 1",

		"//additem 1285003 1",


		--31-60级枪手紫装

		"//additem 1205004 1",

		"//additem 1255004 1",

		"//additem 2035004 1",

		"//additem 2135004 1",

		"//additem 1275004 1",

		"//additem 2635004 1",

		"//additem 1285004 1",

		"//additem 1205005 1",

		"//additem 1255005 1",

		"//additem 2035005 1",

		"//additem 2135005 1",

		"//additem 1275005 1",

		"//additem 2635005 1",

		"//additem 1205006 1",

		"//additem 1255006 1",

		"//additem 2035006 1",

		"//additem 2135006 1",

		"//additem 1275006 1",

		"//additem 2635006 1",

		"//additem 1285006 1", 
	},
	
	{
		"//cmdpgfs1",   --法师装备
		1,
		54,
		120,
		"//packageclear 3",
		

		--10-30级法师绿装

		"//additem 1103002 1",

		"//additem 2023002 1",

		"//additem 2123002 1",

		"//additem 2223002 1",

		"//additem 2623002 1",

		"//additem 1183002 1",

		"//additem 1101701 1",

		"//additem 1103003 1",

		"//additem 2023003 1",

		"//additem 2021701 1",

		"//additem 2123003 1",

		"//additem 2121701 1",

		"//additem 2223003 1",

		"//additem 2221701 1",

		"//additem 2623003 1",

		"//additem 2621701 1",

		"//additem 1181701 1",

		"//additem 1183003 1",

		"//additem 2921701 1",

		"//additem 2921721 1",

		"//additem 2821701 1",

		"//additem 2721701 1",

		"//additem 2721721 1",

		--31-60级法师绿装

		"//additem 1103004 1",

		"//additem 2023004 1",

		"//additem 2123004 1",

		"//additem 2223004 1",

		"//additem 2623004 1",

		"//additem 1183004 1",

		"//additem 1103005 1",

		"//additem 2023005 1",

		"//additem 2123005 1",

		"//additem 2223005 1",

		"//additem 2623005 1",

		"//additem 1183005 1",

		"//additem 1103006 1",

		"//additem 2023006 1",

		"//additem 2123006 1",

		"//additem 2223006 1",

		"//additem 2623006 1",

		"//additem 1183006 1",


		--10-30级法师蓝装


		"//additem 1104002 1",

		"//additem 2024002 1",

		"//additem 2124002 1",

		"//additem 2224002 1",

		"//additem 2624002 1",

		"//additem 1184002 1",

		"//additem 1104003 1",

		"//additem 2024003 1",

		"//additem 2124003 1",

		"//additem 2224003 1",

		"//additem 2624003 1",

		"//additem 1184003 1",
	},
	
	
	{
		"//cmdpgfs2",   --法师装备
		1,
		90,
		120,
		"//packageclear 3",
		--31-60级法师蓝装

		"//additem 1104004 1",

		"//additem 1101601 1",

		"//additem 2024004 1",

		"//additem 2021601 1",

		"//additem 2124004 1",

		"//additem 2121601 1",

		"//additem 2221601 1",

		"//additem 2224004 1",

		"//additem 2621601 1",

		"//additem 2624004 1",

		"//additem 1184004 1",

		"//additem 1181601 1",

		"//additem 2921621 1",

		"//additem 2921601 1",

		"//additem 2821601 1",

		"//additem 2721621 1",

		"//additem 2721601 1",

		"//additem 1104005 1",

		"//additem 1101702 1",

		"//additem 2021702 1",

		"//additem 2024005 1",

		"//additem 2124005 1",

		"//additem 2121702 1",

		"//additem 2224005 1",

		"//additem 2221702 1",

		"//additem 2621702 1",

		"//additem 2624005 1",

		"//additem 1184005 1",

		"//additem 1181702 1",

		"//additem 2921722 1",

		"//additem 2921702 1",

		"//additem 2821702 1",

		"//additem 2721722 1",

		"//additem 2721702 1",

		"//additem 1101602 1",

		"//additem 1104006 1",

		"//additem 2024006 1",

		"//additem 2021602 1",

		"//additem 2124006 1",

		"//additem 2121602 1",

		"//additem 2221602 1",

		"//additem 2224006 1",

		"//additem 2621602 1",

		"//additem 2624006 1",

		"//additem 1181602 1",

		"//additem 1184006 1",

		"//additem 2921602 1",

		"//additem 2921622 1",

		"//additem 2821602 1",

		"//additem 2721602 1",

		"//additem 2721622 1",

		--10-30级法师紫装

		"//additem 1105002 1",

		"//additem 2025002 1",

		"//additem 2125002 1",

		"//additem 2225002 1",

		"//additem 2625002 1",

		"//additem 1185002 1",

		"//additem 1105003 1",

		"//additem 2025003 1",

		"//additem 2125003 1",

		"//additem 2225003 1",

		"//additem 2625003 1",

		"//additem 1185003 1",


		--31-60级法师紫装

		"//additem 1105004 1",

		"//additem 2025004 1",

		"//additem 2125004 1",

		"//additem 2225004 1",

		"//additem 2625004 1",

		"//additem 1185004 1",

		"//additem 1105005 1",

		"//additem 2025005 1",

		"//additem 2125005 1",

		"//additem 2225005 1",

		"//additem 2625005 1",

		"//additem 1185005 1",

		"//additem 1105006 1",

		"//additem 2025006 1",

		"//additem 2125006 1",

		"//additem 2225006 1",

		"//additem 2625006 1",

		"//additem 1185006 1",

	},
		
	{
		"//cmdpgjs1",   --祭司装备
		1,
		42,
		120,
		"//packageclear 3",
		--10-30级祭司绿装

		"//additem 1503002 1",

		"//additem 2043002 1",

		"//additem 2143002 1",

		"//additem 2243002 1",

		"//additem 2643002 1",

		"//additem 1583002 1",

		"//additem 1501701 1",

		"//additem 1503003 1",

		"//additem 2043003 1",

		"//additem 2041701 1",

		"//additem 2143003 1",

		"//additem 2141701 1",

		"//additem 2243003 1",

		"//additem 2241701 1",

		"//additem 2643003 1",

		"//additem 2641701 1",

		"//additem 1581701 1",

		"//additem 1583003 1",

		"//additem 2941701 1",

		"//additem 2941721 1",

		"//additem 2841701 1",

		"//additem 2741701 1",

		"//additem 2741721 1",


		--31-60级祭司绿装

		"//additem 1503004 1",

		"//additem 2043004 1",

		"//additem 2143004 1",

		"//additem 2243004 1",

		"//additem 2643004 1",

		"//additem 1583004 1",

		"//additem 1503005 1",

		"//additem 2043005 1",

		"//additem 2143005 1",

		"//additem 2243005 1",

		"//additem 2643005 1",

		"//additem 1583005 1",

		"//additem 1503006 1",

		"//additem 2043006 1",

		"//additem 2143006 1",

		"//additem 2243006 1",

		"//additem 2643006 1",

		"//additem 1583006 1",


		--10-30级祭司蓝装

		"//additem 1504002 1",

		"//additem 2044002 1",

		"//additem 2144002 1",

		"//additem 2244002 1",

		"//additem 2644002 1",

		"//additem 1584002 1",

		"//additem 1504003 1",

		"//additem 2044003 1",

		"//additem 2144003 1",

		"//additem 2244003 1",

		"//additem 2644003 1",

		"//additem 1584003 1",
	},
	
	
		{
		"//cmdpgjs2",   --祭司装备
		1,
		82,
		120,
		"//packageclear 3",
		--31-60级祭司蓝装


		"//additem 1501601 1",

		"//additem 1504004 1",

		"//additem 2041601 1",

		"//additem 2044004 1",

		"//additem 2144004 1",

		"//additem 2141601 1",

		"//additem 2241601 1",

		"//additem 2244004 1",

		"//additem 2644004 1",

		"//additem 2641601 1",

		"//additem 1581601 1",

		"//additem 1584004 1",

		"//additem 2941601 1",

		"//additem 2941621 1",

		"//additem 2841601 1",

		"//additem 2741601 1",

		"//additem 2741621 1",

		"//additem 1501702 1",

		"//additem 1504005 1",

		"//additem 2044005 1",

		"//additem 2041702 1",

		"//additem 2141702 1",

		"//additem 2144005 1",

		"//additem 2244005 1",

		"//additem 2241702 1",

		"//additem 2641702 1",

		"//additem 2644005 1",

		"//additem 1584005 1",

		"//additem 1581702 1",

		"//additem 2941722 1",

		"//additem 2941702 1",

		"//additem 2841702 1",

		"//additem 2741722 1",

		"//additem 2741702 1",

		"//additem 1501602 1",

		"//additem 1504006 1",

		"//additem 2041602 1",

		"//additem 2044006 1",

		"//additem 2141602 1",

		"//additem 2144006 1",

		"//additem 2244006 1",

		"//additem 2241602 1",

		"//additem 2644006 1",

		"//additem 2641602 1",

		"//additem 1584006 1",

		"//additem 1581602 1",

		"//additem 2941602 1",

		"//additem 2941622 1",

		"//additem 2841602 1",

		"//additem 2741622 1",

		"//additem 2741602 1", 


		--10-30级祭司紫装

		"//additem 1505002 1",

		"//additem 2045002 1",

		"//additem 2145002 1",

		"//additem 2245002 1",

		"//additem 2645002 1",

		"//additem 1585002 1",

		"//additem 1505003 1",

		"//additem 2045003 1",

		"//additem 2145003 1",

		"//additem 2245003 1",

		"//additem 2645003 1",

		"//additem 1585003 1",


		--31-60级祭司紫装

		"//additem 1505004 1",

		"//additem 2045004 1",

		"//additem 2145004 1",

		"//additem 2245004 1",

		"//additem 2645004 1",

		"//additem 1585004 1",

		"//additem 1505005 1",

		"//additem 2045005 1",

		"//additem 2145005 1",

		"//additem 2245005 1",

		"//additem 2645005 1",

		"//additem 1585005 1",

		"//additem 1505006 1",

		"//additem 2045006 1",

		"//additem 2145006 1",

		"//additem 2245006 1",

		"//additem 2645006 1",

		"//additem 1585006 1",

		
	
	},
	{
		"//cmdpg50zslan",   --50战士蓝
		1,
		15,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//packageclear 3",
		"//additem 1201601 1",
		"//additem 1201602 1",
--		"//additem 1014007 1",
		
		"//additem 2131601 1",
		"//additem 2131602 1",
--		"//additem 1064007 1",
		
		"//additem 2031601 1",
		"//additem 2031602 1",
--		"//additem 2014007 1",
		
		"//additem 1281601 1",
		"//additem 1281602 1",
--		"//additem 2114007 1",
		
		"//additem 2431601 1",
		"//additem 2431602 1",
--		"//additem 2214007 1",
		
		"//additem 1271601 1",
		"//additem 1271602 1",
--		"//additem 2414007 1",
		
		"//additem 2631601 1",
		"//additem 2631602 1",
--		"//additem 2614007 1",
--		
--		"//additem 2714006 1",
--		"//additem 2714006 1",
----		"//additem 2714007 1",
--		
--		"//additem 2814006 1",
--		"//additem 2814006 1",
----		"//additem 2814007 1",
--		
--		"//additem 2914006 1",
--		"//additem 2914006 1",
--		"//additem 2914007 1",
		
--		"//additem 1034007 1",
--		"//additem 1034007 1",
		
	
	},
	
	{
		"//cmdpg60zslan",			--60战士蓝
		1,
		21,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//packageclear 3",
		"//additem 1014007 1",
		"//additem 1014007 1",
		
		"//additem 1064007 1",
		"//additem 1064007 1",
		
		"//additem 2014007 1",
		"//additem 2014007 1",
		
		"//additem 2114007 1",
		"//additem 2114007 1",
		
		"//additem 2214007 1",
		"//additem 2214007 1",
		
		"//additem 2414007 1",
		"//additem 2414007 1",
		
		"//additem 2614007 1",
		"//additem 2614007 1",
		
		"//additem 2714007 1",
		"//additem 2714007 1",
		
		"//additem 2814007 1",
		"//additem 2814007 1",
		
		"//additem 2914007 1",
		"//additem 2914007 1",
		
		"//additem 1034007 1",
		"//additem 1034007 1",
		
	
	},	
	{
		"//cmdpg60zszi",			--60战士紫
		1,
		21,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//packageclear 3",
		"//additem 1015007 1",
		"//additem 1015007 1",
		
		"//additem 1065007 1",
		"//additem 1065007 1",
		
		"//additem 2015007 1",
		"//additem 2015007 1",
		
		"//additem 2115007 1",
		"//additem 2115007 1",
		
		"//additem 2215007 1",
		"//additem 2215007 1",
		
		"//additem 2415007 1",
		"//additem 2415007 1",
		
		"//additem 2615007 1",
		"//additem 2615007 1",
		
		"//additem 2715007 1",
		"//additem 2715007 1",
		
		"//additem 2815007 1",
		"//additem 2815007 1",
		
		"//additem 2915007 1",
		"//additem 2915007 1",
		
		"//additem 1035007 1",
		"//additem 1035007 1",
		
	
	},	
	
	{
		"//cmdpg60zslv",			--60战士绿
		1,
		21,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//packageclear 3",
		"//additem 1013007 1",
		"//additem 1013007 1",
		
		"//additem 1063007 1",
		"//additem 1063007 1",
		
		"//additem 2013007 1",
		"//additem 2013007 1",
		
		"//additem 2113007 1",
		"//additem 2113007 1",
		
		"//additem 2213007 1",
		"//additem 2213007 1",
		
		"//additem 2413007 1",
		"//additem 2413007 1",
		
		"//additem 2613007 1",
		"//additem 2613007 1",
		
		"//additem 2713007 1",
		"//additem 2713007 1",
		
		"//additem 2813007 1",
		"//additem 2813007 1",
		
		"//additem 2913007 1",
		"//additem 2913007 1",
		
		"//additem 1033007 1",
		"//additem 1033007 1",
		
	
	},
	
	{
		"//cmdpg60fslv",			--60法师绿
		1,
		21,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//packageclear 3",
		"//additem 1103007 1",
		"//additem 1103007 1",
		"//additem 1183007 1",
		"//additem 1183007 1",
		"//additem 2023007 1",
		"//additem 2023007 1",
		"//additem 2123007 1",
		"//additem 2123007 1",
		"//additem 2223007 1",
		"//additem 2223007 1",
		"//additem 2423007 1",
		"//additem 2423007 1",
		"//additem 2623007 1",
		"//additem 2623007 1",
		"//additem 2713007 1",
		"//additem 2713007 1",
		"//additem 2813007 1",
		"//additem 2813007 1",
		"//additem 2923007 1",
		"//additem 2923007 1",
	
	},	
	
	{
		"//cmdpg50fslan",			--50法师蓝
		1,
		21,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//packageclear 3",
		"//additem 1104006 1",
		"//additem 1104006 1",
		"//additem 1184006 1",
		"//additem 1184006 1",
		"//additem 2024006 1",
		"//additem 2024006 1",
		"//additem 2124006 1",
		"//additem 2124006 1",
		"//additem 2224006 1",
		"//additem 2224006 1",
		"//additem 2424006 1",
		"//additem 2424006 1",
		"//additem 2624006 1",
		"//additem 2624006 1",
		"//additem 2714006 1",
		"//additem 2714006 1",
		"//additem 2814006 1",
		"//additem 2814006 1",
		"//additem 2924006 1",
		"//additem 2924006 1",
	
	},	
	
	{
		"//cmdpg60fslan",			--60法师蓝
		1,
		21,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//packageclear 3",
		"//additem 1104007 1",
		"//additem 1104007 1",
		"//additem 1184007 1",
		"//additem 1184007 1",
		"//additem 2024007 1",
		"//additem 2024007 1",
		"//additem 2124007 1",
		"//additem 2124007 1",
		"//additem 2224007 1",
		"//additem 2224007 1",
		"//additem 2424007 1",
		"//additem 2424007 1",
		"//additem 2624007 1",
		"//additem 2624007 1",
		"//additem 2714007 1",
		"//additem 2714007 1",
		"//additem 2814007 1",
		"//additem 2814007 1",
		"//additem 2924007 1",
		"//additem 2924007 1",
	
	},	
	
	{
		"//cmdpg60fszi",		--60法师紫
		1,
		21,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//packageclear 3",
		"//additem 1105007 1",
		"//additem 1105007 1",
		"//additem 1185007 1",
		"//additem 1185007 1",
		"//additem 2025007 1",
		"//additem 2025007 1",
		"//additem 2125007 1",
		"//additem 2125007 1",
		"//additem 2225007 1",
		"//additem 2225007 1",
		"//additem 2425007 1",
		"//additem 2425007 1",
		"//additem 2625007 1",
		"//additem 2625007 1",
		"//additem 2715007 1",
		"//additem 2715007 1",
		"//additem 2815007 1",
		"//additem 2815007 1",
		"//additem 2925007 1",
		"//additem 2925007 1",
	
	},	
	
	{
		"//cmdpg50qslv",		--50枪手绿
		1,
		23,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//packageclear 3",
		"//additem 1203006 1",
		"//additem 1253006 1",
		"//additem 1283006 1",
		"//additem 2033006 1",
		"//additem 2133006 1",
		"//additem 1273006 1",
		"//additem 2433006 1",
		"//additem 2633006 1",
		"//additem 2733006 1",
		"//additem 2833006 1",
		"//additem 2933006 1",
		"//additem 1203006 1",
		"//additem 1253006 1",
		"//additem 1283006 1",
		"//additem 2033006 1",
		"//additem 2133006 1",
		"//additem 1273006 1",
		"//additem 2433006 1",
		"//additem 2633006 1",
		"//additem 2733006 1",
		"//additem 2833006 1",
		"//additem 2933006 1",
	
	},	
	
	{
		"//cmdpg60qslv",		--60枪手绿
		1,
		23,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//packageclear 3",
		"//additem 1203007 1",
		"//additem 1253007 1",
		"//additem 1283007 1",
		"//additem 2033007 1",
		"//additem 2133007 1",
		"//additem 1273007 1",
		"//additem 2433007 1",
		"//additem 2633007 1",
		"//additem 2733007 1",
		"//additem 2833007 1",
		"//additem 2933007 1",
		
		"//additem 1203007 1",
		"//additem 1253007 1",
		"//additem 1283007 1",
		"//additem 2033007 1",
		"//additem 2133007 1",
		"//additem 1273007 1",
		"//additem 2433007 1",
		"//additem 2633007 1",
		"//additem 2733007 1",
		"//additem 2833007 1",
		"//additem 2933007 1",
	
	},		
	
	{
		"//cmdpg60qslan",		--60枪手绿
		1,
		23,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//packageclear 3",
		"//additem 1204007 1",
		"//additem 1254007 1",
		"//additem 1284007 1",
		"//additem 2034007 1",
		"//additem 2134007 1",
		"//additem 1274007 1",
		"//additem 2434007 1",
		"//additem 2634007 1",
		"//additem 2734007 1",
		"//additem 2834007 1",
		"//additem 2934007 1",
		
		"//additem 1204007 1",
		"//additem 1254007 1",
		"//additem 1284007 1",
		"//additem 2034007 1",
		"//additem 2134007 1",
		"//additem 1274007 1",
		"//additem 2434007 1",
		"//additem 2634007 1",
		"//additem 2734007 1",
		"//additem 2834007 1",
		"//additem 2934007 1",
	
	},	
	
	{
		"//cmdpg60qszi",		--60枪手绿
		1,
		23,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//packageclear 3",
		"//additem 1205007 1",
		"//additem 1255007 1",
		"//additem 1285007 1",
		"//additem 2035007 1",
		"//additem 2135007 1",
		"//additem 1275007 1",
		"//additem 2435007 1",
		"//additem 2635007 1",
		"//additem 2735007 1",
		"//additem 2835007 1",
		"//additem 2935007 1",
		
		"//additem 1205007 1",
		"//additem 1255007 1",
		"//additem 1285007 1",
		"//additem 2035007 1",
		"//additem 2135007 1",
		"//additem 1275007 1",
		"//additem 2435007 1",
		"//additem 2635007 1",
		"//additem 2735007 1",
		"//additem 2835007 1",
		"//additem 2935007 1",
	
	},	
	
	{
		"//cmdpg50jslv",		--50祭祀绿
		1,
		23,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//packageclear 3",
		"//additem 1503006 1",
		"//additem 1583006 1",
		"//additem 2043006 1",
		"//additem 2143006 1",
		"//additem 2243006 1",
		"//additem 2443006 1",
		"//additem 2643006 1",
		"//additem 2743006 1",
		"//additem 2733006 1",
		"//additem 2843006 1",
		"//additem 2943006 1",
		
		"//additem 1503006 1",
		"//additem 1583006 1",
		"//additem 2043006 1",
		"//additem 2143006 1",
		"//additem 2243006 1",
		"//additem 2443006 1",
		"//additem 2643006 1",
		"//additem 2743006 1",
		"//additem 2733006 1",
		"//additem 2843006 1",
		"//additem 2943006 1",
	
	},	
	
	
	{
		"//cmdpg60jslv",		--60祭祀绿
		1,
		23,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//packageclear 3",
		"//additem 1503007 1",
		"//additem 1583007 1",
		"//additem 2043007 1",
		"//additem 2143007 1",
		"//additem 2243007 1",
		"//additem 2443007 1",
		"//additem 2643007 1",
		"//additem 2743007 1",
		"//additem 2733007 1",
		"//additem 2843007 1",
		"//additem 2943007 1",
		
		"//additem 1503007 1",
		"//additem 1583007 1",
		"//additem 2043007 1",
		"//additem 2143007 1",
		"//additem 2243007 1",
		"//additem 2443007 1",
		"//additem 2643007 1",
		"//additem 2743007 1",
		"//additem 2733007 1",
		"//additem 2843007 1",
		"//additem 2943007 1",
	
	},

	{
		"//cmdpg60jslan",		--60祭祀绿
		1,
		23,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//packageclear 3",
		"//additem 1504007 1",
		"//additem 1584007 1",
		"//additem 2044007 1",
		"//additem 2144007 1",
		"//additem 2244007 1",
		"//additem 2444007 1",
		"//additem 2644007 1",
		"//additem 2744007 1",
		"//additem 2734007 1",
		"//additem 2844007 1",
		"//additem 2944007 1",
		
		"//additem 1504007 1",
		"//additem 1584007 1",
		"//additem 2044007 1",
		"//additem 2144007 1",
		"//additem 2244007 1",
		"//additem 2444007 1",
		"//additem 2644007 1",
		"//additem 2744007 1",
		"//additem 2734007 1",
		"//additem 2844007 1",
		"//additem 2944007 1",
	
	},	

	{
		"//cmdpg60jszi",		--60祭祀绿
		1,
		23,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//packageclear 3",
		"//additem 1505007 1",
		"//additem 1585007 1",
		"//additem 2045007 1",
		"//additem 2145007 1",
		"//additem 2245007 1",
		"//additem 2445007 1",
		"//additem 2645007 1",
		"//additem 2745007 1",
		"//additem 2735007 1",
		"//additem 2845007 1",
		"//additem 2945007 1",
		
		"//additem 1505007 1",
		"//additem 1585007 1",
		"//additem 2045007 1",
		"//additem 2145007 1",
		"//additem 2245007 1",
		"//additem 2445007 1",
		"//additem 2645007 1",
		"//additem 2745007 1",
		"//additem 2735007 1",
		"//additem 2845007 1",
		"//additem 2945007 1",
	
	},	
	
	{
		"//cmdpg50shipin",		--60祭祀绿
		1,
		17,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//packageclear 3",

		"//additem 2924006 1",
		"//additem 2924006 1",
		"//additem 2924006 1",
		"//additem 2924006 1",
		
		"//additem 2834006 1",
		"//additem 2834006 1",
		"//additem 2834006 1",
		"//additem 2834006 1",
		
		"//additem 2914006 1",
		"//additem 2914006 1",
		"//additem 2914006 1",
		"//additem 2914006 1",

		"//additem 2714006 1",
		"//additem 2714006 1",
		"//additem 2714006 1",
		"//additem 2714006 1",
	
	},	
		
	{
		"//cmdpg60shipinlan",		--60祭祀绿
		1,
		17,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//packageclear 3",

		"//additem 2924007 1",
		"//additem 2924007 1",
		"//additem 2924007 1",
		"//additem 2924007 1",
		
		"//additem 2834007 1",
		"//additem 2834007 1",
		"//additem 2834007 1",
		"//additem 2834007 1",
		
		"//additem 2914007 1",
		"//additem 2914007 1",
		"//additem 2914007 1",
		"//additem 2914007 1",

		"//additem 2714007 1",
		"//additem 2714007 1",
		"//additem 2714007 1",
		"//additem 2714007 1",
	
	},	
	{
		"//cmdpg60shipinlv",		--60祭祀绿
		1,
		17,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//packageclear 3",

		"//additem 2923007 1",
		"//additem 2923007 1",
		"//additem 2923007 1",
		"//additem 2923007 1",
		
		"//additem 2833007 1",
		"//additem 2833007 1",
		"//additem 2833007 1",
		"//additem 2833007 1",
		
		"//additem 2913007 1",
		"//additem 2913007 1",
		"//additem 2913007 1",
		"//additem 2913007 1",

		"//additem 2713007 1",
		"//additem 2713007 1",
		"//additem 2713007 1",
		"//additem 2713007 1",
	
	},	
	
	{
		"//cmdpg60shipinzi",		--60祭祀绿
		1,
		17,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//packageclear 3",

		"//additem 2925007 1",
		"//additem 2925007 1",
		"//additem 2925007 1",
		"//additem 2925007 1",
		
		"//additem 2835007 1",
		"//additem 2835007 1",
		"//additem 2835007 1",
		"//additem 2835007 1",
		
		"//additem 2915007 1",
		"//additem 2915007 1",
		"//additem 2915007 1",
		"//additem 2915007 1",

		"//additem 2715007 1",
		"//additem 2715007 1",
		"//additem 2715007 1",
		"//additem 2715007 1",
	
	},
	
	{
		"//cmdpgg",  		--积累数
		0,
		2,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//addcollectnum 1000",
		"//skillcool",
		
	},
	
	{
		"//cmdpglv",
		0,
		2,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//addattr exp=100000000",
		"//additem 5030021 1",
	
	},
		
	{
		"//cmdpggw",  		--标准怪物
		0,
		2,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//applystatus 0 1051 1 10000",   --无敌
		"//monster 233020 1",
		
	},
	{
		"//cmdpgskill",  		--圣祭祀技能
		0,
		25,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//packageclear 3",
		"//additem 5030022 1",
		"//additem 5030023 1",
		"//additem 5030024 1",
		"//additem 5030025 1",
		"//additem 5030026 1",
		"//additem 5030027 1",
		"//additem 5030028 1",
		"//additem 5030029 1",
		"//additem 5030030 1",
		"//additem 5030031 1",
		"//additem 5030032 1",
		"//additem 5030033 1",
		"//additem 5030034 1",
		"//additem 5030035 1",
		"//additem 5030036 1",
		"//additem 5030037 1",
		"//additem 1014007 1",
		"//additem 1034007 1",
		"//additem 1104007 1",
		"//additem 1114007 1",
		"//additem 1204007 1",
		"//additem 1254007 1",
		"//additem 1504007 1",
		"//additem 1514007 1",
	
	},
}

function InsterGMPackage( cmdpg )
	table.insert( cmdTable,cmdpg );
end

function CallGMPackage( cmd )
	local mainPlayer = ActorMgr:getMainPlayer();
	local playerType = mainPlayer:getRace();
--	ShowMidTips("ceshi");
	for i = 1, table.getn(cmdTable) do
		if cmd == cmdTable[i][1] then  --暂时忽略职业
--		if cmd == cmdTable[i][1] and ( playerType == cmdTable[i][2] or cmdTable[i][2] == 0 )then
--			for j = 5, table.getn(cmdTable)[i] do  --更多道具
--			ShowMidTips("ceshi1 = "..i);
			for j = 5, cmdTable[i][3]+4 do
				SetGMExcCmd( cmdTable[i][j] );
				if string.find( cmdTable[i][j], "//cmd", 1, true )
				and mainPlayer:getLv() <= cmdTable[i][4] then
					CallGMPackage( cmdTable[i][j] );
				end
			end
		end
	end
	
	SetGMCMDBeginExc( os.clock() );
=======
-------------------------------------------------
-- g_nCAREER_WARRIOR = 1;  --战士
-- g_nCAREER_MAGICOR = 2;  --法师
-- g_nCAREER_GUNER   = 3;	--枪手
-- g_nCAREER_CHURCH  = 4;	--牧师
-------------------------------------------------

local cmdTable =
{
	{
		"//cmdpgtest (%d+)",
		0,
		120,
		"//applystatus %1 1017 1 100000",
		"//applystatus %1 1005 1 100000",
		"//applystatus %1 1005 2 100000",
		"//applystatus %1 1005 3 100000",
		"//applystatus %1 1005 4 100000",
		"//applystatus %1 1202 1 100000",
		"//applystatus %1 1202 2 100000",
	},
	{
		"//cmdpglv0",
		0,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//updateskill 9997 1",
		"//updateskill 9999 1",

	},


	{
		"//cmdpgsuitt1",
		1,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//additem 1011601 1",
		"//additem 1061601 1",
		"//additem 2011601 1",
		"//additem 2111601 1",
		"//additem 2211601 1",
		"//additem 2411601 1"

	},
	{
		"//cmdpgsuitt1",
		2,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//additem 1101601 1",
		"//additem 1181601 1",
		"//additem 2021601 1",
		"//additem 2121601 1",
		"//additem 2221601 1",
		"//additem 2421601 1"

	},
	{
		"//cmdpgsuitt1",
		3,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//additem 1201601 1",
		"//additem 1251601 1",
		"//additem 2031601 1",
		"//additem 2131601 1",
		"//additem 1271601 1",
		"//additem 2431601 1"

	},
	{
		"//cmdpgsuitt1",
		4,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//additem 1501601 1",
		"//additem 1581601 1",
		"//additem 2041601 1",
		"//additem 2141601 1",
		"//additem 2241601 1",
		"//additem 2441601 1"

	},


	{
		"//cmdpgsuitt2",
		1,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//cmdpgsuitt1",
		"//additem 1011602 1",
		"//additem 1061602 1",
		"//additem 2011602 1",
		"//additem 2111602 1",
		"//additem 2211602 1",
		"//additem 2411602 1"
	},
	{
		"//cmdpgsuitt2",
		2,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//cmdpgsuitt1",
		"//additem 1101602 1",
		"//additem 1181602 1",
		"//additem 2021602 1",
		"//additem 2121602 1",
		"//additem 2221602 1",
		"//additem 2421602 1"
	},
	{
		"//cmdpgsuitt2",
		3,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//cmdpgsuitt1",
                "//additem 1201602 1",
		"//additem 1251602 1",
		"//additem 2031602 1",
		"//additem 2131602 1",
		"//additem 1271602 1",
		"//additem 2431602 1"
	},
	{
		"//cmdpgsuitt2",
		4,
		120,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//cmdpgsuitt1",
		"//additem 1501602 1",
		"//additem 1581602 1",
		"//additem 2041602 1",
		"//additem 2141602 1",
		"//additem 2241602 1",
		"//additem 2441602 1"
	},
	{
		"//cmdpglv5",
		1,
		5,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//cmdpglv1",
		"//addattr level=4",
		"//updateskill 1022 1",
		"//updateskill 1018 1",
		"//additem 1019901 1",
		"//additem 2019901 1",
		"//additem 5000003 99",
		"//additem 5001003 99",
		"//addattr money=3000000 1",
		"//addattr bull=3000000 1"
	},
	{
		"//cmdpglv5",
		2,
		5,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//cmdpglv1",
                "//addattr level=4",
		"//updateskill 2020 1",
		"//updateskill 2028 1",
		"//additem 1109901 1",
		"//additem 2029901 1",
		"//additem 5000003 99",
		"//additem 5001003 99",
		"//addattr money=3000000 1",
		"//addattr bull=3000000 1"
	},
	{
		"//cmdpglv5",
		3,
		5,
		"//cmdpglv1",
		"//addattr level=4",
		"//updateskill 3022 1",
		"//updateskill 3024 1",
		"//additem 1209901 1",
		"//additem 2039901 1",
		"//additem 5000003 99",
		"//additem 5001003 99",
		"//addattr money=3000000 1",
		"//addattr bull=3000000 1"
	},
	{
		"//cmdpglv5",
		4,
		5,
		"//cmdpglv1",
		"//addattr level=4",
		"//updateskill 4002 1",
		"//updateskill 4020 1",
		"//additem 1509901 1",
		"//additem 2049901 1",
		"//additem 5000003 99",
		"//additem 5001003 99",
		"//addattr money=3000000 1",
		"//addattr bull=3000000 1"
	},
	{
		"//cmdpglv10",
		1,
		5,		-- 前置GM指令等级 玩家等级超过该等级则不执行cmdpglv5指令
		"//cmdpglv5",
		"//addattr level=5",
		"//updateskill 1010 2",
		"//updateskill 1018 2",
		"//updateskill 1012 1",
		"//updateskill 1026 1",
		"//additem 1019902 1"
	},
	{
		"//cmdpglv10",
		2,
		5,
		"//cmdpglv5",
		"//addattr level=5",
		"//updateskill 2010 2",
		"//updateskill 2020 2",
		"//updateskill 2028 2",
		"//updateskill 2016 1",
		"//updateskill 2030 1",
		"//additem 1109902 1"
	},
	{
		"//cmdpglv10",
		3,
		5,
		"//cmdpglv5",
		"//addattr level=5",
		"//updateskill 3010 2",
		"//updateskill 3018 2",
		"//updateskill 3024 1",
		"//additem 1209902 1"
	},
	{
		"//cmdpglv10",
		4,
		5,
		"//cmdpglv5",
		"//addattr level=5",
		"//updateskill 4010 2",
		"//updateskill 4002 2",
		"//updateskill 4006 1",
		"//additem 1509902 1"
	},
	{
		"//cmdpglv15",
		1,
		10,
		"//cmdpglv10",
		"//addattr level=5",
		"//additem 2013002 1",
		"//additem 2113002 1",
		"//additem 2413002 1",
		"//additem 1013002 1",
		"//additem 1063002 1",
		"//updateskill 1010 3",
		"//updateskill 1018 3",
		"//updateskill 1028 1",
		"//updateskill 1030 1"
	},
	{
		"//cmdpglv15",
		2,
		10,		-- 前置GM指令等级 玩家等级超过该等级则不执行cmdpglv10指令
		"//cmdpglv10",
		"//addattr level=5",
		"//additem 1103002 1",
		"//additem 2023002 1",
		"//additem 2123002 1",
		"//additem 2423002 1",
		"//updateskill 2010 3",
		"//updateskill 2020 3",
		"//updateskill 2028 3",
		"//updateskill 2022 1"
	},
	{
		"//cmdpglv15",
		3,
		10,
		"//cmdpglv10",
		"//additem 1253002 1",
		"//additem 2033002 1",
		"//additem 2133002 1",
		"//additem 2433002 1",
		"//addattr level=5",
		"//updateskill 3010 3",
		"//updateskill 3018 3",
		"//updateskill 3020 1",
		"//updateskill 3028 1"
	},
	{
		"//cmdpglv15",
		4,
		10,
		"//cmdpglv10",
		"//additem 1503002 1",
		"//additem 2043002 1",
		"//additem 2143002 1",
		"//additem 2443002 1",
		"//addattr level=5",
		"//updateskill 4002 3",
		"//updateskill 4000 3",
		"//updateskill 4006 2",
		"//updateskill 4016 1"
	},
	{
		"//cmdpglv20",
		1,
		15,
		"//cmdpglv15",
		"//additem 2013002 1",
		"//additem 2113002 1",
		"//additem 1019903 1",
		"//additem 2219903 1",
		"//additem 2019903 1",
		"//addattr level=5",
		"//updateskill 1010 4",
		"//updateskill 1022 2",
		"//updateskill 1018 4",
		"//updateskill 1016 1",
		"//updateskill 7000 1"
	},
	{
		"//cmdpglv20",
		2,
		15,
		"//cmdpglv15",
		"//additem 2023002 1",
		"//additem 2123002 1",
		"//additem 1109903 1",
		"//additem 2229903 1",
		"//additem 2029903 1",
		"//addattr level=5",
		"//updateskill 2010 4",
		"//updateskill 2020 4",
		"//updateskill 2028 4",
		"//updateskill 2014 1",
		"//updateskill 7001 1"
	},
	{
		"//cmdpglv20",
		3,
		15,
		"//cmdpglv15",
		"//additem 2033002 1",
		"//additem 2133002 1",
		"//additem 1209903 1",
		"//additem 1279903 1",
		"//additem 2039903 1",
		"//addattr level=5",
		"//updateskill 3010 4",
		"//updateskill 3018 4",
		"//updateskill 3012 1",
		"//updateskill 3016 1",
		"//updateskill 7002 1"
	},
	{
		"//cmdpglv20",
		4,
		15,
		"//cmdpglv15",
		"//additem 2043002 1",
		"//additem 2143002 1",
		"//additem 1509903 1",
		"//additem 2249903 1",
		"//additem 2049903 1",
		"//addattr level=5",
		"//updateskill 4000 4",
		"//updateskill 4002 4",
		"//updateskill 4006 3",
		"//updateskill 4018 1",
		"//updateskill 4022 1",
		"//updateskill 7003 1"
	},
	{
		"//cmdpglv30",
		1,
		20,
		"//cmdpglv20",
		"//additem 1019904 1",
		"//additem 2019904 1",
		"//additem 2119904 1",
		"//addattr level=10",
		"//updateskill 1010 5",
		"//updateskill 1012 2",
		"//updateskill 1014 2",
		"//updateskill 1016 3",
		"//updateskill 1018 5",
		"//updateskill 1026 2",
		"//updateskill 1030 2"
	},
	{
		"//cmdpglv30",
		2,
		20,
		"//cmdpglv20",
		"//additem 1109904 1",
		"//additem 2029904 1",
		"//additem 2129904 1",
		"//addattr level=10",
		"//updateskill 2010 5",
		"//updateskill 2012 2",
		"//updateskill 2014 3",
		"//updateskill 2020 5",
		"//updateskill 2028 5",
		"//updateskill 2030 2",
		"//updateskill 2032 1"
	},
	{
		"//cmdpglv30",
		3,
		20,
		"//cmdpglv20",
		"//additem 1209904 1",
		"//additem 2039904 1",
		"//additem 2139904 1",
		"//addattr level=10",
		"//updateskill 3010 5",
		"//updateskill 3012 3",
		"//updateskill 3016 2",
		"//updateskill 3024 2",
		"//updateskill 3018 5",
		"//updateskill 3026 2"
	},
	{
		"//cmdpglv30",
		4,
		20,
		"//cmdpglv20",
		"//additem 1509904 1",
		"//additem 2049904 1",
		"//additem 2149904 1",
		"//addattr level=10",
		"//updateskill 4000 5",
		"//updateskill 4002 5",
		"//updateskill 4004 2",
		"//updateskill 4018 3",
		"//updateskill 4020 2",
		"//updateskill 4022 3",
		"//updateskill 4024 2"
	},
	{
		"//cmdpglv40",
		1,
		30,
		"//cmdpglv30",
		"//additem 1019905 1",
		"//additem 2019905 1",
		"//additem 2119905 1",
		"//addattr level=10",
		"//updateskill 1010 6",
		"//updateskill 1022 3",
		"//updateskill 1014 4",
		"//updateskill 1016 5",
		"//updateskill 1018 6",
		"//updateskill 1024 1",
		"//updateskill 1028 3"
	},
	{
		"//cmdpglv40",
		2,
		30,
		"//cmdpglv30",
		"//additem 1109905 1",
		"//additem 2029905 1",
		"//additem 2129905 1",
		"//addattr level=10",
		"//updateskill 2010 6",
		"//updateskill 2012 4",
		"//updateskill 2014 5",
		"//updateskill 2020 6",
		"//updateskill 2022 3",
		"//updateskill 2026 1",
		"//updateskill 2028 6"
	},
	{
		"//cmdpglv40",
		3,
		30,		-- 前置GM指令等级 玩家等级超过该等级则不执行cmdpglv30指令
		"//cmdpglv30",
		"//additem 1209905 1",
		"//additem 2039905 1",
		"//additem 2139905 1",
		"//addattr level=10",
		"//updateskill 3010 6",
		"//updateskill 3012 5",
		"//updateskill 3014 1",
		"//updateskill 3016 4",
		"//updateskill 3018 6",
		"//updateskill 3020 3",
		"//updateskill 3022 1",
		"//updateskill 3026 4",
		"//updateskill 3028 3"
	},
	{
		"//cmdpglv40",
		4,
		30,
		"//cmdpglv30",
		"//additem 1509905 1",
		"//additem 2049905 1",
		"//additem 2149905 1",
		"//addattr level=10",
		"//updateskill 4000 6",
		"//updateskill 4002 6",
		"//updateskill 4004 4",
		"//updateskill 4014 1",
		"//updateskill 4016 3",
		"//updateskill 4018 4",
		"//updateskill 4022 4",
		"//updateskill 4024 4"
	},
	{
		"//cmdpglv50",
		1,
		40,
		"//cmdpglv40",
		"//additem 1019906 1",
		"//additem 2019906 1",
		"//additem 2119906 1",
		"//addattr level=10",
		"//updateskill 1010 7",
		"//updateskill 1014 6",
		"//updateskill 1016 7",
		"//updateskill 1018 7"
	},
	{
		"//cmdpglv50",
		2,
		40,
		"//cmdpglv40",
		"//additem 1109906 1",
		"//additem 2029906 1",
		"//additem 2129906 1",
		"//addattr level=10",
		"//updateskill 2010 7",
		"//updateskill 2012 6",
		"//updateskill 2014 7",
		"//updateskill 2020 7",
		"//updateskill 2028 7",
		"//updateskill 2026 2"
	},
	{
		"//cmdpglv50",
		3,
		40,
		"//cmdpglv40",
		"//additem 1209906 1",
		"//additem 2039906 1",
		"//additem 2139906 1",
		"//addattr level=10",
		"//updateskill 3010 7",
		"//updateskill 3012 7",
		"//updateskill 3014 2",
		"//updateskill 3016 6",
		"//updateskill 3022 2",
		"//updateskill 3018 7",
		"//updateskill 3026 6"
	},
	{
		"//cmdpglv50",
		4,
		40,
		"//cmdpglv40",
		"//additem 1509906 1",
		"//additem 2049906 1",
		"//additem 2149906 1",
		"//addattr level=10",
		"//updateskill 4000 7",
		"//updateskill 4002 7",
		"//updateskill 4004 6",
		"//updateskill 4014 2",
		"//updateskill 4018 6",
		"//updateskill 4022 6",
		"//updateskill 4024 6"
	},
	{
		"//cmdpglv60",
		1,
		50,
		"//cmdpglv50",
		"//additem 1019907 1",
		"//additem 2019907 1",
		"//additem 2119907 1",
		"//addattr level=10",
		"//updateskill 1010 8",
		"//updateskill 1012 3",
		"//updateskill 1014 8",
		"//updateskill 1016 8",
		"//updateskill 1018 8",
		"//updateskill 1024 2",
		"//updateskill 1026 3",
		"//updateskill 1030 3"
	},
	{
		"//cmdpglv60",
		2,
		50,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//cmdpglv50",
		"//additem 1109907 1",
		"//additem 2029907 1",
		"//additem 2129907 1",
		"//addattr level=10",
		"//updateskill 2010 8",
		"//updateskill 2012 8",
		"//updateskill 2014 8",
		"//updateskill 2016 2",
		"//updateskill 2020 8",
		"//updateskill 2026 3",
		"//updateskill 2028 8",
		"//updateskill 2030 3",
		"//updateskill 2032 2"
	},
	{
		"//cmdpglv60",
		3,
		50,		-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//cmdpglv50",
		"//additem 1209907 1",
		"//additem 2039907 1",
		"//additem 2139907 1",
		"//addattr level=10",
		"//updateskill 3010 8",
		"//updateskill 3012 8",
		"//updateskill 3014 3",
		"//updateskill 3016 8",
		"//updateskill 3018 8",
		"//updateskill 3022 3",
		"//updateskill 3024 3",
		"//updateskill 3026 8"
	},
	{
		"//cmdpglv60",
		4,
		50,			-- 前置GM指令等级 玩家等级超过该等级则不执行GM指令
		"//cmdpglv50",
		"//additem 1509907 1",
		"//additem 2049907 1",
		"//additem 2149907 1",
		"//addattr level=10",
		"//updateskill 4000 8",
		"//updateskill 4002 8",
		"//updateskill 4004 8",
		"//updateskill 4014 3",
		"//updateskill 4020 3",
		"//updateskill 4018 8",
		"//updateskill 4020 3",
		"//updateskill 4022 8",
		"//updateskill 4024 8"
	},
}

local function pack(...)
	return {...}
end

function InsterGMPackage( cmdpg )
	table.insert( cmdTable,cmdpg );
end

function CallGMPackage(cmd, limit)
	local mainPlayer = ActorMgr:getMainPlayer();
	local playerType = mainPlayer:getRace();

	for i = 1, table.getn(cmdTable) do
		local str = string.format("^%s$", cmdTable[i][1])
		local result = pack(string.find(cmd, str))
		if result[1] and (playerType == cmdTable[i][2] or cmdTable[i][2] == 0) and (not limit or mainPlayer:getLv() <= cmdTable[i][3]) then
			for j = 4, #cmdTable[i] do
				str = cmdTable[i][j]
				for k = 3, #result do
					str = string.gsub(str, "%%"..(k - 2), result[k])
				end
				if string.find(str, "^//cmdpg") then
					CallGMPackage(str, true)
				else
					SetGMExcCmd(str)
				end
			end
		end
	end
	SetGMCMDBeginExc( os.clock() );
>>>>>>> .r22464
end