<?php 
	require_once(dirname(__FILE__).'/../inc/config.inc.php'); 
	if($_SESSION['type'] != "5")
	{
		$_SESSION = array();
		session_destroy();	
		echo '<script type="text/javascript">window.top.location.href="login.php";</script>';
		exit();
	}
$tbname="sysgmlog";
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
                <td width="10%">操作者</td>
                <td width="14%">操作类型</td>
                <td width="8%">大区ID</td>
                <td width="8%">服ID</td>
                <td width="10%">执行总单数</td>               
                <td width="12%">开始执行时间</td>
                <td width="8%">间隔时间</td>
                <td width="12%">结束时间</td>
                <td width="10%">执行情况</td>
                <td width="8%">操作</td>
         </tr>
 		</table>
        <table width="100%" border="0" cellpadding="0" cellspacing="0" class="mgr_table" id="ajaxlist2">
        <?php	
			$sql = "select * from `sysgmlog` ";
			$dopage->GetPage($sql,13,'LogId DESC');
			while($row = $dosql->GetArray()){
		 ?>
        	 <tr align="center" height="32">
                 <td width="8%">
				 	<?php  
						$sql2 = "select * from `adminor` where `id` = '$row[UserId]' ";
					    $rr = mysql_query($sql2);
					    $user = mysql_fetch_array($rr); echo $user['username']
					 ?>
                 </td>
                 <td width="14%"><?php echo $row['optype'] ?></td>
                 <td width="8%"><?php echo $row['qu'] ?></td>
                 <td width="8%"><?php echo $row['fu'] ?></td>
                 <td width="10%"><?php echo $row['dnums'] ?></td>
                 <td width="12%"><?php if(0 == $row['stime']){ echo "";}else{ echo $row['stime'];} ?></td>
                 <td width="8%"><?php echo $row['jgtime'] ?>秒</td>
                 <td width="12%"><?php if(0 == $row['stime']){ echo "";}else{ echo $row['etime'];} ?></td>
                 <td width="8%"><?php if($row['flag'] == 1) { echo "成功" ; } else if($row['flag'] == 0){ echo "失败";}?></td>
                 <div id="<?php echo $row['logid'] ?>" class="DialogDiv" style="display:none; background:#FFF; margin-top:20px; border:1px solid #ccc">
                    <div style="height:10px"></div>
                    <div align="center" width="80%" height="80%">
                    <pre style="font-size:14px;"><?php echo $row['log']; ?></pre>
                    <input style="text-align:center; margin-top:8px; margin-bottom:8px;" type="button" value="确认" onclick="closeDIV(<?php echo $row['logid'] ?>)" />
                    </div>
                </div>
                <td width="8%"><a href="#" onclick="ShowDIV(<?php echo $row['logid'] ?>)">详细</a></td> 
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