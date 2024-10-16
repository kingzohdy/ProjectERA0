<?php
$xml = "";
$client = new SoapClient("../cfg/cs_service.wsdl",array('trace' => 1,'exceptions' => true,'cache_wsdl' => WSDL_CACHE_NONE,'features' => SOAP_SINGLE_ELEMENT_ARRAYS,"login" => "soapuser","password"=> "soapuser#1234"));
//获取服务器列表
function fwqlist()
{
	global $client;
	$xml=$client->GetRegionWorldInfo(array('TimeOut'=>0));
	return $xml;
}
//添加公告到一个服务器
/*
区ID·服ID·内容·滚动时间·活跃时间·开始时间
*/
function noticeADD($RegionID,$WorldID,$Text,$RollInterval,$ActiveTime,$ActiveTimeStart)
{
	global $client;
	$xml=$client->AddBulletin(
							array
							(
								'TimeOut'=>0,
								'RegionID'=>$RegionID,
								'WorldID'=>$WorldID,
								'Text'=>$Text,
								'RollInterval'=>$RollInterval,
								'ActiveTime'=>$ActiveTime,
								'ActiveTimeStart'=>$ActiveTimeStart
							)
					);
	return $xml;
}
/*
内容·滚动时间·活跃时间·开始时间
*/
//添加公告到所有服务器
function noticeAllADD($Text,$RollInterval,$ActiveTime,$ActiveTimeStart)
{
	global $client;
	$xml=$client->AddBulletinToAll(
							array
							(
								'TimeOut'=>0,
								'Text'=>$Text,
								'RollInterval'=>$RollInterval,
								'ActiveTime'=>$ActiveTime,
								'ActiveTimeStart'=>$ActiveTimeStart
							)
					);
	return $xml;
}
//公告列表
/*
区·服ID
*/
function noticeList($RegionID,$WorldID)
{
		global $client;
	$xml=$client->GetBulletinList(
								array
									(
										'TimeOut'=>0,
										'RegionID'=>$RegionID,
										'WorldID'=>$WorldID
									)
						);
	return $xml;
}
//从所有服务器删除一条公告
/*
删除公告ID
*/
function noticeDelet($ID)
{
	global $client;
	$xml=$client->DelBulletinFromAll(
									array
										(
											'TimeOut'=>0,
											'ID'=>$ID
										)
									);
	return $xml;
}
//从一个服务器删除一条公告
/*
大区ID,服ID,删除公告ID
*/
function noticeDeletOne($Regionid,$Worldid,$ID)
{
	global $client;
	$xml=$client->DelBulletin(
							array
									(
										'TimeOut'=>0,
										'RegionID'=>$Regionid,
										'WorldID'=>$Worldid,
										'ID'=>$ID
									)
							);
	return $xml;
}
//查找邮件
/*
第一次查找邮件.大区ID.服ID.角色名
*/
function selectFirstMail($RegionID,$WorldID,$RoleName)
{
	global $client;
	$xml=$client->FindFirstMaillist(
								array
									(
										'TimeOut'=>20000,
										'RegionID'=>$RegionID ,
										'WorldID' => $WorldID ,
										'RoleName'=>$RoleName
									)
						);
	return $xml;
}
/*
下一次查找邮件.查询句柄
*/
function selectNextMail($FindHandleID)
{
	global $client;
	$xml=$client->FindNextMaillist(
								array
									(
										'TimeOut'=>20000,
										'FindHandleID'=>$FindHandleID
									)
						);
	return $xml;
}
//用账号名查询账号
/*
账号名,(模糊查找.第一次调用)
*/
function selectFirstAccount($AccName)
{
	global $client;
	$xml=$client->FindAccountByNameFirst(
								array
									(
										'TimeOut'=>20000,
										'AccName'=>$AccName
									)
						);
	return $xml;
}
/*
用账号查询账号,下一次调用.查询句柄
*/
function selectNextAccount($FindHandleID)
{
	global $client;
	$xml=$client->FindAccountByNameNext(
								array
									(
										'TimeOut'=>20000,
										'FindHandleID'=>$FindHandleID
									)
						);
	return $xml;
}
//用角色名查找账号
/*
第一次调用,角色名
*/
function selectRoleAccountFirst($RoleName)
{
	global $client;
	$xml=$client->FindAccountByRoleFirst(
								array
									(
										'TimeOut'=>20000,
										'RoleName'=>$RoleName
									)
						);
	return $xml;
}
/*
角色名查账号,下一次调用，查询句柄
*/
function selectRoleAccountNext($FindHandleID)
{
	global $client;
	$xml=$client->FindAccountByRoleNext(
								array
									(
										'TimeOut'=>0,
										'FindHandleID'=>$FindHandleID
									)
						);
	return $xml;
}
//指定大区,用账号查询账号
/*
大区ID,账号
*/
function DQselectAccount($RegionID,$AccName)
{
	global $client;
	$xml=$client->FindRegionAccountByNameRoleFirst(
								array
									(
										'TimeOut'=>0,
										'RegionID'=>$RegionID,
										'AccName'=>$AccName
									)
						);
	return $xml;
}
//指定大区,用帐号查询帐号下一次调用
/*
查询句柄
*/
function DQselectAccountNext($FindHandleID)
{
	global $client;
	$xml=$client->FindRegionAccountByNameRoleNext(
								array
									(
										'TimeOut'=>0,
										'FindHandleID'=>$FindHandleID
									)
						);
	return $xml;
}
//指定大区用角色名查询角色(第一次)
/*
区ID,服ID,角色名
*/
function SelctRole($RegionID,$WorldID,$RoleName)
{
	global $client;
	$xml=$client->FindFirstRoleByRolename(
								array
									(
										'TimeOut'=>10000,
										'RegionID'=>$RegionID,
										'WorldID'=>$WorldID,
										'RoleName'=>$RoleName
									)
						);
	return $xml;
}
//指定大区用角色名查询角色(下一次)
/*
查询句柄
*/
function SelctRoleNxet($FindHandleID)
{
	global $client;
	$xml=$client->FindNextRole(
								array
									(
										'TimeOut'=>0,
										'FindHandleID'=>$FindHandleID
									)
						);
	return $xml;
}
//通过账号名踢角色
/*
区ID，账号名
*/
function KickAccount($RegionID,$AccName)
{
	global $client;
	$xml=$client->KickOffByLoginName(
								array
									(
										'TimeOut'=>0,
										'RegionID'=>$RegionID,
										'AccName'=>$AccName
									)
						);
	return $xml;
}
//解/封账号
/*
区ID，账号名，是否解封(0否，1是),封号时间（秒）
*/
function LockAccount($RegionID,$AccName,$Enable,$Time,$Reason)
{
	global $client;
	$xml=$client->SetAccountFreeze(
								array
									(
										'TimeOut'=>0,
										'RegionID'=>$RegionID,
										'AccName'=>$AccName,
										'Enable' =>$Enable,
										'Time' => $Time,
										'Reason' => $Reason
									)
						);		
	return $xml;
}
//禁/解言账号
/*
区id，账号名,是否禁言(0否  1是) 禁言时间（秒）
*/
function TalkAccount($RegionID,$AccName,$Enable,$Time)
{
	global $client;
	$xml=$client->SetAccountSilence(
								array
									(
										'TimeOut'=>0,
										'RegionID'=>$RegionID,
										'AccName'=>$AccName,
										'Enable' =>$Enable,
										'Time' => $Time
									)
						);
	return $xml;
}
//设置帐号安全锁
/*
区ID,帐号名,新的安全码
*/
function AccSafelock($RegionID,$AccName,$SafePwd)
{
	global $client;
	$xml=$client->SetAccSafelock(
								array
									(
										'TimeOut'=>0,
										'RegionID'=>$RegionID,
										'AccName'=>$AccName,
										'SafePwd' => $SafePwd
									)
						);
	return $xml;
}
//获取角色列表
/*
区ID,服ID,帐号Uin
*/
function RoleList($RegionID,$WorldID,$Uin)
{
	global $client;
	$xml=$client->GetRoleList(
								array
									(
										'TimeOut'=>0,
										'RegionID'=>$RegionID,
										'WorldID'=>$WorldID,
										'Uin' => $Uin
									)
						);
	return $xml;
}
//获取角色信息
/*
区ID,服ID,角色名
*/
function RoleInfo($RegionID,$WorldID,$RoleName)
{
	global $client;
	$xml=$client->GetRoleInfo(
						array
							(
								'TimeOut'=>0,
								'RegionID'=>$RegionID,
								'WorldID'=>$WorldID,
								'RoleName' =>$RoleName
							)
						);
	
	return $xml;
}
//获取物品信息
/*
区ID,服ID,角色名
*/
function ItemInfo($RegionID,$WorldID,$RoleName)
{
	global $client;
	$xml=$client->GetItemInfo(
						array
							(
								'TimeOut'=>0,
								'RegionID'=>$RegionID,
								'WorldID'=>$WorldID,
								'RoleName' =>$RoleName
							)
						);
	
	return $xml;
}
//获得技能信息
/*
区ID,服ID,角色名
*/
function SkillInfo($RegionID,$WorldID,$RoleName)
{
	global $client;
	$xml=$client->GetSkillInfo(
						array
							(
								'TimeOut'=>0,
								'RegionID'=>$RegionID,
								'WorldID'=>$WorldID,
								'RoleName' =>$RoleName
							)
						);
	
	return $xml;
}
//封角色
/*
区ID,服ID,角色名,时间
*/
function BlockRole($RegionID,$WorldID,$RoleName,$Time)
{
	global $client;
	$xml=$client->BlockRole(
						array
							(
								'TimeOut'=>0,
								'RegionID'=>$RegionID,
								'WorldID'=>$WorldID,
								'RoleName' =>$RoleName,
								'Time' => $Time
							)
						);
	
	return $xml;
}
//禁言角色
/*
区ID,服ID,角色名,时间
*/
function SilenceRole($RegionID,$WorldID,$RoleName,$Time)
{
	global $client;
	$xml=$client->SilenceRole(
						array
							(
								'TimeOut'=>0,
								'RegionID'=>$RegionID,
								'WorldID'=>$WorldID,
								'RoleName' =>$RoleName,
								'Time' => $Time
							)
						);
	return $xml;
}
//改变角色地图和坐标
/*
区ID,服ID,角色名,地图ID,坐标x,坐标Y
*/
function updateMap($RegionID,$WorldID,$RoleName,$MapID,$X,$Y)
{
	global $client;
	$xml=$client->ChgRolePos(
						array
							(
								'TimeOut'=>0,
								'RegionID'=>$RegionID,
								'WorldID'=>$WorldID,
								'RoleName' =>$RoleName,
								'MapID'=>$MapID,
								'X' => $X,
								'Y' => $Y
							)
						);
	return $xml;
}
//获取坐骑信息
/*
区ID,服ID,角色名
*/
function GetRideInfo($RegionID,$WorldID,$RoleName)
{
	global $client;
	$xml=$client->GetRideInfo(
						array
							(
								'TimeOut'=>0,
								'RegionID'=>$RegionID,
								'WorldID'=>$WorldID,
								'RoleName' =>$RoleName
							)
						);
	return $xml;
	//echo json_encode($xml);
}
//角色快照详细信息
/*
查到的二进制数据
*/
function ParseBlob($DataHex)
{
	global $client;
	$xml=$client->ParseBlob(
						array
							(
								'TimeOut'=>0,
								'MetaVersion'=>0,
								'DataHex'=>$DataHex,
								'MetaName' =>"RoleData"
							)
						);
	return $xml;
	//echo json_encode($xml);
}
//通过邮件发送物品给玩家(大区，服，收件人，发件人[默认gm]，标题，邮件文本，附带银币数量[不提供默认为0]，附件【字符串】)
function SendSysMail($RegionID,$WorldID,$ToName,$Title,$Text,$MailItems)
{
	global $client;
	$xml=$client->SendSysMail(
						array
							(
								'RegionID'=>$RegionID,
								'WorldID'=>$WorldID,
								'ToName'=>$ToName,
								'FromName' => '游戏管理员',
								'Title' => $Title,
								'Text' => $Text,
								'Money' => 0,
								'MailItems' =>$MailItems,
								'TimeOut' => 0
							)
						);
	return $xml;
}
//会长换给别人
function Epresident($RegionID,$WorldID,$ClanName,$RoleName)
{
	global $client;
	$xml=$client->ExchangeClanLeader(
						array
							(
								'RegionID'=>$RegionID,
								'WorldID'=>$WorldID,
								'ClanName' => $ClanName,
								'RoleName' => $RoleName,
								'TimeOut' => 0
							)
						);
	return $xml;
}
//恢复删除的角色
function RestoreRole($RegionID,$WorldID,$RoleWID)
{
	global $client;
	$xml=$client->RestoreDeletedRole(
						array
							(
								'RegionID'=>$RegionID,
								'WorldID'=>$WorldID,
								'RoleWID' => $RoleWID,
								'TimeOut' => 0
							)
						);
	return $xml;
}
//修改角色名
function ChgRoleName($RegionID,$WorldID,$RoleName,$NewRoleName)
{
	global $client;
	$xml=$client->ChgRoleName(
						array
							(
								'RegionID'=>$RegionID,
								'WorldID'=>$WorldID,
								'RoleName' => $RoleName,
								'NewRoleName' => $NewRoleName,
								'TimeOut' => 0
							)
						);
	return $xml;
}
//玩家宠物设置
function SetPetInfo($RegionID,$WorldID,$RoleName,$RolePetXml,$OpMode)
{
	global $client;
	$xml=$client->SetPetInfo(
						array
							(
								'RegionID'=>$RegionID,
								'WorldID'=>$WorldID,
								'RoleName' => $RoleName,
								'RolePetXml' => $RolePetXml,
								'OpMode' => (int)$OpMode,
								'TimeOut' => 20000
							)
						);
	return $xml;
}
//mail详情
function GetRoleMailData($RegionID,$WorldID,$RoleName,$MailID)
{
	global $client;
	/*$RoleName = "linzhongkai2";
	$MailID = "562947029543459";*/
	$xml=$client->GetRoleMailData(
						array
							(
								'RegionID'=>$RegionID,
								'WorldID'=>$WorldID,
								'RoleName' => $RoleName,
								'MailID' => $MailID,
								'TimeOut' => 20000
							)
						);
	return $xml;
}
//关闭句柄
/*
句柄ID
*/
function HandleClose($FindHandleID)
{
	global $client;
	$xml=$client->FindHandleClose(
							   array
									(
										'TimeOut'=>0,
										'FindHandleID'=>$FindHandleID
									)
						);
	return $xml;
}
?>
