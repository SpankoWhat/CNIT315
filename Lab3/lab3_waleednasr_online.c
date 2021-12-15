/* Lab 3 Graded - CIT315
 * Name: Waleed Nasr
 * Email: wnasr@purdue.edu
 * Lab time: Online
 *
 * General Documentation: make a list dynamic by replicating the movie Unbreakable.
 */
#include <stdio.h>
#include <stdlib.h>
#define _BITS  64

//a student struct
struct sStudent{
    char firstName[_BITS];
    char lastName[_BITS];
    char PUID[_BITS];
    int age;
    struct sStudent *nextNode;
};

//a struct for keeping updated data about the list, probably not as robust but im lazy
struct linkedList{
    struct sStudent *start;
    struct sStudent *end;
    int listSize;
};

//Function Initialization:
void CreateListNoNodes();
void CreateListNode();
void InsertFront();
void InsertMiddle();
void InsertEnd();
void DeleteFront();
void DeleteMiddle();
void DeleteEnd();
void LookUpByIndex();


struct sStudent *returnStudentInfo();
void printStudent();
void printList();
void cleanUp();
struct sStudent *findBefore();
void printMenu();

int main() {
    struct linkedList *mainList;
    int cont = 1;
    int index;

    printf("-----Welcome to Linked Array List-----\n");

    while(1){
        index = 0;
        printMenu();

        //using the output of scanf to determine proper function...
        if (scanf("%i", &cont) != 0) {
            if (cont == 11){
                cleanUp(&mainList);
                return 0;
            }

            switch (cont) {
                case 1:
                    CreateListNoNodes(&mainList);
                    break;
                case 2:
                    CreateListNode(&mainList);
                    break;
                case 3:
                    InsertFront(&mainList);
                    break;
                case 4:
                    InsertMiddle(&mainList);
                    break;
                case 5:
                    InsertEnd(&mainList);
                    break;
                case 6:
                    DeleteFront(&mainList);
                    break;
                case 7:
                    DeleteMiddle(&mainList);
                    break;
                case 8:
                    DeleteEnd(&mainList);
                    break;
                case 9:
                    while(1){
                        printf("Enter Index: \n");

                        //using the output of scanf to determine proper input...
                        if (scanf("%i", &index) == 0) {
                            printf("This is not a number, please try again...\n");
                        }else{
                            fflush(stdin);
                            break;
                        }
                        fflush(stdin);
                    }
                    LookUpByIndex(&mainList, index);
                    break;
                case 10:
                    printList(&mainList);
                    break;
                default:
                    printf("Please input a proper selection\n");
            }
        }else{
            printf("Please input a proper selection\n");
        }
        fflush(stdin);
    }

}

//Creates and sets pointers
void CreateListNoNodes(struct linkedList *mainList){
    mainList->start = NULL;
    mainList->end = NULL;
    mainList->listSize = 0;
}

//creates one node and sets pointers
void CreateListNode(struct linkedList *mainList){
    struct sStudent *newStudent = returnStudentInfo();
    mainList->start = newStudent;
    mainList->end = newStudent;
    mainList->listSize += 1;
}

//creates one node in the front and reconfigures pointers
void InsertFront(struct linkedList *mainList){
    struct sStudent *newStudent = returnStudentInfo();
    struct sStudent *oldStart = mainList->start;

    newStudent->nextNode = oldStart;
    mainList->start=newStudent;
    mainList->listSize += 1;
}

//creates one node in the middle and reconfigures pointers
//  for even numbered lists: node is placed in the middle -> () () (+) () ()
//  for odd numbered lists: node is placed before the middle node -> () (+) () ()
void InsertMiddle(struct linkedList *mainList){
    if(mainList->listSize < 3){
        printf("List size needs to be at least 3...Aborting\n");
        return;
    }
    struct sStudent *middleNode = mainList->start;

    //makes sure the counters are proper for odd and even numbered lists
    int counter = mainList->listSize/2;
    if (counter%2 == 0){
        --counter;
    }

    //basically a find by index function
    for(counter; counter > 0; --counter){
        middleNode = middleNode->nextNode;
    }

    //magic pointer reconfiguration
    struct sStudent *addMid = returnStudentInfo();
    addMid->nextNode = middleNode->nextNode;
    middleNode->nextNode = addMid;
    mainList->listSize += 1;
}

//places a new node at the end of list
void InsertEnd(struct linkedList *mainList){
    struct sStudent *newStudent = returnStudentInfo();
    struct sStudent *oldStudent = mainList->end;

    oldStudent->nextNode = newStudent;
    mainList->end = newStudent;
    mainList->listSize += 1;
}

//deletes node from the end, updates linkedList: mainList
void DeleteFront(struct linkedList *mainList){
    struct sStudent *oldStart = mainList->start;
    struct sStudent *newStart = oldStart->nextNode;
    free(oldStart);
    mainList->start = newStart;
    mainList->listSize -= 1;
}

//deletes node from the middle, updates linkedList: mainList
//  for odd numbered lists: node is placed in the middle -> () () (x) () ()
//  for even numbered lists: node is placed before the middle node -> () (x) () ()
void DeleteMiddle(struct linkedList *mainList){
    if(mainList->listSize < 3){
        printf("List size needs to be at least 3...Aborting");
        return;
    }

    struct sStudent *middleNode = mainList->start;

    int counter = mainList->listSize/2;
    if (counter%2 == 0){
        --counter;
    }

    for(int c = counter; c > 0; --c){
        middleNode = middleNode->nextNode;
    }

    struct sStudent *nodeAfter = middleNode->nextNode;
    struct sStudent *nodeBefore = findBefore(middleNode, mainList);
    nodeBefore->nextNode = nodeAfter;
    free(middleNode);
    mainList->listSize -= 1;
}

//deletes end node
void DeleteEnd(struct linkedList *mainList){
    struct sStudent *oldEnd = mainList->end;
    struct sStudent *newEnd = findBefore(oldEnd, mainList);
    free(oldEnd);
    newEnd->nextNode = NULL;
    mainList->end = newEnd;
    mainList->listSize -= 1;
}

//irritates through the linked list with the help of mainList.size
void LookUpByIndex(struct linkedList *mainList, int index){
    struct sStudent *counter;
    counter = mainList->start;

    if(counter == NULL){
        printf("List is empty\n");
        return;
    }

    if(index+1 > mainList->listSize){
        printf("Invalid index pointer\n");
        return;
    }

    while(counter && index > 0){
        --index;
        counter = counter->nextNode;
    }

    printStudent(counter);
}

//Handles the memory allocation, assignment of values and input handling.
//Probably better if made input handling a function.
struct sStudent *returnStudentInfo(){
    struct sStudent *student;
    student = malloc(sizeof(struct sStudent));

    if(student == NULL){
        printf("ERROR");
    }

    printf("Enter student first name: \n");
    scanf("%s",student->firstName);
    printf("Enter student last name: \n");
    scanf("%s",student->lastName);
    printf("Enter student PUID: \n");
    scanf("%s",student->PUID);

    while(1){
        printf("Enter student Age: \n");

        //using the output of scanf to determine proper input...
        if (scanf("%i", &student->age) == 0) {
            printf("This is not a number, please try again...\n");
        }else if(student->age < 0){
            printf("Please input an age greater than 0\n");
        }else{
            fflush(stdin);
            break;
        }
        fflush(stdin);
    }

    student->nextNode = NULL;

    return student;
}

//Handles printing a single student
void printStudent(struct sStudent *student){
    if (student==NULL){
        printf("INVALID STUDENT");
    }else{
        printf("Student Memory Address: %p\n"
               "..First Name: %s\n"
               "..Last Name: %s\n"
               "..PUID: %s\n"
               "..Age: %i\n"
               "..Next Address: %p\n",
               student, student->firstName, student->lastName, student->PUID,
               student->age, student->nextNode);
    }
}

//Handles going through each student and calling printStudent()
void printList(struct linkedList *list){
    struct sStudent *counter;
    counter = list->start;

    if(counter == NULL){
        printf("List is Empty\n");
        return;
    }
    while(counter) {
        printStudent(counter);
        counter = counter->nextNode;
    }
}

//cleans memory when choosing to exit
void cleanUp(struct linkedList *mainList){
    struct sStudent *nextQ;
    struct sStudent *next;
    next = mainList->start;

    printf("\n--------Memory Cleaning--------\n");

    while (next){
        printf("Clean Memory Location: %p\n", next);
        nextQ = next->nextNode;
        free(next);
        next = nextQ;
    }
}

//A helper function used to find the address of the previous node
struct sStudent *findBefore(struct sStudent *location, struct linkedList *mainList){
    struct sStudent *counter = mainList->start;
    struct sStudent *selector = NULL;

    while(counter != location){
        selector = counter;
        counter = counter->nextNode;
    }

    return selector;
}

//Helper function used to display the menu
void printMenu() {
    printf("Please select from one of the functions (must use function 1 first):\n"
           "  1)  Create list with no node\n"
           "  2)  Create list with one node\n"
           "  3)  Insert Front\n"
           "  4)  Insert Middle\n"
           "  5)  Insert End\n"
           "  6)  Delete Front\n"
           "  7)  Delete Middle\n"
           "  8)  Delete End\n"
           "  9)  Look up by index\n"
           "  10) Traverse\n"
           "  11)  Quit\n\n");

    printf("Your Selection:\n");
}
