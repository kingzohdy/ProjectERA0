<?php require_once(dirname(__FILE__).'/../inc/config.inc.php');
//检测是否登录
if($_SESSION['type']!="2"){

	$_SESSION = array();
	session_destroy();	
	echo '<script type="text/javascript">window.top.location.href="login.php";</script>';
	exit();
}
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>左侧菜单</title>
<link href="style/menu.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="js/jquery.min.js"></script>
<script type="text/javascript" src="js/leftmenu.js"></script>
</head>
<body>
<div class="quickbtn"> 
	<span class="quickbtn_left"><a href="changepasswrod.php" target="main" >修改密码</a></span> 
    <span class="quickbtn_right"><a>【零纪元】</a></span> 
</div>
<div class="tGradient"></div>
<div id="scrollmenu">
	<div class="scrollbar">
		<div class="track">
		</div>
	</div>
	<div class="viewport">
		<div class="overview">
			<!--scrollbar start-->
            <?php 
				if($_SESSION['admin1']=="游戏管理员" || $_SESSION['admin1']=="高级游戏管理员"){
						echo "<!--";
				} 
			?>			
			<div class="menubox">
				<div class="title" style="color:red" onclick="DisplayMenu('leftmenu022');" title="点击进行显示隐藏操作"> 管理员管理 </div>
				<div id="leftmenu022"> 
                    <a href="adminorlist_add.php" target="main">添加管理员</a>
                	<a href="adminor.php" target="main">查看/更改管理员</a> 
                </div>
                <div class="title" ><a href="OperateLog.php" target="main" style="color:red">操作记录</a> </div>
                <div class="title" ><a href="lines.php" target="main" style="color:red">在线人数</a> </div>
				<div class="title" ><a href="changename.php" target="main" style="color:#F00;">修改角色名</a></div>
			</div>
            
             <?php 
				if($_SESSION['admin1']=="游戏管理员" || $_SESSION['admin1']=="高级游戏管理员"){
						echo "-->";
				} 
			?>           	
            <div class="menubox">
				<div class="title" ><a href="closefindle.php" target="main" style="color:#F00;"> 关闭句柄 </a></div>
			</div>
            <?php 
				if($_SESSION['admin1']=="游戏管理员"){
						echo "<!--";
				} 
			?>
            <div class="menubox">
				<div class="title" ><a href="updateCDR.php" target="main" style="color:#F00;"> 换会长 </a></div>
			</div>
            <div class="menubox">
				<div class="title" ><a href="mibao.php" target="main" style="color:#F00;"> 密保挂失 </a></div>
			</div>
            <div class="menubox">
				<div class="title" ><a href="sfzlost.php" target="main" style="color:#F00;"> 身份证挂失 </a></div>
			</div>	
			<div class="menubox">
				<div class="title" style="color:red" onclick="DisplayMenu('leftmenu03');" title="点击进行显示隐藏操作"> 日志查询 </div>
				<div id="leftmenu03">
				   <a href="reg_world.php" target="main">聊天日志</a>
                   <a href="AccLog.php" target="main">帐号登录登出</a>
                   <a href="RoleLog.php" target="main">角色登录登出</a>
                   <a href="IPloginLog.php" target="main">关联查询</a>
				</div>
			</div>
            <?php 
				if($_SESSION['admin1']=="游戏管理员"){
						echo "-->";
				} 
			?>
            <div class="menubox">
				<div class="title" style="color:red" onclick="DisplayMenu('leftmenu02');" title="点击进行显示隐藏操作"> 角色操作记录 </div>
				<div id="leftmenu02">
				   <a href="npcShop.php" target="main">npc商店操作</a>
                   <a href="mailOper.php" target="main">邮件操作</a>
                   <a href="trade.php" target="main">玩家交易</a>
                   <a href="stallBuy.php" target="main">购买摆摊物品</a>
                   <a href="RideChg.php" target="main">坐骑操作</a>
                   <a href="LogRolePet.php" target="main">宠物操作</a >
                   <a href="opBuyShop.php" target="main">商城购买道具</a>
                   <a href="OpRoleItemDec.php" target="main">道具消耗</a>
                   <a href="OpSpar.php" target="main">晶石操作</a>
                   <a href="LogRoleItemDrop.php" target="main">角色丢弃道具</a>
                   <a href="LogRoleDieDrop.php" target="main">角色死亡掉落</a>
                   <a href="traceprop.php" target="main">追踪道具</a>
                   <a href="LogRoleSnapshot.php" target="main">角色快照</a>
                   <a href="OpTradeInfo.php" target="main">金币、银币交易(商城)</a>
                   <a href="AddBillRecord.php" target="main">充值查询</a>
                   <a href="OpBindGoldConsume.php" target="main">金券消耗</a>
                   <a href="OpMoneyConsume.php" target="main">绑银或银币消耗</a>
                   <a href="GodCoinConsumeRecord.php" target="main">金币消耗</a>
                   <a href="LogGift.php" target="main">礼包领取记录</a>
                   <a href="ArmMachining.php" target="main">装备加工查询</a>
				</div>
			</div>
			 <div class="menubox">
				<div class="title" style="color:red" onclick="DisplayMenu('leftmenu09');" title="点击进行显示隐藏操作"> 其他操作日志 </div>
				<div id="leftmenu09">
				   <a href="tasklog_do.php" target="main">任务操作记录</a>
				   <a href="clanlog.php" target="main">公会升级,兑换道具等</a>
				   <a href="OpClanPropertyCreate.php" target="main">公会属性产生</a>
				   <a href="OpContriConsume.php" target="main">公会属性消耗</a>
				   <a href="OpStrongInfo.php" target="main">据点占领记录</a>
				   <a href="LogWorldCitySiege.php" target="main">时间城占领记录</a>
				   <a href="roleloglist.php" target="main">角色道具操作(汇总)</a>
				   <a href="LogRolePickItem.php" target="main">道具拾取</a>
				</div>
			</div>
            <div class="menubox">
				<div class="title" style="color:red" onclick="DisplayMenu('leftmenu04');" title="点击进行显示隐藏操作"> 帐号查询 </div>
				<div id="leftmenu04">
                   <a href="AccountSelect.php" target="main">输入账号查找</a>
                   <a href="roleSelect.php" target="main">输入角色查找</a>
                   <a href="FreezeAccountPL.php" target="main">批量封停账号</a>     
				</div>
			</div>
            <div class="menubox">
				<div class="title" style="color:red" onclick="DisplayMenu('leftmenu05');" title="点击进行显示隐藏操作"> 角色查询 </div>
				<div id="leftmenu05">
                   <a href="SelectRole.php" target="main">输入角色查找</a>
                   <a href="rolelists.php" target="main">角色列表</a>     
				</div>
			</div>
            <?php 
				if($_SESSION['admin1']=="游戏管理员"){
						echo "<!--";
				} 
			?>
            <div class="menubox">
				<div class="title" ><a href="LogStore.php" target="main" style="color:#F00;">玩家挂单</a></div>
			</div>	
            <div class="menubox">
				<div class="title" style="color:red" onclick="DisplayMenu('leftmenu06');" title="点击进行显示隐藏操作"> 公告管理 </div>
				<div id="leftmenu06">
				   <a href="noticeADD.php" target="main">发布公告</a>
                   <a href="noticeList.php" target="main">公告列表</a>
				</div>
			</div>
            <?php 
				if($_SESSION['admin1']=="游戏管理员" ){
						echo "<!--";
				} 
			?>	
		</div>
	</div>
</div>
<div class="bGradient"></div>
<div class="hr_5"></div>
</body>
</html>