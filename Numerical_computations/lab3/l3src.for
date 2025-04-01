      program main
      common/size/n,n_al
        real mem(10000)
        call readDimension()
        call readVector(mem(1),n+1,'ia.txt')
        n_al=int(mem(n+1))-1
        call readVector(mem(n+2),n_al,'al.txt')
        call readVector(mem(n+2+n_al),n_al,'ja.txt')
        call readVector(mem(n+2+2*n_al),n,'di.txt')
        call readVector(mem(2*n+2+2*n_al),n,'bb.txt')
        
        call printVector(mem(1),n+1)
        call printVector(mem(n+2),n_al)
        call printVector(mem(n+2+n_al),n_al)
        call printVector(mem(n+2+2*n_al),n)
        call printVector(mem(2*n+2+2*n_al),n)
        
        call multipl(mem(1),mem(n+2),mem(n+2+n_al),
     /  mem(n+2+2*n_al),mem(2*n+2+2*n_al),mem(3*n+2+2*n_al))
        print*,'a'
        call printVector(mem(3*n+2+2*n_al), n)
        print*,'endline'
        call printDense(mem(1),mem(n+2),mem(n+2+n_al),
     /  mem(n+2+2*n_al))
        pause
      end
      
      subroutine multipl(ia,al,ja,di,b,x)
      common/size/n,n_al
      real ia(*)
      real ja(*)
      real al(*)
      real di(*)
      real b(*)
      real x(*)
      do i=1,n
        x(i)=di(i)*b(i)
        do j=nint(ia(i)),nint(ia(i+1)-1)
          x(i)=x(i)+al(j)*b(nint(ja(j)))
          x(nint(ja(j)))=x(nint(ja(j)))+al(j)*b(i)
        enddo
      enddo
      end
      
      subroutine printDense(ia,al,ja,di)
      common/size/n,n_al
      real ia(*)
      real ja(*)
      real al(*)
      real di(*)
      real b(*)
      real x(*)
      do i=1,n
        write(*, '(e11.4,'|',\)')di(i)
      enddo
      end
      
      subroutine readVector(v, n_v, name)
      dimension v(*)
      character*6 name
      open(1,file=name, status='old', err=1)
      read(1,*)(v(i), i=1,n_v)
      close(1)
      return
    1 print*,'Couldnt open ',name,' file'
      pause 
      stop
      end
      
      subroutine readDimension()
        common/size/n,n_al
        n=-1
        open(1,file='dimension.txt', status='old', err=1)
        read(1,*)n
        close(1)
        if(n.lt.1)then
          print*,'Incorrect parameter: n must be positive'
          pause
          stop
        endif
        return
        
    1   print*,'Couldnt open dimension file'
        pause 
        stop
      end
      
      subroutine printVector(v, n_v)
        common/size/n,n_al
        dimension v(*)
        do i=1,n_v
          write(*,fmt='(f5.1,\)')v(i)
        enddo
        print*,' '
      end
