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
    while (newIndividual->mem_id > curr->mem_id && curr != NULL)
    {
        prev = curr;
        curr = curr->next;
    }
    if (prev != NULL)
    {
        prev->next = newIndividual;
        newIndividual->next = curr;
    }
    else
    {
        newIndividual->next = curr;
        head = newIndividual;
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
            for (int j = 0; j < 7 && (curr->arr_mem[i] != NULL); j++)
            {
                sum = sum + curr->arr_mem[i]->stepcount[j];
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

// Function to merge two sorted linked lists
leader *merge(leader *left, leader *right)
{
    if (left == NULL)
        return right;
    if (right == NULL)
        return left;

    leader *result = NULL;

    if (left->gr_stepcount >= right->gr_stepcount)
    {
        result = left;
        result->next = merge(left->next, right);
    }
    else
    {
        result = right;
        result->next = merge(left, right->next);
    }

    return result;
}

// Function to split the linked list into two halves
void split(leader *source, leader **frontRef, leader **backRef)
{
    leader *slow;
    leader *fast;
    slow = source;
    fast = source->next;

    while (fast != NULL)
    {
        fast = fast->next;
        if (fast != NULL)
        {
            slow = slow->next;
            fast = fast->next;
        }
    }

    *frontRef = source;
    *backRef = slow->next;
    slow->next = NULL;
}

// Merge sort function for sorting the leaderboard
leader *mergeSort(leader **headRef)
{
    leader *head = *headRef;
    leader *a;
    leader *b;

    if (head == NULL || head->next == NULL)
    {
        return NULL;
    }

    split(head, &a, &b);

    mergeSort(&a);
    mergeSort(&b);

    *headRef = merge(a, b);
    return *headRef;
}

void get_top_3(individual *individual_list)
{
    individual *current = individual_list;
    int top3[3] = {0, 0, 0};
    int top3_individuals[3] = {0, 0, 0};
    int count = 0;
    while (current != NULL)
    {
        int total_steps = 0;
        for (int i = 0; i < 7; i++)
        {
            total_steps += current->stepcount[i];
        }
        if (total_steps >= current->ind_goal)
        {
            if (count < 3)
            {
                top3[count] = total_steps;
                top3_individuals[count] = current->mem_id;
                count++;
            }
            else
            {
                if (top3[0] < total_steps)
                {
                    top3[2] = top3[1];
                    top3_individuals[2] = top3_individuals[1];
                    top3[1] = top3[0];
                    top3_individuals[1] = top3_individuals[0];
                    top3[0] = total_steps;
                    top3_individuals[0] = current->mem_id;
                }
                else if (top3[1] < total_steps)
                {
                    top3[2] = top3[1];
                    top3_individuals[2] = top3_individuals[1];
                    top3[1] = total_steps;
                    top3_individuals[1] = current->mem_id;
                }
                else if (top3[2] < total_steps)
                {
                    top3[2] = total_steps;
                    top3_individuals[2] = current->mem_id;
                }
            }
        }
        current = current->next;
    }
    printf("Top 3 individuals:\n");
    for (int i = 0; i < 3; i++)
    {
        if (top3_individuals[i] != 0)
        {
            individual *curr = individual_list;
            while (curr->mem_id != top3_individuals[i])
            {
                curr = curr->next;
            }
            if (curr == NULL)
            {
                printf("ERROR");
            }
            else
            {
                printf("%d. ID: %d, Name: %s, Total Steps: %d\n", i + 1, top3_individuals[i], curr->Name, top3[i]);
            }
        }
    }
}

void Generate_leader_board(group *gptr)
{
    leader *lptr, *prev = NULL;
    group *curr = gptr;
    int flag = 1;
    while (curr != NULL)
    {
        leader *nptr = (leader *)malloc(sizeof(leader));
        strcpy(nptr->gr_name, curr->gr_name);
        int sum = 0;
        for (int i = 0; i < 5; i++)
        {
            for (int j = 0; j < 7; j++)
            {
                sum += curr->arr_mem[i]->stepcount[j];
            }
        }
        prev->next = lptr;
        nptr->next = NULL;
        if (flag)
        {
            lptr = nptr;
            flag = 0;
        }
        curr = curr->next;
    }
    lptr = mergeSort(&lptr);
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

group *Merge_groups(int group_id_1, int group_id_2, group *head)
{
    // Find the groups with the given IDs
    group *group1 = NULL;
    group *group2 = NULL;
    group *curr = head;
    while (curr != NULL)
    {
        if (curr->gr_id == group_id_1)
        {
            group1 = curr;
        }
        if (curr->gr_id == group_id_2)
        {
            group2 = curr;
        }
        curr = curr->next;
    }

    // Check if the groups were found
    if (group1 == NULL || group2 == NULL)
    {
        printf("One or both of the groups not found.\n");
        return NULL;
    }

    // Create a new group with a new ID and name
    int new_group_id = group1->gr_id > group2->gr_id ? group1->gr_id : group2->gr_id + 1;
    char new_group_name[NAME_SIZE];
    strcpy(new_group_name, "Merged Group");

    // Calculate the new group goal
    int new_group_goal = group1->gr_goal + group2->gr_goal;

    // Create a new group
    group *new_group = (group *)malloc(sizeof(group));
    new_group->gr_id = new_group_id;
    strcpy(new_group->gr_name, new_group_name);
    new_group->gr_goal = new_group_goal;
    memset(new_group->arr_mem, 0, SIZE * sizeof(new_group->arr_mem[0]));

    // Merge the members of the two groups into the new group
    int i = 0;
    while (i < SIZE)
    {
        if (group1->arr_mem[i] != NULL)
        {
            new_group->arr_mem[i] = group1->arr_mem[i];
            new_group->memberIDs[i] = group1->memberIDs[i];
            i++;
        }
    }
    int j = 0;
    while (i < SIZE)
    {

        if (group2->arr_mem[i] != NULL)
        {
            new_group->arr_mem[i] = group2->arr_mem[j];
            new_group->memberIDs[i] = group2->memberIDs[j];
            j++;
            i++;
        }
    }

    // Remove the original groups from the list of groups
    if (head == group1)
    {
        head = group1->next;
    }
    else
    {
        curr = head;
        while (curr->next != group1)
        {
            curr = curr->next;
        }
        curr->next = group1->next;
    }
    free(group1);

    if (head == group2)
    {
        head = group2->next;
    }
    else
    {
        curr = head;
        while (curr->next != group2)
        {
            curr = curr->next;
        }
        curr->next = group2->next;
    }
    free(group2);

    // Add the new group to the list of groups
    curr = head;
    while (curr->next != NULL)
    {
        curr = curr->next;
    }
    curr->next = new_group;

    return new_group;
}

int main()
{
    FILE *fptr = fopen("input.txt", "r");
    individual *head = (individual *)malloc(sizeof(individual));
    head = NULL;
    printf("...");
    for (int i = 0; i < 20; i++)
    {
        int mem_id, age, ind_goal;
        char Name[NAME_SIZE];
        int stepcount[7];
        fscanf(fptr, "%d", &mem_id);
        fflush(stdin);
        fscanf(fptr, "%s", Name);
        fflush(stdin);
        fscanf(fptr, "%d", &age);
        fflush(stdin);
        fscanf(fptr, "%d", &ind_goal);
        fflush(stdin);

        for (int i = 0; i < 7; i++)
        {
            fscanf(fptr, "%d", &stepcount[i]);
            // printf("%d ",stepcount[i]);
        }
        Add_Person(mem_id, Name, age, ind_goal, stepcount, head);
    }
    fclose(fptr);
    get_top_3(head);

    group *ghead = (group *)malloc(sizeof(group));
    FILE *gptr = fopen("group.txt", "r");
    int group_id;
    int group_goal;
    int NO_of_members;
    char group_name[NAME_SIZE];
    int members_Id[4];
    for (int i = 0; i < 4; i++)
    {
        group *grp = (group *)malloc(sizeof(group));
        fscanf(gptr, "%d", &group_id);
        fflush(stdin);
        fscanf(gptr, "%s", group_name);
        fflush(stdin);
        fscanf(gptr, "%d", &group_goal);
        fflush(stdin);
        grp = createGroup(group_id, group_name, group_goal, ghead);
        fscanf(gptr, "%d", &NO_of_members);
        fflush(stdin);
        for (int i = 0; i < NO_of_members; i++)
        {
            fscanf(gptr, "%d", &members_Id[i]);
            individual *curr = head;
            while (curr->mem_id != group_id)
            {
                curr = curr->next;
            }
            if (curr != NULL)
            {
                printf("ERROR:-The individual with member id-%d not found in the list", group_id);
            }
            else
            {
                addIndividualToGroup(grp, curr, ghead);
            }
        }
    }
    fclose(gptr);
    return 0;
}