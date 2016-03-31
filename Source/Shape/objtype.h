#ifndef _OBJTYPE_H
#define _OBJTYPE_H



/* -------------------------------------------------------------------- */
/*      Shape types (nSHPType)                                          */
/* -------------------------------------------------------------------- */
#define SHPT_NULL	0
#define SHPT_POINT	1
#define SHPT_ARC	3
#define SHPT_POLYGON	5
#define SHPT_MULTIPOINT	8
#pragma pack(1)
/* -------------------------------------------------------------------- */
/*      Minimum Bounding Rectangle                                      */
/* -------------------------------------------------------------------- */
typedef struct
{
	double	xmin;
	double	ymin;
	double	xmax;
	double	ymax;
}MBR;

/* -------------------------------------------------------------------- */
/*      Point Struct                                                    */
/* -------------------------------------------------------------------- */
typedef struct{
	double x;
	double y;
} GeoPoint;

/* -------------------------------------------------------------------- */
/*      PoliLine & Polygon                                              */
/* -------------------------------------------------------------------- */

typedef struct{
	MBR		m_MBR;
	int		m_nNumParts;
	int		m_nNumPoints;
	int*		m_pParts;
	GeoPoint*	m_pPoints;
}PolyObject;


/* -------------------------------------------------------------------- */
/*      Operate Setting                                                  */
/* -------------------------------------------------------------------- */
#define MAP_ZOOMIN 0
#define MAP_ZOOMOUT 1
#define MAP_MOVE 2
#define MAP_ZOOMAREA 3
#define MAP_PAN 4
#define MAP_IDENTIFY 5
#define MAP_SELECTBYPOINT 6
#define MAP_SELECTBYRECT 7
#define MAP_SELECTBYPOLYGON 8
#define MAP_DISTANCE 9
#define MAP_CALCULATEAREA 10


typedef struct Dbf_header3{
    unsigned char           version;
    unsigned char           year;
    unsigned char           month;
    unsigned char           day;
    unsigned long           recordn;
    unsigned short int      header_length; // 헤더 전체 크기
    unsigned short int      record_length;
    unsigned char           ver10_reserved[20];
} DBF_FILEHEADER;

typedef struct Field_descriptor32{        // 32 Byte Field Descriptor
    unsigned char           name[11];
    unsigned char           type;
    unsigned long           field_data_address;
    unsigned char           field_length;
    unsigned char           decimal_count;
    unsigned char           ver10_reserved[14];
} DBF_FILEDDESCRIPTOR;

#pragma pack(8)
#endif