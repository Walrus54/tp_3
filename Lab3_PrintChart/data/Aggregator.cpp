#include "data/Aggregator.h"

#include <QHash>
#include <QVector>
#include <cmath>

namespace
{
// Округление до двух знаков после запятой для усреднённого значения.
constexpr double kRoundScale = 100.0;
} // namespace

namespace Aggregator
{
Series aggregate(const Series &src, int keyLen)
{
    Series out;
    out.name = src.name;

    QHash<QString, QPair<double, int>> acc; // ключ -> (сумма, количество)
    QVector<QString> order;                 // порядок первого появления ключей

    for (const ChartPoint &p : src.points) {
        const QString &raw = p.label;
        // "DD.MM.YYYY ..." -> пропускаем день; иначе берём с начала (ISO и пр.).
        const int offset =
            (raw.size() > kDayPrefixLen && raw.at(2) == '.') ? kDayPrefixLen : 0;
        const QString key = raw.mid(offset, keyLen);

        auto it = acc.find(key);
        if (it == acc.end()) {
            acc.insert(key, {p.value, 1});
            order.push_back(key);
        } else {
            it->first += p.value;
            it->second += 1;
        }
    }

    out.points.reserve(order.size());
    for (const QString &key : order) {
        const auto &pair = acc.value(key);
        const double avg = pair.first / pair.second;
        out.points.push_back({key, std::round(avg * kRoundScale) / kRoundScale});
    }
    return out;
}

Series aggregateIfLarge(const Series &src)
{
    return src.points.size() > kThreshold ? aggregate(src) : src;
}
} // namespace Aggregator
