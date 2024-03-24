//BT20CSE034 Dhanesh Choudhary
//DSPD2 HW Assignment 2

/*
    Assumption: date are taken as integer value i,e date will be 1,2,3 only .and date2>date1 if
    date2 value(say 4) is greater date1(say 2) values.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Library_mange.h"

Node *book_root = NULL;
request_queue *req_ptr1 = NULL; // higher priority
request_queue *req_ptr2 = NULL; // lower priority
borrower_list *brr_ptr = NULL;

request_queue *get_reqNode()
{
    request_queue *temp = (request_queue *)malloc(sizeof(request_queue));
    temp->next = NULL;

    printf("Enter student Name: ");
    scanf("%s", temp->Name_of_student);
    fflush(stdin);
    printf("Enter Book Id want to request : ");
    scanf("%d", &temp->book_id);
    fflush(stdin);
    printf("Enter Book Title want to request : ");
    scanf("%s", temp->title_of_book);
    fflush(stdin);
    printf("Enter Req date (integer ex: 2 or 4): ");
    scanf("%d", &temp->date);
    return temp;
}

borrower_list *get_borrNode(char name[])
{
    borrower_list *temp = (borrower_list *)malloc(sizeof(borrower_list));
    strcpy(temp->Name_of_student, name);
    temp->next = NULL;
    temp->cnt = 0;
    return temp;
}

int insert_borrList(request_queue *node, int currDate)
{
    int ret_val = 1;
    borrower_list *temp = brr_ptr;
    while (temp != NULL)
    {
        if (strcmp(temp->Name_of_student, node->Name_of_student) == 0)
            break;
        else
            temp = temp->next;
    }

    if (temp == NULL)
    {
        borrower_list *new_node = get_borrNode(node->Name_of_student);

        new_node->book_id[0] = node->book_id;
        new_node->issued_date[0] = currDate;
        new_node->return_date[0] = currDate + 15;
        strcpy(new_node->title_of_book[0], node->title_of_book);
        new_node->cnt++;

        new_node->next = brr_ptr;
        brr_ptr = new_node;
    }
    else
    {
        if (temp->cnt < 3)
        {
            temp->book_id[temp->cnt] = node->book_id;
            temp->issued_date[temp->cnt] = currDate;
            temp->return_date[temp->cnt] = currDate + 15;
            strcpy(temp->title_of_book[temp->cnt], node->title_of_book);
            temp->cnt++;
        }
        else
        {
            ret_val = 0;
            printf("%s already borrow 3 books\n", node->Name_of_student);
        }
    }

    return ret_val;
}

void disply_borrow(borrower_list *node)
{
    printf("--------------------------------------\n");
    printf("Name: %s\t", node->Name_of_student);
    printf("Number of issued book: %d\n", node->cnt);
    printf("--------------------------------------\n");
    for (int i = 0; i < node->cnt; i++)
    {
        printf("Book id: %d\t Book Title: %s\n", node->book_id[i], node->title_of_book[i]);
        printf("Issued Date: %d \t Return date: %d\n", node->issued_date[i], node->return_date[i]);
    }
}

void display_borrow_list()
{
    borrower_list *temp = brr_ptr;
    while (temp != NULL)
    {
        disply_borrow(temp);
        temp = temp->next;
    }
}
//--------------------------------------------------------------------------------------------/
borrower_list *findMidBorrow(borrower_list *lptr)
{
    borrower_list *nptr = NULL;
    if (lptr != NULL && lptr->next != NULL)
    {
        borrower_list *slow = lptr;
        borrower_list *fast = lptr->next;
        while (fast != NULL && fast->next != NULL)
        {
            slow = slow->next;
            fast = fast->next->next;
        }
        nptr = slow->next;
        slow->next = NULL;
    }
    return nptr;
}

borrower_list *MergeBorrow(borrower_list *lptr, borrower_list *nptr)
{
    borrower_list *result, *ptr1, *ptr2, *tail;
    ptr1 = lptr;
    ptr2 = nptr;
    if (ptr1->cnt > ptr2->cnt)
    {
        result = ptr1;
        ptr1 = ptr1->next;
    }
    else
    {
        result = ptr2;
        ptr2 = ptr2->next;
    }
    tail = result;

    while (ptr1 != NULL && ptr2 != NULL)
    {

        if (ptr1->cnt >= ptr2->cnt)
        {
            tail->next = ptr1;
            tail = ptr1;
            ptr1 = ptr1->next;
        }
        else
        {
            tail->next = ptr2;
            tail = ptr2;
            ptr2 = ptr2->next;
        }
    }
    if (ptr1 == NULL)
    {
        tail->next = ptr2;
    }
    else
    {
        tail->next = ptr1;
    }
    return result;
}

borrower_list *MergeSortBorrow(borrower_list *lptr)
{
    borrower_list *nptr;
    if (lptr != NULL && lptr->next != NULL)
    {
        nptr = findMidBorrow(lptr);
        lptr = MergeSortBorrow(lptr);
        nptr = MergeSortBorrow(nptr);
        lptr = MergeBorrow(lptr, nptr);
    }
    return lptr;
}
//----------------------------------------------borrower_list

void Sort_display_borrow()
{
    brr_ptr = MergeSortBorrow(brr_ptr);

    borrower_list *temp = brr_ptr;
    while (temp != NULL)
    {
        disply_borrow(temp);
        temp = temp->next;
    }
}
//----------------------------------------------borrower_list

void insert_To_requestQ_1(request_queue *node)
{
    if (req_ptr1 == NULL)
    {
        req_ptr1 = node;
        return;
    }

    request_queue *temp = req_ptr1;
    while (temp->next != NULL)
    {
        temp = temp->next;
    }
    temp->next = node;
}
void insert_To_requestQ_2(request_queue *node)
{
    if (req_ptr2 == NULL)
    {
        req_ptr2 = node;
        return;
    }

    request_queue *temp = req_ptr2;
    while (temp->next != NULL)
    {
        temp = temp->next;
    }
    temp->next = node;
}

void put_request(request_queue *temp)
{
    // request_queue *new_node = get_reqNode();
    request_queue *new_node = temp;

    int k;
    Node *flag_node = Search(book_root, new_node->book_id, &k);

    if (flag_node != NULL)
    {
        flag_node->key[k]->No_of_request++;

        int flag = 0;

        borrower_list *temp = brr_ptr;
        while (temp != NULL && flag == 0)
        {
            // printf("hello\n");
            if (strcmp(temp->Name_of_student, new_node->Name_of_student) == 0)
                flag = 1;

            temp = temp->next;
        }

        if (flag == 1)
            insert_To_requestQ_2(new_node);
        else
            insert_To_requestQ_1(new_node);

        printf("Requested have been put to list!!\n");
    }
    else
    {
        printf("No such book available!!\n");
    }
}

void compile_request()
{
    int currDate;
    printf("Enter todays date(int):");
    scanf("%d", &currDate);

    request_queue *temp = req_ptr1;

    request_queue *prev = NULL;

    while (temp != NULL)
    {
        int k;
        Node *flag = Search(book_root, temp->book_id, &k);

        if (flag->key[k]->number_of_copies_available != 0)
        {
            int flg = insert_borrList(temp, currDate);
            if (flg == 1)
            {
                request_queue *tmp;
                if (prev == NULL)
                {
                    tmp = req_ptr1;
                    temp = req_ptr1 = req_ptr1->next;
                    free(tmp);
                }
                else
                {
                    tmp = temp;
                    temp = temp->next;
                    prev->next = tmp->next;
                }

                flag->key[k]->number_of_copies_available--;
                flag->key[k]->number_of_copies_issued++;
                flag->key[k]->No_of_request--;
            }
            else
            {
                prev = temp;
                temp = temp->next;
            }
        }
        else
        {
            prev = temp;
            temp = temp->next;
        }
    }

    temp = req_ptr2;
    prev = NULL;

    while (temp != NULL)
    {
        int k;
        Node *flag = Search(book_root, temp->book_id, &k);

        if (flag->key[k]->number_of_copies_available != 0)
        {
            int flg = insert_borrList(temp, currDate);
            if (flg == 1)
            {
                request_queue *tmp;
                if (prev == NULL)
                {
                    tmp = req_ptr2;
                    temp = req_ptr2 = req_ptr2->next;
                    free(tmp);
                }
                else
                {
                    tmp = temp;
                    temp = temp->next;
                    prev->next = tmp->next;
                }

                flag->key[k]->number_of_copies_available--;
                flag->key[k]->number_of_copies_issued++;
                flag->key[k]->No_of_request--;
            }
            else
            {
                prev = temp;
                temp = temp->next;
            }
        }
        else
        {
            prev = temp;
            temp = temp->next;
        }
    }

    printf("request list is compiled successfully\n");
}

void display_req()
{
    request_queue *temp = req_ptr1;
    printf("Request list:\n");
    while (temp != NULL)
    {
        printf("Name: %s\n", temp->Name_of_student);
        printf("Book id: %d   Title of book: %s\n", temp->book_id, temp->title_of_book);
        printf("Request date: %d\n", temp->date);
        printf("-------------------------------\n");
        temp = temp->next;
    }

    temp = req_ptr2;
    while (temp != NULL)
    {
        printf("Name: %sn\n", temp->Name_of_student);
        printf("Book id: %d   Title of book: %s\n", temp->book_id, temp->title_of_book);
        printf("Request date: %d\n", temp->date);
        printf("-------------------------------\n");
        temp = temp->next;
    }
}

int check_borrow_list(request_queue *node)
{
    borrower_list *temp = brr_ptr;
    while (temp != NULL)
    {
        if (strcmp(temp->Name_of_student, node->Name_of_student) == 0)
            break;
        else
            temp = temp->next;
    }

    if (temp == NULL)
        return 0;
    else
    {
        if (temp->cnt == 3)
            return 1;
        else
            return 0;
    }
}
void display_who_getBook()
{
    request_queue *temp;

    temp = req_ptr1;

    while (temp != NULL)
    {
        int k;
        Node *flag_node = Search(book_root, temp->book_id, &k);
        flag_node->key[k]->cnt = 0;
        temp = temp->next;
    }

    temp = req_ptr2;
    while (temp != NULL)
    {
        int k;
        Node *flag_node = Search(book_root, temp->book_id, &k);
        flag_node->key[k]->cnt = 0;
        temp = temp->next;
    }
    //------------------------------------------------//
    temp = req_ptr1;
    while (temp != NULL)
    {
        int k;
        Node *flag_node = Search(book_root, temp->book_id, &k);

        int flag2 = check_borrow_list(temp);

        if (flag2 == 1)
        {
            printf("%s cannot have book(already 3 borrowed)\n", temp->Name_of_student);
        }
        else if (flag_node->key[k]->cnt < flag_node->key[k]->number_of_copies_available)
        {
            printf("%s will get book \n", temp->Name_of_student);
            (flag_node->key[k]->cnt)++;
        }
        else
        {
            printf("%s cannot have book(out of stock)\n", temp->Name_of_student);
        }
        temp = temp->next;
    }

    temp = req_ptr2;
    while (temp != NULL)
    {
        int k;
        Node *flag_node = Search(book_root, temp->book_id, &k);

        int flag2 = check_borrow_list(temp);

        if (flag2 == 1)
        {
            printf("%s cannot have book(already 3 borrowed)\n", temp->Name_of_student);
        }
        else if (flag_node->key[k]->cnt < flag_node->key[k]->number_of_copies_available)
        {
            printf("%s will get book \n", temp->Name_of_student);
            (flag_node->key[k]->cnt)++;
        }
        else
        {
            printf("%s cannot have book(out of stock)\n", temp->Name_of_student);
        }
        temp = temp->next;
    }
}

void Range_search(Node *root, int From_book_id, int To_book_id)
{
    if (root != NULL)
    {
        for (int i = 1; i <= root->count; i++)
        {

            if (root->key[i]->book_id > From_book_id)
            {
                Range_search(root->branch[i - 1], From_book_id, To_book_id);
            }

            if ((root->key[i]->book_id >= From_book_id) && (root->key[i]->book_id <= To_book_id))
            {
                printf("Book id : %d\tBook Name: %s \n", root->key[i]->book_id, root->key[i]->Title);
            }

            if (root->key[i]->book_id > To_book_id)
                break;
        }

        if (root->key[root->count]->book_id < To_book_id)
            Range_search(root->branch[root->count], From_book_id, To_book_id);
    }
}

void Display_req_avail(Node *root)
{
    if (root != NULL)
    {
        for (int i = 1; i <= root->count; i++)
        {
            Display_req_avail(root->branch[i - 1]);
            if (root->key[i]->No_of_request > 0 && root->key[i]->number_of_copies_available != 0)
            {
                printf("Book id : %d\tBook Name: %s ", root->key[i]->book_id, root->key[i]->Title);
                printf("\tNo of copies available: %d\n", root->key[i]->number_of_copies_available);
            }
        }
        Display_req_avail(root->branch[root->count]);
    }
}

void Display_No_issued(Node *root)
{
    if (root != NULL)
    {
        for (int i = 1; i <= root->count; i++)
        {
            Display_No_issued(root->branch[i - 1]);
            if (root->key[i]->number_of_copies_issued == 0)
            {
                printf("Book id : %d\tBook Name: %s \n", root->key[i]->book_id, root->key[i]->Title);
            }
        }
        Display_No_issued(root->branch[root->count]);
    }
}

void Find_most_demandBook(Node *root, book_node **max_demand, int *max)
{
    if (root != NULL)
    {
        for (int i = 1; i <= root->count; i++)
        {
            Find_most_demandBook(root->branch[i - 1], max_demand, max);
            int cnt = root->key[i]->number_of_copies_issued + root->key[i]->No_of_request;
            if (cnt > *max)
            {
                *max = cnt;
                *max_demand = root->key[i];
            }
        }
        Find_most_demandBook(root->branch[root->count], max_demand, max);
    }
}

//--------------------------------------------------------------------------------------------/
Question5 *findMid(Question5 *lptr)
{
    Question5 *nptr = NULL;
    if (lptr != NULL && lptr->next != NULL)
    {
        Question5 *slow = lptr;
        Question5 *fast = lptr->next;
        while (fast != NULL && fast->next != NULL)
        {
            slow = slow->next;
            fast = fast->next->next;
        }
        nptr = slow->next;
        slow->next = NULL;
    }
    return nptr;
}

Question5 *Merge(Question5 *lptr, Question5 *nptr)
{
    Question5 *result, *ptr1, *ptr2, *tail;
    ptr1 = lptr;
    ptr2 = nptr;
    if (ptr1->bookAvail > ptr2->bookAvail)
    {
        result = ptr1;
        ptr1 = ptr1->next;
    }
    else
    {
        result = ptr2;
        ptr2 = ptr2->next;
    }
    tail = result;

    while (ptr1 != NULL && ptr2 != NULL)
    {

        if (ptr1->bookAvail >= ptr2->bookAvail)
        {
            tail->next = ptr1;
            tail = ptr1;
            ptr1 = ptr1->next;
        }
        else
        {
            tail->next = ptr2;
            tail = ptr2;
            ptr2 = ptr2->next;
        }
    }
    if (ptr1 == NULL)
    {
        tail->next = ptr2;
    }
    else
    {
        tail->next = ptr1;
    }
    return result;
}

Question5 *MergeSort(Question5 *lptr)
{
    Question5 *nptr;
    if (lptr != NULL && lptr->next != NULL)
    {
        nptr = findMid(lptr);
        lptr = MergeSort(lptr);
        nptr = MergeSort(nptr);
        lptr = Merge(lptr, nptr);
    }
    return lptr;
}

void Traverse_Q5(Node *root, Question5 **head, char sub[])
{
    if (root != NULL)
    {
        for (int i = 1; i <= root->count; i++)
        {
            Traverse_Q5(root->branch[i - 1], head, sub);

            if (strcmp(root->key[i]->subject, sub) == 0)
            {
                Question5 *temp = (Question5 *)malloc(sizeof(Question5));
                temp->next = NULL;

                strcpy(temp->Title, root->key[i]->Title);
                strcpy(temp->Author, root->key[i]->Author);
                temp->bookAvail = root->key[i]->number_of_copies_available;

                temp->next = *head;
                *head = temp;
            }
        }
        Traverse_Q5(root->branch[root->count], head, sub);
    }
}

void display_book_Based_NoIssued()
{
    Question5 *head = NULL;

    char sub[20];
    printf("Enter Subject: ");
    scanf("%s", sub);

    Traverse_Q5(book_root, &head, sub);

    head = MergeSort(head);

    Question5 *temp = head;
    while (temp != NULL)
    {
        printf("Title : %s ", temp->Title);
        printf("Author : %s ", temp->Author);
        printf("No of book Available : %d\n", temp->bookAvail);

        temp = temp->next;
    }

    temp = head;
    while (head != NULL)
    {
        head = head->next;
        free(temp);
        temp = head;
    }
}

void display_who_get_3days(int currDate)
{
    request_queue *temp;

    temp = req_ptr1;

    while (temp != NULL)
    {
        int k;
        Node *flag_node = Search(book_root, temp->book_id, &k);
        flag_node->key[k]->cnt = 0;
        temp = temp->next;
    }

    temp = req_ptr2;
    while (temp != NULL)
    {
        int k;
        Node *flag_node = Search(book_root, temp->book_id, &k);
        flag_node->key[k]->cnt = 0;
        temp = temp->next;
    }
    //------------------------------------------------//
    // check borrow list who will return in within 3 days

    borrower_list *temp_brr = brr_ptr;
    while (temp_brr != NULL)
    {
        for (int i = 0; i < temp_brr->cnt; i++)
        {
            if (temp_brr->return_date[i] <= (currDate + 2) && temp_brr->return_date[i] >= currDate)
            {
                int k;
                Node *flag_node = Search(book_root, temp_brr->book_id[i], &k);
                (flag_node->key[k]->cnt)--;
            }
        }

        temp_brr = temp_brr->next;
    }

    temp = req_ptr1;

    while (temp != NULL)
    {
        int k;
        Node *flag_node = Search(book_root, temp->book_id, &k);

        int flag2 = check_borrow_list(temp);

        if (flag2 == 1)
        {
            printf("%s cannot have book(already 3 borrowed)\n", temp->Name_of_student);
        }
        else if (flag_node->key[k]->cnt < flag_node->key[k]->number_of_copies_available)
        {
            printf("%s will get book \n", temp->Name_of_student);
            (flag_node->key[k]->cnt)++;
        }
        else
        {
            printf("%s cannot have book(out of stock)\n", temp->Name_of_student);
        }
        temp = temp->next;
    }

    temp = req_ptr2;
    while (temp != NULL)
    {
        int k;
        Node *flag_node = Search(book_root, temp->book_id, &k);

        int flag2 = check_borrow_list(temp);

        if (flag2 == 1)
        {
            printf("%s cannot have book(already 3 borrowed)\n", temp->Name_of_student);
        }
        else if (flag_node->key[k]->cnt < flag_node->key[k]->number_of_copies_available)
        {
            printf("%s will get book \n", temp->Name_of_student);
            (flag_node->key[k]->cnt)++;
        }
        else
        {
            printf("%s cannot have book(out of stock)\n", temp->Name_of_student);
        }
        temp = temp->next;
    }
}

int main()
{

    // This is main fxn... menu is given below  if u dont want to add data again and again manually
    //  u can uncomment this hard coded data to debug and to check corrrectness of code
    //  here data validation is not done such book id 1 if u write title as JDLEE it wont give error
    //  so upto user to valid data such date .title etc which are crucial for output.

    /*
    book_node *temp_book = (book_node *)malloc(sizeof(book_node));

    temp_book->book_id = 1;
    strcpy(temp_book->Author, "HCVERMA");
    strcpy(temp_book->Title, "hcverma");
    strcpy(temp_book->subject, "physics");
    temp_book->No_of_request = 0;
    temp_book->number_of_copies_issued = 0;
    temp_book->number_of_copies_available = 2;
    temp_book->cnt = 0;

    book_root = insert_book(book_root, temp_book);
    temp_book = (book_node *)malloc(sizeof(book_node));

    temp_book->book_id = 2;
    strcpy(temp_book->Author, "JDLEE");
    strcpy(temp_book->Title, "JD");
    strcpy(temp_book->subject, "chemistry");
    temp_book->No_of_request = 0;
    temp_book->number_of_copies_issued = 0;
    temp_book->number_of_copies_available = 2;
    temp_book->cnt = 0;

    book_root = insert_book(book_root, temp_book);
    temp_book = (book_node *)malloc(sizeof(book_node));

    temp_book->book_id = 3;
    strcpy(temp_book->Author, "RD_SHARMA");
    strcpy(temp_book->Title, "rd_sharma");
    strcpy(temp_book->subject, "math");
    temp_book->No_of_request = 0;
    temp_book->number_of_copies_issued = 0;
    temp_book->number_of_copies_available = 2;
    temp_book->cnt = 0;

    book_root = insert_book(book_root, temp_book);
    temp_book = (book_node *)malloc(sizeof(book_node));

    temp_book->book_id = 4;
    strcpy(temp_book->Author, "SYZ");
    strcpy(temp_book->Title, "bookname1");
    strcpy(temp_book->subject, "CPL");
    temp_book->No_of_request = 0;
    temp_book->number_of_copies_issued = 0;
    temp_book->number_of_copies_available = 1;
    temp_book->cnt = 0;

    book_root = insert_book(book_root, temp_book);
    temp_book = (book_node *)malloc(sizeof(book_node));

    temp_book->book_id = 5;
    strcpy(temp_book->Author, "XYZ1");
    strcpy(temp_book->Title, "bookname2");
    strcpy(temp_book->subject, "physics");
    temp_book->No_of_request = 0;
    temp_book->number_of_copies_issued = 0;
    temp_book->number_of_copies_available = 1;
    temp_book->cnt = 0;

    book_root = insert_book(book_root, temp_book);

    temp_book = (book_node *)malloc(sizeof(book_node));

    temp_book->book_id = 6;
    strcpy(temp_book->Author, "XYZ2");
    strcpy(temp_book->Title, "bookname3");
    strcpy(temp_book->subject, "physics");
    temp_book->No_of_request = 0;
    temp_book->number_of_copies_issued = 0;
    temp_book->number_of_copies_available = 2;
    temp_book->cnt = 0;
    book_root = insert_book(book_root, temp_book);



    request_queue *temp;

    //---------------------------------------------------------
    temp = (request_queue *)malloc(sizeof(request_queue));
    temp->next = NULL;
    temp->book_id = 1;
    strcpy(temp->Name_of_student, "dhanesh1");
    strcpy(temp->title_of_book, "hcverma");
    temp->date = 1;
    put_request(temp);
    //---------------------------------------------------------
    //---------------------------------------------------------
    temp = (request_queue *)malloc(sizeof(request_queue));
    temp->next = NULL;
    temp->book_id = 1;
    strcpy(temp->Name_of_student, "dhanesh2");
    strcpy(temp->title_of_book, "hcverma");
    temp->date = 1;
    put_request(temp);
    //---------------------------------------------------------
    //---------------------------------------------------------
    temp = (request_queue *)malloc(sizeof(request_queue));
    temp->next = NULL;
    temp->book_id = 6;
    strcpy(temp->Name_of_student, "dhanesh2");
    strcpy(temp->title_of_book, "bookname3");
    temp->date = 1;
    put_request(temp);
    //---------------------------------------------------------
    //---------------------------------------------------------
    temp = (request_queue *)malloc(sizeof(request_queue));
    temp->next = NULL;
    temp->book_id = 6;
    strcpy(temp->Name_of_student, "dhanesh4");
    strcpy(temp->title_of_book, "bookname3");
    temp->date = 2;
    put_request(temp);
*/
    int flag = 1;

    while (flag)
    {
        printf("\n-------------------------------------------------\n");
        printf("1.insert new book\n");
        printf("2.delete book\n");
        printf("3.display books\n");
        printf("4.Request a book\n");
        printf("5.display request queue\n");
        printf("6.Compile all request\n");
        printf("7.check who will get book\n");
        printf("8.display borrow list\n");
        printf("9.most demand book\n");
        printf("10.check who will get book in first 3 day\n");
        printf("11.sort borrow list (based on no of issued book)\n");
        printf("12.display book  of a particular subject on the basis of number_of_copies_available.\n");
        printf("13.Display the names of the requested books whose copies are available.\n");
        printf("14.Display the title of all the books which have not been issued by anyone.\n");
        printf("15.Range Search\n");
        printf("16.stop program\n");
        printf("\n-------------------------------------------------\n");
        int ch;
        printf("Enter choice:");
        scanf("%d", &ch);
        printf("\n-------------------------------------------------\n");

        switch (ch)
        {
        case 1:
        {
            book_node *new_book = get_new_book();
            book_root = insert_book(book_root, new_book);
        }
        break;
        case 2:
        {
            int id;
            printf("Enter book id: ");
            scanf("%d", &id);
            book_root = Delete(book_root, id);
        }

        break;
        case 3:
            inorder(book_root);
            break;
        case 4:
        {
            int n;
            printf("No of request to be insert? : ");
            scanf("%d", &n);
            for (int i = 0; i < n; i++)
            {
                request_queue *temp = get_reqNode();
                put_request(temp);
            }
        }

        break;
        case 5:
            display_req();
            break;
        case 6:
            compile_request();
            break;
        case 7:
            display_who_getBook();
            break;
        case 8:
            display_borrow_list();
            break;
        case 9:
        {
            book_node *mst_dmd = NULL;
            printf("---most demand book ----\n");
            int max = 0;
            Find_most_demandBook(book_root, &mst_dmd, &max);
            if (mst_dmd == NULL)
            {
                printf("No one!!\n");
            }
            else
                display_book(mst_dmd);
        }
        break;
        case 10:
        {
            int currDate;
            printf("Enter date for refernce(int): \n");
            scanf("%d", &currDate);
            display_who_get_3days(currDate);
        }
        break;
        case 11:
            Sort_display_borrow();
            break;
        case 12:
            display_book_Based_NoIssued();
            break;
        case 13:
            Display_req_avail(book_root);
            break;
        case 14:
            Display_No_issued(book_root);
            break;
        case 15:
        {
            int from_id;
            int to_id;
            printf("Enter From_id: ");
            scanf("%d", &from_id);
            printf("Enter TO_id: ");
            scanf("%d", &to_id);
            Range_search(book_root, from_id, to_id);
        }
        break;
        case 16:
            flag = 0;
            break;

        default:
            break;
        }
    }

    return 0;
}
