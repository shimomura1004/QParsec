#include "input.h"

namespace qparsec {

Input::Input(const QString &str) : index_(0), str_(str)
{
}

bool Input::isEmpty() {
    return index_ >= str_.length();
}

int Input::length() {
    return str_.length() - index_;
}

int Input::index() {
    return index_;
}

QCharRef Input::operator[](int idx) {
    return str_[index_ + idx];
}

const QString Input::str() {
    return mid(0);
}

const QString Input::mid(int position, int n) {
    return str_.mid(index_ + position, n);
}

const QStringRef Input::midRef(int position, int n) {
    return str_.midRef(index_ + position, n);
}

const QString Input::left(int n) {
    return mid(n);
}

void Input::preserve() {
    indexes_.push_back(index_);
}

QString Input::consume(int n) {
    const QString s = str_.mid(index_, n);
    index_ += s.length();
    return s;
}

void Input::restore() {
    index_ = indexes_.takeLast();
}

void Input::clear() {
    indexes_.pop_back();
}

}
