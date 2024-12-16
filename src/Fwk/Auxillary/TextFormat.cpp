/*===========================================================================*\
 *  DKC Level Builder Toolkit
 *  Copyright (C) 2025 Simion32
 *
 *  This file is part of the DKC Level Builder Toolkit (DKCLB).
 *
 *  DKCLB is free software: you can redistribute it and/or modify it under the
 *  terms of the GNU General Public License as published by the Free Software 
 *  Foundation, either version 3 of the License, or (at your option) any later 
 *  version.
 *
 *  DKCLB is distributed in the hope that it will be useful, but WITHOUT ANY 
 *  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS 
 *  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along 
 *  with DKCLB. If not, see <https://www.gnu.org/licenses/>. 
**===========================================================================*/
#include <stdarg.h> //Veradic Functions
#include "include.h"
#include "Hex128.h"

#define getchi(x) (INT(fmt[(x)])&0x7F)

TXT __real_ngui_textf(TXT fmt, va_list& ap)
{
    TXT dstr  = "";
    size_t index = 0;
    INT    chi   = 0;
    while(true)
    {
        chi = getchi(index); index++;
        if(chi == 0x00) break;
        if(chi == '%')
        {
        	chi = getchi(index); index++;
        	switch(chi)
            {
            	default://unknown switch, ignore and do a % character
            	case '%':{//% character
                    dstr += '%';
                }break;
                case 's':{//(STR)
                    dstr += va_arg(ap, STR);
                }break;
				case 'S':{//(TXT&)
                    dstr += (*va_arg(ap, TXT*));
                }break;
                case 'x':case 'u':case 'i':case 'f':case 'd':case 'p':
                case '+':case '_':case '-':case '.':case ',':
                case '0':case '1':case '2':case '3':case '4':
                case '5':case '6':case '7':case '8':case '9':
                {
                	BIT extra = false, plusA = false, 
                	     zeroA = false, hasdecimalpoint = false, 
                	     leadingspacesA = false, nosign = false, delimiters = false;
                	INT fmtINTA = 0, fmtINTB = 0;
                	if((chi!='x')
					&& (chi!='u')
					&& (chi!='i')
					&& (chi!='f')
					&& (chi!='d')
					&& (chi!='p'))
					{
						BIT sign = false;
						extra = true;
						plusA |= (chi == '+');
						leadingspacesA |= (chi == '_');
						sign |= ((chi == '-') || (chi == '+'));
						delimiters |= (chi == ',');
						if((chi == '+') || (chi == '_') || (chi == '-') || (chi == ',')){
							chi = getchi(index); index++;
						}
						plusA |= (chi == '+');
						leadingspacesA |= (chi == '_');
						sign |= ((chi == '-') || (chi == '+'));
						delimiters |= (chi == ',');
						if((chi == '+') || (chi == '_') || (chi == '-') || (chi == ',')){
							chi = getchi(index); index++;
						}
						plusA |= (chi == '+');
						leadingspacesA |= (chi == '_');
						sign |= ((chi == '-') || (chi == '+'));
						delimiters |= (chi == ',');
						if((chi == '+') || (chi == '_') || (chi == '-') || (chi == ',')){
							chi = getchi(index); index++;
						}
						plusA |= (chi == '+');
						leadingspacesA |= (chi == '_');
						sign |= ((chi == '-') || (chi == '+'));
						delimiters |= (chi == ',');
						if((chi == '+') || (chi == '_') || (chi == '-') || (chi == ',')){
							chi = getchi(index); index++;
						}
						nosign = !sign;
						hasdecimalpoint = (chi == '.');
						if(!hasdecimalpoint)
						{//only check for beginning numbers if point not specified!
							zeroA = (chi == '0');
							while((chi>='0')&&(chi<='9')){
								fmtINTA = (fmtINTA * 10) + (chi - 0x30);
								chi = getchi(index); index++;
							}//exits with chi==(not a number character)
							hasdecimalpoint = (chi == '.');
						}
						if(hasdecimalpoint)
						{
							chi = getchi(index); index++;
							while((chi>='0')&&(chi<='9')){
								fmtINTB = (fmtINTB * 10) + (chi - 0x30);
								chi = getchi(index); index++;
							}//exits with chi==(not a number character)
						}
                	}
                	switch(chi)
					{
						case 'x':{//prINT an INTeger in hex format (capitalized only, always has leading 0s)
							static STR hex = "0123456789ABCDEF"; //! TRUSTED THIS ARRAY.
							INT drawstart = (fmtINTA+fmtINTB);
							chi = getchi(index); index++;
							switch(chi)
							{
								case 'b':
								case 'c':
								case 'h':{//half-word (8bit)
									if((fmtINTA+fmtINTB) == 0){fmtINTA = 2; fmtINTB = 0; hasdecimalpoint = false;}
									U08 value = (U08)va_arg(ap, INT);
									if(extra) drawstart = (2-drawstart); else drawstart = 0;
									if(hasdecimalpoint && (fmtINTB == 2)) dstr += '.';
									if(drawstart <= 0) dstr += hex[(value >> 4) & 0x0F];
									if(hasdecimalpoint && (fmtINTB == 1)) dstr += '.';
									if(drawstart <= 1) dstr += hex[value & 0x0F];
									if(hasdecimalpoint && (fmtINTB == 0)) dstr += '.';
								}break;
								case 'w':{//word (16bit)
									if((fmtINTA+fmtINTB) == 0){fmtINTA = 4; fmtINTB = 0; hasdecimalpoint = false;}
									U16 value = (U16)va_arg(ap, INT);
									if(extra) drawstart = (4-drawstart); else drawstart = 0;
									if(hasdecimalpoint && (fmtINTB == 4)) dstr += '.';
									if(drawstart <= 0) dstr += hex[(value >> 12) & 0x0F];
									if(hasdecimalpoint && (fmtINTB == 3)) dstr += '.';
									if(drawstart <= 1) dstr += hex[(value >> 8) & 0x0F];
									if(hasdecimalpoint && (fmtINTB == 2)) dstr += '.';
									if(drawstart <= 2) dstr += hex[(value >> 4) & 0x0F];
									if(hasdecimalpoint && (fmtINTB == 1)) dstr += '.';
									if(drawstart <= 3) dstr += hex[value & 0x0F];
									if(hasdecimalpoint && (fmtINTB == 0)) dstr += '.';
								}break;
								default://MUST UNDO CHARACTER GET
									index--; chi = 'x';
								case 'd':{//double-word (32bit)
									if((fmtINTA+fmtINTB) == 0){fmtINTA = 8; fmtINTB = 0; hasdecimalpoint = false;}
									U32 value = (U32)va_arg(ap, INT);
									if(extra) drawstart = (8-drawstart); else drawstart = 0;
									if(hasdecimalpoint && (fmtINTB == 8)) dstr += '.';
									if(drawstart <= 0) dstr += hex[(value >> 28) & 0x0F];
									if(hasdecimalpoint && (fmtINTB == 7)) dstr += '.';
									if(drawstart <= 1) dstr += hex[(value >> 24) & 0x0F];
									if(hasdecimalpoint && (fmtINTB == 6)) dstr += '.';
									if(drawstart <= 2) dstr += hex[(value >> 20) & 0x0F];
									if(hasdecimalpoint && (fmtINTB == 5)) dstr += '.';
									if(drawstart <= 3) dstr += hex[(value >> 16) & 0x0F];
									if(hasdecimalpoint && (fmtINTB == 4)) dstr += '.';
									if(drawstart <= 4) dstr += hex[(value >> 12) & 0x0F];
									if(hasdecimalpoint && (fmtINTB == 3)) dstr += '.';
									if(drawstart <= 5) dstr += hex[(value >> 8) & 0x0F];
									if(hasdecimalpoint && (fmtINTB == 2)) dstr += '.';
									if(drawstart <= 6) dstr += hex[(value >> 4) & 0x0F];
									if(hasdecimalpoint && (fmtINTB == 1)) dstr += '.';
									if(drawstart <= 7) dstr += hex[value & 0x0F];
									if(hasdecimalpoint && (fmtINTB == 0)) dstr += '.';
								}break;
								case 'q':{//quad-word (64bit)
									if((fmtINTA+fmtINTB) == 0){fmtINTA = 16; fmtINTB = 0; hasdecimalpoint = false;}
									U64 value = va_arg(ap, U64);
									U32 value1 = U32(value >> 32);
									U32 value0 = U32(value);
									if(extra) drawstart = (16-drawstart); else drawstart = 0;
									if(hasdecimalpoint && (fmtINTB == 16)) dstr += '.';
									if(drawstart <= 0) dstr += hex[(value1 >> 28) & 0x0F];
									if(hasdecimalpoint && (fmtINTB == 15)) dstr += '.';
									if(drawstart <= 1) dstr += hex[(value1 >> 24) & 0x0F];
									if(hasdecimalpoint && (fmtINTB == 14)) dstr += '.';
									if(drawstart <= 2) dstr += hex[(value1 >> 20) & 0x0F];
									if(hasdecimalpoint && (fmtINTB == 13)) dstr += '.';
									if(drawstart <= 3) dstr += hex[(value1 >> 16) & 0x0F];
									if(hasdecimalpoint && (fmtINTB == 12)) dstr += '.';
									if(drawstart <= 4) dstr += hex[(value1 >> 12) & 0x0F];
									if(hasdecimalpoint && (fmtINTB == 11)) dstr += '.';
									if(drawstart <= 5) dstr += hex[(value1 >> 8) & 0x0F];
									if(hasdecimalpoint && (fmtINTB == 10)) dstr += '.';
									if(drawstart <= 6) dstr += hex[(value1 >> 4) & 0x0F];
									if(hasdecimalpoint && (fmtINTB == 9)) dstr += '.';
									if(drawstart <= 7) dstr += hex[value1 & 0x0F];
									if(hasdecimalpoint && (fmtINTB == 8)) dstr += '.';
									if(drawstart <= 8) dstr += hex[(value0 >> 28) & 0x0F];
									if(hasdecimalpoint && (fmtINTB == 7)) dstr += '.';
									if(drawstart <= 9) dstr += hex[(value0 >> 24) & 0x0F];
									if(hasdecimalpoint && (fmtINTB == 6)) dstr += '.';
									if(drawstart <= 10) dstr += hex[(value0 >> 20) & 0x0F];
									if(hasdecimalpoint && (fmtINTB == 5)) dstr += '.';
									if(drawstart <= 11) dstr += hex[(value0 >> 16) & 0x0F];
									if(hasdecimalpoint && (fmtINTB == 4)) dstr += '.';
									if(drawstart <= 12) dstr += hex[(value0 >> 12) & 0x0F];
									if(hasdecimalpoint && (fmtINTB == 3)) dstr += '.';
									if(drawstart <= 13) dstr += hex[(value0 >> 8) & 0x0F];
									if(hasdecimalpoint && (fmtINTB == 2)) dstr += '.';
									if(drawstart <= 14) dstr += hex[(value0 >> 4) & 0x0F];
									if(hasdecimalpoint && (fmtINTB == 1)) dstr += '.';
									if(drawstart <= 15) dstr += hex[value0 & 0x0F];
									if(hasdecimalpoint && (fmtINTB == 0)) dstr += '.';
								}break;
								case 'o':{//octo-word (128bit)
									if((fmtINTA+fmtINTB) == 0){fmtINTA = 32; fmtINTB = 0; hasdecimalpoint = false;}
									Hex128 value = *va_arg(ap, Hex128*);
									U32 value3 = value.dw3;
									U32 value2 = value.dw2;
									U32 value1 = value.dw1;
									U32 value0 = value.dw0;
									if(extra) drawstart = (32-drawstart); else drawstart = 0;
									if(hasdecimalpoint && (fmtINTB == 32)) dstr += '.';
									if(drawstart <= 0) dstr += hex[(value3 >> 28) & 0x0F];
									if(hasdecimalpoint && (fmtINTB == 31)) dstr += '.';
									if(drawstart <= 1) dstr += hex[(value3 >> 24) & 0x0F];
									if(hasdecimalpoint && (fmtINTB == 30)) dstr += '.';
									if(drawstart <= 2) dstr += hex[(value3 >> 20) & 0x0F];
									if(hasdecimalpoint && (fmtINTB == 29)) dstr += '.';
									if(drawstart <= 3) dstr += hex[(value3 >> 16) & 0x0F];
									if(hasdecimalpoint && (fmtINTB == 28)) dstr += '.';
									if(drawstart <= 4) dstr += hex[(value3 >> 12) & 0x0F];
									if(hasdecimalpoint && (fmtINTB == 27)) dstr += '.';
									if(drawstart <= 5) dstr += hex[(value3 >> 8) & 0x0F];
									if(hasdecimalpoint && (fmtINTB == 26)) dstr += '.';
									if(drawstart <= 6) dstr += hex[(value3 >> 4) & 0x0F];
									if(hasdecimalpoint && (fmtINTB == 25)) dstr += '.';
									if(drawstart <= 7) dstr += hex[value3 & 0x0F];
									if(hasdecimalpoint && (fmtINTB == 24)) dstr += '.';
									if(drawstart <= 8) dstr += hex[(value2 >> 28) & 0x0F];
									if(hasdecimalpoint && (fmtINTB == 23)) dstr += '.';
									if(drawstart <= 9) dstr += hex[(value2 >> 24) & 0x0F];
									if(hasdecimalpoint && (fmtINTB == 22)) dstr += '.';
									if(drawstart <= 10) dstr += hex[(value2 >> 20) & 0x0F];
									if(hasdecimalpoint && (fmtINTB == 21)) dstr += '.';
									if(drawstart <= 11) dstr += hex[(value2 >> 16) & 0x0F];
									if(hasdecimalpoint && (fmtINTB == 20)) dstr += '.';
									if(drawstart <= 12) dstr += hex[(value2 >> 12) & 0x0F];
									if(hasdecimalpoint && (fmtINTB == 19)) dstr += '.';
									if(drawstart <= 13) dstr += hex[(value2 >> 8) & 0x0F];
									if(hasdecimalpoint && (fmtINTB == 18)) dstr += '.';
									if(drawstart <= 14) dstr += hex[(value2 >> 4) & 0x0F];
									if(hasdecimalpoint && (fmtINTB == 17)) dstr += '.';
									if(drawstart <= 15) dstr += hex[value2 & 0x0F];
									if(hasdecimalpoint && (fmtINTB == 16)) dstr += '.';
									if(drawstart <= 16) dstr += hex[(value1 >> 28) & 0x0F];
									if(hasdecimalpoint && (fmtINTB == 15)) dstr += '.';
									if(drawstart <= 17) dstr += hex[(value1 >> 24) & 0x0F];
									if(hasdecimalpoint && (fmtINTB == 14)) dstr += '.';
									if(drawstart <= 18) dstr += hex[(value1 >> 20) & 0x0F];
									if(hasdecimalpoint && (fmtINTB == 13)) dstr += '.';
									if(drawstart <= 19) dstr += hex[(value1 >> 16) & 0x0F];
									if(hasdecimalpoint && (fmtINTB == 12)) dstr += '.';
									if(drawstart <= 20) dstr += hex[(value1 >> 12) & 0x0F];
									if(hasdecimalpoint && (fmtINTB == 11)) dstr += '.';
									if(drawstart <= 21) dstr += hex[(value1 >> 8) & 0x0F];
									if(hasdecimalpoint && (fmtINTB == 10)) dstr += '.';
									if(drawstart <= 22) dstr += hex[(value1 >> 4) & 0x0F];
									if(hasdecimalpoint && (fmtINTB == 9)) dstr += '.';
									if(drawstart <= 23) dstr += hex[value1 & 0x0F];
									if(hasdecimalpoint && (fmtINTB == 8)) dstr += '.';
									if(drawstart <= 24) dstr += hex[(value0 >> 28) & 0x0F];
									if(hasdecimalpoint && (fmtINTB == 7)) dstr += '.';
									if(drawstart <= 25) dstr += hex[(value0 >> 24) & 0x0F];
									if(hasdecimalpoint && (fmtINTB == 6)) dstr += '.';
									if(drawstart <= 26) dstr += hex[(value0 >> 20) & 0x0F];
									if(hasdecimalpoint && (fmtINTB == 5)) dstr += '.';
									if(drawstart <= 27) dstr += hex[(value0 >> 16) & 0x0F];
									if(hasdecimalpoint && (fmtINTB == 4)) dstr += '.';
									if(drawstart <= 28) dstr += hex[(value0 >> 12) & 0x0F];
									if(hasdecimalpoint && (fmtINTB == 3)) dstr += '.';
									if(drawstart <= 29) dstr += hex[(value0 >> 8) & 0x0F];
									if(hasdecimalpoint && (fmtINTB == 2)) dstr += '.';
									if(drawstart <= 30) dstr += hex[(value0 >> 4) & 0x0F];
									if(hasdecimalpoint && (fmtINTB == 1)) dstr += '.';
									if(drawstart <= 31) dstr += hex[value0 & 0x0F];
									if(hasdecimalpoint && (fmtINTB == 0)) dstr += '.';
								}break;
							}
						}break;
						case 'u':{//prINT an U32eger
							static Vx<CHR> render_area(128,'\0');
							chi = getchi(index); index++;
							switch(chi)
							{
								case 'b':
								case 'c':
								case 'h':{//half-word (8bit)
									U08 value = (U08)va_arg(ap, INT);
									INT i = 127, cc = 0;
									for(IND j = ((fmtINTA)?fmtINTA:3); ((i >= 0) && ((value != 0) || (zeroA || leadingspacesA)) && (j > 0)); --i, --j)
									{
										 if(delimiters && ((cc != 0) && ((cc % 3) == 0))){
											if((value == 0) && !zeroA){
												if((i >= 2) && leadingspacesA){
													render_area[i] = ' '; i--;
												}
											}else{
												if((i >= 1)){
													render_area[i] = ','; i--;
										}}}
										render_area[i] = ((leadingspacesA && (value == 0) && cc) ? ' ' : ((CHR)((value % 10)+'0')));
										value /= 10;
										cc++;
									}
									i++;
									for(; i < 128; ++i){if(render_area[i]!=' '){dstr += render_area[i];}else{dstr += ' ';}}
								}break;
								case 'w':{//word (16bit)
									U16 value = (U16)va_arg(ap, INT);
									INT i = 127, cc = 0;
									for(IND j = ((fmtINTA)?fmtINTA:5); ((i >= 0) && ((value != 0) || (zeroA || leadingspacesA)) && (j > 0)); --i, --j)
									{
										 if(delimiters && ((cc != 0) && ((cc % 3) == 0))){
											if((value == 0) && !zeroA){
												if((i >= 2) && leadingspacesA){
													render_area[i] = ' '; i--;
												}
											}else{
												if((i >= 1)){
													render_area[i] = ','; i--;
										}}}
										render_area[i] = ((leadingspacesA && (value == 0) && cc) ? ' ' : ((CHR)((value % 10)+'0')));
										value /= 10;
										cc++;
									}
									i++;
									for(; i < 128; ++i){if(render_area[i]!=' '){dstr += render_area[i];}else{dstr += ' ';}}
								}break;
								default://MUST UNDO CHARACTER GET
									index--; chi = 'u';
								case 'd':{//double-word (32bit)
									U32 value = (U32)va_arg(ap, INT);
									INT i = 127, cc = 0;
									for(IND j = ((fmtINTA)?fmtINTA:10); ((i >= 0) && ((value != 0) || (zeroA || leadingspacesA)) && (j > 0)); --i, --j)
									{
										 if(delimiters && ((cc != 0) && ((cc % 3) == 0))){
											if((value == 0) && !zeroA){
												if((i >= 2) && leadingspacesA){
													render_area[i] = ' '; i--;
												}
											}else{
												if((i >= 1)){
													render_area[i] = ','; i--;
										}}}
										render_area[i] = ((leadingspacesA && (value == 0) && cc) ? ' ' : ((CHR)((value % 10)+'0')));
										value /= 10;
										cc++;
									}
									i++;
									for(; i < 128; ++i){if(render_area[i]!=' '){dstr += render_area[i];}else{dstr += ' ';}}
								}break;
								case 'q':{//quad-word (64bit)
									U64 value = va_arg(ap, U64);
									INT i = 127, cc = 0;
									for(IND j = ((fmtINTA)?fmtINTA:20); ((i >= 0) && ((value != 0) || (zeroA || leadingspacesA)) && (j > 0)); --i, --j)
									{
										 if(delimiters && ((cc != 0) && ((cc % 3) == 0))){
											if((value == 0) && !zeroA){
												if((i >= 2) && leadingspacesA){
													render_area[i] = ' '; i--;
												}
											}else{
												if((i >= 1)){
													render_area[i] = ','; i--;
										}}}
										render_area[i] = ((leadingspacesA && (value == 0) && cc) ? ' ' : ((CHR)((value % 10)+'0')));
										value /= 10;
										cc++;
									}
									i++;
									for(; i < 128; ++i){if(render_area[i]!=' '){dstr += render_area[i];}else{dstr += ' ';}}
								}break;
								case 'o':{//octo-word (128bit)
									Hex128 value = *va_arg(ap, Hex128*);
									INT i = 127, cc = 0;
									for(IND j = ((fmtINTA)?fmtINTA:39); ((i >= 0) && ((value != 0) || (zeroA || leadingspacesA)) && (j > 0)); --i, --j)
									{
										 if(delimiters && ((cc != 0) && ((cc % 3) == 0))){
											if((value == 0) && !zeroA){
												if((i >= 2) && leadingspacesA){
													render_area[i] = ' '; i--;
												}
											}else{
												if((i >= 1)){
													render_area[i] = ','; i--;
										}}}
										render_area[i] = ((leadingspacesA && (value == 0) && cc) ? ' ' : ((CHR)(U32(value % U32(10))+'0')));
										value /= U32(10);
										cc++;
									}
									i++;
									for(; i < 128; ++i){if(render_area[i]!=' '){dstr += render_area[i];}else{dstr += ' ';}}
								}break;
							}
						}break;
						case 'i':{//prINT a signed INTeger
							static Vx<CHR> render_area(128,'\0');
							chi = getchi(index); index++;
							switch(chi)
							{
								case 'b':
								case 'c':
								case 'h':{//half-word (8bit)
									U08 value = (U08)va_arg(ap, INT);
									BIT issigned = (value & 0x80);
									if(issigned) value = -value;
									INT i = 127, cc = 0;
									for(IND j = ((fmtINTA)?fmtINTA:3); ((i >= 0) && ((value != 0) || (zeroA || leadingspacesA)) && (j > 0)); --i, --j)
									{
										render_area[i] = ((leadingspacesA && (value == 0) && cc) ? ' ' : ((CHR)((value % 10)+'0')));
										value /= 10;
										cc++;
									}
									if(!nosign){
										if(issigned){
											render_area[i] = '-';
										}else if(plusA){
											render_area[i] = '+';
										}else{
											render_area[i] = ' ';
										}
									}else{i++;}
									for(; i < 128; ++i){if(render_area[i]!=' '){dstr += render_area[i];}else{dstr += ' ';}}
								}break;
								case 'w':{//word (16bit)
									U16 value = (U16)va_arg(ap, INT);
									BIT issigned = (value & 0x8000);
									if(issigned) value = -value;
									INT i = 127, cc = 0;
									for(IND j = ((fmtINTA)?fmtINTA:5); ((i >= 0) && ((value != 0) || (zeroA || leadingspacesA)) && (j > 0)); --i, --j)
									{
										 if(delimiters && ((cc != 0) && ((cc % 3) == 0))){
											if((value == 0) && !zeroA){
												if((i >= 2) && leadingspacesA){
													render_area[i] = ' '; i--;
												}
											}else{
												if((i >= 1)){
													render_area[i] = ','; i--;
										}}}
										render_area[i] = ((leadingspacesA && (value == 0) && cc) ? ' ' : ((CHR)((value % 10)+'0')));
										value /= 10;
										cc++;
									}
									if(!nosign){
										if(issigned){
											render_area[i] = '-';
										}else if(plusA){
											render_area[i] = '+';
										}else{
											render_area[i] = ' ';
										}
									}else{i++;}
									for(; i < 128; ++i){if(render_area[i]!=' '){dstr += render_area[i];}else{dstr += ' ';}}
								}break;
								default://MUST UNDO CHARACTER GET
									index--; chi = 'i';
								case 'd':{//double-word (32bit)
									U32 value = va_arg(ap, U32);
									BIT issigned = (value & 0x80000000);
									if(issigned) value = -value;
									INT i = 127, cc = 0;
									for(IND j = ((fmtINTA)?fmtINTA:10); ((i >= 0) && ((value != 0) || (zeroA || leadingspacesA)) && (j > 0)); --i, --j)
									{
										 if(delimiters && ((cc != 0) && ((cc % 3) == 0))){
											if((value == 0) && !zeroA){
												if((i >= 2) && leadingspacesA){
													render_area[i] = ' '; i--;
												}
											}else{
												if((i >= 1)){
													render_area[i] = ','; i--;
										}}}
										render_area[i] = ((leadingspacesA && (value == 0) && cc) ? ' ' : ((CHR)((value % 10)+'0')));
										value /= 10;
										cc++;
									}
									if(!nosign){
										if(issigned){
											render_area[i] = '-';
										}else if(plusA){
											render_area[i] = '+';
										}else{
											render_area[i] = ' ';
										}
									}else{i++;}
									for(; i < 128; ++i){if(render_area[i]!=' '){dstr += render_area[i];}else{dstr += ' ';}}
								}break;
								case 'q':{//quad-word (64bit)
									U64 value = va_arg(ap, U64);
									BIT issigned = (U32(value >> 32) & 0x80000000);
									if(issigned) value = -value;
									INT i = 127, cc = 0;
									for(IND j = ((fmtINTA)?fmtINTA:20); ((i >= 0) && ((value != 0) || (zeroA || leadingspacesA)) && (j > 0)); --i, --j)
									{
										 if(delimiters && ((cc != 0) && ((cc % 3) == 0))){
											if((value == 0) && !zeroA){
												if((i >= 2) && leadingspacesA){
													render_area[i] = ' '; i--;
												}
											}else{
												if((i >= 1)){
													render_area[i] = ','; i--;
										}}}
										render_area[i] = ((leadingspacesA && (value == 0) && cc) ? ' ' : ((CHR)((value % 10)+'0')));
										value /= 10;
										cc++;
									}
									if(!nosign){
										if(issigned){
											render_area[i] = '-';
										}else if(plusA){
											render_area[i] = '+';
										}else{
											render_area[i] = ' ';
										}
									}else{i++;}
									for(; i < 128; ++i){if(render_area[i]!=' '){dstr += render_area[i];}else{dstr += ' ';}}
								}break;
								case 'o':{//octo-word (128bit)
									Hex128 value = *va_arg(ap, Hex128*);
									BIT issigned = (value.dw3 & 0x80000000);
									if(issigned) value = -value;
									INT i = 127, cc = 0;
									for(IND j = ((fmtINTA)?fmtINTA:39); ((i >= 0) && ((value != 0) || (zeroA || leadingspacesA)) && (j > 0)); --i, --j)
									{
										 if(delimiters && ((cc != 0) && ((cc % 3) == 0))){
											if((value == 0) && !zeroA){
												if((i >= 2) && leadingspacesA){
													render_area[i] = ' '; i--;
												}
											}else{
												if((i >= 1)){
													render_area[i] = ','; i--;
										}}}
										render_area[i] = ((leadingspacesA && (value == 0) && cc) ? ' ' : ((CHR)(U32(value % U32(10))+'0')));
										value /= U32(10);
										cc++;
									}
									if(!nosign){
										if(issigned){
											render_area[i] = '-';
										}else if(plusA){
											render_area[i] = '+';
										}else{
											render_area[i] = ' ';
										}
									}else{i++;}
									for(; i < 128; ++i){if(render_area[i]!=' '){dstr += render_area[i];}else{dstr += ' ';}}
								}break;
							}
						}break;
                        case 'f':
						case 'd':{//prINT a floating point double
							F64 value = va_arg(ap, F64);
							U64 fp_bits = *((U64*)&value);//hack - little endian-ness preserved thru U64
							static Vx<CHR> render_area(128,'\0');
							U32 sign = ((fp_bits >> 63) & 1);
							U32 expbits = (U32(fp_bits >> 52) & 0x7FF);
							S32 exp =  ((expbits - 1023) & 0x7FF);//actual exponent without bias
							exp = (signed(exp << 21) >> 21);///BUGFIX!!! MUST BE SIGNED FOR 0.5 TYPE NUMBERS.
							U64 mantissa = (fp_bits & 0x000FFFFFFFFFFFFFLL);
							///BIT is_SubNormal = ((sign == 1) && (expbits == ((1 - 1023) & 0x7FF)));
							BIT is_NotANumber = (expbits == 0x7FF);
							BIT is_inf = ((mantissa == 0LL) && is_NotANumber);
							///BUGFIX: Use exponent and high bit anyway if subnormal
							mantissa |= (U64(1) << 52); 
							///mantissa |= (U64(!is_SubNormal) << 52); //(BUG, REMOVED)
							///if(is_SubNormal) exp = 0; //(BUG, REMOVED) fix the exponent value to 0 if subnormal
							if((fp_bits & 0x7FFFFFFFFFFFFFFFLL) == 0)
							{
								//(Signed) Zero
								if(!nosign){
									if(sign){
										dstr += '-';
									}else if(plusA){
										dstr += '+';
									}else{
										dstr += ' ';
									}
								}
								INT i = 127;
								if((fmtINTB == 0) && (fmtINTA == 0)){fmtINTA = 3; fmtINTB = 2;}
								if(fmtINTA > 100) fmtINTA = 100;
								if(fmtINTB > 100) fmtINTB = 100;
								if(hasdecimalpoint)
								{
									render_area[i] = '.'; i--;
								}
								if(fmtINTA > 0){//prINT whole part
									INT cc = 0;
									for(IND j = 0; ((i >= 0) && (zeroA || leadingspacesA) && (j < fmtINTA)); --i, ++j)
									{
										 if(delimiters && ((cc != 0) && ((cc % 3) == 0))){
											if(!zeroA){
												if((i >= 2) && leadingspacesA){
													render_area[i] = ' '; i--;
												}
											}else{
												if((i >= 1)){
													render_area[i] = ','; i--;
										}}}
										render_area[i] = ((leadingspacesA && cc)?(' '):('0'));
										cc++;
									}
								}
								i++;
								if(hasdecimalpoint || (fmtINTA > 0)) for(; i < 128; ++i) dstr += render_area[i];
								if(fmtINTB > 0){//prINT fractional part
									for(IND j = 0; (j < fmtINTB); ++j)
									{
										dstr += '0';
									}
								}
								break;
							}
							if(is_inf){
								//INFINITY!
								if(!nosign){
									if(sign){
										dstr += '-';
									}else{
										dstr += '+';
									}
								}
								dstr += "INF";
								break;
							}
							if(is_NotANumber){
								//NaN
								dstr += "NaN";
								break;
							}
							//Normals and Subnormals:
							// ((-1)^signbit) x (2^(exponentbits-127)) x ((BIT_notSubnormal).mantissabits)
							if(!nosign){
								if(sign){
									dstr += '-';
								}else if(plusA){
									dstr += '+';
								}else{
									dstr += ' ';
								}
							}
							INT i = 127;
							if((fmtINTB == 0) && (fmtINTA == 0)){fmtINTA = 3; fmtINTB = 2;}
							if(fmtINTA > 100) fmtINTA = 100;
							if(fmtINTB > 100) fmtINTB = 100;
							if(hasdecimalpoint)
							{
								render_area[i] = '.'; i--;
							}
							if(fmtINTA > 0){//prINT whole part
								INT real_shift_ammount = (exp-52);
								Hex128 wide_mantissa(0LL,mantissa);
								Hex128 wide_zero(0,0,0,0);
								Hex128 wide_ten(0,0,0,10);
								if(real_shift_ammount >= 0)	wide_mantissa <<= real_shift_ammount;
								else						wide_mantissa >>= abs(real_shift_ammount);
								INT cc = 0;
								for(IND j = 0; ((i >= 0) && ((wide_mantissa != 0) || zeroA || leadingspacesA) && (j < fmtINTA)); --i, ++j)
								{
									 if(delimiters && ((cc != 0) && ((cc % 3) == 0))){
										if((wide_mantissa == wide_zero) && !zeroA){
											if((i >= 2) && leadingspacesA){
												render_area[i] = ' '; i--;
											}
										}else{
											if((i >= 1)){
												render_area[i] = ','; i--;
									}}}
									render_area[i] = ((leadingspacesA && (wide_mantissa == wide_zero) && cc)? 
													  (' '):((CHR)(U32(wide_mantissa % wide_ten)+'0')));
									wide_mantissa /= wide_ten;
									cc++;
								}
							}
							i++;
							if(hasdecimalpoint || (fmtINTA > 0)) for(; i < 128; ++i){if(render_area[i]!=' '){dstr += render_area[i];}else{dstr += ' ';}}
							if(fmtINTB > 0){//prINT fractional part
								//make whole value a fractional part then shift back by one hex digit
								//to get digits, continually multiply by 10 and grab top 4 bits as a decimal digit.
								INT real_shift_ammount = (128-4-52+exp);
								///BUGFIX: __volatile__ prevents compiler optimization from breaking decimal precision.
								///        Additionally, the extra casting makes the compiler not warn when compiled.
								Hex128 __volatile__ wide_fraction(0LL,mantissa);
								if(real_shift_ammount >= 0)	((Hex128&)wide_fraction) <<= real_shift_ammount;
								else						((Hex128&)wide_fraction) >>= abs(real_shift_ammount);
								for(IND j = 0; (j < fmtINTB); ++j)
								{
									wide_fraction.dw3 &= 0x0FFFFFFF;
									((Hex128&)wide_fraction) *= U32(10);
									dstr += (CHR)((((wide_fraction.dw3) >> 28) % 10)+'0');
								}
							}
						}break;
						//END NUMBER PRINTING SWITCH
					}
                }
                //END switch(chi)
            }
        }
        else
        {
        	dstr += (CHR)chi;
        }
        if(index >= fmt.size()){
            va_end(ap);
            return dstr;
        }
    }
    va_end(ap);
    return dstr;
}
TXT __ngui_textf(TXT fmt, ...)
{
	va_list ap;
    va_start(ap, fmt);
    TXT output = __real_ngui_textf(fmt,ap);
    va_end(ap);
    return output;
}
//##############################################################################
//##############################################################################
//##############################################################################
//##############################################################################
//##############################################################################
//##############################################################################
