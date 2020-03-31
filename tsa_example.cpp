#include "ffa.h"
#include "mfa.h"
#include "mwfa.h"

#include <chrono>
#include <iostream>
#include <random>

int main() {
  constexpr unsigned T = 11;
  constexpr unsigned NP = 5;
  constexpr unsigned N = 16;

  std::random_device
      rd; // Will be used to obtain a seed for the random number engine
  std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
  std::uniform_int_distribution<uint64_t> dis(0ull, (1ull << T) - 1);

  std::uniform_int_distribution<uint64_t> wdis(1ull, 10ull);

  std::vector<BitMap> BIS_BK, BOS_BK;
  std::vector<std::vector<unsigned>> weights(N, std::vector<unsigned>(N, 0));
  for (unsigned i = 0; i < N; ++i) {
    BIS_BK.emplace_back(T, dis(gen));
    BOS_BK.emplace_back(T, dis(gen));

    for (unsigned j = 0; j < N; ++j)
      weights[i][j] = wdis(gen);
  }

  unsigned j = 1;
  std::vector<unsigned> proposals(NP, 0);
  for (unsigned k = 0; k < NP; ++k)
    proposals[k] = k;

  {
    std::vector<BitMap> BIS(BIS_BK), BOS(BOS_BK);

    std::cout << "O: " << BOS.at(j) << "\n";
    for (const auto p : proposals)
      std::cout << "I: " << BIS.at(p) << "\n";

    ffa allocator(BIS, BOS);
    auto res = allocator(j, proposals);
    std::cout << "Result of " << allocator.name << ": \n";
    for (unsigned t = 0; t < T; ++t)
      std::cout << t << " ==> " << res.at(t) << "\n";
  }

  {
    std::vector<BitMap> BIS(BIS_BK), BOS(BOS_BK);

    std::cout << "O: " << BOS.at(j) << "\n";
    for (const auto p : proposals)
      std::cout << "I: " << BIS.at(p) << "\n";

    mfa allocator(BIS, BOS);
    auto res = allocator(j, proposals);
    std::cout << "Result of " << allocator.name << ": \n";
    for (unsigned t = 0; t < T; ++t)
      std::cout << t << " ==> " << res.at(t) << "\n";
  }

  {
    std::vector<BitMap> BIS(BIS_BK), BOS(BOS_BK);

    std::cout << "O: " << BOS.at(j) << "\n";
    for (const auto p : proposals)
      std::cout << "I: " << BIS.at(p) << "\n";

    mwfa allocator(BIS, BOS, weights);
    auto res = allocator(j, proposals);
    std::cout << "Result of " << allocator.name << ": \n";
    for (unsigned t = 0; t < T; ++t)
      std::cout << t << " ==> " << res.at(t) << "\n";
  }

  return 0;
}