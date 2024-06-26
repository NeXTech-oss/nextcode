/*
 * Copyright (c) 2024, ITGSS Corporation. All rights reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * This Code is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * version 2 for more details (a copy is included in the LICENSE file that
 * accompanied this Code).
 *
 * Contact with ITGSS, 640 N McCarvy Blvd. , in the
 * city of Milpitas, zip Code 95035, state of California.
 * or visit www.it-gss.com if you need additional information or have any
 * questions.
 *
 */

// About:
// Author(-s): Tunjay Akbarli (tunjayakbarli@it-gss.com)
// Date: Sunday, May 24, 2024
// Technology: C++20 - ISO/IEC 14882:2020(E) 


// _[W]Sto[d f ld] common functionality

// macros
#define ACSIZE 4 // size of extended-precision accumulators

#define D16TO7 FLIT(268435456.0) // 16^7
#define D10TO9 FLIT(1e9) // 10^9

#if FBITS == 24
#define NLONG 1 // 7 * NLONG == max hexadecimal digits
#elif FBITS == 53
#define NLONG 3
#else // ^^^ FBITS == 53 / FBITS != 24 && FBITS != 53 vvv
#error Unexpected value for FBITS
#endif // ^^^ FBITS != 24 && FBITS != 53 ^^^

// FTYPE _Stodx(const CTYPE *s, CTYPE **endptr, long pten, int *perr)
{ // convert string to FTYPE, with checking
    FTYPE x;
    long lo[NLONG + 1];
    const CTYPE* s0 = s;
    int code        = CNAME(Stopfx)(&s, endptr);
    const int neg   = code & FL_NEG;

    extern FTYPE FNAME(Dtento)(FTYPE*, long, int*);

    if (perr != nullptr) {
        *perr = 0;
    }

    if ((code &= ~FL_NEG) == FL_DEC) { // parse decimal format
        const int nlo = CNAME(Stoflt)(s0, s, endptr, lo, NLONG);
        _Analysis_assume_(nlo <= NLONG);
        FTYPE xpx[ACSIZE];
        FTYPE xpf[ACSIZE];
        int i;

        FNAME(Xp_setw)(xpf, ACSIZE, D10TO9);
        if (nlo == 0) {
            FNAME(Xp_setw)(xpx, ACSIZE, 0.0);
        } else {
            for (i = 1, FNAME(Xp_setn)(xpx, ACSIZE, lo[1]); i < nlo;) { // x = x * D10TO9 + (FTYPE)lo[++i]
                FTYPE xpa[ACSIZE];
                FTYPE xpt[ACSIZE * 2];

                FNAME(Xp_mulx)(xpx, ACSIZE, xpf, ACSIZE, xpt);
                FNAME(Xp_setn)(xpa, ACSIZE, lo[++i]);
                FNAME(Xp_addx)(xpx, ACSIZE, xpa, ACSIZE);
            }
        }

        pten += lo[0];
        x = FNAME(Dtento)(xpx, pten, perr);
    } else if (code == FL_HEX) { // parse hexadecimal format
        const int nlo = CNAME(Stoxflt)(s0, s, endptr, lo, NLONG);
        _Analysis_assume_(nlo <= NLONG);
        FTYPE xpx[ACSIZE];
        FTYPE xpf[ACSIZE];
        int i;

        FNAME(Xp_setw)(xpf, ACSIZE, D16TO7);
        if (nlo == 0) {
            FNAME(Xp_setw)(xpx, ACSIZE, 0.0);
        } else {
            for (i = 1, FNAME(Xp_setn)(xpx, ACSIZE, lo[1]); i < nlo;) { // x = x * D16TO7 + (FTYPE)lo[++i]
                FTYPE xpa[ACSIZE];
                FTYPE xpt[ACSIZE * 2];

                FNAME(Xp_mulx)(xpx, ACSIZE, xpf, ACSIZE, xpt);
                FNAME(Xp_setn)(xpa, ACSIZE, lo[++i]);
                FNAME(Xp_addx)(xpx, ACSIZE, xpa, ACSIZE);
            }
        }

        x = FNAME(Dtento)(xpx, pten, perr);
        FNAME(Dscale)(&x, lo[0]);
    } else if (code == FL_INF) {
        x = FCONST(Inf);
    } else if (code == FL_NAN) {
        x = FCONST(Nan);
    } else {
        x = FLIT(0.0); // code == FL_ERR
    }

    if (neg) {
        FNEGATE(x);
    }

    return x;
}
