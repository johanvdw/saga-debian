/**********************************************************
 * Version $Id: api_translator.cpp 1921 2014-01-09 10:24:11Z oconrad $
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
//                  api_translator.cpp                   //
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
// 51 Franklin Street, 5th Floor, Boston, MA 02110-1301, //
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
#include "api_core.h"

#include "table.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CSG_Translator		gSG_Translator;

//---------------------------------------------------------
CSG_Translator &	SG_Get_Translator(void)
{
	return( gSG_Translator );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
const SG_Char *		SG_Translate(const CSG_String &Text)
{
	return( gSG_Translator.Get_Translation(Text) );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CSG_Translator::CSG_Translator(void)
{
	m_nTranslations	= 0;
	m_Translations	= NULL;
}

CSG_Translator::CSG_Translator(const CSG_String &File_Name, bool bSetExtension, int iText, int iTranslation, bool bCmpNoCase)
{
	m_nTranslations	= 0;
	m_Translations	= NULL;

	Create(File_Name, bSetExtension, iText, iTranslation, bCmpNoCase);
}

CSG_Translator::CSG_Translator(class CSG_Table *pTable, int iText, int iTranslation, bool bCmpNoCase)
{
	m_nTranslations	= 0;
	m_Translations	= NULL;

	Create(pTable, iText, iTranslation, bCmpNoCase);
}

//---------------------------------------------------------
CSG_Translator::~CSG_Translator(void)
{
	Destroy();
}

//---------------------------------------------------------
void CSG_Translator::Destroy(void)
{
	if( m_Translations )
	{
		for(int i=0; i<m_nTranslations; i++)
		{
			delete(m_Translations[i]);
		}

		SG_Free(m_Translations);

		m_nTranslations	= 0;
		m_Translations	= NULL;
	}
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CSG_Translator::Create(const CSG_String &File_Name, bool bSetExtension, int iText, int iTranslation, bool bCmpNoCase)
{
	CSG_Table	Translations;
	CSG_String	fName(bSetExtension ? SG_File_Make_Path(NULL, File_Name, SG_T("lng")) : File_Name);

	SG_UI_Msg_Lock(true);

	Destroy();

	if( SG_File_Exists(fName) && Translations.Create(fName) )
	{
		Create(&Translations, iText, iTranslation, bCmpNoCase);
	}

	SG_UI_Msg_Lock(false);

	return( m_nTranslations > 0 );
}

//---------------------------------------------------------
bool CSG_Translator::Create(class CSG_Table *pTranslations, int iText, int iTranslation, bool bCmpNoCase)
{
	SG_UI_Msg_Lock(true);

	Destroy();

	if( iText != iTranslation && pTranslations && pTranslations->Get_Field_Count() > iText && pTranslations->Get_Field_Count() > iTranslation && pTranslations->Get_Record_Count() > 0 )
	{
		int		i;

		m_bCmpNoCase	= bCmpNoCase;

		if( m_bCmpNoCase )
		{
			for(i=0; i<pTranslations->Get_Record_Count(); i++)
			{
				CSG_Table_Record	*pRecord	= pTranslations->Get_Record(i);

				CSG_String	s	= pRecord->asString(iText);

				pRecord->Set_Value(iText, s.Make_Lower().c_str());
			}
		}

		pTranslations->Set_Index(iText, TABLE_INDEX_Ascending);

		m_Translations	= (CSG_Translation **)SG_Malloc(pTranslations->Get_Record_Count() * sizeof(CSG_Translation *));

		for(i=0; i<pTranslations->Get_Record_Count(); i++)
		{
			CSG_Table_Record	*pRecord	= pTranslations->Get_Record_byIndex(i);

			if( *pRecord->asString(iText) && *pRecord->asString(iTranslation) )
			{
				m_Translations[m_nTranslations++]	= new CSG_Translation(pRecord->asString(iText), pRecord->asString(iTranslation));
			}
		}

		if( m_nTranslations < pTranslations->Get_Record_Count() )
		{
			m_Translations	= (CSG_Translation **)SG_Realloc(m_Translations, m_nTranslations * sizeof(CSG_Translation *));
		}
	}

	SG_UI_Msg_Lock(false);

	return( m_nTranslations > 0 );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#define COMPARE(Index, Text)	(m_bCmpNoCase ? m_Translations[Index]->m_Text.CmpNoCase(Text) : m_Translations[Index]->m_Text.Cmp(Text))

//---------------------------------------------------------
int CSG_Translator::_Get_Index(const CSG_String &Text) const
{
	int		a, b, i, c;

	if( m_nTranslations == 1 )
	{
		c	= COMPARE(0, Text);

		return( c >= 0 ? 0 : 1 );
	}

	if( m_nTranslations > 1 )
	{
		for(a=0, b=m_nTranslations-1; b - a > 1; )
		{
			i	= a + (b - a) / 2;
			c	= COMPARE(i, Text);

			if( c > 0 )
			{
				b	= i;
			}
			else if( c < 0 )
			{
				a	= i;
			}
			else
			{
				return( i );
			}
		}

		if( COMPARE(a, Text) < 0 )
		{
			if( COMPARE(b, Text) < 0 )
			{
				return( m_nTranslations );
			}

			return( b );
		}

		if( COMPARE(b, Text) > 0 )
		{
			return( a );
		}
	}

	return( m_nTranslations );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
const SG_Char * CSG_Translator::Get_Translation(const SG_Char *Text, bool bReturnNullOnNotFound) const
{
	if( Text )
	{
		if( m_nTranslations > 0 )
		{
			int			i;
			CSG_String	s(Text);

			if( *Text == '{' )
			{
				s	= s.AfterFirst('{').BeforeFirst('}');
			}

			if(	(i = _Get_Index(s)) < m_nTranslations && !COMPARE(i, s) )
			{
				return( m_Translations[i]->m_Translation );
			}
		}

		if( bReturnNullOnNotFound )
		{
			return( NULL );
		}

		//-------------------------------------------------
		if( *Text == '{' )
		{
			do	{	Text++;	}	while( *Text != '}' && *Text != '\0' );
			do	{	Text++;	}	while( *Text == ' ' && *Text != '\0' );
		}
	}

	return( Text );
}

//---------------------------------------------------------
bool CSG_Translator::Get_Translation(const SG_Char *Text, CSG_String &Translation) const
{
	if( Text )
	{
		if( m_nTranslations > 0 )
		{
			int			i;
			CSG_String	s(Text);

			if( *Text == '{' )
			{
				s	= s.AfterFirst('{').BeforeFirst('}');
			}

			if(	(i = _Get_Index(s)) < m_nTranslations && !COMPARE(i, s) )
			{
				Translation	= m_Translations[i]->m_Translation;

				return( true );
			}
		}

		//-------------------------------------------------
		if( *Text == '{' )
		{
			do	{	Text++;	}	while( *Text != '}' && *Text != '\0' );
			do	{	Text++;	}	while( *Text == ' ' && *Text != '\0' );
		}

		Translation	= Text;
	}

	return( false );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
