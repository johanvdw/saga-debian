
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
//                   api_colors.cpp                      //
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
//					class CSG_Colors					 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#include <stdlib.h>
#include <string.h>

#include "api_core.h"
#include "mat_tools.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#define COLORS_SERIAL_VERSION_BINARY	SG_T("SAGA_COLORPALETTE_VERSION_0.100_BINARY")
#define COLORS_SERIAL_VERSION__ASCII	SG_T("SAGA_COLORPALETTE_VERSION_0.100__ASCII")


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CSG_Colors::CSG_Colors(void)
{
	m_Colors	= NULL;
	m_nColors	= 0;

	Set_Count(100);
}

//---------------------------------------------------------
CSG_Colors::CSG_Colors(const CSG_Colors &Colors)
{
	m_Colors	= NULL;
	m_nColors	= 0;

	Assign(Colors);
}

//---------------------------------------------------------
CSG_Colors::CSG_Colors(int nColors, int Palette, bool bRevert)
{
	m_Colors	= NULL;
	m_nColors	= 0;

	if( nColors <= 1 )
	{
		nColors	= 100;
	}

	Set_Count(nColors);

	Set_Palette(Palette, bRevert, nColors);
}

//---------------------------------------------------------
CSG_Colors::~CSG_Colors(void)
{
	Destroy();
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CSG_Colors::Destroy(void)
{
	if( m_nColors > 0 )
	{
		SG_Free(m_Colors);

		m_Colors	= NULL;
		m_nColors	= 0;
	}
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CSG_Colors::Set_Count(int nColors)
{
	int		i, j, ja, jb;
	long	*Colors;
	double	d, dj, dRed, dGreen, dBlue;

	if( nColors > 0 && nColors != m_nColors )
	{
		if( m_nColors == 0 )
		{
			Set_Default(nColors);
		}
		else
		{
			Colors	= (long *)SG_Malloc(nColors * sizeof(long));

			//---------------------------------------------
			if( nColors < m_nColors )
			{
				d	= (double)m_nColors / (double)nColors;

				for(i=0; i<nColors; i++)
				{
					j	= (int)(i * d);

					Colors[i]	= SG_GET_RGB(Get_Red(j), Get_Green(j), Get_Blue(j));
				}
			}

			//---------------------------------------------
			else
			{
				jb	= 0;
				d	= (double)nColors / (double)(m_nColors - 1);

				for(i=0; i<m_nColors-1; i++)
				{
					ja	= jb;
					jb	= (int)((i + 1.0) * d);
					dj	= jb - ja;

					if( dj > 0 )
					{
						dRed	= (double)(Get_Red  (i) - Get_Red  (i + 1)) / dj;
						dGreen	= (double)(Get_Green(i) - Get_Green(i + 1)) / dj;
						dBlue	= (double)(Get_Blue (i) - Get_Blue (i + 1)) / dj;

						for(j=ja; j<jb; j++)
						{
							Colors[j]	= SG_GET_RGB(
								Get_Red  (i) - (j - ja) * dRed,
								Get_Green(i) - (j - ja) * dGreen,
								Get_Blue (i) - (j - ja) * dBlue
							);
						}
					}
					else
					{
						Colors[ja]	= m_Colors[i];
					}
				}
			}

			//---------------------------------------------
			SG_Free(m_Colors);

			m_nColors	= nColors;
			m_Colors	= Colors;
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
bool CSG_Colors::Set_Color(int Index, long Color)
{
	if( Index >= 0 && Index < m_nColors )
	{
		m_Colors[Index]	= Color;

		return( true );
	}

	return( false );
}

//---------------------------------------------------------
bool CSG_Colors::Set_Color(int Index, int Red, int Green, int Blue)
{
	return( Set_Color(Index, SG_GET_RGB(Red, Green, Blue)) );
}

//---------------------------------------------------------
bool CSG_Colors::Set_Red(int Index, int Value)
{
	return( Set_Color(Index, Value			, Get_Green(Index)	, Get_Blue(Index)) );
}

//---------------------------------------------------------
bool CSG_Colors::Set_Green(int Index, int Value)
{
	return( Set_Color(Index, Get_Red(Index)	, Value				, Get_Blue(Index)) );
}

//---------------------------------------------------------
bool CSG_Colors::Set_Blue(int Index, int Value)
{
	return( Set_Color(Index, Get_Red(Index)	, Get_Green(Index)	, Value) );
}

//---------------------------------------------------------
bool CSG_Colors::Set_Brightness(int Index, int Value)
{
	double	r, g, b, ds;

	//-----------------------------------------------------
	if( Value < 0 )
	{
		Value	= 0;
	}
	else if( Value > 255 )
	{
		Value	= 255;
	}

	//-----------------------------------------------------
	r	= Get_Red  (Index);
	g	= Get_Green(Index);
	b	= Get_Blue (Index);
	ds	= (r + g + b) / 3.0;

	if( ds > 0.0 )
	{
		ds	= Value / ds;
		r	*= ds;
		g	*= ds;
		b	*= ds;

		_Set_Brightness(r, g, b);
	}
	else
	{
		r	= g	= b	= Value / 3.0;
	}

	return( Set_Color(Index, (int)r, (int)g, (int)b) );
}

//---------------------------------------------------------
void CSG_Colors::_Set_Brightness(double &a, double &b, double &c, int Pass)
{
	if( a > 255 )
	{
		int		addSum;

		addSum	= (int)((a - 255) / 2.0);
		a		= 255;

		b		+= addSum;
		c		+= addSum;

		if( b > 255 )
		{
			addSum	= (int)(b - 255);
			b		= 255;

			c		+= addSum;

			if( c > 255 )
			{
				c	= 255;
			}
		}
		else if( c > 255 )
		{
			addSum	= (int)(c - 255);
			c		= 255;

			b		+= addSum;

			if( b > 255 )
			{
				b	= 255;
			}
		}
	}
	else if( Pass < 2 )
	{
		_Set_Brightness(b, c, a, Pass + 1);
	}
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CSG_Colors::Set_Default(int nColors)
{
	int		i;
	double	d, dStep;

	if( nColors > 0 )
	{
		m_nColors	= nColors;
		m_Colors	= (long *)SG_Realloc(m_Colors, m_nColors * sizeof(long));

		dStep		= 2 * M_PI / (double)Get_Count();

		for(i=0, d=0; i<Get_Count(); i++, d+=dStep)
		{
			Set_Color(i,
				(int)(d < M_PI / 2 ? 0 : 128 - 127 * sin(M_PI - d)),
				(int)(128 - 127 * cos(d)),
				(int)(d > M_PI * 3 / 2 ? 0 : 128 + 127 * sin(d))
			);
		}

		return( true );
	}

	return( false );
}

//---------------------------------------------------------
bool CSG_Colors::Set_Palette(int Index, bool bRevert, int nColors)
{
	switch( Index )
	{
	default:
		return( false );

	case SG_COLORS_DEFAULT:
		Set_Default(nColors);
		break;

	case SG_COLORS_DEFAULT_BRIGHT:
		Set_Default(nColors);
		Set_Ramp_Brighness(127, 127);
		break;

	case SG_COLORS_BLACK_WHITE:
		Set_Ramp(SG_GET_RGB(  0,   0,   0), SG_GET_RGB(255, 255, 255));
		break;

	case SG_COLORS_BLACK_RED:
		Set_Ramp(SG_GET_RGB(  0,   0,   0), SG_GET_RGB(255,   0,   0));
		break;

	case SG_COLORS_BLACK_GREEN:
		Set_Ramp(SG_GET_RGB(  0,   0,   0), SG_GET_RGB(  0, 255,   0));
		break;

	case SG_COLORS_BLACK_BLUE:
		Set_Ramp(SG_GET_RGB(  0,   0,   0), SG_GET_RGB(  0,   0, 255));
		break;

	case SG_COLORS_WHITE_RED:
		Set_Ramp(SG_GET_RGB(255, 255, 255), SG_GET_RGB(255,   0,   0));
		break;

	case SG_COLORS_WHITE_GREEN:
		Set_Ramp(SG_GET_RGB(255, 255, 255), SG_GET_RGB(  0, 127,   0));
		break;

	case SG_COLORS_WHITE_BLUE:
		Set_Ramp(SG_GET_RGB(255, 255, 255), SG_GET_RGB(  0,   0, 191));
		break;

	case SG_COLORS_YELLOW_RED:
		Set_Ramp(SG_GET_RGB(255, 255,   0), SG_GET_RGB(191,   0,   0));
		break;

	case SG_COLORS_YELLOW_GREEN:
		Set_Ramp(SG_GET_RGB(255, 255,   0), SG_GET_RGB(  0,  63,   0));
		break;

	case SG_COLORS_YELLOW_BLUE:
		Set_Ramp(SG_GET_RGB(255, 255,   0), SG_GET_RGB(  0,   0,  255));
		break;

	case SG_COLORS_RED_GREEN:
//		Set_Ramp(SG_GET_RGB(255,   0,   0), SG_GET_RGB(  0, 255,   0));
		Set_Count(5);
		Set_Color(0, SG_GET_RGB(  0, 255,   0));
		Set_Color(1, SG_GET_RGB(191, 191,   0));
		Set_Color(2, SG_GET_RGB(255, 127,   0));
		Set_Color(3, SG_GET_RGB(223,  63,   0));
		Set_Color(4, SG_GET_RGB( 63,   0,   0));
		break;

	case SG_COLORS_RED_BLUE:
		Set_Ramp(SG_GET_RGB(255,   0,   0), SG_GET_RGB(  0,   0, 255));
		break;

	case SG_COLORS_GREEN_BLUE:
		Set_Ramp(SG_GET_RGB(  0, 255,   0), SG_GET_RGB(  0,   0, 255));
		break;

	case SG_COLORS_RED_GREY_BLUE:
		Set_Count(5);
		Set_Color(0, SG_GET_RGB(127,   0,   0));
		Set_Color(1, SG_GET_RGB(255, 127,   0));
		Set_Color(2, SG_GET_RGB(200, 200, 200));
		Set_Color(3, SG_GET_RGB(  0, 127, 255));
		Set_Color(4, SG_GET_RGB(  0,   0, 127));
		break;

	case SG_COLORS_RED_GREY_GREEN:
		Set_Count(5);
		Set_Color(0, SG_GET_RGB(127,   0,   0));
		Set_Color(1, SG_GET_RGB(255, 127,   0));
		Set_Color(2, SG_GET_RGB(200, 200, 200));
		Set_Color(3, SG_GET_RGB(  0, 255, 127));
		Set_Color(4, SG_GET_RGB(  0, 127,   0));
		break;

	case SG_COLORS_GREEN_GREY_BLUE:
		Set_Count(5);
		Set_Color(0, SG_GET_RGB(  0, 127,   0));
		Set_Color(1, SG_GET_RGB(127, 255,   0));
		Set_Color(2, SG_GET_RGB(200, 200, 200));
		Set_Color(3, SG_GET_RGB(  0, 127, 255));
		Set_Color(4, SG_GET_RGB(  0,   0, 127));
		break;

	case SG_COLORS_RED_GREEN_BLUE:
		Set_Count(5);
		Set_Color(0, SG_GET_RGB(127,   0, 127));
		Set_Color(1, SG_GET_RGB(255,   0,   0));
		Set_Color(2, SG_GET_RGB(  0, 255,   0));
		Set_Color(3, SG_GET_RGB(  0,   0, 255));
		Set_Color(4, SG_GET_RGB(127,   0, 127));
		break;

	case SG_COLORS_RED_BLUE_GREEN:
		Set_Count(5);
		Set_Color(0, SG_GET_RGB(127, 127,   0));
		Set_Color(1, SG_GET_RGB(255,   0,   0));
		Set_Color(2, SG_GET_RGB(  0,   0, 255));
		Set_Color(3, SG_GET_RGB(  0, 255,   0));
		Set_Color(4, SG_GET_RGB(127, 127,   0));
		break;

	case SG_COLORS_GREEN_RED_BLUE:
		Set_Count(5);
		Set_Color(0, SG_GET_RGB(  0, 127, 127));
		Set_Color(1, SG_GET_RGB(  0, 255,   0));
		Set_Color(2, SG_GET_RGB(255,   0,   0));
		Set_Color(3, SG_GET_RGB(  0,   0, 255));
		Set_Color(4, SG_GET_RGB(  0, 127, 127));
		break;

	case SG_COLORS_RAINBOW:
		Set_Count(8);
		Set_Color(0, SG_GET_RGB(127,   0, 127));
		Set_Color(1, SG_GET_RGB(  0,   0, 255));
		Set_Color(2, SG_GET_RGB(  0, 255, 255));
		Set_Color(3, SG_GET_RGB(  0, 191,   0));
		Set_Color(4, SG_GET_RGB(255, 255,   0));
		Set_Color(5, SG_GET_RGB(255, 127,   0));
		Set_Color(6, SG_GET_RGB(255,   0,   0));
		Set_Color(7, SG_GET_RGB(127,   0,   0));
		break;

	case SG_COLORS_NEON:
		Set_Count(7);
		Set_Color(0, SG_GET_RGB(  0,   0,   0));
		Set_Color(1, SG_GET_RGB(255,   0,   0));
		Set_Color(2, SG_GET_RGB(  0,   0,   0));
		Set_Color(3, SG_GET_RGB(255, 255,   0));
		Set_Color(4, SG_GET_RGB(  0,   0,   0));
		Set_Color(5, SG_GET_RGB(  0, 255,   0));
		Set_Color(6, SG_GET_RGB(  0,   0,   0));
		break;
	}

	//-----------------------------------------------------
	Set_Count(nColors);

	if( bRevert )
	{
		Revert();
	}

	return( true );
}

//---------------------------------------------------------
bool CSG_Colors::Set_Ramp(long Color_A, long Color_B)
{
	return( Set_Ramp(Color_A, Color_B, 0, Get_Count() - 1) );
}

//---------------------------------------------------------
bool CSG_Colors::Set_Ramp(long Color_A, long Color_B, int iColor_A, int iColor_B)
{
	int		i, n, ar, ag, ab;
	double	dr, dg, db;

	//-----------------------------------------------------
	if( iColor_A > iColor_B )
	{
		i			= iColor_A;
		iColor_A	= iColor_B;
		iColor_B	= i;
	}

	if( iColor_A < 0 )
	{
		iColor_A	= 0;
	}

	if( iColor_B >= Get_Count() )
	{
		iColor_B	= Get_Count() - 1;
	}

	//-----------------------------------------------------
	if( (n = iColor_B - iColor_A) > 0 )
	{
		ar		= SG_GET_R(Color_A);
		dr		= (double)(SG_GET_R(Color_B) - ar) / (double)n;

		ag		= SG_GET_G(Color_A);
		dg		= (double)(SG_GET_G(Color_B) - ag) / (double)n;

		ab		= SG_GET_B(Color_A);
		db		= (double)(SG_GET_B(Color_B) - ab) / (double)n;

		for(i=0; i<=n; i++)
		{
			Set_Color(iColor_A + i,
				(int)(ar + i * dr),
				(int)(ag + i * dg),
				(int)(ab + i * db)
			);
		}

		return( true );
	}

	return( false );
}

//---------------------------------------------------------
bool CSG_Colors::Set_Ramp_Brighness(int Brightness_A, int Brightness_B)
{
	return( Set_Ramp_Brighness(Brightness_A, Brightness_B, 0, Get_Count() - 1) );
}

//---------------------------------------------------------
bool CSG_Colors::Set_Ramp_Brighness(int Brightness_A, int Brightness_B, int iColor_A, int iColor_B)
{
	int		i, n;
	double	dBrightness;

	//-----------------------------------------------------
	if( iColor_A > iColor_B )
	{
		i			= iColor_A;
		iColor_A	= iColor_B;
		iColor_B	= i;
	}

	if( iColor_A < 0 )
	{
		iColor_A	= 0;
	}

	if( iColor_B >= Get_Count() )
	{
		iColor_B	= Get_Count() - 1;
	}

	//-----------------------------------------------------
	if( (n = iColor_B - iColor_A) > 0 )
	{
		dBrightness	= (double)(Brightness_B - Brightness_A) / (double)n;

		for(i=0; i<=n; i++)
		{
			Set_Brightness(iColor_A + i, (int)(Brightness_A + i * dBrightness));
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
bool CSG_Colors::Random(void)
{
	int		i;

	for(i=0; i<Get_Count(); i++)
	{
		Set_Color(i,
			(int)(255.0 * (double)rand() / (double)RAND_MAX),
			(int)(255.0 * (double)rand() / (double)RAND_MAX),
			(int)(255.0 * (double)rand() / (double)RAND_MAX)
		);
	}

	return( Get_Count() > 0 );
}

//---------------------------------------------------------
bool CSG_Colors::Invert(void)
{
	int		i;

	for(i=0; i<Get_Count(); i++)
	{
		Set_Color(i, 255 - Get_Red(i), 255 - Get_Green(i), 255 - Get_Blue(i));
	}

	return( Get_Count() > 0 );
}

//---------------------------------------------------------
bool CSG_Colors::Revert(void)
{
	int		i, j;
	long	c;

	for(i=0, j=Get_Count()-1; i<j; i++, j--)
	{
		c		=    Get_Color(j);
		Set_Color(j, Get_Color(i));
		Set_Color(i, c);
	}

	return( Get_Count() > 0 );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CSG_Colors & CSG_Colors::operator = (const CSG_Colors &Colors)
{
	Assign(Colors);

	return( *this );
}

bool CSG_Colors::Assign(const CSG_Colors &Colors)
{
	if( Colors.m_nColors > 0 )
	{
		m_nColors	= Colors.m_nColors;
		m_Colors	= (long *)SG_Realloc(m_Colors, m_nColors * sizeof(long));

		memcpy(m_Colors, Colors.m_Colors, m_nColors * sizeof(long));

		return( true );
	}

	return( false );
}

bool CSG_Colors::Assign(CSG_Colors *pColors)
{
	return( pColors ? Assign(*pColors) : false );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CSG_Colors::Load(const CSG_String &File_Name)
{
	CSG_String	Version;
	CSG_File	Stream;

	if( Stream.Open(File_Name, SG_FILE_R, true) )
	{
		Stream.Read(Version, sizeof(COLORS_SERIAL_VERSION_BINARY));

		if( !Version.Cmp(COLORS_SERIAL_VERSION_BINARY) )
		{
			return( Serialize(Stream, false, true) );
		}
		else if( !Version.Cmp(COLORS_SERIAL_VERSION__ASCII) )
		{
			return( Serialize(Stream, false, false) );
		}
		else	// SAGA 1.x compatibility...
		{
			short		nColors;

			Stream.Seek_Start();
			Stream.Read(&nColors, sizeof(short));

			if( Stream.Length() == (int)(sizeof(short) + 3 * nColors) )
			{
				BYTE	*R, *G, *B;

				R	= (BYTE *)SG_Malloc(nColors * sizeof(BYTE));
				G	= (BYTE *)SG_Malloc(nColors * sizeof(BYTE));
				B	= (BYTE *)SG_Malloc(nColors * sizeof(BYTE));

				Stream.Read(R, nColors * sizeof(BYTE));
				Stream.Read(G, nColors * sizeof(BYTE));
				Stream.Read(B, nColors * sizeof(BYTE));

				Set_Count(nColors);

				for(int i=0; i<nColors; i++)
				{
					Set_Color(i, R[i], G[i], B[i]);
				}

				SG_Free(R);
				SG_Free(G);
				SG_Free(B);

				return( true );
			}
		}
	}

	return( false );
}

//---------------------------------------------------------
bool CSG_Colors::Save(const CSG_String &File_Name, bool bBinary)
{
	CSG_File	Stream;

	if( Stream.Open(File_Name, SG_FILE_W, true) )
	{
		if( bBinary )
		{
			Stream.Write((void *)COLORS_SERIAL_VERSION_BINARY, sizeof(COLORS_SERIAL_VERSION_BINARY));
		}
		else
		{
			Stream.Printf(SG_T("%s\n"), COLORS_SERIAL_VERSION__ASCII);
		}

		Serialize(Stream, true, bBinary);

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
bool CSG_Colors::Serialize(CSG_File &Stream, bool bSave, bool bBinary)
{
	int		i, r, g, b;

	if( Stream.is_Open() )
	{
		if( bBinary )
		{
			if( bSave )
			{
				if( m_nColors > 0 )
				{
					Stream.Write(&m_nColors, sizeof(m_nColors));
					Stream.Write(m_Colors, sizeof(long), m_nColors);
				}
			}
			else
			{
				Stream.Read(&i, sizeof(m_nColors));

				if( i > 0 )
				{
					Set_Count(i);

					Stream.Read(m_Colors, sizeof(long), m_nColors);
				}
			}

			return( true );
		}
		else
		{
			if( bSave )
			{
				if( m_nColors > 0 )
				{
					Stream.Printf(SG_T("%d\n"), m_nColors);

					for(i=0; i<m_nColors; i++)
					{
						Stream.Printf(SG_T("%03d %03d %03d\n"), (int)Get_Red(i), (int)Get_Green(i), (int)Get_Blue(i));
					}
				}
			}
			else
			{
				SG_FILE_SCANF(Stream.Get_Stream(), SG_T("%d"), &i);

				if( i > 0 )
				{
					Set_Count(i);

					for(i=0; i<m_nColors; i++)
					{
						SG_FILE_SCANF(Stream.Get_Stream(), SG_T("%d %d %d"), &r, &g, &b);

						m_Colors[i]	= SG_GET_RGB(r, g, b);
					}
				}
			}

			return( true );
		}
	}

	return( false );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
