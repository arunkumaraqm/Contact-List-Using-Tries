// Contacts List (Names and Phone Numbers) using Tries
// Contributors:
// Arun Kumar, Anirudh BM, Anirudh Shastri, Ayaan J Ahmed, Amogh Padukone, Ashish Agnihotri

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

// Loopholes: You can add multiple contacts with the same phone number.

#define MAX_NAME_LEN (20)
#define MAX_PHONE_LEN (10)

#define tester(num) printf("%d\n", num); //*

struct Node;
struct Trie;
typedef struct Node* NodePointer;

typedef struct Node
{
	char symbol;
	NodePointer sibling;
    NodePointer child;
	char* phone;
	
} Node;

typedef struct Trie{

	NodePointer root;
} Trie;

// Returns new trie node (initialized to NULLs)
NodePointer create_node(char data)
{
    NodePointer new_node = (NodePointer)malloc(sizeof *new_node);

    if (new_node != NULL)
    {
    	new_node->sibling = new_node->child = NULL;
    	new_node->symbol = data;
    }
    else{
            printf("Heap Full.\n"); //TODO Check whether return value is null everytime create node is called
    }
    return new_node;
}

// Returns the address of the node which stores 'data' symbol.
// All symbols in the linked list are distinct by design.
NodePointer ll_insert_distinct_and_sorted(NodePointer parent, char data){

	// If a parent has no child, let us create the first child
	if (parent->child == NULL){

		parent->child = create_node(data);
		return parent->child;
	}

	// Let's traverse the linked list until we find the same symbol as 'data' 
	// If we don't find it, let us add it to the end of the linked list.
	
	NodePointer current = parent->child, past = NULL; 
	// If past points to one node in the ll, current points to the very next node in the ll. 

	// Upcoming while block won't work for the first node of the ll so this.
	{
		if (current->symbol == data) return current;
		else if (current->symbol > data){

			parent->child = create_node(data);
			parent->child->sibling = current;
			return parent->child;
		} 
		past = current;
		current = current->sibling;
	}

	while (current != NULL){

		if (data == current->symbol){

			return current;
		}
		
		else if (data < current->symbol){
		
			// The ll is presorted in asc order.
			// So, data should be added before the symbol that is more than it.

			past->sibling = create_node(data);
			past->sibling->sibling = current;
			return past->sibling;
		}

		// Loop updation 
		past = current;
		current = current->sibling;
	}

	// We've traversed through the ll now. The data belongs at the end of the ll.
	{
			past->sibling = create_node(data);
			past->sibling->sibling = current; //Remember current is Null here.
			return past->sibling;
	} 
}

// Inserts a given word to dictionary.
void n_add_contact(NodePointer parent, char* new_name, char* new_phone){

	NodePointer new_node = NULL;
	
	// Iterating over new_name.
	// Adding every symbol such that one of their children is the next symbol.
	for (int i = 0; new_name[i] != '\0'; ++i){
	
		new_node = ll_insert_distinct_and_sorted(parent, new_name[i]);
		parent = new_node; 
	}
	// If phone number is already filled in.
	if (new_node->phone != NULL){

		char yn;
		printf("Contact already exists.\nDo you want to update the phone no.?: ");
		scanf(" %c", &yn);
		if (yn != 'y' && yn != 'Y') return;
	}
	
	// Filling in phone number at for the last symbol.
	new_node->phone = new_phone;	
}

// To check the words are having only alphabets and converts to lower case.
char* is_valid(char* input){

    for(int i = 0; input[i] ; ++i){
    
        if(isalpha(input[i]) == false)
            return NULL;

        input[i] = tolower(input[i]);
    }
    return input;
}

void n_display_all_contacts(NodePointer parent, char* buffer, int level){

	if (parent->phone != NULL){

		buffer[level] = '\0';
		printf("%s %s\n", buffer, parent->phone);

		if (parent->child == NULL) return;
	}

	NodePointer current = parent->child;
	do{

		buffer[level] = current->symbol;
		n_display_all_contacts(current, buffer, level + 1);
		current = current->sibling;
	} while (current);
}

// Returns address of the child of parent that contains symbol as 'data'
NodePointer ll_find(NodePointer parent, char data){

	if (parent->child == NULL) return NULL;

	NodePointer current = parent->child;

	do{
		if (current->symbol == data) return current;

		current = current->sibling;

	}while (current);
	
	return NULL;	
}

// Reads characters one by one and offers auto complete suggestions
void n_search_contact_by_prefix(NodePointer root){

	char search_string[MAX_NAME_LEN] = "";
	char yn;

	NodePointer node = root; int i;
	
	for (i = 0; ; ++i){ // TODO i shouldnt exceed MAX_NAME_LEN 

		scanf(" %c", &search_string[i]); // Again doesn't read spaces

		node = ll_find(node, search_string[i]);

		if (node == NULL){

			printf("None found.\n");
			break;
		}
		
		else{

			char suggestion[MAX_NAME_LEN];
			strcpy(search_string, suggestion);
			NodePointer current = node; int j = i;
			while (current->child){

				suggestion[++j] = current->child->symbol;
				current = current->child;
			}
			printf("Suggestion: %s", suggestion);
			
			scanf(" %c", &yn);
			if (yn == 'y' || yn == 'Y'){

				// TODO Display record
				printf("Record = %s\n", suggestion);
				return;
			}
		}
		scanf(" %c", &yn);
		if (yn != 'y' && yn != 'Y')
			break;
	}
}

void t_constructor(Trie* trie){

	trie->root = create_node(0);	
}

void t_add_contact(Trie* trie){

	char new_contact_name[MAX_NAME_LEN];
	scanf("%s", new_contact_name); // TODO FIX Stops at a space
	
    if (is_valid(new_contact_name)){
    
    	char* new_phone = (char*) calloc(MAX_PHONE_LEN, sizeof *new_phone);
    	scanf("%s", new_phone);
    	//TODO: check validity of phone no
    	n_add_contact(trie->root, new_contact_name, new_phone);
    }
    	
    else
    	printf("Name does not meet validity criteria.\n");
}

void t_search_contact_by_prefix(Trie* trie){

	n_search_contact_by_prefix(trie->root);
}

void t_display_all_contacts(Trie* trie){

	char buffer[MAX_NAME_LEN];
	n_display_all_contacts(trie->root, buffer, 0);
}

int main()
{
	Trie trie;
	t_constructor(&trie);
	
	int option;

    do
    {
        scanf("%d",&option);

        switch(option)
        {
        case 1: t_add_contact(&trie); break;
        
        case 2: t_search_contact_by_prefix(&trie); break;
        
        case 3: t_display_all_contacts(&trie); break;
        
        default: option = 0;
        
        }
        
    }while(option);
    
    return 0;
}
/*
1 Adam 901
1 Angelo 902
1 Ada 903
1 Addison 904
1 Bob 905
1 Bojack 906
1 Carmela 907
1 Chad 90898
*/

/* UI expectation
Menu...

<types in something which takes him to "Find Contact">

Suggestion:
Input: 

<types in A>

Suggestion:
Input: A

<types in r>

Suggestion: Arun Kumar
Input: Ar

<types in v>

Suggestion: Arvind
Input: Arv

<types in t>

None found
Input: Arvt

<presses backspace>

Suggestion: Arvind
Input: Arv

<presses backspace>

Suggestion: Arun Kumar
Input: Ar

<types in u>

Suggestion: Arun Kumar
Input: Aru

<presses Enter>

Contact: Arun Kumar 9876543210
1. Call 2. Delete 3. Back

<types in 3>

Suggestion: 
Input: 

<types in G>

Suggestion:
Input: G

<presses Esc - wants to go back>

Menu...

*/
