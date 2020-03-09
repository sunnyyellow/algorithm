#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "skip_list.h"

int main(){
    srand((unsigned)time(NULL));

    
    skipList *sl = slCreate();
    for (int i = 0; i < 10; i++){
        slInsert(sl, rand()%100, "aaa");
    }
    printList(sl);
    return 0;
}