      program main
      common/xy/xMin,xMax,xH,yMin,yMax,yH
      call Input
      call Table
      print*,'Table done'
      pause
      end
      
      subroutine trim(x)
      character*11 xStr
      write(xStr,'(E11.4\)')x
      read(xStr,'(E11.4\)')x
      end
      
      subroutine Input
      common/xy/xMin,xMax,xH,yMin,yMax,yH
      common/consts/EPS,PI
      EPS=1e-7
      PI=3.14159265
      open(1,file='input.txt')
      read(1,*)xMin,xMax,xH,yMin,yMax,yH
      xEPS=xH/max(abs(xMin),abs(xMax))
      yEPS=yH/max(abs(yMin),abs(yMax))
      xyEPS=min(xEPS,yEPS)
      if(xMin.gt.xMax.or.yMin.gt.yMax)then
      print*,'Invalid boundaries: min>max'
      pause
      stop
      endif
      if(xH.lt.0.or.yH.lt.0)then
      print*,'Invalid step: h<0'
      pause
      stop
      endif
      if(xH.lt.xyEPS.and.abs(xMax-xMin).gt.xyEPS)then
      print*,'Invalid X parameter: h=0 but min!=max'
      pause
      stop
      endif
      if(yH.lt.xyEPS.and.abs(yMax-yMin).gt.xyEPS)then
      print*,'Invalid Y parameter: h=0 but min!=max'
      pause
      stop
      endif
      close(1)
      end
      
      subroutine WriteDashes
      common/xy/xMin,xMax,xH,yMin,yMax,yH
      common/dashes/dash_counter
      do i=1,dash_counter
      write(2,54)
      enddo
      write(2,*)'-'
   54 format(12('-'),\)
      end
      
      subroutine Table
      common/xy/xMin,xMax,xH,yMin,yMax,yH
      common/xyCurr/xCur,yCur
      common/consts/EPS,PI
      character*11 yTrgt,yPrev,yCrS
      columnCntr=0
      open(2,file='table.txt')
      xCur=xMin
      yCur=yMin
      call trim(xCur)
      call trim(yCur)
      call WriteHeader
      call WriteLine
      write(yPrev,'(E11.4\)')yCur
      yCur=yCur+yH
      call trim(yCur)
      write(yTrgt,'(E11.4\)')yMax
      write(yCrS,'(E11.4\)')yCur
   71 continue
      call trim(yCur)
      if(.not.yCrS.eq.yPrev.and.yCur.le.yMax)then
      call WriteLine
      write(yPrev,'(E11.4\)')yCur
      yCur=yCur+yH
      write(yCrS,'(E11.4\)')yCur
      goto 71
      endif
      if(yCrS.eq.yPrev.and.yCrS.eq.yTrgt)then
      goto 79
      else
      yCur=yMax
      write(yCrS,'(E11.4\)')yCur
      goto 71
      endif
      if(yCur.ge.yMax)then
      yCur=yMax
      write(yCrS,'(E11.4\)')yCur
      goto 71
      endif
   79 close(2)
      end
      
      subroutine WriteEntry
      common/consts/EPS,PI
      common/xyCurr/xCur,yCur
      if(abs(mod(xCur,180.0)).lt.EPS)then
      write(2,55)
      else
      if(abs(mod(yCur,180.0)-90.0).lt.EPS)then
      write(2,51)0.0
      else
      write(2,51)fn2(xCur,yCur)
      endif
      endif
   51 format(e11.4,'|',\)
   55 format('not defined|',\)
      end
      
      subroutine WriteLine
      common/xy/xMin,xMax,xH,yMin,yMax,yH
      common/rows_columns/m,n
      common/xyCurr/xCur,yCur
      common/consts/EPS,PI
      character*11 xTrgt,xPrev,xCrS
      xCur=xMin
      write(2,'(a1,\)')'|'
      write(2,51)yCur
      call trim(xCur)
      call WriteEntry
      write(xPrev,'(E11.4\)')xCur
      xCur=xCur+xH
      call trim(xCur)
      write(xTrgt,'(E11.4\)')xMax
      write(xCrS,'(E11.4\)')xCur
   81 continue
      call trim(xCur)
      if(.not.xCrS.eq.xPrev.and.xCur.le.xMax)then
      call WriteEntry
      write(xPrev,'(E11.4\)')xCur
      xCur=xCur+xH
      write(xCrS,'(E11.4\)')xCur
      goto 81
      endif
      if(xCrS.eq.xPrev.and.xCrS.eq.xTrgt)then
      goto 89
      else
      xCur=xMax
      write(xCrS,'(E11.4\)')xCur
      goto 81
      endif
      if(xCur.ge.xMax)then
      xCur=xMax
      write(xCrS,'(E11.4\)')xCur
      goto 81
      endif
   89 write(2,*)
      call WriteDashes
   51 format(e11.4,'|',\)
      end
      
      subroutine WriteHeader
      common/xy/xMin,xMax,xH,yMin,yMax,yH
      common/xyCurr/xCur,yCur
      common/consts/EPS,PI
      common/dashes/dash_counter
      character*11 xTrgt,xPrev,xCrS
      xCur=xMin
      write(2,'(a1,\)')'|'
      write(2,53)
      dash_counter=1
      call trim(xCur)
      write(2,51)xCur
      dash_counter=dash_counter+1
      write(xPrev,'(E11.4\)')xCur
      xCur=xCur+xH
      call trim(xCur)
      write(xTrgt,'(E11.4\)')xMax
      write(xCrS,'(E11.4\)')xCur
   91 continue
      call trim(xCur)
      if(.not.xCrS.eq.xPrev.and.xCur.le.xMax)then
      write(2,51)xCur
      dash_counter=dash_counter+1
      write(xPrev,'(E11.4\)')xCur
      xCur=xCur+xH
      write(xCrS,'(E11.4\)')xCur
      goto 91
      endif
      if(xCrS.eq.xPrev.and.xCrS.eq.xTrgt)then
      goto 99
      else
      xCur=xMax
      write(xCrS,'(E11.4\)')xCur
      goto 91
      endif
      if(xCur.ge.xMax)then
      xCur=xMax
      write(xCrS,'(E11.4\)')xCur
      goto 91
      endif
   99 write(2,*)
      call WriteDashes
   51 format(e11.4,'|',\)
   53 format(4x,'Y\X',4x,'|',\)
      end
      
      function fn2(x,y)
      common/consts/EPS,PI
      fn2=cos(y*PI/180.0)/sin(x*PI/180.0)
      end
