      program main
      common/consts/epsx,epsy,pi,nlength
      pi=3.1415926
      nlength=5
      call handle_input
      call write_table
      print*,'Table written to table.txt'
      pause
      end
      
      subroutine write_table
      common/consts/epsx,epsy,pi,nlength
      common/x/xmin,xmax,xh,xc
      common/y/ymin,ymax,yh,yc
      common/dashes/n
      common/flag/ndone
      ndone=0
      open(2,file='table.txt',status='unknown',err=1)
      xc=xmin
      do while(ndone.eq.0)
        call write_page
      end do
      goto 99
    1 print*,'Unable to open table.txt'
      goto 10
   10 pause
      stop
   99 continue
      close(2)
      end
      
      subroutine write_page
      common/consts/epsx,epsy,pi,nlength
      common/x/xmin,xmax,xh,xc
      common/y/ymin,ymax,yh,yc
      common/dashes/n
      character*11 ch,chpr
      xmem=xc
      call write_header
      xc=xmem
      yc=ymin
      ymem=yc
      do while(yc.lt.ymax.and.abs(ymax-yc).gt.epsy)
        xmem=xc
        call write_line
        xc=xmem
        write(chpr,2)yc
        yc=yc+yh
        write(ch,2)yc
        if(ch.eq.chpr)then
        print*,'yH is too small'
        pause
        stop
        endif
      end do
      yc=ymax
      call write_line
      yc=ymem
    2 format(E11.4)
      end
      
      subroutine write_header
      common/consts/epsx,epsy,pi,nlength
      common/x/xmin,xmax,xh,xc
      common/dashes/n
      common/flag/ndone
      character*11 ch,chpr
      write(2,'(a13,\)')'|    Y\X    |'
      n=0
      do while(xc.lt.xmax.and.abs(xmax-xc).gt.epsx
     +.and.n.lt.nlength)
        write(2,1)xc
        write(chpr,2)xc
        xc=xc+xh
        n=n+1
        write(ch,2)xc
        if(ch.eq.chpr)then
        print*,'xH is too small'
        pause
        stop
        endif
      end do
      if(n.lt.nlength)then
      write(2,1)xmax
      ndone=1
      endif
      write(2,*)
    1 format(E11.4,'|',\)
    2 format(E11.4)
    3 format(E11.4,'|')
      call write_dashes
      end
      
      subroutine write_line
      common/consts/epsx,epsy,pi,nlength
      common/x/xmin,xmax,xh,xc
      common/y/ymin,ymax,yh,yc
      common/dashes/n
      n=0
      write(2,4)yc
      do while(xc.lt.xmax.and.abs(xmax-xc).gt.epsx
     +.and.n.lt.nlength)
        if(abs(xc).lt.1.19e-7)then
        write(2,'(a12,\)')'not defined|'
        else if(abs(mod(yc,180.0)-90.0).lt.1e-4)then
        write(2,1)0.0
        else
        write(2,1)fn(xc,yc)
        endif
        xc=xc+xh
        n=n+1
      end do
      if(n.lt.nlength)then
      if(abs(xc).lt.1.19e-7)then
        write(2,'(a12,\)')'not defined|'
        else if(abs(mod(yc,180.0)-90.0).lt.1e-7)then
        write(2,1)0
        else
        write(2,1)fn(xmax,yc)
      endif
      endif
      write(2,*)
    1 format(E11.4,'|',\)
    2 format(E11.4)
    3 format(E11.4,'|')
    4 format('|',E11.4,'|',\)
      call write_dashes
      end
      
      subroutine write_dashes
      common/dashes/n
      common/flag/ndone
      do i=1,n+1+ndone
        write(2,1)
      end do
      write(2,*)'-'
    1 format(12('-'),\)
      end
      
      subroutine handle_input
      common/consts/epsx,epsy,pi,nlength
      common/x/xmin,xmax,xh,xc
      common/y/ymin,ymax,yh,yc
      open(1,file='input.txt',status='old',err=1)
      read(1,*)xmin,xmax,xh,ymin,ymax,yh
      call trim(xmax)
      call trim(xmin)
      call trim(xh)
      call trim(ymax)
      call trim(ymin)
      call trim(yh)
      epsx=1*10**(log10(abs(xmax))-4)
      epsy=1*10**(log10(abs(ymax))-4)
      if(xmax.lt.xmin)then
      print*,'xMax is less than xMin'
      goto 10
      endif
      if(ymax.lt.ymin)then
      print*,'yMax is less than yMin'
      goto 10
      endif
      if(xh.lt.0)then
      print*,'xH is negative'
      goto 10
      endif
      if(yh.lt.0)then
      print*,'yH is negative'
      goto 10
      endif
      goto 99
    1 print*,'Unable to open input.txt'
      goto 10
   10 pause
      stop
   99 continue
      close(1)
      end
      
      real function fn(x,y)
      common/consts/epsx,epsy,pi,nlength
      fn=cos(y*pi/180.0)/sin(x*pi/180.0)
      end
      
      subroutine trim(x)
      character*11 xch
      write(xch,1)x
      read(xch,1)x
    1 format(E11.4)
      end
