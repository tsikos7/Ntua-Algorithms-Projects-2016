//
//  main.c
//
//  Created by Konstantinos Mitropoulos on 29/11/2016.
//  Copyright ΞΒΞΒΞΒΞΒ© 2016 NTUA. All rights reserved.
//

#include <stdio.h>
#include <limits.h>
#include <time.h>

#ifndef max
#define max(a,b) ((a) > (b) ? (a) : (b))
#endif

long int profit[2][100000];

long int max_Profit(long int price[], long int max_days, long int max_transactions)    {
    if(max_transactions == 0)
        return 0;
    if(max_days == 0)
        return 0;
    //Complexity O(n) for 1 transaction
    if(max_transactions == 1) {
        long int min = price[0];
        int i;
        long int max = INT_MIN;
        for(i=1;i<max_days;i++) {
            if(price[i]-min > max)
                max = price[i]-min;
            if(min > price[i])
                min = price[i];
        }
        if(max < 0)
            max = 0;
        return max;
    }
    //Complexity O(n) for k >= n/2 transactions which means no limit to transaction
    else if(max_transactions >= max_days/2) {
        long int cost = 0;
        int i = 0;
        while(i < max_days) {
            while((i<(max_days-1)) && price[i] >= price[i+1])
                i++;
            int a = i;
            i++;
            while(i<max_days && price[i] > price[i-1])
                i++;
            int b = i-1;
            if(price[b]-price[a] > 0)
                cost = cost+(price[b]-price[a]);
            
        }
        return cost;
    }
    
    else {
        //long int profit[max_transactions+1][max_days+1];
    
        long int ans;
        int i,j;
        long int max_diff;
        //Initiallize day 0
        for (long int i = 0 ; i <= 1 ; i++)
        profit[i][0] = 0;
    
        //Initiallize with max transactions 0
        for (long int j = 0 ; j <= max_days ; j++)
            profit[0][j] = 0;
    
        long int in = 1;
    
        //main function (dynamic programming)
        for(j=1;j<=max_transactions;j++) {
            max_diff = INT_MIN;
            profit[in][0] = 0;
        
            if (in == 1)max_diff = profit[in-1][0] - price[0];
            else max_diff = profit[in+1][0] - price[0];
        
            //temp = profit[in-1][0] - prices[0];
            for(i=1;i<max_days;i++) {
                if (profit[in][i-1] > max_diff+price[i]) profit[in][i] = profit[in][i-1];
                else profit[in][i] = max_diff+price[i];
                if (in == 1)    {
                    if (max_diff > profit[in-1][i]-price[i]) max_diff = max_diff;
                    else max_diff =  profit[in-1][i]-price[i];
                } else    {
                    if (max_diff > profit[in+1][i]-price[i]) max_diff = max_diff;
                    else max_diff =  profit[in+1][i]-price[i];
                }
            }
        
            if (in == 1) in = 0;
            else in = 1;
        
            ans = profit[in][max_days-1];
            
        }
        if (in == 0) return profit[in+1][max_days-1];
        else return profit[in-1][max_days-1];
    }
}

void read_file (long int *trans,  long int *days, long int Prices[])
{
    fscanf(stdin, "%ld", days);
    fscanf(stdin, "%ld", trans);
    long int temp;
    long int i = 0;
    while (fscanf(stdin, "%ld", &temp) != -1)
    {
        Prices[i++] = temp;
        if (i == *days) break;
    }
    Prices[i] = -1;
    
    return;
};








int main(int argc, const char * argv[]) {
    long int transactions, days, prices[100000];
    clock_t start, end;
    start = clock();
    //FILE *fd;
    //fd = fopen("/Users/konstantinos/InProgress/lab02/chocolate/input24.txt", "r");
    read_file(&transactions, &days, prices);
    
    
    long int res = max_Profit(prices, days, transactions);
    printf("%ld\n", res);
    end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Duration: %f\n", time_spent);
    return 0;
}
