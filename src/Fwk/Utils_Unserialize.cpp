#include "include.h"
void KUtils::UnserializeVector(Vx<U08>& in, Vx< Vx<U08> >& out)
{
	INT out_offset = 0;
	INT y_size = 0;
	y_size |= ((in[out_offset++] & 0xFF) << 24);
	y_size |= ((in[out_offset++] & 0xFF) << 16);
	y_size |= ((in[out_offset++] & 0xFF) << 8);
	y_size |= ((in[out_offset++] & 0xFF) << 0);
	out.clear();
	for(IND yy = 0; yy < y_size; ++yy)
	{
		out.push_back( Vx<U08>() );
		INT x_size = 0;
		x_size |= ((in[out_offset++] & 0xFF) << 24);
		x_size |= ((in[out_offset++] & 0xFF) << 16);
		x_size |= ((in[out_offset++] & 0xFF) << 8);
		x_size |= ((in[out_offset++] & 0xFF) << 0);
		for(IND xx = 0; xx < x_size; ++xx)
		{
			INT v_data = 0;
			v_data |= ((in[out_offset++] & 0xFF) << 24);
			v_data |= ((in[out_offset++] & 0xFF) << 16);
			v_data |= ((in[out_offset++] & 0xFF) << 8);
			v_data |= ((in[out_offset++] & 0xFF) << 0);
			out[yy].push_back( v_data );
		}
	}
}
void KUtils::UnserializeVector(Vx<U08>& in, Vx< Vx<U16> >& out)
{
	INT out_offset = 0;
	INT y_size = 0;
	y_size |= ((in[out_offset++] & 0xFF) << 24);
	y_size |= ((in[out_offset++] & 0xFF) << 16);
	y_size |= ((in[out_offset++] & 0xFF) << 8);
	y_size |= ((in[out_offset++] & 0xFF) << 0);
	out.clear();
	for(IND yy = 0; yy < y_size; ++yy)
	{
		out.push_back( Vx<U16>() );
		INT x_size = 0;
		x_size |= ((in[out_offset++] & 0xFF) << 24);
		x_size |= ((in[out_offset++] & 0xFF) << 16);
		x_size |= ((in[out_offset++] & 0xFF) << 8);
		x_size |= ((in[out_offset++] & 0xFF) << 0);
		for(IND xx = 0; xx < x_size; ++xx)
		{
			INT v_data = 0;
			v_data |= ((in[out_offset++] & 0xFF) << 24);
			v_data |= ((in[out_offset++] & 0xFF) << 16);
			v_data |= ((in[out_offset++] & 0xFF) << 8);
			v_data |= ((in[out_offset++] & 0xFF) << 0);
			out[yy].push_back( v_data );
		}
	}
}
void KUtils::UnserializeVector(Vx<U08>& in, Vx< Vx<U32> >& out)
{
	INT out_offset = 0;
	INT y_size = 0;
	y_size |= ((in[out_offset++] & 0xFF) << 24);
	y_size |= ((in[out_offset++] & 0xFF) << 16);
	y_size |= ((in[out_offset++] & 0xFF) << 8);
	y_size |= ((in[out_offset++] & 0xFF) << 0);
	out.clear();
	for(IND yy = 0; yy < y_size; ++yy)
	{
		out.push_back( Vx<U32>() );
		INT x_size = 0;
		x_size |= ((in[out_offset++] & 0xFF) << 24);
		x_size |= ((in[out_offset++] & 0xFF) << 16);
		x_size |= ((in[out_offset++] & 0xFF) << 8);
		x_size |= ((in[out_offset++] & 0xFF) << 0);
		for(IND xx = 0; xx < x_size; ++xx)
		{
			INT v_data = 0;
			v_data |= ((in[out_offset++] & 0xFF) << 24);
			v_data |= ((in[out_offset++] & 0xFF) << 16);
			v_data |= ((in[out_offset++] & 0xFF) << 8);
			v_data |= ((in[out_offset++] & 0xFF) << 0);
			out[yy].push_back( v_data );
		}
	}
}
void KUtils::UnserializeVector(Vx<U08>& in, Vx< Vx<S08> >& out)
{
	INT out_offset = 0;
	INT y_size = 0;
	y_size |= ((in[out_offset++] & 0xFF) << 24);
	y_size |= ((in[out_offset++] & 0xFF) << 16);
	y_size |= ((in[out_offset++] & 0xFF) << 8);
	y_size |= ((in[out_offset++] & 0xFF) << 0);
	out.clear();
	for(IND yy = 0; yy < y_size; ++yy)
	{
		out.push_back( Vx<S08>() );
		INT x_size = 0;
		x_size |= ((in[out_offset++] & 0xFF) << 24);
		x_size |= ((in[out_offset++] & 0xFF) << 16);
		x_size |= ((in[out_offset++] & 0xFF) << 8);
		x_size |= ((in[out_offset++] & 0xFF) << 0);
		for(IND xx = 0; xx < x_size; ++xx)
		{
			INT v_data = 0;
			v_data |= ((in[out_offset++] & 0xFF) << 24);
			v_data |= ((in[out_offset++] & 0xFF) << 16);
			v_data |= ((in[out_offset++] & 0xFF) << 8);
			v_data |= ((in[out_offset++] & 0xFF) << 0);
			out[yy].push_back( v_data );
		}
	}
}
void KUtils::UnserializeVector(Vx<U08>& in, Vx< Vx<S16> >& out)
{
	INT out_offset = 0;
	INT y_size = 0;
	y_size |= ((in[out_offset++] & 0xFF) << 24);
	y_size |= ((in[out_offset++] & 0xFF) << 16);
	y_size |= ((in[out_offset++] & 0xFF) << 8);
	y_size |= ((in[out_offset++] & 0xFF) << 0);
	out.clear();
	for(IND yy = 0; yy < y_size; ++yy)
	{
		out.push_back( Vx<S16>() );
		INT x_size = 0;
		x_size |= ((in[out_offset++] & 0xFF) << 24);
		x_size |= ((in[out_offset++] & 0xFF) << 16);
		x_size |= ((in[out_offset++] & 0xFF) << 8);
		x_size |= ((in[out_offset++] & 0xFF) << 0);
		for(IND xx = 0; xx < x_size; ++xx)
		{
			INT v_data = 0;
			v_data |= ((in[out_offset++] & 0xFF) << 24);
			v_data |= ((in[out_offset++] & 0xFF) << 16);
			v_data |= ((in[out_offset++] & 0xFF) << 8);
			v_data |= ((in[out_offset++] & 0xFF) << 0);
			out[yy].push_back( v_data );
		}
	}
}
void KUtils::UnserializeVector(Vx<U08>& in, Vx< Vx<S32> >& out)
{
	INT out_offset = 0;
	INT y_size = 0;
	y_size |= ((in[out_offset++] & 0xFF) << 24);
	y_size |= ((in[out_offset++] & 0xFF) << 16);
	y_size |= ((in[out_offset++] & 0xFF) << 8);
	y_size |= ((in[out_offset++] & 0xFF) << 0);
	out.clear();
	for(IND yy = 0; yy < y_size; ++yy)
	{
		out.push_back( Vx<S32>() );
		INT x_size = 0;
		x_size |= ((in[out_offset++] & 0xFF) << 24);
		x_size |= ((in[out_offset++] & 0xFF) << 16);
		x_size |= ((in[out_offset++] & 0xFF) << 8);
		x_size |= ((in[out_offset++] & 0xFF) << 0);
		for(IND xx = 0; xx < x_size; ++xx)
		{
			INT v_data = 0;
			v_data |= ((in[out_offset++] & 0xFF) << 24);
			v_data |= ((in[out_offset++] & 0xFF) << 16);
			v_data |= ((in[out_offset++] & 0xFF) << 8);
			v_data |= ((in[out_offset++] & 0xFF) << 0);
			out[yy].push_back( v_data );
		}
	}
}
void KUtils::UnserializeVector(Vx<U08>& in, Vx< Vx< Vx<U08> > >& out)
{
	INT out_offset = 0;
	INT z_size = 0;
	z_size |= ((in[out_offset++] & 0xFF) << 24);
	z_size |= ((in[out_offset++] & 0xFF) << 16);
	z_size |= ((in[out_offset++] & 0xFF) << 8);
	z_size |= ((in[out_offset++] & 0xFF) << 0);
	out.clear();
	for(IND zz = 0; zz < z_size; ++zz)
	{
		out.push_back( Vx< Vx<U08> >() );
		INT y_size = 0;
		y_size |= ((in[out_offset++] & 0xFF) << 24);
		y_size |= ((in[out_offset++] & 0xFF) << 16);
		y_size |= ((in[out_offset++] & 0xFF) << 8);
		y_size |= ((in[out_offset++] & 0xFF) << 0);
		for(IND yy = 0; yy < y_size; ++yy)
		{
			out[zz].push_back( Vx<U08>() );
			INT x_size = 0;
			x_size |= ((in[out_offset++] & 0xFF) << 24);
			x_size |= ((in[out_offset++] & 0xFF) << 16);
			x_size |= ((in[out_offset++] & 0xFF) << 8);
			x_size |= ((in[out_offset++] & 0xFF) << 0);
			for(IND xx = 0; xx < x_size; ++xx)
			{
				INT v_data = 0;
				v_data |= ((in[out_offset++] & 0xFF) << 24);
				v_data |= ((in[out_offset++] & 0xFF) << 16);
				v_data |= ((in[out_offset++] & 0xFF) << 8);
				v_data |= ((in[out_offset++] & 0xFF) << 0);
				out[zz][yy].push_back( v_data );
			}
		}
	}
}
void KUtils::UnserializeVector(Vx<U08>& in, Vx< Vx< Vx<U16> > >& out)
{
	INT out_offset = 0;
	INT z_size = 0;
	z_size |= ((in[out_offset++] & 0xFF) << 24);
	z_size |= ((in[out_offset++] & 0xFF) << 16);
	z_size |= ((in[out_offset++] & 0xFF) << 8);
	z_size |= ((in[out_offset++] & 0xFF) << 0);
	out.clear();
	for(IND zz = 0; zz < z_size; ++zz)
	{
		out.push_back( Vx< Vx<U16> >() );
		INT y_size = 0;
		y_size |= ((in[out_offset++] & 0xFF) << 24);
		y_size |= ((in[out_offset++] & 0xFF) << 16);
		y_size |= ((in[out_offset++] & 0xFF) << 8);
		y_size |= ((in[out_offset++] & 0xFF) << 0);
		for(IND yy = 0; yy < y_size; ++yy)
		{
			out[zz].push_back( Vx<U16>() );
			INT x_size = 0;
			x_size |= ((in[out_offset++] & 0xFF) << 24);
			x_size |= ((in[out_offset++] & 0xFF) << 16);
			x_size |= ((in[out_offset++] & 0xFF) << 8);
			x_size |= ((in[out_offset++] & 0xFF) << 0);
			for(IND xx = 0; xx < x_size; ++xx)
			{
				INT v_data = 0;
				v_data |= ((in[out_offset++] & 0xFF) << 24);
				v_data |= ((in[out_offset++] & 0xFF) << 16);
				v_data |= ((in[out_offset++] & 0xFF) << 8);
				v_data |= ((in[out_offset++] & 0xFF) << 0);
				out[zz][yy].push_back( v_data );
			}
		}
	}
}
void KUtils::UnserializeVector(Vx<U08>& in, Vx< Vx< Vx<U32> > >& out)
{
	INT out_offset = 0;
	INT z_size = 0;
	z_size |= ((in[out_offset++] & 0xFF) << 24);
	z_size |= ((in[out_offset++] & 0xFF) << 16);
	z_size |= ((in[out_offset++] & 0xFF) << 8);
	z_size |= ((in[out_offset++] & 0xFF) << 0);
	out.clear();
	for(IND zz = 0; zz < z_size; ++zz)
	{
		out.push_back( Vx< Vx<U32> >() );
		INT y_size = 0;
		y_size |= ((in[out_offset++] & 0xFF) << 24);
		y_size |= ((in[out_offset++] & 0xFF) << 16);
		y_size |= ((in[out_offset++] & 0xFF) << 8);
		y_size |= ((in[out_offset++] & 0xFF) << 0);
		for(IND yy = 0; yy < y_size; ++yy)
		{
			out[zz].push_back( Vx<U32>() );
			INT x_size = 0;
			x_size |= ((in[out_offset++] & 0xFF) << 24);
			x_size |= ((in[out_offset++] & 0xFF) << 16);
			x_size |= ((in[out_offset++] & 0xFF) << 8);
			x_size |= ((in[out_offset++] & 0xFF) << 0);
			for(IND xx = 0; xx < x_size; ++xx)
			{
				INT v_data = 0;
				v_data |= ((in[out_offset++] & 0xFF) << 24);
				v_data |= ((in[out_offset++] & 0xFF) << 16);
				v_data |= ((in[out_offset++] & 0xFF) << 8);
				v_data |= ((in[out_offset++] & 0xFF) << 0);
				out[zz][yy].push_back( v_data );
			}
		}
	}
}
void KUtils::UnserializeVector(Vx<U08>& in, Vx< Vx< Vx<S08> > >& out)
{
	INT out_offset = 0;
	INT z_size = 0;
	z_size |= ((in[out_offset++] & 0xFF) << 24);
	z_size |= ((in[out_offset++] & 0xFF) << 16);
	z_size |= ((in[out_offset++] & 0xFF) << 8);
	z_size |= ((in[out_offset++] & 0xFF) << 0);
	out.clear();
	for(IND zz = 0; zz < z_size; ++zz)
	{
		out.push_back( Vx< Vx<S08> >() );
		INT y_size = 0;
		y_size |= ((in[out_offset++] & 0xFF) << 24);
		y_size |= ((in[out_offset++] & 0xFF) << 16);
		y_size |= ((in[out_offset++] & 0xFF) << 8);
		y_size |= ((in[out_offset++] & 0xFF) << 0);
		for(IND yy = 0; yy < y_size; ++yy)
		{
			out[zz].push_back( Vx<S08>() );
			INT x_size = 0;
			x_size |= ((in[out_offset++] & 0xFF) << 24);
			x_size |= ((in[out_offset++] & 0xFF) << 16);
			x_size |= ((in[out_offset++] & 0xFF) << 8);
			x_size |= ((in[out_offset++] & 0xFF) << 0);
			for(IND xx = 0; xx < x_size; ++xx)
			{
				INT v_data = 0;
				v_data |= ((in[out_offset++] & 0xFF) << 24);
				v_data |= ((in[out_offset++] & 0xFF) << 16);
				v_data |= ((in[out_offset++] & 0xFF) << 8);
				v_data |= ((in[out_offset++] & 0xFF) << 0);
				out[zz][yy].push_back( v_data );
			}
		}
	}
}
void KUtils::UnserializeVector(Vx<U08>& in, Vx< Vx< Vx<S16> > >& out)
{
	INT out_offset = 0;
	INT z_size = 0;
	z_size |= ((in[out_offset++] & 0xFF) << 24);
	z_size |= ((in[out_offset++] & 0xFF) << 16);
	z_size |= ((in[out_offset++] & 0xFF) << 8);
	z_size |= ((in[out_offset++] & 0xFF) << 0);
	out.clear();
	for(IND zz = 0; zz < z_size; ++zz)
	{
		out.push_back( Vx< Vx<S16> >() );
		INT y_size = 0;
		y_size |= ((in[out_offset++] & 0xFF) << 24);
		y_size |= ((in[out_offset++] & 0xFF) << 16);
		y_size |= ((in[out_offset++] & 0xFF) << 8);
		y_size |= ((in[out_offset++] & 0xFF) << 0);
		for(IND yy = 0; yy < y_size; ++yy)
		{
			out[zz].push_back( Vx<S16>() );
			INT x_size = 0;
			x_size |= ((in[out_offset++] & 0xFF) << 24);
			x_size |= ((in[out_offset++] & 0xFF) << 16);
			x_size |= ((in[out_offset++] & 0xFF) << 8);
			x_size |= ((in[out_offset++] & 0xFF) << 0);
			for(IND xx = 0; xx < x_size; ++xx)
			{
				INT v_data = 0;
				v_data |= ((in[out_offset++] & 0xFF) << 24);
				v_data |= ((in[out_offset++] & 0xFF) << 16);
				v_data |= ((in[out_offset++] & 0xFF) << 8);
				v_data |= ((in[out_offset++] & 0xFF) << 0);
				out[zz][yy].push_back( v_data );
			}
		}
	}
}
void KUtils::UnserializeVector(Vx<U08>& in, Vx< Vx< Vx<S32> > >& out)
{
	INT out_offset = 0;
	INT z_size = 0;
	z_size |= ((in[out_offset++] & 0xFF) << 24);
	z_size |= ((in[out_offset++] & 0xFF) << 16);
	z_size |= ((in[out_offset++] & 0xFF) << 8);
	z_size |= ((in[out_offset++] & 0xFF) << 0);
	out.clear();
	for(IND zz = 0; zz < z_size; ++zz)
	{
		out.push_back( Vx< Vx<S32> >() );
		INT y_size = 0;
		y_size |= ((in[out_offset++] & 0xFF) << 24);
		y_size |= ((in[out_offset++] & 0xFF) << 16);
		y_size |= ((in[out_offset++] & 0xFF) << 8);
		y_size |= ((in[out_offset++] & 0xFF) << 0);
		for(IND yy = 0; yy < y_size; ++yy)
		{
			out[zz].push_back( Vx<S32>() );
			INT x_size = 0;
			x_size |= ((in[out_offset++] & 0xFF) << 24);
			x_size |= ((in[out_offset++] & 0xFF) << 16);
			x_size |= ((in[out_offset++] & 0xFF) << 8);
			x_size |= ((in[out_offset++] & 0xFF) << 0);
			for(IND xx = 0; xx < x_size; ++xx)
			{
				INT v_data = 0;
				v_data |= ((in[out_offset++] & 0xFF) << 24);
				v_data |= ((in[out_offset++] & 0xFF) << 16);
				v_data |= ((in[out_offset++] & 0xFF) << 8);
				v_data |= ((in[out_offset++] & 0xFF) << 0);
				out[zz][yy].push_back( v_data );
			}
		}
	}
}
