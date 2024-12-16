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
#define qyz __attribute__((warn_unused_result))
#define CTHW can_throw_t
#define CT can_throw_tt
#define LCD LUCID
#define XHD EXTHD
#define SVD SVirtualData
struct SVirtualData
{
        TXT     path;
        U32     size;
        U08*    data;
                SVirtualData(const SVirtualData& s): path(s.path), size(s.size), data(s.data){}
                SVirtualData(TXT path, U32 size, U08* data): path(path), size(size), data(data){}
};
class CFileIO
{
	public:
		CTHW	Reserve(U32 reserve_memory = 0x10000);
        void    FreeMemory();
		
		U32		GetR()					qyz;
		U32		GetW()					qyz;
		U32		ByteSize()				qyz;
       Vx<U08>& Vector()				qyz;
		
		CTHW	SetR(U32 location);
		CTHW	SetW(U32 location);
		
		// throw if footer SHA256 doesn't match
		// calling this function subtracts 32 from the len_position_ address
		CTHW	GvCheckSHA256(U32 sha256_start);
		
		//When called, this returns the chunk ID, allowing the 
		//program to differentiate between various data chunks.
		//Chunk ID $0 means an empty chunk and is treated specially.
		CT<U32> GvChunkID()	            qyz;
		CT<U32> GvPeekAtChunkID()	    qyz; //get the next chunk ID without affecting the read position
		
		//If the program wants to skip over a chunk it doesn't
		//understand, then this call uses the current chunk length
		//to skip to the position of the next chunk ID.
		CTHW    GvIgnoreChunk();
		
		//These manage the loops that deal with each section of chunks.
		//The functions here do NOT modify the read position (they use GvPeekAtChunkID()).
		///WARNING: A file format, once established, cannot switch to a different section layout.
		/// This is because there wouldn't be a way to determine which parts of the file are new.
		CT<BIT> GvIsSectionEntry();      //return true while section is not over
		CT<BIT> GvIsSectionArrayEntry(); //return true while section array is not over
		
		//this call will end the previous chunk. 
		//the value $0 is a special trigger (empty chunk) that marks the end of
		//either an array of chunks or the end of the file. Chunks marked with $0
		//are always empty (size == 0) and are not stored in the file's footer data.
		//note that $0 chunks can also be used to end sections or arrays of sections in the file.
		CTHW    SvChunkID(U32 id);
		
		//this call automatically ends the last chunk before writing the footer data.
		//note that a SHA256 is calculated and appended to the file data.
		CTHW    SvFooter();
		
		//These functions manage creating file sections (each made up of chunks).
		//Note that the implementation of these may occasionally not do anything.
		CTHW    SvBeginSection();
		CTHW    SvEndSection();
		CTHW    SvBeginSectionArray();
		CTHW    SvEndSectionArray();
		
		CT<U64>	GvHex(U32 bytes)		qyz;
		CT<S64>	GvVsx(U32 bytes)		qyz;
		CT<U64>	GvHexLe(U32 bytes)		qyz;
		CT<S64>	GvVsxLe(U32 bytes)		qyz;
		CT<F32> GvFloat()				qyz;
        CT<F64> GvDouble()				qyz;
		CT<TXT>	GvString()				qyz;
		CT<TXT>	GvText(U32 chars)       qyz;
		CT<TXT>	GvTextBlock(U32 bytes)  qyz;
		CT<U32>	GvHeader(TXT magic5)    qyz; // GvHeader() throws exception if not proper file header.
	    CT<BMP>	GvBitmapBlock(U32 expected_w, U32 expected_h); ///WATCH OUT: reads raw pixels
		CT<BMP>	GvPNG()				    qyz;
		CT<LCD> GvLUCID()               qyz;
		CT<XHD> GvEXTHD()               qyz;
		CTHW    GvSkipBytes(U32 bytes);
		CTHW    GvSkipPNG();
CT< Vx<U08> >   GvFile();
		
		CTHW	SvHex(U64 value, U32 bytes);
		CTHW	SvVsx(S64 value, U32 bytes);
		CTHW	SvHexLe(U64 value, U32 bytes);
		CTHW	SvVsxLe(S64 value, U32 bytes);
		CTHW	SvFloat(F32 value);
		CTHW	SvDouble(F64 value);
		CTHW	SvString(TXT text);
		CTHW	SvText(TXT text);
		CTHW	SvTextBlock(TXT text, U32 bytes);
		CTHW	SvHeader(TXT magic5, U32 type, U32 filecount, U32 sha256_calc_start);
		CTHW	SvBitmapBlock(BITMAP *bmp, U32 expected_w, U32 expected_h); ///WATCH OUT: writes raw pixels
		CTHW	SvPNG(BITMAP *bmp);
		CTHW    SvLUCID(LUCID identifier);
		CTHW    SvEXTHD(EXTHD identifier);
		CTHW    SvSkipBytes(U32 bytes);
		CTHW    SvFile(Vx<U08>& datavec);
		CTHW    SvFile(TXT path);
		CTHW    SvFile_NoLen(TXT path);
		
		CT<U64>	FileSize(TXT filename);
		CTHW	Open(TXT filename);
		CTHW	Load(TXT filename);
		CTHW	LoadSection(TXT filename, U32 offset, U32 size);
		CTHW	LoadSectionEx(TXT filename, U32 offset, U32 size, U08* res_data, U32 res_size);
		CTHW	Fill(Vx<U08>& data);
		CTHW	Fill(U08* data, U32 bytes);
		CTHW	Append(Vx<U08>& data);
		CTHW	Append(U08* data, U32 bytes);
		CTHW	Dump(Vx<U08>& data);
		CTHW	Flush(TXT filename);
		CTHW	Save(TXT filename);
		void	Close();
		
		CTHW	FlushToCxx(TXT filename); 
		CTHW	FlushToCxx(TXT filename, TXT dataname, TXT homepath); 
		
		CTHW    FillPNG(BITMAP *bmp);
        CTHW    AppendPNG(BITMAP *bmp);
        CTHW    SavePNG(BMP bmp, TXT filename);
        CTHW    SaveOpaquePNG(BMP bmp, TXT filename);
        CT<BMP> LoadPNG(TXT filename);
        CTHW    SavePNG_FAST(BMP bmp, TXT filename);
        
                CFileIO();
                ~CFileIO();
		
		U08&    operator[](U32 index);
		U08&    operator()(U32 index);
		UVL     operator()(U32 index, U32 bytes);
		
		//=======================================================================
		//=======================================================================
		
		void    SvSubFile(U32 type);
		CT<U32>	GetSubFileCount();
		CT<U32>	GetSubFileSize(U32 which);
		CT<DGEID>	GetSubFileDgeid(U32 which);
		
    private:
		Vx<U08> buffer_;
		U32     get_position_;
		U32		set_position_;
		TXT     last_error_;
		Vx<U32> len_buffer_;   //holds non-empty chunk lengths, from 0 to N (written in backwards order)
		U32     len_position_; //used by chunk reader code
		U32     len_begin_;    //chunk starting position, for length buffer (if zero, no chunk started)
		U32     last_chunk_length_;
		U32     last_chunk_id_;
		U32     sha256_calc_start_;
		BIT     has_subfiles_;
		U32     subfile_offset_;
		Vx<U32> subfile_sizes_;
		Vx<DGEID> subfile_dgeids_;
static  Vx<SVD> VirtualFileArray;

		void    SetupChunkValues_();
		void    EndChunk_();
};
extern CFileIO FIO;
#undef CTHW
#undef CT
#undef LCD
#undef XHD
#undef SVD
#undef qyz
