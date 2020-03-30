#ifndef BITMAP__BITMAP_H_
#define BITMAP__BITMAP_H_

// Reference:
// https://github.com/gcc-mirror/gcc/blob/master/libstdc%2B%2B-v3/include/std/bitset

#include <cstdint>
#include <limits>
/**
 * \brief Short-Length BitMap (can fit in a single CPU word)
 *
 * NOTICE: It is just a special case of the std::bitset
 *
 */
class BitMap {
public:
  class reference;
  explicit BitMap(unsigned len, uint64_t value = 0);

  // length or size of this bitmap
  unsigned size() const { return _len; };

  // find the first set bit
  unsigned ffs() const;

  // flip the entire bitmap
  BitMap &flip() noexcept;

  // flip only the (pos)-th bit in the bitmap
  // NOTICE: Users should make sure that pos is less than the length of this
  // bitmap.
  BitMap &flip(unsigned pos);

  // set to all-1
  BitMap &set() noexcept;

  // set the (pos)-th bit to value
  BitMap &set(unsigned pos, bool value = true);

  // reset the entire bitmap to all-0
  BitMap &reset() noexcept;

  // reset the (pos)-th bit
  BitMap &reset(unsigned pos);

  // convert to an uint64 number
  uint64_t to_uint64() const { return _data; }

  /* operators */

  // (read-only) access the (pos)-th bit
  constexpr bool operator[](unsigned pos) const {
    return ((_data & BitMap::_maskbit(pos)) != 0ull);
  }

  // (read-write) access the (pos)-th bit
  reference operator[](unsigned pos);

  // flip the entire bitmap
  BitMap &operator~() { return flip(); }

  // bitwise-XOR with another bitmap
  BitMap &operator^=(const BitMap &other) {
    _data ^= other.to_uint64();
    return *this;
  }

  // bitwise-AND with another bitmap
  BitMap &operator&=(const BitMap &other) {
    _data &= other.to_uint64();
    return *this;
  }

  // bitwise-OR with another bitmap
  BitMap &operator|=(const BitMap &other) {
    _data |= other.to_uint64();
    return *this;
  }

  /* friend binary operators */

  // bitwise-XOR
  friend inline BitMap operator^(const BitMap &lhs, const BitMap &rhs);
  // bitwise-AND
  friend inline BitMap operator&(const BitMap &lhs, const BitMap &rhs);
  // bitwise-OR
  friend inline BitMap operator|(const BitMap &lhs, const BitMap &rhs);
  // equal
  friend inline bool operator==(const BitMap &lhs, const BitMap &rhs);

  // copied from http://www.cplusplus.com/reference/bitset/bitset/reference/
  class reference {
    friend class BitMap;
    uint64_t *_data_ptr;
    unsigned _pos;
    reference(BitMap &b, unsigned pos) noexcept; // no public constructor
  public:
    ~reference() = default;
    operator bool() const noexcept;                    // convert to bool
    reference &operator=(bool x) noexcept;             // assign bool
    reference &operator=(const reference &x) noexcept; // assign bit
    reference &flip() noexcept;                        // flip bit value
    bool operator~() const noexcept;                   // return inverse value
  };

private:
  static constexpr uint64_t _maskbit(unsigned pos) noexcept {
    return (1ull << pos);
  }
  unsigned _len;
  uint64_t _data;
};

/* Implementations */

BitMap::BitMap(unsigned len, uint64_t value) : _len(len), _data(value) {}

unsigned BitMap::ffs() const {
#if defined(__GNUC__)
  if (_data == 0ull) return _len;
  return __builtin_ctzl(_data);
#else
  for (unsigned i = 0; i < _len; ++i) {
    if (_data & BitMap::_maskbit(i))
      return i;
  }
  return _len;
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

BitMap &BitMap::set() noexcept {
  if (_len == 64)
    _data = std::numeric_limits<uint64_t>::max();
  else
    _data = (1ull << _len) - 1ull;

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

// constexpr bool BitMap::operator[](unsigned pos) const {
//   return ((_data & BitMap::_maskbit(pos)) != 0ull);
// }

BitMap::reference BitMap::operator[](unsigned pos) {
  return reference(*this, pos);
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

#endif // BITMAP__BITMAP_H_
