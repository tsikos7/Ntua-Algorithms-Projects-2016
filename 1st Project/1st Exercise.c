//
//  main.c
//  Αλγόριθμοι - Άσκηση 1 - Σειρά 1
//
//  Created by Konstantinos Mitropoulos on 6/11/2016.
//  Copyright © 2016 NTUA. All rights reserved.
//

#include <stdio.h>

void read_file (int *n, int *k, int (*a)[700])
{
    fscanf(stdin, "%d", n);
    fscanf(stdin, "%d", k);
    char c;
    int i, j;
    for (i = 0; i < *n; i++)
        for (j = 0; j < *n; j++)
        {
            c = fgetc(stdin);
            if(c == '\n')  c = fgetc(stdin);
            a[i][j] = c - 48;
        }
    return;
};

void Create_C (int n, int (*a)[700], int (*c)[700])
{
    int i, j;
    for (i = 0 ; i < n ; i++)
        for (j = 0 ; j < n ; j++)
        {
            if (i == 0) c[i][j] = a[i][j];
            else c[i][j] = c[i-1][j] + a[i][j];
        }

};

long long int Rear_Front (int (*a)[700], int (*c)[700], int x, int y, int n, int k)
{
    int rear = 0, front = 0;
    long long int count = 0;
    int currsum;
    if (x > 0) currsum = c[y][front] - c[x-1][front];
    else currsum = c[y][front];
    while (1)
    {
        int df = 0, dr = 0;
        if (currsum < k)  {
            if (front != n-1)
            {
                front++;
                if (x > 0) currsum += c[y][front] - c[x-1][front];
                else currsum += c[y][front];
            }
            else return count;
        }
        else if (currsum > k)
        {
            if (front == rear && front != n-1)
            {
                front++;
                rear++;
                if (x > 0) currsum = c[y][front] - c[x-1][front];
                else currsum = c[y][front];
            }
            else if (front == rear && front == n-1) return count;
            else if (rear < front)
            {
                rear++;
                if (x > 0) currsum -= c[y][rear-1] - c[x-1][rear-1];
                else currsum -= c[y][rear-1];
            }
        }
        else
        {
            if (x > 0)
            {
                while (front != n-1 && (c[y][front+1] - c[x-1][front+1] == 0))
                {
                
                    front++;
                    df++;
                }
            }
            else
            {
                while (front != n-1 && (c[y][front+1] == 0))
                {
                    
                    front++;
                    df++;
                }
            }
            
            if (x > 0)
            {
                while (rear != n-1 && c[y][rear] - c[x-1][rear] == 0)
                {
                    if (rear == front) break;
                    rear++;
                    dr++;
                }
            }
            else
            {
                while (rear != n-1 && c[y][rear] == 0)
                {
                    if (rear == front) break;
                    rear++;
                    dr++;
                }
            }
           
            count += (dr + 1)*(df + 1);
            
            if (front < n-1) {
                if (x > 0) currsum += c[y][front+1] - c[x-1][front+1];
                else currsum += c[y][front+1];
                front++;
            }
            else if (rear < n-1) {
                if (x > 0) currsum -= c[y][rear] - c[x-1][rear];
                else currsum -= c[y][rear];
                rear++;
            }
            else return count;
            if (x > 0)
            {
                if (front == n-1 && rear == n-1 && c[y][rear] - c[x-1][rear] != k) return count;
            }
            else
            {
                if (front == n-1 && rear == n-1 && c[y][rear] != k) return count;
            }
        }
    }
    return count;
};



int main(int argc, const char * argv[]) {

    int N, K, A[700][700], C[700][700];
    int i, j;
    read_file(&N, &K, A);
    Create_C(N, A, C);
    long long int res = 0;
    
    for (j = 0 ; j < N ; j++)
    {
        for (i = 0 ; i <= j ; i++)
        {
            
            res += Rear_Front(A, C, i, j, N, K);
        }
    }

    //res = Rear_Front(A, C, 0, 0, N, K);
    
    printf("%lld\n", res);
    return 0;
}
