#ifndef __FFA_H_
#define __FFA_H_

#include "base_allocate.h"

// First fit allocation
struct ffa : base_allocate {
  ffa(bitmap_vector &in, bitmap_vector &out) : base_allocate(in, out, "FFA") {}

  std::vector<unsigned int>
  operator()(unsigned int j, const std::vector<unsigned> &proposals) override {
    unsigned batch_size = left->front().size();
    constexpr unsigned UNMATCHED = -1;

    std::vector<unsigned int> allocation(batch_size, UNMATCHED);

    auto &bo = right->at(j);
    for (auto p : proposals) {
      auto &bi = left->at(p);
      auto b = bi & bo;
      unsigned k = b.ffs();
      if (k < batch_size) {
        allocation[k] = p;
        bo.reset(k);
      }
    }

    return allocation;
  }
};

#endif //__FFA_H_
