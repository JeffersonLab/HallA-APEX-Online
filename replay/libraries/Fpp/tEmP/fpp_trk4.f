      function trk4(itrcode,nhit,hit,ntrack,track
     &                xsave,zsave,dxsave,plsave,debug)
c
c
c      include 'option.h'
c
      integer*4 nhit(25),debug		!number of hits(plane)
      real*8 hit(5,35,24)	!hit info(info type, hit on plane, plane)
      integer*4 ntrack		!# of tracks
      integer*4 itrcode           !tracking code 
      real*8 track(40,25)	!track information
      integer*4 nh1_tr(10),nh2_tr(10)      ! number of hits per track
      real*8 xsave(6),zsave(6),dxsave(6)
      integer*4 plsave(6)
c
c			track(1,i)= uatz0f
c			     (2,i)= uanglef
c			     (3,i)= vatz0f
c			     (4,i)= vanglef
c			     (5,i)= uatz0r
c			     (6,i)= uangler
c			     (7,i)= vatz0r
c			     (8,i)= vangler
c			     (9,i)= yatz0r
c			     (10,i)= yangler
c                            (11,i)= vfchi
c                            (12,i)= ufchi
c                            (13,i)= vrchi
c                            (14,i)= urchi
c                            (15,i)= nhits in vf
c                            (16,i)= nhits in uf
c                            (17,i)= nhits in vr
c                            (18,i)= nhits in ur
c
c	track4clustrg.for a new algorithm, trying to make use of hit clusters,
c	to try to do multiple track tracking, and simplify conditions on
c	single track tracking...
c	Since each plane has 8 - 10 sections, one can imagine a maximum
c	of 9 tracks found.
c	An individual track can have 0 - 4 hits on a plane.
c	Assumption: all hits are on adjacent straws. If a track passes
c	through 2 or more straws on each of 2 intermixed sections, it will
c	fire the two with the shortest drift distances, which should be
c	adjacent.
c
c      common /effarray/nhgood(80,4), nhbad(80,4), sigbad(80,4)
c      common /doplot/isplot
      integer*4 isplot
c     local variables:
      integer*4 lrfit(25), ipnum(6)
      integer*4 utra(2,25,35), nwtra(2,25,35)
      integer*4 nfplane,nbplane
      integer*4 nhitx, nhitx0
      real*8    xat0
      real*8    chif, slope
      real*8	ztra(2,25,35), xtra(2,25,35), dxtra(2,25,35)
      real*8    chisave(25),chis(25)
      real*8    dxfit(2,25,35)
      integer*4 pltra(2,25,35)
      real*8    zone(25),xone(25),dxone(25),dxfone(25)
      integer*4 uone(25),i,j,k,plone(25)
      integer ilast,ipl1,ipl2,ipl3
      integer ns1,ns2,ns3a,ns3b,ns3c
c
      integer num_tr(2),nhits_tr(2,10)
      logical one_plane_hit(2)
      integer hit_1,hit_2,hit_3,hitnum(6)
      logical track_123(10)
      logical track_found
      logical hit_3_used(10),hit_2_used(10)
      logical track_12,track_23
c
c
c      do i=1,5
c        do j=1,35
c            print*,(hit(i,j,k),k=1,24)
c         enddo
c      enddo
 
      write(*,*) itrcode
      write(*,*) (nhit(i),i=1,25)
      write(*,*) ntrack
      write(*,*) (xsave(i),i=1,6)
      write(*,*) (zsave(i),i=1,6)
      write(*,*) (dxsave(i),i=1,6)
      write(*,*) (plsave(i),i=1,6)
      print*, 'debug = ', debug
      
      if(itrcode.eq.1) then     !Front U Tracking
         ipnum(1)=4
         ipnum(2)=5
         ipnum(3)=6
         ipnum(4)=10
         ipnum(5)=11
         ipnum(6)=12
      endif
      if(itrcode.eq.2) then     !Front V Tracking
         ipnum(1)=1
         ipnum(2)=2
         ipnum(3)=3
         ipnum(4)=7
         ipnum(5)=8
         ipnum(6)=9
      endif
      if(itrcode.eq.3) then     !Rear U Tracking
         ipnum(1)=13
         ipnum(2)=14
         ipnum(3)=25
         ipnum(4)=19
         ipnum(5)=20
         ipnum(6)=21
      endif
      if(itrcode.eq.4) then     !Rear V Tracking
         ipnum(1)=15
         ipnum(2)=16
         ipnum(3)=25
         ipnum(4)=22
         ipnum(5)=23
         ipnum(6)=24
      endif
      if(itrcode.eq.5) then     !XTracking
         ipnum(1)=17
         ipnum(2)=18
         ipnum(3)=25
         ipnum(4)=25
         ipnum(5)=25
         ipnum(6)=25
      endif
c     
      chif = 999999.
      xat0 = -1000.
      slope = -1000.
      isplot = 0
c     
c     start analysis
      if(itrcode.le.4) then
         ntrack = 0
      else
         ntrack=1              
      endif
      nhitx=0
      do i=1,25
         zone(i)=0.0
         xone(i)=0.0
         dxone(i)=0.0
         dxfone(i)=0.0
         uone(i)=0
      enddo
      do i=1,6         
         nhitx = nhitx + nhit(ipnum(i))
         do nchm=1,2
         do j=1,35
            ztra(nchm,i,j)=0.0
            xtra(nchm,i,j)=0.0
            dxtra(nchm,i,j)=0.0
            dxfit(nchm,i,j)=0.0
            utra(nchm,i,j)=0
            pltra(nchm,i,j)=0
         enddo
         enddo
      enddo
      nhitx0 = nhitx
      nfplane = 0
      nbplane= 0
      ilast = ipnum(3)
      if (itrcode .ge. 3) ilast = ipnum(2)
      one_plane_hit(1) = .false.
      one_plane_hit(2) = .false.      
      do i = ipnum(1), ilast
         if(nhit(i).gt.0) nfplane = nfplane + 1
      enddo
      if ( nfplane .eq. 1) one_plane_hit(1) = .true.
      do i = ipnum(4), ipnum(6)
         if(nhit(i).gt.0)nbplane = nbplane + 1
      enddo
      if ( nbplane .eq. 1) one_plane_hit(2) = .true.
c
c
      if (debug .gt. 0) then
	if(itrcode.eq.1) write(25,*)'Front U tracking'
	if(itrcode.eq.2) write(25,*)'Front V tracking'
	if(itrcode.eq.3) write(25,*)'Rear U tracking'
	if(itrcode.eq.4) write(25,*)'Rear V tracking'
       if(itrcode.eq.5) write(25,*)'Rear X tracking'
	write(25,*)'Number of hits  =',nhitx
	write(25,*)'Number of first chamber planes hit =',nfplane
	write(25,*)'Number of second chamber planes hit =',nbplane
      endif
c
c	bag out if too few hits / too few planes
c
      if(itrcode.eq.5) then
         if(nhitx.lt.1 .or. nfplane.lt.2)then
	    return
         endif
      endif
      if(itrcode.eq.1.or.itrcode.eq.2) then
         if(nhitx.lt.3 .or. nbplane.lt.1 .or.
     &        nfplane.lt.1 )then
	    return
         endif
      endif
      if(itrcode.eq.3.or.itrcode.eq.4) then
        if(nhitx.lt.3 .or. nbplane.lt.1
     & .or. nfplane .lt. 1) return
      endif

c
c	bag out if too many hits, > 35 per plane
c
      if(nhitx.gt.210)then
         return
      endif
c
      ipl1=1
      ipl2=2
      ipl3=3
      do 999 nchm=1,2 ! loop through both set of chambers
c
      num_tr(nchm) = 0
      do hit_1=1,10
         nhits_tr(nchm,hit_1) = 0
      enddo
c
      if (one_plane_hit(nchm)) then
      ilast = ipl3
      if (itrcode .ge. 3 .and. nchm .eq. 1) ilast = ipl2
         do i=ipl1,ilast
          do hit_1=1,nhit(ipnum(i))
           if (num_tr(nchm) .ge. 10) goto 888
           num_tr(nchm) = num_tr(nchm) + 1
           nhits_tr(nchm,num_tr(nchm)) = 
     &      nhits_tr(nchm,num_tr(nchm)) + 1
           k = nhits_tr(nchm,num_tr(nchm))
           ztra(nchm,k,num_tr(nchm)) = hit(4,hit_1,ipnum(i))
           pltra(nchm,k,num_tr(nchm)) = hit(5,hit_1,ipnum(i))
           xtra(nchm,k,num_tr(nchm)) = hit(3,hit_1,ipnum(i))
           dxtra(nchm,k,num_tr(nchm)) = hit(2,hit_1,ipnum(i))
           utra(nchm,k,num_tr(nchm)) = 1
           dxfit(nchm,k,num_tr(nchm)) = 0.
           nwtra(nchm,k,num_tr(nchm)) = hit(1,hit_1,ipnum(i))
          enddo   
         enddo
      endif ! oneplane hit
c
      do hit_3=1,min(10,nhit(ipnum(ipl3)))
         track_123(hit_3) = .false.
         hit_3_used(hit_3) = .false.
      enddo
      do hit_2=1,min(10,nhit(ipnum(ipl2)))
         track_123(hit_2) = .false.
         hit_2_used(hit_2) = .false.
      enddo
      if (itrcode .le. 2) then
        ns1 = 1
        ns2 = -1
        ns3a = -1
        ns3b = 0
        ns3c = 1
      endif
      if (itrcode .ge. 3 .and. nchm .eq. 1) then
        ns1 = -1
        ns2 = 0  !  shouldn't be used by code
        ns3 = 0
      endif
      if (itrcode .eq. 3 .and. nchm .eq. 2) then
        ns1 = 1
        ns2 = 1
        ns3a = 2 
        ns3b = 1
        ns3c = 0
      endif
      if (itrcode .eq. 4 .and. nchm .eq. 2) then
        ns1 = -1
        ns2 = -1
        ns3a = -2 
        ns3b = -1
        ns3c = 0
      endif
c
      if ( .not. one_plane_hit(nchm)) then
         if (debug .gt. 0) write(25,*) ' nhit = ' 
     &,nhit(ipnum(ipl1)),nhit(ipnum(ipl2)),nhit(ipnum(ipl3))
      do 100 hit_1=1,min(10,nhit(ipnum(ipl1)))
       hitnum(ipl1) = hit_1
       track_12 = .false.
       track_found = .false.
       do 110 hit_2=1,min(10,nhit(ipnum(ipl2)))
        hitnum(ipl2) = hit_2
        track_12 = .false.
        if (hit(1,hit_1,ipnum(ipl1)) .eq. hit(1,hit_2,ipnum(ipl2)) .or.
     2  (hit(1,hit_1,ipnum(ipl1))+ns1) .eq. 
     3  hit(1,hit_2,ipnum(ipl2))) then
        track_12 = .true.
        hit_2_used(hit_2) = .true.
        do 120 hit_3=1,min(10,nhit(ipnum(ipl3)))
          hitnum(ipl3) = hit_3
          track_123(hit_3) = .false.
         if (hit(1,hit_2,ipnum(ipl2)) .eq. hit(1,hit_3,ipnum(ipl3)) 
     2    .or. (hit(1,hit_2,ipnum(ipl2))+ns2) 
     3    .eq. hit(1,hit_3,ipnum(ipl3))) then
            if (debug .gt. 0) write(25,*) ' found match 123'
           track_123(hit_3) = .true.
           track_12 = .false.
           track_found = .true.
           hit_3_used(hit_3) = .true.
           if (num_tr(nchm) .ge. 10) goto 888
           num_tr(nchm) = num_tr(nchm) + 1
           do i=ipl1,ipl3
       nhits_tr(nchm,num_tr(nchm)) = nhits_tr(nchm,num_tr(nchm)) + 1
            k = nhits_tr(nchm,num_tr(nchm))
            ztra(nchm,k,num_tr(nchm)) = hit(4,hitnum(i),ipnum(i))
            pltra(nchm,k,num_tr(nchm)) = hit(5,hitnum(i),ipnum(i))
            xtra(nchm,k,num_tr(nchm)) = hit(3,hitnum(i),ipnum(i))
            dxtra(nchm,k,num_tr(nchm)) = hit(2,hitnum(i),ipnum(i))
            utra(nchm,k,num_tr(nchm)) = 1
            dxfit(nchm,k,num_tr(nchm)) = 0.
            nwtra(nchm,k,num_tr(nchm)) = hit(1,hitnum(i),ipnum(i))
           enddo   
          endif  ! bot hit
 120     enddo  ! go through bottom hits
         if (track_12 ) then  ! no bottom hit
            if (debug .gt. 0) write(25,*) ' match 12'
           track_found = .true.
           if (num_tr(nchm) .ge. 10) goto 888
           num_tr(nchm) = num_tr(nchm) + 1
           do i=ipl1,ipl2
       nhits_tr(nchm,num_tr(nchm)) = nhits_tr(nchm,num_tr(nchm)) + 1
            k = nhits_tr(nchm,num_tr(nchm))
            ztra(nchm,k,num_tr(nchm)) = hit(4,hitnum(i),ipnum(i))
            pltra(nchm,k,num_tr(nchm)) = hit(5,hitnum(i),ipnum(i))
            xtra(nchm,k,num_tr(nchm)) = hit(3,hitnum(i),ipnum(i))
            dxtra(nchm,k,num_tr(nchm)) = hit(2,hitnum(i),ipnum(i))
            utra(nchm,k,num_tr(nchm)) = 1
            dxfit(nchm,k,num_tr(nchm)) = 0.
            nwtra(nchm,k,num_tr(nchm)) = hit(1,hitnum(i),ipnum(i))
           enddo   
         endif ! no bottom hit
        endif  ! mid hit check
 110  enddo ! go through mid hits
      do 140 hit_3=1,min(10,nhit(ipnum(ipl3)))
        hitnum(ipl3)=hit_3
        if (.not. track_123(hit_3) .and. 
     & ( (hit(1,hit_1,ipnum(ipl1))+ns3a) .eq. hit(1,hit_3,ipnum(ipl3))
     & .or. (hit(1,hit_1,ipnum(ipl1))+ns3b) 
     &       .eq. hit(1,hit_3,ipnum(ipl3))
     & .or. (hit(1,hit_1,ipnum(ipl1))+ns3c) 
     &        .eq. hit(1,hit_3,ipnum(ipl3)) )  ) then
            if (debug .gt. 0) write(25,*) ' match 13'
           if (num_tr(nchm) .ge. 10) goto 888
           num_tr(nchm) = num_tr(nchm) + 1
           track_found = .true.
           hit_3_used(hit_3) = .true.
           do i=ipl1,ipl3,2
        nhits_tr(nchm,num_tr(nchm)) = nhits_tr(nchm,num_tr(nchm)) + 1
            k = nhits_tr(nchm,num_tr(nchm))
            ztra(nchm,k,num_tr(nchm)) = hit(4,hitnum(i),ipnum(i))
            pltra(nchm,k,num_tr(nchm)) = hit(5,hitnum(i),ipnum(i))
            xtra(nchm,k,num_tr(nchm)) = hit(3,hitnum(i),ipnum(i))
            dxtra(nchm,k,num_tr(nchm)) = hit(2,hitnum(i),ipnum(i))
            utra(nchm,k,num_tr(nchm)) = 1
            dxfit(nchm,k,num_tr(nchm)) = 0.
            nwtra(nchm,k,num_tr(nchm)) = hit(1,hitnum(i),ipnum(i))
           enddo   
        endif
 140  enddo ! go through bottom hits
      if (.not. track_found) then
            if (debug .gt. 0) write(25,*) ' no match 123 or 12 or 13 '
           if (num_tr(nchm) .ge. 10) goto 888
            num_tr(nchm) = num_tr(nchm) + 1
      nhits_tr(nchm,num_tr(nchm)) = nhits_tr(nchm,num_tr(nchm)) + 1
            k = nhits_tr(nchm,num_tr(nchm))
          ztra(nchm,k,num_tr(nchm)) = hit(4,hitnum(ipl1),ipnum(ipl1))
          pltra(nchm,k,num_tr(nchm)) = hit(5,hitnum(ipl1),ipnum(ipl1))
          xtra(nchm,k,num_tr(nchm)) = hit(3,hitnum(ipl1),ipnum(ipl1))
          dxtra(nchm,k,num_tr(nchm)) = hit(2,hitnum(ipl1),ipnum(ipl1))
          utra(nchm,k,num_tr(nchm)) = 1
          dxfit(nchm,k,num_tr(nchm)) = 0.
          nwtra(nchm,k,num_tr(nchm)) = hit(1,hitnum(ipl1),ipnum(ipl1))
      endif !
 100  enddo ! go through top hits
c
      if (itrcode .ge. 3 .and. nchm .eq. 1) then
      do 400 hit_2=1,min(10,nhit(ipnum(ipl2)))
         hitnum(ipl2) = hit_2
         if (.not. hit_2_used(hit_2)) then
            if (debug .gt. 0) write(25,*) ' no match for pl 2'
           if (num_tr(nchm) .ge. 10) goto 888
            num_tr(nchm) = num_tr(nchm) + 1
       nhits_tr(nchm,num_tr(nchm)) = nhits_tr(nchm,num_tr(nchm)) + 1
            k = nhits_tr(nchm,num_tr(nchm))
           ztra(nchm,k,num_tr(nchm)) = hit(4,hitnum(ipl2),ipnum(ipl2))
           pltra(nchm,k,num_tr(nchm)) = hit(5,hitnum(ipl2),ipnum(ipl2))
           xtra(nchm,k,num_tr(nchm)) = hit(3,hitnum(ipl2),ipnum(ipl2))
           dxtra(nchm,k,num_tr(nchm)) = hit(2,hitnum(ipl2),ipnum(ipl2))
           utra(nchm,k,num_tr(nchm)) = 1
           dxfit(nchm,k,num_tr(nchm)) = 0.
           nwtra(nchm,k,num_tr(nchm)) = hit(1,hitnum(ipl2),ipnum(ipl2))
         endif
 400  enddo
      endif
c
      if ( ( itrcode .le. 2) .or. 
     &     (itrcode .ge. 3 .and. nchm .eq. 2) ) then
       do 200 hit_2=1,min(10,nhit(ipnum(ipl2)))
          hitnum(ipl2) = hit_2
          track_23 = .false.
          if (.not. hit_2_used(hit_2)) then
          track_found = .false.
          do 210 hit_3=1,min(10,nhit(ipnum(ipl3)))
             hitnum(ipl3) = hit_3
             if (hit(1,hit_2,ipnum(ipl2)) .eq.
     & hit(1,hit_3,ipnum(ipl3)) .or. (hit(1,hit_2,ipnum(ipl2))+ns2) 
     & .eq. hit(1,hit_3,ipnum(ipl3))) then
                if (debug .gt. 0) write(25,*) ' Match 23 '
                track_23 = .true.
                track_found = .true.
                hit_3_used(hit_3) = .true.
           if (num_tr(nchm) .ge. 10) goto 888
                num_tr(nchm) = num_tr(nchm) + 1
                do i=ipl2,ipl3
       nhits_tr(nchm,num_tr(nchm)) = nhits_tr(nchm,num_tr(nchm)) + 1
            k = nhits_tr(nchm,num_tr(nchm))
            ztra(nchm,k,num_tr(nchm)) = hit(4,hitnum(i),ipnum(i))
            pltra(nchm,k,num_tr(nchm)) = hit(5,hitnum(i),ipnum(i))
            xtra(nchm,k,num_tr(nchm)) = hit(3,hitnum(i),ipnum(i))
            dxtra(nchm,k,num_tr(nchm)) = hit(2,hitnum(i),ipnum(i))
            utra(nchm,k,num_tr(nchm)) = 1
            dxfit(nchm,k,num_tr(nchm)) = 0.
            nwtra(nchm,k,num_tr(nchm)) = hit(1,hitnum(i),ipnum(i))
                enddo   
             endif   
 210      enddo
          if (.not. track_found) then
             if (debug .gt. 0) write(25,*) ' no match for pl 2'
           if (num_tr(nchm) .ge. 10) goto 888
            num_tr(nchm) = num_tr(nchm) + 1
       nhits_tr(nchm,num_tr(nchm)) = nhits_tr(nchm,num_tr(nchm)) + 1
            k = nhits_tr(nchm,num_tr(nchm))
           ztra(nchm,k,num_tr(nchm)) = hit(4,hitnum(ipl2),ipnum(ipl2))
           pltra(nchm,k,num_tr(nchm)) = hit(5,hitnum(ipl2),ipnum(ipl2))
           xtra(nchm,k,num_tr(nchm)) = hit(3,hitnum(ipl2),ipnum(ipl2))
           dxtra(nchm,k,num_tr(nchm)) = hit(2,hitnum(ipl2),ipnum(ipl2))
           utra(nchm,k,num_tr(nchm)) = 1
           dxfit(nchm,k,num_tr(nchm)) = 0.
           nwtra(nchm,k,num_tr(nchm)) = hit(1,hitnum(ipl2),ipnum(ipl2))
          endif !  not track_found
          endif ! not hit_2_used
 200   enddo
      endif !  check planes 2 and 3 for ch1 , ch2 and ch4
c
      do 300 hit_3=1,min(10,nhit(ipnum(ipl3)))
         hitnum(ipl3) = hit_3
         if (.not. hit_3_used(hit_3)) then
            if (debug .gt. 0) write(25,*) ' no match for pl 3'
           if (num_tr(nchm) .ge. 10) goto 888
            num_tr(nchm) = num_tr(nchm) + 1
       nhits_tr(nchm,num_tr(nchm)) = nhits_tr(nchm,num_tr(nchm)) + 1
            k = nhits_tr(nchm,num_tr(nchm))
           ztra(nchm,k,num_tr(nchm)) = hit(4,hitnum(ipl3),ipnum(ipl3))
           pltra(nchm,k,num_tr(nchm)) = hit(5,hitnum(ipl3),ipnum(ipl3))
           xtra(nchm,k,num_tr(nchm)) = hit(3,hitnum(ipl3),ipnum(ipl3))
           dxtra(nchm,k,num_tr(nchm)) = hit(2,hitnum(ipl3),ipnum(ipl3))
           utra(nchm,k,num_tr(nchm)) = 1
           dxfit(nchm,k,num_tr(nchm)) = 0.
           nwtra(nchm,k,num_tr(nchm)) = hit(1,hitnum(ipl3),ipnum(ipl3))
         endif
 300  enddo
c
      endif ! not one plane
c
 888  if (debug .gt. 0) then
         write(25,*) ' Tracks in  chamber ',nchm,' = ', num_tr(nchm)
         do hit_1=1,num_tr(nchm)
            write(25,*) ' Track number = ',hit_1
            do hit_2=1,nhits_tr(nchm,hit_1)
             write(25,'(2i4,3f10.5)') hit_2,nwtra(nchm,hit_2,hit_1),
     &  ztra(nchm,hit_2,hit_1), xtra(nchm,hit_2,hit_1)
     &  ,dxtra(nchm,hit_2,hit_1)
            enddo
         enddo
      endif
c
      ipl1=4
      ipl2=5
      ipl3=6
 999  enddo                     !  nchm loop
c
              if(itrcode.eq.1) then
               n1 = 1 
               n2 = 2 
               n3 = 12 
               n4 = 16 
              else if(itrcode.eq.2) then
               n1 = 3 
               n2 = 4 
               n3 = 11 
               n4 = 15 
              else if(itrcode.eq.3) then
               n1 = 5
               n2 = 6 
               n3 = 14 
               n4 = 18 
              else if(itrcode.eq.4) then
               n1 = 7 
               n2 = 8 
               n3 = 13 
               n4 = 17 
              endif
c
      if (debug .gt. 0) write(25,*) ' number of possible
     & tracks = ', num_tr(1)*num_tr(2)
      ntrack = 0
      chi_best = 10000.
      nh_tr_max = 0
      if (debug .gt. 0) write(25,*) ' num track ',
     & ' intercept    slope    chi '
        do 800 ntr1=1,num_tr(1)
           do hit_1 = 1,nhits_tr(1,ntr1)
               zone(hit_1)=ztra(1,hit_1,ntr1)
               xone(hit_1)=xtra(1,hit_1,ntr1)
               dxone(hit_1)=dxtra(1,hit_1,ntr1)
               dxfone(hit_1)=dxfit(1,hit_1,ntr1)
               uone(hit_1)=utra(1,hit_1,ntr1)
               plone(hit_1)=pltra(1,hit_1,ntr1)
           enddo ! hit_1
           do 900 ntr2=1,num_tr(2)
            if ( ((nhits_tr(1,ntr1)+nhits_tr(2,ntr2)) .ge. 3) ) then
              if ( ( (nhits_tr(1,ntr1)+nhits_tr(2,ntr2)) .eq. 3 
     & .and. num_tr(1)*num_tr(2) .eq. 1 ) .or. 
     & (nhits_tr(1,ntr1)+nhits_tr(2,ntr2)) .ge. 4) then
              do hit_2 = 1,nhits_tr(2,ntr2)
               hit_3 = nhits_tr(1,ntr1) + hit_2
               zone(hit_3)=ztra(2,hit_2,ntr2)
               xone(hit_3)=xtra(2,hit_2,ntr2)
               dxone(hit_3)=dxtra(2,hit_2,ntr2)
               dxfone(hit_3)=dxfit(2,hit_2,ntr2)
               uone(hit_3)=utra(2,hit_2,ntr2)
               plone(hit_3)=pltra(2,hit_2,ntr2)
              enddo
              ntrack = ntrack + 1
              if (ntrack .gt. 10) then
                 ntrack = ntrack - 1
                 goto 800
              endif
              nh1_tr(ntrack) = nhits_tr(1,ntr1)
              nh2_tr(ntrack) = nhits_tr(2,ntr2)
              call trackit(hit_3,zone,xone,dxone,
     >           uone,dxfone,lrfit,xat0,slope,chif,chisave)
              if (debug .gt. 0) write(25,'(i4,3f10.5)') 
     & ntrack,xat0,slope,chif 
               track(n1,ntrack) = xat0             
               track(n2,ntrack) = slope             
               track(n3,ntrack) = chif             
               track(n4,ntrack) = hit_3             
             if ( (track(n3,ntrack) .lt. chi_best .and. 
     &    track(n4,ntrack) .eq. nh_tr_max) .or.
     &    (  track(n4,ntrack) .gt. nh_tr_max .and.
     &      track(n3,ntrack) .lt. chi_best*3.)  .or. 
     &    (  track(n4,ntrack) .lt. nh_tr_max .and.
     &       track(n3,ntrack) .lt. chi_best*0.333 ) ) then
                 chi_best = track(n3,ntrack)
                 do ii=1,hit_3
                 xsave(ii) = xone(ii)
                 zsave(ii) = zone(ii)
                 dxsave(ii) = dxone(ii)
                 plsave(ii) = plone(ii)
                 chis(ii) = chisave(ii)
                 enddo
                 temp_1 = track(n1,1)
                 temp_2 = track(n2,1)
                 temp_3 = track(n3,1)
                 temp_4 = track(n4,1)
                 track(n1,1)=track(n1,ntrack)
                 track(n2,1)=track(n2,ntrack)
                 track(n3,1)=track(n3,ntrack)
                 track(n4,1)=track(n4,ntrack)
                 track(n1,ntrack)=temp_1
                 track(n2,ntrack)=temp_2
                 track(n3,ntrack)=temp_3
                 track(n4,ntrack)=temp_4
                 nh_tr_max = track(n4,ntrack)
              endif
              endif
              endif
 900         enddo
 800         enddo
c     
             if (debug .gt. 0) then
                write(25,*) ' after sorting'
                write(25,*) ' intercept  slope chi '
     & ,' nhits '
             do i=1,ntrack
                write(25,'(4f10.5)') track(n1,i),track(n2,i),track(n3,i)
     & , track(n4,i)
             enddo
              write(25,*) ' chi at each hit '
              write(25,*) (chis(i),i=1,track(n4,1))
             endif
c kludge until test multiple tracks
      if (ntrack .ge. 1) then
      ntrack = 1
      endif
c
c
      return
c     
      end

