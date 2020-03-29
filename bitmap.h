#ifndef BITMAP__BITMAP_H_
#define BITMAP__BITMAP_H_

#include <cstdint>
#include <limits>
/**
 * \brief Short-Length BitMap (can fit in a single CPU word)
 */
class BitMap {
public:
  class reference;
  static const unsigned npos = -1;
  explicit BitMap(unsigned len, uint64_t value = 0);

  unsigned size() const { return _len; };
  unsigned ffs() const;

  BitMap &flip() noexcept;
  BitMap &flip(unsigned pos);

  BitMap &set() noexcept;
  BitMap &set(unsigned pos, bool value = true);

  BitMap &reset() noexcept;
  BitMap &reset(unsigned pos);

  uint64_t to_uint64() const { return _data; }

  // operators
  bool operator[](unsigned pos) const;
  reference operator[](unsigned pos);

  BitMap &operator~() { return flip(); }

  BitMap &operator^=(const BitMap &other) {
    _data ^= other.to_uint64();
    return *this;
  }

  BitMap &operator&=(const BitMap &other) {
    _data &= other.to_uint64();
    return *this;
  }

  BitMap &operator|=(const BitMap &other) {
    _data |= other.to_uint64();
    return *this;
  }

  // friend binary operators
  friend inline BitMap operator^(const BitMap &lhs, const BitMap &rhs);
  friend inline BitMap operator&(const BitMap &lhs, const BitMap &rhs);
  friend inline BitMap operator|(const BitMap &lhs, const BitMap &rhs);
  friend inline bool operator==(const BitMap &lhs, const BitMap &rhs);

  // copied from http://www.cplusplus.com/reference/bitset/bitset/reference/
  class reference {
    friend class BitMap;
    uint64_t *_data_ptr;
    unsigned _pos;
    reference(BitMap &b, unsigned pos) noexcept; // no public constructor
  public:
    ~reference() = default;
    ;
    explicit operator bool() const noexcept;           // convert to bool
    reference &operator=(bool x) noexcept;             // assign bool
    reference &operator=(const reference &x) noexcept; // assign bit
    reference &flip() noexcept;                        // flip bit value
    bool operator~() const noexcept;                   // return inverse value
  };

private:
  static uint64_t _maskbit(unsigned pos) noexcept { return (1ull << pos); }
  unsigned _len;
  uint64_t _data;
};

BitMap::BitMap(unsigned len, uint64_t value) : _len(len), _data(value) {}

unsigned BitMap::ffs() const {
#if defined(__GNUC__)
  return __builtin_clzl(_data);
#else
  if (_data == 0)
    return BitMap::npos;
  for (unsigned i = 0; i < _len; ++i) {
    if (_data & BitMap::_maskbit(i))
      return i;
  }
#endif
}

BitMap &BitMap::flip() noexcept {
  _data = ~_data;
  return *this;
}
BitMap &BitMap::flip(unsigned pos) {
  _data ^= BitMap::_maskbit(pos);
  return *this;
}

BitMap &BitMap::set(unsigned pos, bool value) {
  if (value)
    _data |= BitMap::_maskbit(pos);
  else
    _data &= ~BitMap::_maskbit(pos);

  return *this;
}
BitMap &BitMap::reset() noexcept {
  _data = 0ull;
  return *this;
}
BitMap &BitMap::reset(unsigned pos) { return set(pos, false); }
bool BitMap::operator[](unsigned pos) const {
  return (_data & BitMap::_maskbit(pos)) != 0;
}
BitMap::reference BitMap::operator[](unsigned pos) {
  return reference{*this, pos};
}

BitMap::reference::reference(BitMap &b, unsigned pos) noexcept
    : _data_ptr(&b._data), _pos(pos) {}

BitMap::reference::operator bool() const noexcept {
  auto bit_val = ((*_data_ptr) >> _pos) & 1ull;
  return (bit_val != 0ull);
}
BitMap::reference &BitMap::reference::operator=(bool x) noexcept {
  if (x)
    *_data_ptr |= BitMap::_maskbit(_pos);
  else
    *_data_ptr &= ~BitMap::_maskbit(_pos);

  return *this;
}

BitMap::reference &
BitMap::reference::operator=(const BitMap::reference &x) noexcept {
  if (((*x._data_ptr) & BitMap::_maskbit(_pos)))
    *_data_ptr |= BitMap::_maskbit(_pos);
  else
    *_data_ptr &= ~BitMap::_maskbit(_pos);

  return *this;
}

BitMap::reference &BitMap::reference::flip() noexcept {
  *_data_ptr ^= BitMap::_maskbit(_pos);
  return *this;
}
bool BitMap::reference::operator~() const noexcept {
  return ((*_data_ptr & BitMap::_maskbit(_pos)) == 0);
}

// overloading operators
inline BitMap operator^(const BitMap &lhs, const BitMap &rhs) {
  return BitMap{lhs.size(), lhs.to_uint64() ^ rhs.to_uint64()};
}

inline BitMap operator&(const BitMap &lhs, const BitMap &rhs) {
  return BitMap{lhs.size(), lhs.to_uint64() & rhs.to_uint64()};
}

inline BitMap operator|(const BitMap &lhs, const BitMap &rhs) {
  return BitMap{lhs.size(), lhs.to_uint64() | rhs.to_uint64()};
}

inline bool operator==(const BitMap &lhs, const BitMap &rhs) {
  return lhs.size() == rhs.size() && lhs.to_uint64() == rhs.to_uint64();
}
BitMap &BitMap::set() noexcept {
  if (_len == 64)
    _data = std::numeric_limits<uint64_t>::max();
  else
    _data = (1ull << _len) - 1ull;

  return *this;
}

#endif // BITMAP__BITMAP_H_
