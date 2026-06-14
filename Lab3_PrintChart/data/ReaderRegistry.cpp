#include "data/ReaderRegistry.h"

void ReaderRegistry::add(std::shared_ptr<IDataReader> reader)
{
    if (reader)
        m_readers.push_back(std::move(reader));
}

std::shared_ptr<IDataReader> ReaderRegistry::readerFor(const QString &filePath) const
{
    for (const auto &reader : m_readers)
        if (reader->canRead(filePath))
            return reader;
    return nullptr;
}

QStringList ReaderRegistry::supportedExtensions() const
{
    QStringList exts;
    for (const auto &reader : m_readers)
        for (const QString &e : reader->extensions())
            if (!exts.contains(e))
                exts << e;
    return exts;
}
