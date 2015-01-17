#include <stdio.h>
#include "TList.h"

class TestClass
{
public:
	int _data;
	bool _more_data;


	TestClass()
	{
		_data = 24;
		_more_data = true;
	}

	~TestClass()
	{

	}
};

int main(int argc, char** argv)
{
	TList<TestClass*> list;

	TestClass* class1 = new TestClass();
	class1->_data = 554;

 	list.PushBack(new TestClass());
	list.PushBack(class1);

	bool empty = list.IsEmpty();
	int count = list.Count();
	TestClass* tmp = list.PopBack();
	empty = list.IsEmpty();
	count = list.Count();

	return 0;
}