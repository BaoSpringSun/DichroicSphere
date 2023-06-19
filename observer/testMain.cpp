/**
 *
*/

#include "observer.hpp"
#include "subserver.hpp"

void sample_observer_subserver_main()
{
    weak_ptr<Subject> subweakp;
    weak_ptr<Observer> obs_bobweakp;
    weak_ptr<Observer> obs_kevinweakp;
    weak_ptr<Observer> obs_zhixingweakp;
    {
        shared_ptr<Subject> sub = make_shared<Subject>();
        subweakp = sub;
        shared_ptr<Observer> obs_bob = make_shared<Observer>(sub, "bob");
        // sub->init();
        shared_ptr<Observer> obs_kevin = make_shared<Observer>(sub, "kevin");
        shared_ptr<Observer> obs_zhixing = make_shared<Observer>(sub, "zhixing");
        obs_bobweakp = obs_bob;
        obs_kevinweakp = obs_kevin;
        obs_zhixingweakp = obs_zhixing;
        sub->attach(obs_bob);
        sub->attach(obs_kevin);
        sub->attach(obs_zhixing);
        sub->notify();
        cout << "sub.use_count() = " << sub.use_count() << endl;
    }
    cout << "subweakp.use_count() = " << subweakp.use_count() << endl;
    cout << "obs_bobweakp.use_count() = " << obs_bobweakp.use_count() << endl;
    cout << "obs_kevinweakp.use_count() = " << obs_kevinweakp.use_count() << endl;
    cout << "obs_zhixingweakp.use_count() = " << obs_zhixingweakp.use_count() << endl;

    if(subweakp.use_count() > 0)
    {
        cout << "free subweakp" << endl;
        subweakp.lock().~shared_ptr();
    }

    if(obs_bobweakp.use_count() > 0)
    {
        cout << "free obs_bobweakp" << endl;
        obs_bobweakp.lock().~shared_ptr();
    }
    if(obs_kevinweakp.use_count() > 0)
    {
        cout << "free obs_kevinweakp" << endl;
        obs_kevinweakp.lock().~shared_ptr();
    }
    if(obs_zhixingweakp.use_count() > 0)
    {
        cout << "free obs_zhixingweakp" << endl;
        obs_zhixingweakp.lock().~shared_ptr();
    }


    cout << "----------------------------------" << endl;
    cout << "subweakp.use_count() = " << subweakp.use_count() << endl;
    cout << "obs_bobweakp.use_count() = " << obs_bobweakp.use_count() << endl;
    cout << "obs_kevinweakp.use_count() = " << obs_kevinweakp.use_count() << endl;
    cout << "obs_zhixingweakp.use_count() = " << obs_zhixingweakp.use_count() << endl;

    shared_ptr<Subject> subTTT = make_shared<Subject>();
    shared_ptr<Subject> subTTT2(subTTT);
    cout << "subTTT.use_count() = " << subTTT.use_count() << endl;
    weak_ptr<Subject> subTTTweakptr(subTTT);
    cout << "subTTT.use_count() = " << subTTTweakptr.use_count() << endl;
    subTTTweakptr.reset();
    cout << "reset -->subTTT.use_count() = " << subTTTweakptr.use_count() << endl;
    cout << "subTTT.use_count() = " << subTTT.use_count() << endl;

    cout << "----------------------------------" << endl;
    subTTT2.reset();
    cout << "subTTT2.use_count() = " << subTTT2.use_count() << endl;
    cout << "subTTT.use_count() = " << subTTT.use_count() << endl;
    cout << "----------------------------------" << endl;

    return;
}






