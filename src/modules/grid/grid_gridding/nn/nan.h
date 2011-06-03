/**********************************************************
 * Version $Id: nan.h 911 2011-02-14 16:38:15Z reklov_w $
 *********************************************************/
/******************************************************************************
 *
 * File:           nan.h
 *
 * Created:        18/10/2001
 *
 * Author:         Pavel Sakov
 *                 CSIRO Marine Research
 *
 * Purpose:        NaN definition
 *
 * Description:    Should cover machines with 64 bit doubles or other machines
 *                 with GCC
 *
 * Revisions:      None
 *
 *****************************************************************************/

#if !defined(_NAN_H)
#define _NAN_H


#if defined(__GNUC__)
	static const double NaN = 0.0 / 0.0;

#elif defined(BIG_ENDIAN) || defined(_BIG_ENDIAN)
	static const long long lNaN = 0x7fffffffffffffff;
	#define NaN (*(double*)&lNaN)

#elif defined(_SAGA_VC)
	static const __int64 lNaN = 0xfff8000000000000;
	#define NaN (*(double*)&lNaN)

#else
	static const long long lNaN = 0xfff8000000000000;
	#define NaN (*(double*)&lNaN)
#endif


#endif
