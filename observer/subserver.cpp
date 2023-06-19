/**
 *
*/
#include "subserver.hpp"


void Subject::attach(shared_ptr<AbstractObserver> observer)
{
	observers.push_back(observer);
    return;
}

void Subject::detach(shared_ptr<AbstractObserver> observer)
{
	observers.remove(observer);
    return;
}

void Subject::notify()
{
	list<shared_ptr<AbstractObserver>>::iterator it = observers.begin();
	while(it != observers.end()){

		(*it)->update();
		it++;
	}
    return;
}

void Subject::init()
{
	self = shared_from_this();
	return;
}

Subject::Subject()
{
    observers.clear();
}

Subject::~Subject()
{
	cout << "开始析构了" << endl;
	list<shared_ptr<AbstractObserver>>::iterator itor = observers.begin();
	for(; itor!=observers.end(); itor++)
	{
		// (*itor).~shared_ptr();//这里强制析构有点危险，因为不清楚后面的程序是否还会继续使用指针所指对象，所以这里释放有点过早
		(*itor).reset();//重置指针本身为空，所指对象的引用计数减1；这种推荐，安全
	}
	observers.clear();
	cout << "self.use_count() = " << self.use_count() << endl;
}



