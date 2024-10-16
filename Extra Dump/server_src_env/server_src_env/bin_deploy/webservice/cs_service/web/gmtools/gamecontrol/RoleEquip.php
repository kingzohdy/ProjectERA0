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
function ShowDIV(thisObjID) {
	var x = event.clientX;
	var y = event.clientY;
    var yscroll = document.documentElement.scrollTop;
	//$("#" + thisObjID).show().css({"left":x,"top":y});
	$("#" + thisObjID).show();
    //$("#" + thisObjID ).css("top", "100px");
    //$("#" + thisObjID ).css("display", "block");
    document.documentElement.scrollTop = 0;
 }
 function ColseDIV(thisObjID){
    $("#" + thisObjID ).hide();	
}
</script>
<body>
<table width="100%" border="1" cellpadding="0" cellspacing="0" class="mgr_table" id="ajaxlist" >
		<tr align="center" class="mgr_tr">
            <td  height="15"  align="center" colspan="4" >&nbsp;&nbsp;<a href="#" onClick="ShowDIV('DialogDiv');" onMouseDown="ColseDIV('DialogDiv')"  >主手</a></td>
         	<td rowspan="14" width="60%" style="background:#FFF;">
            	<table id="DialogDiv">
                	<tr align="left">
           				 <td  height="15"  align="left"  >装备名称：<a href="#"  >卓越的梦魇深渊炙炎</a></td>
					</tr>
        			<tr align="left" >
            			<td  height="15"  align="left" >装备等级：<a href="#"  >81</a></td>
					</tr>
       	 			<tr align="left" >
          				 <td  height="15"  align="left"  >耐久度：<a href="#"  >800/800</a></td>
					</tr>
        			<tr align="left" >
            			<td  height="15"  align="left"  >职业需求：<a href="#"  >炼魂师</a></td>
					</tr>
        			<tr align="left" >
            			<td  height="auto"  align="left"  >强化星级：<a href="#"  >(1星10%，2星8%，3星6%，4星4%，5星3%，6星3%，7星1%，8星4%，9星2%)</a></td>
					</tr>
       				<tr align="left" >
						<td  height="15"  align="left"  >基础属性：<a href="#"  >2330-5208(2640-5280)</a></td>
					</tr>
        			<tr align="left" >
            			<td height="auto" align="left">附加属性:<a href="#"  >
                        (魔法攻击+236;物理上害减免+56;体质+87;破防等级+32;感知+65;攻击枪手玩家伤害+3.1999999)</a></td>
					</tr>
        			<tr align="left">
            			<td  height=""  align="left" >宝石镶嵌状态：<a href="#"  >
                        (无暇生命宝石：生命值+142;无暇秘法宝石：魔法防御+36;无暇法术宝石：魔法攻击+32;)</a></td>
					</tr>
        			<tr align="left">
            				<td  height="15"  align="left"  >星级属性：<a href="#"  >无</a></td>
					</tr> 
                </table>
            </td>
		</tr>
        <tr align="center" class="mgr_tr">
        <td  height="15"  align="center" colspan="4" >&nbsp;&nbsp;<a href="#" onClick="ShowDIV('DialogDiv');" >副手</a></td>
        </tr>
        <tr align="center" class="mgr_tr">
            <td  height="15"  align="center" colspan="4" >&nbsp;&nbsp;<a href="#" onClick="ShowDIV('DialogDiv');" >头盔</a></td>
		</tr>
        <tr align="center" class="mgr_tr">
        <td  height="15"  align="center" colspan="4" >&nbsp;&nbsp;<a href="#" onClick="ShowDIV('DialogDiv');" >项链</a></td>
        </tr>
        <tr align="center" class="mgr_tr">
           <td  height="15"  align="center" colspan="4" >&nbsp;&nbsp;<a href="#" onClick="ShowDIV('DialogDiv');" >肩甲</a></td>
		</tr>
        <tr align="center" class="mgr_tr">
            <td  height="15"  align="center" colspan="4" >&nbsp;&nbsp;<a href="#" onClick="ShowDIV('DialogDiv');" >面饰</a></td>
		</tr>
        <tr align="center" class="mgr_tr">
           <td  height="15"  align="center" colspan="4" >&nbsp;&nbsp;<a href="#" onClick="ShowDIV('DialogDiv');" >戒指1</a></td>
		</tr>
        <tr align="center" class="mgr_tr">
            <td  height="15"  align="center" colspan="4" >&nbsp;&nbsp;<a href="#" onClick="ShowDIV('DialogDiv');" >戒指2</a></td>
		</tr>
        <tr align="center" class="mgr_tr">
            <td  height="15"  align="center" colspan="4" >&nbsp;&nbsp;<a href="#" onClick="ShowDIV('DialogDiv');" >胸甲</a></td>
		</tr>
        <tr align="center" class="mgr_tr">
            <td  height="15"  align="center" colspan="4" >&nbsp;&nbsp;<a href="#" onClick="ShowDIV('DialogDiv');" >鞋子</a></td>
        </tr>
        <tr align="center" class="mgr_tr">
            <td  height="15"  align="center" colspan="4" >&nbsp;&nbsp;<a href="#" onClick="ShowDIV('DialogDiv');" >护符1</a></td>
		</tr>
        <tr align="center" class="mgr_tr">
            <td  height="15"  align="center" colspan="4" >&nbsp;&nbsp;<a href="#" onClick="ShowDIV('DialogDiv');" >护符2</a></td>
		</tr>
        <tr align="center" class="mgr_tr">
            <td  height="15"  align="center" colspan="4" >&nbsp;&nbsp;<a href="#" onClick="ShowDIV('DialogDiv');" >徽章</a></td>
		</tr>
         <tr align="center" class="mgr_tr">
            <td  height="15"  align="center" colspan="4" >&nbsp;&nbsp;<a href="#" onClick="ShowDIV('DialogDiv');" >圣杯</a></td>
		</tr>
</table>
</body>
</html>
