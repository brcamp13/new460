int tqe_init()
{
    int i;
    for (i=0; i<9; i++){
        tq = &tqe_list[i];
        tq->pid = 0;
        tq->time = -1;
        tq->next = 0;
    }

    for (i=0; i<8; i++){
        tq = &tqe_list[i];
        tq->pid = 0;
        tq->time = -1;
        tq->next = &tqe_list[i+1];
    }


    free_tqe_list = &tqe_list[0];
}

TQE *TQE_dequeue(TQE **queue)
{
    TQE *tp = *queue;
    if(tp) {
        *queue = tp->next;
    }
    return tp;
}

int tqe_enqueue(TQE **queue, TQE *tp, int time) {
    int prev_sum, time_difference, position;
    TQE *q = queue;

    if (queue == 0) {
        *queue = tp;
        tp->time = time;
        tp->next = q;   
        return;
    }

    while (!position) {
        prev_sum += q->time;
        if(q->next) {
            if (time > prev_sum && time < q->next->time) {
                position = 1;
            }
        }
        q = q->next;
    }
    tp->time = time - prev_sum;
    tp->next = q->next;
    q->next = tp;
    if (tp->next) {
        tp->next->time -= tp->time;
    }
}

int print_tqe_list() {
    while (active_tqe_list) {
        printf("[time: %d, pid: %d] ->", active_tqe_list->time, active_tqe_list->pid);
        active_tqe_list = active_tqe_list->next;
    }
    printf("NULL\n");
    return 0;
}

int tqe_insert(int time_value)
{
    int sr = int_off();
    TQE *tp;
    tp = (TQE *)TQE_dequeue(&free_tqe_list);
    if (tp==0) {
        printf("no more tqe available\n");
        return 0;
    }
    tqe_enqueue(&active_tqe_list, tp, time_value);
    int_on(sr);
}