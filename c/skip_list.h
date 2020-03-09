#define MAXLEVEL 32
#define SKIPLIST_P 0.25

typedef struct skiplistNode
{
    char *ele;
    int score;
    //struct skiplistNode *backward;
    struct skiplistLevel {
        struct skiplistNode *forward;
        unsigned long span;  //当前层，该节点距离上前驱节点的实际距离。
    }level[];
}skiplistNode;

typedef struct skipList{
    skiplistNode *head, *tail;
    unsigned long length;
    int level;
}skipList;

skiplistNode *slCreateNode(int level, int score, char* ele);
skipList *slCreate(void);
void slFreeNode(skiplistNode *node);
void slFree(skipList *sl);
int randomLevel(void);
skiplistNode *slInsert(skipList *sl, int score, char* ele);
void slDeleteNode(skipList *sl, skiplistNode *node, skiplistNode **update);
int slDelete(skipList *sl, int score, char* ele);
char* slGetData(skipList *sl, int score);
void printList(skipList *sl);