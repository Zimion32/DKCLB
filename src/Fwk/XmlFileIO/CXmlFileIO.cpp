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
#include "CXmlFileIO.h"
// This software uses pugixml library (https://pugixml.org).
// pugixml is Copyright (C) 2006-2024 Arseny Kapoulkine.
//---------------------------------------------------------------
CXmlFileIO::CXmlFileIO()
{
    filepath_ = "";
	headers_end_ = 0;
	selected_subfile_ = 0xFFFFFFFF;
	cdl_build_detect_ = 0;
	is_cdl_ = false;
	res_data_ = NULL;
	res_bytes_ = 0;
	xmldoc_.reset();
}
CXmlFileIO::~CXmlFileIO()
{
	
}

//escape < with &lt;
//escape & with &amp;
//escape > with &gt;
//escape ' with &apos;
//escape " with &quot;
