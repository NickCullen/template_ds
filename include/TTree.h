#ifndef TTREE_H
#define TTREE_H

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

	TTreeIter<T>* _last_visitor; /** The last iterator to of checked this node (used in iterating over lists */

	/**
	* The default constructor of TTreeNode which initilizes all pointers to NULL
	*/
	TTreeNode()
	{
		_left = _right = _parent = NULL;
		_last_visitor = NULL;
	}

	/**
	* Default destructor 
	*/
	~TTreeNode()
	{
		_left = _right = _parent = NULL;
		_last_visitor = NULL;
	}

	/**
	* Inline function which will return if this is a leaf node 
	* or false if node
	* @return Boolean
	*/
	inline bool IsLeaf()
	{
		return (_left == NULL && _right == NULL);
	}

	/**
	* Inline function which will return true if this is the root node of the tree
	* @return Boolean
	*/
	inline bool IsRoot()
	{
		return (_parent == NULL);
	}

	/**
	* Inline function which will return true if this node has only a single child
	* @return Boolean
	*/
	inline bool HasOneChild()
	{
		return ((_left != NULL && _right == NULL) || (_right != NULL && _left == NULL));
	}

	
	/**
	* Sets the specified child to null
	* @param child The child to set to NULL
	*/
	inline void SetChildToNull(TTreeNode<T>* child)
	{
		if (child == _left) _left = NULL;
		else if (child == _right) _right = NULL;
	}

	/** 
	* Replaces the specified child node to the new_child
	* @param child The child to be replaced
	* @param new_child the new child to replace the old
	*/
	inline void ReplaceChild(TTreeNode<T>* child, TTreeNode<T>* new_child)
	{
		if (child == _left)
		{
			_left = new_child;
			new_child->_parent = this;
		}
		else if (child == _right)
		{
			_right = new_child;
			new_child->_parent = this;
		}
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
	* it will find the smallest node in the subtree starting (and including) root)
	* @param root the node at the root of the current search
	*/
	TTreeNode<T>* FindSmallestFromNode(TTreeNode<T>* root)
	{
		//has to be the smalled if its the only one
		if (root->IsLeaf())
		{
			return root;
		}
		//if it has no left node then it must be smaller than it
		else if (root->_left == NULL)
		{
			return root;
		}
		//recursive search next
		else
		{
			return FindSmallestFromNode(root->_left);
		}
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
	~TTree()
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
			Remove(_root);
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
		if (_count <= 0) return;

		TTreeNode<T>* node = Find(data);
		Remove(node);
	}

	/**
	* Overloaded remove to remove an iterator from the tree (note that this will set the iterator
	* back to the previous node to prevent pointing to unallocated memory
	* @param itr The iterator to remove passed by reference
	*/
	virtual void Remove(TTreeIter<T>& itr)
	{
		if (_count <= 0) return;

		//see if this its the root node
		bool is_root = itr._current->_parent == NULL;

		TTreeIter<T> tmp(itr);
		
		//if not the root node make tmp go back
		if (!is_root)
		{
			tmp.Prev();
			//make sure the node current visitor is not this
			itr._current->_last_visitor = NULL;
		}

		TTreeNode<T>* node = itr._current;
		Remove(node);

		//if it was the root node set it to root
		if (is_root)
		{
			tmp._current = _root;
			//make sure the node current visitor is not this
			itr._current->_last_visitor = NULL;
		}

		itr = tmp;
	}

	/**
	* Overloaded Remove function to remove a node from the tree 
	* @param node The node to be removed from the tree
	*/
	virtual void Remove(TTreeNode<T>* node)
	{
		//dont try to remove a null node
		if (node == NULL || _count <= 0) return;

		//case 1 (lead node)
		if (node->IsLeaf())
		{
			//simply remove reference and delete node
			if (node->_parent != NULL)
			{
				node->_parent->SetChildToNull(node);
				SAFE_DELETE(node);
			}
			else
			{
				SAFE_DELETE(_root);
			}
				
			_count--;
		}
		//second case (1 child)
		else if (node->HasOneChild())
		{
			//get the child node
			TTreeNode<T>* child = node->_left != NULL ? node->_left : node->_right;
			
			//delete this node from parent
			if (node->_parent != NULL)
			{
				node->_parent->ReplaceChild(node, child);
				//delete node
				SAFE_DELETE(node);
			}
			//we are removing the single node from a root node this is abit different....
			else
			{
				_root = child;
				child->_parent = NULL;
				if (_root->_left == NULL && _root->_right == NULL) SAFE_DELETE(_root);
			}

			
			_count--;
		}
		//third case has 2 children
		else
		{
			//find the min in the right child
			TTreeNode<T>* smallest = FindSmallestFromNode(node->_right);

			//replace data in node
			node->_data = smallest->_data;

			//remove the duplicate (smallest) node
			Remove(smallest);
		}
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
	TTreeNode<T>* _current; /**< The current node this iterator is at in the tree*/
  
	/**
	* Can only be accessed and used by TTree to move back the iterator
	* if it is being removed from the tree
	* @return The previous iterator (this -1)
	*/
	inline TTreeIter<T> Prev()
	{
		//going back in a BST is to go to parent
		if (_current != NULL)
		{
			_current = _current->_parent;
		}

		return (*this);
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
        //get the root node
        _current = tree->_root;
        
        //set its last visitor to be this
        _current->_last_visitor = this;
    }
    
	/**
	* Returns true if the iterator as finished iterating over the tree
	* @return Boolean
	*/
    inline bool IsFinished()
    {
        return (_current == NULL);
    }
    
	/**
	* Overloaded operator to get the next iterator
	* @return the next iterator (this +1)
	*/
    inline TTreeIter<T> operator++()
    {
        return Next();
    }

	/**
	* Overloaded operator to get the next iterator
	* @return the next iterator (this +1)
	*/
    inline TTreeIter<T> operator++(int)
    {
        TTreeIter<T> tmp(*this); // copy
        operator++(); // pre-increment
        return tmp;   // return old value
    }

	/**
	* Gets the next iterator in the tree
	* @return the next iterator (this +1)
	*/
    inline TTreeIter<T> Next()
    {
        // actual increment takes place here
		while (_current != NULL && _current->_last_visitor == this)
        {
			//go left if available
			if (_current->_left && _current->_left->_last_visitor != this)
            {
                _current = _current->_left;
                _current->_last_visitor = this;
				break;
            }
			//go right if available
			else if (_current->_right && _current->_right->_last_visitor != this)
            {
                _current = _current->_right;
                _current->_last_visitor = this;
				break;
            }
            //set parent
            else
            {
				_current = _current->_parent;
            }
        }
        return (*this);
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