CREATE TABLE IF NOT EXISTS Global (
 ID INT UNSIGNED  NOT NULL  ,
 ClanLastIdx INT UNSIGNED    ,
 Misc MEDIUMBLOB     ,
 GoldRate INT UNSIGNED    ,
 GoldRateData BLOB    ,
 LevelFeng INT    ,
 LevelFengEnd INT    ,
PRIMARY KEY(ID))  ENGINE=InnoDB DEFAULT CHARSET=latin1;
