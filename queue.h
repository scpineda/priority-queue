#ifndef QUEUES_H
#define QUEUES_H
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <algorithm>

using namespace std;

// The following is a Node struct

template<typename Data, typename Priority>
struct node
{
    Data data;          // The actual data. Example: Patient's name
    Priority priority;  // Priority. Can be a number

    node(Data d = Data(), Priority p = Priority())
    {
        data = d;
        priority = p;
    }

    ~node()
    {
        data = Data();
        priority = Priority();
    }

    node(const node<Data,Priority> &other)
    {
        data = other.data;
        priority = other.priority;
    }

    node<Data,Priority>& operator=(const node<Data,Priority> &other)
    {
        if(this != &other)
        {
            data = other.data;
            priority = other.priority;
        }
        return *this;
    }

    template<typename D, typename P>
    friend
    bool operator<(const node<D,P> &x, const node<D,P> &y);

    template<typename D, typename P>
    friend
    bool operator<=(const node<D,P> &x, const node<D,P> &y);

    template<typename D, typename P>
    friend
    bool operator>(const node<D,P> &x, const node<D,P> &y);

};

template<typename D, typename P>
bool operator<(const node<D,P> &x, const node<D,P> &y)
{
    return x.priority < y.priority;
}

template<typename D, typename P>
bool operator<=(const node<D,P> &x, const node<D,P> &y)
{
    return x.priority <= y.priority;
}

template<typename D, typename P>
bool operator>(const node<D,P> &x, const node<D,P> &y)
{
    return x.priority > y.priority;
}


// BEGINNING QUEUE CODE
// Note that this code is modified to make an array

enum QUEUE_ERRORS {Q_FULL, Q_EMPTY, Q_BAD_SIZE};

template<typename Data, typename Priority = int>
class queue
{
    public:
        queue(size_t c = 10);
        ~queue();
        queue(const queue<Data,Priority> &other);
        queue<Data,Priority>& operator=(const queue<Data,Priority> &other);

        bool full();
        bool empty();
        bool search(Data &d);
        size_t size();
        size_t capacity();
        void resize(size_t newSize);
        Data peek();
        Priority highestPriority();
        void enqueue(const Data &d, const Priority &p = Priority());
        Data dequeue();

        queue<Data,Priority>& operator<<(const Data &d);
        queue<Data,Priority>& operator>>(Data &d);

        template<typename D, typename P>
        friend
        ostream& operator<<(ostream &out, const queue<D,P> &q);

        template<typename D, typename P>
        friend
        istream& operator>>(istream &in, queue<D,P> &q);

    private:
        size_t mySize, myCapacity, first, last;
        node<Data, Priority> *list;

        void copy(const queue<Data,Priority> &other);
        size_t nextIndex(size_t index);
};

template<typename Data, typename Priority>
queue<Data,Priority>::queue(size_t c)
{
    // Creates a new array based on capacity
    list = new node<Data, Priority>[c];

    // Set my capacity to equal c
    myCapacity = c;

    // first and mySize are set to zero
    first = mySize = 0;

    // last is set to -1
    last = -1;
}

template<typename Data, typename Priority>
queue<Data,Priority>::~queue()
{
    mySize = myCapacity = 0;
    delete list;
}

template<typename Data, typename Priority>
queue<Data,Priority>::queue(const queue<Data,Priority> &other)
{
    copy(other);
}

template<typename Data, typename Priority>
queue<Data,Priority>& queue<Data,Priority>::operator=(const queue<Data,Priority> &other)
{
    if(this != &other)
    {
        copy(other);
    }
    return *this;
}


template<typename Data, typename Priority>
bool queue<Data,Priority>::full()
{
    return mySize == myCapacity;
}

template<typename Data, typename Priority>
size_t queue<Data,Priority>::nextIndex(size_t index)
{
    // This is a helper function that makes our array circular
    // it returns the next index and effectively is the same as
    // index + 1
    // if (index == Capacity) return 0

    return (index + 1) % myCapacity;
}

template<typename Data, typename Priority>
bool queue<Data,Priority>::empty()
{
    return !mySize;
}

template<typename Data, typename Priority>
bool queue<Data,Priority>::search(Data &d)
{
    for(size_t i = first; i != last; i = nextIndex(i))
        if(list[i].data == d)
            return true;

    return false;
}

template<typename Data, typename Priority>
Priority queue<Data,Priority>::highestPriority()
{
    return list[first].priority + 1;
}

template<typename Data, typename Priority>
size_t queue<Data,Priority>::size()
{
    return mySize;
}

template<typename Data, typename Priority>
size_t queue<Data,Priority>::capacity()
{
    return myCapacity;
}

template<typename Data, typename Priority>
void queue<Data,Priority>::resize(size_t newSize)
{
    if(myCapacity < newSize)
        myCapacity = newSize;
    else
    {
        cout << "Q_BAD_SIZE. Cannot resize." << endl;
        throw Q_BAD_SIZE;
    }
}

template<typename Data, typename Priority>
Data queue<Data,Priority>::peek()
{
    if(empty())
    {
        cout << "Q_EMPTY. Cannot peek." << endl;
        throw Q_EMPTY;
    }

    return list[first].data;
}

template<typename Data, typename Priority>
queue<Data,Priority>& queue<Data,Priority>::operator<<(const Data &d)
{
    enqueue(d);
    return *this;
}

template<typename Data, typename Priority>
queue<Data,Priority>& queue<Data,Priority>::operator>>(Data &d)
{
    Priority pri = Priority();
    d = dequeue();
    return *this;
}

template<typename Data, typename Priority>
void queue<Data,Priority>::enqueue(const Data &d, const Priority &p)
{
    if(full())
    {
        cout << "Q_FULL. Cannot enqueue." << endl;
        throw Q_FULL;
    }

    last = nextIndex(last);

    if(empty())
        list[first] = node<Data, Priority>(d, p);

    else
    {
        list[last] = node<Data, Priority>(d, p);

        for(size_t i = first; i != last; i = nextIndex(i))
            for(size_t j = first; j != last; j = nextIndex(j))
                if(list[i].priority < list[j].priority)
                    std::swap(list[i], list[j]);
    }

    mySize++;

}

template<typename Data, typename Priority>
Data queue<Data,Priority>::dequeue()
{
    if(empty())
    {
        cout << "Q_EMPTY. Cannot DeQueue" << endl;
        throw Q_EMPTY;
    }

    node<Data, Priority> temp = list[first];

    first = nextIndex(first);

    mySize--;

    return temp.data;

}

template<typename Data, typename Priority>
void queue<Data,Priority>::copy(const queue<Data,Priority> &other)
{
    mySize = other.mySize;
    myCapacity = other.myCapacity;
    first = other.first;
    last = other.last;
    for(size_t i = first; i != last; i = nextIndex(i))
        enqueue(other.list[i].data, other.list[i].priority);
}

template<typename D, typename P>
ostream& operator<<(ostream &out, const queue<D,P> &q)
{
    node<D,P> displayNode;
    size_t in = 0;

    while(in < q.mySize)
        {
            displayNode = q.list[in];

            out << "Data: " << displayNode.data;
            out << " Priority: " << displayNode.priority ;

            out << endl;

            in++;
        }

    return out;
}

template<typename D, typename P>
istream& operator>>(istream &in, queue<D,P> &q)
{
    string word;
    string wordTwo;

    D data;
    P pri;

    // Grab Data
    getline(in, word, ' ');

    in >> data;

    if(in.peek() == 'P')
    {
        getline(in, wordTwo, ' ');

        in >> pri;
    }

    q.enqueue(data, pri);

    return in;
}

template<>
istream& operator>>(istream &in, queue<char*,int> &q)
{
    string word;
    string wordTwo;

    char data[256];
    int pri;

    // Grab Data
    getline(in, word, ' ');

    in >> data;

    in >> ws;

    if(in.peek() == 'P')
    {
        getline(in, wordTwo, ' ');

        in >> pri;
    }

    if(data != "")
        q.enqueue(data, pri);

    return in;
}

template<>
istream& operator>>(istream &in, queue<string,int> &q)
{
    string word;
    string wordTwo;

    string data;
    int pri;

    // Grab Data
    getline(in, word, ' ');

    getline(in, data, ' ');

    in >> ws;

    if(in.peek() == 'P')
    {
        getline(in, wordTwo, ' ');

        in >> pri;
    }

    if(data != "")
        q.enqueue(data, pri);

    return in;
}


#endif // QUEUES_H
