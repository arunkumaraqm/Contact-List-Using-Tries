# Contact-List-Using-Tries

Tries are often used to store strings. Strings are broken down into their characters and each node stores a single character. Traversing down a specific path in a trie will give a specific name, traversing down another will give another name. Strings are stored in a top to bottom manner on the basis of their prefix in a trie.

In a traditional dictionary using tries, each node stores an array of size 26 (26 letters of the alphabet). This is quick but memory expensive. Here, we used a linked list (left-child-right-sibling representation) instead of an array, storing letters in sorted order.

The project currently has three functionalities: adding a contact, searching for a contact (prefix search), and displaying all contacts. You can also modify a contact's phone number by adding the contact again with the new phone number. The highlight of this project is the search because it offers suggestions for the entered prefix--a basic autocomplete.
