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
    char gr_name[NAME_SIZE];
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
    while (curr != NULL && newIndividual->mem_id > curr->mem_id)
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

group *createGroup(int groupID, char *groupName, int weeklyGroupGoal,group *head)
{
    group *newGroup = (group *)malloc(sizeof(group));
    newGroup->gr_id = groupID;
    strcpy(newGroup->gr_name, groupName);
    newGroup->gr_goal = weeklyGroupGoal;
    memset(newGroup->memberIDs, 0, SIZE * sizeof(int));
    memset(newGroup->arr_mem, 0, SIZE * sizeof(newGroup->arr_mem[0]));

    newGroup->next = NULL;

    // group *head = *headptr;

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
    int found = 0;
    while (currentGroup != NULL)
    {
        for (int i = 0; i < 5; ++i)
        {
            if (currentGroup->arr_mem[i] == individual && !found)
            {
                found = 1;
            }
        }
        currentGroup = currentGroup->next;
    }
    return found;
}

void addIndividualToGroup(group *group1, individual *individual1,group *groupList)
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
            group1->memberIDs[i]=individual1->mem_id;
            group1->arr_mem[i] = individual1;
            printf("successful\n");
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
    if (head == NULL)
    {
        printf("individual list is empty: \n");
    }
    else
    {
        while (curr != NULL && id != curr->mem_id)
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
    }
    printf("%d is deleted successfully...", id);
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
            printf("Group id:%d \n The group with above group id have achieved the "
                   "group goal for this week\n Cumulative step count of group:%d \n "
                   "Group goal:%d",
                   curr->gr_id, sum, curr->gr_goal);
        }
        else
        {
            printf("Group id:%d \n The group with above group id have not achieved "
                   "the group goal for this week\n Cumulative step count of group:%d "
                   "\n Group goal:%d \n The group is short of %d no. of steps in the "
                   "current week",
                   curr->gr_id, sum, curr->gr_goal, curr->gr_goal - sum);
        }
    }
    else
    {
        printf("The group with group id:%d not found in the list", group_id);
    }
}


int* get_top_3(individual *individual_list,int *top3_individuals) {
  individual *current = individual_list;
  static int top3[3] = {0, 0, 0};
  int count = 0;

  while (current != NULL) {
    int total_steps = 0;
    int flag = 1; // Reset flag for each individual
    for (int i = 0; i < 7; i++) {
      if (current->stepcount[i] > current->ind_goal) {
        total_steps += current->stepcount[i];
      } else {
        flag = 0;
        break; // No need to continue summing if goal not met
      }
    }
    if (flag) {
      if (count < 3) {
        top3[count] = total_steps;
        top3_individuals[count] = current->mem_id;
        count++;
      } else {
        // Sort top 3 if necessary
        for (int i = 0; i < 2; i++) {
          for (int j = 0; j < 2 - i; j++) {
            if (top3[j] < top3[j + 1]) {
              int temp = top3[j];
              top3[j] = top3[j + 1];
              top3[j + 1] = temp;
              temp = top3_individuals[j];
              top3_individuals[j] = top3_individuals[j + 1];
              top3_individuals[j + 1] = temp;
            }
          }
        }
        // Reset flag for next iterations
        flag = 0;
      }
    }
    current = current->next;
  }
  // Print top 3 individuals
  printf("Top 3 individuals:\n");
  for (int i = 0; i < 3; i++) {
    if (top3_individuals[i] != 0) {
      individual *curr = individual_list;
      while (curr->mem_id != top3_individuals[i]) {
        curr = curr->next;
      }
      if (curr == NULL) {
        printf("ERROR");
      } else {
        printf("%d. ID: %d, Name: %s, Total Steps: %d\n", i + 1,
               top3_individuals[i], curr->Name, top3[i]);
      }
    }
  }
  return top3;
}

void Check_individual_rewards(int id,individual *head)
{
    int *arr,*ptr;
    arr = (int*)malloc(sizeof(int)*3);
    ptr = (int*)malloc(sizeof(int)*3);
    ptr  = get_top_3(head,arr);
    if (arr[0] == id)
    {
        printf("The person with person id:%d is on top 1 of the leaderboard\n", id);
    }
    else if (arr[1] == id)
    {
        printf("The person with person id:%d is on top 2 of the leaderboard\n", id);
    }
    else if (arr[2]==id)
    {
        printf("The person with person id:%d is on top 3 of the leaderboard\n", id);
    }
    else
    {
        printf("The person with person id:%d is not in the top 3 of the leaderboard\n", id);
    }
    free(arr);
    free(ptr);
}

void Suggest_goal_update(individual *head)
{
    int *arr,*ptr,goal;
    arr = (int*)malloc(sizeof(int)*3);
    ptr = (int*)malloc(sizeof(int)*3);
    ptr  = get_top_3(head,arr);
    for (int i = 0; i < 3; i++)
    {
        goal = ptr[i]/7;
        printf("\nyou need to have minimum daily goal of %d for rank %d \n",goal+1,i+1);
    }
    free(arr);
    free(ptr);
}

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

void split(leader *source, leader **frontRef, leader **backRef)
{
    if (source == NULL || source->next == NULL)
    {
        *frontRef = source;
        *backRef = NULL;
        return;
    }

    leader *slow = source;
    leader *fast = source->next;

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

leader *mergeSort(leader *head)
{
    leader *a;
    leader *b;

    if (head == NULL || head->next == NULL)
    {
        return NULL;
    }

    split(head, &a, &b);

    a = mergeSort(a);
    b = mergeSort(b);

    head = merge(a, b);
    return head;
}

// void Generate_leader_board(group *gptr)
// {
//     leader *lptr= NULL, *prev = NULL, *nptr;
//     group *curr = gptr;
//     while (curr != NULL)
//     {
//         nptr = (leader *)malloc(sizeof(leader));
//         strcpy(nptr->gr_name, curr->gr_name);
//         int sum = 0;
//         for (int i = 0; i < 5; i++)
//         {
//             for (int j = 0; j < 7; j++)
//             {
//                 sum += curr->arr_mem[i]->stepcount[j];
//             }
//         }
//         nptr->next = lptr;
//         lptr = nptr;
//         printf("%s",curr->gr_name);
//         curr = curr->next;
//     }
//     lptr = mergeSort(lptr);
//     leader *present = lptr;
//     int pos = 1;
//     printf("Position \t Group Name \t Stepcount \n");
//     while (pos <= 3 && present != NULL)
//     {
//         printf("%d \t %s \t %d \n", pos, present->gr_name, present->gr_stepcount);
//         pos++;
//         present = present->next;
//     }
// }

void Generate_leader_board(group *gptr) {
    leader *lptr = NULL, *prev = NULL, *nptr;
    group *curr = gptr;
    while (curr != NULL) {
        nptr = (leader *)malloc(sizeof(leader));
        strcpy(nptr->gr_name, curr->gr_name);
        int sum = 0;
        for (int i = 0; i < SIZE; i++) { // Iterate over the members of the group
            if (curr->arr_mem[i] != NULL) { // Check if member exists
                for (int j = 0; j < 7; j++) {
                    sum += curr->arr_mem[i]->stepcount[j];
                }
            }
        }
        nptr->gr_stepcount = sum; // Assign the calculated sum to gr_stepcount
        nptr->next = lptr;
        lptr = nptr;
        curr = curr->next;
    }
    leader *present = lptr;
    int pos = 1;
    printf("Position \t Group Name \t Stepcount \n");
    while (pos <= 3 && present != NULL) {
        printf("%d \t %s \t %d \n", pos, present->gr_name, present->gr_stepcount);
        pos++;
        present = present->next;
    }
}

void Display_group_info(group *ghead, int id)
{
    group *current = ghead;
    if (ghead == NULL)
    {
        printf("group list is empty\n");
    }
    else
    {
        while (current != NULL && current->gr_id != id)
        {
            current = current->next;
        }
        if (current != NULL)
        {

            printf("Group name :- %s\n", current->gr_name);
            printf("Group id :- %d\n", current->gr_id);
            printf("Group weekly goal :-%d \n", current->gr_goal);
            printf("Group members :-\n");
            printf("ID \t Name \t\n");
            for (int i = 0; i < 5; i++)
            {
                printf("%d \t %s\n", current->memberIDs[i], current->arr_mem[i]->Name);
            }
        }
        else
        {
            printf("Group not found");
        }
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

group *Merge_groups(int group_id_1, int group_id_2, group **headptr)
{
    group *group1 = NULL;
    group *group2 = NULL;
    group *curr = *headptr;
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

    if (group1 == NULL || group2 == NULL)
    {
        printf("One or both of the groups not found.\n");
        return NULL;
    }

    int new_group_id = (group1->gr_id > group2->gr_id) ? group1->gr_id : group2->gr_id;
    char new_group_name[NAME_SIZE];
    strcpy(new_group_name, "Merged Group");

    int new_group_goal = group1->gr_goal + group2->gr_goal;

    group *new_group = (group *)malloc(sizeof(group));
    if (new_group == NULL) {

        return NULL;
    }
    new_group->gr_id = new_group_id;
    strcpy(new_group->gr_name, new_group_name);
    new_group->gr_goal = new_group_goal;
    memset(new_group->arr_mem, 0, SIZE * sizeof(new_group->arr_mem[0]));
    int i = 0;
    while (i < SIZE && group1->arr_mem[i] != NULL)
    {
        new_group->arr_mem[i] = group1->arr_mem[i];
        i++;
    }
    int j = 0;
    while (i < SIZE && group2->arr_mem[j] != NULL)
    {
        new_group->arr_mem[i] = group2->arr_mem[j];
        i++;
        j++;
    }

    if (*headptr == group1)
    {
        *headptr = group1->next;
        free(group1);
    }
    else
    {
        curr = *headptr;
        while (curr != NULL && curr->next != group1)
        {
            curr = curr->next;
        }
        if (curr != NULL) {
            curr->next = group1->next;
            free(group1);
        }
    }

    if (*headptr == group2)
    {
        *headptr = group2->next;
        free(group2);
    }
    else
    {
        curr = *headptr;
        while (curr != NULL && curr->next != group2)
        {
            curr = curr->next;
        }
        if (curr != NULL) {
            curr->next = group2->next;
            free(group2);
        }
    }

    if (*headptr == NULL)
    {
        *headptr = new_group;
    }
    else
    {
        curr = *headptr;
        while (curr->next != NULL)
        {
            curr = curr->next;
        }
        curr->next = new_group;
    }

    printf("successfully");
    return new_group;
}

void display_indiv_info(individual *ind_list)
{
    individual *curr = ind_list;
    printf("S.no \t Name \t Age \t Daily Goal \t Stepcount\n");
    int i = 0;
    while (curr != NULL)
    {
        printf("\n %d \t %s \t %d \t %d\t ", curr->mem_id, curr->Name, curr->age, curr->ind_goal);
        for (int i = 0; i < 7; i++)
        {
            printf("%d ", curr->stepcount[i]);
        }
        printf("\n");
        curr = curr->next;
        i++;
    }
    if (curr == NULL)
    {
        return;
    }
}

int main()
{
    FILE *fptr = fopen("input.txt", "r");
    FILE *gptr = fopen("group.txt", "r");
    individual *head;
    head = NULL;
    group *ghead;
    ghead = NULL;
    leader *lead;

    int choice = 0;
    while (choice != 13)
    {
        printf("\n\n\n\n\nCHOOSE ANY OPTION:-\n");
        printf("1- Add a person\n2- Create a group\n3- Get top 3 winners\n4- Check group achievements\n5- Generate Leaderboard of groups\n6- check individual rewards\n7- delete individual\n8- Delete a group\n9- suggest goal updates for any person\n10- merge two groups\n11- display group info\n12-display all individual info\n13-EXIT\n");
        printf("Enter:");
        scanf("%d", &choice);
        if (choice == 1)
        {
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

                for (int j = 0; j < 7; j++)
                {
                    fscanf(fptr, "%d", &stepcount[j]);
                }
                head = Add_Person(mem_id, Name, age, ind_goal, stepcount, head);
                printf("%d is added\n",mem_id);
            }
        }
        else if (choice == 2)
        {   
            int group_id;
            int group_goal;
            int NO_of_members;
            char group_name[NAME_SIZE];
            int members_Id[4];
            for (int i = 0; i < 4; i++)
            {
                fscanf(gptr, "%d", &group_id);
                fflush(stdin);
                fscanf(gptr, "%s", group_name);
                fflush(stdin);
                fscanf(gptr, "%d", &group_goal);
                fflush(stdin);
                ghead = createGroup(group_id, group_name, group_goal, ghead);
                fscanf(gptr, "%d", &NO_of_members);
                fflush(stdin);
                // printf("%d ", NO_of_members);
                for (int i = 0; i < NO_of_members; i++)
                {
                    fscanf(gptr, "%d", &members_Id[i]);
                    individual *curr = head;
                    while (curr != NULL && curr->mem_id != members_Id[i])
                    {
                        curr = curr->next;
                        printf("mem id:%d  members_Id[i]:%d\n", curr->mem_id, members_Id[i]);
                        printf(".");
                    }
                    if (curr == NULL)
                    {
                        printf("ERROR:-The individual with member id-%d not found in the list\n", members_Id[i]);
                    }
                    else
                    {
                        group *current = ghead;
                        printf("\n group id: %d \tcurrent group id :%d\n", group_id, current->gr_id);
                        while (current != NULL && current->gr_id != group_id)
                        {
                            current = current->next;
                            printf("\n group id: %d \tcurrent group id :%d\n", group_id, current->gr_id);
                        }
                        if (current == NULL)
                        {
                            printf("ERROR:-The group with group id-%d not found in the list\n", group_id);
                        }
                        else
                        {
                            addIndividualToGroup(current, curr, ghead);
                        }
                    }
                }
            }
        }
        else if (choice == 3)
        {
            int *arr;
            arr = (int*)malloc(sizeof(int)*3);
            int* ptr = get_top_3(head,arr);
        }
        else if (choice == 4)
        {
            int id;
            printf("enter group id no.:");
            scanf("%d", &id);
            Check_group_achievement(id,ghead);
        }
        else if (choice == 5)
        {
            Generate_leader_board(ghead);
        }
        else if (choice == 6)
        {
            int id;
            printf("enter id no.:");
            scanf("%d", &id);
            Check_individual_rewards(id,head);
        }
        else if (choice == 7)
        {
            int id;
            printf("enter id no.:");
            scanf("%d", &id);
            Delete_individual(id, head);
        }
        else if (choice == 8)
        {
            int id;
            printf("enter group id no.:");
            scanf("%d", &id);
            ghead = Delete_group(id,ghead);
        }
        else if (choice == 9)
        {
            Suggest_goal_update(head);
        }
        else if (choice == 10)
        {
            int id1,id2;
            printf("enter group id Nos.:");
            scanf("%d", &id1);
            scanf("%d",&id2);
            ghead = Merge_groups(id1,id2,&ghead);   
        }
        else if (choice == 11)
        {
            int id;
            printf("enter group id no.:");
            scanf("%d", &id);
            Display_group_info(ghead,id);
        }
        else if (choice == 12)
        {
            display_indiv_info(head);
        }
        else if (choice == 13)
        {
            printf("EXITING...");
        }
        else
        {
            printf("wrong input, try again...!");
        }
    }
    fclose(fptr);
    fclose(gptr);
    return 0;
}