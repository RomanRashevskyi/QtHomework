#include "converter.h"
#include <QMap>

double Converter::convert(double value, const QString &fromUnit, const QString &toUnit, Mode mode) {
    QString from = fromUnit.trimmed();
    QString to = toUnit.trimmed();

    if (mode == Temperature) {
        double k;
        if (from == "C") k = value + 273.15;
        else if (from == "F") k = (value - 32) * 5.0/9.0 + 273.15;
        else k = value;

        if (to == "C") return k - 273.15;
        if (to == "F") return (k - 273.15) * 9.0/5.0 + 32;
        return k;
    }

    static QMap<QString, double> factors = {
        {"m", 1.0}, {"km", 1000.0}, {"in", 0.0254}, {"ft", 0.3048}, {"mi", 1609.34},
        {"kg", 1.0}, {"lb", 0.453592}, {"oz", 0.0283495}
    };

    double fromFactor = factors.value(from, 1.0);
    double toFactor = factors.value(to, 1.0);

    return (value * fromFactor) / toFactor;
}
