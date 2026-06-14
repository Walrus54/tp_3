#ifndef READERREGISTRY_H
#define READERREGISTRY_H

#include <vector>

#include "data/IReaderRegistry.h"

/**
 * @brief Реестр читателей: подбирает читатель по расширению файла.
 */
class ReaderRegistry : public IReaderRegistry
{
public:
    /**
     * @brief Добавить читатель в реестр (nullptr игнорируется).
     */
    void add(std::shared_ptr<IDataReader> reader);

    std::shared_ptr<IDataReader> readerFor(const QString &filePath) const override;
    QStringList supportedExtensions() const override;

private:
    std::vector<std::shared_ptr<IDataReader>> m_readers;
};

#endif // READERREGISTRY_H
