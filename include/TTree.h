#ifndef TTREE_H
#define TTREE_H

/* Definitions */
#ifndef NULL
#define NULL 0
#endif

/* A Tree Node */
template<typename T>
struct TTreeNode
{
	T _data;
	TTreeNode<T>* _left;
	TTreeNode<T>* _right;
	TTreeNode<T>* _parent;

	//ctor
	TTreeNode()
	{
		_left = _right = _parent = NULL;
	}
}; /* End of TTreeNode */


/* Template class for TTree */
template<typename T>
class TTree
{
private:
	//root of the tree
	TTreeNode<T>* _root;

	//node count
	int _count;

	/* The comparison function used to insert and retrieve nodes from the tree *
	* return -1 if lhs < rhs                                                  *
	* return 0 if lhs == rhs                                                  *
	* return 1 if lhs > rhs                                                   */
	int (*_comparison)(T, T);

public:
	//ctor
	TTree()
	{
		_root = NULL;
		_comparison = NULL;
		_count = 0;
	}

	//this is the ctor that should be used!
	TTree(int (*ComparisonFunc)(T, T))
	{
		SetComparisonFunc(ComparisonFunc);
	}

	//dtor
	~TTree()
	{
		//empty list
		Empty();
	}

	//sets the comparison function (if the default ctor was used)
	void SetComparisonFunc(int(*ComparisonFunc)(T, T))
	{
		//set the comparison func
		_comparison = ComparisonFunc;
	}

	//returns count
	inline int Count()
	{
		return _count;
	}

	//return if list is empty
	inline bool IsEmpty()
	{
		return !_count;
	}

	//emptys list
	void Empty()
	{

	}

	//inserts into list
	void Insert(T data)
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
	}

	/* This function will search the tree for the required item                            *
	 * it accepts a search function similar to the comparison function                     *
	 * Except it will required the function to let the tree know if to go left or right    *
	 * return -1 will go left                                                              *
	 * return 1 will go right                                                              *
	 * return 0 if object has been found                                                   */
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

};
#endif