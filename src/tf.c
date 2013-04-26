/*Daala video codec
Copyright (c) 2013 Daala project contributors.  All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

- Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

- Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS “AS IS”
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.*/
#include "tf.h"

/*Increase horizontal frequency resolution of an entire block and return the LF
   half.*/
void od_tf_up_h_lp(od_coeff *_dst,int _dstride,
 const od_coeff *_src,int _sstride,int _dx,int _n){
  int x;
  int y;
  for(y=0;y<_n;y++){
    for(x=0;x<_n>>1;x++){
      od_coeff ll;
      od_coeff lh;
      int      hswap;
      ll=_src[y*_sstride+x];
      lh=_src[y*_sstride+x+_dx];
      lh=ll-lh;
      ll-=OD_DCT_RSHIFT(lh,1);
      hswap=x&1;
      _dst[y*_dstride+2*x+hswap]=ll;
      _dst[y*_dstride+2*x+1-hswap]=lh;
    }
  }
}

/*Increase vertical frequency resolution of an entire block and return the LF
   half.*/
void od_tf_up_v_lp(od_coeff *_dst,int _dstride,
 const od_coeff *_src,int _sstride,int _dy,int _n){
  int x;
  int y;
  for(y=0;y<_n>>1;y++){
    int vswap;
    vswap=y&1;
    for(x=0;x<_n;x++){
      od_coeff ll;
      od_coeff hl;
      ll=_src[y*_sstride+x];
      hl=_src[(y+_dy)*_sstride+x];
      hl=ll-hl;
      ll-=OD_DCT_RSHIFT(hl,1);
      _dst[(2*y+vswap)*_dstride+x]=ll;
      _dst[(2*y+1-vswap)*_dstride+x]=hl;
    }
  }
}

/*Increase horizontal and vertical frequency resolution of an entire block and
   return the LF quarter.*/
void od_tf_up_hv_lp(od_coeff *_dst,int _dstride,
 const od_coeff *_src,int _sstride,int _dx,int _dy,int _n){
  int x;
  int y;
  for(y=0;y<_n>>1;y++){
    int vswap;
    vswap=y&1;
    for(x=0;x<_n>>1;x++){
      od_coeff ll;
      od_coeff lh;
      od_coeff lh_2;
      od_coeff hl;
      od_coeff hl_2;
      od_coeff hh;
      int      hswap;
      ll=_src[y*_sstride+x];
      lh=_src[y*_sstride+x+_dx];
      hl=_src[(y+_dy)*_sstride+x];
      hh=_src[(y+_dy)*_sstride+x+_dx];
      hl=ll-hl;
      lh+=hh;
      hl_2=OD_DCT_RSHIFT(hl,1);
      lh_2=OD_DCT_RSHIFT(lh,1);
      ll-=hl_2;
      hh=lh_2-hh;
      ll+=lh_2;
      hh=hl_2-hh;
      lh=ll-lh;
      hl-=hh;
      hswap=x&1;
      _dst[(2*y+vswap)*_dstride+2*x+hswap]=ll;
      _dst[(2*y+vswap)*_dstride+2*x+1-hswap]=lh;
      _dst[(2*y+1-vswap)*_dstride+2*x+hswap]=hl;
      _dst[(2*y+1-vswap)*_dstride+2*x+1-hswap]=hh;
    }
  }
}

/*Increase horizontal and vertical frequency resolution of a 2x2 group of
   blocks, combining them into a single block.*/
void od_tf_up_hv(od_coeff *_dst,int _dstride,
 const od_coeff *_src,int _sstride,int _n){
  int x;
  int y;
  for(y=0;y<_n;y++){
    int vswap;
    vswap=y&1;
    for(x=0;x<_n;x++){
      od_coeff ll;
      od_coeff lh;
      od_coeff lh_2;
      od_coeff hl;
      od_coeff hl_2;
      od_coeff hh;
      int      hswap;
      ll=_src[y*_sstride+x];
      lh=_src[y*_sstride+x+_n];
      hl=_src[(y+_n)*_sstride+x];
      hh=_src[(y+_n)*_sstride+x+_n];
      /*This kernel is identical to that of od_tf_down_hv with the roles of
         hl and lh swapped.*/
      hl=ll-hl;
      lh+=hh;
      hl_2=OD_DCT_RSHIFT(hl,1);
      lh_2=OD_DCT_RSHIFT(lh,1);
      ll-=hl_2;
      hh=lh_2-hh;
      ll+=lh_2;
      hh=hl_2-hh;
      lh=ll-lh;
      hl-=hh;
      hswap=x&1;
      _dst[(2*y+vswap)*_dstride+2*x+hswap]=ll;
      _dst[(2*y+vswap)*_dstride+2*x+1-hswap]=lh;
      _dst[(2*y+1-vswap)*_dstride+2*x+hswap]=hl;
      _dst[(2*y+1-vswap)*_dstride+2*x+1-hswap]=hh;
    }
  }
}

/*Increase horizontal and vertical time resolution of a block, splitting it
   into a 2x2 group of blocks.*/
void od_tf_down_hv(od_coeff *_dst,int _dstride,
 const od_coeff *_src,int _sstride,int _n){
  int x;
  int y;
  OD_ASSERT(!(_n&1));
  for(y=0;y<_n>>1;y++){
    int vswap;
    vswap=y&1;
    for(x=0;x<_n>>1;x++){
      od_coeff ll;
      od_coeff lh;
      od_coeff lh_2;
      od_coeff hl;
      od_coeff hl_2;
      od_coeff hh;
      int      hswap;
      hswap=x&1;
      ll=_src[(2*y+vswap)*_sstride+2*x+hswap];
      lh=_src[(2*y+vswap)*_sstride+2*x+1-hswap];
      hl=_src[(2*y+1-vswap)*_sstride+2*x+hswap];
      hh=_src[(2*y+1-vswap)*_sstride+2*x+1-hswap];
      /*This kernel is identical to that of od_tf_up_hv with the roles of
         hl and lh swapped.*/
      lh=ll-lh;
      hl+=hh;
      lh_2=OD_DCT_RSHIFT(lh,1);
      hl_2=OD_DCT_RSHIFT(hl,1);
      ll-=lh_2;
      hh=hl_2-hh;
      ll+=hl_2;
      hh=lh_2-hh;
      hl=ll-hl;
      lh-=hh;
      _dst[y*_dstride+x]=ll;
      _dst[y*_dstride+x+_n]=lh;
      _dst[(y+_n)*_dstride+x]=hl;
      _dst[(y+_n)*_dstride+x+_n]=hh;
    }
  }
}