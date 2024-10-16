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
	$uid = $_SESSION['id']; 
	$sql = "select * from `adminor` where `id` = '$uid' ";
	$dosql->Execute($sql);
	$user = $dosql->GetArray();
 require_once('soap.php');
 $result="";

 
//获取服务器列表
 if(isset($_POST['fwq']))
{
	$result=fwqlist();
	if($result->ErrorCode==0 )
	{
		$str=$result->StrXML;
		$array=simplexml_load_string($str);
		//echo json_encode(12);
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
//添加一条公告到所有服务器
if(isset($_POST['noticeAdd']) && isset($_POST['all']))
{
	$gdtime=$_POST['gdtime'];
	$hytime=$_POST['hytime'];
	$kstime=$_POST['kstime'];
	$content=$_POST['content'];
	
	//设置时区
	date_default_timezone_set('PRC');
	//获取当前时间
	$dqtime = date('Y-m-d H:i:s');
	//日志记录
	$log1 = "<b>向全区发布公告</b><hr class=dotline>操作者：".$user['username']."<hr class=dotline>操作时间为：".$dqtime."<hr class=dotline>经过".$kstime."秒后发出<hr class=dotline>滚动时间为：".$gdtime."秒<hr class=dotline>活跃时间为:".$hytime."秒<hr class=dotline>内容：".$content;
		
	$result=noticeAllADD($content,$gdtime,$hytime,$kstime);
	if($result->ErrorCode==0)
	{
		$log2 = "<hr class=dotline>操作成功";
		$gmlog = $log1.$log2;
		$sql = "INSERT INTO `gmlog` VALUES (null, '$user[id]', '', '', '-1', '-1', now(), DATE_ADD(now(), INTERVAL '$kstime' SECOND),'$hytime', '1','0','$gmlog');";
		$dosql->ExecNoneQuery($sql);
		echo json_encode(0);
		exit(-1);
	}
	else if($result->ErrorCode!=0)
	{
		$arr['ErrorCode'] = $result->ErrorCode;
		$arr['ErrorText'] = $result->StrError->ErrorText;
		$log2 = "<hr class=dotline>操作失败<hr class=dotline>错误：".$arr['ErrorCode']."<hr class=dotline>".$arr['ErrorText'];
		$gmlog = $log1.$log2;
		$sql = "INSERT INTO `gmlog` VALUES (null, '$user[id]', '', '', '-1', '-1', now(), DATE_ADD(now(), INTERVAL '$kstime' SECOND),'$hytime','1','1', '$gmlog');";
		$dosql->ExecNoneQuery($sql);
		echo json_encode($arr);
		exit(-1);
	}
}
//添加一条公告到一个服务器
if(isset($_POST['noticeAdd']) && isset($_POST['only']))
{
	$daqu=$_POST['daqu'];
	$fu=$_POST['fu'];
	$gdtime=$_POST['gdtime'];
	$hytime=$_POST['hytime'];
	$kstime=$_POST['kstime'];
	$content=$_POST['content'];
	
	//设置时区
	date_default_timezone_set('PRC');
	//获取当前时间
	$dqtime = date('Y-m-d H:i:s');
	//日志记录
	$log1 = "<b>向指定服务器发布公告</b><hr class=dotline>大区:&nbsp;".$daqu.",&nbsp;&nbsp;服：&nbsp;".$fu."<hr class=dotline >操作者：".$user['username']."<hr class=dotline>操作时间为：".$dqtime."<hr class=dotline>经过".$kstime."秒后发出<hr class=dotline>滚动时间为：".$gdtime."秒<hr class=dotline >活跃时间为:".$hytime."秒<hr class=dotlin>内容：".$content;
	
	$result=noticeADD($daqu,$fu,$content,$gdtime,$hytime,$kstime);
	if($result->ErrorCode==0 )
	{
		$log2 = "<hr class=dotline>操作成功";
		$gmlog = $log1.$log2;
		$sql = "INSERT INTO `gmlog` VALUES (null, '$user[id]', '', '', '$daqu', '$fu', now(), DATE_ADD(now(), INTERVAL '$kstime' SECOND)
,'$hytime','1','0', '$gmlog');";
		$dosql->ExecNoneQuery($sql);
		echo json_encode(0);
		exit(-1);
	}
	if($result->ErrorCode!=0)
	{
		$arr['ErrorCode'] = $result->ErrorCode;
		$arr['ErrorText'] = $result->StrError->ErrorText;
		$log2 = "<hr class=dotline>操作失败<hr class=dotline>错误：".$arr['ErrorCode']."<hr class=dotline>".$arr['ErrorText'];
		$gmlog = $log1.$log2;
		$sql = "INSERT INTO `gmlog` VALUES (null, '$user[id]', '', '', '$daqu', '$fu', now(), DATE_ADD(now(), INTERVAL '$kstime' SECOND),'$hytime', '1','1','$gmlog');";
		$dosql->ExecNoneQuery($sql);
		echo json_encode($arr);
		exit(-1);
	}
}
//获取公告列表
if(isset($_POST['noticelist']))
{
	$q=$_POST['q'];
	$f=$_POST['f'];
	$result=noticeList($q,$f);
	if($result->ErrorCode==0 )
	{
		$str=$result->StrXML;
		$array=simplexml_load_string($str);
		//var_dump($result);
		echo json_encode($array);
		exit(-1);
	}
	if($result->ErrorCode!=0)
	{
		$arr['ErrorCode'] = $result->ErrorCode;
		$arr['ErrorText'] = $result->StrError->ErrorText;
		echo json_encode($arr);
		exit(-1);
	}
}
//从所有服务器删除一条公告
if(isset($_POST['delnoticeall']))
{
	$noticeID=$_POST['delnoticeall'];
	$result = noticeDelet($noticeID);
	//设置时区
	date_default_timezone_set('PRC');
	//获取当前时间
	$dqtime = date('Y-m-d H:i:s');
	//日志记录
	$log1 = "<b>从所有服务器删除公告</b><hr class=dotline>操作者：".$user['username']."<hr class=dotline>操作时间为：".$dqtime."<hr class=dotline>删除公告ID：".$noticeID;
	if($result->ErrorCode==0 )
	{
		$log2 = "<hr class=dotline>操作成功";
		$gmlog = $log1.$log2;
		$sql = "INSERT INTO `gmlog` VALUES (null, '$user[id]', '', '', '-1', '-1', now(), now(),'', '2','0','$gmlog');";
		$dosql->ExecNoneQuery($sql);
		echo json_encode(0);
		exit(-1);
	}
	if($result->ErrorCode!=0)
	{
		$arr['ErrorCode'] = $result->ErrorCode;
		$arr['ErrorText'] = $result->StrError->ErrorText;
		$log2 = "<hr class=dotline>操作失败<hr class=dotline>错误：".$arr['ErrorCode']."<hr class=dotline>".$arr['ErrorText'];
		$gmlog = $log1.$log2;
		$sql = "INSERT INTO `gmlog` VALUES (null, '$user[id]', '', '', '-1', '-1', now(), now(),'', '2','1','$gmlog');";
		$dosql->ExecNoneQuery($sql);
		echo json_encode($arr);
		exit(-1);
	}
}
//从指定服务器删除一条公告
if(isset($_POST['delnotice']))
{
	$noticeID=$_POST['delnotice'];
	$qu=$_POST['qu'];
	$fu=$_POST['fu'];
	
	//设置时区
	date_default_timezone_set('PRC');
	//获取当前时间
	$dqtime = date('Y-m-d H:i:s');
	//日志记录
	$log1 = "<b>从指定服务器删除公告</b><hr class=dotline>操作者：".$user['username']."<hr class=dotline>操作时间为：".$dqtime."<hr class=dotline>大区：&nbsp;".$qu."&nbsp;&nbsp;服:&nbsp;".$fu."<hr class=dotline>删除公告ID：".$noticeID;
	
	$result = noticeDeletOne($qu,$fu,$noticeID);
	if($result->ErrorCode==0 )
	{
		$log2 = "<hr class=dotline>操作成功";
		$gmlog = $log1.$log2;
		$sql = "INSERT INTO `gmlog` VALUES (null, '$user[id]', '', '', '$qu', '$fu', now(), now(),'', '2','0','$gmlog');";
		$dosql->ExecNoneQuery($sql);
		echo json_encode(0);
		exit(-1);
	}
	if($result->ErrorCode!=0)
	{
		$arr['ErrorCode'] = $result->ErrorCode;
		$arr['ErrorText'] = $result->StrError->ErrorText;
		$log2 = "操作失败<hr class=dotline>错误：".$arr['ErrorCode']."<hr class=dotline>".$arr['ErrorText'];
		$gmlog = $log1.$log2;
		$sql = "INSERT INTO `gmlog` VALUES (null, '$user[id]', '', '', '', '', now(), now(),'', '2','1','$gmlog');";
		$dosql->ExecNoneQuery($sql);
		echo json_encode($arr);
		exit(-1);
	}
}

//通过账号查询账号
if(isset($_POST['AccName']))
{
	$RegionID = $_POST['qu'];
	$AccName = $_POST['AccName'];
	$cfs = $_POST['cfs'];
	if($cfs == 2)
	{
		$AccName = "%".$AccName."%";
	}
	if($RegionID == -1)
	{
		$result = selectFirstAccount($AccName);
	}
	if($RegionID != -1)
	{
		$result = DQselectAccount($RegionID,$AccName);
	}
	if($result->ErrorCode==0 )
	{
		$str=$result->StrXML;
		$data=simplexml_load_string($str);
		$array['FindHandleID']=$result->FindHandleID;
		$array['data']=$data;
		//var_dump($result);
		echo json_encode($array);
		exit(-1);
	}
	if($result->ErrorCode!=0)
	{
		$arr['ErrorCode'] = $result->ErrorCode;
		$arr['ErrorText'] = $result->StrError->ErrorText;
		echo json_encode($arr);
		exit(-1);
	}
	
}
//通过账号查询账号(下一次调用,句柄查询)
if(isset($_POST['AccName2np']))
{
	$RegionID = $_POST['daqu'];
	$AccName = $_POST['AccName2np'];
	$page = $_POST['page'];
	if($RegionID == -1)
	{
		$result = selectNextAccount($page);
	}
	if($RegionID != -1)
	{
		$result = DQselectAccountNext($page);
	}
	if($result->ErrorCode==0 )
	{
		$str=$result->StrXML;
		$data=simplexml_load_string($str);
		$array['data']=$data;
		//var_dump($result);
		echo json_encode($array);
		exit(-1);
	}
	if($result->ErrorCode!=0)
	{
		$arr['ErrorCode'] = $result->ErrorCode;
		$arr['ErrorText'] = $result->StrError->ErrorText;
		echo json_encode($arr);
		exit(-1);
	}
	
}

//通过角色查询账号(不用指定服务器)
if(isset($_POST['roleselect1']))
{
	$cfs = $_POST['cfs'];
	$rolename = $_POST['rolename'];
	if($cfs == 2)
	{
		$rolename = "%".$rolename."%";
	}
	$result = selectRoleAccountFirst($rolename);
	if($result->ErrorCode==0 )
	{
		$str=$result->StrXML;
		$data=simplexml_load_string($str);
		$array['FindHandleID']=$result->FindHandleID;
		$array['data']=$data;
		//var_dump($result);
		echo json_encode($array);
		exit(-1);
	}
	if($result->ErrorCode!=0)
	{
		$arr['ErrorCode'] = $result->ErrorCode;
		$arr['ErrorText'] = $result->StrError->ErrorText;
		echo json_encode($arr);
		exit(-1);
	}
}
//通过角色查询账号(不指定服务器,句柄查询)
if(isset($_POST['roleselect1np']))
{
	$page = $_POST['page'];
	$result = selectRoleAccountNext($page);
	if($result->ErrorCode==0 )
	{
		$str=$result->StrXML;
		$data=simplexml_load_string($str);
		$array['data']=$data;
		//var_dump($result);
		echo json_encode($array);
		exit(-1);
	}
	if($result->ErrorCode!=0)
	{
		$arr['ErrorCode'] = $result->ErrorCode;
		$arr['ErrorText'] = $result->StrError->ErrorText;
		echo json_encode($arr);
		exit(-1);
	}
}

//通过角色查询角色(指定服务器)
if(isset($_POST['roleselect2']))
{
	$q = $_POST['q'];
	$f = $_POST['f'];
	$cfs = $_POST['cfs'];
	$rolename = $_POST['rolename'];
	if($cfs == 2)
	{
		$rolename = "%".$rolename."%";
	}
	$result = SelctRole($q,$f,$rolename);
	if($result->ErrorCode==0 )
	{
		$str=$result->StrXML;
		$data=simplexml_load_string($str);
		$array['FindHandleID']=$result->FindHandleID;
		$array['data']=$data;
		echo json_encode($array);
		exit(-1);
	}
	if($result->ErrorCode!=0)
	{
		$arr['ErrorCode'] = $result->ErrorCode;
		$arr['ErrorText'] = $result->StrError->ErrorText;
		echo json_encode($arr);
		exit(-1);
	}
}
//通过角色查询角色(指定服务器,句柄查询)下一页
if(isset($_POST['roleselect2np']))
{
	$page = $_POST['page'];
	$result = SelctRoleNxet($page);
	if($result->ErrorCode==0 )
	{
		$str=$result->StrXML;
		$data=simplexml_load_string($str);
		$array['data']=$data;
		echo json_encode($array);
		exit(-1);
	}
	if($result->ErrorCode!=0)
	{
		$arr['ErrorCode'] = $result->ErrorCode;
		$arr['ErrorText'] = $result->StrError->ErrorText;
		echo json_encode($arr);
		exit(-1);
	}
}

//操作类型，OperateType等于1代表添加公告，等于2代表删除公告，等于3代表通过账号踢角色，等于4代表禁言账号，等于5解禁账号,等于6代表冻结账号
				//7代表：改安全锁.8代表封角色
//通过账号踢角色(踢下线)
if(isset($_POST['tiAccountName']))
{
	$RegionID=$_POST['quID'];
	$AccName=$_POST['AccountName'];
	
	//设置时区
	date_default_timezone_set('PRC');
	//获取当前时间
	$dqtime = date('Y-m-d H:i:s');
	//日志记录
	$log1 = "<b>通过账号将角色踢下线</b><hr class=dotline>操作者：".$user['username']."<hr class=dotline>操作时间为：".$dqtime."<hr class=dotline>大区：".$RegionID."<hr class=dotline>账号名:".$AccName;
	
	$result=KickAccount($RegionID,$AccName);
	if($result->ErrorCode==0)
	{
		$log2 = "<hr class=dotline>操作成功";
		$gmlog = $log1.$log2;
		$sql = "INSERT INTO `gmlog` VALUES (null, '$user[id]', '$AccName', '', '$RegionID ', '', now(), now(),'', '3','0','$gmlog');";
		$dosql->ExecNoneQuery($sql);
		echo json_encode("0");
		exit(-1);
	}
	if($result->ErrorCode!=0)
	{
		$arr['ErrorCode'] = $result->ErrorCode;
		$arr['ErrorText'] = $result->StrError->ErrorText;
		$log2 = "<hr class=dotline>操作失败<hr class=dotline>错误：".$arr['ErrorCode']."<hr class=dotline>".$arr['ErrorText'];
		$gmlog = $log1.$log2;
		$sql = "INSERT INTO `gmlog` VALUES (null, '$user[id]', '$AccName', '', '$RegionID ', '', now(), now(),'', '3','1','$gmlog');";
		echo json_encode($arr);
		exit(-1);
	}
}

//禁言/解禁账号
if(isset($_POST['jingyan']))
{
	$RegionID=$_POST['daqu'];
	$AccName=$_POST['Account'];
	$jtime=$_POST['jintime'];
	//设置时区
	date_default_timezone_set('PRC');
	//获取当前时间
	$dqtime = date('Y-m-d H:i:s');
	
	if($jtime == 0)
	{
		$Enable=0;
		//日志记录
		$log1 = "<b>解除禁言</b><hr class=dotline>操作者：".$user['username']."<hr class=dotline>操作时间为：".$dqtime."<hr class=dotline>大区：".$RegionID."<hr class=dotline>账号名:".$AccName;
		$fag = 4;
	}
	else if($jtime != 0)
	{
		$Enable=1;
		//日志记录
		$log1 = "<b>禁言用户</b><hr class=dotline>操作者：".$user['username']."<hr class=dotline>操作时间为：".$dqtime."<hr class=dotline>大区：".$RegionID."<hr class=dotline>账号名:".$AccName."<hr class=dotline>禁言时间:".$jtime."秒<hr class=dotline>";
		$fag = 5;
	}
	//TalkAccount($RegionID,$AccName,$Enable,$Time)
	$result=TalkAccount($RegionID,$AccName,$Enable,$jtime);
	if($result->ErrorCode==0)
	{
		$log2 = "操作成功";
		$gmlog = $log1.$log2;
		$sql = "INSERT INTO `gmlog` VALUES (null, '$user[id]', '$AccName', '', '$RegionID ', '', now(), now(),'$jtime', '$fag','0','$gmlog');";
		$dosql->ExecNoneQuery($sql);
		echo json_encode("0");
	}
	if($result->ErrorCode!=0)
	{
		$arr['ErrorCode'] = $result->ErrorCode;
		$arr['ErrorText'] = $result->StrError->ErrorText;
		$log2 = "操作失败<hr class=dotline>错误：".$arr['ErrorCode']."<hr class=dotline>".$arr['ErrorText'];
		$gmlog = $log1.$log2;
		$sql = "INSERT INTO `gmlog` VALUES (null, '$user[id]', '$AccName', '', '$RegionID ', '', now(), now(),'$jtime', '$fag','1','$gmlog');";
		$dosql->ExecNoneQuery($sql);
		echo json_encode($arr);
		exit(-1);
	}
}
//禁言/角色
if(isset($_POST['jingyanRole']))
{
	$RegionID=$_POST['daqu'];
	$RoleName=$_POST['RoleName'];
	$jtime=$_POST['jintime'];
	$WorldID=$_POST['fu'];
	//设置时区
	date_default_timezone_set('PRC');
	//获取当前时间
	$dqtime = date('Y-m-d H:i:s');
	
	if($jtime == 0)
	{
		$Enable=0;
		//日志记录
		$log1 = "<b>解除禁言</b><hr class=dotline>操作者：".$user['username']."<hr class=dotline>操作时间为：".$dqtime."<hr class=dotline>大区：".$RegionID."<hr class=dotline>服：".$WorldID."<hr class=dotline>角色名:".$RoleName;
		$fag = 4;
	}
	else if($jtime != 0)
	{
		$Enable=1;
		//日志记录
		$log1 = "<b>禁言用户</b><hr class=dotline>操作者：".$user['username']."<hr class=dotline>操作时间为：".$dqtime."<hr class=dotline>大区：".$RegionID."<hr class=dotline>服：".$WorldID."<hr class=dotline>角色名:".$RoleName."<hr class=dotline>禁言时间:".$jtime."秒<hr class=dotline>";
		$fag = 5;
	}
	//SilenceRole($RegionID,$WorldID,$RoleName,$Time)
	$result=SilenceRole($RegionID,$WorldID,$RoleName,$jtime);
	if($result->ErrorCode==0)
	{
		$log2 = "操作成功";
		$gmlog = $log1.$log2;
		$sql = "INSERT INTO `gmlog` VALUES (null, '$user[id]', '', '$RoleName', '$RegionID ', '', now(), now(),'$jtime', '$fag','0','$gmlog');";
		$dosql->ExecNoneQuery($sql);
		echo json_encode("0");
	}
	if($result->ErrorCode!=0)
	{
		$arr['ErrorCode'] = $result->ErrorCode;
		$arr['ErrorText'] = $result->StrError->ErrorText;
		$log2 = "操作失败<hr class=dotline>错误：".$arr['ErrorCode']."<hr class=dotline>".$arr['ErrorText'];
		$gmlog = $log1.$log2;
		$sql = "INSERT INTO `gmlog` VALUES (null, '$user[id]', '', '$RoleName', '$RegionID ', '', now(), now(),'$jtime', '$fag','1','$gmlog');";
		$dosql->ExecNoneQuery($sql);
		echo json_encode($arr);
		exit(-1);
	}
}

//冻结账号
if(isset($_POST['dongjie']))
{
	$RegionID=$_POST['daqu'];
	$AccName=$_POST['Account'];
	$dtime=$_POST['jintime'];
	$Reason=$_POST['cause'];
	
	//设置时区
	date_default_timezone_set('PRC');
	//获取当前时间
	$dqtime = date('Y-m-d H:i:s');
			
	if($dtime == 0)
	{
		$Enable=0;
		//日志记录
		$log1 = "<b>解除冻结</b><hr class=dotline>操作者：".$user['username']."<hr class=dotline>操作时间为：".$dqtime."<hr class=dotline>大区：".$RegionID."<hr class=dotline>账号名:".$AccName."<hr class=dotline>冻结原因:".$Reason;
	}
	else
	{
		$Enable=1;
		$log1 = "<b>冻结账号</b><hr class=dotline>操作者：".$user['username']."<hr class=dotline>操作时间为：".$dqtime."<hr class=dotline>大区：".$RegionID."<hr class=dotline>账号名:".$AccName."<hr class=dotline>冻结原因:".$Reason;
	}
	//TalkAccount($RegionID,$AccName,$Enable,$Time)
	$result=LockAccount($RegionID,$AccName,$Enable,$dtime,$Reason);
	if($result->ErrorCode==0)
	{
		$log2 = "<hr class=dotline>操作成功";
		$gmlog = $log1.$log2;
		$sql = "INSERT INTO `gmlog` VALUES (null, '$user[id]', '$AccName', '', '$RegionID ', '', now(), now(),'', '6','0','$gmlog');";
		$dosql->ExecNoneQuery($sql);
		echo json_encode("0");
	}
	if($result->ErrorCode!=0)
	{
		$arr['ErrorCode'] = $result->ErrorCode;
		$arr['ErrorText'] = $result->StrError->ErrorText;
		$log2 = "<hr class=dotline>操作失败<hr class=dotline>错误：".$arr['ErrorCode']."<hr class=dotline>".$arr['ErrorText'];
		$gmlog = $log1.$log2;
		$sql = "INSERT INTO `gmlog` VALUES (null, '$user[id]', '$AccName', '', '$RegionID ', '', now(), now(),'', '6','1','$gmlog');";
		$dosql->ExecNoneQuery($sql);
		echo json_encode($arr);
		exit(-1);
	}
}
//改安全锁
if(isset($_POST['anquansuo']))
{
	//"psd": psd,"daqu":daqu,"Account":Account
	$RegionID=$_POST['daqu'];
	$AccName=$_POST['Account'];
	$SafePwd=$_POST['psd'];
	
	//设置时区
	date_default_timezone_set('PRC');
	//获取当前时间
	$dqtime = date('Y-m-d H:i:s');
	//日志记录
	$log1 = "<b>改安全锁</b>操作者：".$user['username']."<hr class=dotline>操作时间为：".$dqtime."<hr class=dotline>大区：".$RegionID.",账号名:".$AccName."密码改为:".$SafePwd."<hr class=dotline>";
	
	$result = AccSafelock($RegionID,$AccName,$SafePwd);
	if($result->ErrorCode==0)
	{
		$log2 = "操作成功";
		$gmlog = $log1.$log2;
		$sql = "INSERT INTO `gmlog` VALUES (null, '$user[id]', '$AccName', '', '$RegionID ', '', now(), now(),'', '7','0','$gmlog');";
		$dosql->ExecNoneQuery($sql);
		echo json_encode("0");
	}
	if($result->ErrorCode!=0)
	{
		$arr['ErrorCode'] = $result->ErrorCode;
		$arr['ErrorText'] = $result->StrError->ErrorText;
		$arr['ErrorCode'] = $result->ErrorCode;
		$arr['ErrorText'] = $result->StrError->ErrorText;
		$log2 = "操作失败<hr class=dotline>错误：".$arr['ErrorCode']."<hr class=dotline>".$arr['ErrorText'];
		$gmlog = $log1.$log2;
		$sql = "INSERT INTO `gmlog` VALUES (null, '$user[id]', '$AccName', '', '$RegionID ', '', now(), now(),'', '7','1','$gmlog');";
		$dosql->ExecNoneQuery($sql);
		echo json_encode($arr);
		exit(-1);
	}
}
//封角色
if(isset($_POST['dongjieRole']))
{
	$RegionID=$_POST['daqu'];
	$WorldID=$_POST['fu'];
	$RoleName=$_POST['RoleName'];
	$dtime=$_POST['jintime'];
	
	//设置时区
	date_default_timezone_set('PRC');
	//获取当前时间
	$dqtime = date('Y-m-d H:i:s');
	//日志记录
	$log1 = "<b>封角色</b><hr class=dotline>操作者：".$user['username']."<hr class=dotline>操作时间为：".$dqtime."<hr class=dotline>大区：".$RegionID."服：".$WorldID."<hr class=dotline>账号名:".$AccName."<hr class=dotline>角色名:".$RoleName."<hr class=dotline>封时间:".$dtime."(秒)<hr class=dotline>";
	
	//TalkAccount($RegionID,$AccName,$Enable,$Time)
	$result=BlockRole($RegionID,$WorldID,$RoleName,$dtime,$Reason);
	if($result->ErrorCode==0)
	{
		$log2 = "操作成功";
		$gmlog = $log1.$log2;
		$sql = "INSERT INTO `gmlog` VALUES (null, '$user[id]', '$AccName', '$RoleName', '$RegionID ', '$WorldID', now(), now(),'$dtime', '8','0','$gmlog');";
		$dosql->ExecNoneQuery($sql);
		echo json_encode("0");
	}
	if($result->ErrorCode!=0)
	{
		$arr['ErrorCode'] = $result->ErrorCode;
		$arr['ErrorText'] = $result->StrError->ErrorText;
		$log2 = "操作失败<hr class=dotline>错误：".$arr['ErrorCode']."<hr class=dotline>".$arr['ErrorText'];
		$gmlog = $log1.$log2;
		$sql = "INSERT INTO `gmlog` VALUES (null, '$user[id]', '$AccName', '$RoleName', '$RegionID ', '$WorldID', now(), now(),'$dtime', '8','1','$gmlog');";
		$dosql->ExecNoneQuery($sql);
		echo json_encode($arr);
		exit(-1);
	}
}
//改坐标
if(isset($_POST['gzb']))
{
	$RegionID = $_POST['daqu'];
	$WorldID = $_POST['fu'];
	$RoleName = $_POST['role'];
	$MapID = $_POST['mapid'];
	$X=$_POST['mapx'];
	$Y=$_POST['mapy'];
	
	$result = updateMap($RegionID,$WorldID,$RoleName,$MapID,$X,$Y);
	
	
	//设置时区
	date_default_timezone_set('PRC');
	//获取当前时间
	$dqtime = date('Y-m-d H:i:s');
	//日志记录
	$log1 = "<b>修改角色坐标</b><hr class=dotline>操作者：".$user['username']."<hr class=dotline>操作时间为：".$dqtime."<hr class=dotline>大区：".$RegionID."服：".$WorldID."<hr class=dotline>角色名:".$RoleName."<hr class=dotline>:修改到地图ID：&nbsp;".$MapID."&nbsp;&nbsp;X坐标：&nbsp;".$X."Y坐标：&nbsp;".$Y."<hr class=dotline>";

	if($result->ErrorCode==0)
	{
		$log2 = "操作成功";
		$gmlog = $log1.$log2;
		$sql = "INSERT INTO `gmlog` VALUES (null, '$user[id]', '', '$RoleName', '$RegionID ', '$WorldID', now(), now(),'$dtime', '9','0','$gmlog');";
		$dosql->ExecNoneQuery($sql);
		echo json_encode("0");
		exit(-1);		
	}
	if($result->ErrorCode!=0)
	{
		$arr['ErrorCode'] = $result->ErrorCode;
		$arr['ErrorText'] = $result->StrError->ErrorText;
		$log2 = "操作失败<hr class=dotline>错误：".$arr['ErrorCode']."<hr class=dotline>".$arr['ErrorText'];
		$gmlog = $log1.$log2;
		$sql = "INSERT INTO `gmlog` VALUES (null, '$user[id]', '', '$RoleName', '$RegionID ', '$WorldID', now(), now(),'$dtime', '9','1','$gmlog');";
		$dosql->ExecNoneQuery($sql);
		echo json_encode($arr);
		exit(-1);
	}
}
//通过邮件句柄查询邮件
if(isset($_POST['mail']))
{
	$page = $_POST['page'];
	$result = selectNextMail($page);
	if($result->ErrorCode==0 )
	{
		$str=$result->StrXML;
		$data=simplexml_load_string($str);
		$array['data']=$data;
		echo json_encode($array);
		exit(-1);
	}
	if($result->ErrorCode!=0)
	{
		$arr['ErrorCode'] = $result->ErrorCode;
		$arr['ErrorText'] = $result->StrError->ErrorText;
		echo json_encode($arr);
		exit(-1);
	}
}
//获取角色列表
//获取角色信息RoleInfo($RegionID,$AccName,$RoleName)
//关闭句柄
if(isset($_POST['FindIdColse']))
{
	$FindId = $_POST['FindIdColse'];
	$result = HandleClose($FindId);
	if($result->ErrorCode==0 )
	{
		echo json_encode(0);
		exit(-1);
	}
	if($result->ErrorCode!=0)
	{
		$arr['ErrorCode'] = $result->ErrorCode;
		$arr['ErrorText'] = $result->StrError->ErrorText;
		echo json_encode($arr);
		exit(-1);
	}
}
if(isset($_GET['fid']))
{
	$FindId = $_GET['fid'];
	$result = HandleClose($FindId);
	if($result->ErrorCode==0 )
	{
		echo json_encode(0);
		exit(-1);
	}
	if($result->ErrorCode!=0)
	{
		$arr['ErrorCode'] = $result->ErrorCode;
		$arr['ErrorText'] = $result->StrError->ErrorText;
		echo json_encode($arr);
		exit(-1);
	}
}
//批量冻结账号
if(isset($_POST['dongjies']))
{
	$RegionID=$_POST['daqu'];
	$AccName=$_POST['account'];
	$dtime=$_POST['jintime'];
	$Reason=$_POST['cause'];
	
	if("" == $AccName || "" == $dtime || "" == $Reason)
	{
		echo json_encode("数据输入错误");
		exit;
	}
	
	$accs = explode("|",$AccName);

	//设置时区
	date_default_timezone_set('PRC');
	//获取当前时间
	$dqtime = date('Y-m-d H:i:s');
	
	if($dtime == 0)
	{
		$Enable=0;
		//日志记录
		$log1 = "<b>批量解除冻结</b><hr class=dotline>操作者：".$user['username']."<hr class=dotline>操作时间为：".$dqtime."<hr class=dotline>大区：".$RegionID."<hr class=dotline>账号名:".$AccName."<hr class=dotline>冻结原因:".$Reason;"<hr class=dotline>账号:(".$AccName.")";
	}
	else
	{
		$Enable=1;
		$log1 = "<b>批量冻结账号</b><hr class=dotline>操作者：".$user['username']."<hr class=dotline>操作时间为：".$dqtime."<hr class=dotline>大区：".$RegionID."<hr class=dotline>账号名:".$AccName."<hr class=dotline>冻结原因:".$Reason."<hr class=dotline>账号:(".$AccName.")";
	}
	//TalkAccount($RegionID,$AccName,$Enable,$Time)
	$j=0; //记录操作成功的账号个数
	$jj = 0; //记录操作失败的账号个数
	$arr = "";
	for($i=0;$i<count($accs);$i++)
	{
		$AccName = $accs[$i];
		$result=LockAccount($RegionID,$AccName,$Enable,$dtime,$Reason);
		if($result->ErrorCode==0)
		{
			$j++;
		}
		if($result->ErrorCode!=0)
		{
			//错误码
			$arr .= $result->ErrorCode."：".$result->StrError->ErrorText."|";
			$jj++;
		}
	}
	if($jj == 0)
	{
		$opst = 0;
	}
	else if($j == 0)
	{
		$opst = 1;
	}
	else
	{
		$opst = 2;
	}
	$log2 = "<hr class=dotline>".$j."个账号操作成功".$jj."个账号操作失败";
	$log3 = "<hr class=dotline>错误码：错误信息：".$arr;
	$res = $j."个账号操作成功".$jj."个账号操作失败，\n错误码：错误信息：".$arr;;
	$gmlog = $log1.$log2.$log3;
	$sql = "INSERT INTO `gmlog` VALUES (null, '$user[id]', '$_POST[account]', '', '$RegionID ', '', now(), now(),'', '66','$opst','$gmlog');";
	$dosql->ExecNoneQuery($sql);
	echo json_encode($res);
}
?>