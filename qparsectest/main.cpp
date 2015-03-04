#include "autotest.h"
#include <QObject>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);

    int failures = AutoTest::run(argc, argv);

    if (failures == 0)
    {
        qDebug() << flush;
        QTest::qWait(1);
        qDebug() << "ALL TESTS PASSED";
    }
    else
    {
        qDebug() << flush;
        QTest::qWait(1);
        qDebug() << failures << " TESTS FAILED!";
    }
    return failures;
}
