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
            <td  height="20"  align="left" colspan="2" >&nbsp;&nbsp;背包
		</tr>
        <tr align="center" class="mgr_tr">
            <td  height="20" >硬币：<a href="#" >30256</a></td>
            <td >绑银：<a href="#">12620</a></td>
		</tr>
        <tr align="center" class="mgr_tr">
            <td  height="20"  align="left" colspan="2" >&nbsp;&nbsp;仓库
		</tr>
        <tr align="center" class="mgr_tr">
            <td  height="20" colspan="2" >硬币：<a href="#" >300000</a></td>
		</tr>
        <tr align="center" class="mgr_tr">
            <td  height="20"  align="left" colspan="2" >&nbsp;&nbsp;商城
		</tr>
       <tr align="center" class="mgr_tr">
            <td height="20" >金币：<a href="#" >3680</a></td>
            <td >金卷：<a href="#" >2050</a></td>
       </tr>
</table>   
</body>
</html>
