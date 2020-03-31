# Time Slot Allocations for SB-QPS

Implementation of the three time slot allocation methods for SB-QPS.

## Prerequisites

+ [The Boost Graph Library (BGL)](https://www.boost.org/doc/libs/1_72_0/libs/graph/doc/index.html): Version of (at least) 1.72.0 is required 

```bash
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.sh
./vcpkg install boost_graph
```


## Sample Output

```bash
❯ ./tsa_example 
O: 00101100111
I: 01001101010
I: 00011001101
I: 00011000010
I: 10110101100
I: 00101110100
01001101010 & 00101100111 = 00001100010 ==> 4
00011001101 & 00100100111 = 00000000101 ==> 8
00011000010 & 00100100011 = 00000000010 ==> 9
10110101100 & 00100100001 = 00100100000 ==> 2
00101110100 & 00000100001 = 00000100000 ==> 5
Result of FFA: 
0 ==> 4294967295
1 ==> 4294967295
2 ==> 3
3 ==> 4294967295
4 ==> 0
5 ==> 4
6 ==> 4294967295
7 ==> 4294967295
8 ==> 1
9 ==> 2
10 ==> 4294967295
O: 00101100111
I: 01001101010
I: 00011001101
I: 00011000010
I: 10110101100
I: 00101110100
Result of MFA: 
0 ==> 4294967295
1 ==> 4294967295
2 ==> 3
3 ==> 4294967295
4 ==> 2
5 ==> 4
6 ==> 4294967295
7 ==> 4294967295
8 ==> 4294967295
9 ==> 0
10 ==> 1
O: 00101100111
I: 01001101010
I: 00011001101
I: 00011000010
I: 10110101100
I: 00101110100
Result of MWFA: 
0 ==> 4294967295
1 ==> 4294967295
2 ==> 4294967295
3 ==> 4294967295
4 ==> 4
5 ==> 0
6 ==> 4294967295
7 ==> 4294967295
8 ==> 3
9 ==> 2
10 ==> 1
```

