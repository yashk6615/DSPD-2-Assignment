#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #include <stddef.h>

#define SIZE 5
#define NAME_SIZE 20

typedef struct mem
{
    int mem_id;
    char Name[NAME_SIZE];
    int age;
    int ind_goal;
    int stepcount[7];
    struct mem *next;

} individual;

typedef struct gr
{
    int gr_id;
    char gr_name[NAME_SIZE];
    int memberIDs[5];
    individual *arr_mem[SIZE];
    int gr_goal;
    struct gr *next;
} group;

typedef struct leaderboard
{
    char gr_name[SIZE];
    int gr_stepcount;
    struct leaderboard *next;
} leader;

individual *Add_Person(int ID, char *name, int age, int goal, int *weeksteps, individual *head)
{
    individual *nptr;
    individual *newIndividual = (individual *)malloc(sizeof(individual));
    newIndividual->mem_id = ID;
    strcpy(newIndividual->Name, name);
    newIndividual->age = age;
    newIndividual->ind_goal = goal;
    memcpy(newIndividual->stepcount, weeksteps, 7 * sizeof(int));
    newIndividual->next = NULL;

    individual *curr, *prev;
    curr = head;
    prev = NULL;
    while (nptr->mem_id > curr->mem_id && curr != NULL)
    {
        prev = curr;
        curr = curr->next;
    }
    if (prev != NULL)
    {
        prev->next = nptr;
        nptr->next = curr;
    }
    else
    {
        nptr->next = curr;
        head = nptr;
    }
    return head;
}

group *createGroup(int groupID, char *groupName, int weeklyGroupGoal, group *head)
{
    group *newGroup = (group *)malloc(sizeof(group));
    newGroup->gr_id = groupID;
    strcpy(newGroup->gr_name, groupName);
    newGroup->gr_goal = weeklyGroupGoal;
    memset(newGroup->arr_mem, 0, SIZE * sizeof(newGroup->arr_mem[0]));

    newGroup->next = NULL;

    if (head == NULL || head->gr_id >= newGroup->gr_id)
    {
        newGroup->next = head;
        head = newGroup;
    }
    else
    {
        group *current = head;
        while (current->next != NULL && current->next->gr_id < newGroup->gr_id)
        {
            current = current->next;
        }
        newGroup->next = current->next;
        current->next = newGroup;
    }
    return head;
}

int isIndividualInAnyGroup(group *groupList, individual *individual)
{
    group *currentGroup = groupList;
    int flag = 0;
    while (currentGroup != NULL)
    {
        for (int i = 0; i < 5; ++i)
        {
            if (currentGroup->arr_mem[i] == individual && !flag)
            {
                flag = 1;
            }
        }
        currentGroup = currentGroup->next;
    }
    return flag;
}

void addIndividualToGroup(group *group1, individual *individual1, group *groupList)
{
    if (group1 == NULL || individual1 == NULL)
    {
        printf("Group or individual does not exist.\n");
        return;
    }

    if (isIndividualInAnyGroup(groupList, individual1))
    {
        printf("Individual is already in another group.\n");
        return;
    }

    for (int i = 0; i < 5; ++i)
    {
        if (group1->arr_mem[i] == NULL)
        {
            group1->arr_mem[i] = individual1;
            return;
        }
    }
    printf("Group is full.\n");
}

individual *Delete_individual(int id, individual *head)
{
    individual *curr, *prev;
    curr = head;
    prev = NULL;
    while (id != curr->mem_id)
    {
        prev = curr;
        curr = curr->next;
    }
    if (prev == NULL)
    {
        head = curr->next;
        free(curr);
        curr = NULL;
    }
    else
    {
        prev->next = curr->next;
        free(curr);
        curr = NULL;
    }
    return head;
}

void Check_group_achievement(int group_id, group *head)
{
    group *curr = head;
    int flag = 0;
    while (group_id != curr->gr_id && curr != NULL)
    {
        curr = curr->next;
        if (group_id == curr->gr_id)
        {
            flag = 1;
        }
    }
    if (flag)
    {
        int sum = 0;
        for (int i = 0; i < SIZE; i++)
        {
            if ((curr->arr_mem[i] != NULL))
            {
                sum = sum + curr->arr_mem[i]->stepcount;
            }
        }
        if (sum >= curr->gr_goal)
        {
            printf("Group id:%d \n The group with above group id have achieved the group goal for this week\n Cumulative step count of group:%d \n Group goal:%d", curr->gr_id, sum, curr->gr_goal);
        }
        else
        {
            printf("Group id:%d \n The group with above group id have not achieved the group goal for this week\n Cumulative step count of group:%d \n Group goal:%d \n The group is short of %d no. of steps in the current week", curr->gr_id, sum, curr->gr_goal, curr->gr_goal - sum);
        }
    }
    else
    {
        printf("The group with group id:%d not found in the list", group_id);
    }
}

void Check_individual_rewards(int id, individual *leaderboard)
{
    individual *curr = leaderboard;
    int pos = 1;
    int flag = 0;
    for (pos; curr != NULL; pos++)
    {
        if (id == curr->mem_id)
        {
            flag = 1;
            break;
        }
        curr = curr->next;
    }
    if (flag == 0)
    {
        printf("The person with person id:%d not found in the list", id);
    }

    else
    {
        switch (pos)
        {
        case 1:
            printf("The person with person id:%d is on top of the leaderboard\n", id);
            break;

        case 2:
            printf("The person with person id:%d is on top 2 of the leaderboard\n", id);
            break;
        case 3:
            printf("The person with person id:%d is on top 3 of the leaderboard\n", id);
            break;
        default:
            printf("The person with person id:%d is not in the top 3 of the leaderboard\n", id);
            break;
        }
    }
}

void Suggest_goal_update(individual *leaderboard)
{
    individual *curr = leaderboard;
    int pos = 1;
    int suggest = 0;
    while (pos < 3 && curr != NULL)
    {
        curr = curr->next;
    }
    if (pos <= 3 && curr != NULL)
    {
        for (int i = 0; i < 7; i++)
        {
            suggest += curr->stepcount[i];
        }
        printf("To be in top 3 of the leaderboard, one should have a goal of  %d stepcount or more per week ", suggest);
    }
    else
    {
        printf("Leaderboard not available now \n One can set their goal on their own at their pace");
    }
}

void Generate_leader_board(group *gptr)
{
    leader *lptr, *prev = NULL;
    group *curr = gptr;
    int flag = 1;
    while (gptr != NULL)
    {
        leader *nptr;
        strcpy(nptr->gr_name, curr->gr_name);
        int sum = 0;
        for (int i = 0; i < 5; i++)
        {
            for (int j = 0; j < 7; j++)
            {
                sum += curr->arr_mem[i]->stepcount[j];
            }
        }
        prev->next = nptr;
        nptr->next = NULL;
        if (flag)
        {
            lptr = nptr;
            flag = 0;
        }
    }
    lptr = MergeSort(lptr);
    leader *present = lptr;
    int pos = 1;
    printf("Position \t Group Name \t Stepcount \n");
    while (pos <= 3 && present != NULL)
    {
        printf("%d \t %s \t %d \n", pos, present->gr_name, present->gr_stepcount);
        pos++;
        present = present->next;
    }
}

void Display_group_info(group *gptr, leader *lptr)
{
    printf("Group name :- %s\n", gptr->gr_name);
    printf("Group id :- %d\n", gptr->gr_id);
    printf("Group weekly goal :-%d \n", gptr->gr_goal);
    printf("Group members :-\n");
    printf("ID \t Name \t\n");
    for (int i = 0; i < 5; i++)
    {
        printf("%d \t %s\n", gptr->memberIDs[i], gptr->arr_mem[i]->Name);
    }
}

group *Delete_group(int id, group *gptr)
{
    group *ptr, *prev;
    ptr = gptr;
    prev = NULL;
    while (ptr != NULL && ptr->gr_id != id)
    {
        prev = ptr;
        ptr = ptr->next;
    }
    if (ptr != NULL)
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