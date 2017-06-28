      function fpplast(thits,tsum,thist,t0
     &                    ,calibration,icalib,debug)
c
c Written by Ed Brash - January 23, 1996 - to calculate time offsets
c from statistics accumulated during calibration runs.
c
c Modified by Ed Brash - Febuary 02, 1996 - to calculate alignment 
c corrections at end of run during alignment calibration runs.
c
c Called from UGLAST routine in GEANT simulations. 
c
c
c      include 'fpp_local.h'
c      include 'option.h'
c
      character*40 outfile
      character*40 ofile
      character*40 ffile
      character*40 rfile
      real*8 thits(24,40),tsum(24,40)
      integer*2 thist(24,40,80)
      real*8 t0(24,40)
      integer*4 calibration,icalib,debug
      integer*4 ii,jj,lochan,hichan,midchan,kk,nn,icheck
      real*8 sum,avg,runavg,loavg,hiavg
      real*8 xdelta,xdeterm
      real*8 ydelta,ydeterm
      real*8 tdelta,tdeterm
      real*8 pdelta,pdeterm
      real*8 xdeltar,xdetermr
      real*8 ydeltar,ydetermr
      real*8 tdeltar,tdetermr
      real*8 pdeltar,pdetermr
c
      data outfile / 'fpp_toffset_crude_new.calib' /
      data ofile / 'fpp_toffset_ref_new.calib' /
c      data ffile / 'fpp_alignment_matrix_front_new.dat' /
c      data rfile / 'fpp_alignment_matrix_rear_new.dat' /
c
      if(debug.gt.0)write(25,*)calibration,icalib
      if(calibration.gt.0) then
         write(25,*)'**********************************************'
         write(25,*)'******* End of Run Calibration Routine *******'
         write(25,*)'**********************************************'
         if(icalib.eq.0) then
            open(unit=1,file=outfile,status='new')
            do ii=1,24
               do jj=1,40
                  if(thits(ii,jj).gt.0.0) t0(ii,jj)=
     &                 tsum(ii,jj)/thits(ii,jj)+50.0
                  write(25,*)'Plane ',ii,' Group ',jj, 
     &                 ' Nhits =',thits(ii,jj),
     &                 ' Time Offset =',t0(ii,jj)
                  write(1,*)t0(ii,jj)
               enddo
            enddo
         endif
         if(icalib.eq.1) then
            open(unit=1,file=ofile,status='new')
            do ii=1,24
               do kk=1,40
                  sum=0.0
                  do jj=60,80
                     sum=sum+thist(ii,kk,jj)
                  enddo
                  avg=sum/21.0
                  loavg=0.05*avg
                  hiavg=0.95*avg
                  sum=0.0
                  lochan=80
                  do jj=1,80
                     if(jj.le.20) then
                        sum=sum+thist(ii,kk,jj)
                     else
                        sum=sum+thist(ii,kk,jj)-thist(ii,kk,jj-20)
                        runavg=sum/20.0
                     endif
                     if(runavg.gt.loavg.and.jj.gt.20) then
                        lochan=jj
                        goto 1123
                     endif
                  enddo
 1123             sum=0.0
                  hichan=1
                  do jj=80,1,-1
                     if(jj.gt.60) then
                        sum=sum+thist(ii,kk,jj)
                     else
                        sum=sum+thist(ii,kk,jj)-thist(ii,kk,jj+20)
                        runavg=sum/20.0
                     endif
                     if(runavg.lt.hiavg.and.jj.le.60) then
                        hichan=jj
                        goto 1124
                     endif
                  enddo
 1124             if(lochan.lt.hichan) then
                     midchan=(lochan+hichan)/2
                  else
                     midchan=40
                  endif
                  write(25,*)'Plane =',ii,' Group =',kk,
     &                 ' Lochan =',lochan,
     &                 ' Hichan =',hichan,
     &                 ' Midchan =',midchan
                  write(1,*)(midchan-40.0)
               enddo
            enddo
            close(unit=1)
         endif
         if(icalib.eq.2) then
            open(unit=1,file=ofile,status='new')
            do ii=1,24
               do kk=1,40
                  hichan=80
                  lochan=1
                  icheck=0
                  do jj=1,80
                     if(thist(ii,kk,jj).gt.0.and.icheck.eq.0) then
                        lochan=jj
                        icheck=1
                        goto 999
                     endif
                     if(icheck.eq.1) then
                        if(thist(ii,kk,jj).gt.0) then
                           hichan=jj
                           goto 998
                        else
                           icheck=0
                        endif
                     endif
 999              enddo
                  
 998              continue
                  
                  write(25,*)'Plane =',ii,' Group =',kk,
     &                 ' Lochan =',lochan,
     &                 ' Hichan =',hichan
                  if(lochan.ge.20.and.lochan.le.60) then
                     write(1,*)(lochan-40.0)
                  else
                    lochan=40
                    write(1,*)(lochan-40.0)
                 endif
                 
              enddo
           enddo
           close(unit=1)
        endif
      endif
c     
c     Calculate the aligment correction coefficients
c     
      
C      if(alignment) then
C        ...
C        code removed; now in a stand alown code, SD
C        ...
C      endif

      pdelta=0.0
      close(unit=1)

      if(pdelta.eq.0.) goto 1477

 1476 write(25,*)'Error opening file - alignment data not saved'
c     
 1477 return
      end
