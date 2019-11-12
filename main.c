// Contacts List (Names and Phone Numbers) using Tries
// Contributors:
// Arun, Anirudh BM, Anirudh Shastri, Ayaan, Amogh

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

// Loopholes: You can add multiple contacts with the same phone number.

// Alphabet size (Number of symbols)
#define MAX_NAME_LEN (20)
#define MAX_PHONE_LEN (10)

#define tester(num) printf("%d\n", num);

char *strlwr(char *str) // Remove this if you get a compilation error
{
  unsigned char *p = (unsigned char *)str;

  while (*p) {
     *p = tolower((unsigned char)*p);
      p++;
  }

  return str;
}

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
NodePointer ll_insert_distinct(NodePointer parent, char data){

	// If a parent has no child, let us create the first child
	if (parent->child == NULL){

		parent->child = create_node(data);
		return parent->child;
	}

	// Let's traverse the linked list until we find the same symbol as 'data' 
	// If we don't find it, let us add it to the end of the linked list.
	
	NodePointer current = parent->child;
	
	while (current != NULL){

		if (current->symbol == data) return current;
		current = current->sibling;
	}
	// No matches found. Let's add the 'data' symbol to the front of the ll.
	current = create_node(data);
	current->sibling = parent->child;
	parent->child = current;
	return current;

	//TODO: Sort while adding
}

// Inserts a given word to dictionary.
void n_add_contact(NodePointer parent, char* new_name, char* new_phone){

	NodePointer new_node = NULL;
	
	// Iterating over new_name.
	// Adding every symbol such that one of their children is the next symbol.
	for (int i = 0; new_name[i] != '\0'; ++i){
	
		new_node = ll_insert_distinct(parent, new_name[i]);
		parent = new_node; 
	}
	// If phone number is already filled in.
	if (new_node->phone != NULL){

		int yn;
		printf("Contact already exists.\nDo you want to update the phone no.? (Yes - 1): ");
		scanf("%d", &yn);
		if (yn != 1) return;
	}
	
	// Filling in phone number at for the last symbol.
	new_node->phone = new_phone;	
}

// Returns true if key presents in the dictionary, else returns false
bool n_search_contact( NodePointer root, const char *key)
{/*
    NodePointer temp = root;

    int i;
    for (i = 0; i < strlen(key); i++)
    {
        int index = CHAR_TO_INDEX(key[i]);

        if (temp->children[index] == NULL)
            return false;

        temp = temp->children[index];
    }

    return (temp != NULL && temp->phone);*/
}

// Used for prefix search to get the root.
NodePointer n_traverse_till(NodePointer root, const char *key)
{/*
    NodePointer temp = root;

    int i;
    for (i = 0; i < strlen(key); i++)
    {
        int index = CHAR_TO_INDEX(key[i]);
        if (temp->children[index] == NULL)
        {
            printf("\nNo Words present with the given Prefix: %s \n",key);
            return NULL;
        }
        temp = temp->children[index];
    }
    return temp;*/
}

// To check the words are having only alphabets and converts to lower case.
char* is_valid(char* input){

    int i;
    for(i=0;input[i];i++){
        if(!isalpha(input[i])){
            printf("\n\nNot a valid Word !!!\n\n");
            return NULL;
        }
    }
    return strlwr(input);
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
/*
// To display all words present in the dictionary.
void n_display_all_contacts(NodePointer root, char *buffer, int buffIndex)
{
    bool flag=true;

    int i;
    
    for(i = 0; i < SYMBOL_SIZE; i++)
    {
        if(root->children[i] != NULL)
        {
                buffer[buffIndex] = i + 'a';
                n_display_all_contacts(root->children[i], buffer, buffIndex + 1);
        }
        
        if(root->phone && flag)
        {
                buffer[buffIndex] = '\0';
                if(strlen(buffer) > 0)
                {
                    printf("---> %s %s\n", buffer, root->phone);
                    flag=false;
                }
        }
    }
}
*/
void t_constructor(Trie* trie){

	trie->root = create_node(0);	
}

void t_add_contact(Trie* trie){

	char new_contact_name[MAX_NAME_LEN];
	scanf("%s", new_contact_name);
	
    if (is_valid(new_contact_name)){
    
    	char* new_phone = (char*) calloc(MAX_PHONE_LEN, sizeof *new_phone);
    	scanf("%s", new_phone);
    	//TODO: check validity of phone no
    	n_add_contact(trie->root, strlwr(new_contact_name), new_phone);
    }
    	
    else
    	printf("Name does not meet validity criteria.\n");
}

void t_search_prefix(Trie* trie){

	char prefix[MAX_NAME_LEN];
	scanf("%s", prefix);
               
    if (is_valid(prefix)){
    
    	char buffer[MAX_NAME_LEN];
    	strcpy(buffer, prefix);
    	
    	NodePointer prefix_root = n_traverse_till(trie->root, prefix);

    	if (prefix_root != NULL){
    	
    		n_display_all_contacts(prefix_root, buffer, strlen(prefix));
		}
	}        
}

void t_search_contact(Trie* trie){

	char item[MAX_NAME_LEN];
	int yn;
	scanf("%s", item);
	
	if (is_valid(item)){

		if (n_search_contact(trie->root, item))
			printf("\n%s is present in the contacts list.\n", item);
		
		else{

			printf("\n%s is not present in the contacts list.\n", item);

			printf("\nShall I add it ? (Yes - 1 or No - 0):");
			scanf("%d", &yn);
			
			if (yn == 1){
			
				char* new_phone = (char*) calloc(MAX_PHONE_LEN, sizeof *new_phone);
    			scanf("%s", new_phone);
    			//TODO: check validity of phone no
    			n_add_contact(trie->root, strlwr(item), new_phone);
			}

		}
	}
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
        
        case 2: t_search_prefix(&trie); break;
        
        case 3: t_search_contact(&trie); break;
        
        case 4: t_display_all_contacts(&trie); break;
        
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
