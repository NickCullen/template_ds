#ifndef TLIST_H
#define TLIST_H

/* Definitions and macros */
#ifndef NULL
#define NULL 0
#endif 

/* Nodes to store in the TList */
template<typename T>
struct TListNode
{
	//the data 
	T _data;

	//the next and previous nodes
	TListNode<T>* _next;
	TListNode<T>* _prev;

	//ctor
	TListNode()
	{
		_next = _prev = NULL;
	}
};

/* Templated class for linked list */
template<typename T>
class TList
{
private:
	//head of the list
	TListNode<T>* _head;
	
	//the end of the list
	TListNode<T>* _top;
	
	//number of nodes
	int _count;
public:
	//ctor
	TList()
	{
		_head = _top = NULL;
		_count = 0;
	}
	//dtor
	~TList()
	{
		//empty the list
		Empty();
	}

	//empty the list
	void Empty()
	{
		while (!IsEmpty())
			PopBack();
	}

	//returns true if no items on list
	inline bool IsEmpty()
	{
		return (_head == NULL);
	}

	//returns number of nodes
	inline int Count()
	{
		return _count;
	}

	//pops the list node on the list
	T PopBack()
	{
		//the data to return
		T ret = T();

		if (_top != NULL)
		{
			//store the top in a temp var
			TListNode<T>* tmp = _top;
			//set what to return
			ret = tmp->_data;
			//set the top to be the previous node
			_top = _top->_prev;
			//if this was not the last one then set its next to null
			if (_top != NULL)
				_top->_next = NULL;
			//else also set head to NULL
			else
				_head = NULL;
			//delete tmp
			delete(tmp);

			//deduct count 
			_count--;
		}

		//return it
		return ret;
	}

	//adds an item to the end of the list
	void PushBack(T data)
	{
		//if we are appending to the end
		if (_top != NULL)
		{
			//append a new node and set its data
			_top->_next = new TListNode<T>();
			_top->_next->_data = data;

			//set the new nodes previous to be the top
			_top->_next->_prev = _top;
			
			//now make the top the new node
			_top = _top->_next;
		}
		//or to the head of the stack
		else
		{
			//create a new node and set its data
			_top = new TListNode<T>();
			_top->_data = data;
			//also remember to set the head. note we dont need to worry about next/prev pointers
			_head = _top;
		}

		//increment count
		_count++;
	}
};

#endif