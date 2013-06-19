#include "bloom_filter/basic.h"

#include <cmath>

namespace bf {

basic_bloom_filter::basic_bloom_filter(hasher h, size_t cells)
  : hasher_(std::move(h)),
    bits_(cells)
{
}

size_t basic_bloom_filter::m(double fp, size_t capacity)
{
  auto ln2 = std::log(2);
  return std::ceil(-(capacity * std::log(fp) / ln2 / ln2));
}

size_t basic_bloom_filter::k(size_t cells, size_t capacity)
{
  auto frac = static_cast<double>(cells) / static_cast<double>(capacity);
  return std::ceil(frac * std::log(2));
}

void basic_bloom_filter::add(object const& o)
{
  for (auto d : hasher_(o))
    bits_.set(d % bits_.size());
}

size_t basic_bloom_filter::lookup(object const& o) const
{
  for (auto d : hasher_(o))
    if (! bits_[d % bits_.size()])
      return 0;
  return 1;
}

void basic_bloom_filter::clear()
{
  bits_.reset();
}

} // namespace bf
