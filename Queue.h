#pragma once
#include <iostream>
using namespace std;
template <class typ>
class qNode
{
public:
	qNode<typ>* next;
	typ data;
	qNode();
	qNode(typ& data);
	~qNode();
};

template <class typ>
qNode<typ>::qNode()
{
	this->next = nullptr;
	//this->data = 0;
}
template<class typ>
qNode<typ>::qNode(typ& data)
{
	this->data = data;
	next = nullptr;
}
template <class typ>
qNode<typ>::~qNode()
{
}


template <class typ>
class Queue
{
public:
	qNode<typ>* head;
	qNode<typ>* tail;
	int size;
	Queue();
	void enqueue(typ& data);
	void dequeue();
	typ front();
	void clear();
	bool isEmpty();
	void display();
	void operator=(const Queue<typ>& obj);
	typ operator[](int idx);
	~Queue();
};

template<class typ>
Queue<typ>::Queue()
{
	head = nullptr;
	tail = nullptr;
	size = 0;
	//cursor = nullptr;
}

template<class typ>
void Queue<typ>::enqueue(typ& data)
{
	qNode<typ>* n1 = new qNode<typ>(data);

	if (tail == nullptr && head == nullptr)
	{
		head = n1;
		tail = n1;
		//cursor = n1;
	}
	else
	{
		tail->next = n1;
		tail = tail->next;
		//cursor = n1;
	}
	size++;
}
template<class typ>
void Queue<typ>::dequeue()
{
	if (head != nullptr)
	{
		typ tempdata = head->data;
		qNode<typ>* tempqNode = head;
		head = head->next;
		delete tempqNode;
		size--;
		if (head == nullptr)
			tail = nullptr;
	}
}

template<class typ>
inline typ Queue<typ>::front()
{
	if (this->isEmpty() == false)
	{
		qNode<typ>* tempqNode = head;
		return tempqNode->data;
	}
}

template<class typ>
void Queue<typ>::clear()
{
	if (head != nullptr)
	{
		qNode<typ>* prev = head;
		qNode<typ>* cursor = head;
		while (cursor != nullptr)
		{
			cursor = cursor->next;
			delete prev;
			prev = cursor;
		}
		head = nullptr;
		size = 0;
	}
	else
		cout << "List is empty\n";
}

template<class typ>
bool Queue<typ>::isEmpty()
{
	if (head == nullptr)
		return true;
	else
		return false;
}

template<>
void Queue<Vector<string>>::display()
{
	if (this->isEmpty())
	{
		cout << "Queue is Empty";
		return;
	}
	qNode<Vector<string>>* temp = this->head;
	while (temp != nullptr)
	{
		temp->data.display();
		temp = temp->next;
		cout << endl;
	}
}

template<class typ>
void Queue<typ>::operator=(const Queue<typ>& obj)
{
	this->clear();
	//this->size = obj.size;
	qNode<typ>* cursor = obj.head;
	while (cursor != nullptr)
	{
		this->enqueue(cursor->data);
	}
}

template<class typ>
inline typ Queue<typ>::operator[](int idx)
{
	if (this->isEmpty() == false)
	{
		if (idx >= size)
		{
			cout << "Index out of bound\n";
			exit(0);
		}
		qNode<typ>* cursor = this->head;
		int i = 0;
		while (i != idx)
		{
			cursor = cursor->next;
			i++;
		}

		return cursor->data;
	}
}

template<class typ>
Queue<typ>::~Queue()
{

}