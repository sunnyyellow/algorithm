//#include <iostream>
//using namespace std;  //使用这个会调试不通过

#include <stdio.h>

void print(int a[], int n){
    for(int i = 0; i < n; i++){
        printf("%d ", a[i]);
    }
    printf("\n");
}

//最佳 Tn=O(n)
//最差 Tn=O(n^2)
//平均 Tn=O(n^2)
void bubbleSort(int a[], int n){
    for(int i = 0; i < n; i++){
        bool isExchanged = false;
        for(int j = 1; j < n - i; j++){
            if(a[j-1] > a[j]){
                int tmp = a[j];
                a[j] = a[j-1];
                a[j-1] = tmp;
                isExchanged = true;
            }
        }
        if(!isExchanged){
            break;
        }
    }
}

void selectSort(int a[], int n) {
    for(int i = 0; i < n; i++){
        int minIndex = i;
        for(int j = i; j < n; j++){
            if(a[j] < a[minIndex]){
                minIndex = j;
            }
        }
        int tmp = a[minIndex];
        a[minIndex] = a[i];
        a[i] = tmp;
    }
}

//平均 Tn=O(n^2)
//稳定排序
void insertSort(int a[], int n){
    for(int i = 1; i < n; i++){
        int current = a[i];
        int j = i - 1;
        while(j >= 0 && current < a[j]){
            a[j+1] = a[j];
            j--;
        }
        a[j+1] = current;        
    }
}

void shellSort(int a[], int n){
    int gap = n / 2;
    for(; gap >= 1; gap=gap/2){
        for(int i=gap; i<n; i++){
            int current = a[i];
            int j = i - gap;
            while(j >= 0 && current < a[j]){
                a[j+gap] = a[j];
                j = j - gap;
            }
            a[j+gap] = current; 
        } 
    }
}

void merge(int a[], int start, int mid, int end){
    int len = end  - start + 1;
    int tmp[len];
    for(int index=0, i=start, j=mid+1; index<len; index++){
        //printf("a[i]: %d, a[j]: %d\n", a[i], a[j]);
        if(i>mid){
            tmp[index] = a[j];
            j++;
        }
        else if(j>end){
            tmp[index] = a[i];
            i++;
        }
        else if(a[i] < a[j]){
            tmp[index] = a[i];
            i++;
        }
        else{
            tmp[index] = a[j];
            j++;
        }
    }
    for(int index=0; index < len; index++){
        a[index + start] = tmp[index];
    }
}

void mergeSort(int a[], int start, int end){
    int length = end - start + 1;
    if(length < 2) return;
    int mid = (start + end) / 2;
    mergeSort(a, start, mid);
    mergeSort(a, mid+1, end);
    merge(a, start, mid, end);

}




int main(int argc, char *argv[]){
    //int a[11] = {44, 12, 59, 36, 62, 43, 94, 7, 35, 52, 85};
    int a[] = {25, 33, 64, 66, 30, 10, 13, 24, 46, 56};
    //insertSort(a, 10);
    //bubbleSort(a, 10);
    //selectSort(a, 10);
    //shellSort(a, 10);
    //print(a, 10);
    mergeSort(a, 0, 9);
    print(a, 10);
    //10 13 24 25 30 33 46 56 64 66 
    
    return 0;
}