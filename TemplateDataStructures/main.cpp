#include <stdio.h>
#include "TList.h"


class TestClass
{
private:
	static int _static_data;

public:
	int _data;
	bool _more_data;


	TestClass()
	{
		_data = _static_data;
		_more_data = (_data % 2 == 0);
		_static_data += 5;
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
	list.PushBack(new TestClass());
	list.PushBack(new TestClass());
	list.PushBack(new TestClass());
	list.PushBack(new TestClass());

	//removing
	TestClass* val = list.PopBack();

	//count check
	int count = list.Count();

	//iterting manually
	for (TListIter<TestClass*> itr = TListIter<TestClass*>(&list); !itr.IsFinished(); itr++)
	{
		TestClass* cl = (TestClass*)itr;
		printf("value = %d\n", itr->_data);
	}

	//iterating via foreach macro - reads (foreach TestClass* cl in list)
	TLIST_foreach(TestClass*, cl, list)
	{
		printf("value = %d\n", cl->_data);
	}
}

int main(int argc, char** argv)
{
	/* Run TList Tests */
	RunTListTests();

	return 0;
}