#include "bitmap.h"
#include <gtest/gtest.h>

#include <bitset>

TEST(BitMapTest, SimpleAPIs) {
  {
    BitMap bm(7, 3);
    EXPECT_EQ(bm.size(), 7);
    EXPECT_EQ(bm.to_uint64(), 3ull);
  }
  {
    BitMap bm(7);
    EXPECT_EQ(bm.size(), 7);
    EXPECT_EQ(bm.to_uint64(), 0ull);
  }
}

TEST(BitMapTest, SetAndReset) {
  BitMap bm(8);
  bm.set();
  EXPECT_EQ(bm.to_uint64(), 255ull);
  bm.reset(0);
  EXPECT_EQ(bm.to_uint64(), 254ull);
  bm.reset(7);
  EXPECT_EQ(bm.to_uint64(), 126ull);
  bm.reset();
  EXPECT_EQ(bm.to_uint64(), 0ull);
  bm.set(1);
  EXPECT_EQ(bm.to_uint64(), 2ull);
  bm.set(6);
  EXPECT_EQ(bm.to_uint64(), 66ull);
}

TEST(BitMapTest, Flip) {
  auto x = 100ull;
  BitMap bm(8, x);
  bm.flip();
  EXPECT_EQ(bm.to_uint64(), ~x);
  bm.flip();
  EXPECT_EQ(bm.to_uint64(), x);
  bm.flip(0);
  EXPECT_EQ(bm.to_uint64(), x + 1);
  bm.flip(0);
  EXPECT_EQ(bm.to_uint64(), x);
  bm.flip(6);
  EXPECT_EQ(bm.to_uint64(), x - 64);
  bm.flip(6);
  EXPECT_EQ(bm.to_uint64(), x);
}

TEST(BitMapTest, AccessOperators) {
  BitMap bm(16);
  EXPECT_FALSE(bm[0]);
  bm[0] = true;
  EXPECT_EQ(bm.to_uint64(), 1ull);
  EXPECT_FALSE(bm[7]);
  bm[7] = true;
  EXPECT_EQ(bm.to_uint64(), 129ull);

  bool x = bm[9];
  EXPECT_EQ(x, false);
}

TEST(BitMapTest, OtherOperators) {
  BitMap b1(15), b2(15);

  {
    auto b = b1 ^ b2;
    EXPECT_EQ(b.size(), 15);
    EXPECT_EQ(b.to_uint64(), 0ull);
  }

  {
    auto b = b1 | b2;
    EXPECT_EQ(b.size(), 15);
    EXPECT_EQ(b.to_uint64(), 0ull);
  }

  {
    auto b = b1 & b2;
    EXPECT_EQ(b.size(), 15);
    EXPECT_EQ(b.to_uint64(), 0ull);
  }

  BitMap b3(15);
  b3.set();

  {
    auto b = b1 ^ b3;
    EXPECT_EQ(b3, b);
  }

  {
    auto b = b1 | b3;
    EXPECT_EQ(b3, b);
  }

  {
    auto b = b1 & b3;
    EXPECT_EQ(b1, b);
  }
}

TEST(BitMapTest, FFS) {
    BitMap bm(19);
    auto p = bm.ffs();
    EXPECT_EQ(p, bm.size());
    bm.set(10);
    p = bm.ffs();
    EXPECT_EQ(p, 10);
    bm.set(1);
    p = bm.ffs();
    EXPECT_EQ(p, 1);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}