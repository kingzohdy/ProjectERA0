<?php 
	require_once(dirname(__FILE__).'/../inc/config.inc.php'); 
	if($_SESSION['admin1'] != "超级游戏管理员")
	{
		$_SESSION = array();
		session_destroy();	
		echo '<script type="text/javascript">window.top.location.href="login.php";</script>';
		exit();
	}
$tbname="gmlog";
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>列表信息管理</title>
<link href="style/admin.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="js/jquery.min.js"></script>
<script type="text/javascript" src="js/listajax.js"></script>
<script type="text/javascript" >
function ShowDIV(thisObjID)
{
	  $("#BgDiv").css({ display: "block", height: $(document).height() });
	  var yscroll = document.documentElement.scrollTop;
	  $("#" + thisObjID ).css("top", "100px");
	  $("#" + thisObjID ).css("display", "block");
	document.documentElement.scrollTop = 0;
}
function closeDIV(thisObjID)
{
	  $("#BgDiv").hide();
	  $("#" + thisObjID ).hide();;
}
</script>
<style type="text/css">  
#BgDiv{background-color:#F7F7F7; position:absolute; z-index:99; left:0; top:0; display:none; width:100%; height:100%;opacity:0.5;filter: alpha(opacity=50);-moz-opacity: 0.5;} 
.DialogDiv{position:absolute;width:380px; left:55%; top:55%; margin-left:-200px; height:auto; z-index:100; border:0px #ccc solid; padding:1px;}
.dotline {BORDER-BOTTOM-STYLE: dotted; BORDER-LEFT-STYLE: dotted;BORDER-RIGHT-STYLE: dotted; BORDER-TOP-STYLE: dotted; size:1; color:green; width:90%;}

</style>
</head>
<body >

<!--/header end-->
<div class="mgr_divt">

</div>
 <div id="BgDiv"></div>
<div id="mgr_header">
<form name="form" id="form" method="post">
    	<table width="100%" border="0" cellpadding="0" cellspacing="0" class="mgr_table" id="ajaxlist1">
		<tr class="thead2" align="center">
                <td width="8%">操作者</td>
                <td width="8%">操作类型</td>
                <td width="8%">被操作帐号</td>
                <td width="8%">被操作角色</td>
                <td width="12%">操作时间</td>
                <td width="8%">大区ID</td>
                <td width="8%">服ID</td>
                <td width="12%">开始执行时间</td>
                <td width="8%">持续时间</td>
                <td width="8%">执行情况</td>
                <td width="8%">操作</td>
         </tr>
 		</table>
        <table width="100%" border="0" cellpadding="0" cellspacing="0" class="mgr_table" id="ajaxlist2">
        <?php	
			$sql = "select * from `gmlog` ";
			$dopage->GetPage($sql,13,'LogId DESC');
			while($row = $dosql->GetArray()){
				//操作类型，OperateType等于1代表添加公告，等于2代表删除公告，等于3代表通过账号踢角色，等于5代表禁言账号，等于4解禁账号,等于6代表冻结账号
				//7代表：改安全锁.8代表封角色
				if($row['OperateType'] == 1){ $row['OperateType'] = "添加公告";}
				if($row['OperateType'] == 2){ $row['OperateType'] = "删除公告";}
				if($row['OperateType'] == 3){ $row['OperateType'] = "通过帐号踢角色";}
				if($row['OperateType'] == 5){ $row['OperateType'] = "禁言帐号";}
				if($row['OperateType'] == 4){ $row['OperateType'] = "解除禁言帐号";}
				if($row['OperateType'] == 6){ $row['OperateType'] = "冻结帐号";}
				if($row['OperateType'] == 66){ $row['OperateType'] = "批量冻结帐号";}
				if($row['OperateType'] == 7){ $row['OperateType'] = "改安全锁";}
				if($row['OperateType'] == 8){ $row['OperateType'] = "封角色";}
				if($row['OperateType'] == 9){ $row['OperateType'] = "修改角色坐标";}
				if($row['OperateType'] == 10){ $row['OperateType'] = "发送装备";}
				if($row['OperateType'] == 12){ $row['OperateType'] = "发送道具";}
				if($row['OperateType'] == 13){ $row['OperateType'] = "发送晶石";}
				if($row['OperateType'] == 14){ $row['OperateType'] = "修改安全邮箱";}
				if($row['OperateType'] == 15){ $row['OperateType'] = "密保挂失";}
				if($row['OperateType'] == 16){ $row['OperateType'] = "修改会长";}
				if($row['OperateType'] == 17){ $row['OperateType'] = "修改角色名";}
				if($row['OperateState'] == 0){ $row['OperateState'] = "成功";}
				if($row['OperateState'] == 1){ $row['OperateState'] = "失败";}
				if($row['OperateState'] == 2){ $row['OperateState'] = "未完成";}
				if($row['RegionID'] == -1){ $row['RegionID'] = "全区";}
				if($row['WorldID'] == -1){ $row['WorldID'] = "全服";}
		 ?>
        	 <tr align="center" height="32">
                 <td width="8%">
				 	<?php  
						$sql2 = "select * from `adminor` where `id` = '$row[UserId]' ";
					    $rr = mysql_query($sql2);
					    $user = mysql_fetch_array($rr); echo $user['username']
					 ?>
                 </td>
                 <td width="8%"><?php echo $row['OperateType'] ?></td>
                 <td width="8%"><?php echo $row['GameUserID'] ?></td>
                 <td width="8%"><?php echo $row['GameUserRole'] ?></td>
                 <td width="12%"><?php echo $row['time'] ?></td>
                 <td width="8%"><?php echo $row['RegionID'] ?></td>
                 <td width="8%"><?php echo $row['WorldID'] ?></td>
                 <td width="12%"><?php echo $row['StartTime'] ?></td>
                 <td width="8%"><?php echo $row['ContinueTime'] ?>秒</td>
                 <td width="8%"><?php echo $row['OperateState'] ?></td>
                 <div id="<?php echo $row['LogId'] ?>" class="DialogDiv" style="display:none; background:#FFF; margin-top:20px; border:1px solid #ccc">
                    <div style="height:10px"></div>
                    <div align="center" width="80%" height="80%">
                    <pre style="font-size:14px;"><?php echo $row['log']; ?></pre>
                    <input style="text-align:center; margin-top:8px; margin-bottom:8px;" type="button" value="确认" onclick="closeDIV(<?php echo $row['LogId'] ?>)" />
                    </div>
                </div>
                <td width="8%"><a href="#" onclick="ShowDIV(<?php echo $row['LogId'] ?>)">详细</a></td> 
             </tr>
         <?php } ?>
 		</table>
<div class="page_area">
	<?php echo $dopage->AjaxPage(); ?>
</div>
</form>
</div>
<!--/list end-->
</body>
</html>