      program main
      implicit double precision(a-h,o-z)
      common/mesh/n
      double precision mem(200000000)
      open(1,file='params.txt')
      read(1,*)a,b,h
      h=13
      do i=1,28
        write(*,3)h
        h=h/2
      enddo
      h=13
      print*,'------------------'
      do i=1,28
        call makeMesh(mem(1),a,b,h)
        result=r_newton_cotes_4(mem(1))
        write(*,3)result
        h=h/2
      enddo

    ! result=rectangle(mem(1))
    ! print*,'Rectangle method: ',result
    ! result=simpson(mem(1))
    ! print*,'Simpson method: ',result
    ! result=r_newton_cotes_4(mem(1))
    ! print*,'Newton cotes method: ',result
      pause
    3 format(e22.15)
      end

      subroutine makeMesh(v,a,b,h)
      implicit double precision(a-h,o-z)
      common/mesh/n
      double precision v(*)
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

      double precision function rectangle(v)
      implicit double precision(a-h,o-z)
      common/mesh/n
      double precision v(*)
      r=0
      do i=1,n-1
        r=r+(v(i+1)-v(i))*fun((v(i)+v(i+1))/2)
      enddo
      rectangle=r
      end

      double precision function simpson(v)
        implicit double precision(a-h,o-z)
      common/mesh/n
      double precision v(*)
      s=0
      simpson=0
      do i=1,n-1
        s=fun(v(i))+fun(v(i+1))
        s=s+4*fun((v(i)+v(i+1))/2)
        s=s*(v(i+1)-v(i))/6
        simpson=simpson+s
      enddo
      end

      double precision function r_newton_cotes_4(v)
      implicit double precision(a-h,o-z)
      common/mesh/n
      double precision v(*)
      r_newton_cotes_4 = 0
      s=0
      do i=1,n-1
        s=fun(v(i))+3*fun((2*v(i)+v(i + 1))/3)
        s=s+3*fun((v(i)+2*v(i + 1))/3)+fun(v(i + 1))
        s=s*(v(i + 1) - v(i))/8
        r_newton_cotes_4 = r_newton_cotes_4 + s
      enddo
      end

      double precision function fun(x)
      implicit double precision(a-h,o-z)
    ! fun=12477
    ! fun=3*x+2
    ! fun=x**2-4*x+31
    ! fun=23*x**3-5*x**2+34*x+31
    ! fun=2*x**4+6*x**3-10*x**2-7*x+21
    ! fun=x**5-x**4+3*x**3+11*x**2-2*x-9
    ! fun=x**7-2*x**5+5*x**3-7*x
      fun=x*cos(x)
      end
