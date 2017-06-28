      subroutine seti(ntr,i,floop)
c
c       subroutine to set phases for loop combinations
c        
      
      implicit none
      integer*4 ntr, i
      real*8 floop(15)
      integer*4 j, k, jn, jrem
c       
      j = i-1
      do k = 1, ntr
         jn   = j/2
         jrem = j - 2*jn
         if(jrem.eq.0)then
            floop(k) = 1.d0
         else
            floop(k) = -1.d0
         endif
         j = jn
      enddo

      return
      end
