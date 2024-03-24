//BT20CSE034 Dhanesh Choudhary
//DSPD2 HW Assignment 2

#ifndef LIB_H
#define LIB_H

#define MAX 4
#define MIN 2
typedef enum Boolean
{
    FALSE = 0,
    TRUE = 1
} Boolean;

typedef struct book_node_tag
{
    int book_id;
    char Title[20];
    char Author[20];
    char subject[20];
    int number_of_copies_issued;
    int number_of_copies_available;
    int No_of_request;
    int cnt; // used for display who get book
} book_node;

book_node *get_new_book();

typedef struct request_queue
{
    char Name_of_student[20];
    int book_id;
    char title_of_book[20];
    int date;
    struct request_queue *next;
} request_queue;

request_queue *get_reqNode();

typedef struct borrower_list
{
    char Name_of_student[20];
    int cnt;
    int book_id[3];
    char title_of_book[3][20];
    int issued_date[3];
    int return_date[3];
    struct borrower_list *next;
} borrower_list;

borrower_list *get_borrNode();

typedef struct book_tree_Node
{
    int count;
    book_node *key[MAX + 1];
    struct book_tree_Node *branch[MAX + 1];
} Node;

Node *get_treeNode();
Node *Search(Node *root, int target, int *k);
Boolean Search_Node(Node *p, int target, int *k);
Node *insert_book(Node *root, book_node *newbook);
//------------------------------------------------
Boolean PushDown(Node *root, book_node *newbook, book_node **x, Node **xr);

void PushIn(book_node *x, Node *xr, Node *p, int k);
void Split(book_node *x, Node *xr, Node *root, int k, book_node **y, Node **yr);
void inorder(Node *root);
void display_book(book_node *node);
Node *Delete(Node *root, int book_id);
Boolean RecDelete(Node *root, int book_id);
void Successor(Node *p, int k);
void Remove(Node *p, int k);
void Restore(Node *p, int k);
void MoveLeft(Node *p, int k);
void MoveRight(Node *p, int k);
void Combine(Node *p, int k);

Node *Delete(Node *root, int book_id);
Boolean RecDelete(Node *root, int book_id);

void Successor(Node *p, int k); // replace p->key[k] with its successor
void Remove(Node *p, int k);    // delete p->key[k];
void Restore(Node *p, int k);   // restore devoid node in tree i.e (count <min)

// move a key to left
void MoveLeft(Node *p, int k);
// move a key to right
void MoveRight(Node *p, int k);

void Combine(Node *p, int k); // merger parent key "k" with there children or combine sibling

//------------------------

typedef struct Question_5_tag
{

    char Title[20];
    char Author[20];
    int bookAvail;
    struct Question_5_tag *next;
} Question5;

int insert_borrList(request_queue *node, int currDate);
void disply_borrow(borrower_list *node);
void display_borrow_list();
// merge sort for borroow
borrower_list *findMidBorrow(borrower_list *lptr);
borrower_list *MergeBorrow(borrower_list *lptr, borrower_list *nptr);

borrower_list *MergeSortBorrow(borrower_list *lptr);
//----------------------------------------------borrower_list

void Sort_display_borrow();
//----------------------------------------------borrower_list

void insert_To_requestQ_1(request_queue *node);
void insert_To_requestQ_2(request_queue *node);

void put_request();

void compile_request();

void display_req();

int check_borrow_list(request_queue *node);
void display_who_getBook();

void Range_search(Node *root, int From_book_id, int To_book_id);

void Display_req_avail(Node *root);

void Display_No_issued(Node *root);

void Find_most_demandBook(Node *root, book_node **max_demand, int *max);

//--------------------------------------------------------------------------------------------/
Question5 *findMid(Question5 *lptr);

Question5 *Merge(Question5 *lptr, Question5 *nptr);

Question5 *MergeSort(Question5 *lptr);

void Traverse_Q5(Node *root, Question5 **head, char sub[]);
void display_book_Based_NoIssued();

void display_who_get_3days(int currDate);

#endif
