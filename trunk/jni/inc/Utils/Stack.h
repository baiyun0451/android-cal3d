#ifndef CJ_STACK_H
#define CJ_STACK_H

template <class T>
struct node
{
	node():prevNode(0){}
	T data;
	node * prevNode;
};

template <class T>
class Stack
{
	public:
		Stack():list(0),count(0){}
		void push(T d);
		void pop();
		void getCount();
		T top();
		bool isEmpty();
		void clear();
		~Stack();
	private:
		int count;
		node<T> * list;

};

template <class T>
void Stack<T>::push(T d)
{
	node<T> * temp = new node<T>;
	temp->data = d;
	temp->prevNode = list;
	list = temp;
	count++;
}
template <class T>
void Stack<T>::pop()
{
	if(list != 0)
	{
		node<T> * temp = list;
		list = temp->prevNode;
		delete temp;
		count--;
	}
}
template <class T>
void Stack<T>::getCount()
{
	return count;
}
template <class T>
T Stack<T>::top()
{
	if(list != 0)
		return list->data;
	return 0;
}
template <class T>
bool Stack<T>::isEmpty()
{
	return (list == 0);
}
template <class T>
void Stack<T>::clear()
{
	while(!isEmpty())
	{
		pop();
	}
}
template <class T>
Stack<T>::~Stack()
{
	clear();
}
#endif
