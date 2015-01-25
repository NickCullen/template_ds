#ifndef TTREE_H
#define TTREE_H

/* Include for TStack */
#include "TStack.h"

/* Forward Decl */
template<typename T> class TTreeIter;

/* Definitions */
#ifndef NULL
#define NULL 0
#endif

/**
* Macro to delete a pointer and set it to null
*/
#ifndef SAFE_DELETE
#define SAFE_DELETE(p) delete(p); p=NULL;
#endif

/**
* Macro to iterate over a tree (that is not a pointer)
*/
#define TTREE_foreach(Type, name, in_tree) for (TTreeIter<Type> name = TTreeIter<Type>(&in_tree); !name.IsFinished(); name.Next())

/**
* Macro to iterate over a tree (that is a pointer)
*/
#define TTREEPTR_foreach(Type, name, in_tree) for (TTreeIter<Type> name = TTreeIter<Type>(&in_tree); !name.IsFinished(); name.Next())

/**
* A node that is stored in the binary tree containing the data 
* and pointer to its parent and left right sibling nodes
*/

template<typename T>
struct TTreeNode
{
	T _data; /**< The data that this node stores */

	TTreeNode<T>* _left; /**< The child node that contains data of a value less than _data (may be NULL) */
	TTreeNode<T>* _right; /**< The child node that contains data of a value greater or equal to _data (may be NULL) */

	TTreeNode<T>* _parent; /**< The parent node of this node (May be NULL if root) */

	/**
	* The default constructor of TTreeNode which initilizes all pointers to NULL
	*/
	TTreeNode()
	{
		_left = _right = _parent = NULL;
	}

	/**
	* Default destructor 
	*/
	~TTreeNode()
	{
		_left = _right = _parent = NULL;
	}
}; 


/**
* A templated binary tree which must have a comparison function set
* when initlizing the tree so it can be used correctly. This can be done
* by either passing the comparison function into the constructor of by 
* using the SetComparisonFunc method
*/

template<typename T>
class TTree
{
	friend class TTreeIter<T>;
private:
	TTreeNode<T>* _root; /**< The root of the tree */

	int _count; /**< The number of nodes currently stored in this true */

	int(*_comparison)(T, T); /**< A Pointer to the specified comparison function (must not be a method of a class) the function should return -1 if lhs < rhs, 0 if lhs == rhs or 1 if lhs > rhs */

	/**
	* To be used internally when removing a node from the tree which has two sibling nodes
	* it will find the smallest node in the subtree starting at root)
	* @param root The node at the root of the current sub binary tree
	*/
	TTreeNode<T>* FindSmallestFromNode(TTreeNode<T>* root)
	{
		//keep going left from the tree untill its null
		while (root->_left != NULL) 
			root = root->_left;
		return root;
	}

protected:
	/**
	* To be used internally to delete a node from the tree. This is a 
	* recursive method
	* @param root The node to be removed from the tree
	* @param data The data to be removed (required for compairson checking)
	* @return Returns the new value for root
	*/
	TTreeNode<T>* DeleteNode(TTreeNode<T>* root, T data)
	{
		//return null
		if (root == NULL) 
			return root;

		//run comparison
		int result = _comparison(data, root->_data);

		// -1 so data is less then traverse left
		if (result < 0)
			root->_left = DeleteNode(root->_left, data);

		//same as above except result == 1 so traversse right
		else if (result > 0)
			root->_right = DeleteNode(root->_right, data);

		//The node has been found so we can remove it
		else
		{
			// Case 1:  No child - just delete it
			if (root->_left == NULL && root->_right == NULL) 
			{
				SAFE_DELETE(root);
				_count--;
			}

			//Case 2: One child set the parents right child pointer to right
			else if (root->_left == NULL) 
			{
				TTreeNode<T>* temp = root;
				root = root->_right;
				root->_parent = temp->_parent;
				SAFE_DELETE(temp);
				_count--;
			}
			//same as above but swapping left pointer
			else if (root->_right == NULL) 
			{
				TTreeNode<T>* temp = root;
				root = root->_left;
				root->_parent = temp->_parent;
				SAFE_DELETE(temp);
				_count--;
			}
			// case 3: 2 children - need to reorder right subtree
			else 
			{
				TTreeNode<T>* temp = FindSmallestFromNode(root->_right);
				root->_data = temp->_data;
				root->_right = DeleteNode(root->_right, temp->_data);
			}
		}
		return root;
	}

public:
	/**
	* Default constructor of the Template tree 
	*/
	TTree()
	{
		_root = NULL;
		_comparison = NULL;
		_count = 0;
	}

	/**
	* Overloaded constructor which will take and set the comparison function 
	* used to insert and compare objects on the tree
	* @param ComparisonFunc The pointer to the comparison function (note that this CANNOT be a class member function unless it is static)
	*/
	TTree(int (*ComparisonFunc)(T, T))
	{
		_root = NULL;
		_comparison = NULL;
		_count = 0;

		SetComparisonFunc(ComparisonFunc);
	}

	/** 
	* Default destructor which will empty the tree
	*/
	virtual ~TTree()
	{
		//empty list
		Empty();
	}

	/**
	* Sets the comparison function
	* @param ComparisonFunc The pointer to the comparison function (note that this CANNOT be a class member function unless it is static)
	*/
	void SetComparisonFunc(int(*ComparisonFunc)(T, T))
	{
		//set the comparison func
		_comparison = ComparisonFunc;
	}

	/**
	* returns how many nodes are currently stored in the tree
	* @return Integer
	*/
	inline int Count()
	{
		return _count;
	}

	/**
	* Returns true if the tree is empty or false otherwise
	* @return Integer
	*/
	inline bool IsEmpty()
	{
		return !_count;
	}

	/** 
	* Function to call to empty (delete all nodes (not the data)) in the tree
	*/
	void Empty()
	{
		while (!IsEmpty())
		{
			_root = DeleteNode(_root, _root->_data);
		}
	}

	
	/**
	* Insert the specified data into the tree
	* @param data The data to be inserted 
	*/
	virtual void Insert(T data)
	{
		//start at _root
		TTreeNode<T>* cur = _root, *prev = _root;

		//compare the data being iserted with the data in the current node
		//to determine if we should go left or right in the tree and remember (declare outside while loop)
		//so we can check if we went left or right when inserting new node)
		int result = 0;

		//get the next available node
		while (cur != NULL)
		{
			//run comparison
			result = _comparison(data, cur->_data);

			//set prev
			prev = cur;

			//get next node (if -1 got left if 0 or 1 go right)
			cur = result <= -1 ? cur->_left : cur->_right;
		}

		//we are at the next available node
		cur = new TTreeNode<T>();

		//set data
		cur->_data = data;

		//if we are not inserting into the root
		if (prev)
		{
			//set parent 
			cur->_parent = prev;

			//set the previous left/right pointer
			if (result <= -1)
				prev->_left = cur;
			else
				prev->_right = cur;
		}
		//else set root
		else
		{
			_root = cur;
		}

		_count++;
	}

	/**
	* This function can be used to find a specific object on the tree by specifying a callback
	* search function (just like a comparison function) with the exception that the lhs arg can
	* be the id (any data type you specifiy) to check. For example, say I have a bunch of 'MyClass' objects in this
	* tree which all have a unique (integer) id. I can specify the search func to be "void MySearchFunc(int id, MyClass rhs)
	* and the rest will work just like the comparison function.... i.e. return -1 if id < rhs._id, 0 if id == rhs._id
	* and finally return 1 if id > rhs._id
	* @param id The id (any type) that will be passed into the left hand side (first arg) of the search function
	* @param SearchFunc Poitner to the search function (just like comparison function, this cannot be a member function of a class unless static
	*/
	template<typename IDType>
	T Find(IDType id, int(*SearchFunc)(IDType, T))
	{
		//start at _root
		TTreeNode<T>* cur = _root;

		//instantiate result here so we dont reinstantiate it in while loop
		int result = 0;

		//value to return
		T ret = T();

		//get the next available node
		while (cur != NULL)
		{
			//run search function to tell us to go left, right or found
			result = SearchFunc(id, cur->_data);

			//if result != 0 then go left or right
			if (result != 0)
			{
				//get next node (if -1 got left if 0 or 1 go right)
				cur = result <= -1 ? cur->_left : cur->_right;
			}
			//else it has been found so return the data
			else
			{
				ret = cur->_data;
				break;
			}
		}

		//return the data
		return ret;
	}

	/**
	* Overloaded Find object which will return the node of the specified object (or data) 
	* @param obj The object whos node we want to find
	* @return Pointer to the node (can be NULL if not found)
	*/
	TTreeNode<T>* Find(T obj)
	{
		//start at _root
		TTreeNode<T>* cur = _root;

		//compare the data being iserted with the data in the current node
		//to determine if we should go left or right in the tree and remember (declare outside while loop)
		//so we can check if we went left or right when inserting new node)
		int result = 0;

		//get the next available node
		while (cur != NULL)
		{
			//run comparison
			result = _comparison(obj, cur->_data);

			//found case
			if (result == 0) break;

			//get next node (if -1 got left if 0 or 1 go right)
			cur = result <= -1 ? cur->_left : cur->_right;
		}

		return cur;
	}

	
	/**
	* Removes the specfied data from the tree
	* @param data The data to remove from the tree
	*/
	virtual void Remove(T data)
	{
		//recursive call so we start from root
		_root = DeleteNode(_root, data);
	}

	/**
	* Overloaded remove to remove an iterator from the tree (note that this will set the iterator
	* back to the previous node to prevent pointing to unallocated memory
	* @param itr The iterator to remove passed by reference
	*/
	virtual void Remove(TTreeIter<T>& itr)
	{
		itr._current = DeleteNode(itr._current, itr._current->_data);
		
		//push this node
		itr._stack.Push(itr._current);
	}

};


/**
* Iterator class created to iterator (loop through) items on a TTree
* They can cast into the specified data (T) and even use member functions
* if T is a class by using the -> operator
*/

template<typename T>
class TTreeIter
{
	friend class TTree<T>;
private:

	TStack<TTreeNode<T>*> _stack; /**< Stack containing nodes to visit */

	TTreeNode<T>* _current; /**< The current node this iterator is at */

	/**
	* Pushes left node to stack from the given node
	* @param node The node whos left node will be pushed to stack
	*/
	inline void PushLeft(TTreeNode<T>* node)
	{
		if (node != NULL)
		{
			if (node->_left != NULL)
				_stack.Push(node->_left);
		}
	}

	/**
	* Pushes right node to stack from the given node
	* @param node The node whos right node will be pushed to stack
	*/
	inline void PushRight(TTreeNode<T>* node)
	{
		if (node != NULL)
		{
			if (node->_right != NULL)
				_stack.Push(node->_right);
		}
	}
public:
	/** 
	* Default constructor of the tree
	*/
	TTreeIter()
	{
		_current = NULL;
	}

	/**
	* Default destructor of the tree
	*/
	~TTreeIter()
	{

	}

	/**
	* Overloaded constructor which takes the tree to iterate over as a pointer
	* @param tree The pointer to the tree in which this iterator will loop through
	*/
    TTreeIter(TTree<T>* tree)
    {
       //set current to root
		_current = tree->_root;

		//push left and right nodes on the stack (if _current is not NULL)
		if (_current != NULL)
		{
			PushLeft(_current);
			PushRight(_current);
		}
    }
    
	/**
	* Returns true if the iterator as finished iterating over the tree (i.e node stack is empty)
	* @return Boolean
	*/
    inline bool IsFinished()
    {
		return (_current == NULL);
    }
    
	/**
	* Moves the iterator to the next node in the stack
	*/
    inline void Next()
    {
		//pop from stack
		_current = _stack.Pop();

		//push left and right
		PushLeft(_current);
		PushRight(_current);
    }

	/**
	* Returns the data stored in the current node (Can be NULL if pointer or otherwise its default
	* value if the current node is NULL
	* @return The Data (Can be NULL)
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