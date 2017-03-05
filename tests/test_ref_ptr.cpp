#include "test_ref_ptr.h"
#include "../include/ref_ptr.h"
#include <QtTest/QtTest>

void TestRefPtr::test()
{
    using namespace rsl;

    {
        lifetime_trackable object;

        {
            auto guard = make_ref(&object);
            QVERIFY(guard);
        }
    }

    {
        auto object = new lifetime_trackable();

        auto guard = make_ref(object);
        QVERIFY(guard);

        delete object;
        QVERIFY(!guard);
    }

    {
        lifetime_trackable object1;

        auto guard1 = make_ref(&object1);
        ref_ptr<lifetime_trackable>* guard2 = nullptr;

        {
            auto object2 = object1;
            guard2 = new ref_ptr<lifetime_trackable>(object2, &object2);

            QVERIFY(guard1);
            QVERIFY(*guard2);
        }

        QVERIFY(guard1);
        QVERIFY(!guard2);

        delete guard2;
    }

    {
        lifetime_trackable object;

        {
            auto guard = make_ref(&object);
            QVERIFY(guard);
            {
                auto guard = make_ref(&object);
                QVERIFY(guard);
                {
                    auto guard = make_ref(&object);
                    QVERIFY(guard);
                }
                QVERIFY(guard);
            }
            QVERIFY(guard);
        }
    }
}

