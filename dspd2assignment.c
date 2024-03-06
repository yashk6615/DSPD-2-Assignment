#include <stdio.h>

#define SIZE 5

typedef struct mem
{
    int mem_id;
    char *Name;
    int age;
    int ind_goal;
    int *stepcount;
    individual *next;

} individual;

typedef struct gr
{
    int gr_id;
    char *gr_name;
    individual *arr_mem[SIZE];
    int gr_goal;
    group *next;
} group;

individual *Add_Person(int id, char *name, int age, int goal, int *weeksteps, individual *head)
{
    individual *nptr;
    nptr->mem_id = id;
    nptr->Name = name;
    nptr->age = age;
    nptr->ind_goal = goal;
    nptr->stepcount = weeksteps;

    individual *curr, *prev;
    curr = head;
    prev = NULL;
    while (nptr->mem_id > curr->mem_id && curr != NULL)
    {
        prev = curr;
        curr = curr->next;
    }
    if (prev!=NULL)
    {
        prev->next = nptr;
        nptr->next = curr;
    }
    else
    {
        nptr->next=curr;
        head=nptr;
    }
    return head;
}

