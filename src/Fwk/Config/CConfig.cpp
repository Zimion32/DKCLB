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
#include "include.h"
#include "CConfig.h"
U08 CConfig::static_zero_ = 0x00; // KEEP THIS (see below)
CConfig::CConfig(TXT variable_name, U32 secondary_id):
variable_name_(variable_name), is_empty_(true), is_integer_data_(false), 
is_hexadecimal_data_(false), is_hexadecimal_data_64_(false), is_string_data_(false),
is_window_position_data_(false), secondary_id_(secondary_id), is_unedited_(false)
{
	// Nothing else.
}
U08& CConfig::operator[](INT i)
{
	if(i >= variable_name_.size()) return static_zero_; // KEEP THIS: USED FOR DATA BLOCK SORTING PURPOSES
	return *((U08*)(&variable_name_[i]));
}
U32 CConfig::GetDataSz() const
{
	return variable_name_.size();
}
void CConfig::Load(TXT str)
{
	string_data_ = str;
	is_string_data_ = true;
	is_unedited_ = true;
	if(TXT("") == str)
	{
		is_empty_ = true; return; // Cannot definitely be any other kind of data. It's Empty!
	}
	is_empty_ = false;
	if(string_data_.size() == 17)
	{
		BIT valid = true;
		for(IND i = 15; i >= 0; i--)
		{
			valid &= (((string_data_[i]>='0')&&(string_data_[i]<='9'))
					||((string_data_[i]>='A')&&(string_data_[i]<='F')));
		}
		is_window_position_data_ = (valid && ((string_data_[16]=='M')||(string_data_[16]=='W')));
	}
	if(string_data_.size())
	{
		BIT valid = !(string_data_.size() && ((string_data_[0]=='+') || (string_data_[0]=='-')));
		for(IND i = string_data_.size()-1; i >= 0; i--)
		{
			valid &= (((string_data_[i]>='0')&&(string_data_[i]<='9'))
					||((string_data_[i]>='A')&&(string_data_[i]<='F'))
					||((string_data_[i]>='a')&&(string_data_[i]<='f')));
		}
		if(valid)
		{
			INT size = string_data_.size();
			for(IND i = 0; i < size; i+=8)
			{
				if(((size - i) < 8) && ((size - i) != 0))
				{
					hexadecimal_data_.push_back(String.ToHex(string_data_.substr(i,(size - i))));
					continue;
				}
				hexadecimal_data_.push_back(String.ToHex(string_data_.substr(i,8)));
			}
			is_hexadecimal_data_ = true;
		}
		if(valid)
		{
			INT size = string_data_.size();
			for(IND i = 0; i < size; i+=16)
			{
				if(((size - i) < 16) && ((size - i) != 0))
				{
					hexadecimal_data_64_.push_back(String.ToHex(string_data_.substr(i,(size - i))));
					continue;
				}
				hexadecimal_data_64_.push_back(String.ToHex(string_data_.substr(i,16)));
			}
			is_hexadecimal_data_64_ = true;
		}
	}
	if(string_data_.size() && (string_data_.size() < 12))
	{
		BIT valid = !(string_data_.size() && ((string_data_[0]=='0')
		|| ((string_data_[0]>='A') && (string_data_[0]<='F'))
		|| ((string_data_[0]>='a') && (string_data_[0]<='f'))));
		for(IND i = string_data_.size()-1; i >= 0; i--)
		{
			valid &= (((string_data_[i]=='+')||(string_data_[i]=='-'))
					||((string_data_[i]>='0')&&(string_data_[i]<='9')));
		}
		if(valid)
		{
			integer_data_ = atoi(string_data_.c_str());
			is_integer_data_ = true;
		}
	}
}
void CConfig::Set(TXT str)
{
	string_data_ = str;
	is_string_data_ = true;
	//Now that we have set it explicitly, it can't be these other data kinds.
	integer_data_ = 0;
	hexadecimal_data_.clear();
	hexadecimal_data_64_.clear();
	is_hexadecimal_data_ = false;
	is_hexadecimal_data_64_ = false;
	is_integer_data_ = false;
	is_window_position_data_ = false;
	is_unedited_ = false;
	is_empty_ = false;
	is_empty_ |= (TXT("") == str);
}
void CConfig::Set(S32 value)
{
	integer_data_ = value;
	is_integer_data_ = true;
	//Now that we have set it explicitly, it can't be these other data kinds.
	string_data_ = "";
	hexadecimal_data_.clear();
	hexadecimal_data_64_.clear();
	is_string_data_ = false;
	is_hexadecimal_data_ = false;
	is_hexadecimal_data_64_ = false;
	is_window_position_data_ = false;
	is_unedited_ = false;
	is_empty_ = false;
}
void CConfig::Set(U64 value,INT index)
{
	if(index >= hexadecimal_data_64_.size()) hexadecimal_data_64_.resize(index+1,(U32)0x00000000);
	hexadecimal_data_64_[index] = value;
	is_hexadecimal_data_ = false;
	is_hexadecimal_data_64_ = true;
	//Now that we have set it explicitly, it can't be these other data kinds.
	string_data_ = "";
	integer_data_ = 0;
	is_string_data_ = false;
	is_integer_data_ = false;
	is_window_position_data_ = false;
	is_unedited_ = false;
	is_empty_ = false;
}
void CConfig::Set(S64 value,INT index)
{
	if(index >= hexadecimal_data_64_.size()) hexadecimal_data_64_.resize(index+1,(U32)0x00000000);
	hexadecimal_data_64_[index] = (U32)value;
	is_hexadecimal_data_ = false;
	is_hexadecimal_data_64_ = true;
	//Now that we have set it explicitly, it can't be these other data kinds.
	string_data_ = "";
	integer_data_ = 0;
	is_string_data_ = false;
	is_integer_data_ = false;
	is_window_position_data_ = false;
	is_unedited_ = false;
	is_empty_ = false;
}
void CConfig::Set(U32 value,INT index)
{
	if(index >= hexadecimal_data_.size()) hexadecimal_data_.resize(index+1,(U32)0x00000000);
	hexadecimal_data_[index] = value;
	is_hexadecimal_data_ = true;
	is_hexadecimal_data_64_ = false;
	//Now that we have set it explicitly, it can't be these other data kinds.
	string_data_ = "";
	integer_data_ = 0;
	is_string_data_ = false;
	is_integer_data_ = false;
	is_window_position_data_ = false;
	is_unedited_ = false;
	is_empty_ = false;
}
void CConfig::Set(S32 value,INT index)
{
	if(index >= hexadecimal_data_.size()) hexadecimal_data_.resize(index+1,(U32)0x00000000);
	hexadecimal_data_[index] = (U32)value;
	is_hexadecimal_data_ = true;
	is_hexadecimal_data_64_ = false;
	//Now that we have set it explicitly, it can't be these other data kinds.
	string_data_ = "";
	integer_data_ = 0;
	is_string_data_ = false;
	is_integer_data_ = false;
	is_window_position_data_ = false;
	is_unedited_ = false;
	is_empty_ = false;
}
TXT CConfig::Name()
{
	return variable_name_;
}
TXT CConfig::GetString()
{
	if(is_empty_) return "";
	if(is_string_data_) return string_data_;
	if(is_integer_data_)
	{
		return String.FromInt(integer_data_,10);
	}
	if(is_hexadecimal_data_ && (hexadecimal_data_.size()==1))
	{
		return String.FromHex(hexadecimal_data_[0],8);
	}
	return "<ERR:80000000>";
}
U64 CConfig::GetHex64(INT index)
{
	if(is_empty_ || (index >= hexadecimal_data_64_.size())) return 0x8000000000000000LL;
	if(is_hexadecimal_data_64_) return (U64)hexadecimal_data_64_[index];
	if(is_integer_data_) return (U64)integer_data_;
	return 0x8000000000000000LL;
}
S64 CConfig::GetVsx64(INT index)
{
	if(is_empty_ || (index >= hexadecimal_data_64_.size())) return 0x8000000000000000LL;
	if(is_hexadecimal_data_64_) return (S64)hexadecimal_data_64_[index];
	if(is_integer_data_) return (S64)integer_data_;
	return 0x8000000000000000LL;
}
U32 CConfig::GetHex(INT index)
{
	if(is_empty_ || (index >= hexadecimal_data_.size())) return 0x80000000;
	if(is_hexadecimal_data_) return (U32)hexadecimal_data_[index];
	if(is_integer_data_) return (U32)integer_data_;
	return 0x80000000;
}
S32 CConfig::GetVsx(INT index)
{
	if(is_empty_ || (index >= hexadecimal_data_.size())) return 0x80000000;
	if(is_hexadecimal_data_) return (S32)hexadecimal_data_[index];
	if(is_integer_data_) return (S32)integer_data_;
	return 0x80000000;
}
S32 CConfig::GetInt()
{
	if(is_empty_) return 0x80000000;
	if(is_integer_data_) return integer_data_;
	if(is_hexadecimal_data_ && (hexadecimal_data_.size()==1)) return (S32)hexadecimal_data_[0];
	return 0x80000000;
}
void CConfig::SetWinPos(S16 x, S16 y, S16 w, S16 h, BIT m)
{
	string_data_ = (String.FromHex(x, 4) + String.FromHex(y, 4) + String.FromHex(w, 4) + String.FromHex(h, 4) + (m?"M":"W"));
	is_string_data_ = true;
	is_window_position_data_ = true;
	//Now that we have set it explicitly, it can't be these other data kinds.
	integer_data_ = 0;
	hexadecimal_data_.clear();
	is_hexadecimal_data_ = false;
	is_integer_data_ = false;
	is_unedited_ = false;
}
void CConfig::GetWinPos(S16& x, S16& y, S16& w, S16& h, BIT& m)
{
	if(!is_window_position_data_){x=y=w=h=((S16)0x8000); m=false; return;}
	x = S16(String.ToHex(string_data_.substr( 0,4)));
	y = S16(String.ToHex(string_data_.substr( 4,4)));
	w = S16(String.ToHex(string_data_.substr( 8,4)));
	h = S16(String.ToHex(string_data_.substr(12,4)));
	m = (string_data_[16]=='M');
}
void CConfig::GetWinPosNoOverwrite(S16& x, S16& y, S16& w, S16& h, BIT& m)
{
	if(!is_window_position_data_) return;
	x = S16(String.ToHex(string_data_.substr( 0,4)));
	y = S16(String.ToHex(string_data_.substr( 4,4)));
	w = S16(String.ToHex(string_data_.substr( 8,4)));
	h = S16(String.ToHex(string_data_.substr(12,4)));
	m = (string_data_[16]=='M');
}
U32 CConfig::GetSecondaryID()
{
	return secondary_id_;
}
void CConfig::WriteDataTo(CFileIO& ini_bfa)
{
	if(secondary_id_ != 0xFFFFFFFF) return;
	if(is_window_position_data_ || is_unedited_)
	{
		ini_bfa.SvText((variable_name_ + ((variable_name_=="")?(""):(" ")) + "= "));
		ini_bfa.SvText(string_data_);
		ini_bfa.SvText("\r\n");
		return;
	}
	if(is_integer_data_)
	{
		TXT to_save = (TXT("") + String.FromInt(INT(integer_data_), 10, false));
		ini_bfa.SvText((variable_name_ + ((variable_name_=="")?(""):(" ")) + "= "));
		ini_bfa.SvText(to_save);
		ini_bfa.SvText("\r\n");
		return;
	}
	if(is_hexadecimal_data_64_)
	{
		TXT to_save = "";
		for(IND i = 0; i < hexadecimal_data_64_.size(); ++i){
			to_save += String.FromHex(hexadecimal_data_64_[i],16);
		}
		ini_bfa.SvText((variable_name_ + ((variable_name_=="")?(""):(" ")) + "= "));
		ini_bfa.SvText(to_save);
		ini_bfa.SvText("\r\n");
		return;
	}
	else if(is_hexadecimal_data_)
	{
		TXT to_save = "";
		for(IND i = 0; i < hexadecimal_data_.size(); ++i){
			to_save += String.FromHex(hexadecimal_data_[i],8);
		}
		ini_bfa.SvText((variable_name_ + ((variable_name_=="")?(""):(" ")) + "= "));
		ini_bfa.SvText(to_save);
		ini_bfa.SvText("\r\n");
		return;
	}
	if(is_string_data_)
	{
		TXT str = string_data_;
		//"\t\r=#\n\\"
		///BUGFIX: Re-wrote this routine to remove \n missed-detection bug.
		for(INT i = 0; i < str.size(); i++){
			switch(str[i]){
				case '\t':{
					str.erase(str.begin()+i);
					str.insert(str.begin()+i,1,'t');
					str.insert(str.begin()+i,1,'\\'); i++;
				}break;
				case '\n':{
					str.erase(str.begin()+i);
					str.insert(str.begin()+i,1,'n');
					str.insert(str.begin()+i,1,'\\'); i++;
				}break;
				case '\r':{
					str.erase(str.begin()+i);
					str.insert(str.begin()+i,1,'r');
					str.insert(str.begin()+i,1,'\\'); i++;
				}break;
				case '\\':{
					str.insert(str.begin()+i,1,'\\'); i++;
				}break;
				case '=':{
					str.insert(str.begin()+i,1,'\\'); i++;
				}break;
				case '#':{
					str.insert(str.begin()+i,1,'\\'); i++;
				}break;
			}
		}
		ini_bfa.SvText((variable_name_ + ((variable_name_=="")?(""):(" ")) + "= "));
		ini_bfa.SvText(str);
		ini_bfa.SvText("\r\n");
		return;
	}
}
TXT CConfigList::operator[](INT i)
{
	return pointer[i]->Name();
}
