//BT20CSE034 Dhanesh Choudhary
//DSPD2 HW Assignment 2

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Library_mange.h"

book_node *get_new_book()
{
    book_node *temp = (book_node *)malloc(sizeof(book_node));

    printf("Enter book_id: ");
    scanf("%d", &temp->book_id);
    printf("Enter book Title: ");
    scanf("%s", temp->Title);
    printf("Enter book Subject: ");
    scanf("%s", temp->subject);
    printf("Enter book Author: ");
    scanf("%s", temp->Author);
    printf("Enter Number of copies: ");
    scanf("%d", &temp->number_of_copies_available);

    temp->number_of_copies_issued = 0;
    temp->No_of_request = 0;
    temp->cnt = 0;

    return temp;
}

Node *get_treeNode()
{
    Node *temp_node = (Node *)malloc(sizeof(Node));
    for (int i = 0; i <= MAX; i++)
        temp_node->branch[i] = NULL;
    temp_node->count = 0;
    return temp_node;
}

Node *Search(Node *root, int target, int *k)
{
    if (root == NULL)
        return NULL;
    else if (Search_Node(root, target, k))
        return root;
    else
        return Search(root->branch[*k], target, k);
}

Boolean Search_Node(Node *p, int target, int *k)
{
    if (p == NULL)
        return FALSE;

    if (target < p->key[1]->book_id)
    {
        (*k) = 0;
        return FALSE;
    }
    else
    {
        (*k) = p->count;
        while ((target < p->key[*k]->book_id) && ((*k) > 1))
            (*k)--;

        if (target == (p->key[*k]->book_id))
            return TRUE;
        else
            return FALSE;
    }
}

Node *insert_book(Node *root, book_node *newbook)
{
    book_node *x;
    Node *xr;
    Boolean pushUp;

    pushUp = PushDown(root, newbook, &x, &xr);

    if (pushUp == TRUE)
    {
        Node *temp = get_treeNode();
        temp->count = 1;
        temp->key[1] = x;
        temp->branch[0] = root;
        temp->branch[1] = xr;
        return temp;
    }

    return root;
}

Boolean PushDown(Node *root, book_node *newbook, book_node **x, Node **xr)
{
    int k;
    if (root == NULL)
    {
        *x = newbook;
        *xr = NULL;
        return TRUE;
    }
    else
    {
        if (Search_Node(root, newbook->book_id, &k))
        {
            printf("Inserting duplicate value...\n");
        }
        else if (PushDown(root->branch[k], newbook, x, xr))
        {
            if (root->count < MAX)
            {
                PushIn(*x, *xr, root, k);
                return FALSE;
            }
            else
            {
                Split(*x, *xr, root, k, x, xr);
                return TRUE;
            }
        }
        return FALSE;
    }
}

void PushIn(book_node *x, Node *xr, Node *p, int k)
{

    for (int i = p->count; i > k; i--)
    {
        p->key[i + 1] = p->key[i];
        p->branch[i + 1] = p->branch[i];
    }
    p->key[k + 1] = x;
    p->branch[k + 1] = xr;
    (p->count)++;
}

void Split(book_node *x, Node *xr, Node *root, int k, book_node **y, Node **yr)
{
    int median;

    if (k <= MIN)
        median = MIN;
    else
        median = MIN + 1;

    Node *temp = get_treeNode();

    for (int i = median + 1; i <= MAX; i++)
    {
        temp->key[i - median] = root->key[i];
        temp->branch[i - median] = root->branch[i];
    }

    temp->count = MAX - median;
    root->count = median;

    if (k <= MIN)
        PushIn(x, xr, root, k);
    else
        PushIn(x, xr, temp, k - median);

    *y = root->key[root->count];
    temp->branch[0] = root->branch[root->count];
    (root->count)--;

    *yr = temp;
}

void inorder(Node *root)
{
    if (root != NULL)
    {
        for (int i = 1; i <= root->count; i++)
        {
            inorder(root->branch[i - 1]);
            display_book(root->key[i]);
        }

        inorder(root->branch[root->count]);
    }
}
void display_book(book_node *node)
{
    printf("\n--------------------\n");
    printf("Book ID : %d\n", node->book_id);
    printf("Book Title : %s\n", node->Title);
    printf("Book Author : %s\n", node->Author);
    printf("Book Subject : %s\n", node->subject);
    printf("No of copy issued : %d\n", node->number_of_copies_issued);
    printf("No of copy avail : %d\n", node->number_of_copies_available);
    printf("No of Requests : %d\n", node->No_of_request);
    printf("\n--------------------\n");
}

Node *Delete(Node *root, int book_id)
{
    if (!RecDelete(root, book_id))
        printf("Error: element not found!!\n");
    else if (root->count == 0)
    {
        Node *p = root;
        root = root->branch[0];
        free(p);
    }
    return root;
}

Boolean RecDelete(Node *root, int book_id)
{
    int k;
    Boolean found;

    if (root == NULL)
        return FALSE;
    else
    {

        if ((found = Search_Node(root, book_id, &k)))
        {
            if (root->branch[0] != NULL)
            {
                Successor(root, k);

                if (!(found = RecDelete(root->branch[k], root->key[k]->book_id)))
                    printf("Error: key not found (succesor)\n");
            }
            else
                Remove(root, k);
        }
        else
            found = RecDelete(root->branch[k], book_id);
        if (root->branch[k] != NULL)
        {
            if (root->branch[k]->count < MIN)
                Restore(root, k);
        }

        return found;
    }
}

void Remove(Node *p, int k)
{
    book_node *temp = p->key[k];

    for (int i = k + 1; i <= p->count; i++)
    {
        p->key[i - 1] = p->key[i];
        p->branch[i - 1] = p->branch[i];
    }
    (p->count)--;

    free(temp);
    temp = NULL;
}

void Successor(Node *p, int k)
{
    Node *q;
    for (q = p->branch[k]; q->branch[0] != NULL; q = q->branch[0])
        ;
    p->key[k]->book_id = q->key[1]->book_id;
    strcpy(p->key[k]->Title, q->key[1]->Title);
    strcpy(p->key[k]->Author, q->key[1]->Author);
    strcpy(p->key[k]->subject, q->key[1]->subject);
    p->key[k]->number_of_copies_issued = q->key[1]->number_of_copies_issued;
    p->key[k]->number_of_copies_issued = q->key[1]->number_of_copies_issued;
}

void Restore(Node *p, int k)
{

    if (k == 0)
    {
        if (p->branch[1]->count > MIN)
            MoveLeft(p, 1);
        else
            Combine(p, 1);
    }
    else if (k == p->count)
    {
        if (p->branch[k - 1]->count > MIN)
            MoveRight(p, k);
        else
            Combine(p, k);
    }
    else if (p->branch[k - 1]->count > MIN)
        MoveRight(p, k);
    else if (p->branch[k + 1]->count > MIN)
        MoveLeft(p, k + 1);
    else
        Combine(p, k);
}

void MoveRight(Node *p, int k)
{
    Node *t = p->branch[k];

    for (int i = t->count; i > 0; i--)
    {
        t->key[i + 1] = t->key[i];
        t->branch[i + 1] = t->branch[i];
    }
    t->branch[1] = t->branch[0];
    (t->count)++;

    t->key[1] = p->key[k];

    t = p->branch[k - 1];
    p->key[k] = t->key[t->count];
    p->branch[k]->branch[0] = t->branch[t->count];
    (t->count)--;
}

void MoveLeft(Node *p, int k)
{
    Node *t = p->branch[k - 1];
    (t->count)++;
    t->key[t->count] = p->key[k];
    t->branch[t->count] = p->branch[k]->branch[0];

    t = p->branch[k];
    p->key[k] = t->key[1];
    t->branch[0] = t->branch[1];
    (t->count)--;
    for (int i = 1; i <= t->count; i++)
    {
        t->key[i] = t->key[i + 1];
        t->branch[i] = t->branch[i + 1];
    }
}

void Combine(Node *p, int k)
{
    Node *q = p->branch[k];
    Node *l = p->branch[k - 1];

    (l->count)++;
    l->key[l->count] = p->key[k];
    l->branch[l->count] = q->branch[0];

    for (int i = 1; i <= q->count; i++)
    {
        (l->count)++;
        l->key[l->count] = q->key[i];
        l->branch[l->count] = q->branch[i];
    }

    for (int i = k; i < p->count; i++)
    {
        p->key[i] = p->key[i + 1];
        p->branch[i] = p->branch[i + 1];
    }
    (p->count)--;

    free(q);
}

/*

int main()
{
    Node *root = NULL;

    root = insert_book(root, get_new_book());
    root = insert_book(root, get_new_book());
    root = insert_book(root, get_new_book());
    root = insert_book(root, get_new_book());
    root = insert_book(root, get_new_book());
    root = insert_book(root, get_new_book());
    // root = insert_book(root, get_new_book());
    // root = insert_book(root, get_new_book());
    inorder(root);

    int x;
    printf("Enter x: ");
    scanf("%d", &x);
    root = Delete(root, x);

    inorder(root);

    return 0;
}
*/
