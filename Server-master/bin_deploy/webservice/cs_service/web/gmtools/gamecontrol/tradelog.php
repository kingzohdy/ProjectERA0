<?php
require_once(dirname(__FILE__).'/../inc/config.inc.php');
if($_SESSION['type'] != "5")
{
	$_SESSION = array();
	session_destroy();
	echo '<script type="text/javascript">window.top.location.href="login.php";</script>';
	exit();
}
$uid = $_SESSION['id']; 
$sql = "select * from `adminor` where `id` = '$uid' ";
$dosql->Execute($sql);
$user = $dosql->GetArray();
if(isset($_POST['tradeing']))
{
	$rid = $_POST['rid'];
	$wid = $_POST['wid'];
	$db1 = $rid."_".$wid;
	$sql = "select * from `reg_world2` where `dbid`='$db1' ";
	//外
	//$resdb = "world_".$rid."_".$wid;
	//内
	$resdb = "world_".$wid;
	$dosql->Execute($sql);
	$fname = $dosql->GetArray();
	
	$GLOBALS['db_host'] = $fname['dbip'];
	$GLOBALS['db_user'] = $fname['dbuser'];
	$GLOBALS['db_name'] = $resdb;
	$GLOBALS['db_pwd'] = $fname['dbpass'];
	
	$dosql->Close(true);
	$dosql = new MySql(false);
	$dosql->ExecNoneQuery('SET NAMES Default');
	date_default_timezone_set('PRC');
	$ting = time();
	$sql = "SELECT * FROM `TradeSell` ";
	$dosql->Execute($sql);
	$i=0;
	while ( $row = $dosql->GetArray())
	{
		$data['sell'][$i] = $row;
		$i++;	
	}
	$data['count'] = $i;
	$sql2 = "SELECT * FROM `TradeBuy` ";
	$dosql->Execute($sql2);
	$j=0;
	while ($row = $dosql->GetArray())
	{
		$data['buy'][$j] = $row;
		$j++;
	}
	$data['count2'] = $j;
	echo json_encode($data);
	exit;
}
if(isset($_POST['sbuylog']))
{
	$rid = $_POST['rid'];
	$wid = $_POST['wid'];
	date_default_timezone_set('PRC');
	$stime = $_POST['t1'];
	$etime = $_POST['t2'];
	$qz="OpTradeInfo_";

	$statr_time=strtotime($_POST['t1']);
	$end_time=strtotime($_POST['t2']);
	
	$statr_table=date('ymd',$statr_time);
	$end_table=date('ymd',$end_time);
	
	$db1 = $rid."_".$wid;
	$sql = "select * from `reg_world2` where `dbid`='$db1' ";
	
	$resdb = "world_log_".$rid."_".$wid;
	
	$dosql->Execute($sql);
	$fname = $dosql->GetArray();
	
	$GLOBALS['db_host'] = $fname['dbip'];
	$GLOBALS['db_user'] = $fname['dbuser'];
	$GLOBALS['db_name'] = $resdb;
	$GLOBALS['db_pwd'] = $fname['dbpass'];
	
	$dosql->Close(true);
	$dosql = new MySql(false);
	$dosql->ExecNoneQuery('SET NAMES Default');
	date_default_timezone_set('PRC');
	$ting = time();
	//未填写结束时间
	if("" == $end_time || '0' == $end_time)
	{
		$data_table=$qz.$statr_table;
		if($dosql->IsTable($data_table))
		{
			$row['error'] = 0;
			$sqlg1 = "select count(*) as 'g10' from $data_table  where `Flag` = 8661 and  `Gold` = 10" ;
			$sqlg2 = "select count(*) as 'g50' from $data_table  where `Flag` = 8661 and  `Gold` = 50" ;
			$sqlg3 = "select count(*) as 'g100' from $data_table  where `Flag` = 8661 and  `Gold` = 100" ;
			$sqlg4 = "select count(*) as 'g500' from $data_table  where `Flag` = 8661 and  `Gold` = 500" ;
			$sqlg5 = "select count(*) as 'g1000' from $data_table  where `Flag` = 8661 and  `Gold` = 1000" ;
			
			$g10 = $dosql->GetOne($sqlg1);
			$g50 = $dosql->GetOne($sqlg2);
			$g100 = $dosql->GetOne($sqlg3);
			$g500 = $dosql->GetOne($sqlg4);
			$g1000 = $dosql->GetOne($sqlg5);
			$row['rg10'] = (int)$g10['g10'];
			$row['rg50'] = (int)$g50['g50'];
			$row['rg100'] = (int)$g100['g100'];
			$row['rg500'] = (int)$g500['g500'];
			$row['rg1000'] = (int)$g1000['g1000'];
			
			$sqlm1 = "select count(*) as 'm10' from $data_table  where `Flag` = 8662 and  `Money` = 1000";
			$sqlm2 = "select count(*) as 'm50' from $data_table  where `Flag` = 8662 and  `Money` = 5000";
			$sqlm3 = "select count(*) as 'm100' from $data_table  where `Flag` = 8662 and  `Money` = 10000";
			$sqlm4 = "select count(*) as 'm500' from $data_table  where `Flag` = 8662 and  `Money` = 50000";
			$sqlm5 = "select count(*) as 'm1000' from $data_table  where `Flag` = 8662 and  `Money` = 100000";
			$m10 = $dosql->GetOne($sqlm1);
			$m50 = $dosql->GetOne($sqlm2);
			$m100 = $dosql->GetOne($sqlm3);
			$m500 = $dosql->GetOne($sqlm4);
			$m1000 = $dosql->GetOne($sqlm5);
			$row['rm10'] = (int)$m10['m10'];
			$row['rm50'] = (int)$m50['m50'];
			$row['rm100'] = (int)$m100['m100'];
			$row['rm500'] = (int)$m500['m500'];
			$row['rm1000'] = (int)$m1000['m1000'];
			
			
			$sqlsm1 = "select count(*) as 'sm10' from $data_table  where `Flag` = 9991 and `Money` = 1000";
			$sqlsm2 = "select count(*) as 'sm50' from $data_table  where `Flag` = 9991 and `Money` = 5000";
			$sqlsm3 = "select count(*) as 'sm100' from $data_table  where `Flag` = 9991 and `Money` = 10000";
			$sqlsm4 = "select count(*) as 'sm500' from $data_table  where `Flag` = 9991 and `Money` = 50000";
			$sqlsm5 = "select count(*) as 'sm1000' from $data_table  where `Flag` = 9991 and `Money` = 100000";
			$sm10 = $dosql->GetOne($sqlsm1);
			$sm50 = $dosql->GetOne($sqlsm2);
			$sm100 = $dosql->GetOne($sqlsm3);
			$sm500 = $dosql->GetOne($sqlsm4);
			$sm1000 = $dosql->GetOne($sqlsm5);
			$row['sm10'] = (int)$sm10['sm10'];
			$row['sm50'] = (int)$sm50['sm50'];
			$row['sm100'] = (int)$sm100['sm100'];
			$row['sm500'] = (int)$sm500['sm500'];
			$row['sm1000'] = (int)$sm1000['sm1000'];
			
			
			$sqlsg1 = "select count(*) as 'sg10' from $data_table  where `Flag` = 9993 and `Gold` = 10 ";
			$sqlsg2 = "select count(*) as 'sg50' from $data_table  where `Flag` = 9993 and `Gold` = 50 ";
			$sqlsg3 = "select count(*) as 'sg100' from $data_table  where `Flag` = 9993 and `Gold` = 100 ";
			$sqlsg4 = "select count(*) as 'sg500' from $data_table  where `Flag` = 9993 and `Gold` = 500 ";
			$sqlsg5 = "select count(*) as 'sg1000' from $data_table  where `Flag` = 9993 and `Gold` = 1000 ";
			$sg10 = $dosql->GetOne($sqlsg1);
			$sg50 = $dosql->GetOne($sqlsg2);
			$sg100 = $dosql->GetOne($sqlsg3);
			$sg500 = $dosql->GetOne($sqlsg4);
			$sg1000 = $dosql->GetOne($sqlsg5);
			$row['sg10'] = (int)$sg10['sg10'];
			$row['sg50'] = (int)$sg50['sg50'];
			$row['sg100'] = (int)$sg100['sg100'];
			$row['sg500'] = (int)$sg500['sg500'];
			$row['sg1000'] = (int)$sg1000['sg1000'];
		}
		else if(!$dosql->IsTable($data_table))
		{
			$row['error'] = "不存在数据表";
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
				$row['error'] = 0;	
				$sqlg1 = "select count(*) as 'g10' from $data_table where `Flag` = 8661 and  `Gold` = 10  and UNIX_TIMESTAMP(LogTime) >'$statr_time' and UNIX_TIMESTAMP(LogTime)<'$end_time' ";
				$sqlg2 = "select count(*) as 'g50' from $data_table where `Flag` = 8661 and  `Gold` = 50  and UNIX_TIMESTAMP(LogTime) >'$statr_time' and UNIX_TIMESTAMP(LogTime)<'$end_time' ";
				$sqlg3 = "select count(*) as 'g100' from $data_table where `Flag` = 8661 and  `Gold` = 100  and UNIX_TIMESTAMP(LogTime) >'$statr_time' and UNIX_TIMESTAMP(LogTime)<'$end_time' ";
				$sqlg4 = "select count(*) as 'g500' from $data_table where `Flag` = 8661 and  `Gold` = 500  and UNIX_TIMESTAMP(LogTime) >'$statr_time' and UNIX_TIMESTAMP(LogTime)<'$end_time' ";
				$sqlg5 = "select count(*) as 'g1000' from $data_table where `Flag` = 8661 and  `Gold` = 1000  and UNIX_TIMESTAMP(LogTime) >'$statr_time' and UNIX_TIMESTAMP(LogTime)<'$end_time' ";
				$g10 = $dosql->GetOne($sqlg1);
				$g50 = $dosql->GetOne($sqlg2);
				$g100 = $dosql->GetOne($sqlg3);
				$g500 = $dosql->GetOne($sqlg4);
				$g1000 = $dosql->GetOne($sqlg5);
				
				$row['rg10'] = (int)$g10['g10'];
				$row['rg50'] = (int)$g50['g50'];
				$row['rg100'] = (int)$g100['g100'];
				$row['rg500'] = (int)$g500['g500'];
				$row['rg1000'] = (int)$g1000['g1000'];				
			
				$sqlm1 = "select count(*) as 'm10' from $data_table where `Flag` = 8662  and  `Money` = 1000 and UNIX_TIMESTAMP(LogTime) >'$statr_time' and UNIX_TIMESTAMP(LogTime)<'$end_time' ";				
				$sqlm2 = "select count(*) as 'm50' from $data_table where `Flag` = 8662  and  `Money` = 5000 and UNIX_TIMESTAMP(LogTime) >'$statr_time' and UNIX_TIMESTAMP(LogTime)<'$end_time' ";
				$sqlm3 = "select count(*) as 'm100' from $data_table where `Flag` = 8662  and  `Money` = 10000 and UNIX_TIMESTAMP(LogTime) >'$statr_time' and UNIX_TIMESTAMP(LogTime)<'$end_time' ";
				$sqlm4 = "select count(*) as 'm500' from $data_table where `Flag` = 8662  and  `Money` = 50000 and UNIX_TIMESTAMP(LogTime) >'$statr_time' and UNIX_TIMESTAMP(LogTime)<'$end_time' ";
				$sqlm5 = "select count(*) as 'm1000' from $data_table where `Flag` = 8662  and  `Money` = 100000 and UNIX_TIMESTAMP(LogTime) >'$statr_time' and UNIX_TIMESTAMP(LogTime)<'$end_time' ";				
				$m10 = $dosql->GetOne($sqlm1);				
				$m50 = $dosql->GetOne($sqlm2);
				$m100 = $dosql->GetOne($sqlm3);
				$m500 = $dosql->GetOne($sqlm4);
				$m1000 = $dosql->GetOne($sqlm5);
				$row['rm10'] = (int)$m10['m10'];
				$row['rm50'] = (int)$m50['m50'];
				$row['rm100'] = (int)$m100['m100'];
				$row['rm500'] = (int)$m500['m500'];
				$row['rm1000'] = (int)$m1000['m1000'];				
				
				$sqlsm1 = "select count(*) as 'sm10' from $data_table where `Flag` = 9991  and  `Money` = 1000 and UNIX_TIMESTAMP(LogTime) >'$statr_time' and UNIX_TIMESTAMP(LogTime)<'$end_time' ";
				$sqlsm2 = "select count(*) as 'sm50' from $data_table where `Flag` = 9991  and  `Money` = 5000 and UNIX_TIMESTAMP(LogTime) >'$statr_time' and UNIX_TIMESTAMP(LogTime)<'$end_time' ";
				$sqlsm3 = "select count(*) as 'sm100' from $data_table where `Flag` = 9991  and  `Money` = 10000 and UNIX_TIMESTAMP(LogTime) >'$statr_time' and UNIX_TIMESTAMP(LogTime)<'$end_time' ";
				$sqlsm4 = "select count(*) as 'sm500' from $data_table where `Flag` = 9991  and  `Money` = 50000 and UNIX_TIMESTAMP(LogTime) >'$statr_time' and UNIX_TIMESTAMP(LogTime)<'$end_time' ";
				$sqlsm5 = "select count(*) as 'sm1000' from $data_table where `Flag` = 9991  and  `Money` = 100000 and UNIX_TIMESTAMP(LogTime) >'$statr_time' and UNIX_TIMESTAMP(LogTime)<'$end_time' ";
				$sm10 = $dosql->GetOne($sqlsm1);
				$sm50 = $dosql->GetOne($sqlsm2);
				$sm100 = $dosql->GetOne($sqlsm3);
				$sm500 = $dosql->GetOne($sqlsm4);
				$sm1000 = $dosql->GetOne($sqlsm5);
				$row['sm10'] = (int)$sm10['sm10'];
				$row['sm50'] = (int)$sm50['sm50'];
				$row['sm100'] = (int)$sm100['sm100'];
				$row['sm500'] = (int)$sm500['sm500'];
				$row['sm1000'] = (int)$sm1000['sm1000'];
								
				$sqlsg1 = "select count(*) as 'sg10' from $data_table where `Flag` = 9993  and  `Gold` = 10 and UNIX_TIMESTAMP(LogTime) >'$statr_time' and UNIX_TIMESTAMP(LogTime)<'$end_time' ";
				$sqlsg2 = "select count(*) as 'sg50' from $data_table where `Flag` = 9993  and  `Gold` = 50 and UNIX_TIMESTAMP(LogTime) >'$statr_time' and UNIX_TIMESTAMP(LogTime)<'$end_time' ";
				$sqlsg3 = "select count(*) as 'sg100' from $data_table where `Flag` = 9993  and  `Gold` = 100 and UNIX_TIMESTAMP(LogTime) >'$statr_time' and UNIX_TIMESTAMP(LogTime)<'$end_time' ";
				$sqlsg4 = "select count(*) as 'sg500' from $data_table where `Flag` = 9993  and  `Gold` = 500 and UNIX_TIMESTAMP(LogTime) >'$statr_time' and UNIX_TIMESTAMP(LogTime)<'$end_time' ";
				$sqlsg5 = "select count(*) as 'sg1000' from $data_table where `Flag` = 9993  and  `Gold` = 1000 and UNIX_TIMESTAMP(LogTime) >'$statr_time' and UNIX_TIMESTAMP(LogTime)<'$end_time' ";
				$sg10 = $dosql->GetOne($sqlsg1);
				$sg50 = $dosql->GetOne($sqlsg2);
				$sg100 = $dosql->GetOne($sqlsg3);
				$sg500 = $dosql->GetOne($sqlsg4);
				$sg1000 = $dosql->GetOne($sqlsg5);
				$row['sg10'] = (int)$sg10['sg10'];
				$row['sg50'] = (int)$sg50['sg50'];
				$row['sg100'] = (int)$sg100['sg100'];
				$row['sg500'] = (int)$sg500['sg500'];
				$row['sg1000'] = (int)$sg1000['sg1000'];		
					
			}
			else
			{
				$row['error'] = "不存在数据表";
			}
		}
		if(0< $fate)
		{
			for($i=0;$i<=$fate;$i++)
			{
				$data_table[$i]=$qz.($statr_table+$i);
				if($dosql->IsTable($data_table[$i]))
				{
					$row['error'] = 0;
					$sqlg1 = "select count(*) as 'g10' from $data_table[$i] where `Flag` = 8661 and  `Gold` = 10  and UNIX_TIMESTAMP(LogTime) >'$statr_time' and UNIX_TIMESTAMP(LogTime)<'$end_time' ";
					$sqlg2 = "select count(*) as 'g50' from $data_table[$i] where `Flag` = 8661 and  `Gold` = 50  and UNIX_TIMESTAMP(LogTime) >'$statr_time' and UNIX_TIMESTAMP(LogTime)<'$end_time' ";
					$sqlg3 = "select count(*) as 'g100' from $data_table[$i] where `Flag` = 8661 and  `Gold` = 100  and UNIX_TIMESTAMP(LogTime) >'$statr_time' and UNIX_TIMESTAMP(LogTime)<'$end_time' ";
					$sqlg4 = "select count(*) as 'g500' from $data_table[$i] where `Flag` = 8661 and  `Gold` = 500  and UNIX_TIMESTAMP(LogTime) >'$statr_time' and UNIX_TIMESTAMP(LogTime)<'$end_time' ";
					$sqlg5 = "select count(*) as 'g1000' from $data_table[$i] where `Flag` = 8661 and  `Gold` = 1000  and UNIX_TIMESTAMP(LogTime) >'$statr_time' and UNIX_TIMESTAMP(LogTime)<'$end_time' ";
					$g10 = $dosql->GetOne($sqlg1);
					$g50 = $dosql->GetOne($sqlg2);
					$g100 = $dosql->GetOne($sqlg3);
					$g500 = $dosql->GetOne($sqlg4);
					$g1000 = $dosql->GetOne($sqlg5);
					
					$row['rg10'] += (int)$g10['g10'];
					$row['rg50'] += (int)$g50['g50'];
					$row['rg100'] += (int)$g100['g100'];
					$row['rg500'] += (int)$g500['g500'];
					$row['rg1000'] += (int)$g1000['g1000'];				
				
					$sqlm1 = "select count(*) as 'm10' from $data_table[$i] where `Flag` = 8662  and  `Money` = 1000 and UNIX_TIMESTAMP(LogTime) >'$statr_time' and UNIX_TIMESTAMP(LogTime)<'$end_time' ";				
					$sqlm2 = "select count(*) as 'm50' from $data_table[$i] where `Flag` = 8662  and  `Money` = 5000 and UNIX_TIMESTAMP(LogTime) >'$statr_time' and UNIX_TIMESTAMP(LogTime)<'$end_time' ";
					$sqlm3 = "select count(*) as 'm100' from $data_table[$i] where `Flag` = 8662  and  `Money` = 10000 and UNIX_TIMESTAMP(LogTime) >'$statr_time' and UNIX_TIMESTAMP(LogTime)<'$end_time' ";
					$sqlm4 = "select count(*) as 'm500' from $data_table[$i] where `Flag` = 8662  and  `Money` = 50000 and UNIX_TIMESTAMP(LogTime) >'$statr_time' and UNIX_TIMESTAMP(LogTime)<'$end_time' ";
					$sqlm5 = "select count(*) as 'm1000' from $data_table[$i] where `Flag` = 8662  and  `Money` = 100000 and UNIX_TIMESTAMP(LogTime) >'$statr_time' and UNIX_TIMESTAMP(LogTime)<'$end_time' ";				
					$m10 = $dosql->GetOne($sqlm1);				
					$m50 = $dosql->GetOne($sqlm2);
					$m100 = $dosql->GetOne($sqlm3);
					$m500 = $dosql->GetOne($sqlm4);
					$m1000 = $dosql->GetOne($sqlm5);
					$row['rm10'] += (int)$m10['m10'];
					$row['rm50'] += (int)$m50['m50'];
					$row['rm100'] += (int)$m100['m100'];
					$row['rm500'] += (int)$m500['m500'];
					$row['rm1000'] += (int)$m1000['m1000'];				
					
					$sqlsm1 = "select count(*) as 'sm10' from $data_table[$i] where `Flag` = 9991  and  `Money` = 1000 and UNIX_TIMESTAMP(LogTime) >'$statr_time' and UNIX_TIMESTAMP(LogTime)<'$end_time' ";
					$sqlsm2 = "select count(*) as 'sm50' from $data_table[$i] where `Flag` = 9991  and  `Money` = 5000 and UNIX_TIMESTAMP(LogTime) >'$statr_time' and UNIX_TIMESTAMP(LogTime)<'$end_time' ";
					$sqlsm3 = "select count(*) as 'sm100' from $data_table[$i] where `Flag` = 9991  and  `Money` = 10000 and UNIX_TIMESTAMP(LogTime) >'$statr_time' and UNIX_TIMESTAMP(LogTime)<'$end_time' ";
					$sqlsm4 = "select count(*) as 'sm500' from $data_table[$i] where `Flag` = 9991  and  `Money` = 50000 and UNIX_TIMESTAMP(LogTime) >'$statr_time' and UNIX_TIMESTAMP(LogTime)<'$end_time' ";
					$sqlsm5 = "select count(*) as 'sm1000' from $data_table[$i] where `Flag` = 9991  and  `Money` = 100000 and UNIX_TIMESTAMP(LogTime) >'$statr_time' and UNIX_TIMESTAMP(LogTime)<'$end_time' ";
					$sm10 = $dosql->GetOne($sqlsm1);
					$sm50 = $dosql->GetOne($sqlsm2);
					$sm100 = $dosql->GetOne($sqlsm3);
					$sm500 = $dosql->GetOne($sqlsm4);
					$sm1000 = $dosql->GetOne($sqlsm5);
					$row['sm10'] += (int)$sm10['sm10'];
					$row['sm50'] += (int)$sm50['sm50'];
					$row['sm100'] += (int)$sm100['sm100'];
					$row['sm500'] += (int)$sm500['sm500'];
					$row['sm1000'] += (int)$sm1000['sm1000'];
									
					$sqlsg1 = "select count(*) as 'sg10' from $data_table[$i] where `Flag` = 9993  and  `Gold` = 10 and UNIX_TIMESTAMP(LogTime) >'$statr_time' and UNIX_TIMESTAMP(LogTime)<'$end_time' ";
					$sqlsg2 = "select count(*) as 'sg50' from $data_table[$i] where `Flag` = 9993  and  `Gold` = 50 and UNIX_TIMESTAMP(LogTime) >'$statr_time' and UNIX_TIMESTAMP(LogTime)<'$end_time' ";
					$sqlsg3 = "select count(*) as 'sg100' from $data_table[$i] where `Flag` = 9993  and  `Gold` = 100 and UNIX_TIMESTAMP(LogTime) >'$statr_time' and UNIX_TIMESTAMP(LogTime)<'$end_time' ";
					$sqlsg4 = "select count(*) as 'sg500' from $data_table[$i] where `Flag` = 9993  and  `Gold` = 500 and UNIX_TIMESTAMP(LogTime) >'$statr_time' and UNIX_TIMESTAMP(LogTime)<'$end_time' ";
					$sqlsg5 = "select count(*) as 'sg1000' from $data_table[$i] where `Flag` = 9993  and  `Gold` = 1000 and UNIX_TIMESTAMP(LogTime) >'$statr_time' and UNIX_TIMESTAMP(LogTime)<'$end_time' ";
					$sg10 = $dosql->GetOne($sqlsg1);
					$sg50 = $dosql->GetOne($sqlsg2);
					$sg100 = $dosql->GetOne($sqlsg3);
					$sg500 = $dosql->GetOne($sqlsg4);
					$sg1000 = $dosql->GetOne($sqlsg5);
					$row['sg10'] += (int)$sg10['sg10'];
					$row['sg50'] += (int)$sg50['sg50'];
					$row['sg100'] += (int)$sg100['sg100'];
					$row['sg500'] += (int)$sg500['sg500'];
					$row['sg1000'] += (int)$sg1000['sg1000'];
				}
				else
				{
					continue;
				}
			}
		}
		if(0>$fate)
		{
			$row['error'] = "结束时间不能低于开始时间";
		}
		if(0==$fate && $fag<0)
		{
			$row['error'] = "结束时间不能低于开始时间";
		}
	}
	echo json_encode($row);
	exit;
}
if(isset($_POST['shopcgi']))
{
	include "soapshop.php";
	date_default_timezone_set('PRC');
	$qu=(int)$_POST['qu'];
	$fu=(int)$_POST['fu'];
	$type=(int)$_POST['type'];
	$num=(int)$_POST['num'];
	$price=(int)$_POST['price'];
	$exchgnum = (int)$_POST['exchgnum'];
	$stime=strtotime($_POST['stime']);
	$etime=strtotime($_POST['etime']);
	$interval=(int)$_POST['interval'];
	
	$result=ShopCgi($qu,$fu,$type,$num,$price,$exchgnum,$stime,$etime,$interval,"","","");
	
	$dqtime = date('Y-m-d H:i:s');
	if(0 == $type){$optype = "出售金币";}
	if(1 == $type){$optype = "购买金币";}
	if(2 == $type){$optype = "出售银币";}
	if(3 == $type){$optype = "购买银币";}
	
	//日志记录
	$log1 = "<b>".$optype."</b><hr class=dotline>操作者：".$user['username']."<hr class=dotline>操作时间为：".$dqtime;
	if($result->ErrorCode==0 )
	{
		$str=$result->StrXML;
		$array=simplexml_load_string($str);
		if($array->Succ == 0){
			$log2 = "<hr class=dotline>操作成功";
			$gmlog = $log1.$log2;
			$sql = "INSERT INTO `sysgmlog` (`logid`,`UserId`,`qu`,`fu`,`optype`,`sell`,`buy`,`dnums`,`jgtime`,`stime`,`etime`,`flag`,`log`) VALUES ('','$user[id]','$qu','$fu','$optype','$num','$price','$exchgnum','$interval','$_POST[stime]','$_POST[etime]','1','$gmlog')";			
		}
		else
		{
			if($array->Succ == -1)
			{
				$err = "操作的价格不对";
			}
			if($array->Succ == -2)
			{
				$err = "购买或者出售操作已经有5次了";
			}
			if($array->Succ == -3)
			{
				$err = "对应的价格正在操作,若希望重新执行操作，请选择下方同类价格区间点击撤销";
			}
			if($array->Succ == -4)
			{
				$err = "时间不对";
			}
			if($array->Succ == -5)
			{
				$err = "找不到要操作的单";
			}
			if($array->Succ == -6)
			{
				$err = "购买某条玩家银币单时，系统交易进来的金币比将要交易出去的金币少";
			}
			if($array->Succ == -7)
			{
				$err = "就是执行总单数超过100";
			}
			if($array->Succ == -99)
			{
				$err = "系统忙";
			}
			$log2 = "<hr class=dotline>".$err;
			$gmlog = $log1.$log2;
			$sql = "INSERT INTO `sysgmlog` (`logid`,`UserId`,`qu`,`fu`,`optype`,`sell`,`buy`,`dnums`,`jgtime`,`stime`,`etime`,`flag`,`log`) VALUES ('','$user[id]','$qu','$fu','$optype','$num','$price','$exchgnum','$interval','$_POST[stime]','$_POST[etime]','0','$gmlog')";
		}
		$dosql->ExecNoneQuery($sql);
		echo json_encode($array);
		exit();
	}
	if($result->ErrorCode!=0)
	{
		$arr['ErrorCode'] = $result->ErrorCode;
		$arr['ErrorText'] = $result->StrError->ErrorText;
		$log2 = "<hr class=dotline>操作失败<hr class=dotline>错误：".$arr['ErrorCode']."<hr class=dotline>".$arr['ErrorText'];
		$gmlog = $log1.$log2;
		$sql = "INSERT INTO `sysgmlog` (`logid`,`UserId`,`qu`,`fu`,`optype`,`sell`,`buy`,`dnums`,`jgtime`,`stime`,`etime`,`flag`,`log`) VALUES ('','$user[id]','$qu','$fu','$optype','$num','$price','$exchgnum','$interval','$_POST[stime]','$_POST[etime]','0','$gmlog')";
		$dosql->ExecNoneQuery($sql);
		echo json_encode($arr);
		exit();
	}
}
if(isset($_POST['sear']))
{
	include "soapshop.php";
	date_default_timezone_set('PRC');
	$qu=(int)$_POST['qu'];
	$fu=(int)$_POST['fu'];
	$stime=strtotime($_POST['stime']);
	$etime=strtotime($_POST['etime']);
	$result=ShopCgi($qu,$fu,4,"","","","","","","","","");
	if($result->ErrorCode==0 )
	{
		$str=$result->StrXML;
		$array=simplexml_load_string($str);
		echo json_encode($array);
		exit();
	}
	if($result->ErrorCode!=0)
	{
		$arr['ErrorCode'] = $result->ErrorCode;
		$arr['ErrorText'] = $result->StrError->ErrorText;
		echo json_encode($arr);
		exit();
	}
}
if(isset($_POST['chexiao']))
{
	include "soapshop.php";
	date_default_timezone_set('PRC');
	$num=(int)$_POST['num'];
	$tp=(int)$_POST['tp'];
	$qu=(int)$_POST['qu'];
	$fu=(int)$_POST['fu'];
	$result=ShopCgi($qu,$fu,5,$num,0,0,0,0,0,0,0,$tp);
	if($tp == 0)
	{
		$optype = "撤销系统挂单--出售金币";
	}
	if($tp == 1)
	{
		$optype = "撤销系统挂单--购买金币";
	}
	if($tp == 2)
	{
		$optype = "撤销系统挂单--出售银币";
	}
	if($tp == 3)
	{
		$optype = "撤销系统挂单--购买银币";
	}
	$log1 = "<b>".$optype."</b><hr class=dotline>操作者：".$user['username']."<hr class=dotline>操作时间为：".$dqtime;
	if($result->ErrorCode==0 )
	{
		$str=$result->StrXML;
		$array=simplexml_load_string($str);
		if($array->Succ == 0){
			$log2 = "<hr class=dotline>操作成功";
			$gmlog = $log1.$log2;
			$sql = "INSERT INTO `sysgmlog` (`logid`,`UserId`,`qu`,`fu`,`optype`,`sell`,`buy`,`dnums`,`jgtime`,`stime`,`etime`,`flag`,`log`) VALUES ('','$user[id]','$qu','$fu','$optype','$num','','','','','','1','$gmlog')";			
		}
		else
		{
			if($array->Succ == -1)
			{
				$err = "操作的价格不对";
			}
			if($array->Succ == -2)
			{
				$err = "购买或者出售操作已经有5次了";
			}
			if($array->Succ == -3)
			{
				$err = "对应的价格正在操作,若希望重新执行操作，请选择下方同类价格区间点击撤销";
			}
			if($array->Succ == -4)
			{
				$err = "时间不对";
			}
			if($array->Succ == -5)
			{
				$err = "找不到要操作的单";
			}
			if($array->Succ == -6)
			{
				$err = "购买某条玩家银币单时，系统交易进来的金币比将要交易出去的金币少";
			}
			if($array->Succ == -7)
			{
				$err = "就是执行总单数超过100";
			}
			if($array->Succ == -99)
			{
				$err = "系统忙";
			}
			$log2 = "<hr class=dotline>".$err;
			$gmlog = $log1.$log2;
			$sql = "INSERT INTO `sysgmlog` (`logid`,`UserId`,`qu`,`fu`,`optype`,`sell`,`buy`,`dnums`,`jgtime`,`stime`,`etime`,`flag`,`log`) VALUES ('','$user[id]','$qu','$fu','$optype','$num','','','','','','0','$gmlog')";
		}
		$dosql->ExecNoneQuery($sql);
		echo json_encode($array);
		exit();
	}
	if($result->ErrorCode!=0)
	{
		$arr['ErrorCode'] = $result->ErrorCode;
		$arr['ErrorText'] = $result->StrError->ErrorText;
		$log2 = "<hr class=dotline>操作失败<hr class=dotline>错误：".$arr['ErrorCode']."<hr class=dotline>".$arr['ErrorText'];
		$gmlog = $log1.$log2;
		$sql = "INSERT INTO `sysgmlog` (`logid`,`UserId`,`qu`,`fu`,`optype`,`sell`,`buy`,`dnums`,`jgtime`,`stime`,`etime`,`flag`,`log`) VALUES ('','$user[id]','$qu','$fu','$optype','$num',','','','','','0','$gmlog')";
		$dosql->ExecNoneQuery($sql);
		echo json_encode($arr);
		exit();
	}
}
if(isset($_POST['sysbuy']))
{
	include "soapshop.php";
	date_default_timezone_set('PRC');
	$tp=$_POST['tpid'];
	$tps = explode("-",$tp);
	$qu=(int)$_POST['qu'];
	$fu=(int)$_POST['fu'];
	$flag = $_POST['TradeFlag'];
	$type = $tps[0]; 
	$tradeid = $tps[1];
	if(2 == $type)
	{
		echo json_encode(99);
		exit;
	}
	$result=ShopCgi($qu,$fu,6,0,0,0,0,0,0,$tradeid,$flag,"");
	if($flag == 0)
	{
		$optype = "购买玩家金币";
	}
	if($flag != 0)
	{
		$optype = "购买玩家银币";
	}
	$log1 = "<b>".$optype."</b><hr class=dotline>操作者：".$user['username']."<hr class=dotline>操作时间为：".$dqtime;
	if($result->ErrorCode==0 )
	{
		$str=$result->StrXML;
		$array=simplexml_load_string($str);
		if($array->Succ == 0){
			$log2 = "<hr class=dotline>操作成功";
			$gmlog = $log1.$log2;
			$sql = "INSERT INTO `sysgmlog` (`logid`,`UserId`,`qu`,`fu`,`optype`,`sell`,`buy`,`dnums`,`jgtime`,`stime`,`etime`,`flag`,`log`) VALUES ('','$user[id]','$qu','$fu','$optype','','','','','','','1','$gmlog')";			
		}
		else
		{
			if($array->Succ == -1)
			{
				$err = "操作的价格不对";
			}
			if($array->Succ == -2)
			{
				$err = "购买或者出售操作已经有5次了";
			}
			if($array->Succ == -3)
			{
				$err = "对应的价格正在操作,若希望重新执行操作，请选择下方同类价格区间点击撤销";
			}
			if($array->Succ == -4)
			{
				$err = "时间不对";
			}
			if($array->Succ == -5)
			{
				$err = "找不到要操作的单";
			}
			if($array->Succ == -6)
			{
				$err = "购买某条玩家银币单时，系统交易进来的金币比将要交易出去的金币少";
			}
			if($array->Succ == -7)
			{
				$err = "就是执行总单数超过100";
			}
			if($array->Succ == -99)
			{
				$err = "系统忙";
			}
			$log2 = "<hr class=dotline>".$err;
			$gmlog = $log1.$log2;
			$sql = "INSERT INTO `sysgmlog` (`logid`,`UserId`,`qu`,`fu`,`optype`,`sell`,`buy`,`dnums`,`jgtime`,`stime`,`etime`,`flag`,`log`) VALUES ('','$user[id]','$qu','$fu','$optype','','','','','','','0','$gmlog')";
		}
		$dosql->ExecNoneQuery($sql);
		echo json_encode($array);
		exit();
	}
	if($result->ErrorCode!=0)
	{
		$arr['ErrorCode'] = $result->ErrorCode;
		$arr['ErrorText'] = $result->StrError->ErrorText;
		$log2 = "<hr class=dotline>操作失败<hr class=dotline>错误：".$arr['ErrorCode']."<hr class=dotline>".$arr['ErrorText'];
		$gmlog = $log1.$log2;
		$sql = "INSERT INTO `sysgmlog` (`logid`,`UserId`,`qu`,`fu`,`optype`,`sell`,`buy`,`dnums`,`jgtime`,`stime`,`etime`,`flag`,`log`) VALUES ('','$user[id]','$qu','$fu','$optype','',','','','','','0','$gmlog')";
		$dosql->ExecNoneQuery($sql);
		echo json_encode($arr);
		exit();
	}
}
if(isset($_POST['sysrepeal']))
{
	include "soapshop.php";
	date_default_timezone_set('PRC');
	$tp=$_POST['tpid'];
	$tps = explode("-",$tp);
	$qu=(int)$_POST['qu'];
	$fu=(int)$_POST['fu'];
	$flag = $_POST['TradeFlag'];
	$type = $tps[0]; 
	$tradeid = $tps[1];
	if(2 != $type)
	{
		echo json_encode(99);
		exit;
	}
	$result=ShopCgi($qu,$fu,7,0,0,0,0,0,0,$tradeid,$flag,"");
	if($flag == 0)
	{
		$optype = "撤销系统金币单";
	}
	if($flag != 0)
	{
		$optype = "撤销系统银币单";
	}
	$log1 = "<b>".$optype."</b><hr class=dotline>操作者：".$user['username']."<hr class=dotline>操作时间为：".$dqtime;
	if($result->ErrorCode==0 )
	{
		$str=$result->StrXML;
		$array=simplexml_load_string($str);
		if($array->Succ == 0){
			$log2 = "<hr class=dotline>操作成功";
			$gmlog = $log1.$log2;
			$sql = "INSERT INTO `sysgmlog` (`logid`,`UserId`,`qu`,`fu`,`optype`,`sell`,`buy`,`dnums`,`jgtime`,`stime`,`etime`,`flag`,`log`) VALUES ('','$user[id]','$qu','$fu','$optype','','','','','','','1','$gmlog')";			
		}
		else
		{
			if($array->Succ == -1)
			{
				$err = "操作的价格不对";
			}
			if($array->Succ == -2)
			{
				$err = "购买或者出售操作已经有5次了";
			}
			if($array->Succ == -3)
			{
				$err = "对应的价格正在操作,若希望重新执行操作，请选择下方同类价格区间点击撤销";
			}
			if($array->Succ == -4)
			{
				$err = "时间不对";
			}
			if($array->Succ == -5)
			{
				$err = "找不到要操作的单";
			}
			if($array->Succ == -6)
			{
				$err = "购买某条玩家银币单时，系统交易进来的金币比将要交易出去的金币少";
			}
			if($array->Succ == -7)
			{
				$err = "就是执行总单数超过100";
			}
			if($array->Succ == -99)
			{
				$err = "系统忙";
			}
			$log2 = "<hr class=dotline>".$err;
			$gmlog = $log1.$log2;
			$sql = "INSERT INTO `sysgmlog` (`logid`,`UserId`,`qu`,`fu`,`optype`,`sell`,`buy`,`dnums`,`jgtime`,`stime`,`etime`,`flag`,`log`) VALUES ('','$user[id]','$qu','$fu','$optype','','','','','','','0','$gmlog')";
		}
		$dosql->ExecNoneQuery($sql);
		echo json_encode($array);
		exit();
	}
	if($result->ErrorCode!=0)
	{
		$arr['ErrorCode'] = $result->ErrorCode;
		$arr['ErrorText'] = $result->StrError->ErrorText;
		$log2 = "<hr class=dotline>操作失败<hr class=dotline>错误：".$arr['ErrorCode']."<hr class=dotline>".$arr['ErrorText'];
		$gmlog = $log1.$log2;
		$sql = "INSERT INTO `sysgmlog` (`logid`,`UserId`,`qu`,`fu`,`optype`,`sell`,`buy`,`dnums`,`jgtime`,`stime`,`etime`,`flag`,`log`) VALUES ('','$user[id]','$qu','$fu','$optype','',','','','','','0','$gmlog')";
		$dosql->ExecNoneQuery($sql);
		echo json_encode($arr);
		exit();
	}
}
//系统金银币交易记录
if(isset($_POST['OpTradeInfo']))
{
	$db1 = $_POST['xzdb'];
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
	
	date_default_timezone_set('PRC');
	$statr_time=strtotime($_POST['time']);
	$statr_table=date('ymd',$statr_time);
	$data_table=$qz.$statr_table;
	if($dosql->IsTable($data_table))
	{		
		$sql1 = "select * from $data_table  where `Flag` = 8661 order by UNIX_TIMESTAMP(LogTime) DESC ";
		$sql2 = "select * from $data_table  where `Flag` = 8662 order by UNIX_TIMESTAMP(LogTime) DESC " ;
		$sql3 = "select * from $data_table  where `Flag` = 9991 order by UNIX_TIMESTAMP(LogTime) DESC " ;
		$sql4 = "select * from $data_table  where `Flag` = 9993 order by UNIX_TIMESTAMP(LogTime) DESC " ;
		$dosql->ExecNoneQuery('SET NAMES Default');
		$dosql -> Query($sql1);
		while($data = $dosql->GetArray())
		{
			$data['RoleInfo_RoleName'] = iconv("gbk","utf8",$data['RoleInfo_RoleName']);
			$data['OperDesc'] = iconv("gbk","utf8",$data['OperDesc']);
			$row['rbuyg'][] = $data;
		}
		$dosql -> Query($sql2);
		while($data2 = $dosql->GetArray())
		{
			$data2['RoleInfo_RoleName'] = iconv("gbk","utf8",$data2['RoleInfo_RoleName']);
			$data2['OperDesc'] = iconv("gbk","utf8",$data2['OperDesc']);
			$row['rbuym'][] = $data2;
		}
		$dosql -> Query($sql3);
		while($data3 = $dosql->GetArray())
		{
			$data3['RoleInfo_RoleName'] = iconv("gbk","utf8",$data3['RoleInfo_RoleName']);
			$data3['OperDesc'] = iconv("gbk","utf8",$data3['OperDesc']);
			$row['sbuym'][] = $data3;
		}
		$dosql -> Query($sql4);
		while($data4 = $dosql->GetArray())
		{
			$data4['RoleInfo_RoleName'] = iconv("gbk","utf8",$data4['RoleInfo_RoleName']);
			$data4['OperDesc'] = iconv("gbk","utf8",$data4['OperDesc']);
			$row['sbuyg'][] = $data4;
		}
	}
	else if(!$dosql->IsTable($data_table))
	{
		echo json_encode("不存在数据表");
		exit;
	}
	echo json_encode($row);
	exit;
}
?>