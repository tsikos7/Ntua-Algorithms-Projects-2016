//
//  main.c
//  Αλγόριθμοι - Άσκηση 2 - Σειρά 1
//
//  Created by Konstantinos Mitropoulos on 4/11/2016.
//  Copyright © 2016 NTUA. All rights reserved.
//

#include <stdio.h>
#include <time.h>

/*GLOBAL VARIABLES*/
long int Time_Count = 1;

/*FUNCTIONS*/
void read_file (FILE *fd, long int *n, long int *k, long int Times[])
{
    fscanf(fd, "%ld", n);
    fscanf(fd, "%ld", k);
    long int temp;
    int i = 0;
    while (fscanf(fd, "%ld", &temp) != -1)
    {
        Times[i++] = temp;
    }
    Times[i] = -1;
    
    return;
};

long int Num_of_examined_till(long int times[], long int time, long int n)
{
    long int i;
    long int count = 0;
    for (i = 0 ; i < n ; i++) {
        if (time % times[i] == 0) count = count + time/times[i];
        else count = count + time/times[i] + 1;
    }

    return count;
};

long int Create_index_binary(long int times[], long int time, long int k, long int n)
{
    long int p = Num_of_examined_till(times, time, n);
    while (p < k)
    {
        time = 2*time;
        p = Num_of_examined_till(times, time, n);
    }
    return time;
};

long int Binary(long int times[], long int left, long int right, long int k, long int n)
{
    long int res = -1, mid = (left + right)/2;
    long int p = Num_of_examined_till(times, mid, n);
    if (right == left + 1) return mid;
    if (p < k - 1) res = Binary(times, mid, right, k, n);
    else if (p > k - 1) res = Binary(times, left, mid, k, n);
    else return mid;
    
    return res;
};


long int which_examiner(long int times[], long int x, long int n)
{
    long int i, j;
    while (1)
    {
        for (i = 0 ; i < n ; i++)
        {
            j = times[i];
            if (Time_Count % j == 0)
            {
                x--;
                if (x == 0) return i+1;
            }
        }
        Time_Count++;
    }
    
    return -1;
}

/*MAIN*/
int main(int argc, const char * argv[]) {
    time_t start = clock();
    FILE *fd;
    fd = fopen("/Users/konstantinos/Downloads/lab01/exam/input5.txt", "r");
    long int N, K, Times[100000];
    read_file(fd, &N, &K, Times);

    long int left, right;
    right = Create_index_binary(Times, Time_Count, K, N);
    left = right/2;
    Time_Count = Binary(Times, left, right, K, N);
   
    long int X = K - Num_of_examined_till(Times, Time_Count, N);
    long int examiner = which_examiner(Times, X, N);
    time_t t = (clock() -start)*1000.0/CLOCKS_PER_SEC;
    fprintf(stdout, "%ld %ld %ld\n", examiner, Time_Count+Times[examiner-1], t);

    return 0;
}
