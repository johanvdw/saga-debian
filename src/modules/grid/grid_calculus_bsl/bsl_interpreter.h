/**********************************************************
 * Version $Id: bsl_interpreter.h 911 2011-02-14 16:38:15Z reklov_w $
 *********************************************************/

///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#ifndef HEADER_INCLUDED__BSL_Interpreter_H
#define HEADER_INCLUDED__BSL_Interpreter_H

//---------------------------------------------------------
#include "MLB_Interface.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class CBSL_Interpreter : public CSG_Module_Grid
{
public:
	CBSL_Interpreter(bool bFile);
	virtual ~CBSL_Interpreter(void);


protected:

	virtual bool		On_Execute(void);
	

private:

	bool				m_bFile;

	CSG_String			m_BSL;


	bool				Parse_Vars		(bool bFlag);


};

//---------------------------------------------------------
void	g_Add_Grid		(CSG_Grid *pGrid);
bool	g_Set_Progress	(int i, int n);


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#endif // #ifndef HEADER_INCLUDED__BSL_Interpreter_H
