/*
 * Thomas Schorr 2007
 */
 
#ifndef HEADER_INCLUDED__grib2_import_H
#define HEADER_INCLUDED__grib2_import_H


#include "MLB_Interface.h"
extern "C" {
#include "grib2.h"
}


class SGGrib2Import : public CSG_Module
{
public:
	SGGrib2Import(void);
	virtual ~SGGrib2Import(void);

	virtual const SG_Char *	Get_MenuPath	(void)		{	return( _TL("R:Import") );	}


protected:

	virtual bool			On_Execute		(void);


private:
	CSG_Grid	*pGrid;
	gribfield *gf;
	bool handle_latlon( void );
	bool handle_polar_stereographic( void );

};

#endif // #ifndef HEADER_INCLUDED__grib2_import_H
