/*
 * hdmi_driver.c
 *
 *  Created on: 14.05.2014
 *      Author: faisstm
 */

#include "hdmi_driver.h"

#include <stdlib.h>
#include <string.h>

//#define reg32w(b, r, v) (*((volatile uint32_t *)((b)+(r))) = (v))
//#define reg32r(b, r) (*(volatile uint32_t *)((b)+(r)))

#define reg32w(b, r, v) 	writeReg( b + r, v )
#define reg32r(b, r) 		readReg( b + r )

static unsigned char font[2048] =
{
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 000 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 001 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 002 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 003 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 004 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 005 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 006 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 007 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 008 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 009 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 010 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 011 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 012 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 013 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 014 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 015 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 016 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 017 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 018 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 019 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 020 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 021 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 022 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 023 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 024 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 025 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 026 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 027 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 028 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 029 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 030 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 031 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 032 ( )
	0x00, 0x10, 0x10, 0x10, 0x10, 0x00, 0x10, 0x00,	// Char 033 (!)
	0x00, 0x28, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 034 (")
	0x00, 0x24, 0x7E, 0x24, 0x24, 0x7E, 0x24, 0x00,	// Char 035 (#)
	0x3C, 0x42, 0x81, 0x99, 0x99, 0x81, 0x42, 0x3C,	// Char 036 ($)
	0x00, 0x22, 0x04, 0x08, 0x10, 0x20, 0x44, 0x00,	// Char 037 (%)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 038 (&)
	0x00, 0x04, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 039 (')
	0x00, 0x08, 0x10, 0x20, 0x20, 0x10, 0x08, 0x00,	// Char 040 (()
	0x00, 0x10, 0x08, 0x04, 0x04, 0x08, 0x10, 0x00,	// Char 041 ())
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 042 (*)
	0x00, 0x08, 0x08, 0x3E, 0x08, 0x08, 0x00, 0x00,	// Char 043 (+)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x10,	// Char 044 (,)
	0x00, 0x00, 0x00, 0x00, 0x3C, 0x00, 0x00, 0x00,	// Char 045 (-)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00,	// Char 046 (.)
	0x00, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x00,	// Char 047 (/)
	0x00, 0x18, 0x24, 0x2C, 0x34, 0x24, 0x18, 0x00,	// Char 048 (0)
	0x00, 0x08, 0x18, 0x08, 0x08, 0x08, 0x1C, 0x00,	// Char 049 (1)
	0x00, 0x18, 0x24, 0x04, 0x08, 0x10, 0x3C, 0x00,	// Char 050 (2)
	0x00, 0x38, 0x04, 0x38, 0x04, 0x04, 0x38, 0x00,	// Char 051 (3)
	0x00, 0x20, 0x20, 0x28, 0x3C, 0x08, 0x08, 0x00,	// Char 052 (4)
	0x00, 0x3C, 0x20, 0x38, 0x04, 0x04, 0x38, 0x00,	// Char 053 (5)
	0x00, 0x1C, 0x20, 0x38, 0x24, 0x24, 0x18, 0x00,	// Char 054 (6)
	0x00, 0x3C, 0x04, 0x08, 0x10, 0x10, 0x10, 0x00,	// Char 055 (7)
	0x00, 0x18, 0x24, 0x18, 0x24, 0x24, 0x18, 0x00,	// Char 056 (8)
	0x00, 0x18, 0x24, 0x24, 0x1C, 0x04, 0x18, 0x00,	// Char 057 (9)
	0x00, 0x00, 0x00, 0x10, 0x00, 0x10, 0x00, 0x00,	// Char 058 (:)
	0x00, 0x00, 0x00, 0x10, 0x00, 0x10, 0x20, 0x00,	// Char 059 (;)
	0x00, 0x00, 0x08, 0x10, 0x20, 0x10, 0x08, 0x00,	// Char 060 (<)
	0x00, 0x00, 0x00, 0x3C, 0x00, 0x3C, 0x00, 0x00,	// Char 061 (=)
	0x00, 0x00, 0x10, 0x08, 0x04, 0x08, 0x10, 0x00,	// Char 062 (>)
	0x00, 0x30, 0x08, 0x08, 0x10, 0x00, 0x10, 0x00,	// Char 063 (?)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 064 (@)
	0x24, 0x18, 0x24, 0x24, 0x3C, 0x24, 0x24, 0x00,	// Char 065 (A)
	0x00, 0x38, 0x24, 0x38, 0x24, 0x24, 0x38, 0x00,	// Char 066 (B)
	0x00, 0x18, 0x24, 0x20, 0x20, 0x24, 0x18, 0x00,	// Char 067 (C)
	0x00, 0x38, 0x24, 0x24, 0x24, 0x24, 0x38, 0x00,	// Char 068 (D)
	0x00, 0x3C, 0x20, 0x38, 0x20, 0x20, 0x3C, 0x00,	// Char 069 (E)
	0x00, 0x3C, 0x20, 0x38, 0x20, 0x20, 0x20, 0x00,	// Char 070 (F)
	0x00, 0x18, 0x24, 0x20, 0x2C, 0x24, 0x18, 0x00,	// Char 071 (G)
	0x00, 0x24, 0x24, 0x24, 0x3C, 0x24, 0x24, 0x00,	// Char 072 (H)
	0x00, 0x38, 0x10, 0x10, 0x10, 0x10, 0x38, 0x00,	// Char 073 (I)
	0x00, 0x08, 0x08, 0x08, 0x08, 0x28, 0x10, 0x00,	// Char 074 (J)
	0x00, 0x24, 0x28, 0x30, 0x30, 0x28, 0x24, 0x00,	// Char 075 (K)
	0x00, 0x20, 0x20, 0x20, 0x20, 0x20, 0x3C, 0x00,	// Char 076 (L)
	0x00, 0x22, 0x36, 0x2A, 0x22, 0x22, 0x22, 0x00,	// Char 077 (M)
	0x00, 0x24, 0x34, 0x2C, 0x24, 0x24, 0x24, 0x00,	// Char 078 (N)
	0x00, 0x18, 0x24, 0x24, 0x24, 0x24, 0x18, 0x00,	// Char 079 (O)
	0x00, 0x38, 0x24, 0x38, 0x20, 0x20, 0x20, 0x00,	// Char 080 (P)
	0x00, 0x18, 0x24, 0x24, 0x24, 0x24, 0x1C, 0x02,	// Char 081 (Q)
	0x00, 0x38, 0x24, 0x38, 0x30, 0x28, 0x24, 0x00,	// Char 082 (R)
	0x00, 0x1C, 0x20, 0x18, 0x04, 0x04, 0x38, 0x00,	// Char 083 (S)
	0x00, 0x7C, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00,	// Char 084 (T)
	0x00, 0x24, 0x24, 0x24, 0x24, 0x24, 0x18, 0x00,	// Char 085 (U)
	0x00, 0x24, 0x24, 0x24, 0x24, 0x14, 0x0C, 0x00,	// Char 086 (V)
	0x00, 0x44, 0x44, 0x44, 0x54, 0x6C, 0x44, 0x00,	// Char 087 (W)
	0x00, 0x24, 0x24, 0x18, 0x18, 0x24, 0x24, 0x00,	// Char 088 (X)
	0x00, 0x22, 0x22, 0x14, 0x08, 0x08, 0x08, 0x00,	// Char 089 (Y)
	0x00, 0x3C, 0x08, 0x10, 0x20, 0x20, 0x3C, 0x00,	// Char 090 (Z)
	0x00, 0x38, 0x20, 0x20, 0x20, 0x20, 0x38, 0x00,	// Char 091 ([)
	0x00, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x00,	// Char 092 (\)
	0x00, 0x1C, 0x04, 0x04, 0x04, 0x04, 0x1C, 0x00,	// Char 093 (])
	0x00, 0x08, 0x14, 0x22, 0x00, 0x00, 0x00, 0x00,	// Char 094 (^)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x00,	// Char 095 (_)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 096 (`)
	0x00, 0x00, 0x18, 0x04, 0x1C, 0x24, 0x1C, 0x00,	// Char 097 (a)
	0x00, 0x20, 0x20, 0x20, 0x38, 0x24, 0x38, 0x00,	// Char 098 (b)
	0x00, 0x00, 0x00, 0x1C, 0x20, 0x20, 0x1C, 0x00,	// Char 099 (c)
	0x00, 0x04, 0x04, 0x04, 0x1C, 0x24, 0x1C, 0x00,	// Char 100 (d)
	0x00, 0x00, 0x18, 0x24, 0x3C, 0x20, 0x18, 0x00,	// Char 101 (e)
	0x00, 0x0C, 0x10, 0x18, 0x10, 0x10, 0x10, 0x00,	// Char 102 (f)
	0x00, 0x00, 0x1C, 0x24, 0x24, 0x1C, 0x04, 0x38,	// Char 103 (g)
	0x00, 0x20, 0x20, 0x38, 0x24, 0x24, 0x24, 0x00,	// Char 104 (h)
	0x00, 0x08, 0x00, 0x18, 0x08, 0x08, 0x1C, 0x00,	// Char 105 (i)
	0x00, 0x08, 0x00, 0x08, 0x08, 0x08, 0x28, 0x10,	// Char 106 (j)
	0x00, 0x20, 0x20, 0x28, 0x30, 0x28, 0x24, 0x00,	// Char 107 (k)
	0x00, 0x20, 0x20, 0x20, 0x20, 0x28, 0x10, 0x00,	// Char 108 (l)
	0x00, 0x00, 0x00, 0x34, 0x2A, 0x2A, 0x2A, 0x00,	// Char 109 (m)
	0x00, 0x00, 0x00, 0x38, 0x24, 0x24, 0x24, 0x00,	// Char 110 (n)
	0x00, 0x00, 0x00, 0x18, 0x24, 0x24, 0x18, 0x00,	// Char 111 (o)
	0x00, 0x00, 0x00, 0x38, 0x24, 0x38, 0x20, 0x20,	// Char 112 (p)
	0x00, 0x00, 0x00, 0x1C, 0x24, 0x1C, 0x04, 0x06,	// Char 113 (q)
	0x00, 0x00, 0x00, 0x18, 0x20, 0x20, 0x20, 0x00,	// Char 114 (r)
	0x00, 0x00, 0x18, 0x20, 0x18, 0x04, 0x38, 0x00,	// Char 115 (s)
	0x00, 0x10, 0x38, 0x10, 0x10, 0x10, 0x08, 0x00,	// Char 116 (t)
	0x00, 0x00, 0x00, 0x24, 0x24, 0x24, 0x18, 0x00,	// Char 117 (u)
	0x00, 0x00, 0x00, 0x22, 0x14, 0x14, 0x08, 0x00,	// Char 118 (v)
	0x00, 0x00, 0x00, 0x2A, 0x2A, 0x2A, 0x14, 0x00,	// Char 119 (w)
	0x00, 0x00, 0x00, 0x24, 0x18, 0x18, 0x24, 0x00,	// Char 120 (x)
	0x00, 0x00, 0x00, 0x24, 0x24, 0x1C, 0x04, 0x38,	// Char 121 (y)
	0x00, 0x00, 0x00, 0x3C, 0x08, 0x10, 0x3C, 0x00,	// Char 122 (z)
	0x00, 0x08, 0x10, 0x10, 0x20, 0x10, 0x10, 0x08,	// Char 123 ({)
	0x00, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x00,	// Char 124 (|)
	0x00, 0x10, 0x08, 0x08, 0x04, 0x08, 0x08, 0x10,	// Char 125 (})
	0x00, 0x00, 0x00, 0x22, 0x54, 0x08, 0x00, 0x00,	// Char 126 (~)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 127 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 128 (.)
	0x00, 0x24, 0x00, 0x24, 0x24, 0x24, 0x18, 0x00,	// Char 129 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 130 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 131 (.)
	0x14, 0x00, 0x18, 0x04, 0x1C, 0x24, 0x1C, 0x00,	// Char 132 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 133 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 134 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 135 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 136 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 137 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 138 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 139 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 140 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 141 (.)
	0x24, 0x18, 0x24, 0x24, 0x3C, 0x24, 0x24, 0x00,	// Char 142 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 143 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 144 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 145 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 146 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 147 (.)
	0x00, 0x24, 0x00, 0x18, 0x24, 0x24, 0x18, 0x00,	// Char 148 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 149 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 150 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 151 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 152 (.)
	0x24, 0x18, 0x24, 0x24, 0x24, 0x24, 0x18, 0x00,	// Char 153 (.)
	0x24, 0x00, 0x24, 0x24, 0x24, 0x24, 0x18, 0x00,	// Char 154 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 155 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 156 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 157 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 158 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 159 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 160 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 161 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 162 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 163 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 164 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 165 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 166 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 167 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 168 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 169 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 170 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 171 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 172 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 173 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 174 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 175 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 176 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 177 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 178 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 179 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 180 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 181 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 182 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 183 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 184 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 185 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 186 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 187 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 188 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 189 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 190 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 191 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 192 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 193 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 194 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 195 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 196 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 197 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 198 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 199 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 200 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 201 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 202 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 203 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 204 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 205 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 206 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 207 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 208 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 209 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 210 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 211 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 212 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 213 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 214 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 215 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 216 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 217 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 218 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 219 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 220 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 221 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 222 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 223 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 224 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 225 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 226 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 227 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 228 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 229 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 230 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 231 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 232 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 233 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 234 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 235 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 236 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 237 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 238 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 239 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 240 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 241 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 242 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 243 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 244 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 245 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 246 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 247 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 248 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 249 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 250 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 251 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 252 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 253 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 254 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00	// Char 255 (.)
};

uint32_t screen_width;
uint32_t screen_height;

uint32_t window_height;
uint32_t window_width;
uint32_t window_offsetX;
uint32_t window_offsetY;

uint32_t currentRowStart;
uint32_t currentRow;
uint32_t currentLine;
uint32_t currentLineStart;
uint8_t scalingFactor;

uint8_t* framebuff;
long buffSize;

void setScale(uint8_t scale) {
	scalingFactor = scale;
}

void
drawCharacterWithColor(char c, uint32_t color) {

	if(c == '\n') {
		currentLine += 8 * scalingFactor;
		currentLineStart = currentLine;
		currentRow = 0;
		currentRowStart = 0;
		return;
	}

	uint16_t lineBitmap;
	uint32_t framebuffPos = 0;

	uint32_t i;
	uint32_t j;
	uint32_t k;

	if(currentRow + 8*scalingFactor > screen_width * 3) {
		drawCharacter('\n');
	}


	if(currentLine + 8 * scalingFactor > screen_height) {
		currentLine -= 8 * scalingFactor;
		currentLineStart = currentLine;
		memcpy(framebuff, &framebuff[screen_width * 3 * scalingFactor * 8], buffSize - screen_width * 3 * scalingFactor * 8);
		//memset(&framebuff[(960-(8 * scalingFactor))*window_width*3], '0', window_width * 8 * 3 * scalingFactor);
	}

	for(i = 0; i < 8; i++) {
		lineBitmap = font[c*8+i];

		for(k = 0; k < 8; k++) {
			uint32_t temp = (lineBitmap >> (7-k)) & 0x01;

			if(temp == 1) {
				framebuffPos = currentLine * screen_width * 3 + currentRow;
				for(j = 0; j < 3 * scalingFactor; j += 3) {
					framebuff[framebuffPos + j] = (char) (color & 0xFF);
					framebuff[framebuffPos + j + 1] = (char) ((color >> 8) & 0xFF);
					framebuff[framebuffPos + j + 2] = (char) ((color >> 16) & 0xFF);
				}
			}

			currentRow += 3 * scalingFactor;
		}

		uint32_t tempLine = currentLine;
		for(j = 1; j < scalingFactor; j++) {
			++currentLine;
			memcpy(&framebuff[currentLine * screen_width * 3], &framebuff[tempLine * screen_width * 3], screen_width * 3);
		}

		++currentLine;
		currentRow = currentRowStart;

	}

	currentLine = currentLineStart;
	currentRowStart += 3 * scalingFactor * 8;
	currentRow = currentRowStart;

}

void
drawCharacter(char c) {
	drawCharacterWithColor(c, 0xFFFFFF);
}

void
drawHorizontalLine(uint32_t offset, uint32_t length) {

	uint32_t i;
	uint32_t lengthRGB = length * 3;
	uint32_t startAdress = offset * screen_width * 3;
	for(i = 0; i < lengthRGB; i += 3) {
		framebuff[startAdress + i] = (char) 255;
		framebuff[startAdress + i + 1] = (char) 255;
		framebuff[startAdress + i + 2] = (char) 255;
	}
}

void
drawVerticalLine(uint32_t offset, uint32_t length) {
	uint32_t i;
	uint32_t j;

	for(j = 0; j < 15; j += 3) {
		for(i = 0; i < length; i++) {
			uint32_t startAdress = i*screen_width * 3 + offset * 3;
			framebuff[startAdress + j] = (char) 255;
			framebuff[startAdress + 1 + j] = (char) 255;
			framebuff[startAdress + 2 + j] = (char) 255;
		}

	}
}

void
drawStringWithColor( const char *s, uint32_t color) {
	uint32_t i = 0;
	while(s[i] != '\0') {
		drawCharacterWithColor(s[i], color);
		++i;
	}
}

void
setWindowSize(uint32_t width, uint32_t height) {
	window_height = height;
	window_width = width;
	reg32w(DSPC_BASE, DSPC_GFX_SIZE, ((window_height -1) << 16) | ( window_width -1));
}

void
setWindowOffset(uint32_t offsetX, uint32_t offsetY) {
	window_offsetX = offsetX;
	window_offsetY = offsetY;
	reg32w(DSPC_BASE, DSPC_GFX_POS, (window_offsetY << 16) | window_offsetX);
}

void
attachFrameBuffer(uint8_t* fb) {
	framebuff = fb;
	reg32w(DSPC_BASE, DSPC_GFX_BA1, (uint32_t) framebuff);
}

void
writeHDMI( const char* str ) {
	drawStringWithColor( str, 0xFFFFFF);
}

uint32_t
openHDMI( void )
{
	screen_height = 960;
	screen_width = 1024;
	scalingFactor = 1;

	window_height = screen_height;
	window_width = screen_width;
	window_offsetX = 0;
	window_offsetY = 0;

	currentRow = 0;
	currentLine = 0;
	currentRowStart = 0;

	buffSize = screen_height * screen_width * 3;

	reg32w(DSPC_BASE, DSPC_DEFAULT_COLOR_0, 0x000000);
	reg32w(DSPC_BASE, DSPC_SIZE_LCD, (screen_height - 1) << 16 | (screen_width - 1));
	reg32w(DSPC_BASE, DSPC_TIMING_H, 0x0cf03f31);
	reg32w(DSPC_BASE, DSPC_TIMING_V, 0x01400504);

	// NOTE: MALLOC DOESNT WORK FOR NOW - will run into a prefetch abort!
	framebuff = malloc(buffSize);
	memset(framebuff, 0, buffSize);

	reg32w(DSPC_BASE, DSPC_GFX_BA1, (uint32_t) framebuff);
	reg32w(DSPC_BASE, DSPC_GFX_SIZE, ((window_height -1) << 16) | ( window_width -1));
	reg32w(DSPC_BASE, DSPC_GFX_POS, (window_offsetY << 16) | window_offsetX);
	reg32w(DSPC_BASE, DSPC_GFX_ATTRIBUTES, (0x0 << 8) | (0x2 << 6) | (0x9 << 1) | 0x1);
	reg32w(DSPC_BASE, DSPC_CONTROL, reg32r(DSPC_BASE, DSPC_CONTROL) | 0x01 << 5);

	return 0;
}
