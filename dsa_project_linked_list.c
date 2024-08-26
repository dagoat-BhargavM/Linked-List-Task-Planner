#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct task {
    char description[100];
    int priority;
    char due_date[20];
    int is_done;
    struct task* next;
} TASK;

// Head of the linked list
TASK* head = NULL;

// Function to compare tasks for sorting
int compareTasks(const TASK* a, const TASK* b) {
    // Compare by due date
    int dateComparison = strcmp(a->due_date, b->due_date);

    // If due dates are the same, compare by priority
    if (dateComparison == 0) {
        return a->priority - b->priority;
    }

    return dateComparison;
}

// Function to add a task to the to-do list
void addTask(char* description, int priority, char* due_date) {
    TASK* newTask = (TASK*)malloc(sizeof(TASK));
    if (newTask == NULL) {
        printf("Cannot add task.\n");
        return;
    }

    strcpy(newTask->description, description);
    newTask->priority = priority;
    strcpy(newTask->due_date, due_date);
    newTask->is_done = 0;
    newTask->next = NULL;

    // Inserting the new task at the appropriate position to maintain order
    TASK** current = &head;
    while (*current != NULL && compareTasks(newTask, *current) > 0) {
        current = &(*current)->next;
    }

    newTask->next = *current;
    *current = newTask;

    printf("Task added successfully!\n");
}

// Function to display all tasks in the to-do list
void displayTasks() {
    if (head == NULL) {
        printf("The to-do list is empty.\n");
        return;
    }

    printf("To-do List:\n");
    TASK* current = head;
    int taskNumber = 1;
    while (current != NULL) {
        // Printing task details
        printf("Task %d: Description: %s, Priority: %d, Due Date: %s, Status: %s\n",
               taskNumber++, current->description, current->priority, current->due_date,
               current->is_done ? "Done" : "Not Done");
        current = current->next;
    }
}

// Function to mark a task as done and move it to the end
void markTaskAsDone(int taskIndex) {
    TASK* current = head;
    TASK* prev = NULL;
    int currentIndex = 0;

    // Traverse the linked list to find the task at the given index
    while (current != NULL && currentIndex < taskIndex) {
        prev = current;
        current = current->next;
        currentIndex++;
    }

    if (current != NULL) {
        // If the task is already done, no need to rearrange
        if (current->is_done == 1) {
            printf("This task is already marked as done.\n");
            return;
        }

        // Mark the task as done
        current->is_done = 1;

        // If the task is not the last one, rearrange the linked list
        if (current->next != NULL) {
            // Unlink the current task
            if (prev != NULL) {
                prev->next = current->next;
            } else {
                head = current->next;
            }

            // Traverse to the end of the list
            TASK* lastTask = current;
            while (lastTask->next != NULL) {
                lastTask = lastTask->next;
            }

            // Place the completed task at the end
            lastTask->next = current;
            current->next = NULL;
        }

        printf("Task marked as done and moved to the end.\n");
    } else {
        printf("Invalid task index. Task not found.\n");
    }
}

// Function to delete a task
void deleteTask(int taskIndex) {
    TASK** current = &head;
    int currentIndex = 0;

    while (*current != NULL && currentIndex < taskIndex) {
        current = &(*current)->next;
        currentIndex++;
    }

    if (*current != NULL) {
        TASK* temp = *current;
        *current = (*current)->next;
        free(temp);
        printf("Task deleted successfully.\n");
    } else {
        printf("Invalid task index. Task not found.\n");
    }
}

// Function to free the allocated memory for the linked list
void freeTasks() {
    TASK* current = head;
    while (current != NULL) {
        TASK* temp = current;
        current = current->next;
        free(temp);
    }
}

// Main function to run the task planner
int main() {
    int choice;
    char description[100];
    int priority;
    char due_date[20];
    int taskIndex;

    do {
        printf("\nTask Planner Menu:\n");
        printf("1. Add Task\n");
        printf("2. Display Tasks\n");
        printf("3. Mark Task as Done\n");
        printf("4. Delete Task\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Task description: ");
                scanf(" %[^\n]s", description);
                printf("How important is it? (1 for high, 2 for medium, 3 for low): ");
                scanf("%d", &priority);
                printf("When is it due? (YYYY-MM-DD): ");
                scanf(" %[^\n]s", due_date);
                addTask(description, priority, due_date);
                break;
            case 2:
                displayTasks();
                break;
            case 3:
                printf("Enter the number of the task you completed: ");
                scanf("%d", &taskIndex);
                markTaskAsDone(taskIndex - 1); // Adjusting for indexing starting from 0
                break;
            case 4:
                printf("Enter the number of the task you would like to remove: ");
                scanf("%d", &taskIndex);
                deleteTask(taskIndex - 1); // Adjusting for indexing starting from 0
                break;
            case 5:
                freeTasks();  // Free memory
                printf("Exiting!\n");
                break;
            default:
                printf("Invalid choice.\n");
        }
    } while (choice != 5);

    return 0;
}
