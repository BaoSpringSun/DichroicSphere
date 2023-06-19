/**
 *
*/

#pragma once
#include <iostream>
#include <string>
#include <list>
#include <memory>
#include "observer.hpp"
using namespace std;

class AbstractObserver;

class AbstractSubject{

public:
	virtual void attach(shared_ptr<AbstractObserver> observer) = 0;
	virtual void detach(shared_ptr<AbstractObserver> observer) = 0;
	virtual void notify() = 0;
};


/**
 * 被观察者--> 一个
 * 继承不写就默认是private继承，所以一定要写明是私有还是公有继承；
*/
class Subject : public AbstractSubject, public std::enable_shared_from_this<Subject>
{

public:
    Subject();
    ~Subject();

	void init();

public:
	void attach(shared_ptr<AbstractObserver> observer) override;
	void detach(shared_ptr<AbstractObserver> observer) override;
	void notify() override;

private:
	list<shared_ptr<AbstractObserver>> observers;
	weak_ptr<Subject> self;//用于监视自己的销毁情况；不能为shared_ptr，否则永远都析构不了
};

void sample_observer_subserver_main();