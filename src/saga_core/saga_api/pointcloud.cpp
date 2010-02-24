
///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//           Application Programming Interface           //
//                                                       //
//                  Library: SAGA_API                    //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//                    pointcloud.cpp                     //
//                                                       //
//          Copyright (C) 2009 by Olaf Conrad            //
//                                                       //
//-------------------------------------------------------//
//                                                       //
// This file is part of 'SAGA - System for Automated     //
// Geoscientific Analyses'.                              //
//                                                       //
// This library is free software; you can redistribute   //
// it and/or modify it under the terms of the GNU Lesser //
// General Public License as published by the Free       //
// Software Foundation, version 2.1 of the License.      //
//                                                       //
// This library is distributed in the hope that it will  //
// be useful, but WITHOUT ANY WARRANTY; without even the //
// implied warranty of MERCHANTABILITY or FITNESS FOR A  //
// PARTICULAR PURPOSE. See the GNU Lesser General Public //
// License for more details.                             //
//                                                       //
// You should have received a copy of the GNU Lesser     //
// General Public License along with this program; if    //
// not, write to the Free Software Foundation, Inc.,     //
// 59 Temple Place - Suite 330, Boston, MA 02111-1307,   //
// USA.                                                  //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//    contact:    Olaf Conrad                            //
//                Institute of Geography                 //
//                University of Hamburg                  //
//                Germany                                //
//                                                       //
//    e-mail:     oconrad@saga-gis.org                   //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#include "pointcloud.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#define POINTCLOUD_FILE_VERSION	"SGPC01"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CSG_PointCloud * SG_Create_PointCloud(void)
{
	return( new CSG_PointCloud );
}

//---------------------------------------------------------
CSG_PointCloud * SG_Create_PointCloud(const CSG_PointCloud &PointCloud)
{
	return( new CSG_PointCloud(PointCloud) );
}

//---------------------------------------------------------
CSG_PointCloud * SG_Create_PointCloud(const CSG_String &File_Name)
{
	return( new CSG_PointCloud(File_Name) );
}

//---------------------------------------------------------
CSG_PointCloud * SG_Create_PointCloud(CSG_PointCloud *pStructure)
{
	return( new CSG_PointCloud(pStructure) );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CSG_PointCloud::CSG_PointCloud(void)
	: CSG_Data_Object()
{
	_On_Construction();
}

bool CSG_PointCloud::Create(void)
{
	Destroy();

	Add_Field(SG_T(""), SG_DATATYPE_Undefined);	// add x, y, z fields

	return( true );
}

//---------------------------------------------------------
CSG_PointCloud::CSG_PointCloud(const CSG_PointCloud &PointCloud)
	: CSG_Data_Object()
{
	_On_Construction();

	Create(PointCloud);
}

bool CSG_PointCloud::Create(const CSG_PointCloud &PointCloud)
{
	if( Assign((CSG_Data_Object *)&PointCloud) )
	{
		Set_Name(PointCloud.Get_Name());

		return( true );
	}

	return( false );
}

//---------------------------------------------------------
CSG_PointCloud::CSG_PointCloud(const CSG_String &File_Name)
	: CSG_Data_Object()
{
	_On_Construction();

	Create(File_Name);
}

bool CSG_PointCloud::Create(const CSG_String &File_Name)
{
	return( _Load(File_Name) );
}

//---------------------------------------------------------
CSG_PointCloud::CSG_PointCloud(CSG_PointCloud *pStructure)
	: CSG_Data_Object()
{
	_On_Construction();

	Create(pStructure);
}

bool CSG_PointCloud::Create(CSG_PointCloud *pStructure)
{
	Destroy();

	if( pStructure && pStructure->Get_Field_Count() > 0 )
	{
		for(int i=0; i<pStructure->Get_Field_Count(); i++)
		{
			_Add_Field(pStructure->Get_Field_Name(i), pStructure->Get_Field_Type(i));
		}

		return( true );
	}

	return( false );
}

//---------------------------------------------------------
void CSG_PointCloud::_On_Construction(void)
{
	m_nFields		= 0;
	m_Field_Name	= NULL;
	m_Field_Type	= NULL;
	m_Field_Stats	= NULL;
	m_Field_Offset	= NULL;

	m_Points		= NULL;
	m_nPoints		= 0;
	m_nBuffer		= 0;
	m_nPointBytes	= 0;

	m_Cursor		= NULL;
	m_bXYZPrecDbl	= true;
	m_NoData_Value	= -999999;

	Set_Update_Flag();
}

//---------------------------------------------------------
CSG_PointCloud::~CSG_PointCloud(void)
{
	Destroy();
}

bool CSG_PointCloud::Destroy(void)
{
	Del_Points();

	if( m_nFields > 0 )
	{
		for(int i=0; i<m_nFields; i++)
		{
			delete(m_Field_Name [i]);
			delete(m_Field_Stats[i]);
		}

		SG_Free(m_Field_Name);
		SG_Free(m_Field_Type);
		SG_Free(m_Field_Stats);
		SG_Free(m_Field_Offset);

		_On_Construction();
	}

	CSG_Data_Object::Destroy();

	return( true );
}


///////////////////////////////////////////////////////////
//														 //
//						File							 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CSG_PointCloud::_Load(const CSG_String &File_Name)
{
	TSG_Data_Type	Type;

	char		ID[6];
	int			i, iBuffer, nPointBytes, nFields;
	char		Name[1024];
	CSG_File	Stream;

	SG_UI_Msg_Add(CSG_String::Format(SG_T("%s: %s..."), LNG("[MSG] Load point cloud"), File_Name.c_str()), true);

	//-----------------------------------------------------
	if( !Stream.Open(File_Name, SG_FILE_R, true) )
	{
		SG_UI_Msg_Add(LNG("[MSG] failed"), false, SG_UI_MSG_STYLE_FAILURE);
		SG_UI_Msg_Add_Error(LNG("[ERR] file could not be opened."));

		return( false );
	}

	if( !Stream.Read(ID, 6) || strncmp(ID, POINTCLOUD_FILE_VERSION, 5) != 0 )
	{
		SG_UI_Msg_Add(LNG("[MSG] failed"), false, SG_UI_MSG_STYLE_FAILURE);
		SG_UI_Msg_Add_Error(LNG("[ERR] incompatible file."));

		return( false );
	}

	if( !Stream.Read(&nPointBytes, sizeof(int)) || nPointBytes < 3 * sizeof(float) )
	{
		SG_UI_Msg_Add(LNG("[MSG] failed"), false, SG_UI_MSG_STYLE_FAILURE);
		SG_UI_Msg_Add_Error(LNG("[ERR] incompatible file."));

		return( false );
	}

	if( !Stream.Read(&nFields, sizeof(int)) || nFields < 3 )
	{
		SG_UI_Msg_Add(LNG("[MSG] failed"), false, SG_UI_MSG_STYLE_FAILURE);
		SG_UI_Msg_Add_Error(LNG("[ERR] incompatible file."));

		return( false );
	}

	//-----------------------------------------------------
	Destroy();

	for(i=0; i<nFields; i++)
	{
		if( !Stream.Read(&Type		, sizeof(TSG_Data_Type))
		||	!Stream.Read(&iBuffer	, sizeof(int)) || !(iBuffer > 0 && iBuffer < 1024)
		||	!Stream.Read(Name		, iBuffer) )
		{
			SG_UI_Msg_Add(LNG("[MSG] failed"), false, SG_UI_MSG_STYLE_FAILURE);
			SG_UI_Msg_Add_Error(LNG("[ERR] incompatible file."));

			return( false );
		}

		if( ID[5] == '0' )	// Data Type Definition changed!!!
		{
			switch( Type )
			{
			default:	Type	= SG_DATATYPE_Undefined;	break;
			case 1:		Type	= SG_DATATYPE_Char;			break;
			case 2:		Type	= SG_DATATYPE_Short;		break;
			case 3:		Type	= SG_DATATYPE_Int;			break;
			case 4:		Type	= SG_DATATYPE_Long;			break;
			case 5:		Type	= SG_DATATYPE_Float;		break;
			case 6:		Type	= SG_DATATYPE_Double;		break;
			}
		}

		Name[iBuffer]	= '\0';

		if( !_Add_Field(CSG_String((const char *)Name), Type) )
		{
			SG_UI_Msg_Add(LNG("[MSG] failed"), false, SG_UI_MSG_STYLE_FAILURE);
			SG_UI_Msg_Add_Error(LNG("[ERR] incompatible file."));

			return( false );
		}
	}

	if( m_nPointBytes != nPointBytes )
	{
		SG_UI_Msg_Add(LNG("[MSG] failed"), false, SG_UI_MSG_STYLE_FAILURE);
		SG_UI_Msg_Add_Error(LNG("[ERR] incompatible file."));

		return( false );
	}

	//-----------------------------------------------------
	int		fLength	= Stream.Length();

	while( _Inc_Array() && Stream.Read(m_Cursor, m_nPointBytes) && SG_UI_Process_Set_Progress(Stream.Tell(), fLength) )
	{}

	_Dec_Array();

	Set_File_Name(File_Name);

	Load_MetaData(File_Name);

	if( m_nPoints <= 0 )
	{
		SG_UI_Msg_Add(LNG("[MSG] failed"), false, SG_UI_MSG_STYLE_FAILURE);
		SG_UI_Msg_Add_Error(LNG("[ERR] no records in file."));

		return( false );
	}

	SG_UI_Msg_Add(LNG("[MSG] okay"), false, SG_UI_MSG_STYLE_SUCCESS);

	return( true );
}

//---------------------------------------------------------
bool CSG_PointCloud::_Save(const CSG_String &File_Name)
{
	CSG_File	Stream;

	SG_UI_Msg_Add(CSG_String::Format(SG_T("%s: %s..."), LNG("[MSG] Save point cloud"), File_Name.c_str()), true);

	if( Stream.Open(File_Name, SG_FILE_W, true) == false )
	{
		SG_UI_Msg_Add(LNG("[MSG] failed"), false, SG_UI_MSG_STYLE_FAILURE);
		SG_UI_Msg_Add_Error(LNG("[ERR] unable to create file."));

		return( false );
	}

	int		i, iBuffer;

	Stream.Write((void *)POINTCLOUD_FILE_VERSION, 6);
	Stream.Write(&m_nPointBytes	, sizeof(int));
	Stream.Write(&m_nFields		, sizeof(int));

	for(i=0; i<m_nFields; i++)
	{
		Stream.Write(&m_Field_Type[i], sizeof(TSG_Data_Type));

		iBuffer	= m_Field_Name[i]->Length();	if( iBuffer >= 1024 - 1 )	iBuffer	= 1024 - 1;
		Stream.Write(&iBuffer, sizeof(int));
		Stream.Write((void *)m_Field_Name[i]->b_str(), sizeof(char), iBuffer);
	}

	for(i=0; i<m_nPoints && SG_UI_Process_Set_Progress(i, m_nPoints); i++)
	{
		Stream.Write(m_Points[i], m_nPointBytes);
	}

	Set_Modified(false);

	Set_File_Name(SG_File_Make_Path(NULL, File_Name, SG_T("spc")));

	Save_MetaData(File_Name);

	SG_UI_Msg_Add(LNG("[MSG] okay"), false, SG_UI_MSG_STYLE_SUCCESS);

	return( true );
}

bool CSG_PointCloud::Save(const CSG_String &File_Name, int Format)
{
	return( _Save(File_Name) );
}


///////////////////////////////////////////////////////////
//														 //
//						Assign							 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CSG_PointCloud::Assign(CSG_Data_Object *pObject)
{
	if( pObject && pObject->is_Valid() && pObject->Get_ObjectType() == Get_ObjectType() )
	{
		Destroy();

		CSG_PointCloud	*pPointCloud	= (CSG_PointCloud *)pObject;

		Get_History()	= pPointCloud->Get_History();

		for(int iField=0; iField<pPointCloud->m_nFields; iField++)
		{
			_Add_Field(pPointCloud->m_Field_Name[iField]->c_str(), pPointCloud->m_Field_Type[iField]);
		}

		for(int iPoint=0; iPoint<pPointCloud->m_nPoints; iPoint++)
		{
			if( _Inc_Array() )
			{
				memcpy(m_Points[iPoint], pPointCloud->m_Points[iPoint], m_nPointBytes);
			}
		}

		return( true );
	}

	return( false );
}


///////////////////////////////////////////////////////////
//														 //
//						Checks							 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CSG_PointCloud::is_Compatible(CSG_PointCloud *pPointCloud) const
{
	if( Get_Field_Count() == pPointCloud->Get_Field_Count() )
	{
		for(int i=0; i<Get_Field_Count(); i++)
		{
			if( Get_Field_Type(i) != pPointCloud->Get_Field_Type(i) )
			{
				return( false );
			}
		}

		return( true );
	}

	return( false );
}


///////////////////////////////////////////////////////////
//														 //
//						Fields							 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CSG_PointCloud::_Add_Field(const SG_Char *Name, TSG_Data_Type Type)
{
	if( SG_Data_Type_Get_Size(Type) <= 0 )
	{
		return( false );
	}

	m_Field_Name	= (CSG_String            **)SG_Realloc(m_Field_Name  , (m_nFields + 1) * sizeof(CSG_String *));
	m_Field_Type	= (TSG_Data_Type          *)SG_Realloc(m_Field_Type  , (m_nFields + 1) * sizeof(TSG_Data_Type));
	m_Field_Stats	= (CSG_Simple_Statistics **)SG_Realloc(m_Field_Stats , (m_nFields + 1) * sizeof(CSG_Simple_Statistics *));
	m_Field_Offset	= (int                    *)SG_Realloc(m_Field_Offset, (m_nFields + 1) * sizeof(int));

	m_Field_Name  [m_nFields]	= new CSG_String(Name);
	m_Field_Type  [m_nFields]	= Type;
	m_Field_Stats [m_nFields]	= new CSG_Simple_Statistics();
	m_Field_Offset[m_nFields]	= m_nFields == 0 ? 0 : m_Field_Offset[m_nFields - 1] + SG_Data_Type_Get_Size(m_Field_Type[m_nFields - 1]);

	m_nPointBytes	+= SG_Data_Type_Get_Size(m_Field_Type[m_nFields]);
	m_nFields		++;

	return( true );
}

//---------------------------------------------------------
bool CSG_PointCloud::Add_Field(const SG_Char *Name, TSG_Data_Type Type)
{
	if( m_nPoints == 0 )
	{
		if( m_nFields == 0 )
		{
			_Add_Field(SG_T("X"), m_bXYZPrecDbl ? SG_DATATYPE_Double : SG_DATATYPE_Float);
			_Add_Field(SG_T("Y"), m_bXYZPrecDbl ? SG_DATATYPE_Double : SG_DATATYPE_Float);
			_Add_Field(SG_T("Z"), m_bXYZPrecDbl ? SG_DATATYPE_Double : SG_DATATYPE_Float);
		}

		if( Name && SG_Data_Type_Get_Size(Type) > 0 )
		{
			return( _Add_Field(Name, Type) );
		}
	}

	return( false );
}

//---------------------------------------------------------
#ifdef _SAGA_UNICODE
bool CSG_PointCloud::Add_Field(const char *Name, TSG_Data_Type Type)
{	return( Add_Field(CSG_String(Name), Type) );	}
#endif

//---------------------------------------------------------
bool CSG_PointCloud::_Set_Field_Value(char *pPoint, int iField, double Value)
{
	if( pPoint && iField >= 0 && iField < m_nFields )
	{
		pPoint	= ((char *)pPoint) + m_Field_Offset[iField];

		switch( m_Field_Type[iField] )
		{
		default:
		case SG_DATATYPE_Undefined:	break;
		case SG_DATATYPE_Byte:		*((BYTE   *)pPoint)	= (BYTE  )Value;	break;
		case SG_DATATYPE_Char:		*((char   *)pPoint)	= (char  )Value;	break;
		case SG_DATATYPE_Word:		*((WORD   *)pPoint)	= (WORD  )Value;	break;
		case SG_DATATYPE_Short:		*((short  *)pPoint)	= (short )Value;	break;
		case SG_DATATYPE_DWord:		*((DWORD  *)pPoint)	= (DWORD )Value;	break;
		case SG_DATATYPE_Int:		*((int    *)pPoint)	= (int   )Value;	break;
		case SG_DATATYPE_Long:		*((long   *)pPoint)	= (long  )Value;	break;
		case SG_DATATYPE_Float:		*((float  *)pPoint)	= (float )Value;	break;
		case SG_DATATYPE_Double:	*((double *)pPoint)	= (double)Value;	break;
		}

		return( true );
	}

	return( false );
}

//---------------------------------------------------------
double CSG_PointCloud::_Get_Field_Value(char *pPoint, int iField) const
{
	if( pPoint && iField >= 0 && iField < m_nFields )
	{
		pPoint	= ((char *)pPoint) + m_Field_Offset[iField];

		switch( m_Field_Type[iField] )
		{
		case SG_DATATYPE_Undefined:		default:	break;
		case SG_DATATYPE_Byte:			return( *((BYTE   *)pPoint) );
		case SG_DATATYPE_Char:			return( *((char   *)pPoint) );
		case SG_DATATYPE_Word:			return( *((WORD   *)pPoint) );
		case SG_DATATYPE_Short:			return( *((short  *)pPoint) );
		case SG_DATATYPE_DWord:			return( *((DWORD  *)pPoint) );
		case SG_DATATYPE_Int:			return( *((int    *)pPoint) );
		case SG_DATATYPE_Long:			return( *((long   *)pPoint) );
		case SG_DATATYPE_Float:			return( *((float  *)pPoint) );
		case SG_DATATYPE_Double:		return( *((double *)pPoint) );
		}
	}

	return( 0.0 );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
TSG_Point_3D CSG_PointCloud::Get_Point(void)	const
{
	TSG_Point_3D	p;

	if( m_Cursor )
	{
		p.x	= _Get_Field_Value(m_Cursor, 0);
		p.y	= _Get_Field_Value(m_Cursor, 1);
		p.z	= _Get_Field_Value(m_Cursor, 2);
	}
	else
	{
		p.x	= p.y	= p.z	= 0.0;
	}

	return( p );
}

//---------------------------------------------------------
TSG_Point_3D CSG_PointCloud::Get_Point(int iPoint)	const
{
	TSG_Point_3D	p;

	if( iPoint >= 0 && iPoint < m_nPoints )
	{
		char	*pPoint	= m_Points[iPoint];

		p.x	= _Get_Field_Value(pPoint, 0);
		p.y	= _Get_Field_Value(pPoint, 1);
		p.z	= _Get_Field_Value(pPoint, 2);
	}
	else
	{
		p.x	= p.y	= p.z	= 0.0;
	}

	return( p );
}

//---------------------------------------------------------
bool CSG_PointCloud::Set_NoData_Value(double NoData_Value)
{
	if( NoData_Value != m_NoData_Value )
	{
		for(int i=3; i<m_nFields; i++)
		{
			m_Field_Stats[i]->Invalidate();
		}

		return( true );
	}

	return( false );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CSG_PointCloud::Add_Point(double x, double y, double z)
{
	if( _Inc_Array() )
	{
		_Set_Field_Value(m_Cursor, 0, x);
		_Set_Field_Value(m_Cursor, 1, y);
		_Set_Field_Value(m_Cursor, 2, z);

		Set_Modified();

		Set_Update_Flag();

		_Stats_Invalidate();

		return( true );
	}

	return( false );
}

//---------------------------------------------------------
bool CSG_PointCloud::Del_Point(int iPoint)
{
	if( iPoint >= 0 && iPoint < m_nPoints )
	{
		m_Cursor	= NULL;

		delete(m_Points[iPoint]);

		m_nPoints--;

		for(int i=iPoint, j=iPoint+1; i<m_nPoints; i++, j++)
		{
			m_Points[i]	= m_Points[i + 1];
		}

		_Dec_Array();

		Set_Modified();

		Set_Update_Flag();

		return( true );
	}

	return( false );
}

//---------------------------------------------------------
bool CSG_PointCloud::Del_Points(void)
{
	if( m_nBuffer > 0 )
	{
		for(int iPoint=0; iPoint<m_nPoints; iPoint++)
		{
			SG_Free(m_Points[iPoint]);
		}

		SG_Free(m_Points);
		m_Points	= NULL;
		m_nPoints	= 0;
		m_nBuffer	= 0;
		m_Cursor	= NULL;

		return( true );
	}

	return( false );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#define GET_GROW_SIZE(n)	(n < 256 ? 1 : (n < 8192 ? 128 : 1024))

//---------------------------------------------------------
bool CSG_PointCloud::_Inc_Array(void)
{
	if( m_nFields > 0 )
	{
		if( (m_nPoints + 1) >= m_nBuffer )
		{
			char	**pPoints	= (char **)SG_Realloc(m_Points, (m_nBuffer + GET_GROW_SIZE(m_nBuffer)) * sizeof(char *));

			if( pPoints )
			{
				m_Points	= pPoints;
				m_nBuffer	+= GET_GROW_SIZE(m_nBuffer);
			}
			else
			{
				return( false );
			}
		}

		m_Cursor	= m_Points[m_nPoints]	= (char *)SG_Calloc(m_nPointBytes, sizeof(char));
		m_nPoints	++;

		return( true );
	}

	return( false );
}

//---------------------------------------------------------
bool CSG_PointCloud::_Dec_Array(void)
{
	if( m_nPoints > 0 )
	{
		_Stats_Invalidate();

		m_Cursor	= NULL;
		m_nPoints	--;

		SG_Free(m_Points[m_nPoints]);

		if( (m_nPoints - 1) < m_nBuffer - GET_GROW_SIZE(m_nBuffer) )
		{
			char	**pPoints	= (char **)SG_Realloc(m_Points, (m_nBuffer - GET_GROW_SIZE(m_nBuffer)) * sizeof(char *));

			if( pPoints )
			{
				m_Points	= pPoints;
				m_nBuffer	-= GET_GROW_SIZE(m_nBuffer);
			}
		}
	}

	return( true );
}


///////////////////////////////////////////////////////////
//														 //
//						Statistics						 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
const CSG_Rect & CSG_PointCloud::Get_Extent(void)
{
	if( m_nFields >= 2 )
	{
		_Stats_Update(0);
		_Stats_Update(1);

		m_Extent.Assign(
			m_Field_Stats[0]->Get_Minimum(), m_Field_Stats[1]->Get_Minimum(),
			m_Field_Stats[0]->Get_Maximum(), m_Field_Stats[1]->Get_Maximum()
		);
	}

	return( m_Extent );
}

//---------------------------------------------------------
bool CSG_PointCloud::_Stats_Invalidate(void) const
{
	for(int iField=0; iField<m_nFields; iField++)
	{
		_Stats_Invalidate(iField);
	}

	return( true );
}

//---------------------------------------------------------
bool CSG_PointCloud::_Stats_Invalidate(int iField) const
{
	if( iField >= 0 && iField < m_nFields )
	{
		m_Field_Stats[iField]->Invalidate();

		return( true );
	}

	return( false );
}

//---------------------------------------------------------
bool CSG_PointCloud::_Stats_Update(int iField) const
{
	if( iField >= 0 && iField < m_nFields && m_nPoints > 0 )
	{
		if( !m_Field_Stats[iField]->is_Evaluated() )
		{
			char	**pPoint	= m_Points;

			for(int iPoint=0; iPoint<m_nPoints; iPoint++, pPoint++)
			{
				double	Value	= _Get_Field_Value(*pPoint, iField);

				if( iField < 3 || Value != m_NoData_Value )
				{
					m_Field_Stats[iField]->Add_Value(Value);
				}
			}
		}

		return( true );
	}

	return( false );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
