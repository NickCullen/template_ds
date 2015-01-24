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

/**
* Structure which will be stored in the list so we can iterator through the
* elements (which will also be held by this)
*/

template<typename T>
struct TListNode
{
	T _data; /**< The data this node stored */

	TListNode<T>* _next; /**< Pointer to the next node */
	TListNode<T>* _prev; /**< Pointer to the previous node */

	/**
	* Default constructor for TListNode will init all 
	* Pointers to NULL
	*/
	TListNode()
	{
		_next = _prev = NULL;
	}
};



/**
* The template list data structure
*/

template<typename T>
class TList
{
	friend class TListIter<T>;

private:
	TListNode<T>* _head; /**< The head of the list (note that although this has been allocated memory the actual start of the list is at _head->_next) */
	
	TListNode<T>* _top; /**< The last inserted element (may point to _head if list is empty */
	
	int _count; /**< The count of how many elements are stored on the list */

	/**
	* Returns the first node on the list and is only used
	* internally for adding and removing nodes
	* @return Pointer to the first node the _head points to (can be NULL)
	*/
	inline TListNode<T>* FirstNode()
	{
		return _head->_next;
	}
	
	/**
	* Returns the top node of the list 
	* @return returns whatever top is pointing to (will return NULL if it points to head)
	*/
	inline TListNode<T>* Top()
	{
		return _too != _head ? _top : NULL;
	}
public:
	/** 
	* The constructor for the list will allocate memory to _head and set _top and _count 
	* to NULL
	*/
	TList()
	{
		_count = 0;

		//instantiate _head (z node)
		_head = new TListNode<T>();

		//point _top to head
		_top = _head;
	}
	
	/**
	* The destructor for TTList will empty the list (delete nodes
	* and leave data untouched) will also delete _head
	*/
	~TList()
	{
		//empty the list
		Empty();

		//delete _head
		delete(_head);
	}


	/**
	* Call to empty the contents of the list (note this will delete
	* only the list nodes and leave data untouched
	*/
	void Empty()
	{
		while (!IsEmpty())
			PopBack();
	}

	/**
	* Returns true if the list is empty and false if not
	* @return Boolean
	*/
	inline bool IsEmpty()
	{
		return !_count;
	}

	/**
	* Returns the number of items on the list
	* @return integer count
	*/
	inline int Count()
	{
		return _count;
	}

	/** 
	* Pops the last element off the list and also returns the data
	* @return The data of the element that was popped off the list (will be NULL if list was empty)
	*/
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

	/**
	* Pushes the specified data onto the back of the list
	* @param data The data to be pushed onto the end of the list
	*/
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

	/**
	* Removes the spcified data off the list
	* @param instance The data to remove off the list
	* @return Will return true if it got removed false if the element didnt exist
	*/
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

	/**
	* Removes the spcified node off the list
	* @param node The node to remove off the list
	* @return Will return true if it got removed false if the node didnt exist
	*/
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

	/**
	* Removes the spcified iterator off the list (note that this will move the iterator back one place 
	* to be used with the foreach loop of the TList
	* @param itr The iterator to remove off the list
	* @return Will return true if it got removed false if the element didnt exist
	*/
	bool Remove(TListIter<T>& itr)
	{
		TListIter<T> tmp(itr);
		tmp.Prev();

		//remove and cache the return flag
		bool ret = Remove(itr._current);

		if(ret) itr = tmp;

		//remove
		return ret;
	}

}; 


/**
* The TListIter is used to iterate over elements
* of the TList can be used manually or see the 
* TLIST_foreach macro
*/

template<typename T>
class TListIter
{
	friend class TList<T>;
private:
	TListNode<T>* _current; /**< Pointer to the node this iterator is currently at */

public:
	/**
	* Default constructor will init _current to NULL
	*/
	TListIter()
	{
		_current = NULL;
	}

	/**
	* Default destructor will set _current to NULL
	*/
	~TListIter()
	{
		_current = NULL;
	}

	/**
	* Overloaded constructor to pass a pointer of the list
	* we want to iterate over
	* @param list Pointer to the list we want to iterate over
	*/
	TListIter(TList<T>* list)
	{
		//get the head
		_current = list->FirstNode();

	}

	/**
	* Used to check if the iterator has finished iterating 
	* over the list
	* @return Boolean
	*/
	inline bool IsFinished()
	{
		return (_current == NULL);
	}

	/**
	* Overloaded ++ (pre) operator to get the next element in the list
	* @return This iterator +1
	*/
	TListIter<T> operator++()
	{
		return Next();
	}

	/**
	* Overloaded ++ (post) operator to get the next element in the list
	* @return This iterator
	*/
	TListIter<T> operator++(int)
	{
		TListIter<T> tmp(*this); // copy
		operator++(); // pre-increment
		return tmp;   // return old value
	}

	/**
	* Called to set _current to the next item in the list
	* @return This iterator +1
	*/
	inline TListIter<T> Next()
	{
		// actual increment takes place here
		if (_current != NULL)
		{
			_current = _current->_next;
		}
		return (*this);
	}

	/**
	* Overloaded -- (pre) operator to get the previous element in the list
	* @return This iterator -1
	*/
	TListIter<T> operator--()
	{
		return Prev();
	}

	/**
	* Overloaded -- (post) operator to get the prev element in the list
	* @return This iterator
	*/
	TListIter<T> operator--(int)
	{
		TListIter<T> tmp(*this); // copy
		operator--(); // pre-decrement
		return tmp;   // return old value
	}

	/**
	* Called to set _current to the previous item in the list
	* @return This iterator -1
	*/
	inline TListIter<T> Prev()
	{
		// actual decrement takes place here
		if (_current != NULL)
		{
			_current = _current->_prev;
		}
		return (*this);
	}

	/**
	* Gets the data stored in _current (note can be NULL if at the end of the list
	* @return The value of the data stored in _current (can be NULL)
	*/
	inline T Value()
	{
		T ret = T();
		if (_current != NULL)
		{
			ret = _current->_data;
		}
		return ret;
	}
	
	/**
	* Overloaded -> operator to use member functions on the current data if the data is of
	* class type (e.g. itr->MyMemberFunc())
	* @return The Data (Can be NULL)
	*/
	T operator->()
	{
		return Value();
	} 
	
	/**
	* Overloaded operator to de-reference the iterator to the stored data
	* (e.g. (*itr).MyMemberFunc();
	* @return The Data (Can be NULL)
	*/
	T operator*()
	{
		return Value();
	}
	
	/**
	* Overloaded cast operator to cast the iterator into the data type
	* (e.g. MyClass* instance = (MyClass*)itr)
	* @return The Data (Can be NULL)
	*/
	operator T()
	{
		return Value();
	}
};
#endif