/**********************************************************
 * Version $Id: destriping1.cpp 911 2011-02-14 16:38:15Z reklov_w $
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
//                    destriping1.cpp                    //
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
#include "destriping1.h"

//---------------------------------------------------------
Cdestriping1::Cdestriping1(void)
{
	// 1. Info...
	Set_Name(_TL("Destriping 1"));
	Set_Author(_TL("Alessandro Perego"));
	Set_Description(_TL("destriping1 filter for Grids"));

	// 2. Parameters...
	Parameters.Add_Grid(NULL, "INPUT", _TL("Input"), _TL("This must be your input data of type grid."), PARAMETER_INPUT);
	Parameters.Add_Grid(NULL, "RESULT3", _TL("Destriped Grid"), _TL("New grid filtered with the destriping1 module"), PARAMETER_OUTPUT);
	Parameters.Add_Grid(NULL, "RESULT1", _TL("Low-pass 1"), _TL("Step 1: low-pass of stripe"), PARAMETER_OUTPUT);
	Parameters.Add_Grid(NULL, "RESULT2", _TL("Low-pass 2"), _TL("Step 2: low-pass between stripe and its surruondings"), PARAMETER_OUTPUT);
	Parameters.Add_Value(NULL, "ANG", _TL("Angle (in degrees)"), _TL("0 is horizontal, 90 is vertical."), PARAMETER_TYPE_Double, 0.0);
	Parameters.Add_Value(NULL, "R", _TL("Radius"), _TL(""), PARAMETER_TYPE_Double, 10);
	Parameters.Add_Value(NULL, "D", _TL("Stripes distance"), _TL(""), PARAMETER_TYPE_Double, 2);
	
}

//---------------------------------------------------------
Cdestriping1::~Cdestriping1(void)
{}

//---------------------------------------------------------
bool Cdestriping1::On_Execute(void)
{
	int  x, y, r, dxmax, dymax, ix, ax, ay, bx, by, iy, iv, iw, n1, n2;
	double  d, ang, a, si, si2, co, co2, Sum1, Sum2;
	CSG_Grid  *pInput, *pRes1, *pRes2, *pRes3;

	pInput = Parameters("INPUT")->asGrid();
	pRes1 = Parameters("RESULT1")->asGrid();
	pRes2 = Parameters("RESULT2")->asGrid();
	pRes3 = Parameters("RESULT3")->asGrid();
	ang = Parameters("ANG")->asDouble();
	r = Parameters("R")->asInt();
	d = Parameters("D")->asDouble();

	//-----------------------------------------------------
	a = ang * 6.283185307179586 / 360;
	si = sin(a);
	co = cos(a);
	si2 = si * si;
	co2 = co * co;
	dxmax = (int)((r*sqrt(co2))+(d/2*sqrt(si2)));
	dymax = (int)((r*sqrt(si2))+(d/2*sqrt(co2)));

	if(si2 >= co2)
	{
		double  cosi, si05, dsi, iwcosi, ivmin1, ivmax1, ivmin2, ivmax2;
		cosi = co / si;
		si05 = sqrt( (0.5/si)*(0.5/si) );
		dsi = sqrt( (d/2/si)*(d/2/si) );
		for(y=0; y<Get_NY() && Set_Progress(y); y++)
		{
			for(x=0; x<Get_NX(); x++)
			{
				Sum1 = 0.0;
				n1 = 0;
				Sum2 = 0.0;
				n2 = 0;
				//----------------------------------------------------
				if( (ax = x - dxmax) <  0 )		{	ax = 0;			}	
				if( (bx = x + dxmax) >= Get_NX() )	{	bx = Get_NX() - 1;	}
				if( (ay = y - dymax) <  0 )		{	ay = 0;			}
				if( (by = y + dymax) >= Get_NY() )	{	by = Get_NY() - 1;	}
				//-----------------------------------------------------
				for(iy=ay; iy<=by; iy++)
				{
					iw = y - iy;
					iwcosi = iw * cosi;
					ivmin1 = iwcosi - si05;
					ivmax1 = iwcosi + si05;
					ivmin2 = iwcosi - dsi;
					ivmax2 = iwcosi + dsi;
					for(ix=ax; ix<=bx; ix++)
					{
						iv = x - ix;
						if( iv >= ivmin1 && iv <= ivmax1)
						{
							Sum1 += pInput->asDouble(ix, iy);
							n1++;
						}
						if( iv >= ivmin2 && iv <= ivmax2)
						{
							Sum2 += pInput->asDouble(ix, iy);
							n2++;
						}
					}
				}
				if( n1 > 0 && n2 > 0 )
				{
					pRes1->Set_Value(x, y, Sum1/n1);
					pRes2->Set_Value(x, y, Sum2/n2);
					pRes3->Set_Value(x, y, (Sum1/n1)-(Sum2/n2)+pInput->asDouble(x, y));
				}
			}
		}
	}

	if(si2 < co2)
	{
		double  sico, co05, dco, ivsico, iwmin1, iwmax1, iwmin2, iwmax2;
		sico = si / co;
		co05 = sqrt( (0.5/co)*(0.5/co) );
		dco = sqrt( (d/2/co)*(d/2/co) );
		for(y=0; y<Get_NY() && Set_Progress(y); y++)
		{
			for(x=0; x<Get_NX(); x++)
			{
				Sum1 = 0.0;
				n1 = 0;
				Sum2 = 0.0;
				n2 = 0;
				//----------------------------------------------------
				if( (ax = x - dxmax) <  0 )		{	ax = 0;			}	
				if( (bx = x + dxmax) >= Get_NX() )	{	bx = Get_NX() - 1;	}
				if( (ay = y - dymax) <  0 )		{	ay = 0;			}
				if( (by = y + dymax) >= Get_NY() )	{	by = Get_NY() - 1;	}
				//-----------------------------------------------------
				for(ix=ax; ix<=bx; ix++)
				{
					iv = x - ix;
					ivsico = iv * sico;
					iwmin1 = ivsico - co05;
					iwmax1 = ivsico + co05;
					iwmin2 = ivsico - dco;
					iwmax2 = ivsico + dco;
					for(iy=ay; iy<=by; iy++)
					{
						iw = y - iy;
						if( iw >= iwmin1 && iw <= iwmax1)
						{
							Sum1 += pInput->asDouble(ix, iy);
							n1++;
						}
						if( iw >= iwmin2 && iw <= iwmax2)
						{
							Sum2 += pInput->asDouble(ix, iy);
							n2++;
						}
					}
				}
				if( n1 > 0 && n2 > 0 )
				{
					pRes1->Set_Value(x, y, Sum1/n1);
					pRes2->Set_Value(x, y, Sum2/n2);
					pRes3->Set_Value(x, y, (Sum2/n2)-(Sum1/n1)+pInput->asDouble(x, y));
				}
			}
		}
	}
	return( true );
}

