#include <QtDebug>
#include "doublevalidator.h"

DoubleValidator::DoubleValidator(double bottom, double top, int decimals, QObject * parent)
    : QDoubleValidator(bottom, top, decimals, parent) {

}

DoubleValidator::~DoubleValidator() {

}

QValidator::State DoubleValidator::validate(QString &s, int &) const {
    if (s.isEmpty() || s == "-") {
        return QValidator::Intermediate;
    }

    QChar decimalPoint = locale().decimalPoint();

    if(s.indexOf(decimalPoint) != -1) {
        int charsAfterPoint = s.length() - s.indexOf(decimalPoint) - 1;

        if (charsAfterPoint > decimals()) {
            return QValidator::Invalid;
        }
    }

    s.remove(locale().groupSeparator());

    bool ok;
    double d = locale().toDouble(s, &ok);
    qDebug() << ok << " " << s << " " << d;

    bool meet_e = false;
    for (QChar c : s) {
        if (c == 'e') {
            if (meet_e) return QValidator::Invalid;
            meet_e = true;
        }
    }

    if (notation() == ScientificNotation && s.back() == 'e') {
        return QValidator::Intermediate;
    }
    if (ok && d >= bottom() && d <= top()) {
        return QValidator::Acceptable;
    }
    return QValidator::Invalid;
}
