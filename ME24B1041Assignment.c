//ME24B1041
//RATHOD JAISH
//Assignment-1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TASKS 6//defining taks of size=6

// Queue
char taskQueue[MAX_TASKS][20] = {"Scanner", "Digger", "Lift", "Light", "Drone", "Cutter"};
int qFront = -1, qRear = -1;

// Stack
char urgentStack[MAX_TASKS][20];
int stackTop = -1;

// Log
char completedMissions[MAX_TASKS][20];
int missionCount = 0;

// Doubly Linked List (Damaged & Repaired Robots)
typedef struct RobotNode {
    char rName[20];
    struct RobotNode* next;
    struct RobotNode* prev;
} RobotNode;

RobotNode *damagedList = NULL;
RobotNode *repairedStart = NULL;
RobotNode *repairedEnd = NULL;

// Circular Linked List
typedef struct PriorityNode {
    char pName[20];
    struct PriorityNode* next;
} PriorityNode;

PriorityNode* circleTail = NULL;

// Queue Functions
void addToQueue(char task[]) {
    if (qRear == MAX_TASKS - 1) {//condition to add to queue we check wether queue is full or not..
        printf("full, Can't add task,: %s\n", task);
        return;
    }
    if (qFront == -1) qFront = 0;
    strcpy(taskQueue[++qRear], task);
}

char* removeFromQueue() {
    if (qFront == -1 ) return NULL;
    return taskQueue[qFront++];
}

// Stack Functions
void pushToStack(char task[]) {
    if (stackTop == MAX_TASKS - 1) {
        printf(" full. Can't push: %s\n", task);
        return;
    }
    strcpy(urgentStack[++stackTop], task);
}

void handleUrgentTasks() {//for handeling urgent tasks
    while (stackTop != -1) {
        printf("Urgent task being handled: %s\n", urgentStack[stackTop--]);
    }
}

// Mission Logger
void storeMission(char mission[]) {
    if (missionCount < MAX_TASKS) {
        strcpy(completedMissions[missionCount++], mission);
    } else {
        printf("Log's full. Deleting the oldest mission: %s\n", completedMissions[0]);
        for (int i = 1; i < MAX_TASKS; i++) {
            strcpy(completedMissions[i - 1], completedMissions[i]);
        }
        strcpy(completedMissions[MAX_TASKS - 1], mission);
    }
    printf("Mission '%s' has been logged.\n", mission);
}

// Damaged to Repaired
void addDamagedRobot(char name[]) {
    RobotNode* newBot = (RobotNode*)malloc(sizeof(RobotNode));//using malloc..
    strcpy(newBot->rName, name);
    newBot->next = damagedList;
    damagedList = newBot;
}

void fixRobot(char name[]) {
    RobotNode *temp = damagedList, *prev = NULL;
    while (temp && strcmp(temp->rName, name) != 0) {
        prev = temp;
        temp = temp->next;
    }
    if (!temp) return;
    if (prev == NULL) damagedList = temp->next;
    else prev->next = temp->next;

    RobotNode* newBot = (RobotNode*)malloc(sizeof(RobotNode));
    strcpy(newBot->rName, name);
    newBot->next = NULL;
    newBot->prev = repairedEnd;
    if (repairedEnd) repairedEnd->next = newBot;
    else repairedStart = newBot;
    repairedEnd = newBot;
}

void showRepairedForward() {
    RobotNode* temp = repairedStart;
    while (temp) {
        printf("%s ", temp->rName);
        temp = temp->next;
    }
    printf("\n");
}

void showRepairedBackward() {
    RobotNode* temp = repairedEnd;
    while (temp) {
        printf("%s ", temp->rName);
        temp = temp->prev;
    }
    printf("\n");
}

// Circular Linked List
void addPriorityRobot(char name[]) {
    PriorityNode* newNode = (PriorityNode*)malloc(sizeof(PriorityNode));
    strcpy(newNode->pName, name);
    if (!circleTail) {
        newNode->next = newNode;
        circleTail = newNode;
    } else {
        newNode->next = circleTail->next;
        circleTail->next = newNode;
        circleTail = newNode;
    }
}

void showPriorityLoops(int rounds) {
    if (!circleTail) return;
    PriorityNode* current = circleTail->next;
    for (int r = 0; r < rounds; r++) {
        PriorityNode* start = current;
        do {
            printf("%s -> ", current->pName);
            current = current->next;
        } while (current != start);
        printf("(loop %d)\n", r + 1);
    }
}

int main() {
    int status;

    // Enqueue predefined tasks//
    printf("Let's add the 6 tasks to the queue:\n");
    for (int i = 0; i < MAX_TASKS; i++) {
        addToQueue(taskQueue[i]);
    }

    // Dequeue and push to stack//
    while ((status = (qFront != -1 && qFront <= qRear))) {
        char* task = removeFromQueue();
        if (task) pushToStack(task);
    }

    // Urgent Task Execution
    printf("\n--- Urgent Task Execution ---\n");
    handleUrgentTasks();

    // Log predefined missions//
    char missionList[8][20] = {"Mis1", "Mis2", "Mis3", "Mis4", "Mis5", "Mis6", "Mis7", "Mis8"};
    printf("\nLogging 8 missions:\n");
    for (int i = 0; i < 8; i++) {
        storeMission(missionList[i]);
    }

    // Insert predefined damaged robots//
    char brokenBots[2][20] = {"Digger", "Drone"};
    printf("\nInserting damaged robots:\n");
    for (int i = 0; i < 2; i++) {
        addDamagedRobot(brokenBots[i]);
    }

    // Move a robot to repaired list//
    char fixedBots[1][20] = {"Digger"};
    printf("\nMoving repaired robot:\n");
    fixRobot(fixedBots[0]);

    // Display repaired robots//
    printf("\nRepaired Robots (Forward): ");
    showRepairedForward();
    printf("Repaired Robots (Backward): ");
    showRepairedBackward();

    // Insert predefined priority robots//
    char priorityBots[2][20] = {"Scanner", "Lift"};
    printf("\nInserting priority robots for circular list:\n");
    for (int i = 0; i < 2; i++) {
        addPriorityRobot(priorityBots[i]);
    }

    // Redeployment traversal//
    printf("\nHigh Priority Robot Redeployment:\n");
    showPriorityLoops(2);

    //  Why LIFO fits for urgent tasks??//
    
    printf("LIFO works best for urgent tasks like 'Cutter' because the last task to be added is the one that needs immediate action (e.g., 'Cutter' to clear debris).\n");

    //  Reason for logging missions//
    printf("\nLogging missions is essential as it helps track progress and survivor status. If a task is full, removing the oldest keeps the log updated.\n");

    //  Damage and repair example//
    printf("\n'Digger' was damaged, but then it was rebuilt and moved to the repaired list.\n");

   
    return 0;
}