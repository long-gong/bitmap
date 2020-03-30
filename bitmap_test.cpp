#include "bitmap.h"
#include <gtest/gtest.h>

TEST(BitMapTest, SimpleAPIs) {
    BitMap bm(7, 3);
    EXPECT_EQ(bm.size(), 7);
    EXPECT_EQ(bm.to_uint64(), 3ull);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}