<?php	require_once(dirname(__FILE__).'/../inc/config.inc.php');
if($_SESSION['admin1'] != "超级游戏管理员")
	{
		$_SESSION = array();
		session_destroy();	
		echo '<script type="text/javascript">window.top.location.href="login.php";</script>';
		exit();
	}
/*
**************************
(C)2010-2012 phpMyWind.com
update: 2012-1-13 14:55:42
person: Feng
**************************
*/
$tbname="adminor";
//初始化未传递参数
$action  = isset($action) ? $action : '';
$keyword = isset($keyword) ? $keyword : '';
$styleal = '';
$stylet1 = '';
$stylet0 = '';
$styles1 = '';
$styles0 = '';


//删除单条记录
if($action == 'del')
{
	$dosql->ExecNoneQuery("DELETE FROM `#@__$tbname` WHERE id=$id");
}


//删除选中记录
if($action == 'delall')
{
	if($ids != '')
	{
		$dosql->ExecNoneQuery("DELETE FROM `#@__$tbname` WHERE id IN ($ids)");
	}
}
//启用、禁用选中记录
if($action == 'prem')
{
	if($ids != '')
	{
		$dosql->ExecNoneQuery("UPDATE `#@__$tbname` SET prem='99' WHERE id IN ($ids)");
	}
}

//设置属性样式及查询语句
switch($flag)
{
	case 'all':
		$flagquery = 'id<>0';
		$styleal = 'onflag';
	break;  
	case 'group1':
		$flagquery  = "group=1";
		$stylet1 = 'onflag';
	break;
	case 'group2':
		$flagquery  = "group=0";	
		$stylet0 = 'onflag';
	break;
	case 'status':
		$flagquery  = "group=1";
		$styles1 = 'onflag';
	break;
	case 'status2':
		$flagquery  = "group=0";	
		$styles0 = 'onflag';
	break;
}

//Ajax输出数据
?>
<div class="mgr_divt">
		
		<div id="search" class="search"> <span class="s">
				<input name="keyword" id="keyword" type="text" title="输入标题名进行搜索" value="<?php echo $keyword; ?>" />
				</span> <span class="b"><a href="javascript:;" onclick="GetSearch();"><img src="../templates/images/search_btn.png" title="搜索" /></a></span></div>
</div>
<table width="100%" border="0" cellpadding="0" cellspacing="0" class="mgr_table" id="ajaxlist">
		<tr class="thead2" align="center">
				<td width="5%"><input type="checkbox" name="checkid" id="checkid" onclick="CheckAll(this.checked);"></td>
                <td>ID</td>
				<td >用户名</td>
				<td >类型</td>
				<td class="noborder">操作</td>
		</tr>
		<?php
		//检查全局分页数
		if(empty($cfg_pagenum))  $cfg_pagenum = 12;

		//设置sql
		$sql = "SELECT * FROM `#@__$tbname` WHERE prem=3 or prem=4 ";
		$dopage->GetPage($sql,$cfg_pagenum,'id ASC');
	
		while($row = $dosql->GetArray())
		{
		?>
		<tr align="center" class="mgr_tr" onmouseover="this.className='mgr_tr_on'" onmouseout="this.className='mgr_tr'">
            <td height="32"><input type="checkbox" name="checkid[]" id="checkid[]" value="<?php echo $row['id']; ?>" /></td>
            <td align="center" class="titles"><?php echo $row['id']; ?></td>
            <td align="center" class="titles"><?php echo $row['username']; ?></td>
           	<td><?php if($row['prem']==3){echo "高级游戏管理员";}; if($row['prem']==4){echo "游戏管理员";} ?></td>
            <td class="action"><span id="check<?php echo $row['id']; ?>">
            <span>[<a href="adminorlist_update.php?tid=<?php echo $tid; ?>&id=<?php echo $row['id']; ?>">修改</a>]
            </span><span>[<a href="javascript:;" onclick="ClearInfo(<?php echo $row['id']; ?>)">删除</a>]</span></td>
		</tr>
		<?php
		}
		?>                                        
</table>
<div class="mgr_divb"> 
    <div class="selall"><span>选择：</span> <a href="javascript:CheckAll(true);">全部</a> - <a href="javascript:CheckAll(false);">取消</a> - <a href="javascript:;" onclick="AjaxClearAll();">删除</a></div>
    <span class="mgr_btn"><a href="adminorlist_add.php">添加列表信息</a></span>
</div>
<div class="page_area">
	<?php echo $dopage->AjaxPage(); ?>
</div>