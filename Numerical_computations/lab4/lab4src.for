      program main
      implicit real(a-h,o-z)
      common/mesh/n
      real mem(10000)
      h=1
      a=1
      b=10
      call makeMesh(mem(1),a,b,h)
      do i=1,n
        print*,'i:', i, ' mem:', mem(i)
      enddo
      t=rectangle(mem(1))
      print*,t
      p=simpson(mem(1))
      print*,p
      pause
      end

      subroutine makeMesh(v,a,b,h)
      implicit real(a-h,o-z)
      common/mesh/n
      real v(*)
      cur=a
      i=1
      n=999
      print*,a,b
    1 continue
      if(cur.gt.b)then
        goto 2
      endif
      v(i)=cur
      cur=cur+h
      i=i+1
      goto 1
    2 continue
      n=i-1
      print*,'n is ',n
      end

      real function rectangle(v)
      implicit real(a-h,o-z)
      common/mesh/n
      real v(*)
      r=0
      do i=1,n-1
        print*,'doin',v(i)
        r=r+(v(i+1)-v(i))*fun((v(i)+v(i+1))/2)
      enddo
      rectangle=r
      end

      real function simpson(v)
      implicit real(a-h,o-z)
      common/mesh/n
      real v(*)
      r=0
      s=0
      do i=1,n-1
        s=fun(v(i))+fun(v(i+1))
        s=s+4*fun((v(i)+v(i+1))/2)
        s=s*(v(i+1)-v(i))/6
        r=r+s
      enddo
      simpson=r
      end

      real function fun(x)
      implicit real(a-h,o-z)
      fun=x**2
      end
