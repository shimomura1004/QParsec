#include <QCoreApplication>
#include <QParsec.h>
#include <QParsecChar.h>
#include <QParsecCombinator.h>

#include <QDebug>

struct ParserNum : Parser<int> {
    int parse(Input &input) {
        return Many1(Digit())->parse(input).toInt();
    }
};
struct ParserTerm : Parser<int> {
    int parse(Input &input) {
        QList<int> nums = Between( SepBy1(
                                Choice({ new ParserNum(), new ParserTerm() }),
                                Many1(Space())),
                            Char('('),
                            Char(')'))->parse(input);
        int result = 0;
        Q_FOREACH(int x, nums)
            result += x;
        return setOut(result);
    }
};

int main(int argc, char *argv[])
{
    Input input("(12 34 (56 78) 10 (20))");
    auto sum = S(new ParserTerm())->parse(input);
    qDebug() << sum;

    return 0;
}
