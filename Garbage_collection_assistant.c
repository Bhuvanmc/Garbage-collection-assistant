#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#define MAX 30
#define SSIZE 50   //Size of small vehicle
#define MSIZE 200
#define BSIZE 500


struct houseInfo
{
    int hno;
    char name[30];
};
struct stats
{
    int hno,bins;
};
struct vehicle
{
    int size,costperKm;
};

int visited[15],H[20][20], value[200],curPath[200],must_visit[200],mark = 0;
int cur_node = 0,NOFNodes = 0,start = 0,min_dist=9999,cur_dist=0,G[MAX][MAX],T,total_bins=0;



void mustvisit_houses(int c_arr[], int csize)
{
    int i;
    for ( i = 0; i < csize; ++i)
        must_visit[c_arr[i]] = 1;
}
void minPath(int S, int graph[20][20], int Path[],int n)
{
    int i,j,k;
    if (min_dist > cur_dist)
    {
        int flag = 0;
        if (value[S] == -1)
        {
            value[S] = mark++;                 //* assigning value to node
            flag = 1;
            if (must_visit[S])   NOFNodes--;
        }
        curPath[cur_node++] = S;

        if (NOFNodes != 0 || S != start)
        {
             i= S;
            for(j=0;j<n;j++)
            {
                if ((graph[i][j]!=0)&&(value[j] == -1 || value[S] > value[j]))
                {
                    cur_dist += graph[i][j];
                    minPath(j, graph, Path,n);
                    cur_dist -= graph[i][j];
                }
            }
        }
        else
        {
            if (cur_dist < min_dist)
            {
                for (k = 0; k < 200; ++k) Path[k] = -1;
                for (k = 0; k != cur_node; ++k)
                    Path[k] = curPath[k];
                min_dist = cur_dist;
            }
        }

        if (flag)
        {
            value[S] = -1;
            mark--;
        }

        if (must_visit[S] && flag)
            NOFNodes++;
        cur_node += -1;
        curPath[cur_node] = -1;
    }
}
int* SearchMinPath(int graph[][20], int csize,int c_arr[],int n)
{
    int i;
    int* Path = (int*)calloc(200, sizeof(int));
    for ( i = 0; i < 200; ++i)
    {
        Path[i] = curPath[i] = value[i] = -1;
        must_visit[i] = 0;
    }
    start = 0;
    NOFNodes = csize;
    mustvisit_houses(c_arr, csize);
    minPath(start, graph, Path,n);
    return Path;
}


void vehicle_selection(FILE *fp)
{
    struct vehicle small,medium,big;
    small.size=SSIZE;
    medium.size=MSIZE;
    big.size=BSIZE;
    small.costperKm=5;
    medium.costperKm=8;
    big.costperKm=10;
    printf("Total number of bins = %d \n",total_bins);
    if(total_bins < (small.size/2)){
        printf("\n\n-----Don't Go today------\n\n");
        fprintf(fp,"\n\n-----Don't Go today------\n\n");
    }
    else if(total_bins >= (small.size/2)&&total_bins <=small.size){
        printf("\n\n-----Take 1 SMALL Vehicle-----\n\nTotal Cost : %.2f Rs",min_dist*0.1f*small.costperKm);
        fprintf(fp,"\n\n-----Take 1 SMALL Vehicle-----\n\nTotal Cost : %.2fRs",min_dist*0.1f*small.costperKm);
    }
    else if(total_bins > small.size&&total_bins <= (medium.size/2)){
        printf("\n\n-----Take 2 SMALL Vehicle-----\n\nTotal Cost : %.2fRs",min_dist*0.1f*small.costperKm);
        fprintf(fp,"\n\n-----Take 2 SMALL Vehicle-----\n\nTotal Cost : %.2fRs",min_dist*0.1f*small.costperKm);
    }
    else if(total_bins <medium.size&&total_bins > (medium.size/2)){
        printf("\n\n-----Take 1 MEDIUM Vehicle-----\n\nTotal Cost : %.2fRs",min_dist*0.1f*medium.costperKm);
        fprintf(fp,"\n\n-----Take 1 MEDIUM Vehicle-----\n\nTotal Cost : %.2fRs",min_dist*0.1f*medium.costperKm);
    }
    else if(total_bins>medium.size&&total_bins<=(big.size/2)){
        printf("\n\n-----Take 2 MEDIUM Vehicle-----\n\nTotal Cost : %.2fRs",min_dist*0.1f*medium.costperKm);
        fprintf(fp,"\n\n-----Take 2 MEDIUM Vehicle-----\n\nTotal Cost : %.2fRs",min_dist*0.1f*medium.costperKm);
    }
    else{
        printf("\n\n-----Take BIG Vehicle-----\n\nTotal Cost : %.2fRs",min_dist*0.1f*big.costperKm);
        fprintf(fp,"\n\n-----Take BIG Vehicle-----\n\nTotal Cost : %.2fRs",min_dist*0.1f*big.costperKm);
    }
}

void main()
{
    int i,j,k,n=15,graph[20][20],subgraph[20][20],q,m,w,c_arr[20],csize;
    struct stats s;

    FILE *fp;

    //(IF WE WANT TO USE DIFFERENT Adjacency matrix)

    /*fp= fopen("graphmatrix.txt","w");
    if(fp==NULL)
        printf("NULL");
    printf("\nEnter Adjacency matrix:\n");
    for (i = 0; i < n; i++)
    {
        for (j=0;j<n;j++)
        {
            scanf("%d",&graph[i][j]);
            fprintf(fp,"%d ",graph[i][j]);
        }
        fprintf(fp,"\n");
    }
    fclose(fp);*/

    // Reading Adjacency from file
    printf("\n\n----------------------------------:HELLO:-------------------------------------------\n\n");
    fp = fopen("graphmatrix1.txt","r");
    printf("\nGiven Adjacency matrix:\n");
    for (i = 0; i < n; i++)
    {
        for (j=0;j<n;j++)
        {
                   fscanf(fp,"%d",&graph[i][j]);
                   printf("%d ",graph[i][j]);
        }
        printf("\n");
    }
    fclose(fp);
    printf("\n\n----------------------------------------------------------------------------------------------\n\n");
    fp = fopen("details.txt", "r");
    printf("House Owner Records In File:\n");
    struct houseInfo x;
    while (fscanf(fp, "No:%d Name:%s\n", &x.hno,x.name) != EOF)
    {
        fprintf(stdout, "No:%d Name:%s\n",x.hno,x.name);
    }
    fclose(fp);

    fp= fopen("stats.txt","w");
    srand(time(0));
    for(i=0;i<n-1;i++)
    {
            //printf("\nenter the house no. and Number of bins:");
            //scanf("%d %d",&m,&w);
            m=i+1;
            w=rand()%15;

            fprintf(fp,"No: %d\t Bins: %d\n",m,w);
    }
    fclose(fp);


    printf("\n\n----------------------------------------------------------------------------------------------\n\n");
    fp= fopen("stats.txt","r");
    c_arr[0]=0;
    i=1;
    printf("\n");
    while (fscanf(fp, "No: %d\tBins: %d\n", &s.hno,&s.bins) != EOF)
    {
        fprintf(stdout, "No: %d\tNumber of Bins: %d\n",s.hno,s.bins);
        total_bins+=s.bins;
        if(s.bins!=0)
        {
            c_arr[i++]=s.hno;
        }
    }
    T=csize=i;
    fclose(fp);

    printf("\n\n----------------------------------------------------------------------------------------------\n\n");

    printf("\nCOLLECTION ARRAY:");
    for(i=0;i<csize;i++)
        printf("%d ",c_arr[i]);
    printf("\n\n");

    printf("\n\n----------------------------------------------------------------------------------------------\n\n");
    printf("\n\n----------------------------------------------------------------------------------------------\n\n");
    printf("----------------------------------------------------------------------------------------------\n\n");
    fp= fopen("output.txt","w");
    printf("OPTIMAL PATH : ");
    fprintf(fp,"OPTIMAL PATH : ");
    int *Path=SearchMinPath(graph, csize, c_arr,n);
    for(i=0;Path[i]!=-1;i++)
    {
        printf("%d->",Path[i]);
        fprintf(fp,"%d->",Path[i]);
    }

    printf("\n\nMinimum Travelling Distance: %.1f km\n\n",min_dist*0.1f);
    fprintf(fp,"\n\nMinimum Travelling Distance: %.1f km\n\n",min_dist*0.1f);
    vehicle_selection(fp);
    fclose(fp);
    printf("\n\n----------------------------------:THANK YOU:-------------------------------------------\n\n\n\n\n");

}
