#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// ANSI color codes
#define PINK "\033[38;5;219m"
#define LIGHT_PURPLE "\033[38;5;183m"
#define PURPLE "\033[38;5;141m"
#define CYAN "\033[38;5;123m"
#define YELLOW "\033[38;5;229m"
#define GREEN "\033[38;5;157m"
#define RED "\033[38;5;211m"
#define RESET "\033[0m"
#define BOLD "\033[1m"

// Declaration of TypeElement
typedef int TypeElm;

// Declaration of a Stack
typedef struct SNode {
    TypeElm Data;
    struct SNode* Next;
} SNode;

typedef SNode* Stack;

// Declaration of a Queue
typedef struct QNode {
    TypeElm Data;
    struct QNode* Next;
} QNode;

typedef struct {
    QNode* Head;
    QNode* Tail;
} Queue;

// Subtask data structure
typedef struct {
    char name[50];  // Increased size for longer names
    bool completed;
} Subtask;

// Task data structure
typedef struct {
    char name[50];  // Increased size for longer names
    bool completed;
    Subtask** subtasks;  // Array of subtask pointers
    int subtaskCount;    // Count of subtasks
} Maintask;

// Stack operations
void InitStack(Stack* P) {
    *P = NULL;
}

bool isSEmpty(Stack P) {
    return P == NULL;
}

void Push(Stack* P, TypeElm x) {
    SNode* V = malloc(sizeof(SNode));
    if (V == NULL) {
        printf("%sMemory allocation failed!%s\n", RED, RESET);
        return;
    }
    V->Data = x;
    V->Next = *P;
    *P = V;
}

bool Pop(Stack* P, TypeElm* x) {
    if (isSEmpty(*P)) {
        return false;
    }
    SNode* V = *P;
    *x = V->Data;
    *P = V->Next;
    free(V);
    return true;
}

TypeElm Top(Stack P) {
    if (isSEmpty(P)) {
        printf("%sError: Stack is empty%s\n", RED, RESET);
        exit(1);
    }
    return P->Data;
}

// Queue operations
void InitQueue(Queue* F) {
    F->Head = NULL;
    F->Tail = NULL;
}

bool isQEmpty(Queue F) {
    return F.Head == NULL;
}

void Enqueue(Queue* F, TypeElm x) {
    QNode* V = malloc(sizeof(QNode));
    if (V == NULL) {
        printf("%sMemory allocation failed!%s\n", RED, RESET);
        return;
    }
    V->Data = x;
    V->Next = NULL;
    if (F->Head == NULL) {
        F->Head = V;
    } else {
        F->Tail->Next = V;
    }
    F->Tail = V;
}

bool Dequeue(Queue* F, TypeElm* x) {
    if (isQEmpty(*F)) {
        return false;
    }
    QNode* V = F->Head;
    if (x != NULL) {
        *x = V->Data;
    }
    if (F->Head == F->Tail) {
        F->Head = NULL;
        F->Tail = NULL;
    } else {
        F->Head = V->Next;
    }
    free(V);
    return true;
}

TypeElm Head(Queue F) {
    if (isQEmpty(F)) {
        printf("%sError: Queue is empty%s\n", RED, RESET);
        exit(1);
    }
    return F.Head->Data;
}

// Task management functions
void createproject(char name[]) {
    printf("\n%s? The %s'%s%s'%s project was created %s?\n", 
           LIGHT_PURPLE, PINK, name, LIGHT_PURPLE, PINK, LIGHT_PURPLE);
}

void addmaintask(Queue* Q, Maintask* task) {
    Enqueue(Q, (TypeElm)task);
}

void addsubtask(Maintask* task, Subtask* subtask) {
    // Reallocate the subtasks array
    task->subtaskCount++;
    task->subtasks = realloc(task->subtasks, task->subtaskCount * sizeof(Subtask*));
    if (task->subtasks == NULL) {
        printf("%sMemory allocation failed!%s\n", RED, RESET);
        task->subtaskCount--;
        return;
    }
    task->subtasks[task->subtaskCount - 1] = subtask;
}

void markcompletedtask(Queue* Q, char taskName[]) {
    if (isQEmpty(*Q)) {
        printf("%sNo tasks available.%s\n", RED, RESET);
        return;
    }

    Queue tempQ;
    InitQueue(&tempQ);
    bool found = false;

    while (!isQEmpty(*Q)) {
        Maintask* task = (Maintask*)Head(*Q);
        if (strcmp(task->name, taskName) == 0) {
            found = true;
            if (task->completed) {
                printf("%sTask '%s%s%s' was already completed.%s\n", 
                      YELLOW, PINK, taskName, YELLOW, RESET);
            } else {
                task->completed = true;
                printf("%s? Task '%s%s%s' is now completed! %s??%s\n", 
                      GREEN, PINK, taskName, GREEN, PINK, RESET);
            }
        }
        Enqueue(&tempQ, Head(*Q));
        Dequeue(Q, NULL);
    }

    while (!isQEmpty(tempQ)) {
        Enqueue(Q, Head(tempQ));
        Dequeue(&tempQ, NULL);
    }

    if (!found) {
        printf("%sCouldn't find task '%s%s%s'. Please check the name.%s\n", 
              YELLOW, PINK, taskName, YELLOW, RESET);
    }
}

void markcompletedsubtask(Queue* Q, char taskName[], char subtaskName[]) {
    if (isQEmpty(*Q)) {
        printf("%sNo tasks available.%s\n", RED, RESET);
        return;
    }

    Queue tempQ;
    InitQueue(&tempQ);
    bool found = false;

    while (!isQEmpty(*Q)) {
        Maintask* task = (Maintask*)Head(*Q);
        if (strcmp(task->name, taskName) == 0) {
        	int i;
            for ( i = 0; i < task->subtaskCount; i++) {
                if (strcmp(task->subtasks[i]->name, subtaskName) == 0) {
                    found = true;
                    if (task->subtasks[i]->completed) {
                        printf("%sSubtask '%s%s%s' was already completed.%s\n", 
                              YELLOW, PINK, subtaskName, YELLOW, RESET);
                    } else {
                        task->subtasks[i]->completed = true;
                        printf("%s? Subtask '%s%s%s' is now completed! %s??%s\n", 
                              GREEN, PINK, subtaskName, GREEN, PINK, RESET);
                    }
                    break;
                }
            }
        }
        Enqueue(&tempQ, Head(*Q));
        Dequeue(Q, NULL);
    }

    while (!isQEmpty(tempQ)) {
        Enqueue(Q, Head(tempQ));
        Dequeue(&tempQ, NULL);
    }

    if (!found) {
        printf("%sCouldn't find subtask '%s%s%s'. Please check the name.%s\n", 
              YELLOW, PINK, subtaskName, YELLOW, RESET);
    }
}

void viewtasks(Queue Q) {
    if (isQEmpty(Q)) {
        printf("%sThere are no tasks to display.%s\n", YELLOW, RESET);
    } else {
        Queue tempQ;
        InitQueue(&tempQ);
        printf("\n%s?················%s\n", PINK, RESET);
        printf("%s?  %sTASK LIST  %s?%s\n", LIGHT_PURPLE, PINK, LIGHT_PURPLE, RESET);
        printf("%s?················%s\n\n", PINK, RESET);

        while (!isQEmpty(Q)) {
            Maintask* task = (Maintask*)Head(Q);
            if (task->completed) {
                printf("%s? %s%s %s(completed)%s\n", GREEN, PINK, task->name, LIGHT_PURPLE, RESET);
            } else {
                printf("%s? %s%s%s\n", YELLOW, PINK, task->name, RESET);
            }

            // Display subtasks
            if (task->subtaskCount > 0) {
            	int i;
                for (i = 0; i < task->subtaskCount; i++) {
                    if (task->subtasks[i]->completed) {
                        printf("  %s? %s%s %s(completed)%s\n", 
                              GREEN, CYAN, task->subtasks[i]->name, LIGHT_PURPLE, RESET);
                    } else {
                        printf("  %s? %s%s%s\n", YELLOW, CYAN, task->subtasks[i]->name, RESET);
                    }
                }
            }
            
            Enqueue(&tempQ, Head(Q));
            Dequeue(&Q, NULL);
        }

        printf("\n%s?················%s\n", PINK, RESET);

        while (!isQEmpty(tempQ)) {
            Enqueue(&Q, Head(tempQ));
            Dequeue(&tempQ, NULL);
        }
    }
}

void deletetask(Queue* Q, char taskName[]) {
    if (isQEmpty(*Q)) {
        printf("%sNo tasks available to delete.%s\n", RED, RESET);
        return;
    }

    Queue tempQ;
    InitQueue(&tempQ);
    bool found = false;

    while (!isQEmpty(*Q)) {
        Maintask* task = (Maintask*)Head(*Q);
        if (strcmp(task->name, taskName) == 0) {
            found = true;
            // Free all subtasks
            int i;
            for (i = 0; i < task->subtaskCount; i++) {
                free(task->subtasks[i]);
            }
            free(task->subtasks);
            free(task);
            printf("%s? Task '%s%s%s' deleted successfully! %s?%s\n", 
                  PURPLE, PINK, taskName, PURPLE, PINK, RESET);
        } else {
            Enqueue(&tempQ, Head(*Q));
        }
        Dequeue(Q, NULL);
    }

    while (!isQEmpty(tempQ)) {
        Enqueue(Q, Head(tempQ));
        Dequeue(&tempQ, NULL);
    }

    if (!found) {
        printf("%sCouldn't find task '%s%s%s'. Please check the name.%s\n", 
              YELLOW, PINK, taskName, YELLOW, RESET);
    }
}

void deletesubtask(Queue* Q, char taskName[], char subtaskName[]) {
    if (isQEmpty(*Q)) {
        printf("%sNo tasks available.%s\n", RED, RESET);
        return;
    }

    Queue tempQ;
    InitQueue(&tempQ);
    bool found = false;

    while (!isQEmpty(*Q)) {
        Maintask* task = (Maintask*)Head(*Q);
        if (strcmp(task->name, taskName) == 0) {
        	int i;
            for (i = 0; i < task->subtaskCount; i++) {
                if (strcmp(task->subtasks[i]->name, subtaskName) == 0) {
                    found = true;
                    free(task->subtasks[i]);
                    
                    // Shift remaining subtasks
                    int j;
                    for (j = i; j < task->subtaskCount - 1; j++) {
                        task->subtasks[j] = task->subtasks[j + 1];
                    }
                    
                    task->subtaskCount--;
                    if (task->subtaskCount > 0) {
                        task->subtasks = realloc(task->subtasks, task->subtaskCount * sizeof(Subtask*));
                    } else {
                        free(task->subtasks);
                        task->subtasks = NULL;
                    }
                    
                    printf("%s? Subtask '%s%s%s' deleted successfully! %s?%s\n", 
                          PURPLE, PINK, subtaskName, PURPLE, PINK, RESET);
                    break;
                }
            }
        }
        Enqueue(&tempQ, Head(*Q));
        Dequeue(Q, NULL);
    }

    while (!isQEmpty(tempQ)) {
        Enqueue(Q, Head(tempQ));
        Dequeue(&tempQ, NULL);
    }

    if (!found) {
        printf("%sCouldn't find subtask '%s%s%s'. Please check the name.%s\n", 
              YELLOW, PINK, subtaskName, YELLOW, RESET);
    }
}

void displayMenu() {
    printf("\n%s?··························?%s\n", PINK, RESET);
    printf("%s? %s? TASK MANAGER ? %s?%s\n", LIGHT_PURPLE, PINK, LIGHT_PURPLE, RESET);
    printf("%s?··························?%s\n\n", PINK, RESET);
    
    printf("%s 1 %s Create a project %s?%s\n", PINK, LIGHT_PURPLE, PINK, RESET);
    printf("%s 2 %s Add a main task %s?%s\n", PINK, LIGHT_PURPLE, PINK, RESET);
    printf("%s 3 %s Add a subtask %s?%s\n", PINK, LIGHT_PURPLE, PINK, RESET);
    printf("%s 4 %s Mark a task as completed %s?%s\n", PINK, LIGHT_PURPLE, PINK, RESET);
    printf("%s 5 %s Mark a subtask as completed %s?%s\n", PINK, LIGHT_PURPLE, PINK, RESET);
    printf("%s 6 %s View all tasks %s?%s\n", PINK, LIGHT_PURPLE, PINK, RESET);
    printf("%s 7 %s Delete a task %s?%s\n", PINK, LIGHT_PURPLE, PINK, RESET);
    printf("%s 8 %s Delete a subtask %s?%s\n", PINK, LIGHT_PURPLE, PINK, RESET);
    printf("%s 9 %s Exit %s?%s\n", PINK, LIGHT_PURPLE, PINK, RESET);
    printf("\n%s?··························?%s\n", PINK, RESET);
}

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main() {
    int choice;
    Queue taskQueue;
    InitQueue(&taskQueue);
    char projectName[50] = "";
    char input[50];
    
    // Show welcome message
    printf("\n%s???: *???:* *:???*:???%s\n", PINK, RESET);
    printf("%s  WELCOME TO YOUR%s\n", LIGHT_PURPLE, RESET);
    printf("%s? PRETTY TASK MANAGER ?%s\n", PINK, RESET);
    printf("%s???: *???:* *:???*:???%s\n\n", PINK, RESET);
    
    displayMenu();

    do {
        printf("\n%sEnter your choice %s(1-9)%s: ", LIGHT_PURPLE, PINK, RESET);
        if (scanf("%d", &choice) != 1) {
            printf("%sPlease enter a valid number.%s\n", RED, RESET);
            clearInputBuffer();
            continue;
        }
        clearInputBuffer(); // Clear any remaining input

        switch (choice) {
            case 1: // Create a project
                if (projectName[0] == '\0') {
                    printf("%sEnter project name: %s", LIGHT_PURPLE, RESET);
                    if (fgets(input, sizeof(input), stdin) != NULL) {
                        // Remove newline if present
                        input[strcspn(input, "\n")] = 0;
                        strncpy(projectName, input, sizeof(projectName) - 1);
                        projectName[sizeof(projectName) - 1] = '\0'; // Ensure null termination
                        createproject(projectName);
                    }
                } else {
                    printf("%sProject '%s%s%s' already exists!%s\n", 
                          YELLOW, PINK, projectName, YELLOW, RESET);
                }
                break;
                
            case 2: // Add a main task
                if (projectName[0] == '\0') {
                    printf("%sPlease create a project first.%s\n", YELLOW, RESET);
                } else {
                    Maintask* newTask = malloc(sizeof(Maintask));
                    if (newTask == NULL) {
                        printf("%sMemory allocation failed!%s\n", RED, RESET);
                        break;
                    }
                    
                    printf("%sEnter main task name: %s", LIGHT_PURPLE, RESET);
                    if (fgets(input, sizeof(input), stdin) != NULL) {
                        // Remove newline if present
                        input[strcspn(input, "\n")] = 0;
                        strncpy(newTask->name, input, sizeof(newTask->name) - 1);
                        newTask->name[sizeof(newTask->name) - 1] = '\0'; // Ensure null termination
                        
                        newTask->completed = false;
                        newTask->subtasks = NULL;
                        newTask->subtaskCount = 0;
                        addmaintask(&taskQueue, newTask);
                        printf("%s? Main task '%s%s%s' added successfully! %s?%s\n", 
                              PURPLE, PINK, newTask->name, PURPLE, PINK, RESET);
                    }
                }
                break;
                
            case 3: // Add a subtask
                if (projectName[0] == '\0') {
                    printf("%sPlease create a project first.%s\n", YELLOW, RESET);
                } else if (isQEmpty(taskQueue)) {
                    printf("%sNo main tasks available. Please add a main task first.%s\n", 
                          YELLOW, RESET);
                } else {
                    char taskName[50];
                    printf("%sEnter the main task name for this subtask: %s", LIGHT_PURPLE, RESET);
                    if (fgets(input, sizeof(input), stdin) != NULL) {
                        // Remove newline if present
                        input[strcspn(input, "\n")] = 0;
                        strncpy(taskName, input, sizeof(taskName) - 1);
                        taskName[sizeof(taskName) - 1] = '\0'; // Ensure null termination
                        
                        // Find the main task
                        Queue tempQ;
                        InitQueue(&tempQ);
                        bool found = false;
                        Maintask* foundTask = NULL;
                        
                        while (!isQEmpty(taskQueue)) {
                            Maintask* task = (Maintask*)Head(taskQueue);
                            if (strcmp(task->name, taskName) == 0) {
                                found = true;
                                foundTask = task;
                            }
                            Enqueue(&tempQ, Head(taskQueue));
                            Dequeue(&taskQueue, NULL);
                        }
                        
                        // Restore the queue
                        while (!isQEmpty(tempQ)) {
                            Enqueue(&taskQueue, Head(tempQ));
                            Dequeue(&tempQ, NULL);
                        }
                        
                        if (found && foundTask != NULL) {
                            Subtask* newSubtask = malloc(sizeof(Subtask));
                            if (newSubtask == NULL) {
                                printf("%sMemory allocation failed!%s\n", RED, RESET);
                                break;
                            }
                            
                            printf("%sEnter subtask name: %s", LIGHT_PURPLE, RESET);
                            if (fgets(input, sizeof(input), stdin) != NULL) {
                                // Remove newline if present
                                input[strcspn(input, "\n")] = 0;
                                strncpy(newSubtask->name, input, sizeof(newSubtask->name) - 1);
                                newSubtask->name[sizeof(newSubtask->name) - 1] = '\0';
                                
                                newSubtask->completed = false;
                                addsubtask(foundTask, newSubtask);
                                printf("%s? Subtask '%s%s%s' added successfully! %s?%s\n", 
                                      PURPLE, PINK, newSubtask->name, PURPLE, PINK, RESET);
                            }
                        } else {
                            printf("%sCouldn't find main task '%s%s%s'. Please check the name.%s\n", 
                                  YELLOW, PINK, taskName, YELLOW, RESET);
                        }
                    }
                }
                break;
                
            case 4: // Mark a task as completed
                if (projectName[0] == '\0') {
                    printf("%sPlease create a project first.%s\n", YELLOW, RESET);
                } else if (isQEmpty(taskQueue)) {
                    printf("%sNo tasks available to mark as completed.%s\n", YELLOW, RESET);
                } else {
                    printf("%sEnter the name of the task to mark as completed: %s", LIGHT_PURPLE, RESET);
                    if (fgets(input, sizeof(input), stdin) != NULL) {
                        input[strcspn(input, "\n")] = 0; // Remove newline
                        markcompletedtask(&taskQueue, input);
                    }
                }
                break;
                
            case 5: // Mark a subtask as completed
                if (projectName[0] == '\0') {
                    printf("%sPlease create a project first.%s\n", YELLOW, RESET);
                } else if (isQEmpty(taskQueue)) {
                    printf("%sNo tasks available.%s\n", YELLOW, RESET);
                } else {
                    char taskName[50], subtaskName[50];
                    
                    printf("%sEnter the name of the main task: %s", LIGHT_PURPLE, RESET);
                    if (fgets(input, sizeof(input), stdin) != NULL) {
                        input[strcspn(input, "\n")] = 0; // Remove newline
                        strncpy(taskName, input, sizeof(taskName) - 1);
                        taskName[sizeof(taskName) - 1] = '\0';
                        
                        printf("%sEnter the name of the subtask to mark as completed: %s", LIGHT_PURPLE, RESET);
                        if (fgets(input, sizeof(input), stdin) != NULL) {
                            input[strcspn(input, "\n")] = 0; // Remove newline
                            strncpy(subtaskName, input, sizeof(subtaskName) - 1);
                            subtaskName[sizeof(subtaskName) - 1] = '\0';
                            
                            markcompletedsubtask(&taskQueue, taskName, subtaskName);
                        }
                    }
                }
                break;
                
            case 6: // View tasks
                if (projectName[0] == '\0') {
                    printf("%sPlease create a project first.%s\n", YELLOW, RESET);
                } else {
                    printf("\n%s? Tasks in project '%s%s%s' ?%s\n", 
                          LIGHT_PURPLE, PINK, projectName, LIGHT_PURPLE, RESET);
                    viewtasks(taskQueue);
                }
                break;
                
            case 7: // Delete a task
                if (projectName[0] == '\0') {
                    printf("%sPlease create a project first.%s\n", YELLOW, RESET);
                } else if (isQEmpty(taskQueue)) {
                    printf("%sNo tasks available to delete.%s\n", YELLOW, RESET);
                } else {
                    printf("%sEnter the name of the task to delete: %s", LIGHT_PURPLE, RESET);
                    if (fgets(input, sizeof(input), stdin) != NULL) {
                        input[strcspn(input, "\n")] = 0; // Remove newline
                        deletetask(&taskQueue, input);
                    }
                }
                break;
                
            case 8: // Delete a subtask
                if (projectName[0] == '\0') {
                    printf("%sPlease create a project first.%s\n", YELLOW, RESET);
                } else if (isQEmpty(taskQueue)) {
                    printf("%sNo tasks available.%s\n", YELLOW, RESET);
                } else {
                    char taskName[50], subtaskName[50];
                    
                    printf("%sEnter the name of the main task: %s", LIGHT_PURPLE, RESET);
                    if (fgets(input, sizeof(input), stdin) != NULL) {
                        input[strcspn(input, "\n")] = 0; // Remove newline
                        strncpy(taskName, input, sizeof(taskName) - 1);
                        taskName[sizeof(taskName) - 1] = '\0';
                        
                        printf("%sEnter the name of the subtask to delete: %s", LIGHT_PURPLE, RESET);
                        if (fgets(input, sizeof(input), stdin) != NULL) {
                            input[strcspn(input, "\n")] = 0; // Remove newline
                            strncpy(subtaskName, input, sizeof(subtaskName) - 1);
                            subtaskName[sizeof(subtaskName) - 1] = '\0';
                            
                            deletesubtask(&taskQueue, taskName, subtaskName);
                        }
                    }
                }
                break;
                
            case 9: // Exit
                printf("\n%s???: *???:* %sGoodbye!%s *:???*:???%s\n\n", 
                      PINK, LIGHT_PURPLE, PINK, RESET);
                break;
                
            default:
                printf("%sInvalid choice. Please enter a number between 1 and 9.%s\n", RED, RESET);
        }
    } while (choice != 9);

    // Clean up memory before exiting
    while (!isQEmpty(taskQueue)) {
        Maintask* task = (Maintask*)Head(taskQueue);
        // Free all subtasks
        int i;
        for (i = 0; i < task->subtaskCount; i++) {
            free(task->subtasks[i]);
        }
        free(task->subtasks);
        free(task);
        Dequeue(&taskQueue, NULL);
    }

    return 0;
}
