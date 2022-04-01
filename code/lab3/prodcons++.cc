// prodcons++.cc
//	C++ version of producer and consumer problem using a ring buffer.
//
//	Create N_PROD producer threads and N_CONS consumer thread. 
//	Producer and consumer threads are communicating via a shared
//      ring buffer object. The operations on the shared ring buffer
//      are synchronized with semaphores.
//	
//      
// Copyright (c) 1995 The Regents of the University of Southern Queensland.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

//#include <sys/types.h>
//#include <sys/stat.h>
#include <unistd.h>   //exit()
#include <fcntl.h>    //creat()
#include <stdlib.h>   //write()

#include <stdio.h>
#include "copyright.h"
#include "system.h"

#include "synch.h"
#include "ring.h"

// 设置生产者消费者的宏
#define BUFF_SIZE 3  // 环形缓冲区的大小
#define N_PROD    2  // 生产者的数量 
#define N_CONS    2  // 消费者的数量
#define N_MESSG   4  // 每个生产者生产的消息的数量
#define N_CONS_MESSG 4 
#define MAX_NAME  16 // 名字的最大长度

#define MAXLEN	48 
#define LINELEN	24


Thread *producers[N_PROD];  // 生产者队列
Thread *consumers[N_CONS];  // 消费者队列

char prod_names[N_PROD][MAX_NAME];  //array of charater string for prod names
char cons_names[N_CONS][MAX_NAME];  //array of charater string for cons names

Semaphore *nempty, *nfull; //标志空和满的信号量
Semaphore *mutex;          //互斥信号量
    
Ring *ring;



//----------------------------------------------------------------------
// Producer
// 	Loop N_MESSG times and produce a message and put it in the 
//      shared ring buffer each time.
//	"which" is simply a number identifying the producer thread.
//  循环N次
//----------------------------------------------------------------------

void
Producer(_int which)
{
    int num;
    slot *message = new slot(0,0);

    for (num = 0; num < N_MESSG ; num++) {
      // Put the code to prepare the message here.
      // 构建message 信息
      message->thread_id = which;
      message->value = num;

      // Put the code for synchronization before  ring->Put(message) here.
      // 先同步
      nempty->P();
      // 后互斥
      mutex->P();

      // 放入message
      ring->Put(message);

      // Put the code for synchronization after  ring->Put(message) here.
      // 唤醒消费者
      mutex->V();
      nfull->V();

    }
}

//----------------------------------------------------------------------
// Consumer
// 	endless loop to fetch messages from the ring buffer and 
//      record these message in the corresponding file.
//      
//----------------------------------------------------------------------

void
Consumer(_int which)
{
    char str[MAXLEN];
    char fname[LINELEN];
    int fd;
    
    slot *message = new slot(0,0);

    // to form a output file name for this consumer thread.
    // all the messages received by this consumer will be recorded in 
    // this file.
    sprintf(fname, "tmp_%d", which);

    // create a file. Note that this is a UNIX system call.
    if ( (fd = creat(fname, 0600) ) == -1) 
    {
      perror("creat: file create failed");
      exit(1);
    }
    int num;
    for (num = 0;num < N_CONS_MESSG;num++ ) {

      // Put the code for synchronization before ring->Get(message) here.
      // 先同步后互斥
      nfull->P();
      mutex->P();

      // 获取message
      ring->Get(message);

      // Put the code for synchronization after ring->Get(message) here.
      // 唤醒生产者
      mutex->V();
      nempty->V();


      // form a string to record the message
      sprintf(str,"producer id --> %d; Message number --> %d;\n", 
		  message->thread_id,
		  message->value);
      // write this string into the output file of this consumer. 
      // note that this is another UNIX system call.
      if ( write(fd, str, strlen(str)) == -1 ) {
	      perror("write: write failed");
	      exit(1);
	    }
    }
}



//----------------------------------------------------------------------
// ProdCons
// 	Set up semaphores for shared round buffer and 
//	create and fork producers and consumer threads
//----------------------------------------------------------------------

void
ProdCons()
{
    int i;
    DEBUG('t', "Entering ProdCons");

    // Put the code to construct all the semaphores here.
    // 构造信号量
    mutex = new Semaphore("mutex_sem",1);             // 互斥信号量
    nempty = new Semaphore("empty_sem",BUFF_SIZE);    // 缓冲区是否为空的信号量
    nfull = new Semaphore("full_sem",0);              // 缓冲区是否已满的信号量

    // Put the code to construct a ring buffer object with size 
    //BUFF_SIZE here.
    // 定义环形缓冲区
    ring = new Ring(BUFF_SIZE);

    // 创建以及fork N_PROD个生产者线程
    for (i=0; i < N_PROD; i++) 
    {
      // this statemet is to form a string to be used as the name for 
      // produder i. 
      sprintf(prod_names[i], "producer_%d", i);

      // 创建新线程 并且 fork到对应函数
      producers[i] = new Thread(prod_names[i]);
      producers[i]->Fork(Producer,i);

    };

    // 创建以及fork N_CONS个消费者线程
    for (i=0; i < N_CONS; i++) 
    {
      // this statemet is to form a string to be used as the name for 
      // consumer i. 
      sprintf(cons_names[i], "consumer_%d", i);
      
      // Put the code to create and fork a new consumer thread using
      consumers[i] = new Thread(cons_names[i]);
      consumers[i]->Fork(Consumer,i);

    };
}

