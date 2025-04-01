      program main
      call Input
      call Table    
      print*,'Table done succesfully'
      pause
      end
      
      subroutine Input
      common/xy/xMin,xMax,xH,yMin,yMax,yH
      common/consts/EPS,valEPS,PI
      EPS=1e-4
      valEPS=EPS
      PI=3.14159265
      open(1,file='input.txt')
      read(1,*)xMin,xMax,xH,yMin,yMax,yH
      valEPS=valEPS*min(abs(xH),abs(yH))
      if(xMin.gt.xMax)then
      print*,'Invalid input: xMin>xMax'
      pause
      stop
      endif
      if(yMin.gt.yMax)then
      print*,'Invalid input: yMin>yMax'
      pause
      stop
      endif
      if(xH.lt.0)then
      print*,'Invalid input: negative xH'
      pause
      stop
      endif
      if(yH.lt.0)then
      print*,'Invalid input: negative yH'
      pause
      stop
      endif
      if(abs(yH)/min(abs(yMin),abs(yMax)).lt.EPS)then
      if(abs(yMax-yMin).ge.EPS)then
      print*,'yH value is too small'
      pause
      stop
      endif
      endif
      if(abs(xH)/min(abs(xMin),abs(xMax)).lt.EPS)then
      if(abs(xMax-xMin).ge.EPS)then
      print*,'xH value is too small'
      pause
      stop
      endif
      endif
      close(1)
      end
      
      subroutine Table
      common/xy/xMin,xMax,xH,yMin,yMax,yH
      common/consts/EPS,valEPS,PI
      common/current/xCur,yCur
      common/counters/idashC,ixC
      xCur=xMin
      yCur=yMin
      call trim(xCur)
      call trim(yCur)
      open(2,file='table.txt')
    1 continue
      call WritePage
      call WriteDashes
      if(ixC.eq.5)then
      goto 1
      endif
      close(2)
      end
      
      subroutine WritePage
      common/xy/xMin,xMax,xH,yMin,yMax,yH
      common/consts/EPS,valEPS,PI
      common/current/xCur,yCur
      xMem=xCur
      yMem=yCur
      call WriteHeader
      call WriteDashes
      if(yCur.ge.yMax)then
      goto 777
      endif
  991 continue
      call WriteLine
      xCur=xMem
      call WriteDashes
      yCur=yCur+yH
      call trim(yCur)
      if(yCur.lt.yMax)then
      goto 991
      endif
  777 continue
      call WriteLine
      call WriteDashes
      yCur=yMem
      end
      
      subroutine WriteEntry
      common/xy/xMin,xMax,xH,yMin,yMax,yH
      common/current/xCur,yCur
      common/consts/EPS,valEPS,PI
      if(abs(mod(xCur,180.0)).lt.valEPS)then
      write(2,55)
      else
      if(abs(mod(yCur,180.0)-90.0).lt.valEPS)then
      write(2,51)0.0
      else
      write(2,51)fn(xCur,yCur)
      endif
      endif
   51 format(e11.4,'|',\)
   55 format('not defined|',\)
      end
      
      subroutine WriteLine
      common/xy/xMin,xMax,xH,yMin,yMax,yH
      common/current/xCur,yCur
      common/counters/idashC,ixC
      write(2,'(a1,\)')'|'
      write(2,51)yCur
      ixC=0
      if(xCur.ge.xMax)then
      goto 812
      endif
   81 continue
      call WriteEntry
      xCur=xCur+xH
      call trim(xCur)
      ixC=ixC+1
      if(ixC.lt.5.and.xCur.lt.xMax)then
      goto 81
      endif
  812 continue
      if(ixC.lt.5)then
      call WriteEntry
      endif
      write(2,*)
   51 format(e11.4,'|',\)
      end
      
      subroutine WriteHeader
      common/xy/xMin,xMax,xH,yMin,yMax,yH
      common/current/xCur,yCur
      common/counters/idashC,ixC
      xMem=xCur
      write(2,'(a1,\)')'|'
      write(2,53)
      idashC=1
      ixC=0
      if(xCur.ge.xMax)then
      goto 912
      endif
   91 continue
      write(2,51)xCur
      xCur=xCur+xH
      call trim(xCur)
      idashC=idashC+1
      ixC=ixC+1
      if(ixC.lt.5.and.xCur.lt.xMax)then
      goto 91
      endif
  912 continue
      if(ixC.lt.5)then
      write(2,51)xCur
      idashC=idashC+1
      endif
      xCur=xMem
      write(2,*)
   51 format(e11.4,'|',\)
   53 format(4x,'Y\X',4x,'|',\)
      end
      
      subroutine WriteDashes
      common/counters/idashC,ixC
      do i=1,idashC
      write(2,54)
      enddo
      write(2,*)'-'
   54 format(12('-'),\)
      end
      
      function fn(x,y)
      common/consts/EPS,valEPS,PI
      fn=cos(y*PI/180.0)/sin(x*PI/180.0)
      end
      
      subroutine trim(x)
      character*11 xStr
      write(xStr,'(E11.4\)')x
      read(xStr,'(E11.4\)')x
      end

