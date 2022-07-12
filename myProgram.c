// FINAL VERSION
// Alex Kelso
// Computer Systems Organization


// PART 2 - include necessary header files 
#include <stdio.h> // to use printf and scanf
#include <stdlib.h> // to use malloc 
#include <string.h> // to use strlen, strcmp, strcpy 

// PART 15 - defining constants
// these would typically be included in a separate header file, 
// along with the function prototypes below
#define FORWARD 0 // direction const
#define BACKWARD 1 // direction const


// PART 3 - use typedef to define a struct type PERSONNEL_RECORD 
typedef struct {
    char *ln, *fn; // last name, first name
    char mi; // middle initial
    int a,s; // age and salary
    long id; //id number
} PERSONNEL_RECORD; 


// PART 8 - use typedef to define a struct type NODE for the binary tree 
typedef struct NODE {
    PERSONNEL_RECORD *record; // pointer to a PERSONNEL_RECORD
    struct NODE *left, *right; // pointers to left and right NODEs
} NODE;


// PART 12 - use typedef to define a struct type CELL for the DLL
typedef struct CELL {
    PERSONNEL_RECORD *record; // pointer to a PERSONNEL_RECORD
    struct CELL *next, *prev; // pointers to next and prev CELLs
} CELL;


// FUNCTION PROTOTYPE SECTION - where all the functions are declared
// includes the parameters and parameter types for each function
// these would usually be included in a separate header file, along
// with the constants that use the #define keyword
void print_record(PERSONNEL_RECORD *);
int compare_id_num(PERSONNEL_RECORD *p1, PERSONNEL_RECORD *p2);
int compare_name(PERSONNEL_RECORD *p1, PERSONNEL_RECORD *p2);
int compare_age(PERSONNEL_RECORD *p1, PERSONNEL_RECORD *p2);
int compare_salary(PERSONNEL_RECORD *p1, PERSONNEL_RECORD *p2);
PERSONNEL_RECORD * new_record(char *ln, char *fn, char mi, int a, long id, int s);
PERSONNEL_RECORD * read_record();
void insert_personnel_record(NODE **root, PERSONNEL_RECORD *ptr, int (*compFunc)());
void traverse_and_print(NODE *root);
void insert_record_in_list(PERSONNEL_RECORD *recPtr);
void insert_from_tree_into_list(NODE *root);
void print_list(int direction);
void print_n_records(int n);


// PART 9 - declare and initialize 4 global variables each of type NODE* representing BST roots
NODE* name_root = NULL;
NODE* age_root = NULL;
NODE* id_num_root = NULL;
NODE* salary_root = NULL;

// PART 12 - declare and initialize global variable head of type CELL* for the DLL
CELL* head = NULL;


// PART 17 - MAIN PROGRAM
int main() {
    PERSONNEL_RECORD *pRec;

    // testing all of the functions
    while ((pRec = read_record()) != NULL) {
        int (*f1) (PERSONNEL_RECORD*, PERSONNEL_RECORD*) = compare_name;
        insert_personnel_record(&name_root, pRec, f1);

        int (*f2) (PERSONNEL_RECORD*, PERSONNEL_RECORD*) = compare_age;
        insert_personnel_record(&age_root, pRec, f2);

        int (*f3) (PERSONNEL_RECORD*, PERSONNEL_RECORD*) = compare_id_num;
        insert_personnel_record(&id_num_root, pRec, f3);

        int (*f4) (PERSONNEL_RECORD*, PERSONNEL_RECORD*) = compare_salary;
        insert_personnel_record(&salary_root, pRec, f4);
    }

    // testing traversal and print function
    printf("\nSorted by name:\n");
    traverse_and_print(name_root);

    printf("\nSorted by age:\n");
    traverse_and_print(age_root);

    printf("\nSorted by ID number:\n");
    traverse_and_print(id_num_root);

    printf("\nSorted by salary:\n");
    traverse_and_print(salary_root);

    insert_from_tree_into_list(name_root);

    // testing printing order function
    printf("\nList printed in forward direction:\n");
    print_list(FORWARD);

    printf("\nList printed in backward direction: \n");
    print_list(BACKWARD);

    // testing printing first n records (n=120)
    printf("\nPrinting first 120 records:\n");
    print_n_records(120);
    printf("\n");

    return 0;
}


// PART 4 - write several functions that take two pointers (p1,p2) and compare the data fields above and return an int
int compare_id_num(PERSONNEL_RECORD *p1, PERSONNEL_RECORD *p2) { //return pos# if p1 id num > p2 id num, neg# if p1 id num < p2 id num, or 0 otherwise
    if ((p1->id) > (p2->id))
        return 1; // if p1 id > p2 id, return 1
    else if ((p2->id) > (p1->id))
        return -1; // if p2 id > p1 id, return -1
    else
        return 0; // default case: if p1 id = p2 id, return 0
}

int compare_name(PERSONNEL_RECORD *p1, PERSONNEL_RECORD *p2) { 
    // start by comparing last names 
    int lastcmp = strcmp((p1->ln), (p2->ln));
    int firstcmp = strcmp((p1->fn), (p2->fn));

    if (lastcmp != 0) {
        return lastcmp; // if last names dont match, return strcmp result
    } 
    else { // if last names do match, compare first names
        if (firstcmp != 0) {
            return firstcmp; // if first names dont match, return strcmp result
        }

        // if last and first names match, compare middle initials (chars, so cant use strcmp)
        else {
            if (p1->mi > p2->mi) {
                return 1; // return pos# if p1 MI > p2 MI
            }
            else if (p2->mi > p1->mi) {
                return -1; // return neg# if p2 MI > p1 MI
            }
            else {// default case: if middle initials are same compare ID nums
                return compare_id_num(p1, p2);
            }
        }
    }
}

int compare_age(PERSONNEL_RECORD *p1, PERSONNEL_RECORD *p2) {
    if ((p1->a) > (p2->a))
        return 1; // if p1 is older than p2
    else if ((p2->a) > (p1->a))
        return -1; // if p2 is older than p1
    else
        return compare_name(p1, p2); // default case: if p1 and p2 are same age compare names
}

int compare_salary(PERSONNEL_RECORD *p1, PERSONNEL_RECORD *p2) {
   if ((p1->s) > (p2->s))
        return 1; // if p1 salary > p2 salary
    else if ((p2->s) > (p1->s))
        return -1; // if p2 salary > p1 salary
    else
        return compare_name(p1, p2); // default case: if p1 and p2 are same salary, compare names 
}


// PART 5 - new_record function that returns a pointer of type PERSONNEL_RECORD
// pointer that is returned points to the address of the new record
PERSONNEL_RECORD * new_record(char *ln, char *fn, char mi, int a, long id, int s){
    // use malloc to allocate memory for a new PERSONNEL_RECORD struct 
    PERSONNEL_RECORD *ptr = (PERSONNEL_RECORD *) malloc(sizeof(PERSONNEL_RECORD));

    ptr->ln = (char*) malloc(sizeof(char*)*(strlen(ln)+1));
    strcpy(ptr->ln, ln);
    ptr->fn = (char*) malloc(sizeof(char*)*(strlen(fn)+1));
    strcpy(ptr->fn, fn);
    ptr->mi = mi;
    ptr->a = a;
    ptr->id = id;
    ptr->s = s;

    // returns pointer, which points to new record's address
    return ptr;
}


// PART 6 - read_record function that reads in info from a user
PERSONNEL_RECORD * read_record(){
    // declare variables to hold information
    char readLast[100], readFirst[100], readMid;
    int readAge, readSal;
    long readID;

   if (scanf("%s %s %c %d %ld %d", readLast, readFirst, &readMid, &readAge, &readID, &readSal) != EOF)
      // returns pointer of type PERSONNEL_RECORD, indicating where in memory this new information is stored
      return new_record(readLast, readFirst, readMid, readAge, readID, readSal); 
   else 
      return NULL; // returns null if EOF 
}


// PART 7 - takes a pointer to a PERSONNEL_RECORD and prints all fields of record
void print_record(PERSONNEL_RECORD *p) { 
    printf("\n %s %s %c %i %ld %i", p->ln, p->fn, p->mi, p->a, p->id, p->s); // display record info
}


// PART 10 - define a function insert_personnel_record 
// parameters: pointer to a root, pointer to a PERSONNEL_RECORD, and pointer to comparison function
void insert_personnel_record(NODE** root, PERSONNEL_RECORD* ptr, int (*compFunc)()) {
    // create new node using malloc
    NODE* newNode = (NODE*) malloc(sizeof(NODE));
    newNode->record = ptr;

    // check if root is null and if it is, assign newNode to root
    if (*root == NULL){
        *root = newNode;
        return;
    }

    // call compFunc on root and ptr
    if (compFunc(ptr, (*root)->record) <= 0) // if less than, add to left of root
        insert_personnel_record(&((*root)->left), ptr, compFunc);
    else // else add to right of root
        insert_personnel_record(&((*root)->right), ptr, compFunc);
}


// PART 11 - define a function traverse_and_print that takes a pointer of type NODE* to a tree
// and traverses the tree in order, printing out the record encountered at each node
void traverse_and_print(NODE *root) {
    if (root != NULL) { // if end hasn't been reached, print recursively until it has 
        traverse_and_print(root->left);
        print_record(root->record); // print entire record at each node
        traverse_and_print(root->right);
    }
}


// PART 13 - define a function insert_record_in_list that takes a PERSONNEL_RECORD pointer as a parameter
void insert_record_in_list(PERSONNEL_RECORD *recPtr) {
    // allocate memory for a new CELL using malloc
    CELL *newCell = (CELL*) malloc(sizeof(struct CELL));
    CELL *temp;

    // initialize the newCell's member fields
    newCell->record = recPtr;
    newCell->next = NULL;
    newCell->prev = NULL;

    if (head == NULL) { // if head CELL is empty
        // the newCell is the head CELL
        head = newCell;
        newCell->next = head;
        newCell->prev = head;
    }
    else { // if head CELL is not empty
        temp = head;
        while (temp->next != head) { // keep iterating until you reach the end of the DLL
            temp = temp->next;
        }
        // inserting the new CELL at the enf o the DLL
        temp->next = newCell;
        newCell->next = head;
        newCell->prev = temp;
        head->prev = newCell;
    }
}


// PART 14 - define a function insert_from_tree_into_list which takes a pointer of type NODE to a tree 
// as a parameter and traverses the tree using an in-order traversal
void insert_from_tree_into_list(NODE *root) {
    if (root == NULL) return; // base case

    // traverse tree using recursive in order traversal algorithm from above
    if (root != NULL) { 
        // recursion
        insert_from_tree_into_list(root->left);
        // insert record at this node in the tree into the doubly linked list by calling 
        // the insert_record_in_list function above and passing the record's pointer
        insert_record_in_list(root->record);
        insert_from_tree_into_list(root->right);
    }
}

// PART 15 - define a function print_list that take a parameter direction of type integer
void print_list(int direction) {
    if (direction == FORWARD){ // use const defined in beginning of program (0 = FORWARD)
        // initialize head cell to be current temp cell
        CELL *temp = head;
        while (temp->next != head) {
            print_record(temp->record);
            temp = temp->next;    
        }
    }   

    else if (direction == BACKWARD) { // use const defined in beginning of program (1 = BACKWARD)
        // initialize tail cell to be prev cell of head (circular)
        CELL *tail = head->prev;
        // then create a temp cell and set it equal to tail cell as a sentinel 
        CELL *temp = tail;
        while(temp->prev != tail) {
            print_record(temp->record);
            temp = temp->prev;
        }
        printf("\n");
    }
}


// PART 16 - define a function print_n_records that takes an int n as a parameter and prints
// the first n personnel records in the DLL
void print_n_records(int n) {
    int i;
    CELL *temp = head;

    // iterate through n cells in DLL and print info
    for (i=0; i<n; i++) {
        print_record(temp->record);
        temp = temp->next;
    }
}