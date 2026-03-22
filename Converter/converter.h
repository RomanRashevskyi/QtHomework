#ifndef CONVERTER_H
#define CONVERTER_H
#include <QString>

class Converter {
public:
    enum Mode { Length = 0, Mass = 1, Temperature = 2 };
    static double convert(double value, const QString &fromUnit, const QString &toUnit, Mode mode);
};
#endif
