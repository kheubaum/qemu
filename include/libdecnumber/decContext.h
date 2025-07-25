/* Decimal context header module for the decNumber C Library.
   Copyright (C) 2005, 2007 Free Software Foundation, Inc.
   Contributed by IBM Corporation.  Author Mike Cowlishaw.

   This file is part of GCC.

   GCC is free software; you can redistribute it and/or modify it under
   the terms of the GNU General Public License as published by the Free
   Software Foundation; either version 2, or (at your option) any later
   version.

   In addition to the permissions in the GNU General Public License,
   the Free Software Foundation gives you unlimited permission to link
   the compiled version of this file into combinations with other
   programs, and to distribute those combinations without any
   restriction coming from the use of this file.  (The General Public
   License restrictions do apply in other respects; for example, they
   cover modification of the file, and distribution when not linked
   into a combine executable.)

   GCC is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
   for more details.

   You should have received a copy of the GNU General Public License
   along with GCC; see the file COPYING.  If not, see
   <https://www.gnu.org/licenses/>.  */

/* ------------------------------------------------------------------ */
/* Decimal Context module header				      */
/* ------------------------------------------------------------------ */
/*								      */
/* Context variables must always have valid values:		      */
/*								      */
/*  status   -- [any bits may be cleared, but not set, by user]	      */
/*  round    -- must be one of the enumerated rounding modes	      */
/*								      */
/* The following variables are implied for fixed size formats (i.e.,  */
/* they are ignored) but should still be set correctly in case used   */
/* with decNumber functions:					      */
/*								      */
/*  clamp    -- must be either 0 or 1				      */
/*  digits   -- must be in the range 1 through 999999999	      */
/*  emax     -- must be in the range 0 through 999999999	      */
/*  emin     -- must be in the range 0 through -999999999	      */
/*  extended -- must be either 0 or 1 [present only if DECSUBSET]     */
/*  traps    -- only defined bits may be set			      */
/*								      */
/* ------------------------------------------------------------------ */

#ifndef DECCONTEXT_H
#define DECCONTEXT_H

  #define DECCNAME     "decContext"			/* Short name */
  #define DECCFULLNAME "Decimal Context Descriptor"   /* Verbose name */
  #define DECCAUTHOR   "Mike Cowlishaw"		      /* Who to blame */


  /* Extended flags setting -- set this to 0 to use only IEEE flags   */
  #define DECEXTFLAG 1		   /* 1=enable extended flags	      */

  /* Conditional code flag -- set this to 0 for best performance      */
  #define DECSUBSET  0		   /* 1=enable subset arithmetic      */

  /* Context for operations, with associated constants		      */
  enum rounding {
    DEC_ROUND_CEILING,		   /* round towards +infinity	      */
    DEC_ROUND_UP,		   /* round away from 0		      */
    DEC_ROUND_HALF_UP,		   /* 0.5 rounds up		      */
    DEC_ROUND_HALF_EVEN,	   /* 0.5 rounds to nearest even      */
    DEC_ROUND_HALF_DOWN,	   /* 0.5 rounds down		      */
    DEC_ROUND_DOWN,		   /* round towards 0 (truncate)      */
    DEC_ROUND_FLOOR,		   /* round towards -infinity	      */
    DEC_ROUND_05UP,		   /* round for reround		      */
    DEC_ROUND_MAX		   /* enum must be less than this     */
    };
  #define DEC_ROUND_DEFAULT DEC_ROUND_HALF_EVEN;

  typedef struct {
    int32_t  digits;		   /* working precision		      */
    int32_t  emax;		   /* maximum positive exponent	      */
    int32_t  emin;		   /* minimum negative exponent	      */
    enum     rounding round;	   /* rounding mode		      */
    uint32_t traps;		   /* trap-enabler flags	      */
    uint32_t status;		   /* status flags		      */
    uint8_t  clamp;		   /* flag: apply IEEE exponent clamp */
    #if DECSUBSET
    uint8_t  extended;		   /* flag: special-values allowed    */
    #endif
    } decContext;

  /* Maxima and Minima for context settings			      */
  #define DEC_MAX_DIGITS 999999999
  #define DEC_MIN_DIGITS	 1
  #define DEC_MAX_EMAX	 999999999
  #define DEC_MIN_EMAX		 0
  #define DEC_MAX_EMIN		 0
  #define DEC_MIN_EMIN	-999999999
  #define DEC_MAX_MATH	    999999 /* max emax, etc., for math funcs. */

  /* Classifications for decimal numbers, aligned with 754r (note     */
  /* that 'normal' and 'subnormal' are meaningful only with a	      */
  /* decContext or a fixed size format).			      */
  enum decClass {
    DEC_CLASS_SNAN,
    DEC_CLASS_QNAN,
    DEC_CLASS_NEG_INF,
    DEC_CLASS_NEG_NORMAL,
    DEC_CLASS_NEG_SUBNORMAL,
    DEC_CLASS_NEG_ZERO,
    DEC_CLASS_POS_ZERO,
    DEC_CLASS_POS_SUBNORMAL,
    DEC_CLASS_POS_NORMAL,
    DEC_CLASS_POS_INF
    };
  /* Strings for the decClasses */
  #define DEC_ClassString_SN  "sNaN"
  #define DEC_ClassString_QN  "NaN"
  #define DEC_ClassString_NI  "-Infinity"
  #define DEC_ClassString_NN  "-Normal"
  #define DEC_ClassString_NS  "-Subnormal"
  #define DEC_ClassString_NZ  "-Zero"
  #define DEC_ClassString_PZ  "+Zero"
  #define DEC_ClassString_PS  "+Subnormal"
  #define DEC_ClassString_PN  "+Normal"
  #define DEC_ClassString_PI  "+Infinity"
  #define DEC_ClassString_UN  "Invalid"

  /* Trap-enabler and Status flags (exceptional conditions), and      */
  /* their names.  The top byte is reserved for internal use	      */
  #if DECEXTFLAG
    /* Extended flags */
    #define DEC_Conversion_syntax    0x00000001
    #define DEC_Division_by_zero     0x00000002
    #define DEC_Division_impossible  0x00000004
    #define DEC_Division_undefined   0x00000008
    #define DEC_Insufficient_storage 0x00000010 /* [when malloc fails]	*/
    #define DEC_Inexact		     0x00000020
    #define DEC_Invalid_context	     0x00000040
    #define DEC_Invalid_operation    0x00000080
    #if DECSUBSET
    #define DEC_Lost_digits	     0x00000100
    #endif
    #define DEC_Overflow	     0x00000200
    #define DEC_Clamped		     0x00000400
    #define DEC_Rounded		     0x00000800
    #define DEC_Subnormal	     0x00001000
    #define DEC_Underflow	     0x00002000
  #else
    /* IEEE flags only */
    #define DEC_Conversion_syntax    0x00000010
    #define DEC_Division_by_zero     0x00000002
    #define DEC_Division_impossible  0x00000010
    #define DEC_Division_undefined   0x00000010
    #define DEC_Insufficient_storage 0x00000010 /* [when malloc fails]	*/
    #define DEC_Inexact		     0x00000001
    #define DEC_Invalid_context	     0x00000010
    #define DEC_Invalid_operation    0x00000010
    #if DECSUBSET
    #define DEC_Lost_digits	     0x00000000
    #endif
    #define DEC_Overflow	     0x00000008
    #define DEC_Clamped		     0x00000000
    #define DEC_Rounded		     0x00000000
    #define DEC_Subnormal	     0x00000000
    #define DEC_Underflow	     0x00000004
  #endif

  /* IEEE 854 groupings for the flags				      */
  /* [DEC_Clamped, DEC_Lost_digits, DEC_Rounded, and DEC_Subnormal    */
  /* are not in IEEE 854]					      */
  #define DEC_IEEE_854_Division_by_zero	 (DEC_Division_by_zero)
  #if DECSUBSET
  #define DEC_IEEE_854_Inexact		 (DEC_Inexact | DEC_Lost_digits)
  #else
  #define DEC_IEEE_854_Inexact		 (DEC_Inexact)
  #endif
  #define DEC_IEEE_854_Invalid_operation (DEC_Conversion_syntax |     \
					  DEC_Division_impossible |   \
					  DEC_Division_undefined |    \
					  DEC_Insufficient_storage |  \
					  DEC_Invalid_context |	      \
					  DEC_Invalid_operation)
  #define DEC_IEEE_854_Overflow		 (DEC_Overflow)
  #define DEC_IEEE_854_Underflow	 (DEC_Underflow)

  /* flags which are normally errors (result is qNaN, infinite, or 0) */
  #define DEC_Errors (DEC_IEEE_854_Division_by_zero |		      \
		      DEC_IEEE_854_Invalid_operation |		      \
		      DEC_IEEE_854_Overflow | DEC_IEEE_854_Underflow)
  /* flags which cause a result to become qNaN			      */
  #define DEC_NaNs    DEC_IEEE_854_Invalid_operation

  /* flags which are normally for information only (finite results)   */
  #if DECSUBSET
  #define DEC_Information (DEC_Clamped | DEC_Rounded | DEC_Inexact    \
			  | DEC_Lost_digits)
  #else
  #define DEC_Information (DEC_Clamped | DEC_Rounded | DEC_Inexact)
  #endif

  /* Name strings for the exceptional conditions		      */
  #define DEC_Condition_CS "Conversion syntax"
  #define DEC_Condition_DZ "Division by zero"
  #define DEC_Condition_DI "Division impossible"
  #define DEC_Condition_DU "Division undefined"
  #define DEC_Condition_IE "Inexact"
  #define DEC_Condition_IS "Insufficient storage"
  #define DEC_Condition_IC "Invalid context"
  #define DEC_Condition_IO "Invalid operation"
  #if DECSUBSET
  #define DEC_Condition_LD "Lost digits"
  #endif
  #define DEC_Condition_OV "Overflow"
  #define DEC_Condition_PA "Clamped"
  #define DEC_Condition_RO "Rounded"
  #define DEC_Condition_SU "Subnormal"
  #define DEC_Condition_UN "Underflow"
  #define DEC_Condition_ZE "No status"
  #define DEC_Condition_MU "Multiple status"
  #define DEC_Condition_Length 21  /* length of the longest string,   */
				   /* including terminator	      */

  /* Initialization descriptors, used by decContextDefault	      */
  #define DEC_INIT_BASE		0
  #define DEC_INIT_DECIMAL32   32
  #define DEC_INIT_DECIMAL64   64
  #define DEC_INIT_DECIMAL128 128
  /* Synonyms */
  #define DEC_INIT_DECSINGLE  DEC_INIT_DECIMAL32
  #define DEC_INIT_DECDOUBLE  DEC_INIT_DECIMAL64
  #define DEC_INIT_DECQUAD    DEC_INIT_DECIMAL128

  /* decContext routines					      */


  extern decContext  * decContextClearStatus(decContext *, uint32_t);
  extern decContext  * decContextDefault(decContext *, int32_t);
  extern enum rounding decContextGetRounding(decContext *);
  extern uint32_t      decContextGetStatus(decContext *);
  extern decContext  * decContextRestoreStatus(decContext *, uint32_t, uint32_t);
  extern uint32_t      decContextSaveStatus(decContext *, uint32_t);
  extern decContext  * decContextSetRounding(decContext *, enum rounding);
  extern decContext  * decContextSetStatus(decContext *, uint32_t);
  extern decContext  * decContextSetStatusFromString(decContext *, const char *);
  extern decContext  * decContextSetStatusFromStringQuiet(decContext *, const char *);
  extern decContext  * decContextSetStatusQuiet(decContext *, uint32_t);
  extern const char  * decContextStatusToString(const decContext *);
  extern uint32_t      decContextTestSavedStatus(uint32_t, uint32_t);
  extern uint32_t      decContextTestStatus(decContext *, uint32_t);
  extern decContext  * decContextZeroStatus(decContext *);

#endif
