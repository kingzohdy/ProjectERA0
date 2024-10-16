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
?>
<link href="style/admin.css" rel="stylesheet" type="text/css" />
<style type="text/css">
 a{ color:#00F;}
 </style>
<script type="text/javascript" src="js/jquery.min.js"></script>
<script type="text/jscript">
</script>
<body>
<table width="100%" border="1" cellpadding="0" cellspacing="0" class="mgr_table" id="ajaxlist" >
		<tr align="center" class="mgr_tr">
            <td  height="35" width="25%" >账号：<a href="#">xxx</a></td>
            <td  width="25%">角色：<a href="#">你是一头猪</a></td>
            <td width="25%" >等级：<a href="#" >88</a></td>
            <td width="25%" >职业：<a href="#">炼魂师</a></td></td>
		</tr>
        <tr align="center" class="mgr_tr">
            <td  height="35" >性别：<a href="#" >女</a></td>
            <td colspan="2">经验：<a href="#">2566353/420000000</a></td>
            <td >金币：<a href="#" >523</a></td>
		</tr>
        <tr align="center" class="mgr_tr">
          	<td  height="35" >金卷：<a href="#">256</a></td>
            <td  >银币：<a href="#">156350</a></td>
            <td  >绑银：<a href="#" >582364</a></td>
            <td  >状态：<a href="#">未在线</a></td>
		</tr>
         <tr align="center" class="mgr_tr">
        	<td  height="35" >冻结开始：<a href="#">未冻结</a></td>
            <td  >冻结结束：<a href="#" >0(小时)</a></td>
            <td  >冻结剩余：<a href="#">0(小时)</a></td>
            <td  >GM权限：<a href="#">无</a></td>
		</tr>
        <tr align="center" class="mgr_tr" style="width:100%">
        	<td  height="35" >背包空间：<a href="#">126格</a></td>
            <td  >仓库空间：<a href="#">88格</a></td>
            <td  >宠物个数：<a href="#" >5</a></td>
            <td  height="35" >坐骑个数：<a href="#">4</a></td>
		</tr>
        <tr align="center" class="mgr_tr">
            <td  height="35">公会：<a href="#">5000</a></td>
            <td  >公会职务：<a href="#">科技官员</a></td>
            <td colspan="2" >上次登录：<a href="#" >2012.11.26 11:03:35</a></td>
		</tr>
         <tr align="center" class="mgr_tr">
            <td  height="35" colspan="4" >最后下线：<a href="#">2012.11.26 23:38:56</a>
            &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;上次IP：<a href="#" >220.127.23.56</a></td>
		</tr>
        <tr>
        	<td  align="center" height="35" colspan="4">最后登录IP：<a href="#" >220.127.23.56</a>
            &nbsp; &nbsp; &nbsp; 所在地图：<a href="#">银风雪原</a>
            &nbsp; &nbsp; &nbsp; 坐标：<a href="#" >X:206 Y:1824</a></td>
        </tr>
       
</table>   
</body>
</html>
