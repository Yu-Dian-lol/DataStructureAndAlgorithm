#include <iostream>
#include <cmath>
struct Node
{
   int value;
   Node* next = nullptr;
};
void reorder_students(Node* head, int len)
{
    int n = len / 2;
    if (n == 0)
        return;
    
    Node* a = head;
    for (int i = 0; i < n-1; i ++)
    {
        a = a->next;
    }
    Node *b = a->next;
    
    Node * previous = nullptr;
    Node* current = b;
    for (int i = 0; i < n; i ++)
    {
        Node* next = current->next;
        current->next = previous;
        previous = current;
        current = next;
    }

    a->next = previous; 
}
int main()
{

    

}