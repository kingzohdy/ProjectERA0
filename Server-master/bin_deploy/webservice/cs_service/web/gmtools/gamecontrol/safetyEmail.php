<?php 
	require_once(dirname(__FILE__).'/../inc/config.inc.php'); 
	//检测用户是否有权限
if($_SESSION['admin1'] != "高级游戏管理员" && $_SESSION['admin1'] != "超级游戏管理员")
{
	$_SESSION = array();
	session_destroy();	
	echo '<script type="text/javascript">window.top.location.href="login.php";</script>';
	exit();
}
//验证身份证
if(isset($_POST['idc']))
{
	$err = "";
	$j = 0;
	$idc = $_POST['idc'];
	$accout = $_POST['acc'];
	$idcArray = str_split($idc);
	$num1 = count($idcArray);
	
	$GLOBALS['db_host'] = $hostW;
	$GLOBALS['db_user'] = $userW;
	$GLOBALS['db_name'] = $nameW;
	$GLOBALS['db_pwd'] = $pwdW;
	
	$dosql->Close(true);
	$dosql = new MySql(false);
	
	$sql = "select * from `ljy_member_profile` where `username`='$accout' ";
	
	$dosql->Execute($sql);
	$rowdb = $dosql->GetArray();
	
	$idcard = $rowdb['idcard'];
	$idcardArray = str_split($idcard);
	//print_r($idcardArray);
	$num2 = count($idcardArray);

	if($idcard == '')
	{
		$err = -1;
	}
	else if($num1 != $num2)
	{
		$err = -1; 
	}
	else if($num1 == $num2)
	{
		for($i=0;$i<$num1;$i++)
		{
			if($idcArray[$i] != $idcardArray[$i])
			{
				$j++;
			}
		}
		$err = $j; 
	}
	echo json_encode($err);
	exit;
}
if(isset($_GET['AccName']))
{
	$AccName = $_GET['AccName'];
}
if(isset($_POST['AccountM']))
{
	$zhanghao = $_POST['AccountM'];
	$youxiang = $_POST['mail'];
	
	$uid = $_SESSION['id']; 
	$sql = "select * from `adminor` where `id` = '$uid' ";
	$dosql->Execute($sql);
	$user = $dosql->GetArray();
	
	//设置时区
	date_default_timezone_set('PRC');
	//获取当前时间
	$dqtime = date('Y-m-d H:i:s');
	//日志记录
	$log1 = "<b>修改玩家安全邮箱</b><hr class=dotline>操作者：".$user['username']."&nbsp;&nbsp;（玩家账号：".$zhanghao."）<hr class=dotline>操作时间为：".$dqtime."<hr class=dotline>邮箱改为：".$youxiang;
		
	
	$GLOBALS['db_host'] = $hostW;
	$GLOBALS['db_user'] = $userW;
	$GLOBALS['db_name'] = $nameW;
	$GLOBALS['db_pwd'] = $pwdW;
	
	$dosql->Close(true);
	$dosql = new MySql(false);
	
	$sql = "update `ljy_member_profile` set `email`='$youxiang' where `username` = '$zhanghao' ";
	$m = $dosql->ExecNoneQuery($sql);
	if($m)
	{
		$dosql->select_db('ucenter');
		$sql2 = "update `uc_members` set `email`='$youxiang' where `username` = '$zhanghao' ";
		$n = $dosql->ExecNoneQuery($sql2);
		if($n)
		{
			$GLOBALS['db_host'] = $hostGM;
			$GLOBALS['db_user'] = $userGM;
			$GLOBALS['db_name'] = $dbGM;
			$GLOBALS['db_pwd'] = $pwdGM;
			
			$dosql->Close(true);
			$dosql = new MySql(false);
			
			$log2 = "，操作成功";
			$gmlog = $log1.$log2;
			$sql = "INSERT INTO `gmlog` VALUES (null, '$user[id]', '$zhanghao', '', '', '', now(), now(),'', '14','0','$gmlog')";
			$dosql->ExecNoneQuery($sql);
		}
		else
		{
			$GLOBALS['db_host'] = $db_host;
			$GLOBALS['db_user'] = $db_user;
			$GLOBALS['db_name'] = $db_name;
			$GLOBALS['db_pwd'] = $db_pwd;
			
			$dosql->Close(true);
			$dosql = new MySql(false);
			
			$log2 = "，操作失败<hr class=dotline>错误：".$arr['ErrorCode']."，".$arr['ErrorText'];
			$gmlog = $log1.$log2;
			$sql = "INSERT INTO `gmlog` VALUES (null, '$user[id]', '$zhanghao', '', '', '', now(), now(),'', '14','1','$gmlog')";
			$dosql->ExecNoneQuery($sql);
		}
	}

	echo json_encode($n);
	exit;
}
?>
<script type="text/javascript" src="js/jquery.min.js"></script>
<script type="text/jscript">
//邮箱验证
function mail(em) 
{
	var re = /^([a-zA-Z0-9]+[_|\-|\.]?)*[a-zA-Z0-9]+@([a-zA-Z0-9]+[_|\-|\.]?)*[a-zA-Z0-9]+\.[a-zA-Z]{2,3}$/; 
	return re.test(em);
	/*
	var you=em;
	var you1= new RegExp("^www\.[a-zA-Z0-9_-]|^[a-zA-Z0-9_-]+@[a-zA-Z0-9_-]+\.com$");
	if(you1.test(you)==false){	
		document.getElementById("tyx1").innerHTML = "<span style='color:red'><strong>错误</strong></span>";
		return false;
	}
	if(you1.test(you)==true){	
		document.getElementById("tyx1").innerHTML = "<span style='color:green'><strong>正确</strong></span>";
		return true;
	}
	*/
}
function tj()
{
	var Account = $("#Account").val();
	var el=$("#xmi").val();
	mail(el);
	if(mail(el))
	{
		/*
			1.调用接口修改密码
			2.判断返回值是否修改成功(成功则关闭窗口)
		*/
		$.ajax
		({
			type: 'POST',
			url: 'safetyEmail.php',
			dataType: 'json',
			data: {"AccountM":Account,"mail":el}, 
			beforeSend:function(){
				$("#BgDiv").css({ display: "block", height: $(document).height() });
				$('#DialogDiv').show();
			 },
			success: function(msg)
			{
				if(msg)
				{
					alert("修改成功");
				}
				else
				{
					alert("修改失败,错误码("+msg+")");
				}
				javascript:parent.kangThickBoxclose();
				$('#DialogDiv').hide();
				$("#BgDiv").hide();
			}
		});
	}
}
</script>
<style type="text/css" >
#BgDiv{background-color:#F7F7F7; position:absolute; z-index:99; left:0; top:0; display:none; width:100%; height:100%;opacity:0.5;filter: alpha(opacity=50);-moz-opacity: 0.5;} 
#DialogDiv{position:absolute;width:300px; left:15%; top:-10%; margin-left:-200px; height:50px; z-index:10008; border:0px #ccc solid; padding:1px;}
</style>
<body>
<div id="BgDiv"></div>
<div id="DialogDiv" style="display:none">
	<h2 style="color:#F00">请稍等...</h2>
</div>
<p>请输入新的安全邮箱：<span style="color:#F00; font-size:12px; font-weight:bold" id="ts"></span></p>
<p>
  <input type="text" id="xmi" name="xmi" /><span id="tyx1" >注意格式</span>
	<br/>
  <input type="button"  onclick="tj()" value="确认" />
  <input type="hidden" value="<?php echo $AccName; ?>" id="Account" />
</p>
</body>
</html>
