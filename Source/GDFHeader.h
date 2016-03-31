// MyHeader.h : user define header file

#ifndef MYHEADER_H
#define MYHEADER_H

#define MAX_NAME_SIZE	140
typedef unsigned long	GID;
typedef unsigned long	RID;
typedef short TOPOLOGY_TYPE;
typedef short COORDINATE_TYPE;

#define SIZE_OF_SPATIAL 1024
#define MAX_LAYER_LIST	20


enum tagOption
{
	ZOOMIN,
	ZOOMOUT,
	MOVE,
	AREA,
	EXTENT
};

enum tagOpenFile
{
	F_SUCCESS,
	F_MAXLAYER,
	F_REOPEN
};


// GDF Header
typedef struct tagGdfHeader {
	char			Identifier[12];
	long			Version;
	char			GeoName[MAX_NAME_SIZE];
	char			LayerName[MAX_NAME_SIZE];
	unsigned short	TagTableType;
	short			FieldCnt;
	char			AspatialDatabase[372];
	long			CompressTag;
	char			reserved1[348];
} GDF_HDL;

// Link Header
typedef struct tagLinkHeader{
	long           AspatialFieldType;
	long           AspatialFieldSize;
	long           GmtCnt;
	long           GmtRefCnt;
	long           NodeCnt;
} LINK_HDL;

// Link Record
typedef struct tagGeoRecord {
	short  Tag;
	short  wordalignment;
	GID    SpatialID;
	RID    AspatialID;
} LINK_RECORD;

// Link Node Record
typedef struct tagGeoNodeRecord {
	short  Tag;
	short  wordalignment;
	GID    NodeID;
	RID    AspatialID;
} LINK_N_RECORD;

// Line Attribute
typedef struct tagLineAttr {
    unsigned short  ColorIdx;
    unsigned short  Style;
    unsigned short  Width;
} LINE_ATTR;

// Fill Attribute
typedef struct tagFillAttr {
    unsigned short  FColorIdx;
    unsigned short  BColorIdx;
    unsigned short  Pattern;
} FILL_ATTR;

// Poly Attribute
typedef struct tagPolyAttr {
    LINE_ATTR 	Line;
    FILL_ATTR 	Fill;
} POLY_ATTR;

/* 2D Coordinate */
typedef struct tagCoord {
    double  X;
    double  Y;
} COORD2;

// Bound Rect
typedef struct tagBoundRect {
	double  MinX, MinY;
	double  MaxX, MaxY;
} BOUND;

// Spatial Header
typedef struct tagSpatialHeader {
	TOPOLOGY_TYPE		TopologyType;
	COORDINATE_TYPE     CoordinateType;
	LINE_ATTR			LineAttr;
	POLY_ATTR			PolyAttr;
	short				alignment1;
	GID 				BoundaryID;
	long				alignment2;	
	COORD2				LeftUpPos, RightDownPos;
	char				reserved[960];
//	GMT_PUREPOLYGON 	*Boundary;	 /* build after reading */
//	short				VecHdl; 	 /* build after reading */
//	void				*RasterHdl;  /* build after reading */
} SPATIAL_HDL;		//포인터로??why?

// Page Allocation Header
typedef struct tagPatHeader {
	long TotalNum;
	long BaseSize;
	long AllocNum;
	long FreeNum;
} PAT_HEADER;

// Page Allocation Table
typedef struct tagPageMap {
	long	Size;				 /* Size * PatHeader.BaseSize = n bytes  */
	long	Level;				 /* Page Header 						  */
	long	FreeSize;			 /* free bytes							  */
	long	Offset; 			 /* Offset * PatHeader.BaseSize = offset bytes */
							 	 /* 실제 디스크에 들어가는 Offset */
} PAT;		   

// Page Header
typedef struct tagPageHeader{
	long Size;
	long TotalNum;
	long FreeNum;
	long FreeSize;
	long FreeOffset;
} PAGE_HEADER;

// Page Offset
typedef struct tagPageOffset{
	long Size;
	long FreeOffset;
} PAGE_OFFSET;

// GMT_OBJECT
typedef struct tagGmtData {
	short Type;
	BOUND MBR;
} GMT_OBJECT;

/* Object Type Const. */
#define GMT_TYPE_ANNO_POINT 	  0x21 // 33
#define GMT_TYPE_SYM_POINT		  0x22 // 34
#define GMT_TYPE_PURECHAIN		  0x31 // 49
#define GMT_TYPE_ARC			  0x36 // 54
#define GMT_TYPE_PUREPOLYGON	  0x41 // 65
#define GMT_TYPE_MULTI_POLYGON	  0x44 // 68
#define GMT_TYPE_HPOLYGON		  0x45 // 69
#define GMT_TYPE_ELLIPSE		  0x47 // 71

// GMT_ANNO_POINT

typedef struct tagAnnotePoint {
	short	  Type;
	BOUND	  MBR;
	COORD2	  Pos;
	double	   Size;
	double	   Width;
	double	   Angle;
	unsigned short	  ColorIdx;
	char	  FaceName[LF_FACESIZE];
	short	  AnnoteSize;
	char	  *Annote;
} GMT_ANNO_POINT;

/*typedef struct tagAnnotePoint {
	short	  Type;
	BOUND	  MBR;
	COORD2	  Pos;
	float	   Size;
	float	   Width;
	float	   Angle;
	short	  ColorIdx;
	char	  temp[12]; // 길이를 때려 맞추기위함..
	char	  FaceName[LF_FACESIZE];
	short	  AnnoteSize;
	char	  *Annote;
} GMT_ANNO_POINT;
*/
// GMT_SYM_POINT
typedef struct tagSymbolPoint {
	short	  Type;
	BOUND	  MBR;
	COORD2	  Pos;
	short     Tag;                         // if Tag == TRUE fixed_size, else variable_size
	long      Idx;
	double	  Size;
	double	  Angle;
	unsigned short     ColorIdx;
} GMT_SYM_POINT;

// GMT_PURECHAIN
typedef struct tagPureChain {
	short	   Type;
	BOUND	   MBR;
	short	   VtxCnt;
	COORD2	   *VtxList;
	LINE_ATTR  *Attr;
} GMT_PURECHAIN;

// Pure Polygon
typedef struct tagPurePolygon {
	short	  Type;
	BOUND	  MBR;
	short	  VtxCnt;
	COORD2    *VtxList;
	POLY_ATTR *Attr;
} GMT_PUREPOLYGON;

// Ellipse
typedef struct tagEllipse{
	short			Type;
	BOUND			MBR;
	COORD2			P1, P2;		/* p1 is left-top, p2 is right-bottom */
	POLY_ATTR		*Attr;
} GMT_ELLIPSE;

// Arc
typedef struct tagArc{
	short	  Type;
	BOUND	  MBR;
	COORD2	  P1, P2;		/* p1 is left-top, p2 is right-bottom */
	COORD2	  Start, End;
	LINE_ATTR *Attr;
} GMT_ARC;

// Multi Polygon
typedef struct tagMultiPolygon {
	short			Type;
	BOUND			MBR;
	unsigned short	PolyCnt;
	GMT_OBJECT		**Polygons;
	POLY_ATTR		*Attr;
} GMT_MULTI_POLYGON;

// 메모리 구조체
typedef struct tagMemAnno{
	double nSize;
	double nAngle;
	double x,y;
	CString strText;
} MEM_ANNO;

typedef struct tagMemPolygon{
	CPoint *VtxList;
	short VtxCnt;
} MEM_POLYGON;

typedef struct tagMemPolyline{
	CPoint *VtxList;
	short VtxCnt;
} MEM_POLYLINE;

typedef struct tagMemEllipse{
	CPoint P1,P2;
} MEM_ELLIPSE;

typedef struct tagMemArc{
	CPoint P1,P2;
	CPoint Start,End;
} MEM_ARC;

typedef struct tagMemNode{
	short Type;
	int Layer_num;
	MEM_ANNO *pAnno;
	MEM_POLYGON *pPoly;	
	MEM_POLYLINE *pLine;
	MEM_ELLIPSE *pEll;
	MEM_ARC *pArc;
	struct tagMemNode *next;
} MEM_NODE;

typedef struct tagHead{
	int num;
	MEM_NODE *pHead;
	MEM_NODE *pTail;
} HEAD_NODE;

typedef struct twoshort{
	short offsetNum;
	short pageNum;
}TWO_SHORT;



#endif