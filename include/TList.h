#ifndef TLIST_H
#define TLIST_H

/* Forward Decl */
template<typename T>
class TList;
template<typename T>
class TListIter;

/* Definitions and macros */
#ifndef NULL
#define NULL 0
#endif 

/**
* Macro to iterate over the list like a foreach loop when the list is not a pointer
*/
#define TLIST_foreach(Type, name, in_list) for (TListIter<Type> name = TListIter<Type>(&in_list); !name.IsFinished(); name.Next())

/**
* Macro to iterato over the list like a foreach loop when the list is a pointer
*/
#define TLISTPTR_foreach(Type, name, in_list) for (TListIter<Type> name = TListIter<Type>(&in_list); !name.IsFinished(); name.Next())

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
		_count = 0;

		//instantiate _head (z node)
		_head = new TListNode<T>();

		//point _top to head
		_top = _head;
	}
	//dtor
	~TList()
	{
		//empty the list
		Empty();

		//delete _head
		delete(_head);
	}

	//rturns first item
	inline TListNode<T>* FirstItem()
	{
		return _head->_next;
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
		return !_count;
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

		if (!IsEmpty())
		{
			//store the top in a temp var
			TListNode<T>* tmp = _top;
			//set what to return
			ret = tmp->_data;
			//set the top to be the previous node
			_top = _top->_prev;
			//set next to null
			_top->_next = NULL;

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
		//append a new node and set its data
		_top->_next = new TListNode<T>();
		_top->_next->_data = data;

		//set the new nodes previous to be the top
		_top->_next->_prev = _top;
			
		//now make the top the new node
		_top = _top->_next;

		//increment count
		_count++;
	}

	//remove type from list
	bool Remove(T instance)
	{
		//start at head->next (remember head is just a false node)
		TListNode<T>* cur = _head->_next;
		while (cur)
		{
			//if not what we are looking for get next else break
			if (cur->_data != instance)
				cur = cur->_next;
			else
				break;
		}

		//remove node
		return Remove(cur);
	}

	//removes a node from the list
	bool Remove(TListNode<T>* node)
	{
		bool ret = false;
		//make sure it isnt a null node
		if (node != NULL)
		{
			//store prev and next
			TListNode<T>* prev = node->_prev;
			TListNode<T>* next = node->_next;

			//if the node is the top we are removing the end of the list
			if (node == _top) _top = _top->_prev;

			//link prev to next and vis versa
			if(prev) prev->_next = next;
			if(next) next->_prev = prev;

			//delete node
			delete(node);

			//deduct count
			_count--;

			//return true
			ret = true;
		}

		return ret;
	}

	//remove iterator from list (this is implemented at bottom of header file for linking issues
	bool Remove(TListIter<T>& itr)
	{
		TListIter<T> tmp(itr);
		tmp.Prev();

		//remove and cache the return flag
		bool ret = Remove(itr._current);

		itr = tmp;

		//remove
		return ret;
	}

}; /* End of TList */


/*Templated class to iterate over a TList*/
template<typename T>
class TListIter
{
	friend class TList<T>;
private:
	//the current list node
	TListNode<T>* _current;

public:
	//default ctor (should not be used)
	TListIter()
	{
		_current = NULL;
	}

	//dtor
	~TListIter()
	{
		_current = NULL;
	}

	/* this is the ctor that should be used to instantiate a iter */
	TListIter(TList<T>* list)
	{
		//get the head
		_current = list->FirstItem();

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

};/* End of TListIter */

#endif