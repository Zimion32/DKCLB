#pragma once
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
// This software uses pugixml library (https://pugixml.org).
// pugixml is Copyright (C) 2006-2024 Arseny Kapoulkine.
#define CTHW can_throw_t
#define CT can_throw_tt
struct SXmlDelta
{
    U32 type;
    U32 offset;
    U32 size;
    LUCID lucid;
    EXTHD exthd;
};
struct SCdlStream
{
    TXT file;
    U08* resource;
    U32 offset;
    U32 size;
};
class CXmlFileIO
{
    //---------------------------------------------------------------
    // The methods here aim to write human readable data where possible
    // but there are still a few cases where Base64 has to be used.
    //---------------------------------------------------------------
    // Remember that reading from an empty XML document should cause the
    // program to load a file with all defaulted values; no exception
    // will ever be raised (it auto-handles XML exceptions internally).
    //---------------------------------------------------------------
    // NOTE: Raw file formats are supported within a non-CDL file but size may be reported as 0xFFFFFFFF.
    /// WARNING: With CDLs, it is generally not possible to textedit the subfiles due to 
    /// file sizes changing vs the headers, and possible binary data corrupting on save.
    /// If you need a quick edit it is required to use a hex editor to make your tweaks.
    public: 
                CXmlFileIO();
                ~CXmlFileIO();
	    //---------------------------------------------------------------
	    void    GvSelSubFile (U32 subfile); ///selects subfile, but does not open whole XML
	    void    GvUseSubFile (); ///actually loads the XML portion of the selected subfile; closes previous
  SCdlStream    GvCdlStream  (); ///instead of loading the file directly, return streaming information
		//---------------------------------------------------------------
	    U32	    GvHeaderData (LUCID& lucid, EXTHD& exthd); ///automatically loaded, this is just to grab the data
		BIT     GvBegSection (TXT section); //browses inside a node; returns false if does not exist
		void    GvEndSection (); //finish and exit to the parent - ONLY CALL IF GvBegSection() SUCCEEDS
		BIT     GvBegArray   (TXT name); //required for array nesting; returns false if array is empty
		BIT     GvNxtArray   (TXT name); //returns false when all array items are exausted
		BIT     GvLoopArray  (); //calls GvEndArray() when false, keeps using name of sibling for next item
		void    GvEndArray   (); //required for array nesting - ONLY CALL IF GvBegArray() SUCCEEDS
		S64	    GvInt        (TXT name, S64 def);
		U64	    GvHex        (TXT name, U32 bytes, U64 def);
		S64	    GvVsx        (TXT name, U32 bytes, S64 def);
		F32     GvFloat      (TXT name, F32 def);
        F32     GvFloatAsHex (TXT name, F32 def);
        F64     GvDouble     (TXT name, F64 def);
        F64     GvDoubleAsHex(TXT name, F64 def);
		TXT	    GvString     (TXT name, TXT def);
		TXT	    GvText       (TXT name, TXT def);
	    BMP     GvBitmapBlock(TXT name);
		BMP	    GvPNG        (TXT name);
		LUCID   GvLUCID      (TXT name, LUCID def);
        Vx<U08> GvFile       (TXT name);
        //---------------------------------------------------------------
        void    SvBegSubFile (); ///must be called even if there is only one subfile
        void    SvEndSubFile (); ///must be called even if there is only one subfile
        //The following three functions insert a subfile with content from an existing headered file/buffer.
		void    SvCpySubFile (Vx<U08>& datavec);
		void    SvCpySubFile (U08* data, U32 bytes);
		void    SvCpySubFile (TXT filename);
		//These help with copying data to another file but with new header information.
		void    SvChangeLUCID(LUCID lucid); ///only changes subfile zero
		void    SvChangeEXTHD(EXTHD exthd); ///only changes subfile zero
		//These allow the insertion of a raw subfile data given proper type and an access LUCID.
		//In music sets the dynamic's LUCID is already predetermined. It should NOT be recalculated.
		void    SvRawSubFile (U08* data, U32 bytes, U32 type, LUCID lucid);
        void    SvRawSubFile (TXT filename, U32 type, LUCID lucid);
		//---------------------------------------------------------------
	    void	SvHeaderData (U32 type, LUCID lucid, EXTHD exthd); ///DELTA header, stored until save
		void    SvBegSection (TXT section); //creates a node to store things in
		void    SvEndSection (); //finish and exit back to the parent section
		BIT     SvLoopArray  (TXT name, INT i, INT len); //shortcut, pass negative length for reversed loops
		void    SvBegArray   (TXT name); //this is required because we may have nested arrays of items
		void    SvNxtArray   (TXT name); //once for each item; writes element and browses inside
		void    SvEndArray   (); //must be called to reset the data location after last array item is written
		void	SvInt        (TXT name, S64 value);
		void	SvHex        (TXT name, U32 bytes, U64 value);
		void	SvVsx        (TXT name, U32 bytes, S64 value);
		void	SvFloat      (TXT name, F32 value);
		void	SvFloatAsHex (TXT name, F32 value);
		void	SvDouble     (TXT name, F64 value);
		void	SvDoubleAsHex(TXT name, F64 value);
		void	SvString     (TXT name, TXT text);
		void	SvText       (TXT name, TXT text);
		void	SvBitmapBlock(TXT name, BMP bmp);
		void	SvPNG        (TXT name, BMP bmp);
		void    SvLUCID      (TXT name, LUCID value);
		void    SvFile       (TXT name, Vx<U08>& datavec);
		void    SvFile       (TXT name, TXT path);
		//---------------------------------------------------------------
		CT<U32>	Load         (TXT filename); ///returns number of subfiles - does not actually load the XML
		CT<U32>	LoadEx       (TXT filename, U32 offset, U08* res_data, U32 res_bytes);
		CTHW	Save         (TXT filename); ///automatically adds DELTA headers before saveout
		void	Close        ();
		//===============================================================
    private:
Vx<SXmlDelta>   xmldelta_headers_;
        Vx<U08> cdl_buffer_; //memory buffer if building cdl, triggered by empty subfile zero
        BIT     is_cdl_;
        INT     cdl_build_detect_;
		TXT     filepath_; //only set by Load routines
		U32     headers_end_;
		U32     selected_subfile_;
		U08*    res_data_;
		U32     res_bytes_;
xml_document    xmldoc_;
xml_node        xmlcurrentnode_;
		
		CT<U32>	LoadAndUse   (U08* data, U32 bytes);///NON-CDL BUFFER LOADER
		CTHW	SaveXML      (Vx<U08>& datavec); ///**appends** xml to a vector buffer
		U32     ScanHeaders_ (U32 offset, TXT& headersblock);
		U32     ScanHeaders_Buffer_(U08* data, U32 bytes, TXT& headersblock);
		U32     ParseHeaders_(U32 headers_end, TXT& headersblock);
		void    WriteHeaders_(CFileIO& file);
		
};
#undef CT
#undef CTHW
