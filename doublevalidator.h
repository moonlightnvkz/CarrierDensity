#ifndef DOUBLEVALIDATOR_H
#define DOUBLEVALIDATOR_H
#include <QDoubleValidator>

class DoubleValidator : public QDoubleValidator
{
public:
    DoubleValidator(double bottom, double top, int decimals, QObject * parent);

    virtual ~DoubleValidator() override;

    QValidator::State validate(QString &s, int &i) const override;
};

#endif // DOUBLEVALIDATOR_H
