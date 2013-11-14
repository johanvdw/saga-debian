/**********************************************************
 * Version $Id: pointcloud_to_text_file.cpp 1500 2012-10-25 09:42:45Z reklov_w $
 *********************************************************/

///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                    Module Library:                    //
//                       Shapes_IO                       //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//              pointcloud_to_text_file.cpp              //
//                                                       //
//                 Copyright (C) 2011 by                 //
//                    Volker Wichmann                    //
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
// 59 Temple Place - Suite 330, Boston, MA 02111-1307,   //
// USA.                                                  //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//    e-mail:     wichmann@laserdata.at                  //
//                                                       //
//    contact:    LASERDATA GmbH                         //
//                Management and Analysis of             //
//                Laserscanning Data                     //
//                Technikerstr. 21a                      //
//                6020 Innsbruck                         //
//                Austria                                //
//                www.laserdata.at                       //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#include "pointcloud_to_text_file.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CPointcloud_To_Text_File::CPointcloud_To_Text_File(void)
{
	//-----------------------------------------------------
	Set_Name		(_TL("Export Point Cloud to Text File"));

	Set_Author		(SG_T("V. Wichmann, LASERDATA GmbH (c) 2011"));

	Set_Description	(_TW(
					"Exports a point cloud to a text file. Once the module is executed, "
					"a pop-up dialog allows to specify the fields to be exported and their "
					"decimal precision.\n\n"
					"Module usage is different between SAGA GUI and SAGA CMD: With "
					"SAGA GUI you will get prompted to choose the fields to export "
					"and the decimal precisions to use "
					"once you execute the module. With SAGA CMD you have to provide "
					"two strings with the -FIELDS and -PRECISIONS parameters. The first one "
					"must contain the field numbers, the latter the precisions "
					"(separated by semicolon). Field numbers start with 1, e.g. "
					"-FIELDS=\"1;2;3;5\" -PRECISIONS=\"2;2;2;0\".\n\n"
	));


	//-----------------------------------------------------
	Parameters.Add_PointCloud(
		NULL	, "POINTS"		, _TL("Point Cloud"),
		_TL("The point cloud to export."),
		PARAMETER_INPUT
	);

	Parameters.Add_FilePath(
		NULL	, "FILE"	, _TL("Text File"),
		_TL("The file to write the point cloud to."),
		CSG_String::Format(SG_T("%s|%s|%s|%s|%s|%s"),
			_TL("Text Files (*.txt)")	, SG_T("*.txt"),
			_TL("CSV Files (*.csv)")	, SG_T("*.csv"),
			_TL("All Files")			, SG_T("*.*")
		), NULL, true
	);

	Parameters.Add_Value(
        NULL	, "WRITE_HEADER" , _TL("Write Header"),
        _TL("Write column names."),
        PARAMETER_TYPE_Bool, false
    );

    Parameters.Add_Choice(
        NULL	, "FIELDSEP" , _TL("Field Separator"),
        _TL("Field Separator"),
        CSG_String::Format(SG_T("%s|%s|%s|"),
            _TL("tabulator"),
            _TL("space"),
            _TL("comma")
        ), 0
    );

	if (!SG_UI_Get_Window_Main())
	{
		Parameters.Add_String(
            NULL	, "FIELDS"    , _TL("Fields"),
            _TL("The numbers (starting from 1) of the fields to export, separated by semicolon, e.g. \"1;2;3;5\""),
            SG_T("")
        );

		Parameters.Add_String(
            NULL	, "PRECISIONS"    , _TL("Precisions"),
            _TL("The decimal precision to use for each field, separated by semicolon, e.g. \"2;2;2;0\""),
            SG_T("")
        );
	}
}

//---------------------------------------------------------
CPointcloud_To_Text_File::~CPointcloud_To_Text_File(void)
{}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CPointcloud_To_Text_File::On_Execute(void)
{
	CSG_PointCloud			*pPoints;
	CSG_String				fileName;
	CSG_File				*pTabStream = NULL;
	bool					bWriteHeader;
	CSG_String				fieldSep;

	CSG_Parameters			P;
	CSG_Parameter			*pNode;
	CSG_String				s;
	std::vector<int>		vCol, vPrecision;


	//-----------------------------------------------------
	pPoints			= Parameters("POINTS")		->asPointCloud();
	fileName		= Parameters("FILE")		->asString();
	bWriteHeader	= Parameters("WRITE_HEADER")->asBool();


	switch (Parameters("FIELDSEP")->asInt())
    {
	default:
    case 0:		fieldSep = "\t";	break;
    case 1:		fieldSep = " ";		break;
    case 2:		fieldSep = ",";		break;
    }


	if( fileName.Length() == 0 )
	{
		SG_UI_Msg_Add_Error(_TL("Please provide an output file name!"));
		return( false );
	}


	if (SG_UI_Get_Window_Main())
	{
		P.Set_Name(_TL("Check the fields to export"));

		for(int iField=0; iField<pPoints->Get_Field_Count(); iField++)
		{
			s.Printf(SG_T("NODE_%03d") , iField + 1);
			pNode	= P.Add_Node(NULL, s, CSG_String::Format(_TL("%d. %s"), iField + 1, _TL("Field")), _TL(""));

			s.Printf(SG_T("FIELD_%03d"), iField);
			P.Add_Value(pNode, s, CSG_String::Format(SG_T("%s"), pPoints->Get_Field_Name(iField)), _TL(""), PARAMETER_TYPE_Bool, false);

			s.Printf(SG_T("PRECISION_%03d"), iField);
			P.Add_Value(pNode, s, _TL("Decimal Precision"), _TL(""), PARAMETER_TYPE_Int, 2.0, 0.0, true);
		}


		//-----------------------------------------------------
		if( Dlg_Parameters(&P, _TL("Field Properties")) )
		{
			vCol.clear();
			vPrecision.clear();

			for(int iField=0; iField<pPoints->Get_Field_Count(); iField++)
			{
				if( P(CSG_String::Format(SG_T("FIELD_%03d" ), iField).c_str())->asBool() )
				{
					vCol.push_back(iField);
					vPrecision.push_back(P(CSG_String::Format(SG_T("PRECISION_%03d" ), iField).c_str())->asInt());
				}
			}
		}
		else
			return( false );
	}
	else // CMD
	{
		CSG_String		sFields, sPrecision;
		CSG_String		token;
		int				iValue;


		sFields		= Parameters("FIELDS")->asString();
		sPrecision	= Parameters("PRECISIONS")->asString();

		wxStringTokenizer   tkz_fields(sFields.c_str(), wxT(";"), wxTOKEN_STRTOK);

		while( tkz_fields.HasMoreTokens() )
		{
			token	= tkz_fields.GetNextToken().wc_str();

			if( token.Length() == 0 )
				break;

			if( !token.asInt(iValue) )
			{
				SG_UI_Msg_Add_Error(_TL("Error parsing attribute fields: can't convert to number"));
				return( false );
			}

			iValue	-= 1;

			if( iValue < 0 || iValue > pPoints->Get_Field_Count() - 1 )
			{
				SG_UI_Msg_Add_Error(_TL("Error parsing attribute fields: field index out of range"));
				return( false );
			}
			else
				vCol.push_back(iValue);
		}

		wxStringTokenizer   tkz_precisons(sPrecision.c_str(), wxT(";"), wxTOKEN_STRTOK);

		while( tkz_precisons.HasMoreTokens() )
		{
			token	= tkz_precisons.GetNextToken().wc_str();

			if( token.Length() == 0 )
				break;

			if( !token.asInt(iValue) )
			{
				SG_UI_Msg_Add_Error(_TL("Error parsing attribute fields: can't convert to number"));
				return( false );
			}

			vPrecision.push_back(iValue);
		}
	}


	if( vCol.size() == 0 )
	{
		SG_UI_Msg_Add_Error(_TL("Please provide at least one column to export!"));
		return( false );
	}

	if( vCol.size() != vPrecision.size() )
	{
		SG_UI_Msg_Add_Error(_TL("Number of fields and precisions must be equal!"));
		return( false );
	}

	//-----------------------------------------------------
	pTabStream = new CSG_File();

	if( !pTabStream->Open(fileName, SG_FILE_W, false) )
	{
		SG_UI_Msg_Add_Error(CSG_String::Format(_TL("Unable to open output file %s!"), fileName.c_str()));
		delete (pTabStream);
		return (false);
	}


	if( bWriteHeader )
	{
	    CSG_String  sHeader;

		for(size_t i=0; i<vCol.size(); i++)
		{
		    sHeader += CSG_String::Format(SG_T("%s"), pPoints->Get_Field_Name(vCol.at(i)));

		    if( i < vCol.size()-1 )
                sHeader += fieldSep.c_str();
		}

		sHeader += SG_T("\n");

		pTabStream->Write(sHeader);
	}


	for(int iPoint=0; iPoint<pPoints->Get_Count() && Set_Progress(iPoint, pPoints->Get_Count()); iPoint++)
	{
		CSG_String	sLine;

		for(size_t i=0; i<vCol.size(); i++)
		{
			switch (pPoints->Get_Field_Type(vCol.at(i)))
			{
			case SG_DATATYPE_Double:
			case SG_DATATYPE_Float:
				sLine += SG_Get_String(pPoints->Get_Value(iPoint, vCol.at(i)), vPrecision.at(i), false);
				break;
			default:
				sLine += CSG_String::Format(SG_T("%d"), (int)pPoints->Get_Value(iPoint, vCol.at(i)));
				break;
			}

            if( i < vCol.size()-1 )
                sLine += fieldSep.c_str();
		}

		sLine += SG_T("\n");

		pTabStream->Write(sLine);
	}


	pTabStream->Close();
	delete (pTabStream);

	return( true );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
