#ifndef INPUT_H
#define INPUT_H

#include <QString>
#include <QList>

namespace qparsec {

class Input
{
protected:
    int index_;
    QString str_;
    QList<uint64_t> indexes_;

public:
    explicit Input(const QString &str);

    bool isEmpty();
    int length();
    int index();

    QCharRef operator[](int idx);
    const QString str();
    const QString mid(int position, int n = -1);
    const QStringRef midRef(int position, int n = -1);
    const QString left(int n);

    void preserve();
    QString consume(int n);
    void restore();
    void clear();
};

}

#endif // INPUT_H
