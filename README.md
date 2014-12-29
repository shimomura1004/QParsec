## Example

````
struct ParserNum : Parser<int> {
    int parse(Input &input) {
        int result = Many1(Digit())->parse(input).toInt();
        return setOut(result);
    }
};

typedef int(*intoperator)(int, int);
struct ParserPlus : Parser<int(*)(int,int)> {
    int (*parse(Input &input))(int, int) {
        Char('+')->parse(input);
        return [](int x, int y){ return x + y; };
    }
};

struct ParserTerm : Parser<int> {
    int parse(Input &input) {
        auto sum = Between( Chainl( Choice({ new ParserNum(), new ParserTerm() }),
                                    Between(new ParserPlus(), Many(Space()), Many(Space())),
                                    0),
                            Char('('),
                            Char(')'))->parse(input);
        return setOut(sum);
    }
};

int main(int argc, char *argv[])
{
    Q_UNUSED(argc);
    Q_UNUSED(argv);

    Input input("(12 + 34 + (56 + 78) + 10 + (20))");
    auto sum = S(new ParserTerm())->parse(input);
    qDebug() << sum;

    return 0;
}
````
