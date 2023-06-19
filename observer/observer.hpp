/**
 *
*/

#pragma once
#include <iostream>
#include <string>
#include <list>
#include <memory>
using namespace std;

class AbstractSubject;

class AbstractObserver{

public:
	virtual void update() = 0;
    virtual string getMyName() = 0;
};

/**
 * 观察者--> 多个
*/
class Observer : public AbstractObserver
{

public:
	Observer(const weak_ptr<AbstractSubject> subject, string myname);
    ~Observer();

	void update() override;
    string getMyName() override;

private:
	string myname;
	//当两个类中交叉有指向对方的智能指针，避免两个都使用的是强引用的指针；
	//需要将一个改为弱引用weak_ptr，避免循环使用导致引用计数不能为0而资源释放不了；
	weak_ptr<AbstractSubject> subject;
};
