      program main
      implicit real(a-h,o-z)
      common/mesh/n
      real mem(100000000)
      open(1,file='params.txt')
      read(1,*)a,b,h
      h=13
      do i=1,24
        write(*,3)h
        h=h/2
      enddo
      print*,'------------'
      h=13
      do i=1,24
        call makeMesh(mem(1),a,b,h)
        result=r_newton_cotes_4(mem(1))
        write(*,3)result
        h=h/2
      enddo
      
    ! result=simpson(mem(1))
    ! print*,'Simpson method: ',result
    ! result=r_newton_cotes_4(mem(1))
    ! print*,'Newton cotes method: ',result
      pause
    3 format(e15.8)
      end

      subroutine makeMesh(v,a,b,h)
      implicit real(a-h,o-z)
      common/mesh/n
      real v(*)
      cur=a
      n=1
    1 continue
      if(cur.gt.b)then
        goto 2
      endif
      v(n)=cur
      cur=cur+h
      n=n+1
      goto 1
    2 continue
      n=n-1
      end

      real function rectangle(v)
      implicit real(a-h,o-z)
      common/mesh/n
      real v(*)
      r=0
      do i=1,n-1
        r=r+(v(i+1)-v(i))*fun((v(i)+v(i+1))/2)
      enddo
      rectangle=r
      end

      real function simpson(v)
        implicit real(a-h,o-z)
      common/mesh/n
      real v(*)
      s=0
      simpson=0
      do i=1,n-1
        s=fun(v(i))+fun(v(i+1))
        s=s+4*fun((v(i)+v(i+1))/2)
        s=s*(v(i+1)-v(i))/6
        simpson=simpson+s
      enddo
      end

      real function r_newton_cotes_4(v) 
      implicit real(a-h,o-z)
      common/mesh/n 
      real v(*) 
      r_newton_cotes_4 = 0 
      s=0
      do i=1,n-1  
        s=fun(v(i))+3*fun((2*v(i)+v(i + 1))/3)
        s=s+3*fun((v(i)+2*v(i + 1))/3)+fun(v(i + 1))
        s=s*(v(i + 1) - v(i))/8
        r_newton_cotes_4 = r_newton_cotes_4 + s
      enddo
      end

      real function fun(x)
      implicit real(a-h,o-z)
    ! fun=12477
    ! fun=3*x+2
    ! fun=x**2-4*x+31
    ! fun=23*x**3-5*x**2+34*x+31
    ! fun=2*x**4+6*x**3-10*x**2-7*x+21
    ! fun=x**5-x**4+3*x**3+11*x**2-2*x-9
    ! fun=x**7-2*x**5+5*x**3-7*x
      fun=x*cos(x)
      end
