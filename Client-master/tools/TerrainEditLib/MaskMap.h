#pragma once

namespace TE
{

#pragma pack(push,1)

	/*
	struct MNavHead
	{
	char    magic_number[4];
	int      version;
	int             waypoint_data_offset;
	int             num_waypoint;
	int             num_connect;

	int             grid_dat_offset;
	int             ngridx;
	int             ngridz;
	int             grid_num_bits;
	int             start_x;
	int             start_z;
	float           grid_size_x;
	float           grid_size_z;
	};

	struct MNavData
	{
	int gx,gy;
	unsigned char *pData;
	int wayPointNum;
	unsigned char *wayData;
	int wayLinkNum;
	unsigned char *linkInfo;
	};

	struct MWayPoint
	{
	int  x,z;
	unsigned char cPointNum;
	};
	*/

	struct MaskMapHeader
	{
		char magic_number[4];
		int version;
		int headsize;
		int	filesize;
		int ngridx;
		int ngridz;
		int gridsizex;
		int gridsizez;
	};

	struct MaskMapData
	{
		unsigned char *pdata;
	};

#pragma pack(pop)

	class EditorMaskMap
	{
	public:
		EditorMaskMap(void);
		~EditorMaskMap(void);

		bool   SaveNavFile(const char* filename);
		bool   SaveBmpFile( const char* filename, int layer);
		bool	LoadBmpFile( const char* filename);

		MaskMapHeader*   GetHeadData(){return &m_Head;}
		MaskMapData*   GetNavData(){return &m_NavData;}

		void    Release();
	private:
		MaskMapHeader     m_Head;
		MaskMapData     m_NavData;
	};

	extern EditorMaskMap  g_NavMap;
}