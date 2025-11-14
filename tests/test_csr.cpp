#include <gtest/gtest.h>
#include "hpgeom/csr.hpp"

TEST(CSRBasic, CanConstruct) {
    hpgeom::CSR csr(3, 3);
    EXPECT_TRUE(true);
}