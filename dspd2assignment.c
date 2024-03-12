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

group* Delete_group(int id, group* gptr)
{
    group *ptr, *prev;
    ptr = gptr;
    prev = NULL;
    while (ptr != NULL && ptr->gr_id !=id)
    {
        prev = ptr;
        ptr = ptr->next;   
    }
    if(ptr != NULL)
    {
        if (prev == NULL)
        {
            gptr = ptr->next;
        }
        else
        {
            prev->next = ptr->next;
        }
        free(ptr);
    }
    else
    {
        printf("group does not exist");
    }
    return gptr;
}

group* Merge_groups(int id1, int id2, group* gptr)
{
    group *ptr1, *ptr2;
    ptr1 = gptr;
    ptr2 = gptr;
    while (ptr1 != NULL && ptr2 != NULL && (ptr1->gr_id == id1 || ptr2->gr_id == id2))
    {
        if (ptr1->gr_id != id1)
        {
            ptr1 = ptr1->next;
        }
        if (ptr2->gr_id == id2)
        {
            ptr2 = ptr2->next;
        } 
    }
    int id = ptr1->gr_id;
    char *name = (char*)malloc(sizeof(ptr1->gr_name));
    strcpy(name,ptr1->gr_name);
    gptr = Create_group(id,name, ptr1,gptr);
    gptr = Delete_group(ptr1->gr_id, gptr);
    gptr = Delete_group(ptr2->gr_id,gptr);
    return gptr;
}

void get_top_3(individual* head)
{
    individual *arr[3];
    arr[0] = arr[1] = arr[2]->stepcount = 0;
}