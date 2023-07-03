/*===========================================================================*\
 *  DKC Level Builder Toolkit
 *  Copyright (C) 2023 Simion32
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
KDeltaError::KDeltaError()
{
    log_errors_ = 1;
    box_errors_ = 1;
}
KDeltaError::~KDeltaError()
{

}
void KDeltaError::ToggleBoxes()
{
    box_errors_ ^= 2;
}
void KDeltaError::Configure(BIT log_errors, BIT box_errors)
{
    log_errors_ = log_errors;
    box_errors_ = box_errors;
}
TXT KDeltaError::GetLastError()
{
    return error_list_.back();
}
Vx<TXT> KDeltaError::GetErrorList()
{
    return error_list_;
}
void KDeltaError::AddError(TXT error_string)
{
    error_list_.push_back(error_string);
    if(log_errors_) Debug.Log(error_string);
    if((box_errors_ & 1) && !(box_errors_ & 2)) Debug.Box(error_string,"DELTA Engine Error!!");
}
void Derr(TXT function_name, TXT error_string)
{
    DeltaError.AddError(function_name + ":\r\n" + error_string);
}
