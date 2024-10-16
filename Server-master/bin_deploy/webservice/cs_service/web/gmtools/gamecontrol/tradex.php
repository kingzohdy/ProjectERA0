<?php 
	require_once(dirname(__FILE__).'/../inc/config.inc.php'); 
//检测用户是否有权限
if($_SESSION['type'] != 5)
{
	$_SESSION = array();
	session_destroy();	
	echo '<script type="text/javascript">window.top.location.href="login.php";</script>';
	exit();
}
date_default_timezone_set('PRC');
if(isset($_GET['page']))
{
	$cz = $_GET['cz'];
	$pages = unserialize($_GET['page']);
	$rid = $pages['rid'];
	$wid = $pages['wid'];
	$tp =  $pages['tp'];
	$dosql->select_db("gmtools");
	$db1 = $rid."_".$wid;
	$sql = "select * from `reg_world2` where `dbid`='$db1' ";
	$dosql->Execute($sql);
	$fname = $dosql->GetArray();
	if($cz == "up")
	{
		$pages['page']--;
	}
	else if($cz == "next")
	{		
		$pages['page']++;
	}
	$pnums = $pages['page']*10;
	if($pages['page'] < 0)
	{
		echo "<h1 style='color:red'>无上一页</h1>";
	}
	else if($pages['page'] > intval($pages['nums']/10) )
	{
		echo "<h1 style='color:red'>无下一页</h1>";
	}
	else if((intval($pages['nums']/10) == $pages['nums']/10) && ($pages['page'] == $pages['nums']/10))
	{
		echo "<h1 style='color:red'>无下一页</h1>";
	}
	//外
	$resdb = "world_".$rid."_".$wid;
	//内
	//$resdb = "world_".$wid;
	$GLOBALS['db_host'] = $fname['dbip'];
	$GLOBALS['db_user'] = $fname['dbuser'];
	$GLOBALS['db_name'] = $resdb;
	$GLOBALS['db_pwd'] = $fname['dbpass'];
	
	$dosql->Close(true);
	$dosql = new MySql(false);
	$dosql->ExecNoneQuery('SET NAMES Default');
	if($tp == "j")
	{
		$sql = "SELECT * FROM `TradeSell` order by  `Moeny`/`Gold` DESC LIMIT $pnums,10 ";
	}
	else if($tp == "y")
	{
		$sql = "SELECT * FROM `TradeBuy` order by  `Moeny`/`Gold` DESC LIMIT $pnums,10 ";
	}
	if($pages['page'] < 0)
	{
		$sql = "SELECT * FROM `TradeBuy` order by  `Moeny`/`Gold` DESC LIMIT 0,0 ";
	}
	$dosql->Execute($sql);
	$i=0;
	while ( $row = $dosql->GetArray())
	{
		$data[$i] = $row;
		$i++;
	}
}
if(isset($_GET['qu']))
{
	$rid = $_GET['qu'];
	$wid = $_GET['fu'];
	$tp = $_GET['tp'];
	$db1 = $rid."_".$wid;
	$sql = "select * from `reg_world2` where `dbid`='$db1' ";
	$dosql->Execute($sql);
	$fname = $dosql->GetArray();
	//外
	$resdb = "world_".$rid."_".$wid;
	//内
	//$resdb = "world_".$wid;

	$GLOBALS['db_host'] = $fname['dbip'];
	$GLOBALS['db_user'] = $fname['dbuser'];
	$GLOBALS['db_name'] = $resdb;
	$GLOBALS['db_pwd'] = $fname['dbpass'];
	
	$dosql->Close(true);
	$dosql = new MySql(false);
	$dosql->ExecNoneQuery('SET NAMES Default');
	//$dosql->select_db($resdb);
	if($tp == "j")
	{
		$sqlnums = "SELECT * FROM `TradeSell`";
		$ss = mysql_query($sqlnums);
		$nums = mysql_num_rows($ss);
		$sql = "SELECT * FROM `TradeSell` order by  `Moeny`/`Gold` DESC LIMIT 0,10 ";
	}
	else if($tp == "y")
	{
		$sqlnums = "SELECT * FROM `TradeBuy`";
		$ss = mysql_query($sqlnums);
		$nums = mysql_num_rows($ss);
		$sql = "SELECT * FROM `TradeBuy` order by  `Moeny`/`Gold` DESC LIMIT 0,10 ";
	}	
	$dosql->Execute($sql);
	$i=0;
	while ( $row = $dosql->GetArray())
	{
		$data[$i] = $row;		
		$i++;
	}
	$pages['page'] = 0;
	$pages['nums'] = $nums;
	$pages['rid'] = $rid;
	$pages['wid'] = $wid;
	$pages['tp'] = $tp;
}
?>
<link href="style/admin.css" rel="stylesheet" type="text/css" />
<style type="text/css">
 a{ color:#00F;}
 table{border-collapse:collapse;border-spacing:0;border-left:1px solid #888;border-top:1px solid #888;}
</style>
<script type="text/javascript" src="js/jquery.min.js"></script>
<script type="text/jscript">
function buy()
{
	var tpid = $("input:radio[name='tpid']:checked").val();
	var qu = $("#qu").val();
	var fu = $("#fu").val();
	var TradeFlag = $("#TradeFlag").val();
	if("j" == TradeFlag)
	{
		TradeFlag = 0;
	}
	else if("y" == TradeFlag)
	{
		TradeFlag = 1;
	}
	if(!tpid)
	{
		alert("请选择左侧单选框");
		return;
	}
	else
	{
       if(!confirm("确认购买?"))
	   {
		   return;
	   }
		$.ajax({
			url : "tradelog.php",
			type:'post',
			dataType:'json',
			data:{"sysbuy":"","tpid":tpid,"qu":qu,"fu":fu,"TradeFlag":TradeFlag},
			success:function(msg)
			{
				if(99 == msg)
				{
					alert("不能购买系统单");
					return;
				}
				else if(msg.Succ == 0)
				{
					alert("成功");
					javascript:location.reload();
					return;
				}
				else if(msg.Succ == -1)
				{
					alert("操作的价格不对");
				}
				else if(msg.Succ == -2)
				{
					alert("购买或者出售操作已经有5次了");
				}
				else if(msg.Succ == -3)
				{
					alert("对应的价格正在操作,若希望重新执行操作，请选择下方同类价格区间点击撤销");
				}
				else if(msg.Succ == -4)
				{
					alert("时间不对");
				}
				else if(msg.Succ == -5)
				{
					alert("找不到要操作的单");
				}
				else if(msg.Succ == -6)
				{
					alert("购买某条玩家银币单时，系统交易进来的金币比将要交易出去的金币少");
				}
				else if(msg.Succ == -7)
				{
					alert("就是执行总单数超过100");
				}
				else if(msg.Succ == -99)
				{
					alert("系统忙");
				}
				else if(msg.ErrorCode == -1)
				{
					alert("系统错误");
				}
				else if(msg.ErrorCode == -2)
				{
					alert("网络错误");
				}
				else if(msg.ErrorCode == -3)
				{
					alert("TDR错误");
				}
				else if(msg.ErrorCode == -4)
				{
					alert("参数错误");
				}
				else if(msg.ErrorCode == -10)
				{
					alert("超时");
				}
			}
		});
	}
}
function repeal()
{
	var tpid = $("input:radio[name='tpid']:checked").val();
	var qu = $("#qu").val();
	var fu = $("#fu").val();
	var TradeFlag = $("#TradeFlag").val();
	if("j" == TradeFlag)
	{
		TradeFlag = 0;
	}
	else if("y" == TradeFlag)
	{
		TradeFlag = 1;
	}
	if(!tpid)
	{
		alert("请选择左侧单选框");
		return;
	}
	else
	{
		if(!confirm("确认撤销?"))
	   {
		   return;
	   }
		$.ajax({
			url : "tradelog.php",
			type:'post',
			dataType:'json',
			data:{"sysrepeal":"","tpid":tpid,"qu":qu,"fu":fu,"TradeFlag":TradeFlag},
			success:function(msg)
			{
				if(99 == msg)
				{
					alert("不能撤销玩家单");
					return;
				}
				else if(msg.Succ == 0)
				{
					alert("成功");
					javascript:location.reload();
					return;
				}
				else if(msg.Succ == -1)
				{
					alert("操作的价格不对");
				}
				else if(msg.Succ == -2)
				{
					alert("购买或者出售操作已经有5次了");
				}
				else if(msg.Succ == -3)
				{
					alert("对应的价格正在操作,若希望重新执行操作，请选择下方同类价格区间点击撤销");
				}
				else if(msg.Succ == -4)
				{
					alert("时间不对");
				}
				else if(msg.Succ == -5)
				{
					alert("找不到要操作的单");
				}
				else if(msg.Succ == -6)
				{
					alert("购买某条玩家银币单时，系统交易进来的金币比将要交易出去的金币少");
				}
				else if(msg.Succ == -7)
				{
					alert("就是执行总单数超过100");
				}
				else if(msg.Succ == -99)
				{
					alert("系统忙");
				}
			}
		});
	}
}
</script>
<body style="width:612px;">
<table width="700" height="450" border="1" cellpadding="0" cellspacing="0"  >	
	   <tr align="center" class="mgr_tr" width="93%" height="30">
       		<td  align="center" >&nbsp;</td>
            <td  align="center" >服务器</td>
            <td  align="center" >出售</td>
            <td  align="center" >出售价格</td>
            <td  align="center" >出售者</td>
            <td  align="center" >平均售价</td>
            <td  align="center" >到期时间</td>
        </tr>
        <?php for($i=0;$i<count($data);$i++){ ?>
        <tr align="center" class="mgr_tr" width="93%"> 
        	<td align="center" ><input type="radio" name="tpid"  value='<?php echo $data[$i]['Flag']."-".$data[$i]['TradeID']; ?>' ></td>
            <td  align="center" ><a href="#" ><?php echo $fname['worldname'];  ?></a></td>
            <td  align="center" ><a href="#" ><?php if($tp == "j"){echo $data[$i]['Gold']."金"; } else { echo $data[$i]['Moeny']."银"; }  ?></a></td>
            <td  align="center" ><a href="#" ><?php if($tp == "j"){echo $data[$i]['Moeny']."银";}else{ echo $data[$i]['Gold']."金";}  ?></a></td>
            <td  align="center" ><a href="#" ><?php if($data[$i]['Flag'] == "2"){ echo "系统";}else {$acc = iconv("gbk","utf8",$data[$i]['RoleName']); echo $acc;}  ?></a></td>
            <td  align="center" ><a href="#" ><?php echo "1金=".number_format(($data[$i]['Moeny']/ $data[$i]['Gold']),2)."银"; ?></a></td>
            <td  align="center" ><a href="#" ><?php $ts=((int)$data[$i]['StartTime']+(int)$data[$i]['Time']); echo date('Y-m-d H:i:s',$ts) ?></a></td>           
        </tr>
        <?php } ?>
        <tr style="border:0px; font-size:14px;" height="30px">
        	<td colspan="4" align="center" style="border-right:0px;" >
            <input type="hidden" id="qu" value="<?php echo $rid ?>" >
            <input type="hidden" id="fu" value="<?php echo $wid ?>" >
            <input type="hidden" id="TradeFlag" value="<?php echo $tp ?>" >
			选择右边的单选框点击&nbsp;&nbsp;&nbsp;<input type="button" value="购买" style="height:22px;"width="40px;" onClick="buy()">&nbsp;&nbsp;&nbsp;<input type="button" value="撤销" style="height:22px;"width="40px;" onClick="repeal();"></td>
        	<td colspan="3" style="border-left:0px;" >共&nbsp;&nbsp;<?php echo ceil($pages['nums']/10); ?>&nbsp;&nbsp;页，当前第&nbsp;&nbsp;<?php echo $pages['page']+1; ?>&nbsp;&nbsp;页&nbsp;&nbsp;<a href="tradex.php?cz=up&page=<?php echo urlencode(serialize($pages)); ?>" ><input type="button" value="上一页" ></a>&nbsp;&nbsp;&nbsp;&nbsp;<a href="tradex.php?cz=next&page=<?php echo urlencode(serialize($pages)); ?>" ><input type="button" value="下一页" > </a></td>
       </tr>
</table>
</body>
</html>
