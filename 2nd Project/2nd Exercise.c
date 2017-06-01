#include "stdio.h"
#include "time.h"
#include "limits.h"


int sum_row[2501][2501] = {0};
int Chems_Energies[2501][2501] = {0};
int Costs[2501][2501] = {0};
int Energy[2501][2501] = {0};


void read_file (FILE *fd, int *chems, int *vials, int A[2501][2501])
{
    fscanf(fd, "%d", chems);
    fscanf(fd, "%d", vials);
    int temp;
    
    for (int i = 0; i < *chems-1; i++) {
        A[i][i] = 0;
        for (int j = i+1; j < *chems; j++) {
            if (fscanf(fd, "%d", &temp) != -1)  {
                A[i][j] = temp;
                A[j][i] = temp;
            }
        }
        
    }
    return;
};

void create_costs (int *chems, int *vials, int Cost[2501][2501], int A[2501][2501])
{
    for (int r1 = 0; r1 < *chems; r1++) {
        int temp = 0;
        for (int r2 = r1-1; r2 >= 0; r2--)    {
            temp += A[r2][r1];
            sum_row[r2][r1] = temp;
        }
    }
    for (int r1 = 0; r1 < *chems; r1++) {
        int temp2 = 0;
        for (int r2 = r1; r2 < *chems; r2++)    {
            temp2 += sum_row[r1][r2];
            Cost[r1][r2] = temp2;
        }
    }
    
    return;
};


void create_energies (int *chems, int *vials, int Cost[2501][2501], int A[2501][2501], int Energy[2501][2501])
{
    for (int i = 1; i < *chems; i++)
    {
        Energy[i][0] = Cost[0][i];
    }
    //int mink = -1;
    for (int i = 1; i < *chems; i++) {
        for (int j = 1; j < *vials; j++) {
            int min = INT_MAX;
            for (int k = 0; k < i; k++) {
                if (Energy[k][j-1]+Cost[k+1][i] < min)  {
                    min = Energy[k][j-1]+Cost[k+1][i];
                    //mink = k;
                }
            }
            Energy[i][j] = min;
        }
        
    }
    return;
};







int main(int argc, const char * argv[]) {
    int chems, vials;
    
    clock_t start, end;
    start = clock();
    
    FILE *fd;
    fd = fopen("/Users/konstantinos/InProgress/lab02/chem/input13.txt", "r");
    read_file(fd, &chems, &vials, Chems_Energies);
    create_costs(&chems, &vials, Costs, Chems_Energies);
    create_energies(&chems, &vials, Costs, Chems_Energies, Energy);
    
    printf("%d\n", Energy[chems-1][vials-1]);
    end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Duration: %f\n", time_spent);
    return 0;
}
