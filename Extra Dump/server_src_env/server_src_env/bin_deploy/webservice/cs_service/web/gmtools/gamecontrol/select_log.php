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
if(isset($_POST['daqu']))
{
	$daqu=$_POST['daqu'];
	$dosql->Execute("select worldid,worldname from `reg_world` where `regid`='$daqu' ");

	$data;
	while($row = $dosql->GetArray())
	{
		$data[] = $row;
	}
	echo json_encode($data);
}
//角色/角色列表登录登出记录
if(isset($_POST['rolelog']))
{
	$nums = 0;
	$row[]="";
	$db1 = $_POST['ldb'];
	$fg = $_POST['fag'];
	if($fg == 1){ $fg = "RoleName";}
	else if($fg == 2){ $fg = "AccountLoginName";}
	$db="world_log_".$_POST['ldb'];
	
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
	
	$qz="LogRoleLogin_";
	$qz1="LogRoleLogout_";
	$Acc=$_POST['acc'];
	$Acc = iconv("utf8","gbk",$Acc);
	
	date_default_timezone_set('PRC');
	//提交过来的开始时间的int
	$statr_time=strtotime($_POST['time']);

	//获取开始数据表
	$statr_table=date('ymd',$statr_time);
	
	//提交过来的结束时间
	$end_time=strtotime($_POST['time1']);
	
	//获取结束数据表
	$end_table=date('ymd',$end_time);
	
	//未填写结束时间
	if("" == $end_time || '0' == $end_time)
	{
		$data_table=$qz.$statr_table;
		$data_table1=$qz1.$statr_table;
		if($dosql->IsTable($data_table))
		{
			$sql = "select * from $data_table where `$fg` like '%$Acc%' ";
			
			$dosql->ExecNoneQuery('SET NAMES Default');
			$dopage->GetPage($sql,50,'UNIX_TIMESTAMP(LogTime) DESC');
			while($data = $dosql->GetArray())
			{
				$data['RoleName'] = iconv("gbk","utf8",$data['RoleName']);
				$row1[] = $data;
			}
			$row['row1'] = $row1;
			$row['nums1'] = count($row1);
			$nums = count($row1);
		}
		if($dosql->IsTable($data_table1))
		{
			$sql1 = "select * from $data_table1 where `$fg` like '%$Acc%' ";
			
			$dosql->ExecNoneQuery('SET NAMES Default');
			$dopage->GetPage($sql1,50,'UNIX_TIMESTAMP(LogTime) DESC');
			while($data = $dosql->GetArray())
			{
				$data['RoleName'] = iconv("gbk","utf8",$data['RoleName']);
				$row2[] = $data;
			}
			$row['row2'] = $row2;
			$row['nums2'] = count($row2);
			$nums += count($row2);
		}
		else if(!$dosql->IsTable($data_table))
		{
			echo json_encode("不存在数据表");
			exit;
		}
	}
	else
	{
		$fate=$end_table-$statr_table;
		$fag = $end_time-$statr_time;
		if("0"==$fate && $fag>=0)
		{
			$data_table=$qz.$statr_table;
			$data_table1=$qz1.$statr_table;
			if($dosql->IsTable($data_table))
			{
			
				$sql = "select *  from $data_table where `$fg` like '%$Acc%'  and UNIX_TIMESTAMP(LogTime) >'$statr_time' and UNIX_TIMESTAMP(LogTime)<'$end_time' ";
				$dosql->ExecNoneQuery('SET NAMES Default');
				$dopage->GetPage($sql,50,'UNIX_TIMESTAMP(LogTime) DESC');
				while($data = $dosql->GetArray())
				{
					$data['RoleName'] = iconv("gbk","utf8",$data['RoleName']);
					$row1[] = $data;
				}
				$row['row1'] = $row1;
				$row['nums1'] = count($row1);
				$nums = count($row1);
			
				$sql = "select * from $data_table1 where `$fg` like '%$Acc%' and UNIX_TIMESTAMP(LogTime) >'$statr_time' and UNIX_TIMESTAMP(LogTime)<'$end_time' ";
				$dosql->ExecNoneQuery('SET NAMES Default');
				$dopage->GetPage($sql,50,'UNIX_TIMESTAMP(LogTime) DESC');
				while($data = $dosql->GetArray())
				{
					$data['RoleName'] = iconv("gbk","utf8",$data['RoleName']);
					$row2[] = $data;
				}
				$row['row2'] = $row2;
				$row['nums2'] = count($row2);
				$nums += count($row2);
			
			}
			else
			{
				echo json_encode("不存在数据表");
				exit;
			}
		}
		if(0< $fate && $fate < 3)
		{
			for($i=0;$i<=$fate;$i++)
			{
				$data_table[$i]=$qz.($statr_table+$i);
				if($dosql->IsTable($data_table[$i]))
				{
				
					if($i<$fate)
					{
						$sql = "select * from $data_table[$i] where `$fg` like '%$Acc%'  and UNIX_TIMESTAMP(LogTime) >'$statr_time' ";
					}
					if($i == $fate)
					{
						$sql = "select * from $data_table[$i] where `$fg` like '%$Acc%'  and UNIX_TIMESTAMP(LogTime) < '$end_time' ";
					}
					$dosql->ExecNoneQuery('SET NAMES Default');
					$dopage->GetPage($sql,50,'UNIX_TIMESTAMP(LogTime) DESC');
					while($data = $dosql->GetArray())
					{
						$data['RoleName'] = iconv("gbk","utf8",$data['RoleName']);
						$row1[] = $data;
					}
				}
				else
				{
					continue;
				}
			}
			$row['row1'] = $row1;
			$row['nums1'] = count($row1);
			$nums = count($row1);
			for($i=0;$i<=$fate;$i++)
			{
				$data_table1[$i]=$qz1.($statr_table+$i);
				if($dosql->IsTable($data_table1[$i]))
				{
					if($i<$fate)
					{

						$sql = "select * from $data_table1[$i] where `$fg` like '%$Acc%' and UNIX_TIMESTAMP(LogTime) >'$statr_time' ";
					}
					if($i==$fate)
					{
						$sql = "select * from $data_table1[$i] where `$fg` like '%$Acc%'  and UNIX_TIMESTAMP(LogTime)<'$end_time' ";
					}
					$dosql->ExecNoneQuery('SET NAMES Default');
					$dopage->GetPage($sql,50,'UNIX_TIMESTAMP(LogTime) DESC');
					while($data = $dosql->GetArray())
					{
						$data['RoleName'] = iconv("gbk","utf8",$data['RoleName']);
						$row2[] = $data;
					}
					contiue;
				}
				else
				{
					continue;
				}
			}
			$row['row2'] = $row2;
			$row['nums2'] = count($row2);
			$nums += count($row2);
		}
		if( $fate >= 3){
			echo json_encode("结束时间与开始时间相差不能超过3天");
			exit;
		}
		if(0>$fate)
		{
			echo json_encode("结束时间不能低于开始时间");
			exit;
		}
		if(0==$fate && $fag<0)
		{
			echo json_encode("结束时间不能低于开始时间");
			exit;
		}
	}
	$row['nums'] = $nums;
	echo json_encode($row);
}
//查询账号登录登出记录
if(isset($_POST['acclog']))
{
	$nums = 0;
	$row[]="";
	$db1 = $_POST['ldb'];
	$db="world_log_".$_POST['ldb'];
	
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
	
	$qz="LogAccLogin_";
	$qz1="LogAccLogout_";
	$Acc=$_POST['acc'];
	$Acc = iconv("utf8","gbk",$Acc);
	
	date_default_timezone_set('PRC');
	//提交过来的开始时间的int
	$statr_time=strtotime($_POST['time']);

	//获取开始数据表
	$statr_table=date('ymd',$statr_time);
	
	//提交过来的结束时间
	$end_time=strtotime($_POST['time1']);
	
	//获取结束数据表
	$end_table=date('ymd',$end_time);
	
	//未填写结束时间
	if("" == $end_time || '0' == $end_time)
	{
		$data_table=$qz.$statr_table;
		$data_table1=$qz1.$statr_table;
		if($dosql->IsTable($data_table))
		{
			$sql = "select * from $data_table where `AccountLoginName` like '%$Acc%' ";
			
			$dosql->ExecNoneQuery('SET NAMES Default');
			$dopage->GetPage($sql,50,'UNIX_TIMESTAMP(LogTime) DESC');
			while($data = $dosql->GetArray())
			{
				$row1[] = $data;
			}
			$row['row1'] = $row1;
			$row['nums1'] = count($row1);
			$nums = count($row1);
		}
		if($dosql->IsTable($data_table1))
		{
			$sql1 = "select * from $data_table1 where `AccountLoginName` like '%$Acc%' ";
			
			$dosql->ExecNoneQuery('SET NAMES Default');
			$dopage->GetPage($sql1,50,'UNIX_TIMESTAMP(LogTime) DESC');
			while($data = $dosql->GetArray())
			{
				$row2[] = $data;
			}
			$row['row2'] = $row2;
			$row['nums2'] = count($row2);
			$nums += count($row2);
		}
		else if(!$dosql->IsTable($data_table))
		{
			echo json_encode("不存在数据表");
			exit;
		}
	}
	else
	{
		$fate=$end_table-$statr_table;
		$fag = $end_time-$statr_time;
		if("0"==$fate && $fag>=0)
		{
			$data_table=$qz.$statr_table;
			$data_table1=$qz1.$statr_table;
			if($dosql->IsTable($data_table))
			{
			
				$sql = "select *  from $data_table where `AccountLoginName` like '%$Acc%'  and UNIX_TIMESTAMP(LogTime) >'$statr_time' and UNIX_TIMESTAMP(LogTime)<'$end_time' ";
				$dosql->ExecNoneQuery('SET NAMES Default');
				$dopage->GetPage($sql,50,'UNIX_TIMESTAMP(LogTime) DESC');
				while($data = $dosql->GetArray())
				{
					$row1[] = $data;
				}
				$row['row1'] = $row1;
				$row['nums1'] = count($row1);
				$nums = count($row1);
			
				$sql = "select * from $data_table1 where `AccountLoginName` like '%$Acc%' and UNIX_TIMESTAMP(LogTime) >'$statr_time' and UNIX_TIMESTAMP(LogTime)<'$end_time' ";
				$dosql->ExecNoneQuery('SET NAMES Default');
				$dopage->GetPage($sql,50,'UNIX_TIMESTAMP(LogTime) DESC');
				while($data = $dosql->GetArray())
				{
					$row2[] = $data;
				}
				$row['row2'] = $row2;
				$row['nums2'] = count($row2);
				$nums += count($row2);
			
			}
			else
			{
				echo json_encode("不存在数据表");
				exit;
			}
		}
		if(0< $fate && $fate < 3)
		{
			for($i=0;$i<=$fate;$i++)
			{
				$data_table[$i]=$qz.($statr_table+$i);
				if($dosql->IsTable($data_table[$i]))
				{
				
					if($i<$fate)
					{
						$sql = "select * from $data_table[$i] where `AccountLoginName` like '%$Acc%'  and UNIX_TIMESTAMP(LogTime) >'$statr_time' ";
					}
					if($i == $fate)
					{
						$sql = "select * from $data_table[$i] where `AccountLoginName` like '%$Acc%'  and UNIX_TIMESTAMP(LogTime) < '$end_time' ";
					}
					$dosql->ExecNoneQuery('SET NAMES Default');
					$dopage->GetPage($sql,50,'UNIX_TIMESTAMP(LogTime) DESC');
					while($data = $dosql->GetArray())
					{
						$row1[] = $data;
					}
				
				}
				else
				{
					continue;
				}
			}
			$row['row1'] = $row1;
			$row['nums1'] = count($row1);
			$nums = count($row1);
			for($i=0;$i<=$fate;$i++)
			{
				$data_table1[$i]=$qz1.($statr_table+$i);
				if($dosql->IsTable($data_table1[$i]))
				{
					if($i<$fate)
					{
						$sql = "select * from $data_table1[$i] where `AccountLoginName` like '%$Acc%' and UNIX_TIMESTAMP(LogTime) >'$statr_time' ";
					}
					if($i==$fate)
					{
						$sql = "select * from $data_table1[$i] where `AccountLoginName` like '%$Acc%'  and UNIX_TIMESTAMP(LogTime)<'$end_time' ";
					}
					$dosql->ExecNoneQuery('SET NAMES Default');
					$dopage->GetPage($sql,50,'UNIX_TIMESTAMP(LogTime) DESC');
					while($data = $dosql->GetArray())
					{
						$row2[] = $data;
					}
				}
				else
				{
					continue;
				}
			}
			$row['row2'] = $row2;
			$row['nums2'] = count($row2);
			$nums += count($row2);
		}
		if( $fate >= 3){
			echo json_encode("结束时间与开始时间相差不能超过3天");
			exit;
		}
		if(0>$fate)
		{
			echo json_encode("结束时间不能低于开始时间");
			exit;
		}
		if(0==$fate && $fag<0)
		{
			echo json_encode("结束时间不能低于开始时间");
			exit;
		}
	}
	$row['nums'] = $nums;
	echo json_encode($row);
}
//通过IP查询账号
if(isset($_POST['acclogIP']))
{
	$nums = 0;
	$row[]="";
	$db1 = $_POST['ldb'];
	$db="world_log_".$_POST['ldb'];
	
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
	
	$qz="LogAccLogin_";
	$qz1="LogAccLogout_";
	$ip=$_POST['ip'];
	
	date_default_timezone_set('PRC');
	//提交过来的开始时间的int
	$statr_time=strtotime($_POST['time']);

	//获取开始数据表
	$statr_table=date('ymd',$statr_time);
	
	//提交过来的结束时间
	$end_time=strtotime($_POST['time1']);
	
	//获取结束数据表
	$end_table=date('ymd',$end_time);
	
	//未填写结束时间
	if("" == $end_time || '0' == $end_time)
	{
		$data_table=$qz.$statr_table;
		$data_table1=$qz1.$statr_table;
		if($dosql->IsTable($data_table))
		{
			$sql = "select * from $data_table where `IP` = '$ip' ";			
			$dosql->ExecNoneQuery('SET NAMES Default');
			$dopage->GetPage($sql,50,'UNIX_TIMESTAMP(LogTime) DESC');
			while($data = $dosql->GetArray())
			{
				$row1[] = $data;
			}
			$row['row1'] = $row1;
			$row['nums1'] = count($row1);
			$nums = count($row1);			
		}	
		else if(!$dosql->IsTable($data_table))
		{
			echo json_encode("不存在数据表");
			exit;
		}
	}
	else
	{
		$fate=$end_table-$statr_table;
		$fag = $end_time-$statr_time;
		if("0"==$fate && $fag>=0)
		{
			$data_table=$qz.$statr_table;
			$data_table1=$qz1.$statr_table;
			if($dosql->IsTable($data_table))
			{
			
				$sql = "select *  from $data_table where `IP` = '$ip'  and UNIX_TIMESTAMP(LogTime) >'$statr_time' and UNIX_TIMESTAMP(LogTime)<'$end_time' ";
				$dosql->ExecNoneQuery('SET NAMES Default');
				$dopage->GetPage($sql,50,'UNIX_TIMESTAMP(LogTime) DESC');
				while($data = $dosql->GetArray())
				{
					$row1[] = $data;
				}
				$row['row1'] = $row1;
				$row['nums1'] = count($row1);
				$nums = count($row1);			
			}
			else
			{
				echo json_encode("不存在数据表");
				exit;
			}
		}
		if(0< $fate && $fate < 7)
		{
			for($i=0;$i<=$fate;$i++)
			{
				$data_table[$i]=$qz.($statr_table+$i);
				if($dosql->IsTable($data_table[$i]))
				{
				
					if($i<$fate)
					{
						$sql = "select * from $data_table[$i] where `IP` = '$ip'  and UNIX_TIMESTAMP(LogTime) >'$statr_time' ";
					}
					if($i == $fate)
					{
						$sql = "select * from $data_table[$i] where `IP` = '$ip'  and UNIX_TIMESTAMP(LogTime) < '$end_time' ";
					}
					$dosql->ExecNoneQuery('SET NAMES Default');
					$dopage->GetPage($sql,50,'UNIX_TIMESTAMP(LogTime) DESC');
					while($data = $dosql->GetArray())
					{
						$row1[] = $data;
					}
				
				}
				else
				{
					continue;
				}
			}
			$row['row1'] = $row1;
			$row['nums1'] = count($row1);
			$nums = count($row1);
		}
		if( $fate >= 7){
			echo json_encode("结束时间与开始时间相差不能超过7天");
			exit;
		}
		if(0>$fate)
		{
			echo json_encode("结束时间不能低于开始时间");
			exit;
		}
		if(0==$fate && $fag<0)
		{
			echo json_encode("结束时间不能低于开始时间");
			exit;
		}
	}
	$row['nums'] = $nums;
	echo json_encode($row);
}
//查询gm日志和聊天日志
if(isset($_POST['ldb']))
{
	$db1 = $_POST['ldb'];
	$db="world_log_".$_POST['ldb'];
	$qz=$_POST['tab'];
	$role=$_POST['us'];
	$role = iconv("utf8","gbk",$role);
	$key=$_POST['key'];
	$key = iconv("utf8","gbk",$key);

	
	date_default_timezone_set('PRC');
	//提交过来的开始时间的int
	$statr_time=strtotime($_POST['time']);

	//获取开始数据表
	$statr_table=date('ymd',$statr_time);
	
	//提交过来的结束时间
	$end_time=strtotime($_POST['time1']);
	
	//获取结束数据表
	$end_table=date('ymd',$end_time);
	
	//获取服务器的IP,端口号,账户,密码信息
	$sql = "select * from `reg_world` where `dbid`='$db1' ";
	$dosql->Execute($sql);
	$row1 = $dosql->GetArray();
	
	$GLOBALS['db_host'] = $row1['dbip'];
	$GLOBALS['db_user'] = $row1['dbuser'];
	$GLOBALS['db_name'] = $db;
	$GLOBALS['db_pwd'] = $row1['dbpass'];
	
	$dosql->Close(true);
	$dosql = new MySql(false);
	
	//未填写结束时间
	if("" == $end_time || '0' == $end_time)
	{
		$data_table=$qz.$statr_table;
		//$dosql->ExecNoneQuery("SET character_set_connection=latin1,character_set_results=latin1,character_set_client=binary");
		if($dosql->IsTable($data_table))
		{
			if("LogRoleChat_"==$qz)
			{
				//if(1){UNIX_TIMESTAMP();}
				$sql = "select * from $data_table where `RoleName` like '%$role%' and `Content` like '%$key%' ";
				
				$dosql->ExecNoneQuery('SET NAMES Default');
				$dopage->GetPage($sql,50,'UNIX_TIMESTAMP(LogTime) DESC');
				$k = 0;
				while($data = $dosql->GetArray())
				{
					$row[$k] = $data;
					$row[$k]['Content'] = iconv("gbk","utf8",$row[$k]['Content']);
					$row[$k]['RoleName'] = iconv("gbk","utf8",$row[$k]['RoleName']);
					$row[$k]['ToName']= iconv("gbk","utf8",$row[$k]['ToName']);
					$k++;
				}
			}
			else if("LogRoleGmCmd_"==$qz)
			{
				$sql = "select * from $data_table where `RoleName` like '%$role%' and `CmdLine` like '%$key%' ";
				$dopage->GetPage($sql,50,'UNIX_TIMESTAMP(LogTime) DESC');
				$dosql->ExecNoneQuery('SET NAMES utf8');
				while($data = $dosql->GetArray())
				{
					$row[] = $data;
				}
			}
		}
		else
		{
			echo json_encode("不存在数据表");
			exit;
		}
	}
	else
	{
		$fate=$end_table-$statr_table;
		$fag = $end_time-$statr_time;
		if("0"==$fate && $fag>=0)
		{
			$data_table=$qz.$statr_table;
			if($dosql->IsTable($data_table))
			{
				if("LogRoleChat_"==$qz)
				{
					$sql = "select *  from $data_table where `RoleName` like '%$role%' and `Content` like '%$key%' and UNIX_TIMESTAMP(LogTime) >'$statr_time' and UNIX_TIMESTAMP(LogTime)<'$end_time' ";
					$dosql->ExecNoneQuery('SET NAMES Default');
					$dopage->GetPage($sql,50,'UNIX_TIMESTAMP(LogTime) DESC');
					$k = 0;
					while($data = $dosql->GetArray())
					{
						$row[$k] = $data;
						$row[$k]['Content'] = iconv("gbk","utf8",$row['Content']);
						$row[$k]['RoleName'] = iconv("gbk","utf8",$row['RoleName']);
						$row[$k]['ToName']= iconv("gbk","utf8",$row['ToName']);
						$k++;
					}
				}
				else if("LogRoleGmCmd_"==$qz)
				{
					$sql = "select * from $data_table where `RoleName` like '%$role%' and `CmdLine` like '%$key%' and UNIX_TIMESTAMP(LogTime) >'$statr_time' and UNIX_TIMESTAMP(LogTime)<'$end_time' ";
					$dosql->ExecNoneQuery('SET NAMES Default');
					$dopage->GetPage($sql,50,'UNIX_TIMESTAMP(LogTime) DESC');
					$dosql->ExecNoneQuery('SET NAMES latin1');
					while($data = $dosql->GetArray())
					{
						$row[] = $data;
					}
				}
			}
			else
			{
				echo json_encode("不存在数据表");
				exit;
			}
		}
		if(0< $fate && $fate < 3)
		{
			$j = 0;
			for($i=0;$i<=$fate;$i++)
			{
				$data_table[$i]=$qz.($statr_table+$i);
				if($dosql->IsTable($data_table[$i]))
				{
					if("LogRoleChat_"==$qz)
					{
						if($i<$fate)
						{
							$sql = "select * from $data_table[$i] where `RoleName` like '%$role%' and `Content` like '%$key%' and UNIX_TIMESTAMP(LogTime) >'$statr_time' ";
						}
						if($i == $fate)
						{
							$sql = "select * from $data_table[$i] where `RoleName` like '%$role%' and `Content` like '%$key%' and UNIX_TIMESTAMP(LogTime) < '$end_time' ";
						}
						$dosql->ExecNoneQuery('SET NAMES Default');
						$dopage->GetPage($sql,50,'UNIX_TIMESTAMP(LogTime) DESC');
						while($data = $dosql->GetArray())
						{
							$row[$j] = $data;
							$row[$j]['Content'] = iconv("gbk","utf8",$row[$j]['Content']);
							$row[$j]['RoleName'] = iconv("gbk","utf8",$row[$j]['RoleName']);
							$row[$j]['ToName']= iconv("gbk","utf8",$row[$j]['ToName']);
							$j++;
						}
						contiue;
					}
					else if("LogRoleGmCmd_"==$qz)
					{
						if($i<$fate)
						{
							$sql = "select * from $data_table[$i] where `RoleName` like '%$role%' and `CmdLine` like '%$key%' and UNIX_TIMESTAMP(LogTime) >'$statr_time' ";
						}
						if($i==$fate)
						{
							$sql = "select * from $data_table[$i] where `RoleName` like '%$role%' and `CmdLine` like '%$key%' and UNIX_TIMESTAMP(LogTime)<'$end_time' ";
						}
						$dosql->ExecNoneQuery('SET NAMES Default');
						$dopage->GetPage($sql,50,'UNIX_TIMESTAMP(LogTime) DESC');
						while($data = $dosql->GetArray())
						{
							$row[$j] = $data;
							$j++;
						}
						contiue;
					}
				}
				else
				{
					continue;
				}
			}
		}
		if( $fate >= 3){
			echo json_encode("结束时间与开始时间相差不能超过3天");
			exit;
		}
		if(0>$fate)
		{
			echo json_encode("结束时间不能低于开始时间");
			exit;
		}
		if(0==$fate && $fag<0)
		{
			echo json_encode("结束时间不能低于开始时间");
			exit;
		}
	}
	echo json_encode($row);
}
//NPC商店操作记录
if(isset($_POST['npcshop']))
{
	$nums = 0;
	$row[]="";
	$db1 = $_POST['xzdb'];
	$fg = $_POST['fag'];
	if($fg == 1){ $fg = "RoleName";}
	else if($fg == 2){ $fg = "LoginName";}
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
	
	$qz="LogRoleShop_";
	$Acc=$_POST['us'];
	$Acc = iconv("utf8","gbk",$Acc);
	
	date_default_timezone_set('PRC');
	//提交过来的开始时间的int
	$statr_time=strtotime($_POST['time']);

	//获取开始数据表
	$statr_table=date('ymd',$statr_time);
	
	//提交过来的结束时间
	$end_time=strtotime($_POST['time1']);
	
	//获取结束数据表
	$end_table=date('ymd',$end_time);
	
	
	//未填写结束时间
	if("" == $end_time || '0' == $end_time)
	{
		$data_table=$qz.$statr_table;
		$k=0;
		if($dosql->IsTable($data_table))
		{
			$sql = "select * from $data_table where `$fg` like '%$Acc%' ";
			
			$dosql->ExecNoneQuery('SET NAMES Default');
			$dopage->GetPage($sql,50,'UNIX_TIMESTAMP(LogTime) DESC');
			while($data = $dosql->GetArray())
			{
				$row[$k]['acc'] = $data['LoginName'];
				$row[$k]['role'] = iconv("gbk","utf8",$data['RoleName']);
				if($data['ShopAct'] == 0)
				{
					$row[$k]['ShopAct']="购买";
				}
				if($data['ShopAct'] == 1)
				{
					$row[$k]['ShopAct']="购买卖出的物品";
				}
				if($data['ShopAct'] == 2)
				{
					$row[$k]['ShopAct']="卖出";
				}
				if($data['ShopAct'] == 3)
				{
					$row[$k]['ShopAct']="修理单件装备";
				}
				if($data['ShopAct'] == 4)
				{
					$row[$k]['ShopAct']="修理所有装备";
				}
				$row[$k]['yinbi'] = $data['OldShopData_Money'];
				$row[$k]['bangyin'] = $data['OldShopData_Bull'];
				$a = "";
				for($f=1;$f<=4;$f++)
				{
					if($data['GoodsData_GoodsMoneyType'.$f] == 20)
					{
						$a .= "银币：".$data['GoodsData_GoodsMoney'.$f]*$data['GoodsData_Num']."/";
					}
					if($data['GoodsData_GoodsMoneyType'.$f] == 21)
					{
						$a .= "绑银：".$data['GoodsData_GoodsMoney'.$f]*$data['GoodsData_Num']."/";
					}
					if($data['GoodsData_GoodsMoneyType'.$f] == 76)
					{
						$a .= "战场荣誉：".$data['GoodsData_GoodsMoney'.$f]*$data['GoodsData_Num']."/";
					}
					if($data['GoodsData_GoodsMoneyType'.$f] == 77)
					{
						$a .= "战场徽章：".$data['GoodsData_GoodsMoney'.$f]*$data['GoodsData_Num']."/";
					}
					if($data['GoodsData_GoodsMoneyType'.$f] == 78)
					{
						$a .= "优胜奖章：".$data['GoodsData_GoodsMoney'.$f]*$data['GoodsData_Num']."/";
					}
				}
				$a.="公会贡献：".$data['ClanConDec'];
				$row[$k]['xiaohao'] = $a;
				$row[$k]['daoju'] = $data['GoodsData_GoodsId'];
				$row[$k]['time'] = $data['LogTime'];
				$row[$k]['GoodsData_Num'] = $data['GoodsData_Num'];
				$sql = "SELECT `Name` FROM ItemDef where `ItemID`='$data[GoodsData_GoodsId]'";
				mysql_select_db('resdb');
				$rest = mysql_query($sql);
				$rowf = mysql_fetch_array($rest);
				if($rowf != "" && $rowf != false)
				{
					$row[$k]['daoju'] = iconv("gbk","utf8",$rowf['Name']);
				}
				$dosql->select_db($db);
				$k++;
			}
			$nums = count($row);
		}
		else if(!$dosql->IsTable($data_table))
		{
			echo json_encode("不存在数据表");
			exit;
		}
	}
	else
	{
		$fate=$end_table-$statr_table;
		$fag = $end_time-$statr_time;
		if("0"==$fate && $fag>=0)
		{
			$k=0;
			$data_table=$qz.$statr_table;
			if($dosql->IsTable($data_table))
			{
			
				$sql = "select *  from $data_table where `$fg` like '%$Acc%'  and UNIX_TIMESTAMP(LogTime) >'$statr_time' and UNIX_TIMESTAMP(LogTime)<'$end_time' ";
				$dosql->ExecNoneQuery('SET NAMES Default');
				$dopage->GetPage($sql,50,'UNIX_TIMESTAMP(LogTime) DESC');
				while($data = $dosql->GetArray())
				{					
					$row[$k]['acc'] = $data['LoginName'];
					$row[$k]['role'] =iconv("gbk","utf8",$data['RoleName']);
					if($data['ShopAct'] == 0)
					{
						$row[$k]['ShopAct']="购买";
					}
					if($data['ShopAct'] == 1)
					{
						$row[$k]['ShopAct']="购买卖出的物品";
					}
					if($data['ShopAct'] == 2)
					{
						$row[$k]['ShopAct']="卖出";
					}
					if($data['ShopAct'] == 3)
					{
						$row[$k]['ShopAct']="修理单件装备";
					}
					if($data['ShopAct'] == 4)
					{
						$row[$k]['ShopAct']="修理所有装备";
					}
					$row[$k]['yinbi'] = $data['OldShopData_Money'];
					$row[$k]['bangyin'] = $data['OldShopData_Bull'];
					$a = "";
					for($f=1;$f<=4;$f++)
					{
						if($data['GoodsData_GoodsMoneyType'.$f] == 20)
						{
							$a .= "银币：".$data['GoodsData_GoodsMoney'.$f]*$data['GoodsData_Num']."/";
						}
						if($data['GoodsData_GoodsMoneyType'.$f] == 21)
						{
							$a .= "绑银：".$data['GoodsData_GoodsMoney'.$f]*$data['GoodsData_Num']."/";
						}
						if($data['GoodsData_GoodsMoneyType'.$f] == 76)
						{
							$a .= "战场荣誉：".$data['GoodsData_GoodsMoney'.$f]*$data['GoodsData_Num']."/";
						}
						if($data['GoodsData_GoodsMoneyType'.$f] == 77)
						{
							$a .= "战场徽章：".$data['GoodsData_GoodsMoney'.$f]*$data['GoodsData_Num']."/";
						}
						if($data['GoodsData_GoodsMoneyType'.$f] == 78)
						{
							$a .= "优胜奖章：".$data['GoodsData_GoodsMoney'.$f]*$data['GoodsData_Num']."/";
						}
					}
					$a.="公会贡献：".$data['ClanConDec'];
					$row[$k]['xiaohao'] = $a;
					$row[$k]['daoju'] = $data['GoodsData_GoodsId'];
					$row[$k]['time'] = $data['LogTime'];
					$row[$k]['GoodsData_Num'] = $data['GoodsData_Num'];
					$sql = "SELECT `Name` FROM ItemDef where `ItemID`='$data[GoodsData_GoodsId]'";
					mysql_select_db('resdb');
					$rest = mysql_query($sql);
					$rowf = mysql_fetch_array($rest);
					if($rowf != "" && $rowf != false)
					{
						$row[$k]['daoju'] = iconv("gbk","utf8",$rowf['Name']);
					}
					$dosql->select_db($db);
					$k++;
					
				}			
			}
			else
			{
				echo json_encode("不存在数据表");
				exit;
			}
		}
		if(0< $fate && $fate < 3)
		{
			$k = 0;
			for($i=0;$i<=$fate;$i++)
			{
				$data_table[$i]=$qz.($statr_table+$i);
				if($dosql->IsTable($data_table[$i]))
				{
				
					if($i<$fate)
					{
						$sql = "select * from $data_table[$i] where `$fg` like '%$Acc%'  and UNIX_TIMESTAMP(LogTime) >'$statr_time' ";
					}
					if($i == $fate)
					{
						$sql = "select * from $data_table[$i] where `$fg` like '%$Acc%'  and UNIX_TIMESTAMP(LogTime) < '$end_time' ";
					}
					$dosql->ExecNoneQuery('SET NAMES Default');
					$dopage->GetPage($sql,50,'UNIX_TIMESTAMP(LogTime) DESC');
					while($data = $dosql->GetArray())
					{				
						$row[$k]['acc'] = $data['LoginName'];
						$row[$k]['role'] = iconv("gbk","utf8",$data['RoleName']);
						if($data['ShopAct'] == 0)
						{
							$row[$k]['ShopAct']="购买";
						}
						if($data['ShopAct'] == 1)
						{
							$row[$k]['ShopAct']="购买卖出的物品";
						}
						if($data['ShopAct'] == 2)
						{
							$row[$k]['ShopAct']="卖出";
						}
						if($data['ShopAct'] == 3)
						{
							$row[$k]['ShopAct']="修理单件装备";
						}
						if($data['ShopAct'] == 4)
						{
							$row[$k]['ShopAct']="修理所有装备";
						}
						$row[$k]['yinbi'] = $data['OldShopData_Money'];
						$row[$k]['bangyin'] = $data['OldShopData_Bull'];
						$a = "";
						for($f=1;$f<=4;$f++)
						{
							if($data['GoodsData_GoodsMoneyType'.$f] == 20)
							{
								$a .= "银币：".$data['GoodsData_GoodsMoney'.$f]*$data['GoodsData_Num']."/";
							}
							if($data['GoodsData_GoodsMoneyType'.$f] == 21)
							{
								$a .= "绑银：".$data['GoodsData_GoodsMoney'.$f]*$data['GoodsData_Num']."/";
							}
							if($data['GoodsData_GoodsMoneyType'.$f] == 76)
							{
								$a .= "战场荣誉：".$data['GoodsData_GoodsMoney'.$f]*$data['GoodsData_Num']."/";
							}
							if($data['GoodsData_GoodsMoneyType'.$f] == 77)
							{
								$a .= "战场徽章：".$data['GoodsData_GoodsMoney'.$f]*$data['GoodsData_Num']."/";
							}
							if($data['GoodsData_GoodsMoneyType'.$f] == 78)
							{
								$a .= "优胜奖章：".$data['GoodsData_GoodsMoney'.$f]*$data['GoodsData_Num']."/";
							}
						}
						$a.="公会贡献：".$data['ClanConDec'];
						$row[$k]['xiaohao'] = $a;
						$row[$k]['time'] = $data['LogTime'];
						$row[$k]['GoodsData_Num'] = $data['GoodsData_Num'];
						$row[$k]['daoju'] = $data['GoodsData_GoodsId'];
						$sql = "SELECT `Name` FROM ItemDef where `ItemID`='$data[GoodsData_GoodsId]'";
						mysql_select_db('resdb');
						$rest = mysql_query($sql);
						$rowf = mysql_fetch_array($rest);
						if($rowf != "" && $rowf != false)
						{
							$row[$k]['daoju'] = iconv("gbk","utf8",$rowf['Name']);
						}
						$dosql->select_db($db);
						$k++;
					}
				}
				else
				{
					continue;
				}
			}
		}
		if( $fate >= 3){
			echo json_encode("结束时间与开始时间相差不能超过3天");
			exit;
		}
		if(0>$fate)
		{
			echo json_encode("结束时间不能低于开始时间");
			exit;
		}
		if(0==$fate && $fag<0)
		{
			echo json_encode("结束时间不能低于开始时间");
			exit;
		}
	}
	//$row['nums'] = $nums;
	echo json_encode($row);
}

//邮件操作
if(isset($_POST['mailOper']))
{
	$nums = 0;
	$row[]="";
	$db1 = $_POST['xzdb'];
	$fg = $_POST['fag'];
	if($fg == 1){ $fg = "RoleName";}
	else if($fg == 2){ $fg = "LoginName";}
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
	
	$qz="LogRoleMail_";
	$Acc=$_POST['us'];
	$Acc = iconv("utf8","gbk",$Acc);
	
	date_default_timezone_set('PRC');
	//提交过来的开始时间的int
	$statr_time=strtotime($_POST['time']);

	//获取开始数据表
	$statr_table=date('ymd',$statr_time);
	
	//提交过来的结束时间
	$end_time=strtotime($_POST['time1']);
	
	//获取结束数据表
	$end_table=date('ymd',$end_time);
	
	
	//未填写结束时间
	if("" == $end_time || '0' == $end_time)
	{
		$data_table=$qz.$statr_table;
		$k=0;
		if($dosql->IsTable($data_table))
		{
			$sql = "select * from $data_table where `$fg` like '%$Acc%' ";
			
			$dosql->ExecNoneQuery('SET NAMES Default');
			$dopage->GetPage($sql,50,'UNIX_TIMESTAMP(LogTime) DESC');
			while($data = $dosql->GetArray())
			{
				$row[$k]['Uin'] = $data['Uin'];
				$row[$k]['LoginName'] = $data['LoginName'];
				$row[$k]['RoleName'] = iconv("gbk","utf8",$data['RoleName']);
				if($data['Operation'] == 0)
				{
					$row[$k]['Operation']="发送";
				}
				if($data['Operation'] == 1)
				{
					$row[$k]['Operation']="删除";
				}
				if($data['Operation'] == 2)
				{
					$row[$k]['Operation']="取出物品";
				}
				if($data['Operation'] == 3)
				{
					$row[$k]['Operation']="系统发邮件";
				}
				if($data['Operation'] == 4)
				{
					$row[$k]['Operation']="阅读";
				}
				if($data['Operation'] == 5)
				{
					$row[$k]['Operation']="获取后删除";
				}
				if($data['Operation'] == 6)
				{
					$row[$k]['Operation']="系统删邮件";
				}
				$row[$k]['Flag'] = $data['RoleMailData_Flags'];
				//邮件标志
				if($data['RoleMailData_Flags'] == 0)
				{
					$row[$k]['Flag']="未读非系统邮件";
				}
				//邮件标志
				if($data['RoleMailData_Flags'] == 1)
				{
					$row[$k]['Flag']="未读系统邮件";
				}
				//邮件标志
				if($data['RoleMailData_Flags'] == 2)
				{
					$row[$k]['Flag']="已读非系统邮件";
				}
				if($data['RoleMailData_Flags'] == 11)
				{
					$row[$k]['Flag']="已读系统邮件";
				}
				//邮件ID
				$row[$k]['mid'] = $data['RoleMailData_WID'];
				//收件人
				$row[$k]['to'] = iconv("gbk","utf8",$data['RoleMailData_To']);
				//发件人
				$row[$k]['Send'] = iconv("gbk","utf8",$data['RoleMailData_Send']);
				//标题
				$row[$k]['Title'] = iconv("gbk","utf8",$data['RoleMailData_Title']);
				//记录时间
				$row[$k]['LogTime'] = $data['LogTime'];
				$a="";
				for($f=1;$f<=$data['RoleMailData_ItemNum'];$f++)
				{
					$x = $data['RoleMailData_Items'.$f.'_DefID'];
					//邮寄物品
					$sql = "SELECT `Name` FROM ItemDef where `ItemID`='$x'";
					//echo $sql;
					mysql_select_db('resdb');
					$rest = mysql_query($sql);
					$rowf = mysql_fetch_array($rest);
					if($rowf != "" && $rowf != false)
					{
						$a .= iconv("gbk","utf8",$rowf['Name']).'：';
					}
					$dosql->select_db($db);
					//物品唯一ID
					$a .= $data['RoleMailData_Items'.$f.'_WID'].'：';
					//邮寄物品数量
					$a .= $data['RoleMailData_Items'.$f.'_Num'].'：';
					//有效期
					if( $data['RoleMailData_Items'.$f.'_ValidEnd'] == 0)
					{
						$a .= "永久<br/>";
					}
					else if( $data['RoleMailData_Items'.$f.'_ValidEnd'] != 0)
					{
						$a .= date('Y-m-d H:i:s',$data['RoleGrid_ValidEnd'])."b<r/>";
					}
				}
				
				$row[$k]['Items'] = $a;
				$k++;
			}
			$nums = count($row);
		}
		else if(!$dosql->IsTable($data_table))
		{
			echo json_encode("不存在数据表");
			exit;
		}
	}
	else
	{
		$fate=$end_table-$statr_table;
		$fag = $end_time-$statr_time;
		if("0"==$fate && $fag>=0)
		{
			$k=0;
			$data_table=$qz.$statr_table;
			if($dosql->IsTable($data_table))
			{			
				$sql = "select *  from $data_table where `$fg` like '%$Acc%'  and UNIX_TIMESTAMP(LogTime) >'$statr_time' and UNIX_TIMESTAMP(LogTime)<'$end_time' ";
				$dosql->ExecNoneQuery('SET NAMES Default');
				$dopage->GetPage($sql,50,'UNIX_TIMESTAMP(LogTime) DESC');
				while($data = $dosql->GetArray())
				{
					$row[$k]['Uin'] = $data['Uin'];
					$row[$k]['LoginName'] = $data['LoginName'];
					$row[$k]['RoleName'] = iconv("gbk","utf8",$data['RoleName']);
					if($data['Operation'] == 0)
					{
						$row[$k]['Operation']="发送";
					}
					if($data['Operation'] == 1)
					{
						$row[$k]['Operation']="删除";
					}
					if($data['Operation'] == 2)
					{
						$row[$k]['Operation']="取出物品";
					}
					if($data['Operation'] == 3)
					{
						$row[$k]['Operation']="系统发邮件";
					}
					if($data['Operation'] == 4)
					{
						$row[$k]['Operation']="阅读";
					}
					if($data['Operation'] == 5)
					{
						$row[$k]['Operation']="获取后删除";
					}
					if($data['Operation'] == 6)
					{
						$row[$k]['Operation']="系统删邮件";
					}
					//邮件标志
					if($data['RoleMailData_Flags'] == 0)
					{
						$row[$k]['Flag']="未读非系统邮件";
					}
					//邮件标志
					if($data['RoleMailData_Flags'] == 1)
					{
						$row[$k]['Flag']="未读系统邮件";
					}
					//邮件标志
					if($data['RoleMailData_Flags'] == 2)
					{
						$row[$k]['Flag']="已读非系统邮件";
					}
					if($data['RoleMailData_Flags'] == 11)
					{
						$row[$k]['Flag']="已读系统邮件";
					}
					//邮件ID
					$row[$k]['mid'] = $data['RoleMailData_WID'];
					//收件人
					$row[$k]['to'] = iconv("gbk","utf8",$data['RoleMailData_To']);
					//发件人
					$row[$k]['Send'] = iconv("gbk","utf8",$data['RoleMailData_Send']);
					//标题
					$row[$k]['Title'] = iconv("gbk","utf8",$data['RoleMailData_Title']);
					//记录时间
					$row[$k]['LogTime'] = $data['LogTime'];
					$a="";
					for($f=1;$f<=$data['RoleMailData_ItemNum'];$f++)
					{
						$x = $data['RoleMailData_Items'.$f.'_DefID'];
						//邮寄物品
						$sql = "SELECT `Name` FROM ItemDef where `ItemID`='$x'";
						//echo $sql;
						mysql_select_db('resdb');
						$rest = mysql_query($sql);
						$rowf = mysql_fetch_array($rest);
						if($rowf != "" && $rowf != false)
						{
							$a .= iconv("gbk","utf8",$rowf['Name']).'：';
						}
						$dosql->select_db($db);
						//物品唯一ID
						$a .= $data['RoleMailData_Items'.$f.'_WID'].'：';
						//邮寄物品数量
						$a .= $data['RoleMailData_Items'.$f.'_Num'].'：';
						//有效期
						if( $data['RoleMailData_Items'.$f.'_ValidEnd'] == 0)
						{
							$a .= "永久<br/>";
						}
						else if( $data['RoleMailData_Items'.$f.'_ValidEnd'] != 0)
						{
							$a .= date('Y-m-d H:i:s',$data['RoleGrid_ValidEnd'])."<br/>";
						}
					}
					
					$row[$k]['Items'] = $a;
					$k++;
				}
				$nums = count($row);		
			}
			else
			{
				echo json_encode("不存在数据表");
				exit;
			}
		}
		if(0< $fate && $fate < 3)
		{
			$k = 0;
			for($i=0;$i<=$fate;$i++)
			{
				$data_table[$i]=$qz.($statr_table+$i);
				if($dosql->IsTable($data_table[$i]))
				{
				
					if($i<$fate)
					{
						$sql = "select * from $data_table[$i] where `$fg` like '%$Acc%'  and UNIX_TIMESTAMP(LogTime) >'$statr_time' ";
					}
					if($i == $fate)
					{
						$sql = "select * from $data_table[$i] where `$fg` like '%$Acc%'  and UNIX_TIMESTAMP(LogTime) < '$end_time' ";
					}
					$dosql->ExecNoneQuery('SET NAMES Default');
					$dopage->GetPage($sql,50,'UNIX_TIMESTAMP(LogTime) DESC');
					while($data = $dosql->GetArray())
					{
						$row[$k]['Uin'] = $data['Uin'];
						$row[$k]['LoginName'] = $data['LoginName'];
						$row[$k]['RoleName'] = iconv("gbk","utf8",$data['RoleName']);
						if($data['Operation'] == 0)
						{
							$row[$k]['Operation']="发送";
						}
						if($data['Operation'] == 1)
						{
							$row[$k]['Operation']="删除";
						}
						if($data['Operation'] == 2)
						{
							$row[$k]['Operation']="取出物品";
						}
						if($data['Operation'] == 3)
						{
							$row[$k]['Operation']="系统发邮件";
						}
						if($data['Operation'] == 4)
						{
							$row[$k]['Operation']="阅读";
						}
						if($data['Operation'] == 5)
						{
							$row[$k]['Operation']="获取后删除";
						}
						if($data['Operation'] == 6)
						{
							$row[$k]['Operation']="系统删邮件";
						}
						
						$row[$k]['Flag'] = $data['RoleMailData_Flags'];
						//邮件标志
						if($data['RoleMailData_Flags'] == 0)
						{
							$row[$k]['Flag']="未读非系统邮件";
						}
						//邮件标志
						if($data['RoleMailData_Flags'] == 1)
						{
							$row[$k]['Flag']="未读系统邮件";
						}
						//邮件标志
						if($data['RoleMailData_Flags'] == 2)
						{
							$row[$k]['Flag']="已读非系统邮件";
						}
						if($data['RoleMailData_Flags'] == 11)
						{
							$row[$k]['Flag']="已读系统邮件";
						}
						//邮件ID
						$row[$k]['mid'] = $data['RoleMailData_WID'];
						//收件人
						$row[$k]['to'] = iconv("gbk","utf8",$data['RoleMailData_To']);
						//发件人
						$row[$k]['Send'] = iconv("gbk","utf8",$data['RoleMailData_Send']);
						//标题
						$row[$k]['Title'] = iconv("gbk","utf8",$data['RoleMailData_Title']);
						//记录时间
						$row[$k]['LogTime'] = $data['LogTime'];
						$a="";
						for($f=1;$f<=$data['RoleMailData_ItemNum'];$f++)
						{
							$x = $data['RoleMailData_Items'.$f.'_DefID'];
							//邮寄物品
							$sql = "SELECT `Name` FROM ItemDef where `ItemID`='$x'";
							//echo $sql;
							mysql_select_db('resdb');
							$rest = mysql_query($sql);
							$rowf = mysql_fetch_array($rest);
							if($rowf != "" && $rowf != false)
							{
								$a .= iconv("gbk","utf8",$rowf['Name']).':';
							}
							$dosql->select_db($db);
							//物品唯一ID
							$a .= $data['RoleMailData_Items'.$f.'_WID'].':';
							//邮寄物品数量
							$a .= $data['RoleMailData_Items'.$f.'_Num'].':';
							//有效期
							if( $data['RoleMailData_Items'.$f.'_ValidEnd'] == 0)
							{
								$a .= "永久<br/>";
							}
							else if( $data['RoleMailData_Items'.$f.'_ValidEnd'] != 0)
							{
								$a .= date('Y-m-d H:i:s',$data['RoleGrid_ValidEnd'])."<br/>";
							}
						}
						
						$row[$k]['Items'] = $a;
						$k++;
					}
					$nums = count($row);
				}
				else
				{
					continue;
				}
			}
		}
		if( $fate >= 3){
			echo json_encode("结束时间与开始时间相差不能超过3天");
			exit;
		}
		if(0>$fate)
		{
			echo json_encode("结束时间不能低于开始时间");
			exit;
		}
		if(0==$fate && $fag<0)
		{
			echo json_encode("结束时间不能低于开始时间");
			exit;
		}
	}
	//$row['nums'] = $nums;
	echo json_encode($row);
}
//玩家交易
if(isset($_POST['trade']))
{
	$nums = 0;
	$row[]="";
	$db1 = $_POST['xzdb'];
	$fg = $_POST['fag'];
	if($fg == 1){ $fg = "RoleName";}
	else if($fg == 2){ $fg = "LoginName";}
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
	
	$qz="LogRoleExchg_";
	$Acc=$_POST['us'];
	$Acc = iconv("utf8","gbk",$Acc);
	
	date_default_timezone_set('PRC');
	//提交过来的开始时间的int
	$statr_time=strtotime($_POST['time']);

	//获取开始数据表
	$statr_table=date('ymd',$statr_time);
	
	//提交过来的结束时间
	$end_time=strtotime($_POST['time1']);
	
	//获取结束数据表
	$end_table=date('ymd',$end_time);
	
	
	//未填写结束时间
	if("" == $end_time || '0' == $end_time)
	{
		$data_table=$qz.$statr_table;
		$k=0;
		if($dosql->IsTable($data_table))
		{
			$sql = "select * from $data_table where `$fg` like '%$Acc%' ";
			
			$dosql->ExecNoneQuery('SET NAMES Default');
			$dopage->GetPage($sql,50,'UNIX_TIMESTAMP(LogTime) DESC');
			while($data = $dosql->GetArray())
			{
				//交易发起人
				$row[$k]['Wid'] = $data['EventLogWID'];
				$row[$k]['Uiny'] = $data['Uin'];
				$row[$k]['LoginNamey'] = $data['LoginName'];
				$row[$k]['RoleNamey'] = iconv("gbk","utf8",$data['RoleName']);
				//被交易人
				$row[$k]['Uinx'] = $data['ExchgObjUin'];
				$row[$k]['LoginNamex'] = $data['ExchgObjLoginName'];
				$row[$k]['RoleNamex'] = iconv("gbk","utf8",$data['ExchgObjName']);
				
				//交易进来银币
				$row[$k]['Money'] = $data['ExchgIn_Money'];
				//交易进来(物品：唯一ID：交易物品数量：有效时间)
				$ay="";
				for($f=1;$f<=$data['ExchgIn_ItemNum'];$f++)
				{
					$x = $data['ExchgIn_RoleGrid'.$f.'_DefID'];
					//交易的物品名
					$sql = "SELECT `Name` FROM ItemDef where `ItemID`='$x'";
					//echo $sql;
					mysql_select_db('resdb');
					$rest = mysql_query($sql);
					$rowf = mysql_fetch_array($rest);
					if($rowf != "" && $rowf != false)
					{
						$ay .= iconv("gbk","utf8",$rowf['Name']).'：';
					}
					else
					{
						$ay .= $x;
					}
					$dosql->select_db($db);
					//物品唯一ID
					$ay .= $data['ExchgIn_RoleGrid'.$f.'_WID'].'：';
					//交易物品数量
					$ay .= $data['ExchgIn_RoleGrid'.$f.'_Num'].'：';
					if($data['ExchgIn_RoleGrid'.$f.'_ValidEnd'] == 0)
					{
						//物品有效时间
						$ay .= '永久<br/>';
					}
					else if($data['ExchgIn_RoleGrid'.$f.'_ValidEnd'] != 0)
					{
						$ay .= date('Y-m-d H:i:s',$data['ExchgIn_RoleGrid'.$f.'_ValidEnd'])."<br/>";
					}
				}
				
				$row[$k]['Itemsy'] = $ay;
				
				//交易过来的宠物
				$by="";
				for($f=1;$f<=$data['ExchgIn_PetNum'];$f++)
				{
					$x = $data['ExchgIn_RolePet'.$f.'_PetID'];
					//交易的宠物名
					$sql = "SELECT `Name` FROM PetDef where `PetID`='$x'";
					//echo $sql;
					mysql_select_db('resdb');
					$rest = mysql_query($sql);
					$rowf = mysql_fetch_array($rest);
					if($rowf != "" && $rowf != false)
					{
						$by .= iconv("gbk","utf8",$rowf['Name']).':';
					}
					else
					{
						$by .= $x.'：';
					}
					$dosql->select_db($db);
					//宠物一ID
					$by .= $data['ExchgIn_RolePet'.$f.'_WID'].'<br/>';
				}
				$row[$k]['Petsy'] = $by;
			 	//交易出去银币
				$row[$k]['Monex'] = $data['ExchgOut_Money'];
				//交易出去(物品：唯一ID：交易物品数量：有效时间)
				$ax="";
				for($f=1;$f<=$data['ExchgOut_ItemNum'];$f++)
				{
					$x = $data['ExchgOut_RoleGrid'.$f.'_DefID'];
					//交易的物品名
					$sql = "SELECT `Name` FROM ItemDef where `ItemID`='$x'";
					//echo $sql;
					mysql_select_db('resdb');
					$rest = mysql_query($sql);
					$rowf = mysql_fetch_array($rest);
					if($rowf != "" && $rowf != false)
					{
						$ax .= iconv("gbk","utf8",$rowf['Name']).':';
					}
					else
					{
						$ax .= $x;
					}
					$dosql->select_db($db);
					//物品唯一ID
					$ax .= $data['ExchgOut_RoleGrid'.$f.'_WID'].'：';
					//交易物品数量
					$ax .= $data['ExchgOut_RoleGrid'.$f.'_Num'].'：';
					//物品有效时间
					if($data['ExchgOut_RoleGrid'.$f.'_ValidEnd'] == 0)
					{
						$ax .= '永久<br/>';
					}
					else if($data['ExchgOut_RoleGrid'.$f.'_ValidEnd'] != 0)
					{
						$ax .= date('Y-m-d H:i:s',$data['ExchgOut_RoleGrid'.$f.'_ValidEnd'])."<br/>";
					}
				}
				$row[$k]['Itemsx'] = $ax;
				//交易出去的宠物
				$bx="";
				for($fx=1;$fx<=$data['ExchgOut_PetNum'];$fx++)
				{
					$x = $data['ExchgOut_RolePet'.$fx.'_PetID'];
					//交易的宠物名
					$sql = "SELECT `Name` FROM PetDef where `PetID`='$x'";
					//echo $sql;
					mysql_select_db('resdb');
					$rest = mysql_query($sql);
					$rowf = mysql_fetch_array($rest);
					if($rowf != "" && $rowf != false)
					{
						$bx .= iconv("gbk","utf8",$rowf['Name']).'：';
					}
					else
					{
						$bx .= $x;
					}
					$dosql->select_db($db);
					//宠物唯一ID
					$bx .= $data['ExchgOut_RolePet'.$fx.'_WID'].'<br/>';
				}
				$row[$k]['Petsx'] = $bx;
				
				$row[$k]['LogTime'] = $data['LogTime'];			
				$k++;
			}
		}
		else if(!$dosql->IsTable($data_table))
		{
			echo json_encode("不存在数据表");
			exit;
		}
	}
	else
	{
		$fate=$end_table-$statr_table;
		$fag = $end_time-$statr_time;
		if("0"==$fate && $fag>=0)
		{
			$k=0;
			$data_table=$qz.$statr_table;
			if($dosql->IsTable($data_table))
			{			
				$sql = "select *  from $data_table where `$fg` like '%$Acc%'  and UNIX_TIMESTAMP(LogTime) >'$statr_time' and UNIX_TIMESTAMP(LogTime)<'$end_time' ";
				$dosql->ExecNoneQuery('SET NAMES Default');
				$dopage->GetPage($sql,50,'UNIX_TIMESTAMP(LogTime) DESC');
				while($data = $dosql->GetArray())
				{
					//交易发起人
					$row[$k]['Wid'] = $data['EventLogWID'];
					$row[$k]['Uiny'] = $data['Uin'];
					$row[$k]['LoginNamey'] = $data['LoginName'];
					$row[$k]['RoleNamey'] = iconv("gbk","utf8",$data['RoleName']);
					//被交易人
					$row[$k]['Uinx'] = $data['ExchgObjUin'];
					$row[$k]['LoginNamex'] = $data['ExchgObjLoginName'];
					$row[$k]['RoleNamex'] = iconv("gbk","utf8",$data['ExchgObjName']);
					
					//交易进来银币
					$row[$k]['Money'] = $data['ExchgIn_Money'];
					//交易进来(物品：唯一ID：交易物品数量：有效时间)
					$ay="";
					for($f=1;$f<=$data['ExchgIn_ItemNum'];$f++)
					{
						$x = $data['ExchgIn_RoleGrid'.$f.'_DefID'];
						//交易的物品名
						$sql = "SELECT `Name` FROM ItemDef where `ItemID`='$x'";
						//echo $sql;
						mysql_select_db('resdb');
						$rest = mysql_query($sql);
						$rowf = mysql_fetch_array($rest);
						if($rowf != "" && $rowf != false)
						{
							$ay .= iconv("gbk","utf8",$rowf['Name']).'：';
						}
						else
						{
							$ay .= $x;
						}
						$dosql->select_db($db);
						//物品唯一ID
						$ay .= $data['ExchgIn_RoleGrid'.$f.'_WID'].'：';
						//交易物品数量
						$ay .= $data['ExchgIn_RoleGrid'.$f.'_Num'].'：';
						if($data['ExchgIn_RoleGrid'.$f.'_ValidEnd'] == 0)
						{
							//物品有效时间
							$ay .= '永久<br/>';
						}
						else if($data['ExchgIn_RoleGrid'.$f.'_ValidEnd'] != 0)
						{
							$ay .= date('Y-m-d H:i:s',$data['ExchgIn_RoleGrid'.$f.'_ValidEnd'])."/";
						}
					}
					
					$row[$k]['Itemsy'] = $ay;
					
					//交易过来的宠物
					$by="";
					for($f=1;$f<=$data['ExchgIn_PetNum'];$f++)
					{
						$x = $data['ExchgIn_RolePet'.$f.'_PetID'];
						//交易的宠物名
						$sql = "SELECT `Name` FROM PetDef where `PetID`='$x'";
						//echo $sql;
						mysql_select_db('resdb');
						$rest = mysql_query($sql);
						$rowf = mysql_fetch_array($rest);
						if($rowf != "" && $rowf != false)
						{
							$by .= iconv("gbk","utf8",$rowf['Name']).':';
						}
						else
						{
							$by .= $x.'：';
						}
						$dosql->select_db($db);
						//宠物一ID
						$by .= $data['ExchgIn_RolePet'.$f.'_WID'].'<br/>';
					}
					$row[$k]['Petsy'] = $by;
					//交易出去银币
					$row[$k]['Monex'] = $data['ExchgOut_Money'];
					//交易出去(物品：唯一ID：交易物品数量：有效时间)
					$ax="";
					for($f=1;$f<=$data['ExchgOut_ItemNum'];$f++)
					{
						$x = $data['ExchgOut_RoleGrid'.$f.'_DefID'];
						//交易的物品名
						$sql = "SELECT `Name` FROM ItemDef where `ItemID`='$x'";
						//echo $sql;
						mysql_select_db('resdb');
						$rest = mysql_query($sql);
						$rowf = mysql_fetch_array($rest);
						if($rowf != "" && $rowf != false)
						{
							$ax .= iconv("gbk","utf8",$rowf['Name']).':';
						}
						else
						{
							$ax .= $x;
						}
						$dosql->select_db($db);
						//物品唯一ID
						$ax .= $data['ExchgOut_RoleGrid'.$f.'_WID'].'：';
						//交易物品数量
						$ax .= $data['ExchgOut_RoleGrid'.$f.'_Num'].'：';
						//物品有效时间
						if($data['ExchgOut_RoleGrid'.$f.'_ValidEnd'] == 0)
						{
							$ax .= '永久<br/>';
						}
						else if($data['ExchgOut_RoleGrid'.$f.'_ValidEnd'] != 0)
						{
							$ax .= date('Y-m-d H:i:s',$data['ExchgOut_RoleGrid'.$f.'_ValidEnd'])."<br/>";
						}
					}
					$row[$k]['Itemsx'] = $ax;
					//交易出去的宠物
					$bx="";
					for($fx=1;$fx<=$data['ExchgOut_PetNum'];$fx++)
					{
						$x = $data['ExchgOut_RolePet'.$fx.'_PetID'];
						//交易的宠物名
						$sql = "SELECT `Name` FROM PetDef where `PetID`='$x'";
						//echo $sql;
						mysql_select_db('resdb');
						$rest = mysql_query($sql);
						$rowf = mysql_fetch_array($rest);
						if($rowf != "" && $rowf != false)
						{
							$bx .= iconv("gbk","utf8",$rowf['Name']).'：';
						}
						else
						{
							$bx .= $x;
						}
						$dosql->select_db($db);
						//宠物唯一ID
						$bx .= $data['ExchgOut_RolePet'.$fx.'_WID'].'<br/>';
					}
					$row[$k]['Petsx'] = $bx;
					
					$row[$k]['LogTime'] = $data['LogTime'];			
					$k++;
				}
				$nums = count($row);		
			}
			else
			{
				echo json_encode("不存在数据表");
				exit;
			}
		}
		if(0< $fate && $fate < 3)
		{
			$k = 0;
			for($i=0;$i<=$fate;$i++)
			{
				$data_table[$i]=$qz.($statr_table+$i);
				if($dosql->IsTable($data_table[$i]))
				{
				
					if($i<$fate)
					{
						$sql = "select * from $data_table[$i] where `$fg` like '%$Acc%'  and UNIX_TIMESTAMP(LogTime) >'$statr_time' ";
					}
					if($i == $fate)
					{
						$sql = "select * from $data_table[$i] where `$fg` like '%$Acc%'  and UNIX_TIMESTAMP(LogTime) < '$end_time' ";
					}
					$dosql->ExecNoneQuery('SET NAMES Default');
					$dopage->GetPage($sql,50,'UNIX_TIMESTAMP(LogTime) DESC');
					while($data = $dosql->GetArray())
					{
						//交易发起人
						$row[$k]['Wid'] = $data['EventLogWID'];
						$row[$k]['Uiny'] = $data['Uin'];
						$row[$k]['LoginNamey'] = $data['LoginName'];
						$row[$k]['RoleNamey'] = iconv("gbk","utf8",$data['RoleName']);
						//被交易人
						$row[$k]['Uinx'] = $data['ExchgObjUin'];
						$row[$k]['LoginNamex'] = $data['ExchgObjLoginName'];
						$row[$k]['RoleNamex'] = iconv("gbk","utf8",$data['ExchgObjName']);
						
						//交易进来银币
						$row[$k]['Money'] = $data['ExchgIn_Money'];
						//交易进来(物品：唯一ID：交易物品数量：有效时间)
						$ay="";
						for($f=1;$f<=$data['ExchgIn_ItemNum'];$f++)
						{
							$x = $data['ExchgIn_RoleGrid'.$f.'_DefID'];
							//交易的物品名
							$sql = "SELECT `Name` FROM ItemDef where `ItemID`='$x'";
							//echo $sql;
							mysql_select_db('resdb');
							$rest = mysql_query($sql);
							$rowf = mysql_fetch_array($rest);
							if($rowf != "" && $rowf != false)
							{
								$ay .= iconv("gbk","utf8",$rowf['Name']).'：';
							}
							else
							{
								$ay .= $x;
							}
							$dosql->select_db($db);
							//物品唯一ID
							$ay .= $data['ExchgIn_RoleGrid'.$f.'_WID'].'：';
							//交易物品数量
							$ay .= $data['ExchgIn_RoleGrid'.$f.'_Num'].'：';
							if($data['ExchgIn_RoleGrid'.$f.'_ValidEnd'] == 0)
							{
								//物品有效时间
								$ay .= '永久<br/>';
							}
							else if($data['ExchgIn_RoleGrid'.$f.'_ValidEnd'] != 0)
							{
								$ay .= date('Y-m-d H:i:s',$data['ExchgIn_RoleGrid'.$f.'_ValidEnd'])."<br/>";
							}
						}
						
						$row[$k]['Itemsy'] = $ay;
						
						//交易过来的宠物
						$by="";
						for($f=1;$f<=$data['ExchgIn_PetNum'];$f++)
						{
							$x = $data['ExchgIn_RolePet'.$f.'_PetID'];
							//交易的宠物名
							$sql = "SELECT `Name` FROM PetDef where `PetID`='$x'";
							//echo $sql;
							mysql_select_db('resdb');
							$rest = mysql_query($sql);
							$rowf = mysql_fetch_array($rest);
							if($rowf != "" && $rowf != false)
							{
								$by .= iconv("gbk","utf8",$rowf['Name']).':';
							}
							else
							{
								$by .= $x.'：';
							}
							$dosql->select_db($db);
							//宠物一ID
							$by .= $data['ExchgIn_RolePet'.$f.'_WID'].'<br/>';
						}
						$row[$k]['Petsy'] = $by;
						//交易出去银币
						$row[$k]['Monex'] = $data['ExchgOut_Money'];
						//交易出去(物品：唯一ID：交易物品数量：有效时间)
						$ax="";
						for($f=1;$f<=$data['ExchgOut_ItemNum'];$f++)
						{
							$x = $data['ExchgOut_RoleGrid'.$f.'_DefID'];
							//交易的物品名
							$sql = "SELECT `Name` FROM ItemDef where `ItemID`='$x'";
							//echo $sql;
							mysql_select_db('resdb');
							$rest = mysql_query($sql);
							$rowf = mysql_fetch_array($rest);
							if($rowf != "" && $rowf != false)
							{
								$ax .= iconv("gbk","utf8",$rowf['Name']).':';
							}
							else
							{
								$ax .= $x;
							}
							$dosql->select_db($db);
							//物品唯一ID
							$ax .= $data['ExchgOut_RoleGrid'.$f.'_WID'].'：';
							//交易物品数量
							$ax .= $data['ExchgOut_RoleGrid'.$f.'_Num'].'：';
							//物品有效时间
							if($data['ExchgOut_RoleGrid'.$f.'_ValidEnd'] == 0)
							{
								$ax .= '永久<br/>';
							}
							else if($data['ExchgOut_RoleGrid'.$f.'_ValidEnd'] != 0)
							{
								$ax .= date('Y-m-d H:i:s',$data['ExchgOut_RoleGrid'.$f.'_ValidEnd'])."<br/>";
							}
						}
						$row[$k]['Itemsx'] = $ax;
						//交易出去的宠物
						$bx="";
						for($fx=1;$fx<=$data['ExchgOut_PetNum'];$fx++)
						{
							$x = $data['ExchgOut_RolePet'.$fx.'_PetID'];
							//交易的宠物名
							$sql = "SELECT `Name` FROM PetDef where `PetID`='$x'";
							//echo $sql;
							mysql_select_db('resdb');
							$rest = mysql_query($sql);
							$rowf = mysql_fetch_array($rest);
							if($rowf != "" && $rowf != false)
							{
								$bx .= iconv("gbk","utf8",$rowf['Name']).'：';
							}
							else
							{
								$bx .= $x;
							}
							$dosql->select_db($db);
							//宠物唯一ID
							$bx .= $data['ExchgOut_RolePet'.$fx.'_WID'].'<br/>';
						}
						$row[$k]['Petsx'] = $bx;
						
						$row[$k]['LogTime'] = $data['LogTime'];			
						$k++;
					}
				}
				else
				{
					continue;
				}
			}
		}
		if( $fate >= 3){
			echo json_encode("结束时间与开始时间相差不能超过3天");
			exit;
		}
		if(0>$fate)
		{
			echo json_encode("结束时间不能低于开始时间");
			exit;
		}
		if(0==$fate && $fag<0)
		{
			echo json_encode("结束时间不能低于开始时间");
			exit;
		}
	}
	//$row['nums'] = $nums;
	echo json_encode($row);
}

//购买摆摊物品
if(isset($_POST['stallBuy']))
{
	$nums = 0;
	$row[]="";
	$db1 = $_POST['xzdb'];
	$fg = $_POST['fag'];
	if($fg == 1){ $fg = "RoleName";}
	else if($fg == 2){ $fg = "BuyLoginName";}
	else if($fg == 3){ $fg = "StallOwner";}
	else if($fg == 4){ $fg = "OwnerLoginName";}
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
	
	$qz="LogRoleStallBuy_";
	$Acc=$_POST['us'];
	$Acc = iconv("utf8","gbk",$Acc);
	
	date_default_timezone_set('PRC');
	//提交过来的开始时间的int
	$statr_time=strtotime($_POST['time']);

	//获取开始数据表
	$statr_table=date('ymd',$statr_time);
	
	//提交过来的结束时间
	$end_time=strtotime($_POST['time1']);
	
	//获取结束数据表
	$end_table=date('ymd',$end_time);
	
	
	//未填写结束时间
	if("" == $end_time || '0' == $end_time)
	{
		$data_table=$qz.$statr_table;
		$k=0;
		if($dosql->IsTable($data_table))
		{
			$sql = "select * from $data_table where `$fg` like '%$Acc%' ";
			//echo $sql;
			//exit;
			$dosql->ExecNoneQuery('SET NAMES Default');
			$dopage->GetPage($sql,50,'UNIX_TIMESTAMP(LogTime) DESC');
			while($data = $dosql->GetArray())
			{
				//摆摊玩家
				$row[$k]['UinA'] = $data['StallUin'];
				$row[$k]['LoginNameA'] = $data['OwnerLoginName'];
				$row[$k]['RoleNameA'] = iconv("gbk","utf8",$data['StallOwner']);
				//购买玩家
				$row[$k]['UinB'] = $data['Uin'];
				$row[$k]['LoginNameB'] = $data['BuyLoginName'];
				$row[$k]['RoleNameB'] = iconv("gbk","utf8",$data['RoleName']);				
				//记录时间
				$row[$k]['LogTime'] = $data['LogTime'];
				//价格
				$row[$k]['Price'] = $data['Price'];
				//购买的物品
				$buys = "";
				if($data['Type'] == 0)
				{
					$buys .="物品：";
					$x = $data['RoleGrid_DefID'];
					//邮寄物品
					$sql = "SELECT `Name` FROM ItemDef where `ItemID`='$x'";
					//echo $sql;
					mysql_select_db('resdb');
					$rest = mysql_query($sql);
					$rowf = mysql_fetch_array($rest);
					if($rowf != "" && $rowf != false)
					{
						$buys .= iconv("gbk","utf8",$rowf['Name']).'：';
					}
					$dosql->select_db($db);
					//物品唯一ID
					$buys .= $data['RoleGrid_WID'].'：';
					//邮寄物品数量
					$buys .= $data['RoleGrid_Num'].'：';
					//有效期
					if($data['RoleGrid_ValidEnd'] == 0)
					{
						$buys .= "永久";
					}
					else if($data['RoleGrid_ValidEnd'] != 0)
					{
						$buys .= date('Y-m-d H:i:s',$data['RoleGrid_ValidEnd']);
					}
				}
				else if($data['Type'] == 1)
				{
					$buys .="宠物：";
					$x = $data['RolePet_PetID'];
					//邮寄物品
					$sql = "SELECT `Name` FROM PetDef where `PetID`='$x'";
					//echo $sql;
					mysql_select_db('resdb');
					$rest = mysql_query($sql);
					$rowf = mysql_fetch_array($rest);
					if($rowf != "" && $rowf != false)
					{
						$buys .= iconv("gbk","utf8",$rowf['Name']).'：';
					}
					$dosql->select_db($db);
					//宠物ID
					$buys .= $data['RolePet_WID'].'：';
					//宠物数量
					$buys .= $data['ExchgIn_PetNum'].':';
					$buys .= "永久";
				}
				$row[$k]['buys'] = $buys;
				$k++;
			}
		}
		else if(!$dosql->IsTable($data_table))
		{
			echo json_encode("不存在数据表");
			exit;
		}
	}
	else
	{
		$fate=$end_table-$statr_table;
		$fag = $end_time-$statr_time;
		if("0"==$fate && $fag>=0)
		{
			$k=0;
			$data_table=$qz.$statr_table;
			if($dosql->IsTable($data_table))
			{			
				$sql = "select *  from $data_table where `$fg` like '%$Acc%'  and UNIX_TIMESTAMP(LogTime) >'$statr_time' and UNIX_TIMESTAMP(LogTime)<'$end_time' ";
				$dosql->ExecNoneQuery('SET NAMES Default');
				$dopage->GetPage($sql,50,'UNIX_TIMESTAMP(LogTime) DESC');
				while($data = $dosql->GetArray())
				{
					//摆摊玩家
					$row[$k]['UinA'] = $data['StallUin'];
					$row[$k]['LoginNameA'] = $data['OwnerLoginName'];
					$row[$k]['RoleNameA'] = iconv("gbk","utf8",$data['StallOwner']);
					//购买玩家
					$row[$k]['UinB'] = $data['Uin'];
					$row[$k]['LoginNameB'] = $data['BuyLoginName'];
					$row[$k]['RoleNameB'] = iconv("gbk","utf8",$data['RoleName']);				
					//记录时间
					$row[$k]['LogTime'] = $data['LogTime'];
					//价格
					$row[$k]['Price'] = $data['Price'];
					//购买的物品
					$buys = "";
					if($data['Type'] == 0)
					{
						$buys .="物品：";
						$x = $data['RoleGrid_DefID'];
						//邮寄物品
						$sql = "SELECT `Name` FROM ItemDef where `ItemID`='$x'";
						//echo $sql;
						mysql_select_db('resdb');
						$rest = mysql_query($sql);
						$rowf = mysql_fetch_array($rest);
						if($rowf != "" && $rowf != false)
						{
							$buys .= iconv("gbk","utf8",$rowf['Name']).'：';
						}
						$dosql->select_db($db);
						//物品唯一ID
						$buys .= $data['RoleGrid_WID'].'：';
						//邮寄物品数量
						$buys .= $data['RoleGrid_Num'].'：';
						//有效期
						if($data['RoleGrid_ValidEnd'] == 0)
						{
							$buys .= "永久";
						}
						else if($data['RoleGrid_ValidEnd'] != 0)
						{
							$buys .= date('Y-m-d H:i:s',$data['RoleGrid_ValidEnd']);
						}
					}
					else if($data['Type'] == 1)
					{
						$buys .="宠物：";
						$x = $data['RolePet_PetID'];
						//邮寄物品
						$sql = "SELECT `Name` FROM PetDef where `PetID`='$x'";
						//echo $sql;
						mysql_select_db('resdb');
						$rest = mysql_query($sql);
						$rowf = mysql_fetch_array($rest);
						if($rowf != "" && $rowf != false)
						{
							$buys .= iconv("gbk","utf8",$rowf['Name']).'：';
						}
						$dosql->select_db($db);
						//宠物ID
						$buys .= $data['RolePet_WID'].'：';
						//宠物数量
						$buys .= $data['ExchgIn_PetNum'].':';
						$buys .= "永久";
					}
					$row[$k]['buys'] = $buys;
					$k++;
				}
				$nums = count($row);		
			}
			else
			{
				echo json_encode("不存在数据表");
				exit;
			}
		}
		if(0< $fate && $fate < 3)
		{
			$k = 0;
			for($i=0;$i<=$fate;$i++)
			{
				$data_table[$i]=$qz.($statr_table+$i);
				if($dosql->IsTable($data_table[$i]))
				{
				
					if($i<$fate)
					{
						$sql = "select * from $data_table[$i] where `$fg` like '%$Acc%'  and UNIX_TIMESTAMP(LogTime) >'$statr_time' ";
					}
					if($i == $fate)
					{
						$sql = "select * from $data_table[$i] where `$fg` like '%$Acc%'  and UNIX_TIMESTAMP(LogTime) < '$end_time' ";
					}
					$dosql->ExecNoneQuery('SET NAMES Default');
					$dopage->GetPage($sql,50,'UNIX_TIMESTAMP(LogTime) DESC');
					while($data = $dosql->GetArray())
					{						
						//摆摊玩家
						$row[$k]['UinA'] = $data['StallUin'];
						$row[$k]['LoginNameA'] = $data['OwnerLoginName'];
						$row[$k]['RoleNameA'] = iconv("gbk","utf8",$data['StallOwner']);
						//购买玩家
						$row[$k]['UinB'] = $data['Uin'];
						$row[$k]['LoginNameB'] = $data['BuyLoginName'];
						$row[$k]['RoleNameB'] = iconv("gbk","utf8",$data['RoleName']);				
						//记录时间
						$row[$k]['LogTime'] = $data['LogTime'];
						//价格
						$row[$k]['Price'] = $data['Price'];
						//购买的物品
						$buys = "";
						if($data['Type'] == 0)
						{
							$buys .="物品：";
							$x = $data['RoleGrid_DefID'];
							//邮寄物品
							$sql = "SELECT `Name` FROM ItemDef where `ItemID`='$x'";
							//echo $sql;
							mysql_select_db('resdb');
							$rest = mysql_query($sql);
							$rowf = mysql_fetch_array($rest);
							if($rowf != "" && $rowf != false)
							{
								$buys .= iconv("gbk","utf8",$rowf['Name']).'：';
							}
							$dosql->select_db($db);
							//物品唯一ID
							$buys .= $data['RoleGrid_WID'].'：';
							//邮寄物品数量
							$buys .= $data['RoleGrid_Num'].'：';
							//有效期
							if($data['RoleGrid_ValidEnd'] == 0)
							{
								$buys .= "永久";
							}
							else if($data['RoleGrid_ValidEnd'] != 0)
							{
								$buys .= date('Y-m-d H:i:s',$data['RoleGrid_ValidEnd']);
							}
						}
						else if($data['Type'] == 1)
						{
							$buys .="宠物：";
							$x = $data['RolePet_PetID'];
							//邮寄物品
							$sql = "SELECT `Name` FROM PetDef where `PetID`='$x'";
							//echo $sql;
							mysql_select_db('resdb');
							$rest = mysql_query($sql);
							$rowf = mysql_fetch_array($rest);
							if($rowf != "" && $rowf != false)
							{
								$buys .= iconv("gbk","utf8",$rowf['Name']).'：';
							}
							$dosql->select_db($db);
							//宠物ID
							$buys .= $data['RolePet_WID'].'：';
							//宠物数量
							$buys .= $data['ExchgIn_PetNum'].':';
							$buys .= "永久";
						}
						$row[$k]['buys'] = $buys;
						$k++;
					}
				}
				else
				{
					continue;
				}
			}
		}
		if( $fate >= 3){
			echo json_encode("结束时间与开始时间相差不能超过3天");
			exit;
		}
		if(0>$fate)
		{
			echo json_encode("结束时间不能低于开始时间");
			exit;
		}
		if(0==$fate && $fag<0)
		{
			echo json_encode("结束时间不能低于开始时间");
			exit;
		}
	}
	//$row['nums'] = $nums;
	echo json_encode($row);
}

//坐骑操作
if(isset($_POST['RideChg']))
{
	$nums = 0;
	$row[]="";
	$db1 = $_POST['xzdb'];
	$fg = $_POST['fag'];
	if($fg == 1){ $fg = "RoleName";}
	else if($fg == 2){ $fg = "BuyLoginname";}
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
	
	$qz="LogRoleRideChg_";
	$Acc=$_POST['us'];
	$Acc = iconv("utf8","gbk",$Acc);
	
	date_default_timezone_set('PRC');
	//提交过来的开始时间的int
	$statr_time=strtotime($_POST['time']);

	//获取开始数据表
	$statr_table=date('ymd',$statr_time);
	
	//提交过来的结束时间
	$end_time=strtotime($_POST['time1']);
	
	//获取结束数据表
	$end_table=date('ymd',$end_time);
	
	
	//未填写结束时间
	if("" == $end_time || '0' == $end_time)
	{
		$data_table=$qz.$statr_table;
		$k=0;
		if($dosql->IsTable($data_table))
		{
			$sql = "select * from $data_table where `$fg` like '%$Acc%' ";
			
			$dosql->ExecNoneQuery('SET NAMES Default');
			$dopage->GetPage($sql,50,'UNIX_TIMESTAMP(LogTime) DESC');
			while($data = $dosql->GetArray())
			{
				$row[$k]['Uin'] = $data['Uin'];
				$row[$k]['LoginName'] = $data['LoginName'];
				$row[$k]['RoleName'] = iconv("gbk","utf8",$data['RoleName']);
				$row[$k]['RideID'] = $data['RideID']; //坐骑ID
				$row[$k]['Level'] = $data['RideLevel']; //坐骑等级
				//记录时间
				$row[$k]['LogTime'] = $data['LogTime'];
				//坐骑操作
				$row[$k]['OpType'] = $data['OpType'];
				if($data['OpType'] == 0)
				{
					$row[$k]['OpType'] = "获得坐骑";
				}
				else if($data['OpType'] == 1)
				{
					$row[$k]['OpType'] = "强化坐骑";
				}
				else if($data['OpType'] == 2)
				{
					$row[$k]['OpType'] = "丢弃坐骑";
				}
				$k++;
			}
		}
		else if(!$dosql->IsTable($data_table))
		{
			echo json_encode("不存在数据表");
			exit;
		}
	}
	else
	{
		$fate=$end_table-$statr_table;
		$fag = $end_time-$statr_time;
		if("0"==$fate && $fag>=0)
		{
			$k=0;
			$data_table=$qz.$statr_table;
			if($dosql->IsTable($data_table))
			{			
				$sql = "select *  from $data_table where `$fg` like '%$Acc%'  and UNIX_TIMESTAMP(LogTime) >'$statr_time' and UNIX_TIMESTAMP(LogTime)<'$end_time' ";
				$dosql->ExecNoneQuery('SET NAMES Default');
				$dopage->GetPage($sql,50,'UNIX_TIMESTAMP(LogTime) DESC');
				while($data = $dosql->GetArray())
				{
					$row[$k]['Uin'] = $data['Uin'];
					$row[$k]['LoginName'] = $data['LoginName'];
					$row[$k]['RoleName'] = iconv("gbk","utf8",$data['RoleName']);
					$row[$k]['RideID'] = $data['RideID']; //坐骑ID
					$row[$k]['Level'] = $data['RideLevel']; //坐骑等级
					//记录时间
					$row[$k]['LogTime'] = $data['LogTime'];
					//坐骑操作
					$row[$k]['OpType'] = $data['OpType'];
					if($data['OpType'] == 0)
					{
						$row[$k]['OpType'] = "获得坐骑";
					}
					else if($data['OpType'] == 1)
					{
						$row[$k]['OpType'] = "强化坐骑";
					}
					else if($data['OpType'] == 2)
					{
						$row[$k]['OpType'] = "丢弃坐骑";
					}
					$k++;
				}
				$nums = count($row);		
			}
			else
			{
				echo json_encode("不存在数据表");
				exit;
			}
		}
		if(0< $fate && $fate < 3)
		{
			$k = 0;
			for($i=0;$i<=$fate;$i++)
			{
				$data_table[$i]=$qz.($statr_table+$i);
				if($dosql->IsTable($data_table[$i]))
				{
				
					if($i<$fate)
					{
						$sql = "select * from $data_table[$i] where `$fg` like '%$Acc%'  and UNIX_TIMESTAMP(LogTime) >'$statr_time' ";
					}
					if($i == $fate)
					{
						$sql = "select * from $data_table[$i] where `$fg` like '%$Acc%'  and UNIX_TIMESTAMP(LogTime) < '$end_time' ";
					}
					$dosql->ExecNoneQuery('SET NAMES Default');
					$dopage->GetPage($sql,50,'UNIX_TIMESTAMP(LogTime) DESC');
					while($data = $dosql->GetArray())
					{						
						$row[$k]['Uin'] = $data['Uin'];
						$row[$k]['LoginName'] = $data['LoginName'];
						$row[$k]['RoleName'] = iconv("gbk","utf8",$data['RoleName']);
						$row[$k]['RideID'] = $data['RideID']; //坐骑ID
						$row[$k]['Level'] = $data['RideLevel']; //坐骑等级
						//记录时间
						$row[$k]['LogTime'] = $data['LogTime'];
						//坐骑操作
						$row[$k]['OpType'] = $data['OpType'];
						if($data['OpType'] == 0)
						{
							$row[$k]['OpType'] = "获得坐骑";
						}
						else if($data['OpType'] == 1)
						{
							$row[$k]['OpType'] = "强化坐骑";
						}
						else if($data['OpType'] == 2)
						{
							$row[$k]['OpType'] = "丢弃坐骑";
						}
						$k++;
					}
				}
				else
				{
					continue;
				}
			}
		}
		if( $fate >= 3){
			echo json_encode("结束时间与开始时间相差不能超过3天");
			exit;
		}
		if(0>$fate)
		{
			echo json_encode("结束时间不能低于开始时间");
			exit;
		}
		if(0==$fate && $fag<0)
		{
			echo json_encode("结束时间不能低于开始时间");
			exit;
		}
	}
	//$row['nums'] = $nums;
	echo json_encode($row);
}

//商城操作
if(isset($_POST['opBuyShop']))
{
	$nums = 0;
	$row[]="";
	$db1 = $_POST['xzdb'];
	$qq = $_POST['qq'];
	$fg = $_POST['fag'];
	if($fg == 1){ $fg = "RoleInfo_RoleName";}
	else if($fg == 2){ $fg = "Account";}
	$db="world_log_".$_POST['xzdb'];
	$resdb = "world_".$db1;
	
	//选择服务器和数据库
	$sql = "select * from `reg_world` where `dbid`='$db1' ";
	$dosql->Execute($sql);
	$rowdb = $dosql->GetArray();
	
	$GLOBALS['db_host'] = $rowdb['dbip'];
	$GLOBALS['db_user'] = $rowdb['dbuser'];
	$GLOBALS['db_name'] = $db;
	$GLOBALS['db_pwd'] = $rowdb['dbpass'];
	
	$sql2 = "select * from `shopworld` where `dbid`='$db1' ";
	$dosql->Execute($sql2);
	$rowdb2 = $dosql->GetArray();
	
	$shopip = $rowdb2['dbip'];
	$shopport = $rowdb2['dbport'];
	$shopuser = $rowdb2['dbuser'];
	$shoppass = $rowdb2['dbpass'];
	
	//连接shopdef(所在数据库)
	$link = mysql_connect($shopip,$shopuser,$shoppass);
	
	$dosql->Close(true);
	$dosql = new MySql(false);
	
	$qz="OpBuyShoppingChg_";
	$Acc=$_POST['us'];
	$Acc = iconv("utf8","gbk",$Acc);
	
	date_default_timezone_set('PRC');
	//提交过来的开始时间的int
	$statr_time=strtotime($_POST['time']);

	//获取开始数据表
	$statr_table=date('ymd',$statr_time);
	
	//提交过来的结束时间
	$end_time=strtotime($_POST['time1']);
	
	//获取结束数据表
	$end_table=date('ymd',$end_time);
	
	
	//未填写结束时间
	if("" == $end_time || '0' == $end_time)
	{
		$data_table=$qz.$statr_table;
		$k=0;
		if($dosql->IsTable($data_table))
		{
			$sql = "select * from $data_table where `$fg` like '%$Acc%' ";
			
			$dosql->ExecNoneQuery('SET NAMES Default');
			$dopage->GetPage($sql,50,'UNIX_TIMESTAMP(LogTime) DESC');
			while($data = $dosql->GetArray())
			{
				$row[$k]['LoginName'] = $data['Account'];
				$row[$k]['RoleName'] = iconv("gbk","utf8",$data['RoleInfo_RoleName']);
				$row[$k]['ShopNum'] = $data['ShopNum']; //购买数量
				$row[$k]['MoneyNum'] = $data['MoneyNum']; //总价
				$row[$k]['GiftFlag'] = $data['GiftFlag']; //赠送标志
				$row[$k]['GiftToRoleID'] = $data['GiftToRoleID']; //赠送的角色ID
				if($data['GiftFlag'] == 0)
				{
					$row[$k]['GiftFlag'] = "否";
				}
				else if($data['GiftFlag'] == 1)
				{
					$row[$k]['GiftFlag'] = "是";
				}
				//记录时间
				$row[$k]['LogTime'] = $data['LogTime'];
				//坐骑操作
				$row[$k]['OpType'] = $data['Operate'];
				if($data['Operate'] == 0)
				{
					$row[$k]['OpType'] = "金币购买";
				}
				else if($data['Operate'] == 1)
				{
					$row[$k]['OpType'] = "金卷购买";
				}
				$row[$k]['Shop'] = $data['ShopID']; //物品ID
				//物品
				$sql = "SELECT `Name` FROM ShopDef where `ShopID`='$data[ShopID]'";
				mysql_select_db($resdb,$link);
				$rest = mysql_query($sql,$link);
				$rowf = mysql_fetch_array($rest);
				if($rowf != "" && $rowf != false)
				{
					$row[$k]['Shop'] = iconv("gbk","utf8",$rowf['Name']);
				}
				$dosql->select_db($db);
				
				$k++;
			}
		}
		else if(!$dosql->IsTable($data_table))
		{
			echo json_encode("不存在数据表");
			exit;
		}
	}
	else
	{
		$fate=$end_table-$statr_table;
		$fag = $end_time-$statr_time;
		if("0"==$fate && $fag>=0)
		{
			$k=0;
			$data_table=$qz.$statr_table;
			if($dosql->IsTable($data_table))
			{			
				$sql = "select *  from $data_table where `$fg` like '%$Acc%'  and UNIX_TIMESTAMP(LogTime) >'$statr_time' and UNIX_TIMESTAMP(LogTime)<'$end_time' ";
				$dosql->ExecNoneQuery('SET NAMES Default');
				$dopage->GetPage($sql,50,'UNIX_TIMESTAMP(LogTime) DESC');
				while($data = $dosql->GetArray())
				{
					$row[$k]['LoginName'] = $data['Account'];
					$row[$k]['RoleName'] = iconv("gbk","utf8",$data['RoleInfo_RoleName']);
					$row[$k]['ShopNum'] = $data['ShopNum']; //购买数量
					$row[$k]['MoneyNum'] = $data['MoneyNum']; //总价
					$row[$k]['GiftFlag'] = $data['GiftFlag']; //赠送标志
					$row[$k]['GiftToRoleID'] = $data['GiftToRoleID']; //赠送的角色ID
					if($data['GiftFlag'] == 0)
					{
						$row[$k]['GiftFlag'] = "否";
					}
					else if($data['GiftFlag'] == 1)
					{
						$row[$k]['GiftFlag'] = "是";
					}
					//记录时间
					$row[$k]['LogTime'] = $data['LogTime'];
					//坐骑操作
					$row[$k]['OpType'] = $data['Operate'];
					if($data['Operate'] == 0)
					{
						$row[$k]['OpType'] = "金币购买";
					}
					else if($data['Operate'] == 1)
					{
						$row[$k]['OpType'] = "金卷购买";
					}
					$row[$k]['Shop'] = $data['ShopID']; //物品ID
					//物品
					$sql = "SELECT `Name` FROM ShopDef where `ShopID`='$data[ShopID]'";
					mysql_select_db($resdb,$link);
					$rest = mysql_query($sql,$link);
					$rowf = mysql_fetch_array($rest);
					if($rowf != "" && $rowf != false)
					{
						$row[$k]['Shop'] = iconv("gbk","utf8",$rowf['Name']);
					}
					$dosql->select_db($db);
					$k++;
				}
				$nums = count($row);		
			}
			else
			{
				echo json_encode("不存在数据表");
				exit;
			}
		}
		if(0< $fate && $fate < 3)
		{
			$k = 0;
			for($i=0;$i<=$fate;$i++)
			{
				$data_table[$i]=$qz.($statr_table+$i);
				if($dosql->IsTable($data_table[$i]))
				{
				
					if($i<$fate)
					{
						$sql = "select * from $data_table[$i] where `$fg` like '%$Acc%'  and UNIX_TIMESTAMP(LogTime) >'$statr_time' ";
					}
					if($i == $fate)
					{
						$sql = "select * from $data_table[$i] where `$fg` like '%$Acc%'  and UNIX_TIMESTAMP(LogTime) < '$end_time' ";
					}
					$dosql->ExecNoneQuery('SET NAMES Default');
					$dopage->GetPage($sql,50,'UNIX_TIMESTAMP(LogTime) DESC');
					while($data = $dosql->GetArray())
					{						
						$row[$k]['LoginName'] = $data['Account'];
						$row[$k]['RoleName'] = iconv("gbk","utf8",$data['RoleInfo_RoleName']);
						$row[$k]['ShopNum'] = $data['ShopNum']; //购买数量
						$row[$k]['MoneyNum'] = $data['MoneyNum']; //总价
						$row[$k]['GiftFlag'] = $data['GiftFlag']; //赠送标志
						$row[$k]['GiftToRoleID'] = $data['GiftToRoleID']; //赠送的角色ID
						if($data['GiftFlag'] == 0)
						{
							$row[$k]['GiftFlag'] = "否";
						}
						else if($data['GiftFlag'] == 1)
						{
							$row[$k]['GiftFlag'] = "是";
						}
						//记录时间
						$row[$k]['LogTime'] = $data['LogTime'];
						//坐骑操作
						$row[$k]['OpType'] = $data['Operate'];
						if($data['Operate'] == 0)
						{
							$row[$k]['OpType'] = "金币购买";
						}
						else if($data['Operate'] == 1)
						{
							$row[$k]['OpType'] = "金卷购买";
						}
						$row[$k]['Shop'] = $data['ShopID']; //物品ID
						$sql = "SELECT `Name` FROM ShopDef where `ShopID`='$data[ShopID]'";
						mysql_select_db($resdb,$link);
						$rest = mysql_query($sql,$link);
						$rowf = mysql_fetch_array($rest);
						if($rowf != "" && $rowf != false)
						{
							$row[$k]['Shop'] = iconv("gbk","utf8",$rowf['Name']);
						}
						$dosql->select_db($db);
						$k++;
					}
				}
				else
				{
					continue;
				}
			}
		}
		if( $fate >= 3){
			echo json_encode("结束时间与开始时间相差不能超过3天");
			exit;
		}
		if(0>$fate)
		{
			echo json_encode("结束时间不能低于开始时间");
			exit;
		}
		if(0==$fate && $fag<0)
		{
			echo json_encode("结束时间不能低于开始时间");
			exit;
		}
	}
	//$row['nums'] = $nums;
	echo json_encode($row);
}
//晶石操作
if(isset($_POST['OpSpar']))
{
	$nums = 0;
	$row[]="";
	$db1 = $_POST['xzdb'];
	$qq = $_POST['qq'];
	$fg = $_POST['fag'];
	if($fg == 1){ $fg = "RoleInfo_RoleName";}
	else if($fg == 2){ $fg = "Pass9Account";}
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
	
	$qz="OpSpar_";
	$Acc=$_POST['us'];
	$Acc = iconv("utf8","gbk",$Acc);
	
	date_default_timezone_set('PRC');
	//提交过来的开始时间的int
	$statr_time=strtotime($_POST['time']);

	//获取开始数据表
	$statr_table=date('ymd',$statr_time);
	
	//提交过来的结束时间
	$end_time=strtotime($_POST['time1']);
	
	//获取结束数据表
	$end_table=date('ymd',$end_time);
	
	
	//未填写结束时间
	if("" == $end_time || '0' == $end_time)
	{
		$data_table=$qz.$statr_table;
		$k=0;
		if($dosql->IsTable($data_table))
		{
			$sql = "select * from $data_table where `$fg` like '%$Acc%' ";
			
			$dosql->ExecNoneQuery('SET NAMES Default');
			$dopage->GetPage($sql,50,'UNIX_TIMESTAMP(LogTime) DESC');
			while($data = $dosql->GetArray())
			{
				$row[$k]['Uin'] = $data['Uin'];
				$row[$k]['LoginName'] = $data['Pass9Account'];
				$row[$k]['RoleName'] = iconv("gbk","utf8",$data['RoleInfo_RoleName']);
				$row[$k]['ItemNum'] = $data['ItemDec_Num']; //物品数量
				$row[$k]['ItemBindNum'] = $data['ItemDec_BindNum']; //物品绑定数
				//记录时间
				$row[$k]['LogTime'] = $data['LogTime'];
				//坐骑操作
				$row[$k]['OpType'] = $data['Op'];
				if($data['Op'] == 1)
				{
					$row[$k]['OpType'] = "替换";
				}
				else if($data['Op'] == 2)
				{
					$row[$k]['OpType'] = "取下";
				}
				else if($data['Op'] == 3)
				{
					$row[$k]['OpType'] = "镶嵌";
				}
				else if($data['Op'] == 4)
				{
					$row[$k]['OpType'] = "背包道具吸收";
				}
				else if($data['Op'] == 5)
				{
					$row[$k]['OpType'] = "身上晶石吸收";
				}
				$row[$k]['SparID'] = $data['OldSpar_SparID'];  //操作前晶石ID
				$row[$k]['SparLevel'] = $data['OldSpar_SparLevel'];  //等级
				$row[$k]['SparExp'] = $data['OldSpar_SparExp'];  //经验
				$row[$k]['NewSparID'] = $data['NewSpar_SparID']; //操作后晶石ID
				$row[$k]['NewSparLevel'] = $data['NewSpar_SparLevel'];  //等级
				$row[$k]['NewSparExp'] = $data['NewSpar_SparExp'];  //经验
				$row[$k]['num'] = $data['Expend_StuffNum'];
				$row[$k]['money'] = $data['Expend_Bull']."/".$data['Expend_Money'];
				$spars = "";
				for($i=1;$i<=$data['Expend_StuffNum'];$i++)
				{
					$m = "Expend_StuffInfo".$i."_ID";
					$sql = "SELECT `Name` FROM ItemDef where `ItemID`='$data[$m]'";
					mysql_select_db('resdb');
					$rest = mysql_query($sql);
					$rowf = mysql_fetch_array($rest);
					if($rowf != "" && $rowf != false)
					{
						$spars .= iconv("gbk","utf8",$rowf['Name']);
					}
					$spars .= "：".$data['Expend_StuffInfo'.$i.'_Num']." ， ";
				}
				//$row[$k]['']
				$row[$k]['spars'] = $spars;
				
				$dosql->select_db($db);
				
				$k++;
			}
		}
		else if(!$dosql->IsTable($data_table))
		{
			echo json_encode("不存在数据表");
			exit;
		}
	}
	else
	{
		$fate=$end_table-$statr_table;
		$fag = $end_time-$statr_time;
		if("0"==$fate && $fag>=0)
		{
			$k=0;
			$data_table=$qz.$statr_table;
			if($dosql->IsTable($data_table))
			{			
				$sql = "select *  from $data_table where `$fg` like '%$Acc%'  and UNIX_TIMESTAMP(LogTime) >'$statr_time' and UNIX_TIMESTAMP(LogTime)<'$end_time' ";
				$dosql->ExecNoneQuery('SET NAMES Default');
				$dopage->GetPage($sql,50,'UNIX_TIMESTAMP(LogTime) DESC');
				while($data = $dosql->GetArray())
				{
					$row[$k]['Uin'] = $data['Uin'];
					$row[$k]['LoginName'] = $data['Pass9Account'];
					$row[$k]['RoleName'] = iconv("gbk","utf8",$data['RoleInfo_RoleName']);
					$row[$k]['ItemNum'] = $data['ItemDec_Num']; //物品数量
					$row[$k]['ItemBindNum'] = $data['ItemDec_BindNum']; //物品绑定数
					//记录时间
					$row[$k]['LogTime'] = $data['LogTime'];
					//坐骑操作
					$row[$k]['OpType'] = $data['Op'];
					if($data['Op'] == 1)
					{
						$row[$k]['OpType'] = "替换";
					}
					else if($data['Op'] == 2)
					{
						$row[$k]['OpType'] = "取下";
					}
					else if($data['Op'] == 3)
					{
						$row[$k]['OpType'] = "镶嵌";
					}
					else if($data['Op'] == 4)
					{
						$row[$k]['OpType'] = "背包道具吸收";
					}
					else if($data['Op'] == 5)
					{
						$row[$k]['OpType'] = "身上晶石吸收";
					}
					$row[$k]['SparID'] = $data['OldSpar_SparID'];  //操作前晶石ID
					$row[$k]['SparLevel'] = $data['OldSpar_SparLevel'];  //等级
					$row[$k]['SparExp'] = $data['OldSpar_SparExp'];  //经验
					$row[$k]['NewSparID'] = $data['NewSpar_SparID']; //操作后晶石ID
					$row[$k]['NewSparLevel'] = $data['NewSpar_SparLevel'];  //等级
					$row[$k]['NewSparExp'] = $data['NewSpar_SparExp'];  //经验
					$row[$k]['num'] = $data['Expend_StuffNum'];
					$row[$k]['money'] = $data['Expend_Bull']."/".$data['Expend_Money'];
					$spars = "";
					for($i=1;$i<=$data['Expend_StuffNum'];$i++)
					{
						$m = "Expend_StuffInfo".$i."_ID";
						$sql = "SELECT `Name` FROM ItemDef where `ItemID`='$data[$m]'";
						mysql_select_db('resdb');
						$rest = mysql_query($sql);
						$rowf = mysql_fetch_array($rest);
						if($rowf != "" && $rowf != false)
						{
							$spars .= iconv("gbk","utf8",$rowf['Name']);
						}
						$spars .= "：".$data['Expend_StuffInfo'.$i.'_Num']."<br/>";
					}
					$row[$k]['spars'] = $spars;
					$dosql->select_db($db);
					$k++;
				}
				$nums = count($row);		
			}
			else
			{
				echo json_encode("不存在数据表");
				exit;
			}
		}
		if(0< $fate && $fate < 3)
		{
			$k = 0;
			for($i=0;$i<=$fate;$i++)
			{
				$data_table[$i]=$qz.($statr_table+$i);
				if($dosql->IsTable($data_table[$i]))
				{
				
					if($i<$fate)
					{
						$sql = "select * from $data_table[$i] where `$fg` like '%$Acc%'  and UNIX_TIMESTAMP(LogTime) >'$statr_time' ";
					}
					if($i == $fate)
					{
						$sql = "select * from $data_table[$i] where `$fg` like '%$Acc%'  and UNIX_TIMESTAMP(LogTime) < '$end_time' ";
					}
					$dosql->ExecNoneQuery('SET NAMES Default');
					$dopage->GetPage($sql,50,'UNIX_TIMESTAMP(LogTime) DESC');
					while($data = $dosql->GetArray())
					{						
						$row[$k]['Uin'] = $data['Uin'];
						$row[$k]['LoginName'] = $data['Pass9Account'];
						$row[$k]['RoleName'] = iconv("gbk","utf8",$data['RoleInfo_RoleName']);
						$row[$k]['ItemNum'] = $data['ItemDec_Num']; //物品数量
						$row[$k]['ItemBindNum'] = $data['ItemDec_BindNum']; //物品绑定数
						//记录时间
						$row[$k]['LogTime'] = $data['LogTime'];
						//坐骑操作
						$row[$k]['OpType'] = $data['Op'];
						if($data['Op'] == 1)
						{
							$row[$k]['OpType'] = "替换";
						}
						else if($data['Op'] == 2)
						{
							$row[$k]['OpType'] = "取下";
						}
						else if($data['Op'] == 3)
						{
							$row[$k]['OpType'] = "镶嵌";
						}
						else if($data['Op'] == 4)
						{
							$row[$k]['OpType'] = "背包道具吸收";
						}
						else if($data['Op'] == 5)
						{
							$row[$k]['OpType'] = "身上晶石吸收";
						}
						$row[$k]['SparID'] = $data['OldSpar_SparID'];  //操作前晶石ID
						$row[$k]['SparLevel'] = $data['OldSpar_SparLevel'];  //等级
						$row[$k]['SparExp'] = $data['OldSpar_SparExp'];  //经验
						$row[$k]['NewSparID'] = $data['NewSpar_SparID']; //操作后晶石ID
						$row[$k]['NewSparLevel'] = $data['NewSpar_SparLevel'];  //等级
						$row[$k]['NewSparExp'] = $data['NewSpar_SparExp'];  //经验
						$row[$k]['num'] = $data['Expend_StuffNum'];
						$row[$k]['money'] = $data['Expend_Bull']."/".$data['Expend_Money'];
						$spars = "";
						for($i=1;$i<=$data['Expend_StuffNum'];$i++)
						{
							$m = "Expend_StuffInfo".$i."_ID";
							$sql = "SELECT `Name` FROM ItemDef where `ItemID`='$data[$m]'";
							mysql_select_db('resdb');
							$rest = mysql_query($sql);
							$rowf = mysql_fetch_array($rest);
							if($rowf != "" && $rowf != false)
							{
								$spars .= iconv("gbk","utf8",$rowf['Name']);
							}
							$spars .= "：".$data['Expend_StuffInfo'.$i.'_Num']."<br/>";
						}
						$row[$k]['spars'] = $spars;
						$dosql->select_db($db);
						$k++;
					}
				}
				else
				{
					continue;
				}
			}
		}
		if( $fate >= 3){
			echo json_encode("结束时间与开始时间相差不能超过3天");
			exit;
		}
		if(0>$fate)
		{
			echo json_encode("结束时间不能低于开始时间");
			exit;
		}
		if(0==$fate && $fag<0)
		{
			echo json_encode("结束时间不能低于开始时间");
			exit;
		}
	}
	//$row['nums'] = $nums;
	echo json_encode($row);
}

//消耗道具
if(isset($_POST['OpRoleItemDec']))
{
	$nums = 0;
	$row[]="";
	$db1 = $_POST['xzdb'];
	$qq = $_POST['qq'];
	$fg = $_POST['fag'];
	if($fg == 1){ $fg = "RoleInfo_RoleName";}
	else if($fg == 2){ $fg = "Pass9Account";}
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
	
	$qz="OpRoleItemDec_";
	$Acc=$_POST['us'];
	$Acc = iconv("utf8","gbk",$Acc);
	
	date_default_timezone_set('PRC');
	//提交过来的开始时间的int
	$statr_time=strtotime($_POST['time']);

	//获取开始数据表
	$statr_table=date('ymd',$statr_time);
	
	//提交过来的结束时间
	$end_time=strtotime($_POST['time1']);
	
	//获取结束数据表
	$end_table=date('ymd',$end_time);
	
	
	//未填写结束时间
	if("" == $end_time || '0' == $end_time)
	{
		$data_table=$qz.$statr_table;
		$k=0;
		if($dosql->IsTable($data_table))
		{
			$sql = "select * from $data_table where `$fg` like '%$Acc%' ";
			
			$dosql->ExecNoneQuery('SET NAMES Default');
			$dopage->GetPage($sql,50,'UNIX_TIMESTAMP(LogTime) DESC');
			while($data = $dosql->GetArray())
			{
				$row[$k]['Uin'] = $data['Uin'];
				$row[$k]['LoginName'] = $data['Pass9Account'];
				$row[$k]['RoleName'] = iconv("gbk","utf8",$data['RoleInfo_RoleName']);
				$row[$k]['ItemNum'] = $data['ItemDec_Num']; //物品数量
				$row[$k]['ItemBindNum'] = $data['ItemDec_BindNum']; //物品绑定数
				//记录时间
				$row[$k]['LogTime'] = $data['LogTime'];
				//坐骑操作
				$row[$k]['OpType'] = $data['OpType'];
				if($data['OpType'] == 0)
				{
					$row[$k]['OpType'] = "使用消耗";
				}
				else if($data['OpType'] == 1)
				{
					$row[$k]['OpType'] = "脚本删除";
				}
				else if($data['OpType'] == 2)
				{
					$row[$k]['OpType'] = "使用消耗1次";
				}
				$row[$k]['Item'] = $data['ItemDec_ID']; //赠送的角色ID
				$sql = "SELECT `Name` FROM ItemDef where `ItemID`='$data[ItemDec_ID]'";
				mysql_select_db('resdb');
				$rest = mysql_query($sql);
				$rowf = mysql_fetch_array($rest);
				if($rowf != "" && $rowf != false)
				{
					$row[$k]['Item'] = iconv("gbk","utf8",$rowf['Name']);
				}
				$dosql->select_db($db);
				
				$k++;
			}
		}
		else if(!$dosql->IsTable($data_table))
		{
			echo json_encode("不存在数据表");
			exit;
		}
	}
	else
	{
		$fate=$end_table-$statr_table;
		$fag = $end_time-$statr_time;
		if("0"==$fate && $fag>=0)
		{
			$k=0;
			$data_table=$qz.$statr_table;
			if($dosql->IsTable($data_table))
			{			
				$sql = "select *  from $data_table where `$fg` like '%$Acc%'  and UNIX_TIMESTAMP(LogTime) >'$statr_time' and UNIX_TIMESTAMP(LogTime)<'$end_time' ";
				$dosql->ExecNoneQuery('SET NAMES Default');
				$dopage->GetPage($sql,50,'UNIX_TIMESTAMP(LogTime) DESC');
				while($data = $dosql->GetArray())
				{
					$row[$k]['Uin'] = $data['Uin'];
					$row[$k]['LoginName'] = $data['Pass9Account'];
					$row[$k]['RoleName'] = iconv("gbk","utf8",$data['RoleInfo_RoleName']);
					$row[$k]['ItemNum'] = $data['ItemDec_Num']; //物品数量
					$row[$k]['ItemBindNum'] = $data['ItemDec_BindNum']; //物品绑定数
					//记录时间
					$row[$k]['LogTime'] = $data['LogTime'];
					//坐骑操作
					$row[$k]['OpType'] = $data['OpType'];
					if($data['OpType'] == 0)
					{
						$row[$k]['OpType'] = "使用消耗";
					}
					else if($data['OpType'] == 1)
					{
						$row[$k]['OpType'] = "脚本删除";
					}
					else if($data['OpType'] == 2)
					{
						$row[$k]['OpType'] = "使用消耗1次";
					}
					$row[$k]['Item'] = $data['ItemDec_ID']; //赠送的角色ID
					$sql = "SELECT `Name` FROM ItemDef where `ItemID`='$data[ItemDec_ID]'";
					mysql_select_db('resdb');
					$rest = mysql_query($sql);
					$rowf = mysql_fetch_array($rest);
					if($rowf != "" && $rowf != false)
					{
						$row[$k]['Item'] = iconv("gbk","utf8",$rowf['Name']);
					}
					$dosql->select_db($db);
					$k++;
				}
				$nums = count($row);		
			}
			else
			{
				echo json_encode("不存在数据表");
				exit;
			}
		}
		if(0< $fate && $fate < 3)
		{
			$k = 0;
			for($i=0;$i<=$fate;$i++)
			{
				$data_table[$i]=$qz.($statr_table+$i);
				if($dosql->IsTable($data_table[$i]))
				{
				
					if($i<$fate)
					{
						$sql = "select * from $data_table[$i] where `$fg` like '%$Acc%'  and UNIX_TIMESTAMP(LogTime) >'$statr_time' ";
					}
					if($i == $fate)
					{
						$sql = "select * from $data_table[$i] where `$fg` like '%$Acc%'  and UNIX_TIMESTAMP(LogTime) < '$end_time' ";
					}
					$dosql->ExecNoneQuery('SET NAMES Default');
					$dopage->GetPage($sql,50,'UNIX_TIMESTAMP(LogTime) DESC');
					while($data = $dosql->GetArray())
					{						
						$row[$k]['Uin'] = $data['Uin'];
						$row[$k]['LoginName'] = $data['Pass9Account'];
						$row[$k]['RoleName'] = iconv("gbk","utf8",$data['RoleInfo_RoleName']);
						$row[$k]['ItemNum'] = $data['ItemDec_Num']; //物品数量
						$row[$k]['ItemBindNum'] = $data['ItemDec_BindNum']; //物品绑定数
						//记录时间
						$row[$k]['LogTime'] = $data['LogTime'];
						//坐骑操作
						$row[$k]['OpType'] = $data['OpType'];
						if($data['OpType'] == 0)
						{
							$row[$k]['OpType'] = "使用消耗";
						}
						else if($data['OpType'] == 1)
						{
							$row[$k]['OpType'] = "脚本删除";
						}
						else if($data['OpType'] == 2)
						{
							$row[$k]['OpType'] = "使用消耗1次";
						}
						$row[$k]['Item'] = $data['ItemDec_ID']; //赠送的角色ID
						$sql = "SELECT `Name` FROM ItemDef where `ItemID`='$data[ItemDec_ID]'";
						mysql_select_db('resdb');
						$rest = mysql_query($sql);
						$rowf = mysql_fetch_array($rest);
						if($rowf != "" && $rowf != false)
						{
							$row[$k]['Item'] = iconv("gbk","utf8",$rowf['Name']);
						}
						$dosql->select_db($db);
						$k++;
					}
				}
				else
				{
					continue;
				}
			}
		}
		if( $fate >= 3){
			echo json_encode("结束时间与开始时间相差不能超过3天");
			exit;
		}
		if(0>$fate)
		{
			echo json_encode("结束时间不能低于开始时间");
			exit;
		}
		if(0==$fate && $fag<0)
		{
			echo json_encode("结束时间不能低于开始时间");
			exit;
		}
	}
	//$row['nums'] = $nums;
	echo json_encode($row);
}

//道具丢弃
if(isset($_POST['LogRoleItemDrop']))
{
	$nums = 0;
	$row[]="";
	$db1 = $_POST['xzdb'];
	$qq = $_POST['qq'];
	$fg = $_POST['fag'];
	if($fg == 1){ $fg = "RoleInfo_RoleName";}
	else if($fg == 2){ $fg = "Pass9Account";}
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
	
	$qz="LogRoleItemDrop_";
	$Acc=$_POST['us'];
	$Acc = iconv("utf8","gbk",$Acc);
	
	date_default_timezone_set('PRC');
	//提交过来的开始时间的int
	$statr_time=strtotime($_POST['time']);

	//获取开始数据表
	$statr_table=date('ymd',$statr_time);
	
	//提交过来的结束时间
	$end_time=strtotime($_POST['time1']);
	
	//获取结束数据表
	$end_table=date('ymd',$end_time);
	
	
	//未填写结束时间
	if("" == $end_time || '0' == $end_time)
	{
		$data_table=$qz.$statr_table;
		$k=0;
		if($dosql->IsTable($data_table))
		{
			$sql = "select * from $data_table where `$fg` like '%$Acc%' ";
			
			$dosql->ExecNoneQuery('SET NAMES Default');
			$dopage->GetPage($sql,50,'UNIX_TIMESTAMP(LogTime) DESC');
			while($data = $dosql->GetArray())
			{
				$row[$k]['Uin'] = $data['Uin'];
				$row[$k]['LoginName'] = $data['Pass9Account'];
				$row[$k]['RoleName'] = iconv("gbk","utf8",$data['RoleInfo_RoleName']);
				$row[$k]['ItemNum'] = $data['GridData_Num']; //物品丢弃数量
				$row[$k]['ItemID'] = $data['GridData_WID']; //物品唯一ID
				$row[$k]['ValidEnd'] = $data['GridData_ValidEnd']; //有效期
				$row[$k]['qhlv'] = $data['GridData_StarLv'];//强化★级
				$row[$k]['pz'] = "无";
				if($data['GridData_ValidEnd'] == 0)
				{
					$row[$k]['ValidEnd'] = "永久";
				}
				else if($data['GridData_ValidEnd'] != 0)
				{
					$row[$k]['ValidEnd'] = date('Y-m-d H:i:s',$data['GridData_ValidEnd']);
				}
				//记录时间
				$row[$k]['LogTime'] = $data['LogTime'];
				
				$row[$k]['Item'] = $data['GridData_DefID']; 
				$sql = "SELECT `Name`,`Important` FROM ItemDef where `ItemID`='$data[GridData_DefID]'";
				mysql_select_db('resdb');
				$rest = mysql_query($sql);
				$rowf = mysql_fetch_array($rest);
				if($rowf != "" && $rowf != false)
				{
					$row[$k]['Item'] = iconv("gbk","utf8",$rowf['Name']);
				}
				
				if(0==$row[$k]['qhlv'])
				{
					$row[$k]['qhlv'] = " ";
				}
				else
				{
					$row[$k]['qhlv'] = "(".$data['GridData_StarLv']."星：";
					for($ij=0;$ij<$data['GridData_StarLv'];$ij++)
					{
							$ss = "GridData_StarInfo".($ij+1)."_ValIdx";
							$zdj = $data[$ss];
							$zd = "StarInfo".($zdj+1)."_Mul";
							$sqls = "SELECT `$zd` FROM `ArmStarDef` where `Star`='$data[GridData_StarLv]'";
							$xj = mysql_query($sqls);
							$xjs = mysql_fetch_array($xj);
							$row[$k]['qhlv'] .= $xjs[$zd].",";
					}
					$row[$k]['qhlv'] .= ")";
				}								
				
				$dosql->select_db($db);
				//类型
				$row[$k]['OpType'] = $data['GridData_Type'];
				if($data['GridData_Type'] == 0)
				{
					$row[$k]['OpType'] = "道具";
					$row[$k]['pz'] = "";
				}
				else if($data['GridData_Type'] == 1)
				{
					$row[$k]['OpType'] = "装备";
					$row[$k]['pz'] = $rowf['Important'];
					if(4 <= $row[$k]['pz'])
					{
						$row[$k]['pz'] = "粉装";
					}
					else if(3 == $row[$k]['pz'])
					{
						$row[$k]['pz'] = "紫装";
					}
					else if(2 == $row[$k]['pz'])
					{
						$row[$k]['pz'] = "蓝装";
					}
					else if(1 == $row[$k]['pz'])
					{
						$row[$k]['pz'] = "绿装";
					}
					else if(0 == $row[$k]['pz'])
					{
						$row[$k]['pz'] = "白装";
					}
				}				
				$k++;
			}
		}
		else if(!$dosql->IsTable($data_table))
		{
			echo json_encode("不存在数据表");
			exit;
		}
	}
	else
	{
		$fate=$end_table-$statr_table;
		$fag = $end_time-$statr_time;
		if("0"==$fate && $fag>=0)
		{
			$k=0;
			$data_table=$qz.$statr_table;
			if($dosql->IsTable($data_table))
			{			
				$sql = "select *  from $data_table where `$fg` like '%$Acc%'  and UNIX_TIMESTAMP(LogTime) >'$statr_time' and UNIX_TIMESTAMP(LogTime)<'$end_time' ";
				$dosql->ExecNoneQuery('SET NAMES Default');
				$dopage->GetPage($sql,50,'UNIX_TIMESTAMP(LogTime) DESC');
				while($data = $dosql->GetArray())
				{
					$row[$k]['Uin'] = $data['Uin'];
					$row[$k]['LoginName'] = $data['Pass9Account'];
					$row[$k]['RoleName'] = iconv("gbk","utf8",$data['RoleInfo_RoleName']);
					$row[$k]['ItemNum'] = $data['GridData_Num']; //物品丢弃数量
					$row[$k]['ItemID'] = $data['GridData_WID']; //物品唯一ID
					$row[$k]['ValidEnd'] = $data['GridData_ValidEnd']; //有效期
					$row[$k]['qhlv'] = $data['GridData_StarLv'];//强化★级
					$row[$k]['pz'] = "无";
					if($data['GridData_ValidEnd'] == 0)
					{
						$row[$k]['ValidEnd'] = "永久";
					}
					else if($data['GridData_ValidEnd'] != 0)
					{
						$row[$k]['ValidEnd'] = date('Y-m-d H:i:s',$data['GridData_ValidEnd']);
					}
					//记录时间
					$row[$k]['LogTime'] = $data['LogTime'];
					
					$row[$k]['Item'] = $data['GridData_DefID'];
					$sql = "SELECT `Name`,`Important` FROM ItemDef where `ItemID`='$data[GridData_DefID]'";
					mysql_select_db('resdb');
					$rest = mysql_query($sql);
					$rowf = mysql_fetch_array($rest);
					if($rowf != "" && $rowf != false)
					{
						$row[$k]['Item'] = iconv("gbk","utf8",$rowf['Name']);
					}
					
					if(0==$row[$k]['qhlv'])
					{
						$row[$k]['qhlv'] = " ";
					}
					else
					{
						$row[$k]['qhlv'] = "(".$data['GridData_StarLv']."星：";
						for($ij=0;$ij<$data['GridData_StarLv'];$ij++)
						{
							$ss = "GridData_StarInfo".($ij+1)."_ValIdx";
							$zdj = $data[$ss];
							$zd = "StarInfo".($zdj+1)."_Mul";
							$sqls = "SELECT `$zd` FROM `ArmStarDef` where `Star`='$data[GridData_StarLv]'";
							$xj = mysql_query($sqls);
							$xjs = mysql_fetch_array($xj);
							$row[$k]['qhlv'] .= $xjs[$zd].",";
						}
							$row[$k]['qhlv'] .= ")";
					}
					
					$dosql->select_db($db);
					
					//坐骑操作
					$row[$k]['OpType'] = $data['GridData_Type'];
					if($data['GridData_Type'] == 0)
					{
						$row[$k]['OpType'] = "道具";
					}
					else if($data['GridData_Type'] == 1)
					{
						$row[$k]['OpType'] = "装备";
						$row[$k]['pz'] = $rowf['Important'];
						if(4 <= $row[$k]['pz'])
						{
							$row[$k]['pz'] = "粉装";
						}
						else if(3 == $row[$k]['pz'])
						{
							$row[$k]['pz'] = "紫装";
						}
						else if(2 == $row[$k]['pz'])
						{
							$row[$k]['pz'] = "蓝装";
						}
						else if(1 == $row[$k]['pz'])
						{
							$row[$k]['pz'] = "绿装";
						}
						else if(0 == $row[$k]['pz'])
						{
							$row[$k]['pz'] = "白装";
						}
					}
					$k++;
				}
				$nums = count($row);		
			}
			else
			{
				echo json_encode("不存在数据表");
				exit;
			}
		}
		if(0< $fate && $fate < 3)
		{
			$k = 0;
			for($i=0;$i<=$fate;$i++)
			{
				$data_table[$i]=$qz.($statr_table+$i);
				if($dosql->IsTable($data_table[$i]))
				{
				
					if($i<$fate)
					{
						$sql = "select * from $data_table[$i] where `$fg` like '%$Acc%'  and UNIX_TIMESTAMP(LogTime) >'$statr_time' ";
					}
					if($i == $fate)
					{
						$sql = "select * from $data_table[$i] where `$fg` like '%$Acc%'  and UNIX_TIMESTAMP(LogTime) < '$end_time' ";
					}
					$dosql->ExecNoneQuery('SET NAMES Default');
					$dopage->GetPage($sql,50,'UNIX_TIMESTAMP(LogTime) DESC');
					while($data = $dosql->GetArray())
					{						
						$row[$k]['Uin'] = $data['Uin'];
						$row[$k]['LoginName'] = $data['Pass9Account'];
						$row[$k]['RoleName'] = iconv("gbk","utf8",$data['RoleInfo_RoleName']);
						$row[$k]['ItemNum'] = $data['GridData_Num']; //物品丢弃数量
						$row[$k]['ItemID'] = $data['GridData_WID']; //物品唯一ID
						$row[$k]['ValidEnd'] = $data['GridData_ValidEnd']; //有效期
						$row[$k]['qhlv'] = $data['GridData_StarLv'];//强化★级
						$row[$k]['pz'] = "无";
						if($data['GridData_ValidEnd'] == 0)
						{
							$row[$k]['ValidEnd'] = "永久";
						}
						else if($data['GridData_ValidEnd'] != 0)
						{
							$row[$k]['ValidEnd'] = date('Y-m-d H:i:s',$data['GridData_ValidEnd']);
						}
						//记录时间
						$row[$k]['LogTime'] = $data['LogTime'];
						
						$row[$k]['Item'] = $data['GridData_DefID'];
						$sql = "SELECT `Name`,`Important` FROM ItemDef where `ItemID`='$data[GridData_DefID]'";
						mysql_select_db('resdb');
						$rest = mysql_query($sql);
						$rowf = mysql_fetch_array($rest);
						if($rowf != "" && $rowf != false)
						{
							$row[$k]['Item'] = iconv("gbk","utf8",$rowf['Name']);
						}							
						if(0==$row[$k]['qhlv'])
						{
							$row[$k]['qhlv'] = " ";
						}
						else
						{
							$row[$k]['qhlv'] = "(".$data['GridData_StarLv']."星：";
							for($ij=0;$ij<$data['GridData_StarLv'];$ij++)
							{
								$ss = "GridData_StarInfo".($ij+1)."_ValIdx";
								$zdj = $data[$ss];
								$zd = "StarInfo".($zdj+1)."_Mul";
								$sqls = "SELECT `$zd` FROM `ArmStarDef` where `Star`='$data[GridData_StarLv]'";
								$xj = mysql_query($sqls);
								$xjs = mysql_fetch_array($xj);
								$row[$k]['qhlv'] .= $xjs[$zd].",";
							}
							$row[$k]['qhlv'] .= ")";
						}
								
						$dosql->select_db($db);
								
						$row[$k]['OpType'] = $data['GridData_Type'];
						if($data['GridData_Type'] == 0)
						{
							$row[$k]['OpType'] = "道具";
						}
						else if($data['GridData_Type'] == 1)
						{
							$row[$k]['OpType'] = "装备";
							$row[$k]['pz'] = $rowf['Important'];
							if(4 <= $row[$k]['pz'])
							{
								$row[$k]['pz'] = "粉装";
							}
							else if(3 == $row[$k]['pz'])
							{
								$row[$k]['pz'] = "紫装";
							}
							else if(2 == $row[$k]['pz'])
							{
								$row[$k]['pz'] = "蓝装";
							}
							else if(1 == $row[$k]['pz'])
							{
								$row[$k]['pz'] = "绿装";
							}
							else if(0 == $row[$k]['pz'])
							{
								$row[$k]['pz'] = "白装";
							}
						}
						$k++;
					}
				}
				else
				{
					continue;
				}
			}
		}
		if( $fate >= 3){
			echo json_encode("结束时间与开始时间相差不能超过3天");
			exit;
		}
		if(0>$fate)
		{
			echo json_encode("结束时间不能低于开始时间");
			exit;
		}
		if(0==$fate && $fag<0)
		{
			echo json_encode("结束时间不能低于开始时间");
			exit;
		}
	}
	//$row['nums'] = $nums;
	echo json_encode($row);
}

//角色死亡掉落
if(isset($_POST['LogRoleDieDrop']))
{
	$nums = 0;
	$row[]="";
	$db1 = $_POST['xzdb'];
	$qq = $_POST['qq'];
	$fg = $_POST['fag'];
	if($fg == 1){ $fg = "RoleInfo_RoleName";}
	else if($fg == 2){ $fg = "LoginName";}
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
	
	$qz="LogRoleDieDrop_";
	$Acc=$_POST['us'];
	$Acc = iconv("utf8","gbk",$Acc);
	
	date_default_timezone_set('PRC');
	//提交过来的开始时间的int
	$statr_time=strtotime($_POST['time']);

	//获取开始数据表
	$statr_table=date('ymd',$statr_time);
	
	//提交过来的结束时间
	$end_time=strtotime($_POST['time1']);
	
	//获取结束数据表
	$end_table=date('ymd',$end_time);
	
	
	//未填写结束时间
	if("" == $end_time || '0' == $end_time)
	{
		$data_table=$qz.$statr_table;
		$k=0;
		if($dosql->IsTable($data_table))
		{
			$sql = "select * from $data_table where `$fg` like '%$Acc%' ";
			
			$dosql->ExecNoneQuery('SET NAMES Default');
			$dopage->GetPage($sql,50,'UNIX_TIMESTAMP(LogTime) DESC');
			while($data = $dosql->GetArray())
			{
				$row[$k]['wid'] = $data['EventLogWID'];
				$row[$k]['Uin'] = $data['Uin'];
				$row[$k]['LoginName'] = $data['LoginName'];
				$row[$k]['RoleName'] = iconv("gbk","utf8",$data['RoleInfo_RoleName']);
				$row[$k]['DropNum'] = $data['DropNum'];
				//记录时间
				$row[$k]['LogTime'] = $data['LogTime'];
				$row[$k]['Item'] = "";
				$bx="";
				for($fx=1;$fx<= $data['DropNum'];$fx++)
				{
					$x = $data['DropItem'.$fx.'_DefID'];
					//物品名
					$sql = "SELECT `Name`,`Important` FROM ItemDef where `ItemID`='$x'";
					//echo $sql;
					mysql_select_db('resdb');
					$rest = mysql_query($sql);
					$rowf = mysql_fetch_array($rest);
					//品质$rowf['Important'];			
					if($rowf != "" && $rowf != false)
					{
						$bx .= iconv("gbk","utf8",$rowf['Name']).'：';
					}
					else
					{
						$bx .= $x;
					}
					if($rowf['Important'] == "")
					{
						$bx .= "无：无：无：";
					}
					else 
					{
						if($rowf['Important'] == 0)
						{
							$bx .= "白装：";
						}
						else if($rowf['Important'] == 1)
						{
							$bx .= "绿装：";
						}
						else if($rowf['Important'] == 2)
						{
							$bx .= "蓝装：";
						}
						else if($rowf['Important'] == 3)
						{
							$bx .= "紫装：";
						}
						else if($rowf['Important'] >= 4)
						{
							$bx .= "粉装：";
						}
						else 
						{
							$bx .= "无：";
						}
						$findex = "DropItem".$fx."_StarLv";
						$bx .= "[：".$data[$findex]."星：";
						for($ij=0;$ij<$data[$findex];$ij++)
						{
							$ss = "DropItem".$fx."_StarInfo".($ij+1)."_ValIdx";
							$zdj = $data[$ss];
							$zd = "StarInfo".($zdj+1)."_Mul";
							$sqls = "SELECT `$zd` FROM `ArmStarDef` where `Star`='$data[$findex]'";
							$xj = mysql_query($sqls);
							$xjs = mysql_fetch_array($xj);
							$bx .= $xjs[$zd].",";
						}
						$bx .= "]";
					}
					$dosql->select_db($db);
					//唯一ID
					$bx .= $data['DropItem'.$fx.'_WID'].'：';
					if($data['DropItem'.$fx.'_ValidEnd'] == 0)
					{
						$bx .= "永久".'<br/>';
					}
					else if($data['GridData_ValidEnd'] != 0)
					{
						$bx .= date('Y-m-d H:i:s',$data['GridData_ValidEnd']).'<br/>';
					}
				}
				$row[$k]['Item'] = $bx;
				$dosql->select_db($db);
				
				$k++;
			}
		}
		else if(!$dosql->IsTable($data_table))
		{
			echo json_encode("不存在数据表");
			exit;
		}
	}
	else
	{
		$fate=$end_table-$statr_table;
		$fag = $end_time-$statr_time;
		if("0"==$fate && $fag>=0)
		{
			$k=0;
			$data_table=$qz.$statr_table;
			if($dosql->IsTable($data_table))
			{			
				$sql = "select *  from $data_table where `$fg` like '%$Acc%'  and UNIX_TIMESTAMP(LogTime) >'$statr_time' and UNIX_TIMESTAMP(LogTime)<'$end_time' ";
				$dosql->ExecNoneQuery('SET NAMES Default');
				$dopage->GetPage($sql,50,'UNIX_TIMESTAMP(LogTime) DESC');
				while($data = $dosql->GetArray())
				{
					$row[$k]['wid'] = $data['EventLogWID'];
					$row[$k]['Uin'] = $data['Uin'];
					$row[$k]['LoginName'] = $data['LoginName'];
					$row[$k]['RoleName'] = iconv("gbk","utf8",$data['RoleInfo_RoleName']);
					$row[$k]['DropNum'] = $data['DropNum'];
					//记录时间
					$row[$k]['LogTime'] = $data['LogTime'];
					$row[$k]['Item'] = "";
					$bx="";
					for($fx=1;$fx<= $data['DropNum'];$fx++)
					{
						$x = $data['DropItem'.$fx.'_DefID'];
						//物品名
						$sql = "SELECT `Name`,`Important` FROM ItemDef where `ItemID`='$x'";
						//echo $sql;
						mysql_select_db('resdb');
						$rest = mysql_query($sql);
						$rowf = mysql_fetch_array($rest);
						//品质$rowf['Important'];			
						if($rowf != "" && $rowf != false)
						{
							$bx .= iconv("gbk","utf8",$rowf['Name']).'：';
						}
						else
						{
							$bx .= $x;
						}
						if($rowf['Important'] == "")
						{
							$bx .= "无：无：无：";
						}
						else 
						{
							if($rowf['Important'] == 0)
							{
								$bx .= "白装：";
							}
							else if($rowf['Important'] == 1)
							{
								$bx .= "绿装：";
							}
							else if($rowf['Important'] == 2)
							{
								$bx .= "蓝装：";
							}
							else if($rowf['Important'] == 3)
							{
								$bx .= "紫装：";
							}
							else if($rowf['Important'] >= 4)
							{
								$bx .= "粉装：";
							}
							else
							{
								$bx .= "无：";
							}
							$findex = "DropItem".$fx."_StarLv";
							$bx .= "[：".$data[$findex]."星：";
							for($ij=0;$ij<$data[$findex];$ij++)
							{
								$ss = "DropItem".$fx."_StarInfo".($ij+1)."_ValIdx";
								$zdj = $data[$ss];
								$zd = "StarInfo".($zdj+1)."_Mul";
								$sqls = "SELECT `$zd` FROM `ArmStarDef` where `Star`='$data[$findex]'";
								$xj = mysql_query($sqls);
								$xjs = mysql_fetch_array($xj);
								$bx .= $xjs[$zd].",";
							}
							$bx .= "]";
						}
						$dosql->select_db($db);
						//唯一ID
						$bx .= $data['DropItem'.$fx.'_WID'].'：';
						if($data['DropItem'.$fx.'_ValidEnd'] == 0)
						{
							$bx .= "永久".'<br/>';
						}
						else if($data['GridData_ValidEnd'] != 0)
						{
							$bx .= date('Y-m-d H:i:s',$data['GridData_ValidEnd']).'<br/>';
						}
					}
					$row[$k]['Item'] = $bx;
					$dosql->select_db($db);
					$k++;
				}
				$nums = count($row);		
			}
			else
			{
				echo json_encode("不存在数据表");
				exit;
			}
		}
		if(0< $fate && $fate < 3)
		{
			$k = 0;
			for($i=0;$i<=$fate;$i++)
			{
				$data_table[$i]=$qz.($statr_table+$i);
				if($dosql->IsTable($data_table[$i]))
				{
				
					if($i<$fate)
					{
						$sql = "select * from $data_table[$i] where `$fg` like '%$Acc%'  and UNIX_TIMESTAMP(LogTime) >'$statr_time' ";
					}
					if($i == $fate)
					{
						$sql = "select * from $data_table[$i] where `$fg` like '%$Acc%'  and UNIX_TIMESTAMP(LogTime) < '$end_time' ";
					}
					$dosql->ExecNoneQuery('SET NAMES Default');
					$dopage->GetPage($sql,50,'UNIX_TIMESTAMP(LogTime) DESC');
					while($data = $dosql->GetArray())
					{						
						$row[$k]['wid'] = $data['EventLogWID'];
						$row[$k]['Uin'] = $data['Uin'];
						$row[$k]['LoginName'] = $data['LoginName'];
						$row[$k]['RoleName'] = iconv("gbk","utf8",$data['RoleInfo_RoleName']);
						$row[$k]['DropNum'] = $data['DropNum'];
						//记录时间
						$row[$k]['LogTime'] = $data['LogTime'];
						$row[$k]['Item'] = "";
						$bx="";
						for($fx=1;$fx<= $data['DropNum'];$fx++)
						{
							$x = $data['DropItem'.$fx.'_DefID'];
							//物品名
							$sql = "SELECT `Name`,`Important` FROM ItemDef where `ItemID`='$x'";
							//echo $sql;
							mysql_select_db('resdb');
							$rest = mysql_query($sql);
							$rowf = mysql_fetch_array($rest);
							//品质$rowf['Important'];
							if($rowf != "" && $rowf != false)
							{
								$bx .= iconv("gbk","utf8",$rowf['Name']).'：';
							}
							else
							{
								$bx .= $x;
							}
							if($rowf['Important'] == "")
							{
								$bx .= "无：无：无：";
							}
							else
							{
								if($rowf['Important'] == 0)
								{
									$bx .= "白装：";
								}
								else if($rowf['Important'] == 1)
								{
									$bx .= "绿装：";
								}
								else if($rowf['Important'] == 2)
								{
									$bx .= "蓝装：";
								}
								else if($rowf['Important'] == 3)
								{
									$bx .= "紫装：";
								}
								else if($rowf['Important'] >= 4)
								{
									$bx .= "粉装：";
								}
								else
								{
									$bx .= "无：";
								}
								$findex = "DropItem".$fx."_StarLv";
								$bx .= "[：".$data[$findex]."星：";
								for($ij=0;$ij<$data[$findex];$ij++)
								{
									$ss = "DropItem".$fx."_StarInfo".($ij+1)."_ValIdx";
									$zdj = $data[$ss];
									$zd = "StarInfo".($zdj+1)."_Mul";
									$sqls = "SELECT `$zd` FROM `ArmStarDef` where `Star`='$data[$findex]'";
									$xj = mysql_query($sqls);
									$xjs = mysql_fetch_array($xj);
									$bx .= $xjs[$zd].",";
								}
								$bx .= "]";
							}
							$dosql->select_db($db);
							//唯一ID
							$bx .= $data['DropItem'.$fx.'_WID'].'：';
							if($data['DropItem'.$fx.'_ValidEnd'] == 0)
							{
								$bx .= "永久".'<br/>';
							}
							else if($data['GridData_ValidEnd'] != 0)
							{
								$bx .= date('Y-m-d H:i:s',$data['GridData_ValidEnd']).'<br/>';
							}
						}						
						$row[$k]['Item'] = $bx;
						$dosql->select_db($db);
						$k++;
					}
				}
				else
				{
					continue;
				}
			}
		}
		if( $fate >= 3){
			echo json_encode("结束时间与开始时间相差不能超过3天");
			exit;
		}
		if(0>$fate)
		{
			echo json_encode("结束时间不能低于开始时间");
			exit;
		}
		if(0==$fate && $fag<0)
		{
			echo json_encode("结束时间不能低于开始时间");
			exit;
		}
	}
	//$row['nums'] = $nums;
	echo json_encode($row);
}

//角色快照
if(isset($_POST['LogRoleSnapshot']))
{
	$nums = 0;
	$row[]="";
	$db1 = $_POST['xzdb'];
	$qq = $_POST['qq'];
	$fg = $_POST['fag'];
	if($fg == 1){ $fg = "RoleName";}
	else if($fg == 2){ $fg = "LoginName";}
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
	
	$qz="LogRoleSnapshot_";
	$Acc=$_POST['us'];
	$Acc = iconv("utf8","gbk",$Acc);
	
	date_default_timezone_set('PRC');
	//提交过来的开始时间的int
	$statr_time=strtotime($_POST['time']);

	//获取开始数据表
	$statr_table=date('ymd',$statr_time);
	
	//提交过来的结束时间
	$end_time=strtotime($_POST['time1']);
	
	//获取结束数据表
	$end_table=date('ymd',$end_time);
	
	
	//未填写结束时间
	if("" == $end_time || '0' == $end_time)
	{
		$data_table=$qz.$statr_table;
		$k=0;
		if($dosql->IsTable($data_table))
		{
			$sql = "select * from $data_table where `$fg` like '%$Acc%' ";
			
			$dosql->ExecNoneQuery('SET NAMES Default');
			$dopage->GetPage($sql,50,'UNIX_TIMESTAMP(LogTime) DESC');
			while($data = $dosql->GetArray())
			{
				//strtoupper();
				$row[$k]['LogRoleData'] = strtoupper(bin2hex($data['LogRoleData']));
				$row[$k]['Uin'] = $data['Uin'];
				$row[$k]['LoginName'] = $data['LoginName'];
				$row[$k]['RoleName'] = iconv("gbk","utf8",$data['RoleName']);
				$row[$k]['LogTime'] = $data['LogTime'];
				$row[$k]['Level'] = $data['Level'];
				$row[$k]['Exp'] = $data['Exp'];
				$row[$k]['Money'] = $data['Money'];
				$row[$k]['Bull'] = $data['Bull'];
				$row[$k]['StashMoney'] = $data['StashMoney'];
				$row[$k]['SundriesItemNum'] = $data['SundriesItemNum'];
				$row[$k]['StuffItemNum'] = $data['StuffItemNum'];
				$row[$k]['TaskItemNum'] = $data['TaskItemNum'];
				$row[$k]['WearItemNum'] = $data['WearItemNum'];
				$row[$k]['SkillNum'] = $data['SkillNum'];
				$row[$k]['Map'] = $data['Map'];
				//坐标
				$row[$k]['xy'] = "(".$data['Pos_x'].','.$data['Pos_y'].")";
				//记录时间
				$row[$k]['LogTime'] = $data['LogTime'];
				$k++;
			}
		}
		else if(!$dosql->IsTable($data_table))
		{
			echo json_encode("不存在数据表");
			exit;
		}
	}
	else
	{
		$fate=$end_table-$statr_table;
		$fag = $end_time-$statr_time;
		if("0"==$fate && $fag>=0)
		{
			$k=0;
			$data_table=$qz.$statr_table;
			if($dosql->IsTable($data_table))
			{			
				$sql = "select *  from $data_table where `$fg` like '%$Acc%'  and UNIX_TIMESTAMP(LogTime) >'$statr_time' and UNIX_TIMESTAMP(LogTime)<'$end_time' ";
				$dosql->ExecNoneQuery('SET NAMES Default');
				$dopage->GetPage($sql,50,'UNIX_TIMESTAMP(LogTime) DESC');
				while($data = $dosql->GetArray())
				{
					$row[$k]['LogRoleData'] = strtoupper(bin2hex($data['LogRoleData']));
					$row[$k]['Uin'] = $data['Uin'];
					$row[$k]['LoginName'] = $data['LoginName'];
					$row[$k]['RoleName'] = iconv("gbk","utf8",$data['RoleName']);
					$row[$k]['LogTime'] = $data['LogTime'];
					$row[$k]['Level'] = $data['Level'];
					$row[$k]['Exp'] = $data['Exp'];
					$row[$k]['Money'] = $data['Money'];
					$row[$k]['Bull'] = $data['Bull'];
					$row[$k]['StashMoney'] = $data['StashMoney'];
					$row[$k]['SundriesItemNum'] = $data['SundriesItemNum'];
					$row[$k]['StuffItemNum'] = $data['StuffItemNum'];
					$row[$k]['TaskItemNum'] = $data['TaskItemNum'];
					$row[$k]['WearItemNum'] = $data['WearItemNum'];
					$row[$k]['SkillNum'] = $data['SkillNum'];
					$row[$k]['Map'] = $data['Map'];
					//坐标
					$row[$k]['xy'] = "( ".$data['Pos_x'].','.$data['Pos_y']." )";
					//记录时间
					$row[$k]['LogTime'] = $data['LogTime'];
					$k++;
				}
				$nums = count($row);		
			}
			else
			{
				echo json_encode("不存在数据表");
				exit;
			}
		}
		if(0< $fate && $fate < 3)
		{
			$k = 0;
			for($i=0;$i<=$fate;$i++)
			{
				$data_table[$i]=$qz.($statr_table+$i);
				if($dosql->IsTable($data_table[$i]))
				{
				
					if($i<$fate)
					{
						$sql = "select * from $data_table[$i] where `$fg` like '%$Acc%'  and UNIX_TIMESTAMP(LogTime) >'$statr_time' ";
					}
					if($i == $fate)
					{
						$sql = "select * from $data_table[$i] where `$fg` like '%$Acc%'  and UNIX_TIMESTAMP(LogTime) < '$end_time' ";
					}
					$dosql->ExecNoneQuery('SET NAMES Default');
					$dopage->GetPage($sql,50,'UNIX_TIMESTAMP(LogTime) DESC');
					while($data = $dosql->GetArray())
					{
						$row[$k]['LogRoleData'] = strtoupper(bin2hex($data['LogRoleData']));						
						$row[$k]['Uin'] = $data['Uin'];
						$row[$k]['LoginName'] = $data['LoginName'];
						$row[$k]['RoleName'] = iconv("gbk","utf8",$data['RoleName']);
						$row[$k]['LogTime'] = $data['LogTime'];
						$row[$k]['Level'] = $data['Level'];
						$row[$k]['Exp'] = $data['Exp'];
						$row[$k]['Money'] = $data['Money'];
						$row[$k]['Bull'] = $data['Bull'];
						$row[$k]['StashMoney'] = $data['StashMoney'];
						$row[$k]['SundriesItemNum'] = $data['SundriesItemNum'];
						$row[$k]['StuffItemNum'] = $data['StuffItemNum'];
						$row[$k]['TaskItemNum'] = $data['TaskItemNum'];
						$row[$k]['WearItemNum'] = $data['WearItemNum'];
						$row[$k]['SkillNum'] = $data['SkillNum'];
						$row[$k]['Map'] = $data['Map'];
						//坐标
						$row[$k]['xy'] = "( ".$data['Pos_x'].','.$data['Pos_y']." )";
						//记录时间
						$row[$k]['LogTime'] = $data['LogTime'];
						$k++;
					}
				}
				else
				{
					continue;
				}
			}
		}
		if( $fate >= 3){
			echo json_encode("结束时间与开始时间相差不能超过3天");
			exit;
		}
		if(0>$fate)
		{
			echo json_encode("结束时间不能低于开始时间");
			exit;
		}
		if(0==$fate && $fag<0)
		{
			echo json_encode("结束时间不能低于开始时间");
			exit;
		}
	}
	//$row['nums'] = $nums;
	echo json_encode($row);
}
//商城金银币交易
if(isset($_POST['OpTradeInfo']))
{
	$nums = 0;
	$row[]="";
	$db1 = $_POST['xzdb'];
	$qq = $_POST['qq'];
	$fg = $_POST['fag'];
	if($fg == 1){ $fg = "RoleInfo_RoleName";}
	else if($fg == 2){ $fg = "Account";}
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
	
	$qz="OpTradeInfo_";
	$Acc=$_POST['us'];
	$Acc = iconv("utf8","gbk",$Acc);
	
	date_default_timezone_set('PRC');
	//提交过来的开始时间的int
	$statr_time=strtotime($_POST['time']);

	//获取开始数据表
	$statr_table=date('ymd',$statr_time);
	
	//提交过来的结束时间
	$end_time=strtotime($_POST['time1']);
	
	//获取结束数据表
	$end_table=date('ymd',$end_time);
	
	
	//未填写结束时间
	if("" == $end_time || '0' == $end_time)
	{
		$data_table=$qz.$statr_table;
		$k=0;
		if($dosql->IsTable($data_table))
		{
			$sql = "select * from $data_table where `$fg` like '%$Acc%' ";
			
			$dosql->ExecNoneQuery('SET NAMES Default');
			$dopage->GetPage($sql,50,'UNIX_TIMESTAMP(LogTime) DESC');
			while($data = $dosql->GetArray())
			{
				$row[$k]['LoginName'] = $data['Account'];
				$row[$k]['RoleName'] = iconv("gbk","utf8",$data['RoleInfo_RoleName']);
				$row[$k]['LogTime'] = $data['LogTime'];
				$row[$k]['TradeID'] = $data['TradeID'];
				$row[$k]['Money'] = $data['Money'];
				$row[$k]['Gold'] = $data['Gold'];
				$row[$k]['OperDesc'] = iconv("gbk","utf8",$data['OperDesc']);
				$row[$k]['OperType'] = $data['OperType'];
				if($data['OperType'] == 0)
				{
					$row[$k]['OperType'] = "金币交易";
				}
				if($data['OperType'] == 1)
				{
					$row[$k]['OperType'] = "银币交易";
				}
				$k++;
			}
		}
		else if(!$dosql->IsTable($data_table))
		{
			echo json_encode("不存在数据表");
			exit;
		}
	}
	else
	{
		$fate=$end_table-$statr_table;
		$fag = $end_time-$statr_time;
		if("0"==$fate && $fag>=0)
		{
			$k=0;
			$data_table=$qz.$statr_table;
			if($dosql->IsTable($data_table))
			{			
				$sql = "select *  from $data_table where `$fg` like '%$Acc%'  and UNIX_TIMESTAMP(LogTime) >'$statr_time' and UNIX_TIMESTAMP(LogTime)<'$end_time' ";
				$dosql->ExecNoneQuery('SET NAMES Default');
				$dopage->GetPage($sql,50,'UNIX_TIMESTAMP(LogTime) DESC');
				while($data = $dosql->GetArray())
				{
					$row[$k]['LoginName'] = $data['Account'];
					$row[$k]['RoleName'] = iconv("gbk","utf8",$data['RoleInfo_RoleName']);
					$row[$k]['LogTime'] = $data['LogTime'];
					$row[$k]['TradeID'] = $data['TradeID'];
					$row[$k]['Money'] = $data['Money'];
					$row[$k]['Gold'] = $data['Gold'];
					$row[$k]['OperDesc'] = iconv("gbk","utf8",$data['OperDesc']);
					$row[$k]['OperType'] = $data['OperType'];
					if($data['OperType'] == 0)
					{
						$row[$k]['OperType'] = "金币交易";
					}
					if($data['OperType'] == 1)
					{
						$row[$k]['OperType'] = "银币交易";
					}
					$k++;
				}
				$nums = count($row);		
			}
			else
			{
				echo json_encode("不存在数据表");
				exit;
			}
		}
		if(0< $fate && $fate < 3)
		{
			$k = 0;
			for($i=0;$i<=$fate;$i++)
			{
				$data_table[$i]=$qz.($statr_table+$i);
				if($dosql->IsTable($data_table[$i]))
				{
				
					if($i<$fate)
					{
						$sql = "select * from $data_table[$i] where `$fg` like '%$Acc%'  and UNIX_TIMESTAMP(LogTime) >'$statr_time' ";
					}
					if($i == $fate)
					{
						$sql = "select * from $data_table[$i] where `$fg` like '%$Acc%'  and UNIX_TIMESTAMP(LogTime) < '$end_time' ";
					}
					$dosql->ExecNoneQuery('SET NAMES Default');
					$dopage->GetPage($sql,50,'UNIX_TIMESTAMP(LogTime) DESC');
					while($data = $dosql->GetArray())
					{						
						$row[$k]['LoginName'] = $data['Account'];
						$row[$k]['RoleName'] = iconv("gbk","utf8",$data['RoleInfo_RoleName']);
						$row[$k]['LogTime'] = $data['LogTime'];
						$row[$k]['TradeID'] = $data['TradeID'];
						$row[$k]['Money'] = $data['Money'];
						$row[$k]['Gold'] = $data['Gold'];
						$row[$k]['OperDesc'] = iconv("gbk","utf8",$data['OperDesc']);
						$row[$k]['OperType'] = $data['OperType'];
						if($data['OperType'] == 0)
						{
							$row[$k]['OperType'] = "金币交易";
						}
						if($data['OperType'] == 1)
						{
							$row[$k]['OperType'] = "银币交易";
						}
						$k++;
					}
				}
				else
				{
					continue;
				}
			}
		}
		if( $fate >= 3){
			echo json_encode("结束时间与开始时间相差不能超过3天");
			exit;
		}
		if(0>$fate)
		{
			echo json_encode("结束时间不能低于开始时间");
			exit;
		}
		if(0==$fate && $fag<0)
		{
			echo json_encode("结束时间不能低于开始时间");
			exit;
		}
	}
	//$row['nums'] = $nums;
	echo json_encode($row);
}

//金卷消耗查询
if(isset($_POST['OpBindGoldConsume']))
{
	$nums = 0;
	$row[]="";
	$db1 = $_POST['xzdb'];
	$qq = $_POST['qq'];
	$fg = "Uin";
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
	
	$qz="OpBindGoldConsume_";
	$Acc=$_POST['us'];
	$Acc = iconv("utf8","gbk",$Acc);
	
	date_default_timezone_set('PRC');
	//提交过来的开始时间的int
	$statr_time=strtotime($_POST['time']);

	//获取开始数据表
	$statr_table=date('ymd',$statr_time);
	
	//提交过来的结束时间
	$end_time=strtotime($_POST['time1']);
	
	//获取结束数据表
	$end_table=date('ymd',$end_time);
	
	
	//未填写结束时间
	if("" == $end_time || '0' == $end_time)
	{
		$data_table=$qz.$statr_table;
		$k=0;
		if($dosql->IsTable($data_table))
		{
			$sql = "select * from $data_table where `$fg` = '$Acc' ";
			
			$dosql->ExecNoneQuery('SET NAMES Default');
			$dopage->GetPage($sql,50,'UNIX_TIMESTAMP(LogTime) DESC');
			while($data = $dosql->GetArray())
			{
				$row[$k]['Uin'] = $data['Uin'];
				$row[$k]['RoleID'] = $data['RoleID'];
				$row[$k]['RoleLevel'] = $data['RoleLevel'];
				$row[$k]['OpSubType'] = $data['OpSubType']; 
				$row[$k]['BindGoldNum'] = $data['BindGoldNum']; 
				$row[$k]['OpDesc'] = iconv("gbk","utf8",$data['OpDesc']);
				$row[$k]['LogTime'] = $data['LogTime'];
				$row[$k]['OpType'] = $data['OpType'];
				if($data['OpType'] == 1)
				{
					$row[$k]['OpType'] = "脚本加";
				}
				if($data['OpType'] == 2)
				{
					$row[$k]['OpType'] = "脚本减";
				}
				if($data['OpType'] == 3)
				{
					$row[$k]['OpType'] = "脚本加在线属性";
				}
				if($data['OpType'] == 4)
				{
					$row[$k]['OpType'] = "宝石重置";
				}
				if($data['OpType'] == 5)
				{
					$row[$k]['OpType'] = "宝石充能";
				}
				if($data['OpType'] == 6)
				{
					$row[$k]['OpType'] = "装备重铸";
				}
				if($data['OpType'] == 7)
				{
					$row[$k]['OpType'] = "gm加减";
				}
				if($data['OpType'] == 8)
				{
					$row[$k]['OpType'] = "转盘抽奖";
				}
				if($data['OpType'] == 9)
				{
					$row[$k]['OpType'] = "道具效果";
				}
				if($data['OpType'] == 10)
				{
					$row[$k]['OpType'] = "宠物重置技能";
				}
				if($data['OpType'] == 11)
				{
					$row[$k]['OpType'] = "宠物幻化";
				}
				if($data['OpType'] == 12)
				{
					$row[$k]['OpType'] = "今日目标";
				}
				if($data['OpType'] == 13)
				{
					$row[$k]['OpType'] = "商城道具";
				}
				if($data['OpType'] == 14)
				{
					$row[$k]['OpType'] = "BUFF争夺";
				}
				if($data['OpType'] == 15)
				{
					$row[$k]['OpType'] = "任务奖励";
				}
				$k++;
			}
		}
		else if(!$dosql->IsTable($data_table))
		{
			echo json_encode("不存在数据表");
			exit;
		}
	}
	else
	{
		$fate=$end_table-$statr_table;
		$fag = $end_time-$statr_time;
		if("0"==$fate && $fag>=0)
		{
			$k=0;
			$data_table=$qz.$statr_table;
			if($dosql->IsTable($data_table))
			{			
				$sql = "select *  from $data_table where `$fg` like '%$Acc%'  and UNIX_TIMESTAMP(LogTime) >'$statr_time' and UNIX_TIMESTAMP(LogTime)<'$end_time' ";
				$dosql->ExecNoneQuery('SET NAMES Default');
				$dopage->GetPage($sql,50,'UNIX_TIMESTAMP(LogTime) DESC');
				while($data = $dosql->GetArray())
				{
					$row[$k]['Uin'] = $data['Uin'];
					$row[$k]['RoleID'] = $data['RoleID'];
					$row[$k]['RoleLevel'] = $data['RoleLevel'];
					$row[$k]['OpSubType'] = $data['OpSubType']; 
					$row[$k]['BindGoldNum'] = $data['BindGoldNum']; 
					$row[$k]['OpDesc'] = iconv("gbk","utf8",$data['OpDesc']);
					$row[$k]['LogTime'] = $data['LogTime'];
					$row[$k]['OpType'] = $data['OpType'];
					if($data['OpType'] == 1)
					{
						$row[$k]['OpType'] = "脚本加";
					}
					if($data['OpType'] == 2)
					{
						$row[$k]['OpType'] = "脚本减";
					}
					if($data['OpType'] == 3)
					{
						$row[$k]['OpType'] = "脚本加在线属性";
					}
					if($data['OpType'] == 4)
					{
						$row[$k]['OpType'] = "宝石重置";
					}
					if($data['OpType'] == 5)
					{
						$row[$k]['OpType'] = "宝石充能";
					}
					if($data['OpType'] == 6)
					{
						$row[$k]['OpType'] = "装备重铸";
					}
					if($data['OpType'] == 7)
					{
						$row[$k]['OpType'] = "gm加减";
					}
					if($data['OpType'] == 8)
					{
						$row[$k]['OpType'] = "转盘抽奖";
					}
					if($data['OpType'] == 9)
					{
						$row[$k]['OpType'] = "道具效果";
					}
					if($data['OpType'] == 10)
					{
						$row[$k]['OpType'] = "宠物重置技能";
					}
					if($data['OpType'] == 11)
					{
						$row[$k]['OpType'] = "宠物幻化";
					}
					if($data['OpType'] == 12)
					{
						$row[$k]['OpType'] = "今日目标";
					}
					if($data['OpType'] == 13)
					{
						$row[$k]['OpType'] = "商城道具";
					}
					if($data['OpType'] == 14)
					{
						$row[$k]['OpType'] = "BUFF争夺";
					}
					if($data['OpType'] == 15)
					{
						$row[$k]['OpType'] = "任务奖励";
					}
					$k++;
				}
				$nums = count($row);		
			}
			else
			{
				echo json_encode("不存在数据表");
				exit;
			}
		}
		if(0< $fate && $fate < 3)
		{
			$k = 0;
			for($i=0;$i<=$fate;$i++)
			{
				$data_table[$i]=$qz.($statr_table+$i);
				if($dosql->IsTable($data_table[$i]))
				{
				
					if($i<$fate)
					{
						$sql = "select * from $data_table[$i] where `$fg` like '%$Acc%'  and UNIX_TIMESTAMP(LogTime) >'$statr_time' ";
					}
					if($i == $fate)
					{
						$sql = "select * from $data_table[$i] where `$fg` like '%$Acc%'  and UNIX_TIMESTAMP(LogTime) < '$end_time' ";
					}
					$dosql->ExecNoneQuery('SET NAMES Default');
					$dopage->GetPage($sql,50,'UNIX_TIMESTAMP(LogTime) DESC');
					while($data = $dosql->GetArray())
					{						
						$row[$k]['Uin'] = $data['Uin'];
						$row[$k]['RoleID'] = $data['RoleID'];
						$row[$k]['RoleLevel'] = $data['RoleLevel'];
						$row[$k]['OpSubType'] = $data['OpSubType']; 
						$row[$k]['BindGoldNum'] = $data['BindGoldNum']; 
						$row[$k]['OpDesc'] = iconv("gbk","utf8",$data['OpDesc']);
						$row[$k]['LogTime'] = $data['LogTime'];
						$row[$k]['OpType'] = $data['OpType'];
						if($data['OpType'] == 1)
						{
							$row[$k]['OpType'] = "脚本加";
						}
						if($data['OpType'] == 2)
						{
							$row[$k]['OpType'] = "脚本减";
						}
						if($data['OpType'] == 3)
						{
							$row[$k]['OpType'] = "脚本加在线属性";
						}
						if($data['OpType'] == 4)
						{
							$row[$k]['OpType'] = "宝石重置";
						}
						if($data['OpType'] == 5)
						{
							$row[$k]['OpType'] = "宝石充能";
						}
						if($data['OpType'] == 6)
						{
							$row[$k]['OpType'] = "装备重铸";
						}
						if($data['OpType'] == 7)
						{
							$row[$k]['OpType'] = "gm加减";
						}
						if($data['OpType'] == 8)
						{
							$row[$k]['OpType'] = "转盘抽奖";
						}
						if($data['OpType'] == 9)
						{
							$row[$k]['OpType'] = "道具效果";
						}
						if($data['OpType'] == 10)
						{
							$row[$k]['OpType'] = "宠物重置技能";
						}
						if($data['OpType'] == 11)
						{
							$row[$k]['OpType'] = "宠物幻化";
						}
						if($data['OpType'] == 12)
						{
							$row[$k]['OpType'] = "今日目标";
						}
						if($data['OpType'] == 13)
						{
							$row[$k]['OpType'] = "商城道具";
						}
						if($data['OpType'] == 14)
						{
							$row[$k]['OpType'] = "BUFF争夺";
						}
						if($data['OpType'] == 15)
						{
							$row[$k]['OpType'] = "任务奖励";
						}
						$k++;
					}
				}
				else
				{
					continue;
				}
			}
		}
		if( $fate >= 3){
			echo json_encode("结束时间与开始时间相差不能超过3天");
			exit;
		}
		if(0>$fate)
		{
			echo json_encode("结束时间不能低于开始时间");
			exit;
		}
		if(0==$fate && $fag<0)
		{
			echo json_encode("结束时间不能低于开始时间");
			exit;
		}
	}
	//$row['nums'] = $nums;
	echo json_encode($row);
}

//银币绑银消耗
if(isset($_POST['OpMoneyConsume']))
{
	$nums = 0;
	$row[]="";
	$db1 = $_POST['xzdb'];
	$qq = $_POST['qq'];
	$fg = "Uin";
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
	
	$qz="OpMoneyConsume_";
	$Acc=$_POST['us'];
	$Acc = iconv("utf8","gbk",$Acc);
	
	date_default_timezone_set('PRC');
	//提交过来的开始时间的int
	$statr_time=strtotime($_POST['time']);

	//获取开始数据表
	$statr_table=date('ymd',$statr_time);
	
	//提交过来的结束时间
	$end_time=strtotime($_POST['time1']);
	
	//获取结束数据表
	$end_table=date('ymd',$end_time);
	
	
	//未填写结束时间
	if("" == $end_time || '0' == $end_time)
	{
		$data_table=$qz.$statr_table;
		$k=0;
		if($dosql->IsTable($data_table))
		{
			$sql = "select * from $data_table where `$fg` = '$Acc' ";
			
			$dosql->ExecNoneQuery('SET NAMES Default');
			$dopage->GetPage($sql,50,'UNIX_TIMESTAMP(LogTime) DESC');
			while($data = $dosql->GetArray())
			{
				$row[$k]['Uin'] = $data['Uin'];
				$row[$k]['RoleID'] = $data['RoleID'];
				$row[$k]['RoleLevel'] = $data['RoleLevel'];
				$row[$k]['OpSubType'] = $data['OpSubType']; 
				$row[$k]['BindMoneyNum'] = $data['BindMoneyNum']; 
				$row[$k]['OpDesc'] = iconv("gbk","utf8",$data['OpDesc']);
				$row[$k]['LogTime'] = $data['LogTime'];
				$row[$k]['OpType'] = $data['OpDesc'];
				$row[$k]['MoneyNum'] = $data['MoneyNum'];
				$k++;
			}
		}
		else if(!$dosql->IsTable($data_table))
		{
			echo json_encode("不存在数据表");
			exit;
		}
	}
	else
	{
		$fate=$end_table-$statr_table;
		$fag = $end_time-$statr_time;
		if("0"==$fate && $fag>=0)
		{
			$k=0;
			$data_table=$qz.$statr_table;
			if($dosql->IsTable($data_table))
			{			
				$sql = "select *  from $data_table where `$fg` like '%$Acc%'  and UNIX_TIMESTAMP(LogTime) >'$statr_time' and UNIX_TIMESTAMP(LogTime)<'$end_time' ";
				$dosql->ExecNoneQuery('SET NAMES Default');
				$dopage->GetPage($sql,50,'UNIX_TIMESTAMP(LogTime) DESC');
				while($data = $dosql->GetArray())
				{
					$row[$k]['Uin'] = $data['Uin'];
					$row[$k]['RoleID'] = $data['RoleID'];
					$row[$k]['RoleLevel'] = $data['RoleLevel'];
					$row[$k]['OpSubType'] = $data['OpSubType']; 
					$row[$k]['BindMoneyNum'] = $data['BindMoneyNum']; 
					$row[$k]['OpDesc'] = iconv("gbk","utf8",$data['OpDesc']);
					$row[$k]['LogTime'] = $data['LogTime'];
					$row[$k]['OpType'] = $data['OpDesc'];
					$row[$k]['MoneyNum'] = $data['MoneyNum'];
						$k++;
					}
				$nums = count($row);		
			}
			else
			{
				echo json_encode("不存在数据表");
				exit;
			}
		}
		if(0< $fate && $fate < 3)
		{
			$k = 0;
			for($i=0;$i<=$fate;$i++)
			{
				$data_table[$i]=$qz.($statr_table+$i);
				if($dosql->IsTable($data_table[$i]))
				{
				
					if($i<$fate)
					{
						$sql = "select * from $data_table[$i] where `$fg` like '%$Acc%'  and UNIX_TIMESTAMP(LogTime) >'$statr_time' ";
					}
					if($i == $fate)
					{
						$sql = "select * from $data_table[$i] where `$fg` like '%$Acc%'  and UNIX_TIMESTAMP(LogTime) < '$end_time' ";
					}
					$dosql->ExecNoneQuery('SET NAMES Default');
					$dopage->GetPage($sql,50,'UNIX_TIMESTAMP(LogTime) DESC');
					while($data = $dosql->GetArray())
					{						
						$row[$k]['Uin'] = $data['Uin'];
						$row[$k]['RoleID'] = $data['RoleID'];
						$row[$k]['RoleLevel'] = $data['RoleLevel'];
						$row[$k]['OpSubType'] = $data['OpSubType']; 
						$row[$k]['BindMoneyNum'] = $data['BindMoneyNum']; 
						$row[$k]['OpDesc'] = iconv("gbk","utf8",$data['OpDesc']);
						$row[$k]['LogTime'] = $data['LogTime'];
						$row[$k]['OpType'] = $data['OpDesc'];
						$row[$k]['MoneyNum'] = $data['MoneyNum'];
						$k++;
					}
				}
				else
				{
					continue;
				}
			}
		}
		if( $fate >= 3){
			echo json_encode("结束时间与开始时间相差不能超过3天");
			exit;
		}
		if(0>$fate)
		{
			echo json_encode("结束时间不能低于开始时间");
			exit;
		}
		if(0==$fate && $fag<0)
		{
			echo json_encode("结束时间不能低于开始时间");
			exit;
		}
	}
	//$row['nums'] = $nums;
	echo json_encode($row);
}
//金币消耗记录
if(isset($_POST['GodCoinConsumeRecord']))
{
	$nums = 0;
	$row[]="";
	$db1 = $_POST['xzdb'];
	$fg = "Uin";
	$resdb = "world_".$db1."_";
	
	$GLOBALS['db_host'] = $hostBill;
	$GLOBALS['db_user'] = $userBill;
	$GLOBALS['db_name'] = "bill_".$db1;;
	$GLOBALS['db_pwd'] = $pwdBill;
	
	$dosql->Close(true);
	$dosql = new MySql(false);
	
	$data_table="GodCoinConsumeRecord";
	$Acc=$_POST['us'];
	$Acc = iconv("utf8","gbk",$Acc);
	
	
	date_default_timezone_set('PRC');
	//提交过来的开始时间的int
	$statr_time=strtotime($_POST['time']);

	//获取开始数据表
	$statr_table=date('ymd',$statr_time);
	
	//提交过来的结束时间
	$end_time=strtotime($_POST['time1']);
	
	//获取结束数据表
	$end_table=date('ymd',$end_time);
	
	$k=0;
	$fate=$end_table-$statr_table;
	$fag = $end_time-$statr_time;
	if($dosql->IsTable($data_table))
	{
		if("" == $end_time || '0' == $end_time)
		{
			$sql = "select * from $data_table where `$fg` = '$Acc'";
		}
		if("0"==$fate && $fag>=0)
		{
			$sql = "select *  from $data_table where `$fg` = '$Acc'  and UNIX_TIMESTAMP(LogTime) >'$statr_time' and UNIX_TIMESTAMP(LogTime)<'$end_time' ";
		}
		if(0< $fate && $fate < 3)
		{
			$sql = "select * from $data_table where `$fg` = '$Acc'  and UNIX_TIMESTAMP(LogTime) >'$statr_time' and UNIX_TIMESTAMP(LogTime) < '$end_time' ";
		}
		if( $fate >= 3)
		{
			echo json_encode("结束时间与开始时间相差不能超过3天");
			exit;
		}
		if(0>$fate)
		{
			echo json_encode("结束时间不能低于开始时间");
			exit;
		}
		if(0==$fate && $fag<0)
		{
			echo json_encode("结束时间不能低于开始时间");
			exit;
		}
		$dosql->ExecNoneQuery('SET NAMES Default');
		$dopage->GetPage($sql,50,'UNIX_TIMESTAMP(LogTime) DESC');
		while($data = $dosql->GetArray())
		{
			$shopdb = "";
			$row[$k]['seqID'] = $data['seqID'];
			$row[$k]['Uin'] = $data['Uin'];
			$row[$k]['RoleID'] = $data['RoleID']; 
			$row[$k]['RoleLevel'] = $data['RoleLevel']; 
			$row[$k]['WorldID'] = $data['WorldID']; 
			$row[$k]['OperSubType'] = $data['OperSubType']; 
			$row[$k]['GodCoin'] = $data['GodCoin']; 
			$row[$k]['ShopNum'] = $data['ShopNum']; 
			$row[$k]['OperDesc'] = iconv("gbk","utf8",$data['OperDesc']); 				
			$row[$k]['OperType'] = $data['OperType']; 
			if($data['OperType'] == 1)
			{
				$row[$k]['OperType'] = "购买商城商品";
			}
			else if($data['GiftFlag'] == 2)
			{
				$row[$k]['GiftFlag'] = "寄售交易产生的金券税收";
			}
			else if($data['GiftFlag'] == 3)
			{
				$row[$k]['GiftFlag'] = "装备解绑";
			}
			else if($data['GiftFlag'] == 4)
			{
				$row[$k]['GiftFlag'] = "装备解封印";
			}
			else if($data['GiftFlag'] == 5)
			{
				$row[$k]['GiftFlag'] = "宝石重置";
			}
			else if($data['GiftFlag'] == 6)
			{
				$row[$k]['GiftFlag'] = "宝石充能";
			}
			else if($data['GiftFlag'] == 7)
			{
				$row[$k]['GiftFlag'] = "装备重铸";
			}
			else if($data['GiftFlag'] == 8)
			{
				$row[$k]['GiftFlag'] = "宠物重置技能";
			}
			else if($data['GiftFlag'] == 9)
			{
				$row[$k]['GiftFlag'] = "宠物幻化";
			}
			else if($data['GiftFlag'] == 10)
			{
				$row[$k]['GiftFlag'] = "符文属性重置";
			}
			else if($data['GiftFlag'] == 11)
			{
				$row[$k]['GiftFlag'] = "开通砖石贵族";
			}
			else if($data['GiftFlag'] == 12)
			{
				$row[$k]['GiftFlag'] = "系统卖金币产出";
			}
			else if($data['GiftFlag'] == 13)
			{
				$row[$k]['GiftFlag'] = "系统卖银币消耗";
			}
			//记录时间
			$row[$k]['LogTime'] = $data['LogTime'];
			$row[$k]['Shop'] = $data['ShopID']; //物品ID
			$shopdb = $resdb.$data['WorldID'];
			//物品
			$sqlx = "SELECT `Name` FROM `ShopDef` where `ShopID`='$data[ShopID]'";
			mysql_select_db($shopdb,$link);
			$rest = mysql_query($sqlx,$link);
			$rowf = mysql_fetch_array($rest);
			if($rowf != "" && $rowf != false)
			{
				$row[$k]['Shop'] = iconv("gbk","utf8",$rowf['Name']);
			}
			$dosql->select_db($db);
			
			$k++;
		}
	}
	else if(!$dosql->IsTable($data_table))
	{
		echo json_encode("不存在数据表");
		exit;
	}
	//$row['nums'] = $nums;
	echo json_encode($row);
}

//宠物操作
if(isset($_POST['LogRolePet']))
{
	$nums = 0;
	$row[]="";
	$db1 = $_POST['xzdb'];
	$fg = $_POST['fag'];
	if($fg == 1){ $fg = "RoleInfo_RoleName";}
	else if($fg == 2){ $fg = "LoginName";}
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
	
	$qz="LogRolePet_";
	$Acc=$_POST['us'];
	$Acc = iconv("utf8","gbk",$Acc);
	
	date_default_timezone_set('PRC');
	//提交过来的开始时间的int
	$statr_time=strtotime($_POST['time']);

	//获取开始数据表
	$statr_table=date('ymd',$statr_time);
	
	//提交过来的结束时间
	$end_time=strtotime($_POST['time1']);
	
	//获取结束数据表
	$end_table=date('ymd',$end_time);
	
	
	//未填写结束时间
	if("" == $end_time || '0' == $end_time)
	{
		$data_table=$qz.$statr_table;
		$k=0;
		if($dosql->IsTable($data_table))
		{
			$sql = "select * from $data_table where `$fg` like '%$Acc%' ";
			
			$dosql->ExecNoneQuery('SET NAMES Default');
			$dopage->GetPage($sql,500,'UNIX_TIMESTAMP(LogTime) DESC');
			while($data = $dosql->GetArray())
			{
				$row[$k]['Uin'] = $data['Uin'];
				$row[$k]['LoginName'] = $data['LoginName'];
				$row[$k]['RoleName'] = iconv("gbk","utf8",$data['RoleInfo_RoleName']);
				$row[$k]['PetInfo_WID'] = $data['PetInfo_WID'];
				$row[$k]['Genius'] = $data['PetInfo_PetGenius'];  //品质
				if($data['PetInfo_PetGenius']==0 )
				{
					$row[$k]['Genius'] = "普通";
				}
				if($data['PetInfo_PetGenius']==1 )
				{
					$row[$k]['Genius'] = "良好";
				}
				if($data['PetInfo_PetGenius']==2 )
				{
					$row[$k]['Genius'] = "优秀";
				}
				if($data['PetInfo_PetGenius']==3 )
				{
					$row[$k]['Genius'] = "杰出";
				}
				if($data['PetInfo_PetGenius']==4 )
				{
					$row[$k]['Genius'] = "完美";
				}
				$row[$k]['StrengthenLevel'] = $data['PetInfo_StrengthenLevel']."(";
				//强化
				for($i=1;$i<=$data['PetInfo_StrengthenLevel'];$i++)
				{
					$row[$k]['StrengthenLevel'] .= $data['PetInfo_StrengthenInfo_'.$i].",";
				}
				//PetInfo_PetSkills_SkillGridNum(宠物技能数量)
				$row[$k]['PetSkills'] = "";
				for($i=1;$i<=$data['PetInfo_PetSkills_SkillGridNum'];$i++)
				{
					$row[$k]['PetSkills'] .= "(".$data['PetInfo_PetSkills_SkillGrid'.$i.'_SkillID'].",".$data['PetInfo_PetSkills_SkillGrid'.$i.'_SkillLevel'].")"; 
				}
				//天赋技能
				$row[$k]['NativeSkill'] = $data['PetInfo_NativeSkill_SkillID'].",".$data['PetInfo_NativeSkill_SkillLevel'];
				$row[$k]['StrengthenLevel'] .= ")";
				$row[$k]['ItemDec_Num'] .= "(".$data['ItemDec_BindNum'].",".$data['ItemDec_Num'].")";
				$row[$k]['Money'] = $data['Money']; 
				$row[$k]['Level'] = $data['PetInfo_Level'];
				$row[$k]['StarPet'] = $data['PetStrenInfo_Star'];
				$row[$k]['PreValPet'] = $data['PetStrenInfo_PreVal'];
				$row[$k]['LastValPet'] = $data['PetStrenInfo_LastVal'];
				//记录时间
				$row[$k]['LogTime'] = $data['LogTime'];
				//坐骑操作
				$row[$k]['OpType'] = $data['Event'];
				if($data['Event'] == 0)
				{
					$row[$k]['OpType'] = "孵化";
				}
				else if($data['Event'] == 1)
				{
					$row[$k]['OpType'] = "升级";
				}
				else if($data['Event'] == 2)
				{
					$row[$k]['OpType'] = "放生";
				}
				else if($data['Event'] == 3)
				{
					$row[$k]['OpType'] = "强化(保存)";
				}
				else if($data['Event'] == 4)
				{
					$row[$k]['OpType'] = "增强";
				}
				else if($data['Event'] == 5)
				{
					$row[$k]['OpType'] = "融合";
				}
				else if($data['Event'] == 6)
				{
					$row[$k]['OpType'] = "融合损失的宠物";
				}
				else if($data['Event'] == 7)
				{
					$row[$k]['OpType'] = "幻化";
				}
				else if($data['Event'] == 8)
				{
					$row[$k]['OpType'] = "强化消耗(不一定保存)";
				}
				else if($data['Event'] == 9)
				{
					$row[$k]['OpType'] = "重新强化消耗";
				}
				else if($data['Event'] == 10)
				{
					$row[$k]['OpType'] = "宠物技能加锁";
				}
				else if($data['Event'] == 11)
				{
					$row[$k]['OpType'] = "宠物重置技能";
				}
				else if($data['Event'] == 12)
				{
					$row[$k]['OpType'] = "宠物幻化保存";
				}
				else if($data['Event'] == 13)
				{
					$row[$k]['OpType'] = "宠物爱心等级变化"; 
				}
				else if($data['Event'] == 14)
				{
					$row[$k]['OpType'] = "宠物转星";
				}
				else if($data['Event'] == 15)
				{
					$row[$k]['OpType'] = "宠物吞噬";
				}
				else if($data['Event'] == 16)
				{
					$row[$k]['OpType'] = "宠物被吞噬";
				}
				$row[$k]['GetItem'] = "";
				$row[$k]['PetName'] = "";
				$x = $data['PetInfo_PetID'];
				//宠物名
				$sql = "SELECT `Name` FROM PetDef where `PetID`='$x'";
				//echo $sql;
				mysql_select_db('resdb');
				$rest = mysql_query($sql);
				$rowf = mysql_fetch_array($rest);
				if($rowf != "" && $rowf != false)
				{
					$row[$k]['PetName'] = iconv("gbk","utf8",$rowf['Name']).'：';
				}
				$x = $data['GetItem_ID'];
				$sql = "SELECT `Name` FROM ItemDef where `ItemID`='$x'";
				$rest = mysql_query($sql);
				$rowf = mysql_fetch_array($rest);
				if($rowf != "" && $rowf != false)
				{
					$row[$k]['GetItem'] = iconv("gbk","utf8",$rowf['Name']);
				}
				$k++;
			}
			$dosql->select_db($db);
		}
		else if(!$dosql->IsTable($data_table))
		{
			echo json_encode("不存在数据表");
			exit;
		}
	}
	else
	{
		$fate=$end_table-$statr_table;
		$fag = $end_time-$statr_time;
		if("0"==$fate && $fag>=0)
		{
			$k=0;
			$data_table=$qz.$statr_table;
			if($dosql->IsTable($data_table))
			{			
				$sql = "select *  from $data_table where `$fg` like '%$Acc%'  and UNIX_TIMESTAMP(LogTime) >'$statr_time' and UNIX_TIMESTAMP(LogTime)<'$end_time' ";
				$dosql->ExecNoneQuery('SET NAMES Default');
				$dopage->GetPage($sql,500,'UNIX_TIMESTAMP(LogTime) DESC');
				while($data = $dosql->GetArray())
				{
					$row[$k]['Uin'] = $data['Uin'];
					$row[$k]['LoginName'] = $data['LoginName'];
					$row[$k]['RoleName'] = iconv("gbk","utf8",$data['RoleInfo_RoleName']);
					$row[$k]['PetInfo_WID'] = $data['PetInfo_WID'];
					$row[$k]['Genius'] = $data['PetInfo_PetGenius'];  //品质
					if($data['PetInfo_PetGenius']==0 )
					{
						$row[$k]['Genius'] = "普通";
					}
					if($data['PetInfo_PetGenius']==1 )
					{
						$row[$k]['Genius'] = "良好";
					}
					if($data['PetInfo_PetGenius']==2 )
					{
						$row[$k]['Genius'] = "优秀";
					}
					if($data['PetInfo_PetGenius']==3 )
					{
						$row[$k]['Genius'] = "杰出";
					}
					if($data['PetInfo_PetGenius']==4 )
					{
						$row[$k]['Genius'] = "完美";
					}
					$row[$k]['StrengthenLevel'] = $data['PetInfo_StrengthenLevel']."(";
					//强化
					for($i=1;$i<=$data['PetInfo_StrengthenLevel'];$i++)
					{
						$row[$k]['StrengthenLevel'] .= $data['PetInfo_StrengthenInfo_'.$i].",";
					}
					//PetInfo_PetSkills_SkillGridNum(宠物技能数量)
					$row[$k]['PetSkills'] = "";
					for($i=1;$i<=$data['PetInfo_PetSkills_SkillGridNum'];$i++)
					{
						$row[$k]['PetSkills'] .= "(".$data['PetInfo_PetSkills_SkillGrid'.$i.'_SkillID'].",".$data['PetInfo_PetSkills_SkillGrid'.$i.'_SkillLevel'].")"; 
					}
					//天赋技能
					$row[$k]['NativeSkill'] = $data['PetInfo_NativeSkill_SkillID'].",".$data['PetInfo_NativeSkill_SkillLevel'];
					$row[$k]['StrengthenLevel'] .= ")";
					$row[$k]['ItemDec_Num'] .= "(".$data['ItemDec_BindNum'].",".$data['ItemDec_Num'].")";
					$row[$k]['Money'] = $data['Money']; 
					$row[$k]['Level'] = $data['PetInfo_Level'];
					$row[$k]['StarPet'] = $data['PetStrenInfo_Star'];
					$row[$k]['PreValPet'] = $data['PetStrenInfo_PreVal'];
					$row[$k]['LastValPet'] = $data['PetStrenInfo_LastVal'];
					//记录时间
					$row[$k]['LogTime'] = $data['LogTime'];
					//坐骑操作
					$row[$k]['OpType'] = $data['Event'];
					if($data['Event'] == 0)
					{
						$row[$k]['OpType'] = "孵化";
					}
					else if($data['Event'] == 1)
					{
						$row[$k]['OpType'] = "升级";
					}
					else if($data['Event'] == 2)
					{
						$row[$k]['OpType'] = "放生";
					}
					else if($data['Event'] == 3)
					{
						$row[$k]['OpType'] = "强化(保存)";
					}
					else if($data['Event'] == 4)
					{
						$row[$k]['OpType'] = "增强";
					}
					else if($data['Event'] == 5)
					{
						$row[$k]['OpType'] = "融合";
					}
					else if($data['Event'] == 6)
					{
						$row[$k]['OpType'] = "融合损失的宠物";
					}
					else if($data['Event'] == 7)
					{
						$row[$k]['OpType'] = "幻化";
					}
					else if($data['Event'] == 8)
					{
						$row[$k]['OpType'] = "强化消耗(不一定保存)";
					}
					else if($data['Event'] == 9)
					{
						$row[$k]['OpType'] = "重新强化消耗";
					}
					else if($data['Event'] == 10)
					{
						$row[$k]['OpType'] = "宠物技能加锁";
					}
					else if($data['Event'] == 11)
					{
						$row[$k]['OpType'] = "宠物重置技能";
					}
					else if($data['Event'] == 12)
					{
						$row[$k]['OpType'] = "宠物幻化保存";
					}
					$row[$k]['GetItem'] = "";
					$row[$k]['PetName'] = "";
					$x = $data['PetInfo_PetID'];
					//宠物名
					$sql = "SELECT `Name` FROM PetDef where `PetID`='$x'";
					//echo $sql;
					mysql_select_db('resdb');
					$rest = mysql_query($sql);
					$rowf = mysql_fetch_array($rest);
					if($rowf != "" && $rowf != false)
					{
						$row[$k]['PetName'] = iconv("gbk","utf8",$rowf['Name']).'：';
					}
					$x = $data['GetItem_ID'];
					$sql = "SELECT `Name` FROM ItemDef where `ItemID`='$x'";
					$rest = mysql_query($sql);
					$rowf = mysql_fetch_array($rest);
					if($rowf != "" && $rowf != false)
					{
						$row[$k]['GetItem'] = iconv("gbk","utf8",$rowf['Name']);
					}
					$k++;
				}
				$nums = count($row);		
			}
			else
			{
				echo json_encode("不存在数据表");
				exit;
			}
		}
		if(0< $fate && $fate < 3)
		{
			$k = 0;
			for($i=0;$i<=$fate;$i++)
			{
				$data_table[$i]=$qz.($statr_table+$i);
				if($dosql->IsTable($data_table[$i]))
				{
				
					if($i<$fate)
					{
						$sql = "select * from $data_table[$i] where `$fg` like '%$Acc%'  and UNIX_TIMESTAMP(LogTime) >'$statr_time' ";
					}
					if($i == $fate)
					{
						$sql = "select * from $data_table[$i] where `$fg` like '%$Acc%'  and UNIX_TIMESTAMP(LogTime) < '$end_time' ";
					}
					$dosql->ExecNoneQuery('SET NAMES Default');
					$dopage->GetPage($sql,500,'UNIX_TIMESTAMP(LogTime) DESC');
					while($data = $dosql->GetArray())
					{						
						$row[$k]['Uin'] = $data['Uin'];
						$row[$k]['LoginName'] = $data['LoginName'];
						$row[$k]['RoleName'] = iconv("gbk","utf8",$data['RoleInfo_RoleName']);
						$row[$k]['PetInfo_WID'] = $data['PetInfo_WID'];
						$row[$k]['Genius'] = $data['PetInfo_PetGenius'];  //品质
						if($data['PetInfo_PetGenius']==0 )
						{
							$row[$k]['Genius'] = "普通";
						}
						if($data['PetInfo_PetGenius']==1 )
						{
							$row[$k]['Genius'] = "良好";
						}
						if($data['PetInfo_PetGenius']==2 )
						{
							$row[$k]['Genius'] = "优秀";
						}
						if($data['PetInfo_PetGenius']==3 )
						{
							$row[$k]['Genius'] = "杰出";
						}
						if($data['PetInfo_PetGenius']==4 )
						{
							$row[$k]['Genius'] = "完美";
						}
						$row[$k]['StrengthenLevel'] = $data['PetInfo_StrengthenLevel']."(";
						//强化
						for($i=1;$i<=$data['PetInfo_StrengthenLevel'];$i++)
						{
							$row[$k]['StrengthenLevel'] .= $data['PetInfo_StrengthenInfo_'.$i].",";
						}
						//PetInfo_PetSkills_SkillGridNum(宠物技能数量)
						$row[$k]['PetSkills'] = "";
						for($i=1;$i<=$data['PetInfo_PetSkills_SkillGridNum'];$i++)
						{
							$row[$k]['PetSkills'] .= "(".$data['PetInfo_PetSkills_SkillGrid'.$i.'_SkillID'].",".$data['PetInfo_PetSkills_SkillGrid'.$i.'_SkillLevel'].")"; 
						}
						//天赋技能
						$row[$k]['NativeSkill'] = $data['PetInfo_NativeSkill_SkillID'].",".$data['PetInfo_NativeSkill_SkillLevel'];
						$row[$k]['StrengthenLevel'] .= ")";
						$row[$k]['ItemDec_Num'] .= "(".$data['ItemDec_BindNum'].",".$data['ItemDec_Num'].")";
						$row[$k]['Money'] = $data['Money']; 
						$row[$k]['Level'] = $data['PetInfo_Level'];
						$row[$k]['StarPet'] = $data['PetStrenInfo_Star'];
						$row[$k]['PreValPet'] = $data['PetStrenInfo_PreVal'];
						$row[$k]['LastValPet'] = $data['PetStrenInfo_LastVal'];
						//记录时间
						$row[$k]['LogTime'] = $data['LogTime'];
						//坐骑操作
						$row[$k]['OpType'] = $data['Event'];
						if($data['Event'] == 0)
						{
							$row[$k]['OpType'] = "孵化";
						}
						else if($data['Event'] == 1)
						{
							$row[$k]['OpType'] = "升级";
						}
						else if($data['Event'] == 2)
						{
							$row[$k]['OpType'] = "放生";
						}
						else if($data['Event'] == 3)
						{
							$row[$k]['OpType'] = "强化(保存)";
						}
						else if($data['Event'] == 4)
						{
							$row[$k]['OpType'] = "增强";
						}
						else if($data['Event'] == 5)
						{
							$row[$k]['OpType'] = "融合";
						}
						else if($data['Event'] == 6)
						{
							$row[$k]['OpType'] = "融合损失的宠物";
						}
						else if($data['Event'] == 7)
						{
							$row[$k]['OpType'] = "幻化";
						}
						else if($data['Event'] == 8)
						{
							$row[$k]['OpType'] = "强化消耗(不一定保存)";
						}
						else if($data['Event'] == 9)
						{
							$row[$k]['OpType'] = "重新强化消耗";
						}
						else if($data['Event'] == 10)
						{
							$row[$k]['OpType'] = "宠物技能加锁";
						}
						else if($data['Event'] == 11)
						{
							$row[$k]['OpType'] = "宠物重置技能";
						}
						else if($data['Event'] == 12)
						{
							$row[$k]['OpType'] = "宠物幻化保存";
						}
						$row[$k]['GetItem'] = "";
						$row[$k]['PetName'] = "";
						$x = $data['PetInfo_PetID'];
						//宠物名
						$sql = "SELECT `Name` FROM PetDef where `PetID`='$x'";
						//echo $sql;
						mysql_select_db('resdb');
						$rest = mysql_query($sql);
						$rowf = mysql_fetch_array($rest);
						if($rowf != "" && $rowf != false)
						{
							$row[$k]['PetName'] = iconv("gbk","utf8",$rowf['Name']).'：';
						}
						$x = $data['GetItem_ID'];
						$sql = "SELECT `Name` FROM ItemDef where `ItemID`='$x'";
						$rest = mysql_query($sql);
						$rowf = mysql_fetch_array($rest);
						if($rowf != "" && $rowf != false)
						{
							$row[$k]['GetItem'] = iconv("gbk","utf8",$rowf['Name']);
						}
						$k++;
					}
				}
				else
				{
					continue;
				}
			}
		}
		if( $fate >= 3){
			echo json_encode("结束时间与开始时间相差不能超过3天");
			exit;
		}
		if(0>$fate)
		{
			echo json_encode("结束时间不能低于开始时间");
			exit;
		}
		if(0==$fate && $fag<0)
		{
			echo json_encode("结束时间不能低于开始时间");
			exit;
		}
	}
	//$row['nums'] = $nums;
	echo json_encode($row);
}

//礼包领取
if(isset($_POST['LogGift']))
{
	$nums = 0;
	$row[]="";
	$db1 = $_POST['xzdb'];
	$fg = $_POST['fag'];
	if($fg == 1){ $fg = "RoleName";}
	else if($fg == 2){ $fg = "PlayerLoginName";}
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
	
	$qz="LogGift_";
	$Acc=$_POST['us'];
	$Acc = iconv("utf8","gbk",$Acc);
	
	date_default_timezone_set('PRC');
	//提交过来的开始时间的int
	$statr_time=strtotime($_POST['time']);

	//获取开始数据表
	$statr_table=date('ymd',$statr_time);
	
	//提交过来的结束时间
	$end_time=strtotime($_POST['time1']);
	
	//获取结束数据表
	$end_table=date('ymd',$end_time);
	
	
	//未填写结束时间
	if("" == $end_time || '0' == $end_time)
	{
		$data_table=$qz.$statr_table;
		$k=0;
		if($dosql->IsTable($data_table))
		{
			$sql = "select * from $data_table where `$fg` like '%$Acc%' ";
			$dosql->ExecNoneQuery('SET NAMES Default');
			$dopage->GetPage($sql,50,'UNIX_TIMESTAMP(LogTime) DESC');
			while($data = $dosql->GetArray())
			{
				$row[$k]['Uin'] = $data['PlayerUin'];
				$row[$k]['LoginName'] = $data['PlayerLoginName'];
				$row[$k]['RoleName'] = iconv("gbk","utf8",$data['RoleName']);
				$row[$k]['PlayerLevel'] = $data['PlayerLevel']; 
				$row[$k]['GiftID'] = $data['GiftID'];
				//记录时间
				$row[$k]['LogTime'] = $data['LogTime'];
				//坐骑操作
				$row[$k]['OpType'] = $data['GiftType'];
				if($data['GiftType'] == 1)
				{
					$row[$k]['OpType'] = "等级奖励";
				}
				else if($data['GiftType'] == 2)
				{
					$row[$k]['GiftType'] = "每天奖励";
				}
				else if($data['GiftType'] == 3)
				{
					$row[$k]['OpType'] = "在线奖励";
				}
				else if($data['GiftType'] == 4)
				{
					$row[$k]['OpType'] = "特殊奖励(TT1账号奖励)";
				}
				else if($data['GiftType'] == 5)
				{
					$row[$k]['OpType'] = "充值奖励";
				}
				else if($data['GiftType'] == 6)
				{
					$row[$k]['OpType'] = "连续在线组奖励(TT2福利)";
				}
				$k++;
			}
		}
		else if(!$dosql->IsTable($data_table))
		{
			echo json_encode("不存在数据表");
			exit;
		}
	}
	else
	{
		$fate=$end_table-$statr_table;
		$fag = $end_time-$statr_time;
		if("0"==$fate && $fag>=0)
		{
			$k=0;
			$data_table=$qz.$statr_table;
			if($dosql->IsTable($data_table))
			{			
				$sql = "select *  from $data_table where `$fg` like '%$Acc%'  and UNIX_TIMESTAMP(LogTime) >'$statr_time' and UNIX_TIMESTAMP(LogTime)<'$end_time' ";
				$dosql->ExecNoneQuery('SET NAMES Default');
				$dopage->GetPage($sql,50,'UNIX_TIMESTAMP(LogTime) DESC');
				while($data = $dosql->GetArray())
				{
					$row[$k]['Uin'] = $data['PlayerUin'];
					$row[$k]['LoginName'] = $data['PlayerLoginName'];
					$row[$k]['RoleName'] = iconv("gbk","utf8",$data['RoleName']);
					$row[$k]['PlayerLevel'] = $data['PlayerLevel']; 
					$row[$k]['GiftID'] = $data['GiftID'];
					//记录时间
					$row[$k]['LogTime'] = $data['LogTime'];
					//坐骑操作
					$row[$k]['OpType'] = $data['GiftType'];
					if($data['GiftType'] == 1)
					{
						$row[$k]['OpType'] = "等级奖励";
					}
					else if($data['GiftType'] == 2)
					{
						$row[$k]['GiftType'] = "每天奖励";
					}
					else if($data['GiftType'] == 3)
					{
						$row[$k]['OpType'] = "在线奖励";
					}
					else if($data['GiftType'] == 4)
					{
						$row[$k]['OpType'] = "特殊奖励(TT1账号奖励)";
					}
					else if($data['GiftType'] == 5)
					{
						$row[$k]['OpType'] = "充值奖励";
					}
					else if($data['GiftType'] == 6)
					{
						$row[$k]['OpType'] = "连续在线组奖励(TT2福利)";
					}
					$k++;
				}
				$nums = count($row);		
			}
			else
			{
				echo json_encode("不存在数据表");
				exit;
			}
		}
		if(0< $fate && $fate < 3)
		{
			$k = 0;
			for($i=0;$i<=$fate;$i++)
			{
				$data_table[$i]=$qz.($statr_table+$i);
				if($dosql->IsTable($data_table[$i]))
				{
				
					if($i<$fate)
					{
						$sql = "select * from $data_table[$i] where `$fg` like '%$Acc%'  and UNIX_TIMESTAMP(LogTime) >'$statr_time' ";
					}
					if($i == $fate)
					{
						$sql = "select * from $data_table[$i] where `$fg` like '%$Acc%'  and UNIX_TIMESTAMP(LogTime) < '$end_time' ";
					}
					$dosql->ExecNoneQuery('SET NAMES Default');
					$dopage->GetPage($sql,50,'UNIX_TIMESTAMP(LogTime) DESC');
					while($data = $dosql->GetArray())
					{						
						$row[$k]['Uin'] = $data['PlayerUin'];
						$row[$k]['LoginName'] = $data['PlayerLoginName'];
						$row[$k]['RoleName'] = iconv("gbk","utf8",$data['RoleName']);
						$row[$k]['PlayerLevel'] = $data['PlayerLevel']; 
						$row[$k]['GiftID'] = $data['GiftID'];
						//记录时间
						$row[$k]['LogTime'] = $data['LogTime'];
						//坐骑操作
						$row[$k]['OpType'] = $data['GiftType'];
						if($data['GiftType'] == 1)
						{
							$row[$k]['OpType'] = "等级奖励";
						}
						else if($data['GiftType'] == 2)
						{
							$row[$k]['GiftType'] = "每天奖励";
						}
						else if($data['GiftType'] == 3)
						{
							$row[$k]['OpType'] = "在线奖励";
						}
						else if($data['GiftType'] == 4)
						{
							$row[$k]['OpType'] = "特殊奖励(TT1账号奖励)";
						}
						else if($data['GiftType'] == 5)
						{
							$row[$k]['OpType'] = "充值奖励";
						}
						else if($data['GiftType'] == 6)
						{
							$row[$k]['OpType'] = "连续在线组奖励(TT2福利)";
						}
						$k++;
					}
				}
				else
				{
					continue;
				}
			}
		}
		if( $fate >= 3){
			echo json_encode("结束时间与开始时间相差不能超过3天");
			exit;
		}
		if(0>$fate)
		{
			echo json_encode("结束时间不能低于开始时间");
			exit;
		}
		if(0==$fate && $fag<0)
		{
			echo json_encode("结束时间不能低于开始时间");
			exit;
		}
	}
	//$row['nums'] = $nums;
	echo json_encode($row);
}
//装备加工
if(isset($_POST['ArmMachin']))
{
	$nums = 0;
	$row[]="";
	$db1 = $_POST['xzdb'];
	$fg = "RoleName";
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
	
	$qz="LogRoleArmMachining_";
	$Acc=$_POST['us'];
	$Acc = iconv("utf8","gbk",$Acc);
	
	date_default_timezone_set('PRC');
	//提交过来的开始时间的int
	$statr_time=strtotime($_POST['time']);

	//获取开始数据表
	$statr_table=date('ymd',$statr_time);
	
	//提交过来的结束时间
	$end_time=strtotime($_POST['time1']);
	
	//获取结束数据表
	$end_table=date('ymd',$end_time);
	
	
	//未填写结束时间
	if("" == $end_time || '0' == $end_time)
	{
		$data_table=$qz.$statr_table;
		$k=0;
		if($dosql->IsTable($data_table))
		{
			$sql = "select * from $data_table where `$fg` like '%$Acc%' ";
			
			$dosql->ExecNoneQuery('SET NAMES Default');
			$dopage->GetPage($sql,50,'UNIX_TIMESTAMP(LogTime) DESC');
			while($data = $dosql->GetArray())
			{
				$row[$k]['Uin'] = $data['Uin'];
				$row[$k]['oldwid'] = $data['ArmData_OldRoleArm_WID'];
				$row[$k]['newwid'] = $data['ArmData_NewRoleArm_WID'];
				$row[$k]['LoginName'] = $data['LoginName'];
				$row[$k]['RoleName'] = iconv("gbk","utf8",$data['RoleName']);
				$row[$k]['MachiningActType'] = $data['MachiningActType'];
				$row[$k]['LogTime'] = $data['LogTime'];  //品质
				if($data['MachiningActType']==0 )
				{
					$row[$k]['type'] = "装备强化星级(未保存)";
				}
				if($data['MachiningActType']==1 )
				{
					$row[$k]['type'] = "装备等级提升";
				}
				if($data['MachiningActType']==2 )
				{
					$row[$k]['type'] = "装备战斗力提升";
				}
				if($data['MachiningActType']==3 )
				{
					$row[$k]['type'] = "装备鉴定";
				}
				if($data['MachiningActType']==4 )
				{
					$row[$k]['type'] = "宝石升级";
				}
				if($data['MachiningActType']==5 )
				{
					$row[$k]['type'] = "宝石镶嵌";
				}
				if($data['MachiningActType']==6 )
				{
					$row[$k]['type'] = "宝石摘取";
				}
				if($data['MachiningActType']==7 )
				{
					$row[$k]['type'] = "装备开孔";
				}
				if($data['MachiningActType']==8 )
				{
					$row[$k]['type'] = "装备品质转移";
				}
				if($data['MachiningActType']==9 )
				{
					$row[$k]['type'] = "装备绑定";
				}
				if($data['MachiningActType']==10 )
				{
					$row[$k]['type'] = "装备解绑";
				}
				if($data['MachiningActType']==11 )
				{
					$row[$k]['type'] = "装备附魔";
				}
				if($data['MachiningActType']==12 )
				{
					$row[$k]['type'] = "装备刻印";
				}
				if($data['MachiningActType']==13 )
				{
					$row[$k]['type'] = "装备分解";
				}
				if($data['MachiningActType']==14 )
				{
					$row[$k]['type'] = "宝石分解";
				}
				if($data['MachiningActType']==15 )
				{
					$row[$k]['type'] = "宝石重置";
				}
				if($data['MachiningActType']==16 )
				{
					$row[$k]['type'] = "宝石充能";
				}
				if($data['MachiningActType']==17 )
				{
					$row[$k]['type'] = "装备重铸消耗";
				}
				if($data['MachiningActType']==18 )
				{
					$row[$k]['type'] = "装备重铸信息保存";
				}
				if($data['MachiningActType']==19 )
				{
					$row[$k]['type'] = "装备强化保存（星级）";
				}
				if($data['MachiningActType']==20 )
				{
					$row[$k]['type'] = "装备洗练";
				}
				if($data['MachiningActType']==21 )
				{
					$row[$k]['type'] = "装备洗练保存";
				}
				if($data['MachiningActType']==22 )
				{
					$row[$k]['type'] = "能量晶石吸收";
				}
				if($data['MachiningActType']==23 )
				{
					$row[$k]['type'] = "装备升级";
				}
				if($data['MachiningActType']==24 )
				{
					$row[$k]['type'] = "装备属性转移";
				}
				if($data['MachiningActType']==25 )
				{
					$row[$k]['type'] = "装备属性转移";
				}
				if($data['MachiningActType']==26 )
				{
					$row[$k]['type'] = "装备鉴定";
				}
				//ArmData_NewRoleArm_DefID
				$row[$k]['Item'] = $data['ArmData_OldRoleArm_DefID'];
				$sql = "SELECT `Name`,`Important` FROM ItemDef where `ItemID`='$data[ArmData_OldRoleArm_DefID]'";
				mysql_select_db('resdb');
				$rest = mysql_query($sql);
				$rowf = mysql_fetch_array($rest);
				if($rowf != "" && $rowf != false)
				{
					$row[$k]['Item'] = iconv("gbk","utf8",$rowf['Name']);
				}
				if(4 <= $rowf['Important'])
				{
					$row[$k]['Item'] .= "：粉装";
				}
				else if(3 == $rowf['Important'])
				{
					$row[$k]['Item'] .= "：紫装";
				}
				else if(2 == $rowf['Important'])
				{
					$row[$k]['Item'] .= "：蓝装";
				}
				else if(1 == $rowf['Important'])
				{
					$row[$k]['Item'] .= "：绿装";
				}
				else if(0 == $rowf['Important'])
				{
					$row[$k]['Item'] .= "：白装";
				}
				else if("" == $rowf['Important'] )
				{
					$row[$k]['Item'] .= "：无";
				}
				
				//强化前星级
				$row[$k]['oldlv'] = $data['ArmData_OldRoleArm_StarLv']."星：";
				for($ij=0;$ij<$data['ArmData_OldRoleArm_StarLv'];$ij++)
				{
					$ss = "ArmData_OldRoleArm_StarInfo".($ij+1)."_ValIdx";
					$zdj = $data[$ss];
					$zd = "StarInfo".($zdj+1)."_Mul";
					$sqls = "SELECT `$zd` FROM `ArmStarDef` where `Star`='$data[ArmData_OldRoleArm_StarLv]'";
					$xj = mysql_query($sqls);
					$xjs = mysql_fetch_array($xj);
					$row[$k]['oldlv'] .= $xjs[$zd].",";
				}
				//强化后星级
				if($data['MachiningActType']== 19)
				{
					$row[$k]['newlv'] = $data['ArmData_NewRoleArm_StarLv']."星：";
					for($ij=0;$ij<$data['ArmData_NewRoleArm_StarLv'];$ij++)
					{
						$ss = "ArmData_NewRoleArm_StarInfo".($ij+1)."_ValIdx";
						$zdj = $data[$ss];
						$zd = "StarInfo".($zdj+1)."_Mul";
						$sqls = "SELECT `$zd` FROM `ArmStarDef` where `Star`='$data[ArmData_NewRoleArm_StarLv]'";
						$xj = mysql_query($sqls);
						$xjs = mysql_fetch_array($xj);
						$row[$k]['newlv'] .= $xjs[$zd].",";
					}
				}
				else if($data['MachiningActType'] != 19)
				{
					$row[$k]['newlv'] = $row[$k]['oldlv'];
				}
				//装备升级后名称
				if($data['MachiningActType']== 23)
				{
					$row[$k]['Item2'] = $data['ArmData_NewRoleArm_DefID'];
					$sql = "SELECT `Name`,`Important` FROM ItemDef where `ItemID`='$data[ArmData_NewRoleArm_DefID]'";
					$rest = mysql_query($sql);
					$rowf = mysql_fetch_array($rest);
					if($rowf != "" && $rowf != false)
					{
						$row[$k]['Item2'] = iconv("gbk","utf8",$rowf['Name']);
					}
					if(4 <= $rowf['Important'])
					{
						$row[$k]['Item2'] .= "：粉装";
					}
					else if(3 == $rowf['Important'])
					{
						$row[$k]['Item2'] .= "：紫装";
					}
					else if(2 == $rowf['Important'])
					{
						$row[$k]['Item2'] .= "：蓝装";
					}
					else if(1 == $rowf['Important'])
					{
						$row[$k]['Item2'] .= "：绿装";
					}
					else if(0 == $rowf['Important'])
					{
						$row[$k]['Item2'] .= "：白装";
					}
					else if("" == $rowf['Important'] )
					{
						$row[$k]['Item2'] .= "：无";
					}
				}
				else if($data['MachiningActType']!= 23)
				{
					$row[$k]['Item2'] = "无变化";
				}
				//ArmData_NewRoleArm_StarInfo8_ValId
				
				$dosql->select_db($db);
				$row[$k]['Money'] = $data['ExpendData_Money']."：".$data['ExpendData_JinBi']."：".$data['ExpendData_JinQuan'];
				$k++;
			}
		}
		else if(!$dosql->IsTable($data_table))
		{
			echo json_encode("不存在数据表");
			exit;
		}
	}
	else
	{
		$fate=$end_table-$statr_table;
		$fag = $end_time-$statr_time;
		if("0"==$fate && $fag>=0)
		{
			$k=0;
			$data_table=$qz.$statr_table;
			if($dosql->IsTable($data_table))
			{			
				$sql = "select *  from $data_table where `$fg` like '%$Acc%'  and UNIX_TIMESTAMP(LogTime) >'$statr_time' and UNIX_TIMESTAMP(LogTime)<'$end_time' ";
				$dosql->ExecNoneQuery('SET NAMES Default');
				$dopage->GetPage($sql,50,'UNIX_TIMESTAMP(LogTime) DESC');
				while($data = $dosql->GetArray())
				{
				
					$row[$k]['Uin'] = $data['Uin'];					
					$row[$k]['oldwid'] = $data['ArmData_OldRoleArm_WID'];
					$row[$k]['newwid'] = $data['ArmData_NewRoleArm_WID'];
					$row[$k]['LoginName'] = $data['LoginName'];
					$row[$k]['RoleName'] = iconv("gbk","utf8",$data['RoleName']);
					$row[$k]['MachiningActType'] = $data['MachiningActType'];
					$row[$k]['LogTime'] = $data['LogTime'];  //品质
					if($data['MachiningActType']==0 )
					{
						$row[$k]['type'] = "装备品质提升";
					}
					if($data['MachiningActType']==1 )
					{
						$row[$k]['type'] = "装备等级提升";
					}
					if($data['MachiningActType']==2 )
					{
						$row[$k]['type'] = "装备战斗力提升";
					}
					if($data['MachiningActType']==3 )
					{
						$row[$k]['type'] = "装备鉴定";
					}
					if($data['MachiningActType']==4 )
					{
						$row[$k]['type'] = "宝石升级";
					}
					if($data['MachiningActType']==5 )
					{
						$row[$k]['type'] = "宝石镶嵌";
					}
					if($data['MachiningActType']==6 )
					{
						$row[$k]['type'] = "宝石摘取";
					}
					if($data['MachiningActType']==7 )
					{
						$row[$k]['type'] = "装备开孔";
					}
					if($data['MachiningActType']==8 )
					{
						$row[$k]['type'] = "装备品质转移";
					}
					if($data['MachiningActType']==9 )
					{
						$row[$k]['type'] = "装备绑定";
					}
					if($data['MachiningActType']==10 )
					{
						$row[$k]['type'] = "装备解绑";
					}
					if($data['MachiningActType']==11 )
					{
						$row[$k]['type'] = "装备附魔";
					}
					if($data['MachiningActType']==12 )
					{
						$row[$k]['type'] = "装备刻印";
					}
					if($data['MachiningActType']==13 )
					{
						$row[$k]['type'] = "装备分解";
					}
					if($data['MachiningActType']==14 )
					{
						$row[$k]['type'] = "宝石分解";
					}
					if($data['MachiningActType']==15 )
					{
						$row[$k]['type'] = "宝石重置";
					}
					if($data['MachiningActType']==16 )
					{
						$row[$k]['type'] = "宝石充能";
					}
					if($data['MachiningActType']==17 )
					{
						$row[$k]['type'] = "装备重铸消耗";
					}
					if($data['MachiningActType']==18 )
					{
						$row[$k]['type'] = "装备重铸信息保存";
					}
					if($data['MachiningActType']==19 )
					{
						$row[$k]['type'] = "装备品质提升保存（星级）";
					}
					if($data['MachiningActType']==20 )
					{
						$row[$k]['type'] = "装备洗练";
					}
					if($data['MachiningActType']==21 )
					{
						$row[$k]['type'] = "装备洗练保存";
					}
					if($data['MachiningActType']==22 )
					{
						$row[$k]['type'] = "能量晶石吸收";
					}
					if($data['MachiningActType']==23 )
					{
						$row[$k]['type'] = "装备升级";
					}
					if($data['MachiningActType']==24 )
					{
						$row[$k]['type'] = "装备属性转移";
					}
					if($data['MachiningActType']==25 )
					{
						$row[$k]['type'] = "装备属性转移";
					}
					if($data['MachiningActType']==26 )
					{
						$row[$k]['type'] = "装备鉴定";
					}
					$row[$k]['Item'] = $data['ArmData_OldRoleArm_DefID'];
					$sql = "SELECT `Name`,`Important` FROM ItemDef where `ItemID`='$data[ArmData_OldRoleArm_DefID]'";
					mysql_select_db('resdb');
					$rest = mysql_query($sql);
					$rowf = mysql_fetch_array($rest);
					if($rowf != "" && $rowf != false)
					{
						$row[$k]['Item'] = iconv("gbk","utf8",$rowf['Name']);
					}
					if(4 <= $rowf['Important'])
					{
						$row[$k]['Item'] .= "：粉装";
					}
					else if(3 == $rowf['Important'])
					{
						$row[$k]['Item'] .= "：紫装";
					}
					else if(2 == $rowf['Important'])
					{
						$row[$k]['Item'] .= "：蓝装";
					}
					else if(1 == $rowf['Important'])
					{
						$row[$k]['Item'] .= "：绿装";
					}
					else if(0 == $rowf['Important'])
					{
						$row[$k]['Item'] .= "：白装";
					}
					else if("" == $rowf['Important'] )
					{
						$row[$k]['Item'] .= "：无";
					}
					
					//强化前星级
					$row[$k]['oldlv'] = $data['ArmData_OldRoleArm_StarLv']."星：";
					for($ij=0;$ij<$data['ArmData_OldRoleArm_StarLv'];$ij++)
					{
						$ss = "ArmData_OldRoleArm_StarInfo".($ij+1)."_ValIdx";
						$zdj = $data[$ss];
						$zd = "StarInfo".($zdj+1)."_Mul";
						$sqls = "SELECT `$zd` FROM `ArmStarDef` where `Star`='$data[ArmData_OldRoleArm_StarLv]'";
						$xj = mysql_query($sqls);
						$xjs = mysql_fetch_array($xj);
						$row[$k]['oldlv'] .= $xjs[$zd].",";
					}
					//强化后星级
					if($data['MachiningActType']== 19)
					{
						$row[$k]['newlv'] = $data['ArmData_NewRoleArm_StarLv']."星：";
						for($ij=0;$ij<$data['ArmData_NewRoleArm_StarLv'];$ij++)
						{
							$ss = "ArmData_NewRoleArm_StarInfo".($ij+1)."_ValIdx";
							$zdj = $data[$ss];
							$zd = "StarInfo".($zdj+1)."_Mul";
							$sqls = "SELECT `$zd` FROM `ArmStarDef` where `Star`='$data[ArmData_NewRoleArm_StarLv]'";
							$xj = mysql_query($sqls);
							$xjs = mysql_fetch_array($xj);
							$row[$k]['newlv'] .= $xjs[$zd].",";
						}
					}
					else if($data['MachiningActType'] != 19)
					{
						$row[$k]['newlv'] = $row[$k]['oldlv'];
					}
					//装备升级后名称
					if($data['MachiningActType']== 23)
					{
						$row[$k]['Item2'] = $data['ArmData_NewRoleArm_DefID'];
						$sql = "SELECT `Name`,`Important` FROM ItemDef where `ItemID`='$data[ArmData_NewRoleArm_DefID]'";
						$rest = mysql_query($sql);
						$rowf = mysql_fetch_array($rest);
						if($rowf != "" && $rowf != false)
						{
							$row[$k]['Item2'] = iconv("gbk","utf8",$rowf['Name']);
						}
						if(4 <= $rowf['Important'])
						{
							$row[$k]['Item2'] .= "：粉装";
						}
						else if(3 == $rowf['Important'])
						{
							$row[$k]['Item2'] .= "：紫装";
						}
						else if(2 == $rowf['Important'])
						{
							$row[$k]['Item2'] .= "：蓝装";
						}
						else if(1 == $rowf['Important'])
						{
							$row[$k]['Item2'] .= "：绿装";
						}
						else if(0 == $rowf['Important'])
						{
							$row[$k]['Item2'] .= "：白装";
						}
						else if("" == $rowf['Important'] )
						{
							$row[$k]['Item2'] .= "：无";
						}
					}
					else if($data['MachiningActType']!= 23)
					{
						$row[$k]['Item2'] = "无变化";
					}
					//ArmData_NewRoleArm_StarInfo8_ValId
					
					$dosql->select_db($db);
					$row[$k]['Money'] = $data['ExpendData_Money']."：".$data['ExpendData_JinBi']."：".$data['ExpendData_JinQuan'];
					$k++;
				}	
			}
			else
			{
				echo json_encode("不存在数据表");
				exit;
			}
		}
		if(0< $fate && $fate < 3)
		{
			$k = 0;
			for($i=0;$i<=$fate;$i++)
			{
				$data_table[$i]=$qz.($statr_table+$i);
				if($dosql->IsTable($data_table[$i]))
				{
				
					if($i<$fate)
					{
						$sql = "select * from $data_table[$i] where `$fg` like '%$Acc%'  and UNIX_TIMESTAMP(LogTime) >'$statr_time' ";
					}
					if($i == $fate)
					{
						$sql = "select * from $data_table[$i] where `$fg` like '%$Acc%'  and UNIX_TIMESTAMP(LogTime) < '$end_time' ";
					}
					$dosql->ExecNoneQuery('SET NAMES Default');
					$dopage->GetPage($sql,50,'UNIX_TIMESTAMP(LogTime) DESC');
					while($data = $dosql->GetArray())
					{
					
						$row[$k]['Uin'] = $data['Uin'];						
						$row[$k]['oldwid'] = $data['ArmData_OldRoleArm_WID'];
						$row[$k]['newwid'] = $data['ArmData_NewRoleArm_WID'];
						$row[$k]['LoginName'] = $data['LoginName'];
						$row[$k]['RoleName'] = iconv("gbk","utf8",$data['RoleName']);
						$row[$k]['MachiningActType'] = $data['MachiningActType'];
						$row[$k]['LogTime'] = $data['LogTime'];  //品质
						if($data['MachiningActType']==0 )
						{
							$row[$k]['type'] = "装备品质提升";
						}
						if($data['MachiningActType']==1 )
						{
							$row[$k]['type'] = "装备等级提升";
						}
						if($data['MachiningActType']==2 )
						{
							$row[$k]['type'] = "装备战斗力提升";
						}
						if($data['MachiningActType']==3 )
						{
							$row[$k]['type'] = "装备鉴定";
						}
						if($data['MachiningActType']==4 )
						{
							$row[$k]['type'] = "宝石升级";
						}
						if($data['MachiningActType']==5 )
						{
							$row[$k]['type'] = "宝石镶嵌";
						}
						if($data['MachiningActType']==6 )
						{
							$row[$k]['type'] = "宝石摘取";
						}
						if($data['MachiningActType']==7 )
						{
							$row[$k]['type'] = "装备开孔";
						}
						if($data['MachiningActType']==8 )
						{
							$row[$k]['type'] = "装备品质转移";
						}
						if($data['MachiningActType']==9 )
						{
							$row[$k]['type'] = "装备绑定";
						}
						if($data['MachiningActType']==10 )
						{
							$row[$k]['type'] = "装备解绑";
						}
						if($data['MachiningActType']==11 )
						{
							$row[$k]['type'] = "装备附魔";
						}
						if($data['MachiningActType']==12 )
						{
							$row[$k]['type'] = "装备刻印";
						}
						if($data['MachiningActType']==13 )
						{
							$row[$k]['type'] = "装备分解";
						}
						if($data['MachiningActType']==14 )
						{
							$row[$k]['type'] = "宝石分解";
						}
						if($data['MachiningActType']==15 )
						{
							$row[$k]['type'] = "宝石重置";
						}
						if($data['MachiningActType']==16 )
						{
							$row[$k]['type'] = "宝石充能";
						}
						if($data['MachiningActType']==17 )
						{
							$row[$k]['type'] = "装备重铸消耗";
						}
						if($data['MachiningActType']==18 )
						{
							$row[$k]['type'] = "装备重铸信息保存";
						}
						if($data['MachiningActType']==19 )
						{
							$row[$k]['type'] = "装备品质提升保存（星级）";
						}
						if($data['MachiningActType']==20 )
						{
							$row[$k]['type'] = "装备洗练";
						}
						if($data['MachiningActType']==21 )
						{
							$row[$k]['type'] = "装备洗练保存";
						}
						if($data['MachiningActType']==22 )
						{
							$row[$k]['type'] = "能量晶石吸收";
						}
						if($data['MachiningActType']==23 )
						{
							$row[$k]['type'] = "装备升级";
						}
						if($data['MachiningActType']==24 )
						{
							$row[$k]['type'] = "装备属性转移";
						}
						if($data['MachiningActType']==25 )
						{
							$row[$k]['type'] = "装备属性转移";
						}
						if($data['MachiningActType']==26 )
						{
							$row[$k]['type'] = "装备鉴定";
						}
						$row[$k]['Item'] = $data['ArmData_OldRoleArm_DefID'];
						$sql = "SELECT `Name`,`Important` FROM ItemDef where `ItemID`='$data[ArmData_OldRoleArm_DefID]'";
						mysql_select_db('resdb');
						$rest = mysql_query($sql);
						$rowf = mysql_fetch_array($rest);
						if($rowf != "" && $rowf != false)
						{
							$row[$k]['Item'] = iconv("gbk","utf8",$rowf['Name']);
						}
						if(4 <= $rowf['Important'])
						{
							$row[$k]['Item'] .= "：粉装";
						}
						else if(3 == $rowf['Important'])
						{
							$row[$k]['Item'] .= "：紫装";
						}
						else if(2 == $rowf['Important'])
						{
							$row[$k]['Item'] .= "：蓝装";
						}
						else if(1 == $rowf['Important'])
						{
							$row[$k]['Item'] .= "：绿装";
						}
						else if(0 == $rowf['Important'])
						{
							$row[$k]['Item'] .= "：白装";
						}
						else if("" == $rowf['Important'] )
						{
							$row[$k]['Item'] .= "：无";
						}
							
						//强化前星级
						$row[$k]['oldlv'] = $data['ArmData_OldRoleArm_StarLv']."星：";
						for($ij=0;$ij<$data['ArmData_OldRoleArm_StarLv'];$ij++)
						{
							$ss = "ArmData_OldRoleArm_StarInfo".($ij+1)."_ValIdx";
							$zdj = $data[$ss];
							$zd = "StarInfo".($zdj+1)."_Mul";
							$sqls = "SELECT `$zd` FROM `ArmStarDef` where `Star`='$data[ArmData_OldRoleArm_StarLv]'";
							$xj = mysql_query($sqls);
							$xjs = mysql_fetch_array($xj);
							$row[$k]['oldlv'] .= $xjs[$zd].",";
						}
						//强化后星级
						if($data['MachiningActType']== 19)
						{
							$row[$k]['newlv'] = $data['ArmData_NewRoleArm_StarLv']."星：";
							for($ij=0;$ij<$data['ArmData_NewRoleArm_StarLv'];$ij++)
							{
								$ss = "ArmData_NewRoleArm_StarInfo".($ij+1)."_ValIdx";
								$zdj = $data[$ss];
								$zd = "StarInfo".($zdj+1)."_Mul";
								$sqls = "SELECT `$zd` FROM `ArmStarDef` where `Star`='$data[ArmData_NewRoleArm_StarLv]'";
								$xj = mysql_query($sqls);
								$xjs = mysql_fetch_array($xj);
								$row[$k]['newlv'] .= $xjs[$zd].",";
							}
						}
						else if($data['MachiningActType'] != 19)
						{
							$row[$k]['newlv'] = $row[$k]['oldlv'];
						}
						//装备升级后名称
						if($data['MachiningActType']== 23)
						{
								$row[$k]['Item2'] = $data['ArmData_NewRoleArm_DefID'];
								$sql = "SELECT `Name`,`Important` FROM ItemDef where `ItemID`='$data[ArmData_NewRoleArm_DefID]'";
								$rest = mysql_query($sql);
								$rowf = mysql_fetch_array($rest);
								if($rowf != "" && $rowf != false)
								{
									$row[$k]['Item2'] = iconv("gbk","utf8",$rowf['Name']);
								}
								if(4 <= $rowf['Important'])
								{
									$row[$k]['Item2'] .= "：粉装";
								}
								else if(3 == $rowf['Important'])
								{
									$row[$k]['Item2'] .= "：紫装";
								}
								else if(2 == $rowf['Important'])
								{
									$row[$k]['Item2'] .= "：蓝装";
								}
								else if(1 == $rowf['Important'])
								{
									$row[$k]['Item2'] .= "：绿装";
								}
								else if(0 == $rowf['Important'])
								{
									$row[$k]['Item2'] .= "：白装";
								}
								else if("" == $rowf['Important'] )
								{
									$row[$k]['Item2'] .= "：无";
								}
						}
						else if($data['MachiningActType']!= 23)
						{
							$row[$k]['Item2'] = "无变化";
						}
						$dosql->select_db($db);
						$row[$k]['Money'] = $data['ExpendData_Money']."：".$data['ExpendData_JinBi']."：".$data['ExpendData_JinQuan'];
						$k++;
					}
				}
				else
				{
					continue;
				}
			}
		}
		if( $fate >= 3){
			echo json_encode("结束时间与开始时间相差不能超过3天");
			exit;
		}
		if(0>$fate)
		{
			echo json_encode("结束时间不能低于开始时间");
			exit;
		}
		if(0==$fate && $fag<0)
		{
			echo json_encode("结束时间不能低于开始时间");
			exit;
		}
	}
	//$row['nums'] = $nums;
	echo json_encode($row);
}
//玩家系统挂单
if(isset($_POST['logstore']))
{
	$nums = 0;
	$row[]="";
	$db1 = $_POST['xzdb'];
	$fg = $_POST['fag'];
	if($fg == 1){ $fg = "StoreOwnerName";}
	else if($fg == 2){ $fg = "BuyName";}
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
	
	$qz="LogStore_";
	$Acc=$_POST['us'];
	$Acc = iconv("utf8","gbk",$Acc);
	
	date_default_timezone_set('PRC');
	//提交过来的开始时间的int
	$statr_time=strtotime($_POST['time']);

	//获取开始数据表
	$statr_table=date('ymd',$statr_time);
	
	//提交过来的结束时间
	$end_time=strtotime($_POST['time1']);
	
	//获取结束数据表
	$end_table=date('ymd',$end_time);
	
	
	//未填写结束时间
	if("" == $end_time || '0' == $end_time)
	{
		$data_table=$qz.$statr_table;
		$k=0;
		if($dosql->IsTable($data_table))
		{
			$sql = "select * from $data_table where `$fg` = '$Acc' ";
			
			$dosql->ExecNoneQuery('SET NAMES Default');
			$dopage->GetPage($sql,50,'UNIX_TIMESTAMP(LogTime) DESC');
			while($data = $dosql->GetArray())
			{
				$row[$k]['StoreName'] = iconv("gbk","utf8",$data['StoreName']);
				$row[$k]['StoreOwnerName'] = iconv("gbk","utf8",$data['StoreOwnerName']);
				$row[$k]['BuyName'] = iconv("gbk","utf8",$data['BuyName']);
				$row[$k]['Type'] = $data['Type'];
				if($data['Type'] == 0) $row[$k]['Type'] = "开店";
				if($data['Type'] == 1) $row[$k]['Type'] = "上架";
				if($data['Type'] == 2) $row[$k]['Type'] = "下架";
				if($data['Type'] == 3) $row[$k]['Type'] = "卖 ";
				if($data['Type'] == 4) $row[$k]['Type'] = "关店";
				if($data['Type'] == 5) $row[$k]['Type'] = "取钱";
				if($data['Type'] == 6) $row[$k]['Type'] = "存钱";
				if($data['Type'] == 7) $row[$k]['Type'] = "扣店铺租金";
				$row[$k]['PreMoney'] = $data['PreMoney']; //购买操作之前的钱
				$row[$k]['CurrMoney'] = $data['CurrMoney'];
				$row[$k]['Money'] = $data['Money'];
				$row[$k]['Num'] = $data['Num'];
				$row[$k]['time'] = $data['LogTime'];
								
				$sql = "SELECT `Name` FROM ItemDef where `ItemID`='$data[Grid_DefID]'";
				mysql_select_db('resdb');
				$rest = mysql_query($sql);
				$rowf = mysql_fetch_array($rest);
				if($rowf != "" && $rowf != false)
				{
					$row[$k]['daoju'] = iconv("gbk","utf8",$rowf['Name']);
				}
				if($data['Grid_Type'] == 0)
				{
					$row[$k]['daoju'] = "道具：".iconv("gbk","utf8",$rowf['Name']);
				}
				else if($data['Grid_Type'] == 1)
				{
					$row[$k]['daoju'] = "装备：".iconv("gbk","utf8",$rowf['Name'])."(唯一ID：".$data['Grid_WID'].")";
				}
				$dosql->select_db($db);
				$k++;
			}
			$nums = count($row);
		}
		else if(!$dosql->IsTable($data_table))
		{
			echo json_encode("不存在数据表");
			exit;
		}
	}
	else
	{
		$fate=$end_table-$statr_table;
		$fag = $end_time-$statr_time;
		if("0"==$fate && $fag>=0)
		{
			$k=0;
			$data_table=$qz.$statr_table;
			if($dosql->IsTable($data_table))
			{
			
				$sql = "select *  from $data_table where `$fg` = '$Acc'  and UNIX_TIMESTAMP(LogTime) >'$statr_time' and UNIX_TIMESTAMP(LogTime)<'$end_time' ";
				$dosql->ExecNoneQuery('SET NAMES Default');
				$dopage->GetPage($sql,50,'UNIX_TIMESTAMP(LogTime) DESC');
				while($data = $dosql->GetArray())
				{					
					$row[$k]['StoreName'] = iconv("gbk","utf8",$data['StoreName']);
					$row[$k]['StoreOwnerName'] = iconv("gbk","utf8",$data['StoreOwnerName']);
					$row[$k]['BuyName'] = iconv("gbk","utf8",$data['BuyName']);
					$row[$k]['Type'] = $data['Type'];
					if($data['Type'] == 0) $row[$k]['Type'] = "开店";
					if($data['Type'] == 1) $row[$k]['Type'] = "上架";
					if($data['Type'] == 2) $row[$k]['Type'] = "下架";
					if($data['Type'] == 3) $row[$k]['Type'] = "卖 ";
					if($data['Type'] == 4) $row[$k]['Type'] = "关店";
					if($data['Type'] == 5) $row[$k]['Type'] = "取钱";
					if($data['Type'] == 6) $row[$k]['Type'] = "存钱";
					if($data['Type'] == 7) $row[$k]['Type'] = "扣店铺租金";
					$row[$k]['PreMoney'] = $data['PreMoney']; //购买操作之前的钱
					$row[$k]['CurrMoney'] = $data['CurrMoney'];
					$row[$k]['Money'] = $data['Money'];
					$row[$k]['Num'] = $data['Num'];
					$row[$k]['time'] = $data['LogTime'];
									
					$sql = "SELECT `Name` FROM ItemDef where `ItemID`='$data[Grid_DefID]'";
					mysql_select_db('resdb');
					$rest = mysql_query($sql);
					$rowf = mysql_fetch_array($rest);
					if($rowf != "" && $rowf != false)
					{
						$row[$k]['daoju'] = iconv("gbk","utf8",$rowf['Name']);
					}
					if($data['Grid_Type'] == 0)
					{
						$row[$k]['daoju'] = "道具：".iconv("gbk","utf8",$rowf['Name']);
					}
					else if($data['Grid_Type'] == 1)
					{
						$row[$k]['daoju'] = "装备：".iconv("gbk","utf8",$rowf['Name'])."(唯一ID：".$data['Grid_WID'].")";
					}
					$dosql->select_db($db);
					$k++;
					
				}			
			}
			else
			{
				echo json_encode("不存在数据表");
				exit;
			}
		}
		if(0< $fate && $fate < 3)
		{
			$k = 0;
			for($i=0;$i<=$fate;$i++)
			{
				$data_table[$i]=$qz.($statr_table+$i);
				if($dosql->IsTable($data_table[$i]))
				{
				
					if($i<$fate)
					{
						$sql = "select * from $data_table[$i] where `$fg` = '$Acc'  and UNIX_TIMESTAMP(LogTime) >'$statr_time' ";
					}
					if($i == $fate)
					{
						$sql = "select * from $data_table[$i] where `$fg` = '$Acc'  and UNIX_TIMESTAMP(LogTime) < '$end_time' ";
					}
					$dosql->ExecNoneQuery('SET NAMES Default');
					$dopage->GetPage($sql,50,'UNIX_TIMESTAMP(LogTime) DESC');
					while($data = $dosql->GetArray())
					{				
						$row[$k]['StoreName'] = iconv("gbk","utf8",$data['StoreName']);
						$row[$k]['StoreOwnerName'] = iconv("gbk","utf8",$data['StoreOwnerName']);
						$row[$k]['BuyName'] = iconv("gbk","utf8",$data['BuyName']);
						$row[$k]['Type'] = $data['Type'];
						if($data['Type'] == 0) $row[$k]['Type'] = "开店";
						if($data['Type'] == 1) $row[$k]['Type'] = "上架";
						if($data['Type'] == 2) $row[$k]['Type'] = "下架";
						if($data['Type'] == 3) $row[$k]['Type'] = "卖 ";
						if($data['Type'] == 4) $row[$k]['Type'] = "关店";
						if($data['Type'] == 5) $row[$k]['Type'] = "取钱";
						if($data['Type'] == 6) $row[$k]['Type'] = "存钱";
						if($data['Type'] == 7) $row[$k]['Type'] = "扣店铺租金";
						$row[$k]['PreMoney'] = $data['PreMoney']; //购买操作之前的钱
						$row[$k]['CurrMoney'] = $data['CurrMoney'];
						$row[$k]['Money'] = $data['Money'];
						$row[$k]['Num'] = $data['Num'];
						$row[$k]['time'] = $data['LogTime'];
										
						$sql = "SELECT `Name` FROM ItemDef where `ItemID`='$data[Grid_DefID]'";
						mysql_select_db('resdb');
						$rest = mysql_query($sql);
						$rowf = mysql_fetch_array($rest);
						if($rowf != "" && $rowf != false)
						{
							$row[$k]['daoju'] = iconv("gbk","utf8",$rowf['Name']);
						}
						if($data['Grid_Type'] == 0)
						{
							$row[$k]['daoju'] = "道具：".iconv("gbk","utf8",$rowf['Name']);
						}
						else if($data['Grid_Type'] == 1)
						{
							$row[$k]['daoju'] = "装备：".iconv("gbk","utf8",$rowf['Name'])."(唯一ID：".$data['Grid_WID'].")";
						}
						$dosql->select_db($db);
						$k++;
					}
				}
				else
				{
					continue;
				}
			}
		}
		if( $fate >= 3){
			echo json_encode("结束时间与开始时间相差不能超过3天");
			exit;
		}
		if(0>$fate)
		{
			echo json_encode("结束时间不能低于开始时间");
			exit;
		}
		if(0==$fate && $fag<0)
		{
			echo json_encode("结束时间不能低于开始时间");
			exit;
		}
	}
	//$row['nums'] = $nums;
	echo json_encode($row);
}
?>
