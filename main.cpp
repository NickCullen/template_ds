#include <stdio.h>
#include "tds.h"
#include <string>


#define NDEBUG 

class TestClass
{
private:
	static int _static_data;

public:
	int _data;
	bool _more_data;
	char _name[256];

	TestClass(const char* name)
	{
		_data = _static_data;
		_more_data = (_data % 2 == 0);
		_static_data += 5;
		strcpy(_name, name);
	}

	~TestClass()
	{
		_static_data -= 5;
	}
};

//static init
int TestClass::_static_data = 0;

/* Contains all tests running on TList */
void RunTListTests()
{
	//instantiate
	TList<TestClass*> list;

	//adding
	list.PushBack(new TestClass("Instance1"));
	list.PushBack(new TestClass("Instance2"));
	list.PushBack(new TestClass("Instance3"));
	list.PushBack(new TestClass("Instance4"));

	//add some more classes
	/*for(int i =0; i < 10; i++)
	{
		list.PushBack(new TestClass("AutoCreated"));
	}*/

	//removing
	TestClass* val = list.PopBack();

	//add the object to remove later
	TestClass* to_remove = new TestClass("ToRemove");
	list.PushBack(to_remove);

	//count check
	int count = list.Count();
	printf("List Count = %d\n", count);

	//iterting manually
	printf("\nManual Iteration of TList\n");
	for (TListIter<TestClass*> itr = TListIter<TestClass*>(&list,false); !itr.IsFinished(); itr++)
	{
		TestClass* cl = (TestClass*)itr;
		printf("value = %d\n", itr->_data);
	}

	//iterating via foreach macro - reads (foreach TestClass* cl in list)
	printf("\nforeach Iteration off TList\n");
	TLIST_foreach(TestClass*, cl, list)
	{
		printf("value = %d\n", cl->_data);
		list.Remove(cl);
	}

	//reverse foreach iteration testing
	printf("\nreverse foreach iteration of TList\n");
	TLIST_rev_foreach(TestClass*, cl, list)
	{
		printf("%s value = %d\n", cl->_name, cl->_data);
	}

	//remove to_remove
	list.Remove(to_remove);

	//clear list
	list.Empty();
}

/* Contains all tests running on TList */
void RunTStackTests()
{
	//create stack
	TStack<int> stack;

	//test pushing ints onto stack
	for (int i = 0; i < 10; i++)
	{
		stack.Push(i);
	}

	//test popping and peaking
	while (!stack.IsEmpty())
	{
		printf("Popping %d \n", stack.Peek());
		stack.Pop();
	}

}

int CompareInt(int lhs, int rhs)
{
	if (lhs < rhs) return -1;
	else if (lhs > rhs) return 1;
	else return 0;
}

/* Contains all tests running on TTree */
void RunTTreeTests()
{
	//create a ttree
	TTree<int> tree;

	//set the comparison func (note: can be set in constructor)
	tree.SetComparisonFunc(CompareInt);


	//insert into tree
	for (int i = 0; i < 10; i++)
	{
		tree.Insert(rand() % 50);
	}

	//find an item on the left
}

int main(int argc, char** argv)
{
	/* Run TList Tests */
	RunTListTests();

	/* Run TStack Tests */
	RunTStackTests();

	/* Run TTree Tests */
	RunTTreeTests();

	return 0;
}