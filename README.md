# Operating-system-Practicum-2022Spring-Nachos
## Summary	

本课程设计报告的编写目的是为了全面地说明和分析本学期《操作系统课程设计》的情况和成果。操作系统课设以Nachos为例，Nachos的全称是“Not Another Completely Heuristic Operating System”，它是一个可修改和跟踪的操作系统教学软件。它给出了一个支持多线程和虚拟存储的操作系统骨架，可在较短的时间内对操作系统中的基本原理和核心算法有一个全面和完整的了解。

​	本课设从Nachos的配置部署开始，涉及内核的线程管理、内存管理、系统调用、用户进程、文件系统等。课设报告从实验目的和任务出发，进行代码的阅读和内核原理的分析，然后做出抽象的设计架构和具体的代码实现，接着对实现的功能进行较为完备的测试，穿插调试过程以及问题解决方法，最后总结每个实验的工作，进一步反思归纳。

​	课程设计一共分8个子实验：实验一主要是Nachos安装配置，并且利用GDB调试理解Nachos工作原理，这是后续实验的基础; 实验二是了解Make file的编写，方便后续在工作目录下链接其他模块; 实验三是在Nachos的信号量基础上模拟生产者消费者问题，进行线程同步控制; 实验四与五是关于Nachos的文件系统的分析与改进，在分析并且深入理解文件系统不同模块之间的组织构建关系的基础上，对Append等命令进行完善，并且采用带模拟指针的多叉树数据结构来重新组织目录表，实现目录树以及扩展多级目录； 实验六主要对Nachos的用户程序进行分析，理解交叉编译器工作原理、Nachos可执行文件结构、页表设计与实现、Machine类配合Instruction类来实现机器和指令的模拟等，了解用户进程的创建和执行过程、核心线程的映射原理，了解Nachos系统调用，为实验七和八奠定基础；  实验七主要是扩展用户进程的地址空间使得Nachos能够支持多进程机制，完成伪进程号SpaceId的管理，并且初步完成了系统调用Exec的实现；  实验八，在前面实验5、6、7的基础上，实现线程相关的Join等五个系统调用，理解系统调用的实现、参数传递、回传机制，此后分别基于Linux文件系统和Nachos文件系统实现两套文件相关的系统调用，对Linux和Nachos文件系统进行控制，最后基于实现的部分系统调用，实现了一个简易的Nachos Shell，支持echo、touch等命令。

​	在该实验报告中，您将看到我在解决各个子实验时的方案，设计过程中的思路，相关操作系统的总结和面向对象设计模式的使用，以及最重要的我对整个操作系统课设的思考和收获。

**关键词**：**Nachos**、**线程管理**、**系统调用**、**用户进程**、**文件系统**



## Project Structure

```bash
code:
│  Makefile.common
│  Makefile.dep
│  tr.txt
│  
├─bin
│      .cvsignore
│      coff.h
│      coff2flat.c
│      coff2noff.c
│      d.c
│      disasm.c
│      encode.h
│      execute.c
│      instr.h
│      int.h
│      main.c
│      Makefile
│      noff.h
│      opstrings.c
│      out.c
│      system.c
│      
├─filesys
│  │  .cvsignore
│  │  directory.cc
│  │  directory.h
│  │  DISK
│  │  filehdr.cc
│  │  filehdr.h
│  │  filesys.cc
│  │  filesys.h
│  │  fstest.cc
│  │  Makefile
│  │  Makefile.local
│  │  openfile.cc
│  │  openfile.h
│  │  synchdisk.cc
│  │  synchdisk.h
│  │  
│  └─test
│          big
│          medium
│          small
│          
├─lab10
│      addrspace.cc
│      addrspace.h
│      bitmap.cc
│      bitmap.h
│      bool.h
│      copyright.h
│      directory.cc
│      directory.h
│      DISK
│      dump
│      exception.cc
│      filehdr.cc
│      filehdr.h
│      filesys.cc
│      filesys.h
│      fstest.cc
│      init.sh
│      list.cc
│      list.h
│      main.cc
│      Makefile
│      Makefile.local
│      openfile.cc
│      openfile.h
│      progtest.cc
│      scheduler.cc
│      scheduler.h
│      switch-linux.s
│      switch.h
│      switch.s
│      synch.cc
│      synch.h
│      synchdisk.cc
│      synchdisk.h
│      synchlist.cc
│      synchlist.h
│      synchtest.cc
│      syscall.h
│      system.cc
│      system.h
│      thread.cc
│      thread.h
│      threadtest.cc
│      utility.cc
│      utility.h
│      
├─lab2
│      main.cc
│      Makefile
│      Makefile.local
│      scheduler.cc
│      scheduler.h
│      synch.cc
│      synchtest.cc
│      system.cc
│      system.h
│      thread.cc
│      threadtest.cc
│      
├─lab3
│      main.cc
│      Makefile
│      Makefile.local
│      prodcons++.cc
│      ring.cc
│      ring.h
│      tmp_0
│      tmp_1
│      
├─lab5
│  │  directory.cc
│  │  directory.h
│  │  DISK
│  │  filehdr.cc
│  │  filehdr.h
│  │  filesys.cc
│  │  filesys.h
│  │  fstest.cc
│  │  main.cc
│  │  Makefile
│  │  Makefile.local
│  │  openfile.cc
│  │  openfile.h
│  │  synchdisk.cc
│  │  synchdisk.h
│  │  unixfile.txt
│  │  
│  └─test
│          big
│          empty
│          medium
│          small
│          
├─lab7-8
│      addrspace.cc
│      addrspace.h
│      bitmap.cc
│      bitmap.h
│      bool.h
│      copyright.h
│      directory.cc
│      directory.h
│      DISK
│      exception.cc
│      filehdr.cc
│      filehdr.h
│      filesys.cc
│      filesys.h
│      fstest.cc
│      list.cc
│      list.h
│      main.cc
│      Makefile
│      Makefile.local
│      openfile.cc
│      openfile.h
│      progtest.cc
│      scheduler.cc
│      scheduler.h
│      switch-linux.s
│      switch.h
│      switch.s
│      synch.cc
│      synch.h
│      synchdisk.cc
│      synchdisk.h
│      synchlist.cc
│      synchlist.h
│      synchtest.cc
│      syscall.h
│      system.cc
│      system.h
│      thread.cc
│      thread.h
│      threadtest.cc
│      utility.cc
│      utility.h
│      
├─lab9
│      addrspace.cc
│      addrspace.h
│      bitmap.cc
│      bitmap.h
│      bool.h
│      copyright.h
│      directory.cc
│      directory.h
│      DISK
│      dump
│      exception.cc
│      filehdr.cc
│      filehdr.h
│      filesys.cc
│      filesys.h
│      fstest.cc
│      list.cc
│      list.h
│      main.cc
│      Makefile
│      Makefile.local
│      openfile.cc
│      openfile.h
│      progtest.cc
│      scheduler.cc
│      scheduler.h
│      switch-linux.s
│      switch.h
│      switch.s
│      synch.cc
│      synch.h
│      synchdisk.cc
│      synchdisk.h
│      synchlist.cc
│      synchlist.h
│      synchtest.cc
│      syscall.h
│      system.cc
│      system.h
│      thread.cc
│      thread.h
│      threadtest.cc
│      utility.cc
│      utility.h
│      
├─machine
│      console.cc
│      console.h
│      disk.cc
│      disk.h
│      interrupt.cc
│      interrupt.h
│      machine.cc
│      machine.h
│      mipssim.cc
│      mipssim.h
│      network.cc
│      network.h
│      stats.cc
│      stats.h
│      sysdep.cc
│      sysdep.h
│      timer.cc
│      timer.h
│      translate.cc
│      translate.h
│      
├─monitor
│      main.cc
│      Makefile
│      Makefile.local
│      prodcons++.cc
│      ring.cc
│      ring.h
│      synch.cc
│      synch.h
│      tmp_0
│      tmp_1
│      
├─network
│      .cvsignore
│      DISK
│      log
│      log2
│      Makefile
│      Makefile.local
│      nettest.cc
│      post.cc
│      post.h
│      README
│      
├─test
│  │  .cvsignore
│  │  delNoff
│  │  echo.c
│  │  exec.c
│  │  exec.s
│  │  exit.c
│  │  filetest.c
│  │  halt.c
│  │  halt.s
│  │  join.c
│  │  ls.c
│  │  Makefile
│  │  matmult.c
│  │  script
│  │  shell.c
│  │  sort.c
│  │  start.s
│  │  test1.c
│  │  testfilestub.c
│  │  touch.c
│  │  yield.c
│  │  
│  └─arch
│      ├─dec-alpha-osf
│      │  ├─bin
│      │  ├─depends
│      │  └─objects
│      ├─dec-mips-ultrix
│      │  ├─bin
│      │  │      .cvsignore
│      │  │      
│      │  ├─depends
│      │  └─objects
│      ├─sun-sparc-sunos
│      │  ├─bin
│      │  │      .cvsignore
│      │  │      
│      │  ├─depends
│      │  └─objects
│      └─unknown-i386-linux
│          ├─bin
│          │      exec.noff
│          │      exit.noff
│          │      filetest.noff
│          │      halt.noff
│          │      join.noff
│          │      matmult.noff
│          │      shell.noff
│          │      sort.noff
│          │      test1.noff
│          │      testfilestub.noff
│          │      yield.noff
│          │      
│          ├─depends
│          │      exec.d
│          │      exit.d
│          │      filetest.d
│          │      halt.d
│          │      join.d
│          │      matmult.d
│          │      shell.d
│          │      sort.d
│          │      start.d
│          │      test1.d
│          │      testfilestub.d
│          │      yield.d
│          │      
│          └─objects
│                  exec.coff
│                  exec.o
│                  exit.coff
│                  exit.o
│                  filetest.coff
│                  filetest.o
│                  halt.coff
│                  halt.o
│                  join.coff
│                  join.o
│                  matmult.coff
│                  matmult.o
│                  shell.coff
│                  shell.o
│                  sort.coff
│                  sort.o
│                  start.o
│                  test1.coff
│                  test1.o
│                  testfilestub.coff
│                  testfilestub.o
│                  yield.coff
│                  yield.o
│                  
├─threads
│      bool.h
│      copyright.h
│      dump
│      list.cc
│      list.h
│      main.cc
│      Makefile
│      Makefile.local
│      scheduler.cc
│      scheduler.h
│      switch-linux.s
│      switch.h
│      switch.s
│      synch.cc
│      synch.h
│      synchlist.cc
│      synchlist.h
│      synchtest.cc
│      system.cc
│      system.h
│      thread.cc
│      thread.h
│      threadtest.cc
│      utility.cc
│      utility.h
│      
├─userprog
│      .cvsignore
│      addrspace.cc
│      addrspace.h
│      bitmap.cc
│      bitmap.h
│      exception.cc
│      FileStubTest
│      Makefile
│      Makefile.local
│      progtest.cc
│      syscall.h
│      
└─vm
        .cvsignore
        Makefile
        Makefile.local
```


        

