/*
Contacts List (Names and Phone Numbers) using Tries

Semester 3 Mini Project for Data Structures & Applications
Contributors:
Arun Kumar, Anirudh BM, Anirudh Shastri, Ayaan J Ahmed, Amogh Padukone, Ashish Agnihotri
*/

#include <stdio.h>
#include <stdlib.h> // For malloc
#include <string.h>
#include <stdbool.h> // For bool data type
#include <ctype.h>
#include "tries.h" // Stores struct definitions

// Loopholes: You can add multiple contacts with the same phone number.

/* Naming Convention:
ll_ functions - linked list operation
t_ functions can only change trie->root
n_ functions - pretty much everything else to do with the trie's nodes
*/

// Utility macro
#define free_and_null(ptr) {\
\
	free(ptr);\
	ptr = NULL;\
}

// Returns new trie node 
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

// Inserts a given contact. 
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

// Checks if words are having only alphabets and converts to lower case.
char* is_valid(char* input){

    for(int i = 0; input[i] ; ++i){
    
        if(isalpha(input[i]) == false)
            return NULL;

        input[i] = tolower(input[i]);
    }
    return input;
}

// Displays all contacts in ascending order
void n_display_all_contacts(NodePointer parent, char* buffer, int level){
// buffer stores all the characters before the current character
// level signifies the current index of the buffer

	// If phone number exists, the node is a contact. So, print it.
	// But if its child is NULL, that means the node is also a leaf. No more recursive calls.
	if (parent->phone != NULL){

		buffer[level] = '\0';
		printf("%s %s\n", buffer, parent->phone);

		if (parent->child == NULL) return;
	}

	// Calling display on all of the nodes in the linked list.
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
		if (current->symbol == data) return current; // Match found.

		current = current->sibling;

	}while (current);

	// No match found
	return NULL;	
}

// Reads characters one by one and offers auto complete suggestions
void n_search_contact_by_prefix(NodePointer root){

	char search_string[MAX_NAME_LEN] = "";
	char yn;

	NodePointer node = root; int i;

	//TODO Fancier i/o @AnirudhBM
	
	for (i = 0; ; ++i){ // TODO i shouldnt exceed MAX_NAME_LEN @ArunKumar 

	// TODO Letter casing issue @ArunKumar
	
		printf("Enter character: ");//*
		scanf(" %c", &search_string[i]); // Again doesn't read spaces

		node = ll_find(node, search_string[i]);

		
		if (node == NULL){

			printf("None found.\n");
			break;
		}
		
		else{

			char suggestion[MAX_NAME_LEN] = "";
			char* phone = NULL;
			
			if (node->phone == NULL){

				NodePointer current = node->child; int j = 0;

				// By design, current wouldn't be NULL if node->phone is NULL. 
				// If that design ever changes, the next line will give a bug.
				
				suggestion[j++] = current->symbol;
				while (current->phone == NULL){

					current = current->child;
					suggestion[j++] = current->symbol;
				}
				suggestion[j] = '\0';
				phone = current->phone;
			}
			else phone = node->phone;
			
			printf("Suggestion: %s%s\n", search_string, suggestion);

			printf("Accept suggestion? (y/n): ");//*
			scanf(" %c", &yn);
			if (yn == 'y' || yn == 'Y'){

				printf("Record = %s%s %s\n", search_string, suggestion, phone);//*
				return;
			}
		}

		printf("Continue entering characters? (y/n): ");//*
		scanf(" %c", &yn);
		if (yn != 'y' && yn != 'Y')
			break;
	}
}

// Frees memory occupied by nodes
void n_destroy_children(NodePointer parent){

	if (parent->child == NULL) return;

	// Destroys linked list from left to right
	NodePointer past = NULL;
	NodePointer current = parent->child;
	do{

		n_destroy_children(current); // Destroys all of the bottom
		past = current;
		current = current->sibling;
		free_and_null(past);
		
	} while (current != NULL);
}

// Creates the root node.
void t_constructor(Trie* trie){

	trie->root = create_node(0);	
}

// Reads a contact and adds it to the trie
void t_add_contact(Trie* trie){

	char new_contact_name[MAX_NAME_LEN];
	scanf("%s", new_contact_name); // TODO FIX Stops at a space @AnirudhBM
	
    if (is_valid(new_contact_name)){
    
    	char* new_phone = (char*) calloc(MAX_PHONE_LEN, sizeof *new_phone);
    	scanf("%s", new_phone);
    	//TODO: check validity of phone no @AshishAgnihotri
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

// Called at the end of main
void t_destructor(Trie* trie){

	n_destroy_children(trie->root);
	free_and_null(trie->root);
}

int main()
{
	Trie trie;
	t_constructor(&trie);
//TODO t_read_to_file(&trie); @AnirudhShastri
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

//TODO t_write_to_file(&trie); @AnirudhShastri
    t_destructor(&trie);
    return 0;
}

/* Some inputs for test purposes

1 Adam 901
1 Angelo 902
1 Ada 903
1 Addison 904
1 Bob 905
1 Bojack 906
1 Carmela 907
1 Chad 90898

1 m 909
1 mi 910
1 mif 908
1 mil 909
1 mig 910
1 mifg 984
1 milhdhi 984
1 milhd 558789

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
