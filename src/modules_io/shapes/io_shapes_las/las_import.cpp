/**********************************************************
 * Version $Id: las_import.cpp 1921 2014-01-09 10:24:11Z oconrad $
 *********************************************************/

///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                    Module Library:                    //
//                     Shapes_IO_LAS                     //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//                     las_import.cpp                    //
//                                                       //
//                 Copyright (C) 2009 by                 //
//              Olaf Conrad, Volker Wichmann             //
//                                                       //
//-------------------------------------------------------//
//                                                       //
// This file is part of 'SAGA - System for Automated     //
// Geoscientific Analyses'. SAGA is free software; you   //
// can redistribute it and/or modify it under the terms  //
// of the GNU General Public License as published by the //
// Free Software Foundation; version 2 of the License.   //
//                                                       //
// SAGA is distributed in the hope that it will be       //
// useful, but WITHOUT ANY WARRANTY; without even the    //
// implied warranty of MERCHANTABILITY or FITNESS FOR A  //
// PARTICULAR PURPOSE. See the GNU General Public        //
// License for more details.                             //
//                                                       //
// You should have received a copy of the GNU General    //
// Public License along with this program; if not,       //
// write to the Free Software Foundation, Inc.,          //
// 51 Franklin Street, 5th Floor, Boston, MA 02110-1301, //
// USA.                                                  //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//    e-mail:     oconrad@saga-gis.org                   //
//                                                       //
//    contact:    Olaf Conrad                            //
//                Institute of Geography                 //
//                University of Hamburg                  //
//                Germany                                //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#include "las_import.h"

#include <liblas/laspoint.hpp>
#include <liblas/lasreader.hpp>
#include <liblas/capi/las_version.h>
#include <fstream>
#include <iostream>


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
enum
{
	VAR_T	= 0,	// gps-time
	VAR_i,			// intensity
	VAR_a,			// scan angle
	VAR_r,			// number of the return
	VAR_c,			// classification
	VAR_u,			// user data
	VAR_n,			// number of returns of given pulse
	VAR_R,			// red channel color
	VAR_G,			// green channel color
	VAR_B,			// blue channel color
	VAR_e,			// edge of flight line flag
	VAR_d,			// direction of scan flag
	VAR_p,			// point source ID
	VAR_C,			// color
	VAR_Count
};

//---------------------------------------------------------
#define	ADD_FIELD(id, var, name, type)	if( Parameters(id)->asBool() ) { iField[var] = nFields++; pPoints->Add_Field(name, type); } else { iField[var] = -1; }


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CLAS_Import::CLAS_Import(void)
{
	CSG_Parameter	*pNode;

	//-----------------------------------------------------
	// 1. Info...

	Set_Name		(_TL("Import LAS Files"));

	Set_Author		(SG_T("O. Conrad, V. Wichmann (c) 2009"));

	CSG_String		Description(_TW(
		"This module imports ASPRS LAS files (versions 1.0, 1.1 and 1.2) as Point Clouds "
		"using the \"libLAS\" library. "
		"Get more information about this library at "
		"<a href=\"http://liblas.org\">http://liblas.org</a>\n"
		"\n"
		"Besides the x,y,z coordinates, the user can decide upon which attributes "
		"should be imported from the LAS file.\n\n"
		"Module development was supported by "
		"<a href=\"http://www.laserdata.at\">http://Laserdata GmbH, Austria</a>\n"
		"and "
		"<a href=\"http://www.alp-s.at\">http://alpS GmbH, Austria</a>\n"
		"\nliblas version: "
	));

	Description	+= CSG_String(LIBLAS_RELEASE_NAME);

	Set_Description	(Description);

	//-----------------------------------------------------
	// 2. Parameters...
	Parameters.Add_FilePath(
		NULL	, "FILES"		, _TL("Input Files"),
		_TL(""),
		_TL("LAS Files (*.las)|*.las|LAS Files (*.LAS)|*.LAS|All Files|*.*"),
		NULL, false, false, true
	);

	Parameters.Add_PointCloud_List(
		NULL	, "POINTS"		, _TL("Point Clouds"),
		_TL(""),
		PARAMETER_OUTPUT
	);

	pNode	= Parameters.Add_Node(
		NULL	, "NODE_VARS"	, _TL("Attributes to import besides x,y,z ..."),
		_TL("")
	);

	Parameters.Add_Value(pNode, "T", _TL("gps-time")						, _TL(""), PARAMETER_TYPE_Bool, false);
	Parameters.Add_Value(pNode, "i", _TL("intensity")						, _TL(""), PARAMETER_TYPE_Bool, false);
	Parameters.Add_Value(pNode, "a", _TL("scan angle")						, _TL(""), PARAMETER_TYPE_Bool, false);
	Parameters.Add_Value(pNode, "r", _TL("number of the return")			, _TL(""), PARAMETER_TYPE_Bool, false);
	Parameters.Add_Value(pNode, "c", _TL("classification")					, _TL(""), PARAMETER_TYPE_Bool, false);
	Parameters.Add_Value(pNode, "u", _TL("user data")						, _TL(""), PARAMETER_TYPE_Bool, false);
	Parameters.Add_Value(pNode, "n", _TL("number of returns of given pulse"), _TL(""), PARAMETER_TYPE_Bool, false);
	Parameters.Add_Value(pNode, "R", _TL("red channel color")				, _TL(""), PARAMETER_TYPE_Bool, false);
	Parameters.Add_Value(pNode, "G", _TL("green channel color")				, _TL(""), PARAMETER_TYPE_Bool, false);
	Parameters.Add_Value(pNode, "B", _TL("blue channel color")				, _TL(""), PARAMETER_TYPE_Bool, false);
	Parameters.Add_Value(pNode, "e", _TL("edge of flight line flag")		, _TL(""), PARAMETER_TYPE_Bool, false);
	Parameters.Add_Value(pNode, "d", _TL("direction of scan flag")			, _TL(""), PARAMETER_TYPE_Bool, false);
	Parameters.Add_Value(pNode, "p", _TL("point source ID")					, _TL(""), PARAMETER_TYPE_Bool, false);
	Parameters.Add_Value(pNode, "C", _TL("rgb color")						, _TL(""), PARAMETER_TYPE_Bool, false);

	Parameters.Add_Value(
		NULL	, "VALID"	, _TL("Check Point Validity"),
		_TL(""),
		PARAMETER_TYPE_Bool, false
	);

	Parameters.Add_Choice(
		NULL	, "RGB_RANGE", _TL("R,G,B value range"),
		_TL("Range of R,G,B values in LAS file."),
		CSG_String::Format(SG_T("%s|%s|"),
		_TL("16 bit"),
		_TL("8 bit")
		), 0
	);
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CLAS_Import::On_Execute(void)
{
	CSG_Parameter_PointCloud_List	*pPointsList;
	bool			bValidity;
	CSG_Strings		Files;
	int				RGBrange;
	int				cntInvalid = 0;


	bValidity		= Parameters("VALID")->asBool();
	RGBrange		= Parameters("RGB_RANGE")->asInt();
	
	//-----------------------------------------------------
	if( !Parameters("FILES")->asFilePath()->Get_FilePaths(Files) )
	{
		return( false );
	}

	//-----------------------------------------------------
	pPointsList	= Parameters("POINTS")->asPointCloudList();
	pPointsList	->Del_Items();

	for(int i=0; i<Files.Get_Count(); i++)
	{
		SG_UI_Msg_Add(CSG_String::Format(_TL("Parsing %s ... "), SG_File_Get_Name(Files[i], true).c_str()), true);

		std::ifstream   ifs;

		ifs.open(Files[i].b_str(), std::ios::in | std::ios::binary);
		if( !ifs )
		{
			SG_UI_Msg_Add_Error(CSG_String::Format(_TL("Unable to open LAS file!")));
			continue;
		}

		//-----------------------------------------------------
		// Check if LAS version is supported
		liblas::LASReader *pReader;
		try {
			pReader = new liblas::LASReader(ifs);
		}
		catch(std::exception &e) {
			SG_UI_Msg_Add_Error(CSG_String::Format(_TL("LAS header exception: %s"), e.what()));
			ifs.close();
			return( false );
		}
		catch(...) {
			SG_UI_Msg_Add_Error(CSG_String::Format(_TL("Unknown LAS header exception!")));
			ifs.close();
			return( false );
		}
	
		delete (pReader);
		ifs.clear();
		//-----------------------------------------------------


		liblas::LASReader reader(ifs);

		liblas::LASHeader const& header = reader.GetHeader();


		//-----------------------------------------------------
		int		nFields, iField[VAR_Count];

		CSG_PointCloud	*pPoints	= SG_Create_PointCloud();
		pPoints->Set_Name(SG_File_Get_Name(Files[i], false));

		nFields		= 3;

		ADD_FIELD("T", VAR_T, _TL("gps-time")							, SG_DATATYPE_Double);	// SG_DATATYPE_Long
		ADD_FIELD("i", VAR_i, _TL("intensity")							, SG_DATATYPE_Float);	// SG_DATATYPE_Word
		ADD_FIELD("a", VAR_a, _TL("scan angle")							, SG_DATATYPE_Float);	// SG_DATATYPE_Byte
		ADD_FIELD("r", VAR_r, _TL("number of the return")				, SG_DATATYPE_Int);
		ADD_FIELD("c", VAR_c, _TL("classification")						, SG_DATATYPE_Int);		// SG_DATATYPE_Byte
		ADD_FIELD("u", VAR_u, _TL("user data")							, SG_DATATYPE_Double);	// SG_DATATYPE_Byte
		ADD_FIELD("n", VAR_n, _TL("number of returns of given pulse")	, SG_DATATYPE_Int);
		ADD_FIELD("R", VAR_R, _TL("red channel color")					, SG_DATATYPE_Int);		// SG_DATATYPE_Word
		ADD_FIELD("G", VAR_G, _TL("green channel color")				, SG_DATATYPE_Int);
		ADD_FIELD("B", VAR_B, _TL("blue channel color")					, SG_DATATYPE_Int);
		ADD_FIELD("e", VAR_e, _TL("edge of flight line flag")			, SG_DATATYPE_Char);
		ADD_FIELD("d", VAR_d, _TL("direction of scan flag")				, SG_DATATYPE_Char);
		ADD_FIELD("p", VAR_p, _TL("point source ID")					, SG_DATATYPE_Int);		// SG_DATATYPE_Word
		ADD_FIELD("C", VAR_C, _TL("rgb color")							, SG_DATATYPE_Int);

		//-----------------------------------------------------
		int		iPoint	= 0;

		try {
			while( reader.ReadNextPoint() )
			{
				if (iPoint % 100000)
					SG_UI_Process_Set_Progress(iPoint, header.GetPointRecordsCount()); 

				liblas::LASPoint const& point = reader.GetPoint();

				if( bValidity )
				{
					if( !point.IsValid() )
					{
						cntInvalid++;
						continue;
					}
				}

				pPoints->Add_Point(point.GetX(), point.GetY(), point.GetZ());

				if( iField[VAR_T] > 0 )	pPoints->Set_Value(iPoint, iField[VAR_T], point.GetTime());
				if( iField[VAR_i] > 0 )	pPoints->Set_Value(iPoint, iField[VAR_i], point.GetIntensity());
				if( iField[VAR_a] > 0 )	pPoints->Set_Value(iPoint, iField[VAR_a], point.GetScanAngleRank());
				if( iField[VAR_r] > 0 )	pPoints->Set_Value(iPoint, iField[VAR_r], point.GetReturnNumber());
				if( iField[VAR_c] > 0 )	pPoints->Set_Value(iPoint, iField[VAR_c], point.GetClassification());
				if( iField[VAR_u] > 0 )	pPoints->Set_Value(iPoint, iField[VAR_u], point.GetUserData());
				if( iField[VAR_n] > 0 )	pPoints->Set_Value(iPoint, iField[VAR_n], point.GetNumberOfReturns());
				if( iField[VAR_R] > 0 )	pPoints->Set_Value(iPoint, iField[VAR_R], point.GetColor().GetRed());
				if( iField[VAR_G] > 0 )	pPoints->Set_Value(iPoint, iField[VAR_G], point.GetColor().GetGreen());
				if( iField[VAR_B] > 0 )	pPoints->Set_Value(iPoint, iField[VAR_B], point.GetColor().GetBlue());
				if( iField[VAR_e] > 0 )	pPoints->Set_Value(iPoint, iField[VAR_e], point.GetFlightLineEdge());
				if( iField[VAR_d] > 0 )	pPoints->Set_Value(iPoint, iField[VAR_d], point.GetScanDirection());
				if( iField[VAR_p] > 0 )	pPoints->Set_Value(iPoint, iField[VAR_p], point.GetPointSourceID());
				if( iField[VAR_C] > 0 )
				{
					double	r, g, b;
					r = point.GetColor().GetRed();
					g = point.GetColor().GetGreen();
					b = point.GetColor().GetBlue();

					if (RGBrange == 0)		// 16 bit
					{
						r = r / 65535 * 255;
						g = g / 65535 * 255;
						b = b / 65535 * 255;
					}
			
					pPoints->Set_Value(iPoint, iField[VAR_C], SG_GET_RGB(r, g, b));
				}

				iPoint++;
			}
		}
		catch(std::exception &e) {
			SG_UI_Msg_Add_Error(CSG_String::Format(_TL("LAS reader exception: %s"), e.what()));
			ifs.close();
			return( false );
		}
		catch(...) {
			SG_UI_Msg_Add_Error(CSG_String::Format(_TL("Unknown LAS reader exception!")));
			ifs.close();
			return( false );
		}

		ifs.close();

		pPointsList->Add_Item(pPoints);

		DataObject_Add(pPoints);

		//-----------------------------------------------------
		CSG_Parameters		sParms;

		DataObject_Get_Parameters(pPoints, sParms);

		if (sParms("METRIC_ATTRIB")	&& sParms("COLORS_TYPE") && sParms("METRIC_COLORS")
			&& sParms("METRIC_ZRANGE") && sParms("DISPLAY_VALUE_AGGREGATE"))
		{
			sParms("DISPLAY_VALUE_AGGREGATE")->Set_Value(3);		// highest z
			sParms("COLORS_TYPE")->Set_Value(2);                    // graduated color
			sParms("METRIC_COLORS")->asColors()->Set_Count(255);    // number of colors
			sParms("METRIC_ATTRIB")->Set_Value(2);					// z attrib
			sParms("METRIC_ZRANGE")->asRange()->Set_Range(pPoints->Get_Minimum(2),pPoints->Get_Maximum(2));
		}

		DataObject_Set_Parameters(pPoints, sParms);

		SG_UI_Msg_Add(_TL("okay"), false);
	}

	//-----------------------------------------------------
	if( bValidity && cntInvalid > 0 )
		SG_UI_Msg_Add(CSG_String::Format(_TL("WARNING: %d invalid points skipped!"), cntInvalid), true);

	return( true );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
