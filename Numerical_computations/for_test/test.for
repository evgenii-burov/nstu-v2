      program main
      open(1,file='directfile',access='direct',status='new',recl=4)
      r=7.5
      write(1,rec=2)r
      close(1)
      print*,'done'
      pause
      open(2,file='directfile',access='direct',recl=4)
      b=0.0
      print*,b
      read(2,rec=2)b
      print*,b
      close(2)
      pause
      end
