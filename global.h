#ifndef GLOBAL_H
#define GLOBAL_H

#define MATH_OPERATORS "[+\\-*/()<>!^=]"

struct physicalVariable {
    double  value;
    QString unit;
};

typedef struct {
    struct {
        struct {
            int decimalPrecision;
            char format;
        } numbers;
        int unitMode;
        int prefixMode;
    } output;
    struct {
        struct {
            bool outputWithNumbers;
            bool outputResult;
        } expression;
    } lineParser;
} AppSettings;

#endif // GLOBAL_H
