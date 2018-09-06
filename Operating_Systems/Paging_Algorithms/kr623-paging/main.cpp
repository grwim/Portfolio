//
//  main.cpp
//  kr623-paging
//
//  Created by Konrad Rauscher on 17/04/2017.
//  Copyright © 2017 Konrad Rauscher. All rights reserved.
//

#include <iostream>
#include <queue>
#include <vector>
#include <cstdlib>

using namespace std;

// 0x40000c35  --> page 0, offset c35
// 0x40001c35  --> page 1, offset c35

// pages are loaded into frames. if next address is within page that is lodade into frame, then hit

// question: how to convert pages into indices in virtual memory?

// if a page is placed into a frame and isn't ever written to (no W's occur) then is not dirty and you do not need to write that page out to disk as you already havee a copy on disk.
// If a W does occur then you do need to write the page back out before reusing the frame. As this is costly, some algorithms like global clock try to avoid writes.

// You overwrite when there have only been Rs in the trace. You need to swap when there have been any Ws (this is called the page being dirty).

// num misses includes when you are first initliazing physical memory. Therefore, num misses willl be off by the number of frames

// EXAMINE: corect conditions for read and write setting diry and reference bits?

// TO DO NEXT: test on server


int num_memory_access = 0;  // number of lines in input file as each represents a disk access
int num_misses = 0; // every time something is not found din a frame
int num_writes = 0; // count of how many frames are written to disk (when a page in a frame is evicted and has been written to since it was brought in)
int num_drops = 0; // number of frames that were evicted but where not dirty and hence not written back to disk

bool verbose_flag = false;
int num_frames = 0;
string alg = "";
int clock_index = 0;

int drop_count = 0; // for LFU2


//represents frames in memory; if page_id is "empty", then the frame is empty
struct frame
{
    string page_id;
    bool dirtyBit;			//is the frame dirty?
    int referenceBit;		//has the frame been referenced?
    int historyBit;			//used for Global clock
    
    int age; // used for FIFO
    int usageCount; //used for LFU and custom
    int lastUsed; // used for LRU
    
};//frame

frame * mainMemory;

// prototypes
void getParams(int argc, const char * argv[]);
void readInput();
void initialize();
void run();
void displayOutput();

int page_idInMemory(string page_id);
int spaceInMemory();

int main(int argc, const char * argv[]) {
    
    getParams(argc, argv);
    
    initialize();
    
    run();
    
    displayOutput();
}

void getParams(int argc, const char * argv[])
{
    if ( argc >= 2 ) { // flags or params are set
        num_frames =  atoi(argv[1]);
        alg = argv[2];
        
        if (argc == 4) {
            verbose_flag = true;
        }
    }
    else
    {
        // throw error
        cout << "ERROR: invalid number of params: " << argc << " and should be 3 or 4" << endl;
    }
}

void initialize()
{
    mainMemory = new frame[num_frames];
    
    // define all frames as empty
    for (int i = 0; i < num_frames; i++) {
        mainMemory[i].page_id = "empty";
        mainMemory[i].dirtyBit = 0;
        mainMemory[i].referenceBit = 0;
        mainMemory[i].historyBit = 0;
        
        mainMemory[i].age = 0; // used for FIFO
        mainMemory[i].usageCount = 0; //used for LFU and custom
        mainMemory[i].lastUsed = 0; // used for LRU
    }
}

int page_idInMemory(string page_id)
{
    int index = -1;
    for (int i = 0; i < num_frames; i++) {
        if (mainMemory[i].page_id  == page_id) {
            index = i;
        }
    }
    return index;
}

int spaceInMemory()
{
    int freeIndex = -1;
    for (int i = 0; i < num_frames; i++) {
        if (mainMemory[i].page_id  == "empty") {
            freeIndex = i;
        }
    }
    return freeIndex;
}

int evict(string new_page_id)
{
    int index_evict = - 1;
    
    // determine index
    if (alg == "FIFO")
    {
        // find first in page (record time that page was added to frame)
        
        int min = 1410065407;
        
        for (int i = 0; i < num_frames; i++) {
            if (mainMemory[i].age < min) {
                min = mainMemory[i].age;
                index_evict = i;
            }
        }
        // reset age
        mainMemory[index_evict].age = 0;
    }
    else if (alg == "GC")
    {
        // set historyBit based on dirtyBit
        for (int i = 0; i < num_frames; i++) {
            if (mainMemory[i].dirtyBit == 1) {
                mainMemory[i].historyBit = 1;
            }
        }
        
        bool evict_page_found = false;
        // have a clock_index
        while( !evict_page_found )
        {
            
            // if used accesed / modified recently, preference
            if ( ( mainMemory[clock_index].historyBit == 0 ) && (  mainMemory[clock_index].referenceBit == 0 ) )
            {
                // obtain evict index
                evict_page_found = true;
                index_evict = clock_index;
            }
            else if (mainMemory[clock_index].historyBit == 1)
            {
                mainMemory[clock_index].historyBit = 0;
            }
            else if (mainMemory[clock_index].referenceBit == 1)
            {
                mainMemory[clock_index].referenceBit = 0;
            }
            
            clock_index++;
            
            if (clock_index >= num_frames) // circle back around
            {
                clock_index = 0;
            }
        }
    }
    else if (alg == "LRU")
    {
        // increment global use counter for each page hit
        
        // lastUsed = current global use counter at the time of insertion into frame
        
        int min = 1410065407;
        
        for (int i = 0; i < num_frames; i++) {
            if (mainMemory[i].lastUsed < min) {
                min = mainMemory[i].lastUsed;
                index_evict = i;
            }
        }
        
        // reset
        mainMemory[index_evict].lastUsed = 0;
        
    }
    else if (alg == "LFU")
    {
        // remove frame with fewest usages
        
        int min = 1410065407;
        
        for (int i = 0; i < num_frames; i++) {
            if (mainMemory[i].usageCount < min) {
                min = mainMemory[i].usageCount;
                index_evict = i;
            }
        }
        
        // reset usageCount
        mainMemory[index_evict].usageCount = 0;
        
    }
    else if (alg == "RAND")
    {
        // select random number from 0 to num_frames
        index_evict = rand() % num_frames;
        
    }
    else if (alg == "LFU2") // own design
    {
       
        int max = -1;
        int min = 1410065407;
        drop_count++;

        // normally drop least frequent used
        // BUT every num_frames * num_frames, drop most frequent so as to free up space for new paging requests more quickly and preventing new pages getting dropped too often
        if (drop_count >= num_frames * num_frames)
        {
            for (int i = 0; i < num_frames; i++) {
                if (mainMemory[i].usageCount > max) {
                    max = mainMemory[i].usageCount;
                    index_evict = i;
                }
            }
            drop_count = 0; // reset drop_count
        }
        else
        {
            for (int i = 0; i < num_frames; i++) {
                if (mainMemory[i].usageCount < min) {
                    min = mainMemory[i].usageCount;
                    index_evict = i;
                }
            }
        }
        
        // reset usageCount
        mainMemory[index_evict].usageCount = 0;
        
    }
    else
    {
        // error
        cout << "ERROR: invalid alg param: " << alg << endl;
    }
    
    
        if (mainMemory[index_evict].dirtyBit == 1)
        {
            num_writes++;
            if (verbose_flag)
            {
                cout << "Page " << new_page_id << " swaps " << mainMemory[index_evict].page_id << endl;
            }
        }
        else
        {
            num_drops++;
            if (verbose_flag)
            {
                cout << "Page " << new_page_id << " overwrites " << mainMemory[index_evict].page_id << endl;
            }
        }
    
    
    // evict frame at index_freed_frame --> reset page_id, and dirty, and referenceBit and possibly other things
    mainMemory[index_evict].page_id = new_page_id;
    mainMemory[index_evict].dirtyBit = 0;
    mainMemory[index_evict].referenceBit = 0;
    mainMemory[index_evict].historyBit = 0;
    
    return index_evict;
}

void updateBits( int index, bool justAdded )
{
    if (alg == "FIFO")
    {
        // only update age if being added from hard disk
        if (justAdded) {
        // age = time that page was added to frame
        mainMemory[index].age = num_memory_access;
        }
    }
    else if (alg == "LRU")
    {
        // use num_memory_access;
//        update everytime accessed
        // lastUsed = current global use counter
        mainMemory[index].age = num_memory_access;
        
    }
    else if (alg == "LFU")
    {
        // usageCount  --> remove frame with fewest usages
        
        if (! justAdded) {
            mainMemory[index].usageCount = mainMemory[index].usageCount + 1;
        }
    }
    else if (alg == "LFU2")
    {
        // usageCount  --> remove frame with fewest usages
        
        if (justAdded) {
            mainMemory[index].usageCount = mainMemory[index].usageCount + 1;
        }
    }
    
    // already took care of global clock variables
    
}

void run ()
{
    
    string input_line;
    while(cin) {
        getline(cin, input_line);
        num_memory_access++;
        
        if ( input_line.size() == 0 )
        {
            break;
        }
        
        // use 0-6 as page id
        // RUN AGAIN, need condition for quit on empty line 
        
//        cout << "line: " << input_line.size() << endl;
        
        string page_id = input_line.substr(0, 7);
        
        // use 7-9 as offset
//        string offset = input_line.substr(7, 3);
        
        // get read / write info
        
        char RW = input_line.substr(11, 1)[0];
        
        if (RW == ' ')
        {
            RW = input_line.substr(12, 1)[0];
        }
        
//        
//        cout << input_line << endl;
//        cout << "page id: " << page_id << endl;
//        cout << "RW: " << RW << endl;
        
        // check if page_id already in mainMemory
        int location = page_idInMemory(page_id);
        if (location != -1) // then hit
        {
            if (RW == 'W') // if write, then set dirty bit
            {
                mainMemory[location].dirtyBit = 1;
            }
            else if (RW == 'R')
            {
                mainMemory[location].referenceBit = 1;
            }
            
            updateBits(location, false);
        }
        else
        {
            // if not, check if space in mainMemory
            int potential_index = spaceInMemory();
            
            if ( potential_index == -1 ) // no space in memory
            {
                // increment num_misses
                num_misses++;
                
                // evict a page from a frame using appropriate algorithm, return index of freed frame
                int index_freed_frame = evict(page_id);
                
                // load page into recently freed frame
                mainMemory[index_freed_frame].page_id = page_id;
                
                if (RW == 'W')
                {
                    mainMemory[index_freed_frame].dirtyBit = 1;
                }
                else if (RW == 'R')
                {
                    mainMemory[index_freed_frame].referenceBit = 1;
                }
                
//              update proper variables of the frame, based on alg being used
                updateBits(index_freed_frame, true);
                
            }
            else // there is space in memory
            {
                int index = potential_index;
                
                mainMemory[index].page_id = page_id;
                
                if (RW == 'W')
                {
                    mainMemory[index].dirtyBit = 1;
                }
                else if (RW == 'R')
                {
                    mainMemory[index].referenceBit = 1;
                }
                
//                update proper variables of the frame, based on alg being used
                updateBits(index, true);
                
            }
        }
    }
}

void displayOutput()
{
    cout << "Number of memory accesses: " << num_memory_access << endl;
    cout << "Number of misses: " << num_misses << endl;
    cout << "Number of writes: " << num_writes << endl;
    cout << "Number of drops: " << num_drops << endl;
    
}





























