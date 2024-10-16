
#ifndef _COMMON_H
#define _COMMON_H

#define STX		0x02
#define ETX		0x03
#define RS		0x1e
#define US		0x1f

//IP AREA

#define	ID_UNKOWN	0
#define	ID_CHINA	1
#define	ID_JAPAN	2
#define	ID_IND		3
#define	ID_TAILAND	4
#define	ID_TAIWAN	5
#define	ID_SOUTHAFRICA	6
#define	ID_HONGKONG	7
#define	ID_MACAO	8

#define MAX_AREA_ID	8

static char asAreaName[][30]={
	"Unknown",
	"China",
	"Japan",
	"Indonesia",
	"Tailand",
	"Taiwan",
	"SouthAfrica",
	"Hongkong",
	"Macao"
};


#endif
