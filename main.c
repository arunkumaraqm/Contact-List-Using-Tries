#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

// Alphabet size (Number of symbols)
#define ALPHABET_SIZE (26)

// Converts key current character into index
// use only 'a' through 'z' and lower case
#define CHAR_TO_INDEX(c) ((int)c - (int)'a')

// trie node
struct TrieNode
{
    struct TrieNode *children[ALPHABET_SIZE];

    // isLeaf is true if the node represent end of a word.
    bool isLeaf;
};

typedef struct TrieNode* NODEPTR;

// Returns new trie node (initialized to NULLs)
NODEPTR getNode(void)
{
    NODEPTR newNode = (NODEPTR)malloc(sizeof(struct TrieNode));

    if (newNode!=NULL)
    {
        newNode->isLeaf = false;
        int i;
        for (i = 0; i < ALPHABET_SIZE; i++)
            newNode->children[i] = NULL;
    }
    else
    {
            printf("\n\nFailed to create the Node... Malloc Failed...\n");
    }
    return newNode;
}

// Inserts a given word to dictionary.
void insert(NODEPTR root, const char *key)
{
    NODEPTR temp = root;

    int i;
    for (i = 0; i < strlen(key); i++)
    {
        // To get the position of the character eg- a=0 , b=1
        int index = CHAR_TO_INDEX(key[i]);

        // Checks whether node is already present, if not then create one
        if (temp->children[index]==NULL)
            temp->children[index] = getNode();

        // To traverse to next node.
        temp = temp->children[index];
    }

    // To indicate the last character of the word.
    temp->isLeaf = true;
}

// Returns true if key presents in the dictionary, else returns false
bool search( NODEPTR root, const char *key)
{
    NODEPTR temp = root;

    int i;
    for (i = 0; i < strlen(key); i++)
    {
        int index = CHAR_TO_INDEX(key[i]);

        if (temp->children[index] == NULL)
            return false;

        temp = temp->children[index];
    }

    return (temp != NULL && temp->isLeaf);
}

// Used for prefix search to get the root.
NODEPTR traverseTill(NODEPTR root, const char *key)
{
    NODEPTR temp = root;

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
char* validation(char* input){

    int i;
    for(i=0;i<strlen(input);i++){
        if(!isalpha(input[i])){
            printf("\n\nNot a valid Word !!!\n\n");
            return NULL;
        }
    }
    return strlwr(input);
}

// To display all words present in the dictionary.
void printTrieContents(NODEPTR root, char *buffer, int buffIndex)
{
    bool flag=true;
    //bool emptyDictionary=true;

    int i;
    /*
    for(i=0;i<ALPHABET_SIZE;i++)
    {
        if(root->children[i] != NULL)
                emptyDictionary=false;

    }
    if(emptyDictionary)
    {
        printf("\n:- Dictionary is EMPTY....\n");
        return;
    }
    */
    for(i = 0; i < ALPHABET_SIZE; i++)
    {
        if(root->children[i] != NULL)
        {
                buffer[buffIndex] = i + 'a';
                printTrieContents(root->children[i], buffer, buffIndex + 1);
        }
        if(root->isLeaf && flag)
        {
                buffer[buffIndex] = '\0';
                if(strlen(buffer) > 0)
                {
                    printf("---> %s \n", buffer);
                    flag=false;
                }
        }
    }
}

int main()
{
    char buffer[20];

    NODEPTR root = getNode();

    printf(".......Welcome to the Data Structure Project on Dictionary Implementation Using TRIE........\n");

    int option ;
    char wordToDictionary[20];
    char wordToSearch[20];
    char prefix[20];
    char yesOrNo[10];
    //infinite loop
    while(1)
    {
        printf("\n1) ADD the Word to the dictionary.\n");
        printf("2) PRINT_ALL the words in the dictionary.\n");
        printf("3) SEARCH a word from the dictionary.\n");
        printf("4) PRINT_ALL words with a given PRIFIX.\n");
        printf("5) EXIT from the menu.\n");
        printf("Select your option : ");
        scanf("%d",&option);

        switch(option)
        {
        case 1:printf("\nEnter the word to add to dictionary : ");
               scanf("%s", wordToDictionary);
               if(validation(wordToDictionary)!=NULL)
                   insert(root, strlwr(wordToDictionary));
               break;

        case 2:printf("\nContents in the Dictionary are:\n");
               printTrieContents(root,buffer,0);
               break;

        case 3:printf("\nEnter the word to SEARCH : ");
               scanf("%s",wordToSearch);
               if(validation(wordToSearch)!=NULL)
                {
                    if(search(root,wordToSearch))
                        printf("\n%s is present in the dictionary.\n",wordToSearch);
                else
                {
                    printf("\n%s is not present in the dictionary.\n",wordToSearch);

                    printf("\nShall I add it ? (Yes or No):");
                    scanf("%s",yesOrNo);
                    if(strcmp("yes",yesOrNo)==0||strcmp("YES",yesOrNo)==0||strcmp("Yes",yesOrNo)==0)
                            insert(root, wordToSearch);

                }
                }
                break;

        case 4:printf("\nEnter the PRIFIX : ");
               scanf("%s",prefix);
               if(validation(prefix)!=NULL)
               {
                    strcpy(buffer,prefix);
                    printf("\nPrinting all the dictionary words with given PRIFIX : %s \n",prefix);
                    NODEPTR prefixRoot= traverseTill(root,prefix);
                    if(prefixRoot!=NULL)
                        {
                            printTrieContents(prefixRoot,buffer,strlen(prefix));
                        }
                }
                break;

        case 5:printf("\n\nTHANK YOU !!! :) \n\n");
               exit(0);

        default:printf("\n\nInvalid Option.\n\n");
        }
    }
    return 0;
}
