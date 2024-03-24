#include<stdio.h> 
#include<stdlib.h>
#include<string.h> 
#define SIZE 5
#define NAME_SIZE 20
// An AVL tree node 
typedef struct individual 
{ 
	int mem_id;
    char Name[NAME_SIZE];
    int age;
    int ind_goal;
    int stepcount[7];
	struct individual *left; 
	struct individual *right; 
	int height; 
} ind; 

typedef struct gr
{
    int gr_id;
    char gr_name[NAME_SIZE];
    int memberIDs[SIZE];
    ind *arr_mem[SIZE];
    int gr_goal;
    struct gr *left;
    struct gr *right;
    int height;
} group;


// A utility function to get the height of the tree 
int height(struct individual *N) 
{ 
	if (N == NULL) 
		return 0; 
	return N->height; 
} 

// A utility function to get maximum of two integers 
int max(int a, int b) 
{ 
	return (a > b)? a : b; 
} 

/* Helper function that allocates a new node with the given mem_id and 
	NULL left and right pointers. */
struct individual* newNode(int id,char *Name, int age, int goal, int *stepcount) 
{ 
	struct individual* node = (struct individual*) 
						malloc(sizeof(struct individual)); 
    node->mem_id = id;
    strcpy(node->Name, Name);
    node->age = age;
    node->ind_goal = goal;
    memcpy(node->stepcount, stepcount, 7 * sizeof(int));
	node->left = NULL; 
	node->right = NULL; 
	node->height = 1; // new node is initially added at leaf 
	return(node); 
} 

// A utility function to right rotate subtree rooted with y  
struct individual *rightRotate(struct individual *y) 
{ 
	struct individual *x = y->left; 
	struct individual *T2 = x->right; 

	// Perform rotation 
	x->right = y; 
	y->left = T2; 

	// Update heights 
	y->height = max(height(y->left), 
					height(y->right)) + 1; 
	x->height = max(height(x->left), 
					height(x->right)) + 1; 

	// Return new root 
	return x; 
} 

// A utility function to left rotate subtree rooted with x  
struct individual *leftRotate(struct individual *x) 
{ 
	struct individual *y = x->right; 
	struct individual *T2 = y->left; 

	// Perform rotation 
	y->left = x; 
	x->right = T2; 

	// Update heights 
	x->height = max(height(x->left), 
					height(x->right)) + 1; 
	y->height = max(height(y->left), 
					height(y->right)) + 1; 

	// Return new root 
	return y; 
} 

// Get Balance factor of node N 
int getBalance(struct individual *N) 
{ 
	if (N == NULL) 
		return 0; 
	return height(N->left) - height(N->right); 
} 

// Recursive function to insert a mem_id in the subtree rooted 
// with node and returns the new root of the subtree. 
struct individual* Add_Person(struct individual* node, int mem_id,char *Name, int age, int ind_goal, int *stepcount) 
{ 
	/* 1. Perform the normal BST insertion */
	if (node == NULL) 
		return(newNode(mem_id,Name, age, ind_goal, stepcount)); 

	if (mem_id < node->mem_id) 
		node->left = Add_Person(node->left, mem_id,Name, age, ind_goal, stepcount); 
	else if (mem_id > node->mem_id) 
		node->right = Add_Person(node->right, mem_id,Name, age, ind_goal, stepcount); 
	else // Equal mem_ids are not allowed in BST 
		return node; 

	/* 2. Update height of this ancestor node */
	node->height = 1 + max(height(node->left), 
						height(node->right)); 

	/* 3. Get the balance factor of this ancestor 
		node to check whether this node became 
		unbalanced */
	int balance = getBalance(node); 

	// If this node becomes unbalanced, then 
	// there are 4 cases 

	// Left Left Case 
	if (balance > 1 && mem_id < node->left->mem_id) 
		return rightRotate(node); 

	// Right Right Case 
	if (balance < -1 && mem_id > node->right->mem_id) 
		return leftRotate(node); 

	// Left Right Case 
	if (balance > 1 && mem_id > node->left->mem_id) 
	{ 
		node->left = leftRotate(node->left); 
		return rightRotate(node); 
	} 

	// Right Left Case 
	if (balance < -1 && mem_id < node->right->mem_id) 
	{ 
		node->right = rightRotate(node->right); 
		return leftRotate(node); 
	} 

	/* return the (unchanged) node pointer */
	return node; 
} 

int gheight(group *N) 
{ 
	if (N == NULL) 
		return 0; 
	return N->height; 
} 

// A utility function to get maximum of two integers 
int gmax(int a, int b) 
{ 
	return (a > b)? a : b; 
} 

/* Helper function that allocates a new node with the given mem_id and 
	NULL left and right pointers. */
group* newgroupNode(int id,char *Name, int goal) 
{ 
	group* node = (group*) 
						malloc(sizeof(group)); 
    node->gr_id = id;
    strcpy(node->gr_name, Name);
    node->gr_goal = goal;
	node->left = NULL; 
	node->right = NULL; 
	node->height = 1; // new node is initially added at leaf 
	return(node); 
} 

// A utility function to right rotate subtree rooted with y  
group *grouprightRotate(group *y) 
{ 
	group *x = y->left; 
	group *T2 = x->right; 

	// Perform rotation 
	x->right = y; 
	y->left = T2; 

	// Update heights 
	y->height = gmax(gheight(y->left), 
					gheight(y->right)) + 1; 
	x->height = gmax(gheight(x->left), 
					gheight(x->right)) + 1; 

	// Return new root 
	return x; 
} 

// A utility function to left rotate subtree rooted with x 
group *groupleftRotate(group *x) 
{ 
	group *y = x->right; 
	group *T2 = y->left; 

	// Perform rotation 
	y->left = x; 
	x->right = T2; 

	// Update heights 
	x->height = gmax(gheight(x->left), 
					gheight(x->right)) + 1; 
	y->height = gmax(gheight(y->left), 
					gheight(y->right)) + 1; 

	// Return new root 
	return y; 
} 

// Get Balance factor of node N 
int getgroupBalance(group *N) 
{ 
	if (N == NULL) 
		return 0; 
	return gheight(N->left) - gheight(N->right); 
} 

// Recursive function to insert a mem_id in the subtree rooted 
// with node and returns the new root of the subtree. 
group* Add_group(group* node, int gr_id,char *Name, int goal) 
{ 
	/* 1. Perform the normal BST insertion */
	if (node == NULL) 
		return(newgroupNode(gr_id,Name,goal)); 

	if (gr_id < node->gr_id) 
		node->left = Add_group(node->left, gr_id,Name,goal); 
	else if (gr_id > node->gr_id) 
		node->right = Add_group(node->right, gr_id,Name,goal); 
	else // Equal mem_ids are not allowed in BST 
		return node; 

	/* 2. Update height of this ancestor node */
	node->height = 1 + gmax(gheight(node->left), 
						gheight(node->right)); 

	/* 3. Get the balance factor of this ancestor 
		node to check whether this node became 
		unbalanced */
	int balance = getgroupBalance(node); 

	// If this node becomes unbalanced, then 
	// there are 4 cases 

	// Left Left Case 
	if (balance > 1 && gr_id < node->left->gr_id) 
		return grouprightRotate(node); 

	// Right Right Case 
	if (balance < -1 && gr_id > node->right->gr_id) 
		return groupleftRotate(node); 

	// Left Right Case 
	if (balance > 1 && gr_id > node->left->gr_id) 
	{ 
		node->left = groupleftRotate(node->left); 
		return grouprightRotate(node); 
	} 

	// Right Left Case 
	if (balance < -1 && gr_id < node->right->gr_id) 
	{ 
		node->right = grouprightRotate(node->right); 
		return groupleftRotate(node); 
	} 

	/* return the (unchanged) node pointer */
	return node; 
} 

int isIndividualInAnyGroup(group *grouptree, ind *individual)
{
    group *currentGroup = grouptree;
    int found = 0;
    if (currentGroup != NULL)
    {
        for (int i = 0; i < 5; ++i)
        {
            if (currentGroup->arr_mem[i] == individual && !found)
            {
                found = 1;
            }
        }
        if (found==0)
        {
            found = isIndividualInAnyGroup(currentGroup->left,individual);
            if (found==0)
            {
                found = isIndividualInAnyGroup(currentGroup->right,individual);
            }
        }
    }
    return found;
}

void addIndividualToGroup(group *group1, ind *individual1, group *groot)
{
    if (group1 == NULL || individual1 == NULL)
    {
        printf("Group or individual does not exist.\n");
        return;
    }
    if (isIndividualInAnyGroup(groot, individual1))
    {
        printf("Individual is already in another group.\n");
        return;
    }

    for (int i = 0; i < 5; ++i)
    {
        if (group1->arr_mem[i] == NULL)
        {
            group1->memberIDs[i] = individual1->mem_id;
            group1->arr_mem[i] = individual1;
            return;
        }
    }
    printf("Group is full.\n");
}

void display_indiv_info(ind *root)
{
    ind *curr = root;
    if(curr != NULL)
    {
        printf("\n %d \t %10s  %d \t %d\t ", curr->mem_id, curr->Name, curr->age, curr->ind_goal);
        for (int i = 0; i < 7; i++)
        {
            printf("%d ", curr->stepcount[i]);
        }
        printf("\n");
        display_indiv_info(root->left);
        display_indiv_info(root->right);
    }
    if (curr == NULL)
    {
        return;
    }
}

void display_group_info(group *root)
{
    group *curr = root;
    if(curr != NULL)
    {
        printf("\n %d \t %10s  %d\t ", curr->gr_id, curr->gr_name, curr->gr_goal);
        for (int i = 0; i < 5; i++)
        {
            printf("%d ", curr->memberIDs[i]);
        }
        printf("\n");
        display_group_info(root->left);
        display_group_info(root->right);
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
    struct individual* head;
    head = NULL;
    group *ghead;
    ghead = NULL;
    // leader *lead;
    int mem_id, age, ind_goal;
    char Name[NAME_SIZE];
    int stepcount[7];
    for (int i = 0; i < 20; i++)
    {
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
        head = Add_Person(head,mem_id, Name, age, ind_goal, stepcount);
        printf("%d is added\n", mem_id);
    }

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
        ghead = Add_group(ghead,group_id,group_name,group_goal);
        fscanf(gptr, "%d", &NO_of_members);
        fflush(stdin);
        for (int i = 0; i < NO_of_members; i++)
        {
            fscanf(gptr, "%d", &members_Id[i]);
            struct individual *curr = head;
            while (curr != NULL && curr->mem_id != members_Id[i])
            {
                if (curr->mem_id < members_Id[i])
                {
                    curr= curr->right;
                }
                else
                {
                    curr = curr->left;
                }
            }
            if (curr == NULL)
            {
                printf("ERROR:-The individual with member id-%d not found in the list\n", members_Id[i]);
            }
            else
            {
                group *current = ghead;
                while (current != NULL && current->gr_id != group_id)
                {
                    if (current->gr_id < group_id)
                {
                    current= current->right;
                }
                else
                {
                    current = current->left;
                }
                }
                if (current == NULL)
                {
                    printf("ERROR:-The group with group id-%d not found in the list\n", group_id);
                }
                else
                {
                    addIndividualToGroup(current, curr, ghead);
                    // printf("%d",isIndividualInAnyGroup(ghead,curr));
                }
            }
        }
        printf("%d is created and added successfully \n", group_id);
    }
    display_indiv_info(head);
    display_group_info(ghead);
    fclose(fptr);
    fclose(gptr);
    return 0;
}