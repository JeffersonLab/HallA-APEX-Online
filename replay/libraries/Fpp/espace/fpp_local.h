C This file is included in all of the GEANT user routines that make use
c of the common block /myinfo/. (E.J. Brash, Rutgers - August 17,1994)
c
        integer*4 n_2nd,nevent,n_react,n_escape
	integer*4 istraw,iplane,iptype,nhit
	integer*4 ifu1str,ifu2str,ifu3str
	integer*4 ifu4str,ifu5str,ifu6str
	integer*4 ifv1str,ifv2str,ifv3str
	integer*4 ifv4str,ifv5str,ifv6str
	integer*4 iru1str,iru2str,iru3str
	integer*4 iru4str,iru5str,irx1str
	integer*4 irv1str,irv2str,irv3str
	integer*4 irv4str,irv5str,irx2str
        integer*4 nc,nmax
        real*8 xsumx,xsumy,xarray,m1front
        real*8 ysumx,ysumy,yarray,m1rear
        real*8 tsumx,tsumy,tarray
        real*8 psumx,psumy,parray
        real*8 xsumxr,xsumyr,xarrayr
        real*8 ysumxr,ysumyr,yarrayr
        real*8 tsumxr,tsumyr,tarrayr
        real*8 psumxr,psumyr,parrayr
	real*8 xs,ys,zs,x2,y2,z2,x3,y3,z3,w_spacingr,w_spacingf
	real*8 xa,ya,za,dist,hit,pi,sth,dtor,rtod
        real*8 xoff1,yoff1,thoff1,phoff1,psoff1
        real*8 xoff2,yoff2,thoff2,phoff2,psoff2
        real*8 xoff3,yoff3,thoff3,phoff3,psoff3
        real*8 xoff4,yoff4,thoff4,phoff4,psoff4
        real*8 xofff,yofff,thofff,phofff,psofff
	real*8 zairf,zcarbonmin,zcarbonmax
	real*8 z_fpp(24),zairg
        real*8 uvx_zero(24)
	real*8 zcarbon,zaira,zairb,zaird,zairc,zaire
        real*8 xinit,yinit,thinit,phiinit
        real*8 tsum
        real*8 thits
        real*8 t0
        real*8 toff
        real*8 psum,phits,pcentr,pwidth
        real*8 uvx_first,w_spacing,zmid,zshift
        real*8 uvrot,zurot,zvrot,uvx_shift,uvno
        character*80 ntuple_file  ! Ntuple file
        integer*4 NTUPLE_ID
        integer*4 NTUPLE_LUNIT
        integer*4 icycle,is
        integer*4 NTUPLE_NVAR         ! Number of variables per event
        integer*4 NTUPLE_NWBUFF       ! Number of words Ntuple allocation
        real*8 ntuple_array
        character*8 ntuple_tags  ! 8 char. maximum
        integer*2 thist
C
C       new alignment method  (SD)
C       --------------------
C
        integer nalipar               ! Number of alignment parameters
        parameter (nalipar = 6)

        real*8 sum_bf(4,5), sum_br(4,5)
        real*8 sum_Af(5,5), sum_Ar(5,5)
        common / myinfo / sum_bf, sum_Af, sum_br, sum_Ar
c
        common / myinfo / nevent,n_2nd,n_react,n_escape
	common / myinfo / pi,sth,rtod,dtor
	common / myinfo / z_fpp,zairf
	common / myinfo / zairg
        common / myinfo / uvx_zero
	common / myinfo / istraw,iplane,iptype,nhit(25)
	common / myinfo / xs,ys,zs,x2,y2,z2,x3,y3,z3
        common / myinfo / w_spacingr,w_spacingf,zcarbonmin,zcarbonmax
	common / myinfo / xa,ya,za,dist,hit(5,35,24)
	common / myinfo / zcarbon,zaira,zairb,zaird,zairc,zaire
	common / myinfo / ifu1str,ifu2str,ifu3str
	common / myinfo / ifu4str,ifu5str,ifu6str
	common / myinfo / ifv1str,ifv2str,ifv3str
	common / myinfo / ifv4str,ifv5str,ifv6str
	common / myinfo / iru1str,iru2str,iru3str
	common / myinfo / iru4str,iru5str,irx1str
	common / myinfo / irv1str,irv2str,irv3str
	common / myinfo / irv4str,irv5str,irx2str
        common / myinfo / thits(24,40),tsum(24,40),t0(24,40),toff(24,40)
        common / myinfo / phits(24,40,8),psum(24,40,8)
        common / myinfo / pcentr(24,40,8),pwidth(24,40,8)
        common / myinfo / xinit,yinit,thinit,phiinit,thist(24,40,80)
        common / myinfo / NTUPLE_ID,NTUPLE_LUNIT,icycle,NTUPLE_NVAR
        common / myinfo / NTUPLE_NWBUFF,ntuple_file
        common / myinfo / ntuple_tags(20),is,ntuple_array(20)
        common / myinfo / xoff1,yoff1,thoff1,phoff1,psoff1
        common / myinfo / xoff2,yoff2,thoff2,phoff2,psoff2
        common / myinfo / xoff3,yoff3,thoff3,phoff3,psoff3
        common / myinfo / xoff4,yoff4,thoff4,phoff4,psoff4
        common / myinfo / xofff,yofff,thofff,phofff,psofff
c        common / myinfo / m1front(4,nalipar)
        common / myinfo / xsumx(19),xsumy(10),xarray(10,10)
        common / myinfo / ysumx(19),ysumy(10),yarray(10,10)
        common / myinfo / tsumx(19),tsumy(10),tarray(10,10)
        common / myinfo / psumx(19),psumy(10),parray(10,10)
c        common / myinfo / m1rear(4,6)
        common / myinfo / ysumxr(19),ysumyr(10),
     &          yarrayr(10,10)
        common / myinfo / tsumxr(19),tsumyr(10),
     &          tarrayr(10,10)
        common / myinfo / psumxr(19),psumyr(10),
     &          parrayr(10,10)
        common / myinfo / uvx_first(24),w_spacing(24),
     & uvrot(24),zurot(24),zvrot(24)
     & ,uvx_shift(24),zmid(24),zshift(24),uvno(24)

c
c



