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
      real  PI
      parameter        (PI = 3.141592654)
      real  degrad
      parameter        (degrad = PI/180.)
      real  raddeg
      parameter        (raddeg = 180./PI)
      
      integer nalipar               ! Number of alignment 
      parameter (nalipar = 6)

      real*8 xf, yf, tthf, tphf
      real*8 xr, yr, tthr, tphr
      real*8 m1front(4,nalipar),m1rear(4,nalipar)


      real*8 front_old(4), rear_old(4)
      real*8 front_new(4), rear_new(4)

      integer i, j

      real*8 zfront, zrear
      parameter (zfront = 100.D0, 
     &           zrear  = 360.D0)

      front_old(1) = xf + zfront * tphf
      front_old(2) = yf + zfront * tthf
      front_old(3) = atan(tthf)*raddeg
      front_old(4) = atan(tphf)*raddeg

      rear_old(1) = xr + zrear * tphr
      rear_old(2) = yr + zrear * tthr
      rear_old(3) = atan(tthr)*raddeg
      rear_old(4) = atan(tphr)*raddeg
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
c     &        + m1rear(i, 5) * rear_old(1)*rear_old(2)
      enddo
C
      tthf = tan(front_new(3)*degrad)
      tphf = tan(front_new(4)*degrad)
      xf = front_new(1) - zfront * tphf
      yf = front_new(2) - zfront * tthf
                           
      tthr = tan(rear_new(3)*degrad)
      tphr = tan(rear_new(4)*degrad)
      xr = rear_new(1) - zrear * tphr
      yr = rear_new(2) - zrear * tthr
      end









