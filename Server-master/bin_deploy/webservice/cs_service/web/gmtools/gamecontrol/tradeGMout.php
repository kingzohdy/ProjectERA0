<?php
if(isset($_GET['leadout']))
{
	$filename=iconv('UTF-8','GB2312','系统金银币交易详情');
	header("Content-type:application/vnd.ms-excel");
	header("Content-Disposition:attachment;filename=$filename.xls");
	include "soapshop.php";
	$qu = $_GET['qu'];
	$fu = $_GET['fu'];
	$result=ShopCgi($qu,$fu,4,"","","","","","","","","");
	if($result->ErrorCode != 0)
	{
		echo "$result->ErrorCode：&nbsp;".$result->ErrorCode;
		exit;
	}
	$arr = simplexml_load_string($result->StrXML);
	if($arr->Succ != 0)
	{
		echo "$arr->Succ：&nbsp;".$arr->Succ;
		exit;
	}
	$data = $arr->SysShopInfo;
	if($data->iSellGold+$data->iBuyMoney+$data->iBuyGold+$data->iSellMoney == 0)
	{
		echo iconv('UTF-8','GB2312','暂无金银币交易信息');
		exit;
	}
	date_default_timezone_set('PRC');
	echo "<table border='1' width='910px'><tr class='thead2' align='center'><td>操作类型</td><td>开始时间</td><td>执行次数</td><td>剩余次数</td><td>每单执行</td><td>执行价格</td><td>间隔时间(秒)</td><td>结束时间</td></tr>";
	if(1 == $data->iBuyGold)
	{
		$ds = $data->BuyGold;
		echo "<tr height='32' align='center'><td>购买金币</td><td>".date('Y-m-d H:i:s',(int)$ds->StartTime)."</td><td>".$ds->ExchgNum."</td><td>".$ds->RemNum."</td><td>购买：".$ds->Num."金</td><td>".$ds->Price."银</td><td>".$ds->InterVal."</td><td>".date('Y-m-d H:i:s',(int)$ds->EndTime)."</td></tr>";						
	}
	else if(1 < $data->iBuyGold)
	{					
		for($s=0;$s<$data->iBuyGold; $s++)
		{
			$ds = $data->BuyGold[$s];
			echo "<tr height='32' align='center'><td>购买金币</td><td>".date('Y-m-d H:i:s',(int)$ds->StartTime)."</td><td>".$ds->ExchgNum."</td><td>".$ds->RemNum."</td><td>购买：".$ds->Num."金</td><td>".$ds->Price."银</td><td>".$ds->InterVal."</td><td>".date('Y-m-d H:i:s',(int)$ds->EndTime)."</td></tr>";
		}
	}
	if(1 == $data->iBuyMoney)
	{
		$ds = $data->BuyMoney;
		echo "<tr height='32' align='center'><td>购买银币</td><td>".date('Y-m-d H:i:s',(int)$ds->StartTime)."</td><td>".$ds->ExchgNum."</td><td>".$ds->RemNum."</td><td>购买：".$ds->Num."银</td><td>".$ds->Price."金</td><td>".$ds->InterVal."</td><td>".date('Y-m-d H:i:s',(int)$ds->EndTime)."</td></tr>";						
	}
	else if(1 < $data->iBuyMoney)
	{					
		for($s=0;$s<$data->iBuyMoney; $s++)
		{
			$ds = $data->BuyMoney[$s];
			echo "<tr height='32' align='center'><td>购买银币</td><td>".date('Y-m-d H:i:s',(int)$ds->StartTime)."</td><td>".$ds->ExchgNum."</td><td>".$ds->RemNum."</td><td>购买：".$ds->Num."银</td><td>".$ds->Price."金</td><td>".$ds->InterVal."</td><td>".date('Y-m-d H:i:s',(int)$ds->EndTime)."</td></tr>";
		}
	}
	if(1 == $data->iSellGold)
	{
		$ds = $data->SellGold;
		echo "<tr height='32' align='center'><td>出售金币</td><td>".date('Y-m-d H:i:s',(int)$ds->StartTime)."</td><td>".$ds->ExchgNum."</td><td>".$ds->RemNum."</td><td>出售：".$ds->Num."金</td><td>".$ds->Price."银</td><td>".$ds->InterVal."</td><td>".date('Y-m-d H:i:s',(int)$ds->EndTime)."</td></tr>";						
	}
	else if(1 < $data->iSellGold)
	{					
		for($s=0;$s<$data->iSellGold;$s++)
		{
			$ds = $data->SellGold[s];
			echo "<tr height='32' align='center'><td>出售金币</td><td>".date('Y-m-d H:i:s',(int)$ds->StartTime)."</td><td>".$ds->ExchgNum."</td><td>".$ds->RemNum."</td><td>出售：".$ds->Num."金</td><td>".$ds->Price."银</td><td>".$ds->InterVal."</td><td>".date('Y-m-d H:i:s',(int)$ds->EndTime)."</td></tr>";
		}
	}
	if(1 == $data->iSellMoney)
	{
		$ds = $data->SellMoney;
		echo "<tr height='32' align='center'><td>出售银币</td><td>".date('Y-m-d H:i:s',(int)$ds->StartTime)."</td><td>".$ds->ExchgNum."</td><td>".$ds->RemNum."</td><td>出售：".$ds->Num."银</td><td>".$ds->Price."金</td><td>".$ds->InterVal."</td><td>".date('Y-m-d H:i:s',(int)$ds->EndTime)."</td></tr>";						
	}
	else if(1 < $data->iSellMoney)
	{					
		for($s=0;$s<$data->iSellMoney;$s++)
		{
			$ds = $data->SellMoney[s];
			echo "<tr height='32' align='center'><td>出售银币</td><td>".date('Y-m-d H:i:s',(int)$ds->StartTime)."</td><td>".$ds->ExchgNum."</td><td>".$ds->RemNum."</td><td>出售：".$ds->Num."银</td><td>".$ds->Price."金</td><td>".$ds->InterVal."</td><td>".date('Y-m-d H:i:s',(int)$ds->EndTime)."</td></tr>";
		}
	}
	echo "<tr height='32' align='center'><td colspan='8'>系统交易总单数：".$data->ExchgInfo->ExchgNum."，系统回收金币总量：".$data->ExchgInfo->Count."，系统回收金币消耗银币：".$data->ExchgInfo->OutMoneyCount."，系统回收银币总量：".$data->ExchgInfo->MoneyCount."，系统回收银币消耗金币：".$data->ExchgInfo->OutCount."</td></tr></table>";
	exit;
}
?>