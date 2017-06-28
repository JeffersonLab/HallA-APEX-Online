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
      real*8 raddeg
      parameter (raddeg = 180.00000d0/3.141592653589793d0)
      real*8 xf, yf, tthf, tphf
      real*8 xr, yr, tthr, tphr
      real*8 m1front(4,nalipar), m1rear(4,6)
      real*8 front_old(4), rear_old(4)
      real*8 front_new(4), rear_new(4)

      integer i, j

      real*8 zfront, zrear
      parameter (zfront = 100.D0, 
     &           zrear  = 360.D0)
      do i=1,4
        write(*,*)(m1front(i,j),j=1,6)
      enddo
      do i=1,4
        write(*,*)(m1rear(i,j),j=1,6)
      enddo


      front_old(1) = xf + zfront * tphf
      front_old(2) = yf + zfront * tthf
      front_old(3) = datan(tthf)*raddeg
      front_old(4) = datan(tphf)*raddeg

      rear_old(1) = xr + zrear * tphr
      rear_old(2) = yr + zrear * tthr
      rear_old(3) = datan(tthr)*raddeg
      rear_old(4) = datan(tphr)*raddeg
C      if (abs(front_old(4)-1.905).lt.0.003) then
C         write (*,'(8f10.3)') (front_old(i),i=1,4), 
C     &             (rear_old(i),i=1,4)
C      endif

      do i = 1, 4
         front_new(i) = front_old(i)
     &        + m1front(i, nalipar)
     &        + m1front(i, 1) * front_old(1) 
     &        + m1front(i, 2) * front_old(2)
     &        + m1front(i, 3) * front_old(1)**2
     &        + m1front(i, 4) * front_old(2)**2
     &        + m1front(i, 5) * front_old(1)*front_old(2)
         rear_old(i) = rear_old(i) + m1front(i, nalipar)
      enddo
C
C     align 'vdc-aligned rear track' with second set
C     of alignment coefficients
C
      do i = 1, 4
         rear_new(i) = rear_old(i)
     &        + m1rear(i, nalipar)
     &        + m1rear(i, 1) * rear_old(1) 
     &        + m1rear(i, 2) * rear_old(2)
     &        + m1rear(i, 3) * rear_old(1)**2
     &        + m1rear(i, 4) * rear_old(2)**2
     &        + m1rear(i, 5) * rear_old(1)*rear_old(2)
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








