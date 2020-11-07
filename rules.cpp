/*
parent Crawler
    if no threads working && queue is empty
        exit()
    if no threads working && queue has some data
        create more threads
    if some threads are working && queue is empty
        go to sleep
    if some threads are working && queue has some data
        if thread limit is not reached
            create more threads 

*/