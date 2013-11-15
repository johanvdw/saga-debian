/**********************************************************
 * Version $Id: table_io.cpp 1728 2013-06-13 09:37:08Z oconrad $
 *********************************************************/

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
//                    table_io.cpp                       //
//                                                       //
//          Copyright (C) 2005 by Olaf Conrad            //
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
//                University of Goettingen               //
//                Goldschmidtstr. 5                      //
//                37077 Goettingen                       //
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
#include "table.h"
#include "table_dbase.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CSG_Table::_Load(const CSG_String &File_Name, TSG_Table_File_Type Format, const SG_Char *Separator)
{
	if( !SG_File_Exists(File_Name) )
	{
		return( false );
	}

	bool		bResult;
	CSG_String	fName, sSeparator(Separator && *Separator ? Separator : SG_T("\t"));

	Destroy();

	SG_UI_Msg_Add(CSG_String::Format(SG_T("%s: %s..."), _TL("Load table"), File_Name.c_str()), true);

	//-----------------------------------------------------
	if( Format == TABLE_FILETYPE_Undefined )
	{
		if( SG_File_Cmp_Extension(File_Name, SG_T("dbf")) )
		{
			Format	= TABLE_FILETYPE_DBase;
		}
		else if( SG_File_Cmp_Extension(File_Name, SG_T("csv")) )
		{
			Format	= TABLE_FILETYPE_Text;

			if( !Separator || *Separator == '\0' )
			{
				sSeparator	= SG_T(",");	// comma separated values
			}
		}
		else //if( SG_File_Cmp_Extension(File_Name, SG_T("txt")) )
		{
			Format	= TABLE_FILETYPE_Text;
		}
	}

	//-----------------------------------------------------
	switch( Format )
	{
	case TABLE_FILETYPE_Text:
		bResult	= _Load_Text (File_Name, true , sSeparator);
		break;

	case TABLE_FILETYPE_Text_NoHeadLine:
		bResult	= _Load_Text (File_Name, false, sSeparator);
		break;

	case TABLE_FILETYPE_DBase:
		bResult	= _Load_DBase(File_Name);
		break;

	default:
		bResult	= false;
	}

	//-----------------------------------------------------
	if( bResult )
	{
		Set_Modified(false);

		Set_Update_Flag();

		Set_File_Name(File_Name);

		Load_MetaData(File_Name);

		SG_UI_Msg_Add(_TL("okay"), false, SG_UI_MSG_STYLE_SUCCESS);

		return( true );
	}

	SG_UI_Msg_Add(_TL("failed"), false, SG_UI_MSG_STYLE_FAILURE);

	return( false );
}

//---------------------------------------------------------
bool CSG_Table::Save(const CSG_String &File_Name, int Format)
{
	return( Save(File_Name, Format, NULL) );
}

//---------------------------------------------------------
bool CSG_Table::Save(const CSG_String &File_Name, int Format, const SG_Char *Separator)
{
	bool		bResult;
	CSG_String	sSeparator(Separator && *Separator ? Separator : SG_T("\t"));

	SG_UI_Msg_Add(CSG_String::Format(SG_T("%s: %s..."), _TL("Save table"), File_Name.c_str()), true);

	//-----------------------------------------------------
	if( Format <= TABLE_FILETYPE_Undefined || Format > TABLE_FILETYPE_DBase )
	{
		if( SG_File_Cmp_Extension(File_Name, SG_T("dbf")) )
		{
			Format	= TABLE_FILETYPE_DBase;
		}
		else if( SG_File_Cmp_Extension(File_Name, SG_T("csv")) )
		{
			Format	= TABLE_FILETYPE_Text;

			if( Separator == NULL || *Separator == '\0' )
			{
				sSeparator	= SG_T(",");	// comma separated values
			}
		}
		else //if( SG_File_Cmp_Extension(File_Name, SG_T("txt")) )
		{
			Format	= TABLE_FILETYPE_Text;
		}
	}

	//-----------------------------------------------------
	switch( Format )
	{
	case TABLE_FILETYPE_Text:
		bResult	= _Save_Text (File_Name, true , sSeparator);
		break;

	case TABLE_FILETYPE_Text_NoHeadLine:
		bResult	= _Save_Text (File_Name, false, sSeparator);
		break;

	case TABLE_FILETYPE_DBase:
		bResult	= _Save_DBase(File_Name);
		break;
	}

	//-----------------------------------------------------
	if( bResult )
	{
		Set_Modified(false);

		Set_Update_Flag();

		Set_File_Type(Format);

		Set_File_Name(File_Name);

		Save_MetaData(File_Name);

		SG_UI_Msg_Add(_TL("okay"), false, SG_UI_MSG_STYLE_SUCCESS);

		return( true );
	}

	SG_UI_Msg_Add(_TL("failed"), false, SG_UI_MSG_STYLE_FAILURE);

	return( false );
}


///////////////////////////////////////////////////////////
//														 //
//						Text							 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CSG_Table::_Load_Text(const CSG_String &File_Name, bool bHeadline, const SG_Char *Separator)
{
	int			i, iField;
	long		fLength;
	CSG_String	sLine, sField;
	CSG_File	Stream;
	CSG_Table	Table;

	//-----------------------------------------------------
	if( Stream.Open(File_Name, SG_FILE_R, false) == false )
	{
		return( false );
	}

	if( (fLength = Stream.Length()) <= 0 )
	{
		return( false );
	}

	if( !Stream.Read_Line(sLine) )
	{
		return( false );
	}

	//-----------------------------------------------------
	sLine	+= Separator;

	while( (i = sLine.Find(Separator)) >= 0 )
	{
		sField.Clear();

		if( bHeadline )
		{
			sField	= sLine.Left(i);

			if( sField[0] == SG_T('\"') && sField[(int)(sField.Length() - 1)] == SG_T('\"') )	// remove quota
			{
				sField	= sField.AfterFirst('\"').BeforeLast('\"');
			}
		}

		if( sField.Length() == 0 )
		{
			sField.Printf(SG_T("F%02d"), Table.Get_Field_Count() + 1);
		}

		Table.Add_Field(sField, SG_DATATYPE_String);

		sLine.Remove(0, i + 1);
	}

	//-----------------------------------------------------
	TSG_Data_Type	*Type	= new TSG_Data_Type[Table.Get_Field_Count()];

	for(iField=0; iField<Table.Get_Field_Count(); iField++)
	{
		Type[iField]	= SG_DATATYPE_Int;
	}

	if( !bHeadline )
	{
		Stream.Seek_Start();
	}

	while( Stream.Read_Line(sLine) && sLine.Length() > 0 && SG_UI_Process_Set_Progress(Stream.Tell(), fLength) )
	{
		CSG_Table_Record	*pRecord	= Table.Add_Record();

		sLine	+= Separator;

		for(iField=0; iField<Table.Get_Field_Count(); iField++)
		{
			if( (i = sLine.Find(Separator)) >= 0 )
			{
				sField	= sLine.Left(i);

				if( sField[0] == SG_T('\"') && sField[(int)(sField.Length() - 1)] == SG_T('\"') )	// remove quota
				{
					sField	= sField.AfterFirst('\"').BeforeLast('\"');

					Type[iField]	= SG_DATATYPE_String;
				}

				if( Type[iField] != SG_DATATYPE_String && sField.Length() > 0 )
				{
					double	Value;

					if( SG_SSCANF(sField, SG_T("%lf"), &Value) != 1 )
					{
						Type[iField]	= SG_DATATYPE_String;
					}
					else if( Type[iField] != SG_DATATYPE_Double && Value - (int)Value != 0.0 )
					{
						Type[iField]	= SG_DATATYPE_Double;
					}
				}

				pRecord->Set_Value(iField, sField);

				sLine.Remove(0, i + 1);
			}
			else
			{
				break;
			}
		}
	}

	//-----------------------------------------------------
	if( Table.Get_Count() > 0 )
	{
		for(iField=0; iField<Table.Get_Field_Count(); iField++)
		{
			Add_Field(Table.Get_Field_Name(iField), Type[iField]);
		}

		for(int iRecord=0; iRecord<Table.Get_Count() && SG_UI_Process_Set_Progress(iRecord, Table.Get_Count()); iRecord++)
		{
			CSG_Table_Record	*pRecord	= Add_Record();

			for(iField=0; iField<Get_Field_Count(); iField++)
			{
				if( *Table[iRecord].asString(iField) )
				{
					switch( Get_Field_Type(iField) )
					{
					default:					pRecord->Set_Value(iField, Table[iRecord].asString(iField));	break;
					case SG_DATATYPE_Int:		pRecord->Set_Value(iField, Table[iRecord].asInt   (iField));	break;
					case SG_DATATYPE_Double:	pRecord->Set_Value(iField, Table[iRecord].asDouble(iField));	break;
					}
				}
				else
				{
					pRecord->Set_NoData(iField);
				}
			}
		}
	}

	delete[](Type);

	SG_UI_Process_Set_Ready();

	return( Get_Field_Count() > 0 );
}

//---------------------------------------------------------
bool CSG_Table::_Save_Text(const CSG_String &File_Name, bool bHeadline, const SG_Char *Separator)
{
	int			iField, iRecord;
	CSG_File	Stream;

	if( Get_Field_Count() > 0 )
	{
		if( Stream.Open(File_Name, SG_FILE_W, false) )
		{
			for(iField=0; iField<Get_Field_Count(); iField++)
			{
				Stream.Printf(SG_T("%s%s"), Get_Field_Name(iField), iField < Get_Field_Count() - 1 ? Separator : SG_T("\n"));
			}

			for(iRecord=0; iRecord<Get_Record_Count() && SG_UI_Process_Set_Progress(iRecord, Get_Record_Count()); iRecord++)
			{
				for(iField=0; iField<Get_Field_Count(); iField++)
				{
					if( !Get_Record(iRecord)->is_NoData(iField) )
					{
						switch( Get_Field_Type(iField) )
						{
						case SG_DATATYPE_String:
						case SG_DATATYPE_Date:
							Stream.Printf(SG_T("\"%s\""), Get_Record(iRecord)->asString(iField));
							break;

						default:
							Stream.Printf(SG_T("%s")    , Get_Record(iRecord)->asString(iField));
							break;
						}
					}

					Stream.Printf(SG_T("%s"), iField < Get_Field_Count() - 1 ? Separator : SG_T("\n"));
				}
			}

			SG_UI_Process_Set_Ready();

			return( true );
		}
	}

	return( false );
}


///////////////////////////////////////////////////////////
//														 //
//						DBase							 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CSG_Table::_Load_DBase(const CSG_String &File_Name)
{
	CSG_Table_DBase		dbf;

	return( dbf.Open_Read(File_Name, this) );
}

//---------------------------------------------------------
bool CSG_Table::_Save_DBase(const CSG_String &File_Name)
{
	CSG_Table_DBase		dbf;

	return( dbf.Open_Write(File_Name, this) );
}


///////////////////////////////////////////////////////////
//														 //
//						Serialize						 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CSG_Table::Serialize(CSG_File &Stream, bool bSave)
{
	SG_Char				Separator	= SG_T('\t');
	int					iField, nFields, iRecord, nRecords, FieldType;
	CSG_Table_Record	*pRecord;
	CSG_String			s, sLine;

	//-----------------------------------------------------
	if( bSave )
	{
		Stream.Printf(SG_T("%d %d\n"), m_nFields, m_nRecords);

		for(iField=0; iField<m_nFields; iField++)
		{
			Stream.Printf(SG_T("%d \"%s\"\n"), Get_Field_Type(iField), Get_Field_Name(iField));
		}

		for(iRecord=0; iRecord<m_nRecords; iRecord++)
		{
			for(iField=0; iField<m_nFields; iField++)
			{
				Stream.Printf(SG_T("%s%c"), Get_Record(iRecord)->asString(iField), iField < m_nFields - 1 ? Separator : '\n');
			}
		}

		return( true );
	}

	//-----------------------------------------------------
	else if( Stream.Read_Line(sLine) && SG_SSCANF(sLine, SG_T("%d %d"), &nFields, &nRecords) == 2 && nFields > 0 )
	{
		Destroy();

		for(iField=0; iField<nFields; iField++)
		{
			if( Stream.Read_Line(sLine) && SG_SSCANF(sLine, SG_T("%d"), &FieldType) == 1 )
			{
				Add_Field(sLine.AfterFirst(SG_T('\"')).BeforeFirst(SG_T('\"')), (TSG_Data_Type)FieldType);
			}
		}

		for(iRecord=0; iRecord<nRecords; iRecord++)
		{
			if( Stream.Read_Line(sLine) )
			{
				pRecord	= Add_Record();

				for(iField=0; iField<m_nFields; iField++)
				{
					pRecord->Set_Value(iField, sLine.BeforeFirst(Separator));

					sLine	= sLine.AfterFirst(Separator);
				}
			}
		}

		return( true );
	}

	//-----------------------------------------------------
	return( false );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
