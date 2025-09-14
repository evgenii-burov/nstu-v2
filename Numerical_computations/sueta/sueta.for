      program main
      common/var/i,j,n,m
      common/table/x_min,x_max,x_step,y_min,y_max,y_step,
     +x,y,func
      common/pi/pi
      common/eps/eps,epsx,epsy
      pi=3.14159
      eps=1.19e-4
      call input
      call epsil
      call output
      print*,'Table written to table.txt'
      pause
      end

      subroutine input
      common/var/i,j,n,m
      common/table/x_min,x_max,x_step,y_min,y_max,y_step,
     +x,y,func
      common/pi/pi
      common/eps/eps,epsx,epsy
      open(1,file='input.txt',status='old',iostat=i)
      if(i.ne.0)then
      print*,'Error opening input file'
      pause
      stop
      endif
      read(1,*)x_min,x_max,x_step,y_min,y_max,y_step
      close(1)
      if(i.ne.0)then
      print*,'Error reading input file'
      pause
      stop
      endif
      if(((abs(x_max).lt.eps).or.(abs(x_min).lt.eps)
     +.or.(abs(y_max).lt.eps).or.(abs(y_min).lt.eps)).
     +and.x_max.ne.0.and.x_min.ne.0.and.y_max.ne.0.
     +and.y_min.ne.0)then
      print*,'Incorrect input values'
      pause
      stop
      endif
      if(x_min.gt.x_max.or.y_min.gt.y_max)then
      print*,'The minimum is greater than the maximum'
      pause
      stop
      endif
      if(x_step.lt.0.or.y_step.lt.0.or.
     +x_step.gt.abs(x_max-x_min).or.y_step.gt.abs(y_max-y_min))then
      print*,'Negative step'
      pause
      stop
      endif
      if(x_step.eq.0.and.x_min.ne.x_max)then
      print*,'Incorrect input values'
      pause
      stop
      else
      if(y_step.eq.0.and.y_min.ne.y_max)then
      print*,'Incorrect input values'
      pause
      stop
      endif
      endif
      if(((abs(x_max).lt.eps).or.(abs(x_min).lt.eps)
     +.or.(abs(y_max).lt.eps).or.(abs(y_min).lt.eps)).
     +and.x_max.ne.0.and.x_min.ne.0.and.y_max.ne.0.
     +and.y_min.ne.0)then
      print*,'Incorrect input values'
      pause
      stop
      endif
      print*,power(x_max)-power(x_step)
      print*,power(x_min)-power(x_step)
      if(abs(power(x_max)-power(x_step)).gt.4.or.
     +abs(power(x_min)-power(x_step)).gt.4)then
      print*,'Invisible step by x'
      pause
      stop
      endif
      razn=abs(power(y_min)-power(y_step))
      if(abs(power(y_max)-power(y_step)).gt.4.or.
     +razn.gt.4)then
      print*,'Invisible step by y'
      pause
      stop
      endif
      end

      subroutine output
      common/var/i,j,n,m
      common/table/x_min,x_max,x_step,y_min,y_max,y_step,
     +x,y,func
      common/pi/pi
      common/eps/eps,epsx,epsy
      open(2,file='table.txt',status='unknown',action='write',
     +iostat=i)
      if(i.ne.0)then
      print*,'Error opening output file'
      pause
      stop
      endif
      if(x_step.ne.0)then
      n=anint((x_max-x_min)/x_step)
      else
      n=0
      endif
      if(y_step.ne.0)then
      m=anint((y_max-y_min)/y_step)
      else
      m=0
      endif
      x_prev=90
      x=0
      y_prev=90
      y=0
      ! Write column headers
      write(2,101)'|    Y/X    |'
      do j=0,n
      x=(x_min)+(j*x_step)
      x=min(x,x_max)
      if(trim(x).eq.trim(x_prev))then
      print*,'Invisible step by x'
      pause
      stop
      endif
      x_prev=x
      if(abs(x_min+(x_step*j)).lt.epsx)then
      x=0.0
      endif
      write(2,102)x
      enddo
      write(2,103)
      write(2,101)'-'
      do i=0,n+1
      write(2,101)'------------'
      enddo
      write(2,103)

      do i=0,m
      y=y_min+(y_step*i)
      y=min(y,y_max)
      if(trim(y).eq.trim(y_prev))then
      print*,'Invisible step by y'
      pause
      stop
      endif
      y_prev=y
      if(abs(y_min+(y_step*i)).lt.epsy)then
      y=0.0
      endif
      write(2,101)'|'
      write(2,102)y
      do j=0,n
      x=(x_min)+(j*x_step)
      x=min(x,x_max)
      if(abs(x_min+(x_step*j)).lt.epsx)then
      x=0.0
      endif
      if((sin(y*pi/180.0).eq.0)
     +.or.(abs(mod((y)/90,2.0)).eq.0))then
      write(2,101)'not defined|'
      else
      if(abs(mod((x)/90,2.0)).eq.1)then
      func=0
      else
      func=cos(x*pi/180.0)/
     +sin(y*pi/180.0)
      endif
      write(2,102)func
      endif
      enddo
      write(2,103)
      write(2,101)'-'
      do j=0,n+1
      write(2,101)'------------'
      enddo
      write(2,103)
      enddo
101   format(a\)
102   format(e11.4 '|'\)
103   format('')
      close(2)
      end

      real function power(a)
      if(a.eq.0)then
      power=0
      else
      power=aint(log10(abs(a)))
      endif
      return
      end

      function trim(rz)
      character rzstr*11
      write(rzstr,40)rz
      read(rzstr,*)trim
40    format(e11.4)
      end

      subroutine epsil
      common/eps/eps,epsx,epsy
      common/table/x_min,x_max,x_step,y_min,y_max,y_step,
     +x,y,func
      r=abs(power(max(x_min,x_step,x_max)))-1
      if(r.eq.0)then
      epsx=eps
      else
      epsx=eps*(10**r)
      endif
      r=power(max(y_min,y_step,y_max))-1
      if(r.eq.0)then
      epsy=eps
      else
      epsy=eps*(10**r)
      endif
      end
