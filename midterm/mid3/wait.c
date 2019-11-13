int tswitch();

int ksleep(int event)
{
  int sr = int_off();

  printf("proc %d going to sleep on event=%d\n", running->pid, event);

  running->event = event;
  running->status = SLEEP;
  enqueue(&sleepList, running);
  printList("sleepList", sleepList);
  tswitch();
  int_on(sr);
}

int kwakeup(int event)
{
  PROC *temp, *p;
  temp = 0;
  int sr = int_off();

  while (p = dequeue(&sleepList)){
     if (p->event == event){
	    printf("wakeup %d\n", p->pid);
	    p->status = READY;
	    enqueue(&readyQueue, p);
     }
     else{
	    enqueue(&temp, p);
     }
  }
  sleepList = temp;
  int_on(sr);
}

      
int kexit(int exitValue)
{
  //If the running process is p1, don't allow it to be killed
  if(running->pid == 1){
    printf("p1 never die\n");
    return 0;
  }

  printf("proc %d in kexit(), value=%d\n", running->pid, exitValue);
  //Check if running has any children. If so, give them to p1
  //(get access to p1, iterate to end of p1 children if any, iterate 
  //through running's children and append each child to the end of p1 children)
  if(running->child != 0){
    PROC *temp = readyQueue;  //Used to access process 1 and append children
    PROC *otherTemp = running; //Used to access the children of the process-to-be-killed

    printf("You made it here\n");
    printf("%d\n", temp->pid);

    while(temp->pid != 1){
      temp = temp->next;
      printf("temp pid %d\n", temp->pid);
    }

    //Set temp to the first child of p1. P1 should always have a child if you've reached this point
    temp = temp->child;

    //Get temp to point to the last child of p1
    while(temp->sibling){
      printf("temp child pid %d\n", temp->pid);
      temp = temp->sibling;
    }

    printf("Made it to before appending children\n");
    printf("temp child pid %d\n", temp->pid);

    //Append each child of otherTemp to the end of p1 child list
    if(otherTemp->child == 0){
      printf("No children to append\n");
    }else{
      otherTemp = otherTemp->child;
      while(otherTemp){
        printf("%d\n", otherTemp->pid);
        otherTemp->ppid = 1;
        temp->next = otherTemp;
        otherTemp = otherTemp->sibling;
      }
    }
  }

  running->exitCode = exitValue;
  running->status = ZOMBIE;

  //Wakes up the parent. Not sure why this needs to be done
  kwakeup(running->parent);

  tswitch();
}

int kwait(){

  //Running process has no children
  if(!running->child){
    printf("Running process has no children\n");
    return -1;
  }

  PROC *temp = running->child;
  PROC *otherTemp = running;

  while(1){
    if(temp->status == ZOMBIE){
      //Get other temp to the process before zombie
      if(otherTemp->child->status != ZOMBIE){
        otherTemp = otherTemp->child;
        while(otherTemp->sibling->status != ZOMBIE){
          otherTemp = otherTemp->sibling;
        }
      }
      //Print Zombie information
      printf("zombie pid: %d\n", temp->pid);
      printf("zombie exit status: %d\n", temp->exitCode);

      //Reset some of the proc fields
      temp->status = FREE;
      temp->ppid = 0; 
      temp->priority = 0;

      //Remove from sibling linked list
      if(temp->sibling == 0){
        otherTemp->sibling = 0;
      }else{
        otherTemp->sibling = temp->sibling;
      }

      //Put zombie in freelist and print freelist
      enqueue(&freeList, temp);
      printList("freeList", freeList);
      return 0;
    }
    temp = temp->sibling;
  }
}
