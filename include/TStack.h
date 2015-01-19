#ifndef TSTACK_H
#define TSTACK_H

/* Definitions and macros */
#ifndef NULL
#define NULL 0
#endif 

/* Nodes to store in the TStack */
template<typename T>
struct TStackNode
{
	//the data 
	T _data;

	//all a stack nodes needs is a pointer to previous node
	TStackNode<T>* _prev;

	//ctor
	TStackNode()
	{
		_prev = NULL;
	}
};/* End of TStackNode */



/* Templated class for stack */
template<typename T>
class TStack
{
private:
	//top of the stack
	TStackNode<T>* _top;
	
	//number of nodes
	int _count;
public:
	//ctor
	TStack()
	{
		_top = NULL;
		_count = 0;
	}
	//dtor
	~TStack()
	{
		//empty the stack
		Empty();
	}

	//empty the stack
	void Empty()
	{
		while (!IsEmpty())
			Pop();
	}

	//returns true if no items on stack
	inline bool IsEmpty()
	{
		return !_count;
	}

	//returns number of nodes
	inline int Count()
	{
		return _count;
	}

	//pops a stack node from the stack
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

	//adds an item on top of the stack
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

	//peeks at the current data
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
}; /* End of TStack */

#endif