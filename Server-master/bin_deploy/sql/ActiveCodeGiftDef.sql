CREATE TABLE IF NOT EXISTS ActiveCodeGiftDef (
 PreCode VARCHAR(16)  NOT NULL  ,
 Comment VARCHAR(32)    ,
 GiftName VARCHAR(32)    ,
 ItemID INT    ,
 ItemNum INT    ,
PRIMARY KEY(PreCode))  ENGINE=InnoDB DEFAULT CHARSET=latin1;
