#include <iostream>
#include <mutex>
#include <atomic>

struct Node
{
    int value;
    std::atomic<Node*> next;
    std::mutex node_mutex;

    Node(int val) : value(val), next(nullptr) {}
};

class FineGrainedQueue
{
public:
    FineGrainedQueue() : head(nullptr) {}
    ~FineGrainedQueue();

    void insertIntoMiddle(int value, int pos);
    void printList();

private:
    std::atomic<Node*> head;
};

FineGrainedQueue::~FineGrainedQueue()
{
    Node* current = head.load();
    while (current)
    {
        Node* temp = current;
        current = current->next.load();
        delete temp;
    }
}

void FineGrainedQueue::insertIntoMiddle(int value, int pos)
{
    Node* newNode = new Node(value);

    // заблокировал мютекс нового узла
    newNode->node_mutex.lock();

    // поиск узла в заданной позиции или конце списка
    Node* prevNode = nullptr;
    Node* currentNode = head.load();
    int currentPosition = 0;

    while (currentNode && currentPosition < pos)
    {
        prevNode = currentNode;
        currentNode = currentNode->next.load();
        currentPosition++;
    }

    // заблокировал мютекс предыдущего узла
    if (prevNode)
        prevNode->node_mutex.lock();

    // вставка нового узла в список
    newNode->next = currentNode;
    if (prevNode)
        prevNode->next = newNode;
    else
        head.store(newNode);

    // разблокировал мютекс из нового и предыдущего узлов
    if (prevNode)
        prevNode->node_mutex.unlock();
    newNode->node_mutex.unlock();
}

void FineGrainedQueue::printList()
{
    Node* current = head.load();
    while (current)
    {
        std::cout << current->value << " ";
        current = current->next.load();
    }
    std::cout << std::endl;
}

int main()
{
    FineGrainedQueue queue;

    for (int i = 0; i < 5; ++i)
    {
        queue.insertIntoMiddle(i, i);
    }

    // вывод списка
    queue.printList();

    return 0;
}

