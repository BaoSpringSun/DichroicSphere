/**
 *
*/

#include "observer.hpp"



Observer::Observer(const weak_ptr<AbstractSubject> subject, string myname)
    :myname(myname)
    ,subject(subject)
{
    cout << "subject.use_count() = " << this->subject.use_count() << endl;
}

Observer::~Observer()
{
    cout << "开始析构了" << myname << endl;
}

void Observer::update()
{
	cout << "发布者更新东西了!!!阅者：" << myname << "  收到"  << endl;
}

string Observer::getMyName()
{
	return myname;
}


