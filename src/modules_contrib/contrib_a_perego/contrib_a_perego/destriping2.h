/**********************************************************
 * Version $Id: destriping2.h 911 2011-02-14 16:38:15Z reklov_w $
 *********************************************************/

///////////////////////////////////////////////////////////
//                                                       //
//                        Module:                        //
//                      destriping                       //
//                                                       //
//                       for SAGA                        //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//                     destriping2.h                     //
//                                                       //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//                                                       //
//    by Alessandro Perego (Italy)                       //
//                                                       //
//    e-mail:     alper78@alice.it                       //
//                                                       //
//                                                       //
///////////////////////////////////////////////////////////


//---------------------------------------------------------
#ifndef HEADER_INCLUDED__destriping2_H
#define HEADER_INCLUDED__destriping2_H

//---------------------------------------------------------
#include "MLB_Interface.h"

//---------------------------------------------------------
class Cdestriping2 : public CSG_Module_Grid
{
public: ////// public members and functions: //////////////

	Cdestriping2(void);					// constructor
	virtual ~Cdestriping2(void);				// destructor

	virtual const SG_Char *	Get_MenuPath			(void)	{	return( _TL("R:Filter for Grids") );	}


protected: /// protected members and functions: ///////////

	virtual bool		On_Execute(void);		// always override this function


private: ///// private members and functions: /////////////


};


//---------------------------------------------------------
#endif // #ifndef HEADER_INCLUDED__destriping2_H
