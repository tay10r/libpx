#include "Blob.hpp"

#include <vector>

namespace px {

class BlobImpl final
{
  friend Blob;
  /// A pointer to the blob byte data.
  std::vector<unsigned char> data;
};

Blob::Blob() : self(new BlobImpl()) { }

Blob::~Blob() { delete self; }

unsigned char* Blob::data() noexcept
{
  return self->data.data();
}

const unsigned char* Blob::data() const noexcept
{
  return self->data.data();
}

std::size_t Blob::size() const noexcept
{
  return self->data.size();
}

void Blob::resize(std::size_t size)
{
  self->data.resize(size);
}

void Blob::expand(std::size_t size)
{
  self->data.resize(self->data.size() + size);
}

unsigned char& Blob::operator[] (std::size_t index) noexcept
{
  return self->data[index];
}

const unsigned char& Blob::operator[] (std::size_t index) const noexcept
{
  return self->data[index];
}

} // namespace px
