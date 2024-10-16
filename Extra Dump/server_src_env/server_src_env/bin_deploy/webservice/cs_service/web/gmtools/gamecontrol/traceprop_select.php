<?php
require_once(dirname(__FILE__).'/../inc/config.inc.php');
//检测用户是否有权限
if($_SESSION['admin1'] == "")
{
	$_SESSION = array();
	session_destroy();	
	echo '<script type="text/javascript">window.top.location.href="login.php";</script>';
	exit();
}
if(isset($_POST['traceprop']))
{
	$nums = 0;
	$row[]="";
	$db1 = $_POST['xzdb'];
	$fg = $_POST['fag'];
	if($fg == 1){ $fg = "ID";}
	else if($fg == 2){ $fg = "WID";}
	$db="world_log_".$_POST['xzdb'];
	
	//选择服务器和数据库
	$sql = "select * from `reg_world` where `dbid`='$db1' ";
	$dosql->Execute($sql);
	$rowdb = $dosql->GetArray();
	
	$GLOBALS['db_host'] = $rowdb['dbip'];
	$GLOBALS['db_user'] = $rowdb['dbuser'];
	$GLOBALS['db_name'] = $db;
	$GLOBALS['db_pwd'] = $rowdb['dbpass'];
	
	$dosql->Close(true);
	$dosql = new MySql(false);
	
	$jy="LogRoleExchg_";
	$npc = "LogRoleShop_";
	$mail = "LogRoleMail_";
	$drop = "LogRoleItemDrop_";
	$die = "LogRoleDieDrop_";
	$stall = "LogRoleStallBuy_";
	$fen = "LogRoleArmMachining_";
	$Acc=$_POST['us'];
	//交易，NPC商店操作，邮件，丢弃，死亡掉落，玩家摆摊交易
	
	date_default_timezone_set('PRC');
	//提交过来的开始时间的int
	$statr_time=strtotime($_POST['time']);

	//获取开始数据表
	$statr_table=date('ymd',$statr_time);
		
	//交易
	$data_table1=$jy.$statr_table;
	//卖NPC
	$data_table2=$npc.$statr_table;
	//邮寄
	$data_table3=$mail.$statr_table;
	//丢弃
	$data_table4=$drop.$statr_table;
	//死亡掉落
	$data_table5=$die.$statr_table;
	//摆摊
	$data_table6=$stall.$statr_table;
	//装备加工(分解，强化...)
	$data_table7=$fen.$statr_table;
	$k=0;
	$j=0;
	$sqlw = "";
	if($dosql->IsTable($data_table1))
	{
		$dosql->ExecNoneQuery('SET NAMES Default');
		for($i=1; $i<9;$i++)
		{
			$fg = "ExchgOut_RoleGrid".$i."_WID";
			$sql = "select * from $data_table1 where `$fg` = $Acc ";
			$ret = mysql_query($sql);
			//$ret = $dosql->Execute($sql);
			$num = mysql_num_rows($ret);
			//echo $num."<br/>";
			if($num > 0)
			{
				$j = $i;
				$sqlw = $sql;
				break;
			}
		}
		if($sqlw != "")
		{
			$dosql->ExecNoneQuery('SET NAMES Default');
			$dopage->GetPage($sqlw,50,'UNIX_TIMESTAMP(LogTime) DESC');
			while($data = $dosql->GetArray())
			{
				//交易发起人
				$row[$k]['Uin'] = $data['Uin'];
				$row[$k]['LoginName'] = $data['LoginName'];
				$row[$k]['RoleName'] = iconv("gbk","utf8",$data['RoleName']);
				$row[$k]['oper'] = "交易给玩家";

				//交易出去(物品：唯一ID：交易物品数量：有效时间)
				$ax="";
				$ax = "被交易的玩家账号：".$data['ExchgObjLoginName']."，角色：".iconv("gbk","utf8",$data['ExchgObjName'])."<br/>物品：";
				$x = $data['ExchgOut_RoleGrid'.$j.'_DefID'];
				//交易的物品名
				$sql = "SELECT `Name` FROM ItemDef where `ItemID`='$x'";
				//echo $sql;
				mysql_select_db('resdb');
				$rest = mysql_query($sql);
				$rowf = mysql_fetch_array($rest);
				if($rowf != "" && $rowf != false)
				{
					$ax .= iconv("gbk","utf8",$rowf['Name']).'，';
				}
				else
				{
					$ax .= $x;
				}
				$dosql->select_db($db);
				//物品唯一ID
				$row[$k]['WID'] = $data['ExchgOut_RoleGrid'.$j.'_WID'];
				//交易物品数量
				$ax .= "数量：".$data['ExchgOut_RoleGrid'.$j.'_Num'].'，';
				//物品有效时间
				if($data['ExchgOut_RoleGrid'.$j.'_ValidEnd'] == 0)
				{
					$ax .= '有效期：永久';
				}
				else if($data['ExchgOut_RoleGrid'.$j.'_ValidEnd'] != 0)
				{
					$ax .= '有效期：'.date('Y-m-d H:i:s',$data['ExchgOut_RoleGrid'.$j.'_ValidEnd']);
				}
				$row[$k]['Items'] = $ax;
				
				$row[$k]['LogTime'] = $data['LogTime'];			
				$k++;
			}
		}
	}
	//npc
	if($dosql->IsTable($data_table2))
	{
		$dosql->ExecNoneQuery('SET NAMES Default');
		$fg = "GoodsData_GoodsWid";
		$sqls = "select * from $data_table2 where `$fg` = $Acc ";
		$dopage->GetPage($sqls,50,'UNIX_TIMESTAMP(LogTime) DESC');
		while($data = $dosql->GetArray())
		{
			$row[$k]['Uin'] = $data['Uin'];
			$row[$k]['LoginName'] = $data['LoginName'];
			$row[$k]['RoleName'] = iconv("gbk","utf8",$data['RoleName']);
			$row[$k]['oper'] = $data['ShopAct'];
			if($data['ShopAct'] == 0)
			{
				$row[$k]['oper'] = "购买";
			}
			if($data['ShopAct'] == 1)
			{
				$row[$k]['oper'] = "购买卖出的物品";
			}
			if($data['ShopAct'] == 2)
			{
				$row[$k]['oper'] = "卖出";
			}
			//物品唯一ID
			$row[$k]['WID'] = $data['GoodsData_GoodsWid'];

			//出售(物品：唯一ID：交易物品数量：有效时间)
			$ax="";
			$x = $data['GoodsData_GoodsId'];
			//交易的物品名
			$sql = "SELECT `Name` FROM ItemDef where `ItemID`='$x'";
			//echo $sql;
			mysql_select_db('resdb');
			$rest = mysql_query($sql);
			$rowf = mysql_fetch_array($rest);
			if($rowf != "" && $rowf != false)
			{
				$ax .= "物品：".iconv("gbk","utf8",$rowf['Name']).'，';
			}
			else
			{
				$ax .= "物品ID：".$x;
			}
			$dosql->select_db($db);
			//交易物品数量
			$ax .= "数量：".$data['OldShopData_GoodsNum'];
			$row[$k]['Items'] = $ax;
			
			$row[$k]['LogTime'] = $data['LogTime'];			
			$k++;
		}
	}
	//mail
	if($dosql->IsTable($data_table3))
	{
		$dosql->ExecNoneQuery('SET NAMES Default');
		for($i=1; $i<9;$i++)
		{
			$fg = "RoleMailData_Items".$i."_WID";
			$sql = "select * from $data_table3 where `$fg` = $Acc ";
			$ret = mysql_query($sql);
			//$ret = $dosql->Execute($sql);
			$num = mysql_num_rows($ret);
			//echo $num."<br/>";
			if($num > 0)
			{
				$j = $i;
				$sqlm = $sql;
				break;
			}
		}
		if($sqlm != "")
		{
			$dosql->ExecNoneQuery('SET NAMES Default');
			$dopage->GetPage($sqlm,50,'UNIX_TIMESTAMP(LogTime) DESC');
			while($data = $dosql->GetArray())
			{
				$row[$k]['Uin'] = $data['Uin'];
				$row[$k]['LoginName'] = $data['LoginName'];
				$row[$k]['RoleName'] = iconv("gbk","utf8",$data['RoleName']);
				$row[$k]['oper'] = "邮寄";

				//交易出去(物品：唯一ID：交易物品数量：有效时间)
				$ax="";
				$ax = "收件人：".iconv("gbk","utf8",$data['RoleMailData_To'])."，物品：";
				$x = $data['RoleMailData_Items'.$j.'_DefID'];
				//交易的物品名
				$sql = "SELECT `Name` FROM ItemDef where `ItemID`='$x'";
				//echo $sql;
				mysql_select_db('resdb');
				$rest = mysql_query($sql);
				$rowf = mysql_fetch_array($rest);
				if($rowf != "" && $rowf != false)
				{
					$ax .= iconv("gbk","utf8",$rowf['Name']).'<br/>';
				}
				else
				{
					$ax .= $x.'<br/>';
				}
				$dosql->select_db($db);
				//物品唯一ID
				$row[$k]['WID'] = $data['RoleMailData_Items'.$j.'_WID'];
				//交易物品数量
				$ax .= "数量：".$data['RoleMailData_Items'.$j.'_Num'].'，';
				//物品有效时间
				if($data['RoleMailData_Items'.$j.'_ValidEnd'] == 0)
				{
					$ax .= '有效期：永久';
				}
				else if($data['RoleMailData_Items'.$j.'_ValidEnd'] != 0)
				{
					$ax .= '有效期：'.date('Y-m-d H:i:s',$data['RoleMailData_Items'.$j.'_ValidEnd']);
				}
				$row[$k]['Items'] = $ax;
				
				$row[$k]['LogTime'] = $data['LogTime'];			
				$k++;
			}
		}
	}
	//drop
	if($dosql->IsTable($data_table4))
	{
		$dosql->ExecNoneQuery('SET NAMES Default');
		$fg = "GridData_WID";
		$sqld = "select * from $data_table4 where `$fg` = $Acc ";
		$dopage->GetPage($sqld,50,'UNIX_TIMESTAMP(LogTime) DESC');
		while($data = $dosql->GetArray())
		{
			$row[$k]['Uin'] = $data['Uin'];
			$row[$k]['LoginName'] = $data['Pass9Account'];
			$row[$k]['RoleName'] = iconv("gbk","utf8",$data['RoleInfo_RoleName']);
			$row[$k]['oper'] = "丢弃";
			//物品唯一ID
			$row[$k]['WID'] = $data['GridData_WID'];

			//出售(物品：唯一ID：交易物品数量：有效时间)
			$ax="";
			$x = $data['GridData_DefID'];
			//交易的物品名
			$sql = "SELECT `Name` FROM ItemDef where `ItemID`='$x'";
			//echo $sql;
			mysql_select_db('resdb');
			$rest = mysql_query($sql);
			$rowf = mysql_fetch_array($rest);
			if($rowf != "" && $rowf != false)
			{
				$ax .= "物品：".iconv("gbk","utf8",$rowf['Name']).'，';
			}
			else
			{
				$ax .= "物品ID：".$x;
			}
			$dosql->select_db($db);
			//交易物品数量
			$ax .= "数量：".$data['GridData_Num'];
			$row[$k]['Items'] = $ax;
			
			$row[$k]['LogTime'] = $data['LogTime'];			
			$k++;
		}
	}
	//die
	if($dosql->IsTable($data_table5))
	{
		$dosql->ExecNoneQuery('SET NAMES Default');
		for($i=1; $i<3;$i++)
		{
			$fg = "DropItem".$i."_WID";
			$sql = "select * from $data_table5 where `$fg` = $Acc ";
			$ret = mysql_query($sql);
			//$ret = $dosql->Execute($sql);
			$num = mysql_num_rows($ret);
			//echo $num."<br/>";
			if($num > 0)
			{
				$j = $i;
				$sqldie = $sql;
				break;
			}
		}
		if($sqldie != "")
		{
			$dosql->ExecNoneQuery('SET NAMES Default');
			$dopage->GetPage($sqldie,50,'UNIX_TIMESTAMP(LogTime) DESC');
			while($data = $dosql->GetArray())
			{
				//交易发起人
				$row[$k]['Uin'] = $data['Uin'];
				$row[$k]['LoginName'] = $data['LoginName'];
				$row[$k]['RoleName'] = iconv("gbk","utf8",$data['RoleInfo_RoleName']);
				$row[$k]['oper'] = "死亡掉落";

				//交易出去(物品：唯一ID：交易物品数量：有效时间)
				$ax="";
				$ax = "掉落物品：";
				$x = $data['DropItem'.$j.'_DefID'];
				//交易的物品名
				$sql = "SELECT `Name` FROM ItemDef where `ItemID`='$x'";
				//echo $sql;
				mysql_select_db('resdb');
				$rest = mysql_query($sql);
				$rowf = mysql_fetch_array($rest);
				if($rowf != "" && $rowf != false)
				{
					$ax .= iconv("gbk","utf8",$rowf['Name']).'，';
				}
				else
				{
					$ax .= $x;
				}
				$dosql->select_db($db);
				//物品唯一ID
				$row[$k]['WID'] = $data['DropItem'.$j.'_WID'];
				//交易物品数量
				$ax .= "数量：".$data['DropItem'.$j.'_Num'].'，';
				//物品有效时间
				if($data['DropItem'.$j.'_ValidEnd'] == 0)
				{
					$ax .= '有效期：永久';
				}
				else if($data['DropItem'.$j.'_ValidEnd'] != 0)
				{
					$ax .= '有效期：'.date('Y-m-d H:i:s',$data['DropItem'.$j.'_ValidEnd']);
				}
				$row[$k]['Items'] = $ax;
				
				$row[$k]['LogTime'] = $data['LogTime'];			
				$k++;
			}
		}
	}
	//stall
	if($dosql->IsTable($data_table6))
	{
		$dosql->ExecNoneQuery('SET NAMES Default');
		$fg = "RoleGrid_WID";
		$sqls = "select * from $data_table6 where `$fg` = $Acc ";
		//echo $sqls;
		$dopage->GetPage($sqls,50,'UNIX_TIMESTAMP(LogTime) DESC');
		while($data = $dosql->GetArray())
		{
			$row[$k]['Uin'] = $data['Uin'];  //购买玩家UIN
			$row[$k]['LoginName'] = $data['BuyLoginName'];  //购买玩家帐号
			$row[$k]['RoleName'] = iconv("gbk","utf8",$data['RoleName']);
			$row[$k]['oper'] = "购买摆摊物品";
			//物品唯一ID
			$row[$k]['WID'] = $data['RoleGrid_WID'];

			//出售(物品：唯一ID：交易物品数量：有效时间)
			$ax="";
			$x = $data['RoleGrid_DefID'];
			//交易的物品名
			$sql = "SELECT `Name` FROM ItemDef where `ItemID`='$x'";
			//echo $sql;
			mysql_select_db('resdb');
			$rest = mysql_query($sql);
			$rowf = mysql_fetch_array($rest);
			if($rowf != "" && $rowf != false)
			{
				$ax .= "摊主Uin：".$data['StallUin'].",帐号：".$data['OwnerLoginName'].",角色名：".iconv("gbk","utf8",$data['StallOwner'])."<br/>物品：".iconv("gbk","utf8",$rowf['Name']).'，';
			}
			else
			{
				$ax .= "物品ID：".$x;
			}
			$dosql->select_db($db);
			//交易物品数量
			$ax .= "数量：".$data['RoleGrid_Num'];
			$row[$k]['Items'] = $ax;
			
			$row[$k]['LogTime'] = $data['LogTime'];			
			$k++;
		}
	}
	//装备分解
	if($dosql->IsTable($data_table7))
	{
		$dosql->ExecNoneQuery('SET NAMES Default');
		$fg = "ArmData_OldRoleArm_WID";
		$sqls = "select * from $data_table7 where `$fg` = $Acc ";
		//echo $sqls;
		$dopage->GetPage($sqls,50,'UNIX_TIMESTAMP(LogTime) DESC');
		while($data = $dosql->GetArray())
		{
			$row[$k]['Uin'] = $data['Uin'];  //操作玩家UIN
			$row[$k]['LoginName'] = $data['LoginName'];  //操作玩家帐号
			$row[$k]['RoleName'] = iconv("gbk","utf8",$data['RoleName']);
			$row[$k]['oper'] = "装备加工"; //MachiningActType
			if($data['MachiningActType'] == 0)
			{
				$row[$k]['oper'] = "装备品质提升";
			}
			if($data['MachiningActType'] == 1)
			{
				$row[$k]['oper'] = "装备等级提升";
			}
			if($data['MachiningActType'] == 2)
			{
				$row[$k]['oper'] = "装备战斗力提升";
			}
			if($data['MachiningActType'] == 3)
			{
				$row[$k]['oper'] = "装备鉴定";
			}
			if($data['MachiningActType'] == 4)
			{
				$row[$k]['oper'] = "宝石升级";
			}
			if($data['MachiningActType'] == 5)
			{
				$row[$k]['oper'] = "宝石镶嵌";
			}
			if($data['MachiningActType'] == 6)
			{
				$row[$k]['oper'] = "宝石摘取";
			}
			if($data['MachiningActType'] == 7)
			{
				$row[$k]['oper'] = "装备开孔";
			}
			if($data['MachiningActType'] == 8)
			{
				$row[$k]['oper'] = "装备品质转移";
			}
			if($data['MachiningActType'] == 9)
			{
				$row[$k]['oper'] = "宝石升级";
			}
			if($data['MachiningActType'] == 10)
			{
				$row[$k]['oper'] = "装备绑定";
			}
			if($data['MachiningActType'] == 11)
			{
				$row[$k]['oper'] = "装备附魔";
			}
			if($data['MachiningActType'] == 12)
			{
				$row[$k]['oper'] = "装备刻印";
			}
			if($data['MachiningActType'] == 13)
			{
				$row[$k]['oper'] = "装备分解";
			}
			if($data['MachiningActType'] == 14)
			{
				$row[$k]['oper'] = "宝石分解";
			}
			if($data['MachiningActType'] == 15)
			{
				$row[$k]['oper'] = "宝石重置";
			}
			if($data['MachiningActType'] == 16)
			{
				$row[$k]['oper'] = "宝石充能";
			}
			if($data['MachiningActType'] == 17)
			{
				$row[$k]['oper'] = "装备重铸消耗";
			}
			if($data['MachiningActType'] == 18)
			{
				$row[$k]['oper'] = "装备重铸信息保存";
			}
			if($data['MachiningActType'] == 19)
			{
				$row[$k]['oper'] = "装备品质提升保存(星级)";
			}
			if($data['MachiningActType'] == 20)
			{
				$row[$k]['oper'] = "装备洗练";
			}
			if($data['MachiningActType'] == 21)
			{
				$row[$k]['oper'] = "装备洗练保存";
			}
			if($data['MachiningActType'] == 22)
			{
				$row[$k]['oper'] = "能量晶石吸收";
			}
			//物品唯一ID
			$row[$k]['WID'] = $data['ArmData_OldRoleArm_WID'];

			//出售(物品：唯一ID：交易物品数量：有效时间)
			$ax="";
			$row[$k]['Items'] = "无";
			
			$row[$k]['LogTime'] = $data['LogTime'];			
			$k++;
		}
	}
	//$row['nums'] = $nums;
	echo json_encode($row);
}
?>