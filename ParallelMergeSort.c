#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define SIZE 100

int array[SIZE];

void fillArrayWithRandomNumbers(int arr[SIZE]) {
    for(int i = 0; i<SIZE; i++) array[i] = rand()%100;
}

void printArray(int arr[SIZE]) {
    for(int i = 0; i<SIZE; i++) printf("%5d", arr[i]);
    printf("\n");
}

typedef struct StartEndIndexes {
    int start;
    int end;
} StartEndIndexes;

void merge(int l, int m, int r){
  int i, j, k;
  int id1 = m - l +1;
  int id2 = r - m;
  
  int lArr[id1], rArr[id2];
  for (i = 0; i < id1; i++){
    lArr[i] = array[l+i];
  }
  for (j = 0; j < id2; j++){
    rArr[j] = array[m+1+j];
  }
  i = 0;
  j = 0;
  k = l;
  
  while ( i < id1 && j < id2){
    if (lArr[i] <= rArr[j]){
      array[k] = lArr[i];
      i++;
    }
    else {
      array[k] =rArr[j];
      j++;
    }
    k++;
  }
  while (i < id1){
    array[k] = lArr[i];
    i++;
    k++;
  }
  while (j < id2){
    array[k]= rArr[j];
    j++;
    k++;
 }
}
// Runs mergesort on the array segment described in the
// argument. Spawns two threads to mergesort each half
// of the array segment, and then merges the results.
void* mergeSort(void* args) {
    StartEndIndexes indexes = *((StartEndIndexes*)args);
    int left, right, mid;
    if (indexes.start < indexes.end){
      StartEndIndexes index1, index2;
      mid = (indexes.start + indexes.end) / 2;
      index1.start = indexes.start;
      index1.end = mid;
      index2.start = mid + 1;
      index2.end = indexes.end;
      
      pthread_t leftT,rightT;
      
      left = pthread_create(&leftT,NULL,mergeSort,&index1);
      right = pthread_create(&rightT,NULL,mergeSort, &index2);
      
      if (left != 0){
        printf("Oops, something is wrong with left!");
      }
      if (right != 0){
        printf("Oops, something is wrong with right!");
      }
      
      left = pthread_join(leftT, NULL);
      right = pthread_join(rightT, NULL);
      
      if(left != 0){
        printf("Oops, something is wrong with left!");
      }
      if(right != 0){
        printf("Oops, something is wrong with right!");
      }
      
      merge(indexes.start,mid,indexes.end);
    }
  return NULL;
}

int main() {
    srand(time(0));
    StartEndIndexes sei;
    sei.start = 0;
    sei.end = SIZE - 1;
    
    // 1. Fill array with random numbers.
    fillArrayWithRandomNumbers(array);
    
    // 2. Print the array.
    printf("Unsorted array: ");
    printArray(array);
    
    // 3. Create a 2 threads for merge sort.
    pthread_t T;
    int err = pthread_create(&(T),NULL,&mergeSort,&sei);
    if (err != 0){
      printf("Oops, thread creation failed.\n");
    }
    // 4. Wait for mergesort to finish.
    err = pthread_join(T,NULL);
    if (err !=0){
      printf("Oops, joining thread failed.\n");
    }
    // 5. Print the sorted array.
    printf("Sorted array:   ");
    printArray(array);
}
