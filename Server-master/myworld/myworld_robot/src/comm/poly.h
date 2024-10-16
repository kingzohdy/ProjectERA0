/*
**      XPOLY.H
**      Poly functions header.
**      ZJian,2000.7.25.
**
**	Port to linux, and change it to C.
**	Steve 2003.12.11
*/
#ifndef  _XPOLY_H_INCLUDE_
#define  _XPOLY_H_INCLUDE_      1

#include "masktypes.h"

typedef enum POLY_PARAMETER_ENUMS
{ MAX_POLY_NUM              = 15,   // 輪廓最多頂點數
RIGHTFUL_POLY_MIN_NUM       = 3,    // 合法輪廓的最少頂點數
UNDER_SURFACE               = 0,    // 底平面
COVER_SURFACE               = 1     // 頂平面
} POLY_PARAMETEW;

//poly type defines -------------------------------------------------------------------
typedef enum POLY_TYPE_VALUE_ENUMS
{ POLY_TYPE_MAST            = 1,    // 柱体(普通物件輪廓)
POLY_TYPE_TRAPEZIA          = 2,    // 棱体(階梯類有高度物件輪廓)
POLY_TYPE_HORIZONTAL        = 3,    // 水平面(地毯類物件輪廓)
POLY_TYPE_VERTICAL          = 4,    // 垂面(張貼類物件輪廓)
POLY_TYPE_BEVEL             = 5,    // 斜面(橋面類物件輪廓)
} POLY_TYPE_VALUE;

//check view deeply of two segments or two objects ------------------------------------
typedef enum VIEW_DEEPLY_RESULTS
{ DEEPLY_UNKNOWN    =   0,
DEEPLY_FIRST        =   1,
DEEPLY_SECOND       =   2,
} VIEW_DEEPLY_RESULTS;

//compute intersect points of two segments result -------------------------------------
typedef enum INTERSECT_POINTS_RESULT_ENUMS
{ INTERSECT_NO      =   0,
INTERSECT_OK        =   1,
INTERSECT_ONE       =   0x1,
INTERSECT_TWO       =   0x2,
} INTERSECT_POINTS_RESULT;

// D3 draw poly control flags ---------------------------------------------------------
typedef enum DRAW_POLY_CONTROL_FLAG_ENUMS
{ PDP_HEIGHT        =   0x00000001,     // barycenter z height
PDP_BASE            =   0x00000002,     // under(base) lines
PDP_TOP             =   0x00000004,     // cover(top) lines
PDP_ARRIS           =   0x00000008,     // arris
PDP_CROSS           =   0x00000010,     // line(segment) start & end point cross
PDP_FACE            =   0x00000020,     //
PDP_VALID           =   0x00000040,     // if this poly is valid
PDP_NUMBER          =   0x00000080,     // special points number(sx,lx,sy,ly,sz,lz ...)
PDP_ZHEIGHT         =   0x00000100,     // global world height
PDP_PROJECTION      =   0x00000200,     // projection to 0-height horizontal surface
PDP_CROSSBOX        =   0x00000400,     // segment start & end point crossbox
PDP_RED             =   0x00010000,     // color red
PDP_GREEN           =   0x00020000,     // color green
PDP_BLUE            =   0x00030000,     // color blue
PDP_YELLOW          =   0x00040000,     // color yellow
PDP_CYAN            =   0x00050000,     // color cyan
PDP_PINK            =   0x00060000,     // color pink
PDP_COLOR           =   0x00ff0000,     // colors mask
} DRAW_POLY_CONTROL_FLAG;

// D3 find poly barycenter results
typedef enum FIND_POLY_BARYCENTER_RESULTS_ENUMS
{ FIND_BARYCENTER_UNDER =   1,
FIND_BARYCENTER_COVER   =   2,
FIND_BARYCENTER_NONE    =   0
} FIND_POLY_BARYCENTER_RESULTS;

// D3 seek relation point poly
typedef enum SEEK_RELATION_POINT_POLY_RESULTS_ENUMS
{ SEEK_RELATION_NONE    =   0x0,
SEEK_RELATION_FIRST     =   0x1,
SEEK_RELATION_SECOND    =   0x2
} SEEK_RELATION_POINT_POLY_RESULTS;

// D3 add point to poly --- add flags
typedef enum ADD_POINT_TO_POLY_FLAGS_ENUMS
{ ADD_SPECIFY_X         =   0x1,
ADD_SPECIFY_Y           =   0x2,
ADD_SPECIFY_COVER_Z     =   0x4,
ADD_SPECIFY_UNDER_Z     =   0x8
} ADD_POINT_TO_POLY_FLAGS;

//D3 set poly point underside flags
typedef enum SET_POLY_POINY_UNDERSIDE_FLAGS_ENUMS
{ SET_SPECIFY_X         =   0x1,
SET_SPECIFY_Y           =   0x2,
SET_SPECIFY_Z           =   0x4
} SET_POLY_POINY_UNDERSIDE_FLAGS;


//--------------------------------------------------------------------------------------
#pragma pack(1)
struct tagPOLY
{
	int    pn;             // 輪廓邊數(水平投影)
	unsigned int    type;           // 輪廓類型

	unsigned char     min_x;          // 最左頂點索引值
	unsigned char     max_x;          // 最右頂點索引值
	unsigned char     min_y;          // 最上頂點索引值 
	unsigned char     max_y;          // 最下頂點索引值 
	unsigned char     min_z;          // 最低頂點索引值
	unsigned char     max_z;          // 最高頂點索引值

	unsigned char     reserved1;      // 保留值1
	unsigned char     reserved2;      // 保留值2
	unsigned int    reserved3;      // 保留值3

	int    bx;             // 重心點坐標 X
	int    by;             // 重心點坐標 Y
	int    bz;             // 重心點坐標 Z(Z1)
	int    bu;             // 重心點坐標 Z0(under)

	int    px[MAX_POLY_NUM+1]; // 頂點坐標 X
	int    py[MAX_POLY_NUM+1]; // 頂點坐標 Y
	int    pz[MAX_POLY_NUM+1]; // 頂點坐標 Z(Z1)
	int    pu[MAX_POLY_NUM+1]; // 頂點坐標 Z0(under Z)
	int    pa[MAX_POLY_NUM+1]; // 頂點屬性(attrib)

	int    na[2];          // 底(頂)平面法向量(X分量)
	int    nb[2];          // 底(頂)平面法向量(Y分量)
	int    nc[2];          // 底(頂)平面法向量(Z分量)
	int    nd[2];          // 底(頂)平面方程參數 D ( na*X + nb*Y + nc*Z + nd = 0)

};
#pragma pack()

typedef struct tagPOLY		POLY,*LPPOLY;

//-------------------------------------------------------------------------------------

#define D2_INCLUDE(x,y,x1,y1,x2,y2)             (((x)>=(x1))&&((x)<=(x2))&&((y)>=(y1))&&((y)<=(y2)))
#define D3_INCLUDE(x,y,z,x1,y1,z1,x2,y2,z2)     (((x)>=(x1))&&((x)<=(x2))&&((y)>=(y1))&&((y)<=(y2))&&((z)>=(z1))&&((z)<=(z2)))

//==================================================================================
// 2 Demensions functions
//==================================================================================
extern  int   D2_is_sameside_point_line(int px1,int py1,int px2,int py2,int lx1,int ly1,int lx2,int ly2);
extern  int   D2_is_intersect_segment_segment(int px1,int py1,int px2,int py2,int qx1,int qy1,int qx2,int qy2);
extern  int   D2_is_intersect_line_poly(int x1,int y1,int x2,int y2,POLY *pl);
extern  int   D2_is_point_in_poly(int x,int y,POLY *pl);
extern  int   D2_is_segment_in_poly(int x1,int y1,int x2,int y2,POLY *pl);
extern  int   D2_is_poly_in_poly(POLY *pl1,POLY *pl2);
extern  int   D2_is_overlap_poly_poly(POLY *pl1,POLY *pl2);
extern  int   D2_check_deeply_segment_segment(int x11,int y11,int x12,int y12,int x21,int y21,int x22,int y22);
extern  int   D2_distance_point_point(int x1,int y1,int x2,int y2);
extern  int   D2_distance_point_line(int x,int y,int x1,int y1,int x2,int y2);
extern  int   D2_nipangle_line_line(int px1,int py1,int px2,int py2,int qx1,int qy1,int qx2,int qy2);
extern  int   D2_intersect_line_line(int px1,int py1,int px2,int py2,int qx1,int qy1,int qx2,int qy2,int *x,int *y);
extern  int   D2_intersect_segment_segment(int px1,int py1,int px2,int py2,int qx1,int qy1,int qx2,int qy2,int *x,int *y);
extern  int   D2_intersect_segment_poly(int *x1,int *y1,int *x2,int *y2,POLY *pl);
extern  int   D2_chip_point_poly(int x,int y,POLY *pl,int *n1,int *n2);
extern  void    D2_barycenter_poly(POLY *pl);
extern  void    D2_adjust_diamond_position(int *x,int *y,int *cell_x,int *cell_y,int diamond_width,int diamond_height);

//==================================================================================
// 3 Demensions functions
//==================================================================================
extern  int   D3_distance_point_point(int x1,int y1,int z1,int x2,int y2,int z2);
extern  int   D3_distance_point_line(int x,int y,int z,int x1,int y1,int z1,int x2,int y2,int z2);
extern  int   D3_distance_point_surface(int x,int y,int z,int a,int b,int c,int d);
extern  int   D3_distance_line_line(int x11,int y11,int z11,int x12,int y12,int z12,int x21,int y21,int z21,int x22,int y22,int z22);
extern  int   D3_nipangle_line_line(int px1,int py1,int pz1,int px2,int py2,int pz2,int qx1,int qy1,int qz1,int qx2,int qy2,int qz2);
extern  int   D3_normal_surface(int x1,int y1,int z1,int x2,int y2,int z2,int x3,int y3,int z3,int *a,int *b,int *c,int *d);
extern  int   D3_intersect_line_surface(int x1,int y1,int z1,int x2,int y2,int z2,int a,int b,int c,int d,int *x,int *y,int *z);
extern  int   D3_intersect_line_line(int px1,int py1,int pz1,int px2,int py2,int pz2,int qx1,int qy1,int qz1,int qx2,int qy2,int qz2,int *x,int *y,int *z);
extern  int   D3_intersect_segment_segment(int px1,int py1,int pz1,int px2,int py2,int pz2,int qx1,int qy1,int qz1,int qx2,int qy2,int qz2,int *x,int *y,int *z);
extern  int   D3_droop_point_line(int px,int py,int pz,int x1,int y1,int z1,int x2,int y2,int z2,int *x,int *y,int *z);
extern  int   D3_droop_point_surface(int px,int py,int pz,int a,int b,int c,int d,int *x,int *y,int *z);
extern  void    D3_make_rectangle_poly(POLY *pl,int cx,int cy,int a,int b);
extern  void    D3_make_diamond_poly(POLY *pl,int cx,int cy,int a,int b);
extern  void    D3_make_ellipse_poly(POLY *pl,int cx,int cy,int a,int b);
extern  void    D3_set_poly_height(POLY *pl,int under_z,int top_z);
extern  void    D3_neaten_poly_gon(POLY *pl);
extern  void    D3_clear_poly(POLY *pl);
extern  void    D3_add_point_to_poly(POLY *pl,int x,int y,int z,int z2,int add_flag);
extern  int   D3_find_point_poly_underside(int *x,int *y,int r,POLY *pl);
extern  int   D3_find_point_poly_barycenter(int *x,int *y,int *z,int r,POLY *pl);
extern  void    D3_adjust_poly_up_sea_level(POLY *pl);
extern  void    D3_convert_poly_relational_by_barycenter(POLY *pl);
extern  void    D3_convert_poly_relational_by_world(POLY *pl,int stand_x,int stand_y);
extern  int   D3_neaten_poly_height(POLY *pl,int n1,int n2,int n3);
extern  int   D3_seek_relation_point_poly(POLY *pl,int dist,int *x,int *y,int *z,int *x1,int *y1,int *z1,int *x2,int *y2,int *z2);
extern  int   D3_is_poly_rightful(POLY *pl);
extern  int   D3_check_deeply_poly_poly(POLY *pl1,int px1,int py1,int pz1,POLY *pl2,int px2,int py2,int pz2);
extern  int   D3_is_point_in_poly_cover(POLY *pl,int x,int y);
extern  int   D3_is_point_in_poly_underside(POLY *pl,int x,int y);
extern  int   D3_is_point_in_poly_foreside(POLY *pl,int x,int y,int *index);
extern  int   D3_seek_space_point_segment(int x1,int y1,int z1,int x2,int y2,int z2,int *x,int *y,int *z,int rr);
extern  int   D3_clip_segment(int *x1,int *y1,int *z1,int *x2,int *y2,int *z2,int rr);
extern  int   D3_compute_height_point_poly_underside(POLY *pl,int x,int y);
extern  int   D3_compute_height_point_poly_cover(POLY *pl,int x,int y);
extern  int   D3_compute_height_point_poly_foreside(POLY *pl,int side,int x,int y);
extern  int   D3_seek_relation_point_poly_ex(POLY *pl,int dist,int kx, int ky, int *x,int *y,int *z,int *x1,int *y1,int *z1,int *x2,int *y2,int *z2);


extern  POLY *  D3_create_poly(void);
extern  void    D3_destroy_poly(POLY *poly);
extern  void    D3_copy_poly(POLY *dest_poly,POLY *src_poly);
extern  void    D3_set_poly_type(POLY *poly,unsigned int type);
extern  void    D3_set_poly_point_underside(POLY *poly,int index,int x,int y,int z,int set_flag);
extern  void    D3_move_poly_by_barycenter(POLY *pl,int new_bx,int new_by);
extern  void    D3_translate_poly_world_coordinates(POLY *pl,int px,int py,int pz);

extern  int   compute_greatest_common_divisor(int mm,int nn);

extern  void    D3_draw_point(int x,int y,int z);
extern  void    D3_draw_line(int x1,int y1,int z1,int x2,int y2,int z2);
extern  void    D3_draw_poly(int px,int py,int pz,int flag,POLY *pl);

/*
extern  void    D2_fill_poly(int relation_x, int relation_y, POLY *poly,PIXEL color,BMP *bmp);
extern  int   D3_fill_poly_underside(int rx,int ry,POLY *pl,PIXEL color,BMP *bit_map);
extern  int   D3_fill_poly_cover(int rx,int ry,POLY *pl,PIXEL color,BMP *bit_map);
*/


#endif//_XPOLY_H_INCLUDE_
//=======================================================================
