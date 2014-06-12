/**********************************************************
 * Version $Id: Grid_Random_Terrain.h 2072 2014-03-31 08:50:43Z reklov_w $
 *********************************************************/
/*******************************************************************************
    Grid_Random_Terrain.h
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
    Foundation, Inc., 51 Franklin Street, 5th Floor, Boston, MA 02110-1301, USA
*******************************************************************************/ 
//---------------------------------------------------------
#ifndef HEADER_INCLUDED__Grid_Random_Terrain_H
#define HEADER_INCLUDED__Grid_Random_Terrain_H

#include "MLB_Interface.h"

class CGrid_Random_Terrain : public CSG_Module
{
public:
	CGrid_Random_Terrain(void);
	virtual ~CGrid_Random_Terrain(void);

	virtual CSG_String		Get_MenuPath	(void)	{	return( _TL("R:Grid Generation") );	}


protected:

	CSG_Grid *pGrid;

	bool On_Execute(void);
	void addBump (CSG_Grid*, int);


private:

	CSG_Grid* Get_Target_Grid(CSG_Parameters *pParameters);

};

#endif // #ifndef HEADER_INCLUDED__Grid_Random_Terrain_H
