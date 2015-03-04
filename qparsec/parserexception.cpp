#include "parserexception.h"

qparsec::ParserException::ParserException(int index, QString reason) : index_(index), reason_(reason)
{}
