CREATE TABLE IF NOT EXISTS AccGift (
 ID INT  NOT NULL AUTO_INCREMENT ,
 Uin INT UNSIGNED    ,
 RoleID BIGINT UNSIGNED    ,
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
 SetGiftTime DATETIME    ,
 GetGiftTime DATETIME    ,
 KEY(ID),
PRIMARY KEY(ID))  ENGINE=InnoDB DEFAULT CHARSET=latin1;
create index AccGift_Idx_0 on AccGift ( Uin);
create index AccGift_Idx_1 on AccGift ( RoleID);
