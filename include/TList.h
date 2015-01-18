#ifndef TLIST_H
#define TLIST_H

#include <stdio.h>

/* Definitions and macros */
#ifndef NULL
#define NULL 0
#endif 

/* foreach macro */
#define TLIST_foreach(Type, name, in_list) for (TListIter<Type> name = TListIter<Type>(&in_list); !name.IsFinished(); name.Next())
#define TLIST_rev_foreach(Type, name, in_list) for (TListIter<Type> name = TListIter<Type>::EndOf(&in_list); !name.IsFinished(); name.Prev())

/* foreach macro when a list is being used as a pointer */
#define TLISTPTR_foreach(Type, name, in_list) for (TListIter<Type> name = TListIter<Type>(in_list); !name.IsFinished(); name.Next())
#define TLISTPTR_rev_foreach(Type, name, in_list) for (TListIter<Type> name = TListIter<Type>::EndOf(in_list); !name.IsFinished(); name.Prev())

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
};/* End of TListNode */



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

	//getter for head
	inline TListNode<T>* Head()
	{
		return _head;
	}

	//getter for top
	inline TListNode<T>* Top()
	{
		return _top;
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

	

}; /* End of TList */


/*Templated class to iterate over a TList*/
template<typename T>
class TListIter
{
private:
	//the list to iterate over
	TList<T>* _list;

	//the current list node
	TListNode<T>* _current;
public:
	//default ctor (should not be used)
	TListIter()
	{
		_list = NULL;
		_current = NULL;
	}

	//dtor
	~TListIter()
	{
		_list = NULL;
		_current = NULL;
	}

	/* this is the ctor that should be used to instantiate a iter */
	TListIter(TList<T>* list)
	{
		//set let
		_list = list;
		//get the head
		_current = _list->Head();
	}

	/* returns true if the iterator is finished iterating through list */
	inline bool IsFinished()
	{
		return (_current == NULL);
	}

	/* methods for getting the next node */
	TListIter<T> operator++()
	{
		return Next();
	}
	TListIter<T> operator++(int)
	{
		TListIter<T> tmp(*this); // copy
		operator++(); // pre-increment
		return tmp;   // return old value
	}
	inline TListIter<T> Next()
	{
		// actual increment takes place here
		if (_current != NULL)
		{
			_current = _current->_next;
		}
		return (*this);
	} /* End of methods for incrementing */


	/* methods for getting the prev node */
	TListIter<T> operator--()
	{
		return Prev();
	}
	TListIter<T> operator--(int)
	{
		TListIter<T> tmp(*this); // copy
		operator--(); // pre-decrement
		return tmp;   // return old value
	}
	inline TListIter<T> Prev()
	{
		// actual decrement takes place here
		if (_current != NULL)
		{
			_current = _current->_prev;
		}
		return (*this);
	} /* End of methods for decrementing */

	/* Functions for referencing the data that the current node holds */
	//itr.Value()
	inline T Value()
	{
		T ret = T();
		if (_current != NULL)
		{
			ret = _current->_data;
		}
		return ret;
	}
	//itr->
	T operator->()
	{
		return Value();
	} 
	//(*itr)
	T operator*()
	{
		return Value();
	}
	//(T)itr     (i.e. cast operator)
	operator T()
	{
		return Value();
	}/* End of accessing functions */

	/* Static function to create an iterator at the start of a list */
	static TListIter<T> StartOf(TList<T>* list)
	{
		//create a TLIstIter (will default to start of list anyway)
		TListIter<T> start = TListIter<T>(list);
		return start;
	}

	/* Static function to create an iterator at the end of a list */
	static TListIter<T> EndOf(TList<T>* list)
	{
		//create a TLIstIter 
		TListIter<T> end = TListIter<T>(list);
		//set its current to equal the top of the list
		end._current = list->Top();

		return end;
	}
};/* End of TListIter */


#endif