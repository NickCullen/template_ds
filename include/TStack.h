#ifndef TSTACK_H
#define TSTACK_H

/* Definitions and macros */
#ifndef NULL
#define NULL 0
#endif 

/**
* Node that contains the data and is stored on the stack
*/

template<typename T>
struct TStackNode
{
	T _data; /**< The data this node is holding */

	TStackNode<T>* _prev; /**< Pointer to the previous stack node (NULL if we are the first (bottom) item on the stack) */

	/**
	* Default constructor of the node which sets _prev pointer 
	* to NULL
	*/
	TStackNode()
	{
		_prev = NULL;
	}
};



/**
* A template made stack to hold specified data
*/

template<typename T>
class TStack
{
private:
	TStackNode<T>* _top; /**< Pointer to the top of the stack */
	
	int _count; /**< Number of items on the stack */
public:
	/**
	* Default constructor of the stack
	*/
	TStack()
	{
		_top = NULL;
		_count = 0;
	}

	/**
	* Default destructor of the stack
	*/
	~TStack()
	{
		//empty the stack
		Empty();
	}

	/**
	* Call to clear all nodes (will leave data untouched) off 
	* the stack
	*/
	void Empty()
	{
		while (!IsEmpty())
			Pop();
	}

	/**
	* Returns true if the stack is empty false if it contains
	* 1 or more items
	* @return Boolean
	*/
	inline bool IsEmpty()
	{
		return !_count;
	}

	/**
	* Returns however many items are on the stack
	* @return Integer
	*/
	inline int Count()
	{
		return _count;
	}

	/** 
	* Pops an item of the stack and returns the data 
	* @return The data type (will be NULL if there were no nodes to remove)
	*/
	T Pop()
	{
		//the data to return
		T ret = T();

		if (_top != NULL)
		{
			//store the top in a temp var
			TStackNode<T>* tmp = _top;
			//set what to return
			ret = tmp->_data;
			//set the top to be the previous node (will set to NULL if it just popped the last element)
			_top = _top->_prev;

			//delete tmp
			delete(tmp);

			//deduct count 
			_count--;
		}

		//return it
		return ret;
	}

	/**
	* Pushes the specified data onto the stack by adding it to the top of the stack
	* @param data The data to be pushed onto the stack
	*/
	void Push(T data)
	{
		//if there are already items on the list
		if (_top != NULL)
		{
			//append a new node and set its data
			TStackNode<T>* new_top = new TStackNode<T>();

			//set its data
			new_top->_data = data;

			//set the previous node for new_top (i.e. the current _top)
			new_top->_prev = _top;
			
			//now make _top the new node
			_top = new_top;
		}
		else
		{
			//simply create a new node for _top
			_top = new TStackNode<T>();

			//assign data
			_top->_data = data;
		}

		//increment count
		_count++;
	}

	/**
	* See what is on the top of the stack but will leave the stack untouched
	* @return Returns the data that is on the top of the stack (can be NULL if stack is empty)
	*/
	T Peek()
	{
		T ret = T();
		//if there is an item on stack set data in ret and return it
		if (_top != NULL)
		{
			ret = _top->_data;
		}

		return ret;
	}
};

#endif