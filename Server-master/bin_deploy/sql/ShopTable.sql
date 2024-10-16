CREATE TABLE IF NOT EXISTS ShopDef (
 ShopID INT UNSIGNED  NOT NULL  ,
 Name VARCHAR(32)    ,
 Type TINYINT    ,
 SubType TINYINT    ,
 ShopDesc VARCHAR(128)    ,
 IconID INT UNSIGNED    ,
 Items1_ItemID INT UNSIGNED    ,
 Items1_ItemNum INT UNSIGNED    ,
 Items2_ItemID INT UNSIGNED    ,
 Items2_ItemNum INT UNSIGNED    ,
 Items3_ItemID INT UNSIGNED    ,
 Items3_ItemNum INT UNSIGNED    ,
 Items4_ItemID INT UNSIGNED    ,
 Items4_ItemNum INT UNSIGNED    ,
 Items5_ItemID INT UNSIGNED    ,
 Items5_ItemNum INT UNSIGNED    ,
 Items6_ItemID INT UNSIGNED    ,
 Items6_ItemNum INT UNSIGNED    ,
 Items7_ItemID INT UNSIGNED    ,
 Items7_ItemNum INT UNSIGNED    ,
 Items8_ItemID INT UNSIGNED    ,
 Items8_ItemNum INT UNSIGNED    ,
 Flag TINYINT    ,
 Sale TINYINT    ,
 OrderID INT UNSIGNED    ,
 BuyType TINYINT    DEFAULT '1',
 BuyPrice INT UNSIGNED    ,
 BuyUIType INT UNSIGNED    ,
 ShowDate VARCHAR(15)    ,
 HideDate VARCHAR(15)    ,
 LimitDate INT    ,
PRIMARY KEY(ShopID))  ENGINE=InnoDB DEFAULT CHARSET=latin1;
create index ShopDef_Idx_0 on ShopDef ( ShopID);
CREATE TABLE IF NOT EXISTS ShopLimitDef (
 ShopID INT UNSIGNED  NOT NULL  ,
 LimitSellTotalNum INT UNSIGNED    ,
 LimitSelledNum INT UNSIGNED    ,
 ShopLimitTime1_SellBeginTime VARCHAR(9)    ,
 ShopLimitTime2_SellBeginTime VARCHAR(9)    ,
 ShopLimitTime3_SellBeginTime VARCHAR(9)    ,
 ShopLimitTime4_SellBeginTime VARCHAR(9)    ,
 ShopLimitTime5_SellBeginTime VARCHAR(9)    ,
 ModifyFlag TINYINT    ,
 FreshTime INT UNSIGNED    ,
PRIMARY KEY(ShopID))  ENGINE=InnoDB DEFAULT CHARSET=latin1;
create index ShopLimitDef_Idx_0 on ShopLimitDef ( ShopID);
CREATE TABLE IF NOT EXISTS ShopMsgDef (
 seqID INT UNSIGNED  NOT NULL  ,
 ShopMsg TEXT    ,
PRIMARY KEY(seqID))  ENGINE=InnoDB DEFAULT CHARSET=latin1;
create index ShopMsgDef_Idx_0 on ShopMsgDef ( seqID);
CREATE TABLE IF NOT EXISTS VipBarDef (
 DefID INT  NOT NULL  ,
 IP VARCHAR(16)    ,
 Mask VARCHAR(16)    ,
 BarID INT    ,
 Level INT    ,
PRIMARY KEY(DefID))  ENGINE=InnoDB DEFAULT CHARSET=latin1;
create index VipBarDef_Idx_0 on VipBarDef ( IP);
CREATE TABLE IF NOT EXISTS VipBarLevelDef (
 Level INT  NOT NULL  ,
 ExpRate INT    ,
 BootyRate INT    ,
PRIMARY KEY(Level))  ENGINE=InnoDB DEFAULT CHARSET=latin1;
CREATE TABLE IF NOT EXISTS VipBarNumDef (
 Num INT  NOT NULL  ,
 ExExpRate INT    ,
 ExBootyRate INT    ,
PRIMARY KEY(Num))  ENGINE=InnoDB DEFAULT CHARSET=latin1;
CREATE TABLE IF NOT EXISTS RegionGlobal (
 ID INT UNSIGNED  NOT NULL  ,
 Misc BLOB    ,
PRIMARY KEY(ID))  ENGINE=InnoDB DEFAULT CHARSET=latin1;
