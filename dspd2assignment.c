#include <stdio.h>
#include <stdlib.h>
// #include <stddef.h>

#define SIZE 5
#define NAME_SIZE 20

typedef struct mem
{
    int mem_id;
    char Name[NAME_SIZE];
    int age;
    int ind_goal;
    int *stepcount;
    struct mem *next;

} individual;

typedef struct gr
{
    int gr_id;
    char gr_name[NAME_SIZE];
    individual *arr_mem[SIZE];
    int gr_goal;
    struct gr *next;
} group;

individual *Add_Person(int id, char *name, int age, int goal, int *weeksteps, individual *head)
{
    individual *nptr;
    scanf("%d", nptr->mem_id);
    fflush(stdin);
    scanf("%f", nptr->Name);
    scanf("%d", nptr->age);
    fflush(stdin);
    scanf("%d", nptr->ind_goal);

    for (int i = 0; i < SIZE; i++)
    {
        scanf("%d", nptr->stepcount[i]);
        fflush(stdin);
        
    }

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
        for (int i = 0; i < 6; i++)
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
