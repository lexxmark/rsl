#ifndef TEST_REF_PTR_H
#define TEST_REF_PTR_H

#include <QObject>

class TestRefPtr : public QObject
{
    Q_OBJECT
public:
    Q_INVOKABLE TestRefPtr() = default;

private Q_SLOTS:
    void test();
};

#endif // TEST_REF_PTR_H
