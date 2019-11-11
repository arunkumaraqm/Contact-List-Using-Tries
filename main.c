// Contacts List (Names and Phone Numbers) using Tries
// Contributors:
// Arun, Anirudh BM, Anirudh Shastri, Ayaan, Amogh

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

// Alphabet size (Number of symbols)
#define ALPHABET_SIZE (26)
#define MAX_NAME_LEN (20)
#define MAX_PHONE_LEN (10)

#define tester(num) printf("%d\n", num);

// Converts key current character into index
// use only 'a' through 'z' and lower case
#define CHAR_TO_INDEX(c) ((int)c - (int)'a')

char *strlwr(char *str) // Remove this if you get a compilation error
{
  unsigned char *p = (unsigned char *)str;

  while (*p) {
     *p = tolower((unsigned char)*p);
      p++;
  }

  return str;
}

// trie node
typedef struct Node
{
    struct Node *children[ALPHABET_SIZE];
	char* phone;
	
} Node;

typedef struct Node* NodePointer;

typedef struct Trie{

	NodePointer root;
} Trie;

// Returns new trie node (initialized to NULLs)
NodePointer create_node(void)
{
    NodePointer new_node = (NodePointer)malloc(sizeof(struct Node));

    if (new_node != NULL)
    {
        new_node->phone = NULL;
       
        for (int i = 0; i < ALPHABET_SIZE; i++)
            new_node->children[i] = NULL;
    }
    else
    {
            printf("Heap Full.\n");
    }
    return new_node;
}

// Inserts a given word to dictionary.
void n_add_contact(NodePointer root, const char *key, char* phone)
{
    NodePointer temp = root;

    for (int i = 0; key[i]; ++i)
    {
        // To get the position of the character eg- a=0 , b=1
        int index = CHAR_TO_INDEX(key[i]);

        // Checks whether node is already present, if not then create one
        if (temp->children[index]==NULL)
            temp->children[index] = create_node();

        // To traverse to next node.
        temp = temp->children[index];
    }

    temp->phone = phone;
}

// Returns true if key presents in the dictionary, else returns false
bool n_search_contact( NodePointer root, const char *key)
{
    NodePointer temp = root;

    int i;
    for (i = 0; i < strlen(key); i++)
    {
        int index = CHAR_TO_INDEX(key[i]);

        if (temp->children[index] == NULL)
            return false;

        temp = temp->children[index];
    }

    return (temp != NULL && temp->phone);
}

// Used for prefix search to get the root.
NodePointer n_traverse_till(NodePointer root, const char *key)
{
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
    return temp;
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

// To display all words present in the dictionary.
void n_display_all_contacts(NodePointer root, char *buffer, int buffIndex)
{
    bool flag=true;

    int i;
    
    for(i = 0; i < ALPHABET_SIZE; i++)
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

void t_constructor(Trie* trie){

	trie->root = create_node();	
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

		if (n_search_contact(trie->root,item))
			printf("\n%s is present in the dictionary.\n", item);
		
		else{

			printf("\n%s is not present in the dictionary.\n", item);

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
