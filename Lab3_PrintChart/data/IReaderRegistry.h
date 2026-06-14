#ifndef IREADERREGISTRY_H
#define IREADERREGISTRY_H

#include <QStringList>
#include <memory>

#include "data/IDataReader.h"

/**
 * @brief Реестр читателей с диспетчеризацией по файлу/расширению.
 *
 * GUI зависит только от реестра, а не от конкретных читателей. Новый формат
 * добавляется регистрацией нового IDataReader в композиционном корне.
 */
class IReaderRegistry
{
public:
    virtual ~IReaderRegistry() = default;

    /**
     * @brief Подобрать читатель для файла.
     * @param filePath путь к файлу.
     * @return подходящий читатель либо nullptr.
     */
    virtual std::shared_ptr<IDataReader> readerFor(const QString &filePath) const = 0;

    /**
     * @brief Все поддерживаемые расширения (для фильтров файлового диалога).
     */
    virtual QStringList supportedExtensions() const = 0;
};

#endif // IREADERREGISTRY_H
