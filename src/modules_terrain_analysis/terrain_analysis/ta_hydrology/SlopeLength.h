/**********************************************************
 * Version $Id: SlopeLength.h 1016 2011-04-27 18:40:36Z oconrad $
 *********************************************************/
/*******************************************************************************
    SlopeLength.h
    Copyright (C) Victor Olaya
    
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*******************************************************************************/ 


///////////////////////////////////////////////////////////
//                                                       //
//                                                       //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#include "MLB_Interface.h"


///////////////////////////////////////////////////////////
//                                                       //
//                                                       //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class CSlopeLength : public CSG_Module_Grid
{
public:
	CSlopeLength(void);

	virtual const SG_Char *	Get_MenuPath	(void)	{	return( _TL("R:Miscellaneous") );	}


protected:
	
	virtual bool			On_Execute		(void);


private:

	CSG_Grid				*m_pDEM, *m_pLength, m_Slope;


	void					Get_Length		(int x, int y);	

};


///////////////////////////////////////////////////////////
//                                                       //
//                                                       //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
