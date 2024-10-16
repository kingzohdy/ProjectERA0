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
<script type="text/javascript" src="jquery.min.js"></script>
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
            <td  height="75"  align="center" colspan="4" >&nbsp;&nbsp;<a href="#" onClick="ShowDIV('DialogDiv');" onMouseDown="ColseDIV('DialogDiv')"  >宠物1</a></td>
         	<td rowspan="8" width="80%" style="background:#FFF;">
            	<table id="DialogDiv">
                	<tr align="left">
           				 <td  height="15"  align="left"  >昵称：<a href="#"  >小蓝龙</a></td>
                         <td  height="15"  align="left" >等级：<a href="#"  >89级</a></td>
					</tr>
        			<tr align="left">
                         <td  height="15"  align="left" >资质：<a href="#"  >杰出</a></td>
                         <td  height="15"  align="left" >战斗力：<a href="#"  >12326</a></td>
					</tr>
       	 			<tr align="left">
                         <td  height="15"  align="left" >召唤等级：<a href="#"  >50</a></td>
                         <td  height="15"  align="left" >替身率：<a href="#"  >5%</td>
                    <tr align="left">
                         <td  height="auto"  align="left" colspan="2" >宠物强化：<a href="#"  >(1星10%,2星10%,3星7%,4星%6,5星10%,6星10%,7星7%,8星%6,10星10%,2星10%,11星7%,12星%)</a></td>
					</tr>
                    <tr align="left">
                         <td  height="auto"  align="left" colspan="2" >宠物技能：<a href="#"  >(高级坚身+3,高级壁垒+2,高级烧尽+1,技能4,技能5)</a></td>
					</tr>
                    <tr align="left">
                         <td  height="15"  align="left" >生命值：<a href="#"  >23620</a></td>
                         <td  height="15"  align="left" >经验：<a href="#"  >153262/330000</a></td>
					</tr>
                    <tr align="left">
                         <td  height="15"  align="left" >暴击率：<a href="#"  >70%</a></td>
                         <td  height="15"  align="left" >忠诚度：<a href="#"  >99%</a></td>
					</tr>
                    <tr align="left">
                         <td  height="15"  align="left" >天赋：<a href="#"  >无</a></td>
                         <td  height="15"  align="left" >攻击成长：<a href="#"  >1854</a></td>
					</tr>
                    <tr align="left">
                         <td  height="15"  align="left" >生命成长：<a href="#"  >1384</a></td>
                         <td  height="15"  align="left" >物防成长：<a href="#"  >1252</a></td>
					</tr>
                    <tr align="left">
                         <td  height="15"  align="left" >魔防成长：<a href="#"  >1277</a></td>
                         <td  height="15"  align="left" >命中成长：<a href="#"  >1464</a></td>
					</tr>
                     <tr align="left">
                         <td  height="15"  align="left" >闪避成长：<a href="#"  >1327</a></td>
                         <td></td>
					</tr>
                    <tr align="left">
                         <td  height="15"  align="left" >宠物攻击：<a href="#"  >1327</a></td>
                         <td  height="15"  align="left" >生命值：<a href="#"  >236</a></td>
					</tr>
                    <tr align="left">
                         <td  height="15"  align="left" >物理防御：<a href="#"  >1327</a></td>
                         <td  height="15"  align="left" >魔法防御：<a href="#"  >236</a></td>
					</tr>
                    <tr align="left">
                         <td  height="15"  align="left" >命中等级：<a href="#"  >1327</a></td>
                         <td  height="15"  align="left" >闪避等级：<a href="#"  >236</a></td>
					</tr>
                </table>
            </td>
		</tr>
        <tr align="center" class="mgr_tr">
        <td  height="75"  align="center" colspan="4" >&nbsp;&nbsp;<a href="#" onClick="ShowDIV('DialogDiv');" >宠物2</a></td>
        </tr>
        <tr align="center" class="mgr_tr">
            <td  height="75"  align="center" colspan="4" >&nbsp;&nbsp;<a href="#" onClick="ShowDIV('DialogDiv');">宠物3</a></td>
		</tr>
        <tr align="center" class="mgr_tr">
        <td  height="75"  align="center" colspan="4" >&nbsp;&nbsp;<a href="#" onClick="ShowDIV('DialogDiv');" >宠物4</a></td>
        </tr>
        <tr align="center" class="mgr_tr">
           <td  height="75"  align="center" colspan="4" >&nbsp;&nbsp;<a href="#" onClick="ShowDIV('DialogDiv');" >宠物5</a></td>
		</tr>
	</table>
</body>
</html>
