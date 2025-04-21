      program main
      implicit real(a-h,o-z)
      common/mesh/n
      real mem(10000)
      open(1,file='params.txt')
      read(1,*)a,b,h
      call makeMesh(mem(1),a,b,h)
      result=rectangle(mem(1))
      print*,'Rectangle method: ',result
      result=simpson(mem(1))
      print*,'Simpson method: ',result
      pause
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
