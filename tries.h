
#define MAX_NAME_LEN (20)
#define MAX_PHONE_LEN (10)

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


