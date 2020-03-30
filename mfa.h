#ifndef __MFA_H_
#define __MFA_H_

#include "base_allocate.h"
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

using namespace boost;
typedef adjacency_list<vecS, vecS, directedS> uw_graph;

struct mfa : base_allocate {
  mfa(bitmap_vector &in, bitmap_vector &out) : base_allocate(in, out, "MFA") {}

  std::vector<unsigned int>
  operator()(unsigned int j, const std::vector<unsigned> &proposals) override {
    const unsigned batch_size = left->front().size();
    const unsigned np = proposals.size();
    constexpr unsigned UNMATCHED = -1;

    std::vector<unsigned int> allocation(batch_size, UNMATCHED);

    auto &bo = right->at(j);

    const size_t n_vertices = batch_size + np;
    uw_graph g(n_vertices);

    for (unsigned k = 0; k < batch_size; ++k) {
      if (bo[k]) {
        for (unsigned i = 0; i < np; ++i) {
          auto p = proposals[i];
          if (left->at(p)[k]) {
            add_edge(k, i + batch_size, g);
          }
        }
      }
    }

    std::vector<int> mate(n_vertices, -1);
    edmonds_maximum_cardinality_matching(g, &mate[0]);

    for (unsigned k = 0; k < batch_size; ++k) {
      if (mate[k] >= batch_size && mate[k] < n_vertices) {
        allocation[k] = proposals.at(mate[k] - batch_size);
      }
    }
    return allocation;
  }
};

#endif //__MFA_H_
