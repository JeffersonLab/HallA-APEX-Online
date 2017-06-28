c +----------------------------------------------------------------+
c |                                                                |
c |    New alignment method, based on Ed Brash's                   |
c |    track alignment method.                                     |
c |                                                                |
c +----------------------------------------------------------------+

      subroutine aligncorr(xf,yf,tthf,tphf,
     &                           xr,yr,tthr,tphr,m1front,m1rear)

      implicit none

c      include 'fpp_local.h'
C
C     front and rear track coordinates
C
      integer nalipar               ! Number of alignment parameters
      parameter (nalipar = 6)
      double precision raddeg
      parameter (raddeg = 180.00000d0/3.141592653589793d0)
      double precision xf, yf, tthf, tphf
      double precision xr, yr, tthr, tphr
      double precision m1front(4,nalipar), m1rear(4,6)
      double precision front_old(4), rear_old(4)
      double precision front_new(4), rear_new(4)

      integer i, j

      double precision zfront, zrear
      parameter (zfront = 100.D0, 
     &           zrear  = 360.D0)
c      do i=1,4
c        write(*,*)(m1front(i,j),j=1,6)
c      enddo
c      do i=1,4
c        write(*,*)(m1rear(i,j),j=1,6)
c      enddo


      front_old(1) = xf*1.d0 + zfront * tphf*1.d0
      front_old(2) = yf*1.d0 + zfront * tthf*1.d0
      front_old(3) = datan(tthf)*raddeg
      front_old(4) = datan(tphf)*raddeg

      rear_old(1) = xr*1.d0 + zrear * tphr*1.d0
      rear_old(2) = yr*1.d0 + zrear * tthr*1.d0
      rear_old(3) = datan(tthr)*raddeg
      rear_old(4) = datan(tphr)*raddeg
C      if (abs(front_old(4)-1.905).lt.0.003) then
C         write (*,'(8f10.3)') (front_old(i),i=1,4), 
C     &             (rear_old(i),i=1,4)
C      endif

      do i = 1, 4
         front_new(i) = front_old(i)
     &        + m1front(i, nalipar)*1.d0
     &        + m1front(i, 1)*1.d0* front_old(1) 
     &        + m1front(i, 2)*1.d0* front_old(2)
     &        + m1front(i, 3)*1.d0* front_old(1)**2
     &        + m1front(i, 4)*1.d0 * front_old(2)**2
     &        + m1front(i, 5)*1.d0 * front_old(1)*front_old(2)
         rear_old(i) = rear_old(i) + m1front(i, nalipar)*1.d0
      enddo
C
C     align 'vdc-aligned rear track' with second set
C     of alignment coefficients
C
      do i = 1, 4
         rear_new(i) = rear_old(i)
     &        + m1rear(i, nalipar)*1.d0
     &        + m1rear(i, 1)*1.d0 * rear_old(1) 
     &        + m1rear(i, 2)*1.d0 * rear_old(2)
     &        + m1rear(i, 3)*1.d0 * rear_old(1)**2
     &        + m1rear(i, 4)*1.d0 * rear_old(2)**2
     &        + m1rear(i, 5)*1.d0 * rear_old(1)*rear_old(2)
      enddo
C
      tthf = dtan(front_new(3)/raddeg)
      tphf = dtan(front_new(4)/raddeg)
      xf = front_new(1) - zfront * tphf
      yf = front_new(2) - zfront * tthf
                           
      tthr = dtan(rear_new(3)/raddeg)
      tphr = dtan(rear_new(4)/raddeg)
      xr = rear_new(1) - zrear * tphr
      yr = rear_new(2) - zrear * tthr
      end








