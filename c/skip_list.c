#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "skip_list.h"

skiplistNode *slCreateNode(int level, int score, char* ele){
    skiplistNode *node = malloc(sizeof(*node) + level*sizeof(struct skiplistLevel));
    node->score = score;
    node->ele = ele;
    return node;
}

skipList *slCreate(void){
    int j;
    skipList *sl;
    sl = malloc(sizeof(*sl));
    sl->level = 1;
    sl->length = 0;
    sl->head = slCreateNode(MAXLEVEL, 0, NULL);
    for (j=0; j < MAXLEVEL; j++){
        sl->head->level[j].forward = NULL;
        sl->head->level[j].span = 0;
    }
   sl->tail = NULL;
   return sl;

}

void slFreeNode(skiplistNode *node){
    free(node);
}

void slFree(skipList *sl){
    skiplistNode *node = sl->head->level[0].forward, *next;
    free(sl->head);
    while(node){
        next = node->level[0].forward;
        free(node);
        node = next;
    }
    free(sl);

}

int randomLevel(void){
    int level = 1;
    while((rand()&0xFFFF) < (SKIPLIST_P * 0xFFFF))
    {
        level += 1;
    }
    return (level<MAXLEVEL)? level: MAXLEVEL;
}
skiplistNode *slInsert(skipList *sl, int score, char* ele){
    skiplistNode *update[MAXLEVEL], *x;
    unsigned int rank[MAXLEVEL];
    int i, level;
    x = sl->head;
    for(i = sl->level-1;i>=0;i--){
        rank[i] = i == sl->level-1 ? 0: rank[i+1];
        while(x->level[i].forward && 
                (x->level[i].forward->score < score || 
                    (x->level[i].forward->score == score && strcmp(x->level[i].forward->ele, ele) < 0)) ){
            rank[i] += x->level[i].span;
            x = x->level[i].forward;  
        }
        update[i] = x;
    }
    level = randomLevel();
    if (level > sl->level){
        for (i = sl->level; i< level; i++){
            rank[i] = 0;
            update[i] = sl->head;
            update[i]->level[i].span = sl->length;
        }
        sl->level = level;
    }
    x = slCreateNode(level, score, ele);
    for (i = 0; i < level; i++){
        x->level[i].forward = update[i]->level[i].forward;
        update[i]->level[i].forward = x;

        x->level[i].span = update[i]->level[i].span - (rank[0] - rank[i]);
        update[i]->level[i].span = (rank[0] - rank[i] + 1);
    }

    for (i = level; i< sl->level; i++){
        update[i]->level[i].span++;
    }
    if(!x->level[0].forward){sl->tail=x;}
    sl->length++;
    return x;

}

void slDeleteNode(skipList *sl, skiplistNode *node, skiplistNode **update){
    int i;
    for (i = 0; i < sl->level; i++){
        if(update[i]->level[i].forward == node){
            update[i]->level[i].span += node->level[i].span - 1;
            update[i]->level[i].forward = node->level[i].forward;
        }else{
            update[i]->level[i].span -= 1;
        }
    }
    while(sl->level > 1 && sl->head->level[sl->level-1].forward == NULL){
        sl->level--;
    }
    sl->length--;
    free(node);

}

int slDelete(skipList *sl, int score, char* ele){
    skiplistNode *update[MAXLEVEL], *x;
    int i;

    x = sl->head;
    for(i = sl->level-1;i>=0;i--){
        while(x->level[i].forward && 
                (x->level[i].forward->score < score || 
                   (x->level[i].forward->score == score && strcmp(x->level[i].forward->ele, ele) < 0))){
            x = x->level[i].forward;
        }
        update[i] = x;
    }
    x = x->level[0].forward;
    if (x && score == x->score && strcmp(x->ele, ele) == 0){
        slDeleteNode(sl, x, update);
        return 1;
    }
    return 0;

}
char* slGetData(skipList *sl, int score){
    int i;
    skiplistNode *next = sl->head;
    for (i = sl->level-1; i>=0; i--){
        while(next->level[i].forward && next->level[i].forward->score < score){
            next = next->level[i].forward;
        }
        if (next->level[i].forward && next->level[i].forward->score == score){
            return next->level[i].forward->ele;
        }
    }
    return NULL;
}

void printList(skipList *sl){
    printf("一共 %d 层\n", sl->level);
    int i;
    skiplistNode *curr = sl->head;
    for(i = sl->level-1; i>=0; i--){
        printf("第 %d 层:", i);
        while(curr->level[i].forward){
            printf(" %d[%lu]",curr->level[i].forward->score, curr->level[i].span);
            curr = curr->level[i].forward;
        }
        printf("\n");
        curr = sl->head;
    }
}



int main(){
    srand((unsigned)time(NULL));

    int input[] = {25, 33, 64, 66, 30, 10, 13, 24, 30, 56};
    char* str[10] = {"monday","thuesday","thursday","sunday","apple","branan","peach","red","green","black"};
    printf("输入: ");
    skipList *sl = slCreate();
    for (int i = 0; i < 10; i++){
        int score = input[i];//rand()%100;
        slInsert(sl, score, str[i]);
        printf("%d[%s] ", score, str[i]);

    }
    printf("\n");
    printList(sl);

    printf("GetData: %s\n",slGetData(sl, 24));
    printf("GetData: %s\n",slGetData(sl, 25));
    printf("GetData: %s\n",slGetData(sl, 66));
    int ret = slDelete(sl, 30, "green");
    if(ret){
        printf("删除25成功\n");
    }
    printList(sl);
    ret = slDelete(sl, 24, "red");
    if(ret){
        printf("删除24成功\n");
    }
    printList(sl);
    printf("GetData: %s\n",slGetData(sl, 30));
    return 0;
}