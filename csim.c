// Eric Watson 'ewats13' 		CS341L    11/14/14

#include "cachelab.h"
#include <stdint.h>
#include <stdlib.h>
#include <strings.h>
#include <limits.h>
#include <stdio.h>
#include <getopt.h>
#include <unistd.h>
#include <inttypes.h>

typedef struct
{
        int s;
        int S;
        int B;
        int b;
        int E;
} cacheparameter;

//to use 64 bit to hold addresse use unsigned long long int with #<stdint.h>
//or unsinged unsigned long long int?

typedef unsigned long long int mem_address;

void displayUsage(char* argv[])  //argv is pointer to array of chars
{
    printf("Usage: %s [-hv] -s <num> -E <num> -b <num> -t <file>\n", argv[0]);

    printf("Options:\n");
    printf("  -h         Print this help message.\n");
    printf("  -v         Optional verbose flag.\n");
    printf("  -s <num>   Number of set index bits.\n");
    printf("  -E <num>   Number of lines per set.\n");
    printf("  -b <num>   Number of block offset bits.\n");
    printf("  -t <file>  Trace file.\n");

    printf("\nExamples:\n");
    printf("  %s -s 4 -E 1 -b 4 -t traces/yi.trace\n", argv[0]);
    printf("  %s -v -s 8 -E 2 -b 4 -t traces/yi.trace\n", argv[0]);
    exit(0);
}
int vflag;
int main(int argc, char **argv)
//argc and argv are used as a way to send arguments to a program
{
        cacheparameter cp;

//Parsing with getopt function, using atoi c library function char to int   
        char *tracefile;
        char a;
        while (( a = getopt(argc, argv, "s:E:b:t:vh")) !=-1)
//getopt returns the argument by storing it in the variable 'optarg'
//when no more option arguments are available, getopt returns -1.
{
        switch(a)
        {
        default:
        displayUsage(argv);
        exit(1);

        case 'h':
        displayUsage(argv);
        exit(0);

//when -v is called, set to 1 and display full info
        case 'v':
        vflag = 1;
        break;

        case 's':
        cp.s = atoi(optarg);
        break;

        case 'E':
        cp.E = atoi(optarg);
        break;

        case 'b':
        cp.b = atoi(optarg);
        break;

        case 't':
        tracefile = (optarg);
        break;
        }
}

        if (cp.s == 0)
        {       printf( "%s: Missing command-line argument.\n", argv[0]);

                displayUsage(argv);

                exit(1);
        }

          if (cp.E == 0)
        {       printf( "%s: Missing command-line argument.\n", argv[0]);

                displayUsage(argv);

                exit(1);
        }

          if (cp.b == 0)
        {       printf( "%s: Missing command-line argument.\n", argv[0]);

                displayUsage(argv);

                exit(1);
        }

          if (tracefile == NULL)
        {       printf( "%s: Missing command-line argument.\n", argv[0]);

                displayUsage(argv);

                exit(1);
        }
//given b and s we must compute: B = 2^b   S = 2^s
//B is the block size (bytes)  S is the number of sets
// shifting left by N is equivalent to multiplying by 2^N

        cp.B = 1 << cp.b;       //B = 2^b
        cp.S = 1 << cp.s;       //S = 2^s

//Keep track of hit, miss, and eviction count for printSummary
//essetials for reading through file
        int num_hits =0;
        int num_miss = 0;
        int num_evict = 0;

        //int num_hits, num_miss, num_evict = 0;
        int hit = 0;
        int evict = 0;
        int size;
        int LRUval = 0;
        int emptyspace = -1;
       // int victim = 0;
        char operation;
        mem_address address;
//LINE STRUCT: include valid and tag. need to calculate index and tag later
typedef struct
{
        int counter;        //counts for LRU replacement policy
        int valid;
        mem_address tag;
}       sline;

//SET STRUCT: pt to array of lines
typedef struct
{
        sline *lines;
}       cset;

//CACHE STRUCT: ptr to array of sets
typedef struct
{
        cset *sets;
}       cachestruct;

//allocate storage for your simulator's data structures using malloc function
// "man malloc" gives info about the function

        cachestruct cache;
        cache.sets = malloc(cp.S * sizeof(cset)); //# of sets * sizeof setstrct
        for (int i = 0; i < cp.S; i++)
        {
                cache.sets[i].lines = malloc(sizeof(sline)*cp.E);
        }

        // Trace File
//Valgrind memory traces have the form [space] operation address, size
// scan through trace file looking for format of "[space]%c %llx, %d"

        FILE *tfile = fopen (tracefile, "r");
        if (tfile != NULL)
        {
        while (fscanf(tfile, " %c %llx,%d", &operation, &address, &size) == 3)
                {

		int victim =0;
                if (operation != 'I')
                {
//M: a data modify. data load followed by a data store

		 if(operation == 'M')
                 {
                   num_hits++;
                  }

                        if (vflag ==1)
                        {
                        printf("%c ", operation);
                        printf("%llx,%d ", address, size);

                        if(hit ==1)
                        {
                                printf("Hit ");
                        }

                        else if (hit !=1)
                        {
                                printf("Miss ");
                        }
                        if (evict ==1)
                        {
                                printf("Eviction ");
                        }
                        printf("\n");
                        }

           		mem_address tagbits = address >> (cp.s+cp.b);
                        int tagsize = (64-(cp.s+cp.b));
                        unsigned long long int temp = address << (tagsize);
                        unsigned long long int setid = temp >> (tagsize+cp.b);
                        cset set = cache.sets[setid];
                     	 int low = INT_MAX; //include <limits.h>
//INT_MAX gives the maximum value for an object of type int
                   
			for (int i =0; i <cp.E; i++)
			{
			if (set.lines[i].valid == 1)
			{
			if (set.lines[i].tag == tagbits)
			{
				num_hits++;
				hit=1;
				set.lines[i].counter = LRUval;
				LRUval++;
			}
			else if (set.lines[i].counter < low)
			{
			low = set.lines[i].counter;
			victim = i;
			}
			}
			else if (emptyspace ==-1)
			{
				emptyspace = i;
			}

			}

			     if (hit != 1) //MISS
                        {
                                num_miss++;

                        if (emptyspace > -1)
                        {
                                set.lines[emptyspace].valid =1;
                                set.lines[emptyspace].tag = tagbits;
                                set.lines[emptyspace].counter = LRUval;
                                LRUval++;
                        }
                        else if(emptyspace <0)
                        {
                        evict =1;
                        set.lines[victim].tag = tagbits;
                        set.lines[victim].counter = LRUval;
                        LRUval++;		//incremement here
                        num_evict++;
                        }
                        }

                        emptyspace = -1;
                        hit=0;
                        evict=0;
                }
        }
}

    printSummary(num_hits,num_miss, num_evict);

    return 0;
}



