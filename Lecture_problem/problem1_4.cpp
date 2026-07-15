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
    Node* head = new Node();
    head->value = 0;
    head->next = nullptr;
    Node* current = head;

    for (int i = 1; i < 10; i ++)
    {
        Node* new_node = new Node();
        new_node->value = i;
        new_node->next = nullptr;
        current->next = new_node;
        current = new_node;
    }
    // while(head != nullptr)
    // {
    //     std::cout << head->value << " ";
    //     head = head->next;
    // }
    reorder_students(head, 10);
    while(head != nullptr)
    {
        std::cout << head->value << " ";
        head = head->next;
    }
}