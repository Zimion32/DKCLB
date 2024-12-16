#include "include.h"
void KUtils::SerializeVector(Vx< Vx<U08> >& in, Vx<U08>& out)
{
	out.push_back((in.size() >> 24) & 0xFF);
	out.push_back((in.size() >> 16) & 0xFF);
	out.push_back((in.size() >> 8) & 0xFF);
	out.push_back((in.size() >> 0) & 0xFF);
	for(IND yy = 0; yy < (INT)in.size(); ++yy)
	{
		out.push_back((in[yy].size() >> 24) & 0xFF);
		out.push_back((in[yy].size() >> 16) & 0xFF);
		out.push_back((in[yy].size() >> 8) & 0xFF);
		out.push_back((in[yy].size() >> 0) & 0xFF);
		for(IND xx = 0; xx < (INT)in[yy].size(); ++xx)
		{
			out.push_back((in[yy][xx] >> 24) & 0xFF);
			out.push_back((in[yy][xx] >> 16) & 0xFF);
			out.push_back((in[yy][xx] >> 8) & 0xFF);
			out.push_back((in[yy][xx] >> 0) & 0xFF);
		}
	}
}
void KUtils::SerializeVector(Vx< Vx<U16> >& in, Vx<U08>& out)
{
	out.push_back((in.size() >> 24) & 0xFF);
	out.push_back((in.size() >> 16) & 0xFF);
	out.push_back((in.size() >> 8) & 0xFF);
	out.push_back((in.size() >> 0) & 0xFF);
	for(IND yy = 0; yy < (INT)in.size(); ++yy)
	{
		out.push_back((in[yy].size() >> 24) & 0xFF);
		out.push_back((in[yy].size() >> 16) & 0xFF);
		out.push_back((in[yy].size() >> 8) & 0xFF);
		out.push_back((in[yy].size() >> 0) & 0xFF);
		for(IND xx = 0; xx < (INT)in[yy].size(); ++xx)
		{
			out.push_back((in[yy][xx] >> 24) & 0xFF);
			out.push_back((in[yy][xx] >> 16) & 0xFF);
			out.push_back((in[yy][xx] >> 8) & 0xFF);
			out.push_back((in[yy][xx] >> 0) & 0xFF);
		}
	}
}
void KUtils::SerializeVector(Vx< Vx<U32> >& in, Vx<U08>& out)
{
	out.push_back((in.size() >> 24) & 0xFF);
	out.push_back((in.size() >> 16) & 0xFF);
	out.push_back((in.size() >> 8) & 0xFF);
	out.push_back((in.size() >> 0) & 0xFF);
	for(IND yy = 0; yy < (INT)in.size(); ++yy)
	{
		out.push_back((in[yy].size() >> 24) & 0xFF);
		out.push_back((in[yy].size() >> 16) & 0xFF);
		out.push_back((in[yy].size() >> 8) & 0xFF);
		out.push_back((in[yy].size() >> 0) & 0xFF);
		for(IND xx = 0; xx < (INT)in[yy].size(); ++xx)
		{
			out.push_back((in[yy][xx] >> 24) & 0xFF);
			out.push_back((in[yy][xx] >> 16) & 0xFF);
			out.push_back((in[yy][xx] >> 8) & 0xFF);
			out.push_back((in[yy][xx] >> 0) & 0xFF);
		}
	}
}
void KUtils::SerializeVector(Vx< Vx<S08> >& in, Vx<U08>& out)
{
	out.push_back((in.size() >> 24) & 0xFF);
	out.push_back((in.size() >> 16) & 0xFF);
	out.push_back((in.size() >> 8) & 0xFF);
	out.push_back((in.size() >> 0) & 0xFF);
	for(IND yy = 0; yy < (INT)in.size(); ++yy)
	{
		out.push_back((in[yy].size() >> 24) & 0xFF);
		out.push_back((in[yy].size() >> 16) & 0xFF);
		out.push_back((in[yy].size() >> 8) & 0xFF);
		out.push_back((in[yy].size() >> 0) & 0xFF);
		for(IND xx = 0; xx < (INT)in[yy].size(); ++xx)
		{
			out.push_back((in[yy][xx] >> 24) & 0xFF);
			out.push_back((in[yy][xx] >> 16) & 0xFF);
			out.push_back((in[yy][xx] >> 8) & 0xFF);
			out.push_back((in[yy][xx] >> 0) & 0xFF);
		}
	}
}
void KUtils::SerializeVector(Vx< Vx<S16> >& in, Vx<U08>& out)
{
	out.push_back((in.size() >> 24) & 0xFF);
	out.push_back((in.size() >> 16) & 0xFF);
	out.push_back((in.size() >> 8) & 0xFF);
	out.push_back((in.size() >> 0) & 0xFF);
	for(IND yy = 0; yy < (INT)in.size(); ++yy)
	{
		out.push_back((in[yy].size() >> 24) & 0xFF);
		out.push_back((in[yy].size() >> 16) & 0xFF);
		out.push_back((in[yy].size() >> 8) & 0xFF);
		out.push_back((in[yy].size() >> 0) & 0xFF);
		for(IND xx = 0; xx < (INT)in[yy].size(); ++xx)
		{
			out.push_back((in[yy][xx] >> 24) & 0xFF);
			out.push_back((in[yy][xx] >> 16) & 0xFF);
			out.push_back((in[yy][xx] >> 8) & 0xFF);
			out.push_back((in[yy][xx] >> 0) & 0xFF);
		}
	}
}
void KUtils::SerializeVector(Vx< Vx<S32> >& in, Vx<U08>& out)
{
	out.push_back((in.size() >> 24) & 0xFF);
	out.push_back((in.size() >> 16) & 0xFF);
	out.push_back((in.size() >> 8) & 0xFF);
	out.push_back((in.size() >> 0) & 0xFF);
	for(IND yy = 0; yy < (INT)in.size(); ++yy)
	{
		out.push_back((in[yy].size() >> 24) & 0xFF);
		out.push_back((in[yy].size() >> 16) & 0xFF);
		out.push_back((in[yy].size() >> 8) & 0xFF);
		out.push_back((in[yy].size() >> 0) & 0xFF);
		for(IND xx = 0; xx < (INT)in[yy].size(); ++xx)
		{
			out.push_back((in[yy][xx] >> 24) & 0xFF);
			out.push_back((in[yy][xx] >> 16) & 0xFF);
			out.push_back((in[yy][xx] >> 8) & 0xFF);
			out.push_back((in[yy][xx] >> 0) & 0xFF);
		}
	}
}
void KUtils::SerializeVector(Vx< Vx< Vx<U08> > >& in, Vx<U08>& out)
{
	out.push_back((in.size() >> 24) & 0xFF);
	out.push_back((in.size() >> 16) & 0xFF);
	out.push_back((in.size() >> 8) & 0xFF);
	out.push_back((in.size() >> 0) & 0xFF);
	for(IND zz = 0; zz < (INT)in.size(); ++zz)
	{
		out.push_back((in[zz].size() >> 24) & 0xFF);
		out.push_back((in[zz].size() >> 16) & 0xFF);
		out.push_back((in[zz].size() >> 8) & 0xFF);
		out.push_back((in[zz].size() >> 0) & 0xFF);
		for(IND yy = 0; yy < (INT)in[zz].size(); ++yy)
		{
			out.push_back((in[zz][yy].size() >> 24) & 0xFF);
			out.push_back((in[zz][yy].size() >> 16) & 0xFF);
			out.push_back((in[zz][yy].size() >> 8) & 0xFF);
			out.push_back((in[zz][yy].size() >> 0) & 0xFF);
			for(IND xx = 0; xx < (INT)in[zz][yy].size(); ++xx)
			{
				out.push_back((in[zz][yy][xx] >> 24) & 0xFF);
				out.push_back((in[zz][yy][xx] >> 16) & 0xFF);
				out.push_back((in[zz][yy][xx] >> 8) & 0xFF);
				out.push_back((in[zz][yy][xx] >> 0) & 0xFF);
			}
		}
	}
}
void KUtils::SerializeVector(Vx< Vx< Vx<U16> > >& in, Vx<U08>& out)
{
	out.push_back((in.size() >> 24) & 0xFF);
	out.push_back((in.size() >> 16) & 0xFF);
	out.push_back((in.size() >> 8) & 0xFF);
	out.push_back((in.size() >> 0) & 0xFF);
	for(IND zz = 0; zz < (INT)in.size(); ++zz)
	{
		out.push_back((in[zz].size() >> 24) & 0xFF);
		out.push_back((in[zz].size() >> 16) & 0xFF);
		out.push_back((in[zz].size() >> 8) & 0xFF);
		out.push_back((in[zz].size() >> 0) & 0xFF);
		for(IND yy = 0; yy < (INT)in[zz].size(); ++yy)
		{
			out.push_back((in[zz][yy].size() >> 24) & 0xFF);
			out.push_back((in[zz][yy].size() >> 16) & 0xFF);
			out.push_back((in[zz][yy].size() >> 8) & 0xFF);
			out.push_back((in[zz][yy].size() >> 0) & 0xFF);
			for(IND xx = 0; xx < (INT)in[zz][yy].size(); ++xx)
			{
				out.push_back((in[zz][yy][xx] >> 24) & 0xFF);
				out.push_back((in[zz][yy][xx] >> 16) & 0xFF);
				out.push_back((in[zz][yy][xx] >> 8) & 0xFF);
				out.push_back((in[zz][yy][xx] >> 0) & 0xFF);
			}
		}
	}
}
void KUtils::SerializeVector(Vx< Vx< Vx<U32> > >& in, Vx<U08>& out)
{
	out.push_back((in.size() >> 24) & 0xFF);
	out.push_back((in.size() >> 16) & 0xFF);
	out.push_back((in.size() >> 8) & 0xFF);
	out.push_back((in.size() >> 0) & 0xFF);
	for(IND zz = 0; zz < (INT)in.size(); ++zz)
	{
		out.push_back((in[zz].size() >> 24) & 0xFF);
		out.push_back((in[zz].size() >> 16) & 0xFF);
		out.push_back((in[zz].size() >> 8) & 0xFF);
		out.push_back((in[zz].size() >> 0) & 0xFF);
		for(IND yy = 0; yy < (INT)in[zz].size(); ++yy)
		{
			out.push_back((in[zz][yy].size() >> 24) & 0xFF);
			out.push_back((in[zz][yy].size() >> 16) & 0xFF);
			out.push_back((in[zz][yy].size() >> 8) & 0xFF);
			out.push_back((in[zz][yy].size() >> 0) & 0xFF);
			for(IND xx = 0; xx < (INT)in[zz][yy].size(); ++xx)
			{
				out.push_back((in[zz][yy][xx] >> 24) & 0xFF);
				out.push_back((in[zz][yy][xx] >> 16) & 0xFF);
				out.push_back((in[zz][yy][xx] >> 8) & 0xFF);
				out.push_back((in[zz][yy][xx] >> 0) & 0xFF);
			}
		}
	}
}
void KUtils::SerializeVector(Vx< Vx< Vx<S08> > >& in, Vx<U08>& out)
{
	out.push_back((in.size() >> 24) & 0xFF);
	out.push_back((in.size() >> 16) & 0xFF);
	out.push_back((in.size() >> 8) & 0xFF);
	out.push_back((in.size() >> 0) & 0xFF);
	for(IND zz = 0; zz < (INT)in.size(); ++zz)
	{
		out.push_back((in[zz].size() >> 24) & 0xFF);
		out.push_back((in[zz].size() >> 16) & 0xFF);
		out.push_back((in[zz].size() >> 8) & 0xFF);
		out.push_back((in[zz].size() >> 0) & 0xFF);
		for(IND yy = 0; yy < (INT)in[zz].size(); ++yy)
		{
			out.push_back((in[zz][yy].size() >> 24) & 0xFF);
			out.push_back((in[zz][yy].size() >> 16) & 0xFF);
			out.push_back((in[zz][yy].size() >> 8) & 0xFF);
			out.push_back((in[zz][yy].size() >> 0) & 0xFF);
			for(IND xx = 0; xx < (INT)in[zz][yy].size(); ++xx)
			{
				out.push_back((in[zz][yy][xx] >> 24) & 0xFF);
				out.push_back((in[zz][yy][xx] >> 16) & 0xFF);
				out.push_back((in[zz][yy][xx] >> 8) & 0xFF);
				out.push_back((in[zz][yy][xx] >> 0) & 0xFF);
			}
		}
	}
}
void KUtils::SerializeVector(Vx< Vx< Vx<S16> > >& in, Vx<U08>& out)
{
	out.push_back((in.size() >> 24) & 0xFF);
	out.push_back((in.size() >> 16) & 0xFF);
	out.push_back((in.size() >> 8) & 0xFF);
	out.push_back((in.size() >> 0) & 0xFF);
	for(IND zz = 0; zz < (INT)in.size(); ++zz)
	{
		out.push_back((in[zz].size() >> 24) & 0xFF);
		out.push_back((in[zz].size() >> 16) & 0xFF);
		out.push_back((in[zz].size() >> 8) & 0xFF);
		out.push_back((in[zz].size() >> 0) & 0xFF);
		for(IND yy = 0; yy < (INT)in[zz].size(); ++yy)
		{
			out.push_back((in[zz][yy].size() >> 24) & 0xFF);
			out.push_back((in[zz][yy].size() >> 16) & 0xFF);
			out.push_back((in[zz][yy].size() >> 8) & 0xFF);
			out.push_back((in[zz][yy].size() >> 0) & 0xFF);
			for(IND xx = 0; xx < (INT)in[zz][yy].size(); ++xx)
			{
				out.push_back((in[zz][yy][xx] >> 24) & 0xFF);
				out.push_back((in[zz][yy][xx] >> 16) & 0xFF);
				out.push_back((in[zz][yy][xx] >> 8) & 0xFF);
				out.push_back((in[zz][yy][xx] >> 0) & 0xFF);
			}
		}
	}
}
void KUtils::SerializeVector(Vx< Vx< Vx<S32> > >& in, Vx<U08>& out)
{
	out.push_back((in.size() >> 24) & 0xFF);
	out.push_back((in.size() >> 16) & 0xFF);
	out.push_back((in.size() >> 8) & 0xFF);
	out.push_back((in.size() >> 0) & 0xFF);
	for(IND zz = 0; zz < (INT)in.size(); ++zz)
	{
		out.push_back((in[zz].size() >> 24) & 0xFF);
		out.push_back((in[zz].size() >> 16) & 0xFF);
		out.push_back((in[zz].size() >> 8) & 0xFF);
		out.push_back((in[zz].size() >> 0) & 0xFF);
		for(IND yy = 0; yy < (INT)in[zz].size(); ++yy)
		{
			out.push_back((in[zz][yy].size() >> 24) & 0xFF);
			out.push_back((in[zz][yy].size() >> 16) & 0xFF);
			out.push_back((in[zz][yy].size() >> 8) & 0xFF);
			out.push_back((in[zz][yy].size() >> 0) & 0xFF);
			for(IND xx = 0; xx < (INT)in[zz][yy].size(); ++xx)
			{
				out.push_back((in[zz][yy][xx] >> 24) & 0xFF);
				out.push_back((in[zz][yy][xx] >> 16) & 0xFF);
				out.push_back((in[zz][yy][xx] >> 8) & 0xFF);
				out.push_back((in[zz][yy][xx] >> 0) & 0xFF);
			}
		}
	}
}
