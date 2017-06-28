      subroutine trackit(nt,zt,xt,dxt,ut,dxf,lrf,x0,tht,chif,chisave)
ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
c       fits hits nominal positions z, x, xdrift to a straight line
c       x = mz + b
c
c       ntr      number of hits
c       ztra     z positions of track hits (cm)
c       xtra     x positions of track hits (cm)
c       dxtra    drift distances for track (cm)
c       dxfit    (fit distance from wire - drift distance from wire) (cm)
c       lr       ``left/right'' of track at each wire
c       x0       b of the fit (cm)
c       thtx     arctan(m) from the fit (mr)
c       chibest  best fit chi square, assuming sigma = 0.1 (cm, if all else is)
c
c       first find best fit using wire positions only, to get starting estimate
c                 of track
c       then loop over each combination of l/r's wth drift distances, to get
c                 best fit for each combination, leading to best fit
c       iterate using previous theta values to start another fit to improve
c                 theta; it appears wires + 2 iterations with drift distances
c                 are quite enough.
c

      implicit none

      real*8 zt(25), xt(25), dxt(25), dxf(25), x0, tht, chif
      real*8 chisave(25)
      integer*4 nt, lrf(25), nlrin
      integer*4 lr(25), i, j, ibetin, ibetter
      integer*4 nloop
      integer*4 nlr, ju(25), ut(25), ninput
      real*8    xx(25), zz(25), dxx(25), del(25)
      real*8    driftfit(25), floop(25)
      real*8    xbar, ybar, xsqbar, xybar
      real*8    slope, ctg, stg, rcept
      real*8    chisq, dx2, chibest
      real*8    chiterm(25), diff
      real*8    sigma/0.020/, pi/3.1415926/
      
c     first move input data into local arrays
      j = 0
      do i = 1, nt
 900     j = j + 1
	 if(j.gt.25)then
            write(*,*)' err in trackit -- found',i-1,' of',nt,
     $           ' hits w/',j-1,' inputs'
            chif = 9999999.d0
            return
         endif
         if(ut(j).lt.1)go to 900
         lr(i) = 0
         ju(i) = j
         chisave(i) = 0.d0
      enddo
      ninput = j
      nlr = nt
      nlrin = nlr
c     other setup
 1    ibetin = 2
      chibest = 999998.d0
c     first estimate theta from wire positions only
      xbar   = 0.d0
      ybar   = 0.d0
      xsqbar = 0.d0
      xybar  = 0.d0
      do i = 1, nt
         dxx(i) = dxt(ju(i))
         xx(i) = xt(ju(i))
         zz(i) = zt(ju(i))
         xbar   = xbar + zz(i)
         xsqbar = xsqbar + zz(i)**2
         ybar   = ybar + xx(i)
         xybar  = xybar + zz(i)*xx(i)
      enddo
      xbar   = xbar / nlr
      xsqbar = xsqbar / nlr
      ybar   = ybar / nlr
      xybar  = xybar / nlr
      diff   = xsqbar - xbar**2
      if(diff.ne.0.)then
         slope  = (xybar - xbar*ybar) / diff
         ctg    = 1.d0/sqrt(1.d0+slope**2)
         stg    = slope*ctg
      else
         ctg    = 0.d0
         stg    = 1.d0
      endif
c     
      nloop = 2**nlr
      do i = 1, nloop
c     set up all +/- combinations to loop over
         call seti(nlr,i,floop)
         ibetter = 1
 100     xbar   = 0.d0
         ybar   = 0.d0
         xsqbar = 0.d0
         xybar  = 0.d0
         do j = 1, nlr
	    xx(j) = xt(ju(j)) + floop(j)*dxx(j)*ctg
	    zz(j) = zt(ju(j)) - floop(j)*dxx(j)*stg
c     calculate best track for this combination
c     y=f(x) <---> x=f(z)
	    xbar   = xbar + zz(j)
	    xsqbar = xsqbar + zz(j)**2
	    ybar   = ybar + xx(j)
	    xybar  = xybar + zz(j)*xx(j)
         enddo
         xbar   = xbar / nlr
         xsqbar = xsqbar / nlr
         ybar   = ybar / nlr
         xybar  = xybar / nlr
         diff   = xsqbar - xbar**2
         if(diff.ne.0.)then
	    slope  = (xybar - xbar*ybar) / diff
	    ctg    = 1.d0/sqrt(1.d0+slope**2)
	    stg    = slope*ctg
	    rcept  = -1.d0*(xybar*xbar-xsqbar*ybar) / diff
         else
	    ctg    = 0.d0
	    stg    = 1.d0
	    rcept = -999999.d0
	    slope = 999999.d0
         endif
c     iterate a few times to converge
         if(ibetter.le.ibetin)then
	    ibetter = ibetter + 1
	    go to 100
         endif
c     calculate chisq
         chisq = 0.d0
         do j = 1, nlr
	    dx2 = slope * zt(ju(j)) + rcept - xt(ju(j))
            driftfit(j) = dx2 * ctg
c     adjust dxfit sign if track on opposite side of wire from drift distance
c     dx2>0 --> track on + side of wire, dx2<0 --> track on - side of wire
c     lr = 1--> track on + side of wire, lr=-1 --> track on - side of wire
c     use floop if lr = 0...    note dx always +
c...  note: lr always 0 in Los Alamos case, but do not change code below
            if(driftfit(j).gt.0) then !track on + side of wire
               if(lr(j).eq.1 .or. (lr(j).eq.0.and.floop(j).gt.0.)) then
                  del(j) = driftfit(j) - dxx(j)	!track +, drift +
               else
                  del(j) = driftfit(j) + dxx(j)	!track +, drift -
               endif
            else                !track on - side of wire
               if(lr(j).eq.-1 .or. (lr(j).eq.0.and.floop(j).lt.0.))then
                  del(j) = driftfit(j) + dxx(j) !track -, drift -
               else
                  del(j) = driftfit(j) - dxx(j) !track -, drift +
               endif
            endif
	    chiterm(j) = (del(j)/sigma)**2
	    chisq = chisq + chiterm(j)
         enddo
c     save if best fit so far
         if(chisq.lt.chibest)then
c     save lr input to fit, not side of wire of fit
c     do j = 1, nlr
c     dxf(ju(j)) = 0.
c     lrf(ju(j)) = 0
c     enddo
	    do j = 1, nlr
               dxf(ju(j)) = del(j)
               lrf(ju(j)) = 0
               if(floop(j).lt.0.)then
                  lrf(ju(j)) = -1
               else
                  lrf(ju(j)) = 1
               endif
               chisave(j) = chiterm(j)
            enddo
	    tht = slope
            x0 = rcept
	    chibest = chisq
         endif
      enddo
      
      chif = chibest
      if(chif.lt.0.)chif = 0.d0
      chif = sqrt(chif/nt)
      
      return
      end
