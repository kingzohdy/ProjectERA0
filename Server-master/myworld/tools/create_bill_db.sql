CREATE TABLE IF NOT EXISTS Bill (
 Uin INT  NOT NULL  ,
 Sequence BIGINT UNSIGNED    ,
 ServerID BIGINT UNSIGNED    ,
 Amount BIGINT UNSIGNED    ,
 Points BIGINT UNSIGNED    ,
 Bonus BIGINT UNSIGNED    ,
 FreezePoint BIGINT UNSIGNED    ,
 FreezeLastTime INT    ,
 CashTotalPoints BIGINT UNSIGNED    ,
 UsedTotalPoints BIGINT UNSIGNED    ,
PRIMARY KEY(Uin))  ENGINE=InnoDB DEFAULT CHARSET=latin1;
create index Bill_Idx_0 on Bill ( Uin);
CREATE TABLE IF NOT EXISTS AddBillRecord (
 seqID BIGINT UNSIGNED  NOT NULL AUTO_INCREMENT ,
 Sequence BIGINT UNSIGNED    ,
 MessageId INT UNSIGNED    ,
 Uin INT    ,
 ServerID INT UNSIGNED    ,
 Amount BIGINT UNSIGNED    ,
 Points BIGINT UNSIGNED    ,
 Bonus BIGINT UNSIGNED    ,
 PropsID BIGINT UNSIGNED    ,
 PropsName VARCHAR(200)    ,
 LogTime DATETIME    ,
 KEY(seqID),
PRIMARY KEY(seqID))  ENGINE=InnoDB DEFAULT CHARSET=latin1 AUTO_INCREMENT=1;
create index AddBillRecord_Idx_0 on AddBillRecord ( Uin);
CREATE TABLE IF NOT EXISTS DecBillRecord (
 seqID BIGINT UNSIGNED  NOT NULL AUTO_INCREMENT ,
 Uin INT    ,
 OperType TINYINT    ,
 RoleID BIGINT UNSIGNED    ,
 DecTianYuan BIGINT UNSIGNED    ,
 ShopID INT UNSIGNED    ,
 ShopNum INT UNSIGNED    ,
 GiftFlag TINYINT    ,
 GiftToRoleID BIGINT UNSIGNED    ,
 LogTime DATETIME    ,
 KEY(seqID),
PRIMARY KEY(seqID))  ENGINE=InnoDB DEFAULT CHARSET=latin1 AUTO_INCREMENT=1;
create index DecBillRecord_Idx_0 on DecBillRecord ( Uin);
CREATE TABLE IF NOT EXISTS GodCoinConsumeRecord (
 seqID BIGINT UNSIGNED  NOT NULL AUTO_INCREMENT ,
 Uin INT    ,
 RoleID BIGINT UNSIGNED    ,
 WorldID INT    ,
 OperType INT    ,
 GodCoin BIGINT UNSIGNED    ,
 ItemID INT UNSIGNED    ,
 ItemNum INT UNSIGNED    ,
 OperDesc TEXT    ,
 LogTime DATETIME    ,
 KEY(seqID),
PRIMARY KEY(seqID))  ENGINE=InnoDB DEFAULT CHARSET=latin1 AUTO_INCREMENT=1;
create index GodCoinConsumeRecord_Idx_0 on GodCoinConsumeRecord ( Uin);
CREATE TABLE IF NOT EXISTS PropsDef (
 PropsID INT UNSIGNED  NOT NULL AUTO_INCREMENT ,
 BeginDate VARCHAR(9)    ,
 EndDate VARCHAR(9)    ,
 CashUp BIGINT UNSIGNED    ,
 PropsGiftItem1_ItemID INT    ,
 PropsGiftItem1_ItemNum INT    ,
 PropsGiftItem2_ItemID INT    ,
 PropsGiftItem2_ItemNum INT    ,
 PropsGiftItem3_ItemID INT    ,
 PropsGiftItem3_ItemNum INT    ,
 KEY(PropsID),
PRIMARY KEY(PropsID))  ENGINE=InnoDB DEFAULT CHARSET=latin1 AUTO_INCREMENT=1;
CREATE TABLE IF NOT EXISTS PropsGift (
 ID INT UNSIGNED  NOT NULL AUTO_INCREMENT ,
 PropsDefID INT UNSIGNED    ,
 GetFlag INT    ,
 CashNow BIGINT UNSIGNED    ,
 Uin INT UNSIGNED    ,
 PropsGiftItem1_ItemID INT    ,
 PropsGiftItem1_ItemNum INT    ,
 PropsGiftItem2_ItemID INT    ,
 PropsGiftItem2_ItemNum INT    ,
 PropsGiftItem3_ItemID INT    ,
 PropsGiftItem3_ItemNum INT    ,
 PropsGiftDesc VARCHAR(128)    ,
 KEY(ID),
PRIMARY KEY(ID))  ENGINE=InnoDB DEFAULT CHARSET=latin1 AUTO_INCREMENT=1;
create index PropsGift_Idx_0 on PropsGift ( Uin);
CREATE TABLE IF NOT EXISTS AccGift (
 ID INT  NOT NULL  ,
 Uin INT UNSIGNED    ,
 Type INT    ,
 Flag INT    DEFAULT '0',
 Name VARCHAR(32)    ,
 GiftDesc VARCHAR(128)    ,
 GiftItems1_ItemID INT    ,
 GiftItems1_ItemNum INT    ,
 GiftItems2_ItemID INT    ,
 GiftItems2_ItemNum INT    ,
 GiftItems3_ItemID INT    ,
 GiftItems3_ItemNum INT    ,
 GiftItems4_ItemID INT    ,
 GiftItems4_ItemNum INT    ,
PRIMARY KEY(ID))  ENGINE=InnoDB DEFAULT CHARSET=latin1;
create index AccGift_Idx_0 on AccGift ( Uin);
CREATE TABLE IF NOT EXISTS CDKeyTypeDef (
 KeyType VARCHAR(5)  NOT NULL  ,
 EndTime DATETIME    ,
 LimitNum INT    ,
 Items1_ItemID INT    ,
 Items1_ItemNum INT    ,
 Items2_ItemID INT    ,
 Items2_ItemNum INT    ,
 Items3_ItemID INT    ,
 Items3_ItemNum INT    ,
 Items4_ItemID INT    ,
 Items4_ItemNum INT    ,
PRIMARY KEY(KeyType))  ENGINE=InnoDB DEFAULT CHARSET=latin1;
CREATE TABLE IF NOT EXISTS CDKeyDef (
 CDKey VARCHAR(65)  NOT NULL  ,
 State INT    ,
 RoleID BIGINT UNSIGNED    ,
 SvrID INT    ,
PRIMARY KEY(CDKey))  ENGINE=InnoDB DEFAULT CHARSET=latin1;
