# Konrad Rauscher
# Mar 22, 2017


import sys
import pdb
import copy

def load_time_files(param_start_index):
    i = 0

    num_time_files = len(sys.argv) - param_start_index

    process_data_list = []

    while i < num_time_files:
        file_name = sys.argv[i + param_start_index]
        pid = file_name[-5]

        lines = [line.rstrip('\n') for line in open(file_name)]

        time_array = []
        time_array.append(pid)
        for line in lines:
            time_array.append(line.split())

        # total_burst_time = 0
        for element in time_array: # cast numeric values, calculate total burst time, add total bust time to end of time_array
            if element[0] == "B":
                element[1] = int(element[1])
                # total_burst_time += element[1]

            elif element[0] == "I" or  element[0] == "start":
                element[1] = int(element[1])

        # time_array.append(total_burst_time)
        # time_array.append(0) # placeholder for most recent burst time

        process_data_list.append(time_array)

        i += 1

    process_data_list= sorted(process_data_list, key=lambda x: (x[0]) )

    for item in process_data_list:
        del item[0]

    return process_data_list, num_time_files

if (len(sys.argv) < 3):
    print "ERROR: invalid number of arguments: ", len(sys.argv) - 1
else:
    scheduling_algorithm = sys.argv[1]

    verbose = False

    if (scheduling_algorithm == "RR"): # round robin

        quantum =  int(sys.argv[2])

        verbose_check = sys.argv[3]

        start_index = 0
        if (verbose_check == "verbose"):         # start_index @ 5
            verbose = True
            start_index = 4

        else: # start_index @ 4
            start_index = 3

        process_data_list, num_time_files = load_time_files(start_index)

        queu = [] # current_burst_time, pid, last_burst_time
        pid_to_execution_state = [] # keeps track of point of execution for each process (i = 0 to start, for all)
        not_waiting = [] # time at which they will want to run, pid   of processes that aren't waiting to run
        time = 0

        i = 0
        for item in process_data_list: # load data into queu
            pid_to_execution_state.append(0) # populate pid_to_execution_state

            # check that start @ 0
            if item[0][1] == 0:
                entry = []
                entry.append( 100 ) # append placeholder for most recent burst time
                entry.append( i ) # append pid
                # entry.append(0) # placeholder for most recent burst time


                queu.append(entry)

            else:
                entry = []
                entry.append(item[0][1])
                entry.append(i)

                not_waiting.append(entry)

            pid_to_execution_state[i] = pid_to_execution_state[i] + 1 # increase iterator past start element
            i += 1


        while ( ( len(queu) != 0 ) or ( len(not_waiting) != 0 ) ):
        # update queu and not_waiting based on time
            # check not_waiting, remove from not_waiting and add to queu IF time allow
            indices_to_remove = []
            for index in range(len(not_waiting)):

                if not_waiting[index][0] <= time:
                    # add to queu, remove from not_waiting
                    pid = not_waiting[index][1]
                    entry = []

                    if [pid_to_execution_state[pid] ][0] == 1: # have no way of predicting burst time, so assign 100
                        entry.append( 100 )

                    else:
                        entry.append ( process_data_list [pid] [pid_to_execution_state[pid] - 2 ] [1] ) # assign previos burst time (prediction of future)

                    entry.append( pid ) # append pid

                    # add to queu
                    queu.append(entry)

                    # add to list of indeces corresponding to which indeices to pop from not_waiting list
                    indices_to_remove.append(index)
                    #pop off not waiting list

            for index in sorted(indices_to_remove, reverse=True):
                del not_waiting[index]

            # sort on pid
            queu = sorted(queu, key=lambda x: (x[1]) )
            not_waiting = sorted(not_waiting, key=lambda x: (x[1]) )

            if len(queu) == 0: # if no items in queu, idle for appropriate time

                # calculate end time of idle
                end_idle_time = not_waiting[0][0]
                # Print: idle, start time, end time
                print "Idle", time, end_idle_time, '\n'

                #update current time to end_idle_time
                time = end_idle_time

            else: # give all items in queu a turn
                indices_to_remove = []
                for index in range(len(queu)):

                    # starting at index, and wraping around, create queu_state array
                    circular_iterator = index
                    queu_state = []

                    # convert indices_to_remove to pids_to_remove
                    pids_to_remove = []
                    for removal_index in indices_to_remove:
                        pids_to_remove.append(  queu[removal_index][1]  )

                    # add all pid's starting at index
                    while circular_iterator < len(queu):
                        if queu[circular_iterator] [1] not in pids_to_remove:
                            queu_state.append(  queu[circular_iterator] [1] )
                        circular_iterator = circular_iterator + 1
                    # add all pid's before index
                    circular_iterator = 0
                    while circular_iterator < index:
                        if queu[circular_iterator] [1] not in pids_to_remove:
                            queu_state.append( queu[circular_iterator] [1] )
                        circular_iterator = circular_iterator + 1

                    current_pid = queu[index][1]

                    burst_time_remaining =  process_data_list [ current_pid ] [ pid_to_execution_state [ current_pid] ] [ 1 ]

                    # if burst_time_remaining <= time quantum, increase time by burst_time_remaining
                        # if at end, merely pop off queu; else, add to not waiting and pop off queu
                    if burst_time_remaining <= quantum:

                        if verbose:
                            print 'Time: ', time
                            print 'Queu: '
                            for item in queu_state:
                                print item + 1
                            print

                        print current_pid + 1, time, (time + burst_time_remaining), '\n'
                        # print process_data_list [ current_pid ] [ pid_to_execution_state [ current_pid ] ]

                        time = time + burst_time_remaining

                        pid_to_execution_state[current_pid] = pid_to_execution_state[current_pid] + 1

                        if process_data_list [current_pid] [pid_to_execution_state[current_pid]] [0] == "end":
                            indices_to_remove.append(index)

                        # add to not_waiting with time at which process will be ready to be added back to queu
                        elif process_data_list [current_pid] [pid_to_execution_state[current_pid]] [0] == "I":

                            block_time = process_data_list [current_pid] [pid_to_execution_state[current_pid]] [1]
                            entry = []
                            entry.append(block_time + time)
                            entry.append(current_pid)
                            not_waiting.append(entry)

                            indices_to_remove.append(index)

                        # because process is done running for the current burst_time, increment the processes execution state
                        pid_to_execution_state[current_pid] = pid_to_execution_state[current_pid] + 1

                    else:
                        if verbose:
                            print 'Time: ', time
                            print 'Queu: '
                            for item in queu_state:
                                print item + 1
                            print

                        print current_pid + 1, time, (time + quantum), '\n'
                        # print process_data_list [ current_pid ] [ pid_to_execution_state [ current_pid ] ]
                        process_data_list [ current_pid ] [ pid_to_execution_state [ current_pid ] ] [ 1 ] = burst_time_remaining - quantum
                        time = time + quantum

                # remove processes from queu already specified: if they are done runing complietely or need to idle
                for index in sorted(indices_to_remove, reverse=True):
                    del queu[index]


    # keep running, cycling through each , moving to not_waiting if burst time = 0, decrementing by quantum otherwise

    elif (scheduling_algorithm == "SJF"): # shortest job first - can't use burst time to determine shortest job
        verbose_check = sys.argv[2]

        start_index = 0
        if (verbose_check == "verbose"):         # start_index @ 3
            verbose = True
            start_index = 3

        else: # start_index @ 2
            start_index = 2

        process_data_list, num_time_files = load_time_files(start_index)

        # put in order, based on 0th element, then delete 0th element

        queu = [] # current_burst_time, pid, last_burst_time

        pid_to_execution_state = [] # keeps track of point of execution for each process (i = 0 to start, for all)

        not_waiting = [] # time at which they will want to run, pid   of processes that aren't waiting to run

        pid_to_burst = []

        time = 0

        i = 0
        for item in process_data_list: # load data into queu
            pid_to_execution_state.append(0) # populate pid_to_execution_state

            list_entry = []
            list_entry.append(100)
            pid_to_burst.append(list_entry) # populate pid_to_burst

            # check that start @ 0
            if item[0][1] == 0:
                entry = []
                entry.append( 100 ) # append placeholder for most recent burst time
                entry.append( i ) # append pid
                # entry.append(0) # placeholder for most recent burst time

                queu.append(entry)

            else:
                entry = []
                entry.append(item[0][1])
                entry.append(i)

                not_waiting.append(entry)

            pid_to_execution_state[i] = pid_to_execution_state[i] + 1 # increase iterator past start element
            i += 1

        # loop
        while ( ( len(queu) != 0 ) or ( len(not_waiting) != 0 ) ):
            # update queu and not_waiting based on time
                # check not_waiting, remove from not_waiting and add to queu IF time allow

            indices_to_remove = []
            for index in range(len(not_waiting)):

                if not_waiting[index][0] <= time:
                    # add to queu, remove from not_waiting
                    pid = not_waiting[index][1]
                    entry = []

                    # pdb.set_trace()
                    if pid_to_execution_state [pid] == 1: # have no way of predicting burst time, so assign 100
                        entry.append( 100 )
                        # pid_to_burst[pid].append(100)

                    else:
                        # calclulate moving average, then append that
                        # pid_to_burst[pid].append( process_data_list [pid] [pid_to_execution_state[pid] - 2 ] [1] )

                        burst_sum = 0
                        for item in pid_to_burst[pid]:
                            burst_sum = burst_sum + item

                        num_bursts = len( pid_to_burst[pid] )
                        moving_average = burst_sum / num_bursts
                        entry.append ( moving_average ) # assign previos burst time (prediction of future)

                    entry.append( pid ) # append pid

                    # add to queu
                    queu.append(entry)

                    # add to list of indeces corresponding to which indeices to pop from not_waiting list
                    indices_to_remove.append(index)
                    #pop off not waiting list

            for index in sorted(indices_to_remove, reverse=True):
                del not_waiting[index]

            # sort on burst time prediction, resolve ties with pid
            queu = sorted(queu, key=lambda x: (x[0],x[1]) )
            not_waiting = sorted(not_waiting, key=lambda x: (x[0],x[1]) )

            # if no process to execute, print idle and increase time
            # print 'LENGTH', len(queu)
            if len(queu) == 0:

                # calculate end time of idle
                end_idle_time = not_waiting[0][0]
                # Print: idle, start time, end time
                print "Idle", time, end_idle_time, '\n'
                #update current time to end_idle_time
                time = end_idle_time

            # else execute process at top of queu
            else:
                # burst_time = queu[0][0]
                pid = queu[0][1]

                # get relevent burst time of process corresponding to pid
                burst_time =  process_data_list [pid ] [ pid_to_execution_state[pid] ] [ 1 ]

                if verbose:
                    print "Time", time
                    print "Modelled Burst Time: "
                    # iterate through all pids in qeue
                    for curr_pid in range(num_time_files):
                        # pdb.set_trace()
                        burst_sum = 0
                        # get average for given pid
                        for item in pid_to_burst[curr_pid]:
                                burst_sum = burst_sum + item

                        num_bursts = len( pid_to_burst[curr_pid] )
                        moving_average = burst_sum / num_bursts
                        print (curr_pid + 1), moving_average
                    print

                # print pid,  start time, end time
                print pid + 1, time, (time + burst_time), '\n'

                # update time
                time += burst_time

                # update index in pid_to_execution_state
                pid_to_execution_state[pid] = pid_to_execution_state[pid] + 1

                # update array that helps keep track of moving burst time average
                pid_to_burst[pid].append(burst_time)

                # if reached end of execution, remove process from queu
                if process_data_list [pid] [pid_to_execution_state[pid]] [0] == "end":
                    queu.pop(0)

                # otherwise obtain block time
                elif process_data_list [pid] [pid_to_execution_state[pid]] [0] == "I":

                    # add to not_waiting
                    block_time = process_data_list [pid] [pid_to_execution_state[pid]] [1]

                    entry = []
                    entry.append(block_time + time)
                    entry.append(pid)
                    not_waiting.append(entry)

                    # update index in pid_to_execution_state corresponding to current process
                    pid_to_execution_state[pid] = pid_to_execution_state[pid] + 1

                    # pop top off queu
                    queu.pop(0)

        print 'end'

#if a new process arrives with CPU burst length less than remaining time of current executing process, preempt
    elif (scheduling_algorithm == "SJR"): # shortest job remaining - can't use burst time to determine shortest job
        verbose_check = sys.argv[2]

        start_index = 0
        if (verbose_check == "verbose"):         # start_index @ 3
            verbose = True
            start_index = 3

        else: # start_index @ 2
            start_index = 2

        process_data_list, num_time_files = load_time_files(start_index)
        process_data_list_original = copy.deepcopy(process_data_list)

        queu = [] # current_burst_time, pid, last_burst_time
        pid_to_execution_state = [] # keeps track of point of execution for each process (i = 0 to start, for all)
        not_waiting = [] # time at which they will want to run, pid   of processes that aren't waiting to run
        pid_to_burst = []
        pid_to_start_time = []
        not_yet_printed = []
        pid_skip = []
        wtf = False
        time = 0
        i = 0

        for item in process_data_list: # load data into queu
            pid_to_execution_state.append(0) # populate pid_to_execution_state

            list_entry = []
            list_entry.append(100)
            pid_to_burst.append(list_entry) # populate pid_to_burst

            # check that start @ 0
            if item[0][1] == 0:
                entry = []
                entry.append( 100 ) # append placeholder for most recent burst time
                entry.append( i ) # append pid
                # entry.append(0) # placeholder for most recent burst time

                pid_to_start_time.append(0)

                queu.append(entry)

            else:
                entry = []
                entry.append(item[0][1])
                entry.append(i)

                not_waiting.append(entry)
                pid_to_start_time.append( item[0][1] )

            pid_to_execution_state[i] = pid_to_execution_state[i] + 1 # increase iterator past start element
            i += 1

        time = 0
        # loop, adding one to time
        current_pid = 0
        current_start_time = 0
        idle = False
        while ( ( len(queu) != 0 ) or ( len(not_waiting) != 0 ) ):
            # if queu[0][0] == -150 and queu[0][1] == 2:
            #     pdb.set_trace()

            indices_to_remove = []
            for index in range(len(not_waiting)):

                if not_waiting[index][0] <= time:
                    # add to queu, remove from not_waiting
                    if pid_skip and not_waiting[index][1] == pid_skip[0]:
                        del pid_skip[0]
                    else:

                        pid = not_waiting[index][1]
                        entry = []

                        if pid_to_execution_state[pid]  == 1: # have no way of predicting burst time, so assign 100
                            entry.append( 100 )

                        else:

                            burst_sum = 0
                            for item in pid_to_burst[pid]:
                                burst_sum = burst_sum + item

                            num_bursts = len( pid_to_burst[pid] )
                            moving_average = burst_sum / num_bursts
                            entry.append ( moving_average ) # assign previos burst time (prediction of future)

                        entry.append( pid ) # append pid

                        # add to queu
                        queu.append(entry)

                        # add to list of indeces corresponding to which indeices to pop from not_waiting list
                        indices_to_remove.append(index)
                        #pop off not waiting list

            for index in sorted(indices_to_remove, reverse=True):
                del not_waiting[index]

            # sort on burst time remaining prediction, resolve ties with pid
            queu = sorted(queu, key=lambda x: (x[0],x[1]) )
            not_waiting = sorted(not_waiting, key=lambda x: (x[0],x[1]) )

            if len(queu) == 0:
                # calculate end time of idle

                if len(not_yet_printed) != 0:
                    # pdb.set_trace()
                    #obtain actual burst:
                    actual_burst = process_data_list_original [current_pid] [pid_to_execution_state [current_pid] - 1 ] [1]

                    # append burst to pid_to_burst
                    pid_to_burst[current_pid].append(actual_burst)

                    if verbose:
                        print "Time", time
                        print "Modelled Burst Time: "
                        # iterate through all pids in qeue
                        for curr_pid in range(num_time_files):
                            # pdb.set_trace()
                            burst_sum = 0
                            # get average for given pid
                            for item in pid_to_burst[curr_pid]:
                                    burst_sum = burst_sum + item

                            num_bursts = len( pid_to_burst[curr_pid] )
                            moving_average = burst_sum / num_bursts
                            print (curr_pid + 1), moving_average
                        print


                    # print execution data
                    print current_pid + 1, pid_to_start_time[current_pid] , time + actual_burst, '\n'

                    # update time
                    time = time + actual_burst

                    # remove from not_yet_printed
                    del not_yet_printed[0]

                    pid_skip.append(current_pid)

                    wtf = True

                end_idle_time = not_waiting[0][0]
                # Print: idle, start time, end time
                print "Idle", time, end_idle_time, '\n'
                #update current time to end_idle_time
                time = end_idle_time

            else:
                if time == 0: # starting out, run first process, tie broken by pid
                    current_pid = queu[0][1]

                new_pid = queu[0][1]  # new_pid will be same as current_pid if preemption hasn't occured

                if new_pid != current_pid: # preemption occured

                    if wtf:
                        wtf = False
                        pid_to_start_time[new_pid] = end_idle_time
                    else:

                        if verbose:
                            print "Time", time
                            print "Modelled Burst Time: "
                            # iterate through all pids in qeue
                            for curr_pid in range(num_time_files):
                                # pdb.set_trace()
                                burst_sum = 0
                                # get average for given pid
                                for item in pid_to_burst[curr_pid]:
                                        burst_sum = burst_sum + item

                                num_bursts = len( pid_to_burst[curr_pid] )
                                moving_average = burst_sum / num_bursts
                                print (curr_pid + 1), moving_average
                            print

                        print current_pid + 1, pid_to_start_time[current_pid] , time, '\n'

                        # remove pids that have been printed
                        if current_pid in not_yet_printed:
                            for index_local in sorted( range(len(not_yet_printed)), reverse=True):
                                if not_yet_printed[index_local] == current_pid:
                                    del not_yet_printed[index_local]

                        pid_to_burst[current_pid].append( time - pid_to_start_time[current_pid] ) # append burst time to list that helps keep track of running average

                        pid_to_start_time[new_pid] = time

                current_pid = new_pid

                burst_time =  process_data_list [ current_pid ] [ pid_to_execution_state[ current_pid] ] [ 1 ]

                # if remaining burst time == 0, add to not_waiting if not at end and pop of queu, otherwise just pop off queu
                if burst_time == 0:
                    pid_to_execution_state[current_pid] = pid_to_execution_state[current_pid] + 1

                    if process_data_list [current_pid] [pid_to_execution_state[current_pid]] [0] == "end":
                        queu.pop(0)

                    # otherwise obtain block time
                    elif process_data_list [current_pid] [pid_to_execution_state[current_pid]] [0] == "I":

                        block_time = process_data_list [current_pid] [pid_to_execution_state[current_pid]] [1]
                        entry = []
                        entry.append(block_time + time)
                        entry.append(current_pid)
                        not_waiting.append(entry)

                        queu.pop(0)

                    not_yet_printed.append(current_pid)


                # else execute process at top of queu for one time unit, decrement burst time remaining for that process
                else:
                    # decrement actual burst time
                    process_data_list [ current_pid ] [ pid_to_execution_state [ current_pid ] ] [ 1 ] = burst_time - 1
                    #predicted burst
                    queu[0][0] = queu[0][0] - 1
                    time = time + 1

# DECREMENT: decrement predicted value in queue? or actual burst time in process_data_list?
# continue to run a process while its expected burst time is the lowest

            # check start times in not waiting; if start time == time AND
            # expected burst time is less then time remaining of current executing process, then preempt








































