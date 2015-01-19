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

/* foreach macro */
#define TLIST_foreach(Type, name, in_list) for (TListIter<Type> name = TListIter<Type>(&in_list, false, true); !name.IsFinished(); name.Next())
#define TLIST_rev_foreach(Type, name, in_list) for (TListIter<Type> name = TListIter<Type>(&in_list, true, true); !name.IsFinished(); name.Prev())

/* foreach macro when a list is being used as a pointer */
#define TLISTPTR_foreach(Type, name, in_list) for (TListIter<Type> name = TListIter<Type>(&in_list, false, true); !name.IsFinished(); name.Next())
#define TLISTPTR_rev_foreach(Type, name, in_list) for (TListIter<Type> name = TListIter<Type>(&in_list, true, true); !name.IsFinished(); name.Prev())

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

	//remove type from list
	bool Remove(T instance)
	{
		//first find it
		TListNode<T>* cur = _head;
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

			//if the node is the end then we are removing the first item
			if (node == _head) _head = _head->_next;

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
	bool Remove(TListIter<T> itr)
	{
		//raise error if trying to remove an iterator in a foreach loop!
		if (itr._foreach)
		{
			printf("ERROR removing an iterator in a foreach loop - this is not allowed!\n");
			return false;
		}

		//remove
		return Remove(itr._current);
	}

}; /* End of TList */


/*Templated class to iterate over a TList*/
template<typename T>
class TListIter
{
	template<typename T>
	friend class TList;
private:
	//the current list node
	TListNode<T>* _current;

	//set to true when used in a foreach loop
	bool _foreach;

public:
	//default ctor (should not be used)
	TListIter()
	{
		_current = NULL;
		_foreach = false;
	}

	//dtor
	~TListIter()
	{
		_current = NULL;
	}

	/* this is the ctor that should be used to instantiate a iter */
	TListIter(TList<T>* list, bool end, bool used_in_foreach = false)
	{
		//get the head
		_current = end ? list->Top() : list->Head();

		//set if we are using this iterator in a foreach loop or not
		_foreach = used_in_foreach;
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