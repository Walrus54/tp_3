#ifndef JSONDATAREADER_H
#define JSONDATAREADER_H

#include "data/IDataReader.h"

/**
 * @brief Читатель данных из JSON.
 *
 * Поддерживает два варианта одного формата [дата, значение]:
 *  1) массив точек:  [ {"date": "...", "value": 1.0}, ... ]
 *  2) объект ряда:   { "name": "...", "points": [ {"date": "...", "value": 1.0} ] }
 */
class JsonDataReader : public IDataReader
{
public:
    QStringList extensions() const override;
    bool canRead(const QString &filePath) const override;
    QStringList listSubSources(const QString &filePath) const override;
    Series read(const QString &source) override;
};

#endif // JSONDATAREADER_H
