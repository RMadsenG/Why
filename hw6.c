/* HW6 read cafefully and follow all the instructions/comments
   this program compiles and runs as is although it's not doing much.
   DO NOT CHANGE THE SEED. Changing seed gives a different sequence of
   numbers but we want the same numbers for comparison purposes.

   FLOATING point radix sort requires some substantial changes to this code
   float n, lst[N],tmp[N];	
*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

#define OK 1
#define NOK 0
#define NELM 100		/* default 100 elements */
//#define N 1048576		/* 2^20 or 1 meg elements  */
//#define N 2097152
//#define N 4194304
//#define N 8388608
//#define N 16777216
#define N 335544320

void selection_sort();
void swap();

void merge_sort();
void msort_recursive();

void int_radix_sort();
void float_radix_sort();
int getMillis(  struct timeval s,  struct timeval e);

void self_check();

int rand(void);
void srand();
int rand_r();
void init_lst();
void print_lst();

int n, lst[N], buf[N];

int main(int argc,char **argv) {

  long int del_sec,del_msec;
  struct timeval tv_s,tv_e;

  if (argc>1) n = atoi(argv[1]);
  else n = NELM;
  init_lst(lst,n);
  //print_lst(lst,n);
  
  gettimeofday(&tv_s);
  
  //I ran the bash script once for selec,merge, int_radix, and float_radix
  
    //printf("selection sort\n");
    //selection_sort(lst,n);
    
    //printf("merge sort\n");
    //merge_sort(lst,buf,n);
    
    printf("int radix sort\n");
    int_radix_sort(lst,buf,n);
    
    //printf("float radix sort\n");
    //float_radix_sort(lst,buf,n);
  
  //printf("\n");  Nessecary to prevent Segfault
  gettimeofday(&tv_e); 
  //printf("\n");

  int mil=getMillis(tv_s,tv_e);
  //print_lst(lst,n);
  self_check(lst,n);
  self_check(buf,n);
  
  printf("n=%d  %d secs %d milli\n",n,  mil/1000,  mil%1000   );

  
  printf("\n");
  return 0;
}

void selection_sort(int lst[],int n){
  for(int i=0;i<n;i++)
  {
    int min=i;
    for(int j=i+1;j<n;j++)
    {
        if(lst[j]<lst[min])
        {
            min=j;
        }
    }
    int temp=lst[min];
    lst[min]=lst[i];
    lst[i]=temp;
  }
}

void merge_sort(int lst[], int buf[], int n){
  msort_recursive(lst, buf, 0, n-1);
}

//use recursion
void msort_recursive(int lst[], int buff[], int left, int right){
  if(left<right)
  {
      int mid=(right+left)/2;
      //printf("yay0\n");
      //printf("left: %d to %d\n",left,mid);
      msort_recursive(lst,buff,left,mid);//Merge left half
      
     // printf("right: %d to %d\n",mid+1,right);
      msort_recursive(lst,buff,mid+1,right);//Merge right half
      //printf("merging: %d to %d\n\n",left,right);
      int tL=left;//First index of the left array
      int tR=mid+1;//First index of the right array
      
      int buffIdx=0;
      //printf("yay1\n");

      while(tL<mid+1 &&  tR<right+1)
      {
          if(lst[tL]<=lst[tR])
          {
          //  printf("l%d: %d<%d\t\t",tL,lst[tL],lst[tR]);

             buff[buffIdx++]=lst[tL++];
          }
          else
          {
          //  printf("r%d: %d<%d\t\t",tR,lst[tR],lst[tL]);

             buff[buffIdx++]=lst[tR++];
          }
      }
           // printf("\n");

      //printf("yay\n");
      while(tL<mid+1)
      {
          buff[buffIdx++]=lst[tL++];
      }
      while(tR<right+1 )
      {
          buff[buffIdx++]=lst[tR++];
      }
      for(int i=0;i<right-left+1;i++)
      {
            lst[i+left]=buff[i];
            //printf("%d: %d\t",i+left,lst[i+left]);
      }
      //printf("\n");
  }
}


void int_radix_sort(int lst[], int buff[], int n)
{
    //4 iterations of 8 bits=32 bits of sorting
    for(int loop=0;loop<4;loop++)
    {
        int bucketcount[256]={0};
        int bucketIndex[256];

        int mask=255<<(loop*8);
        for(int i=0;i<n;i++)
        {
            bucketcount[   (lst[i]&mask)>>(8*loop)     ]++;
        }
        //Set where each bucket starts
        bucketIndex[0]=0;
        for(int i=1;i<256;i++)
        {
            bucketIndex[   i    ]=bucketcount[i-1]+bucketIndex[i-1];
        }
        
        int bucketCurrentIdx[256]={0};
        for(int i=0;i<n;i++)
        {
            int which= (lst[i]&mask)>>(8*loop);
            buff[    bucketCurrentIdx[which]+bucketIndex[which] ]=lst[i];            
            bucketCurrentIdx[which]++;
        }
        for(int i=0;i<n;i++)
        {
            lst[i]=buff[i];
        }
    }
}
void float_radix_sort(float lst[], int buff[], int n)
{
    float * f=lst; 
    int* tmp=(int *)f;
    //4 iterations of 8 bits=32 bits of sorting
    for(int loop=0;loop<4;loop++)
    {
        int bucketcount[256]={0};
        int bucketIndex[256];

        int mask=255<<(loop*8);
        for(int i=0;i<n;i++)
        {
            bucketcount[   (tmp[i]&mask)>>(8*loop)     ]++;
        }
        //Set where each bucket starts
        bucketIndex[0]=0;
        for(int i=1;i<256;i++)
        {
            bucketIndex[   i    ]=bucketcount[i-1]+bucketIndex[i-1];
        }
        
        int bucketCurrentIdx[256]={0};
        for(int i=0;i<n;i++)
        {
            int which= (tmp[i]&mask)>>(8*loop);
            buff[    bucketCurrentIdx[which]+bucketIndex[which] ]=tmp[i];            
            bucketCurrentIdx[which]++;
        }
        for(int i=0;i<n;i++)
        {
            tmp[i]=buff[i];
        }
    }
}
void print_lst(int *l,int n){
  int i;
  for (i=0; i<n; i++) {
    printf("%d \n",l[i]);
  }
  printf("\n");
}

void init_lst(int *l,int n){
  int i;
  //  int seed = time(0) % 100;	/* seconds since 1/1/1970 */
  //  printf ("seed=%d\n", seed);
  srand(1234);			/* SEED */
  for (i=0; i<n; i++) {
    l[i] = rand();
  }
}

void self_check(int *lst,int n) {
  int i,j,flag=OK,*lstp;

  lstp = lst;
  for (i=0;i<n-1;i++)
     if (lstp[i] > lstp[i+1]) { flag = NOK; break; }

  if (flag == OK) printf("sorted\n");
  else printf("NOT sorted at %d\n",i);
}

int getMillis(  struct timeval s,  struct timeval e)
{
      int secDiff= 1000*(e.tv_sec-s.tv_sec);
      int millDiff= (e.tv_usec-s.tv_usec)/1000;
        return secDiff+millDiff;
}

/* End of file */
