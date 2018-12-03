\ Model arb
\ LP format - for model browsing. Use MPS format to capture full model detail.
Minimize
  x0 + 2 x1 + 3.3 x2 + 2.26 x3 + 1.33 x4 + 2.2 x5 - 9 x6
Subject To
 scenario_constr: 1.05 x0 + 1.05 x1 - 2.5 x2 + x3 - 0.4 x4 - x5 + 8.5 x6
   >= 0
 scenario_constr: 1.05 x0 + 1.1 x1 + 3.1 x2 + 9.8 x3 + 1.2 x4 + 0.011 x5
   + 0.3 x6 >= 0
 scenario_constr: 1.05 x0 + 1.2 x1 + 26 x2 - 10 x3 + 1.5 x4 + 1.5 x5
   + 1.1 x6 >= 0
 binary_constr: x1 + y1 >= 0
 binary_constr: x1 - y1 <= 0
 binary_constr: x2 + y2 >= 0
 binary_constr: x2 - y2 <= 0
 binary_constr: x3 + y3 >= 0
 binary_constr: x3 - y3 <= 0
 binary_constr: x4 + y4 >= 0
 binary_constr: x4 - y4 <= 0
 binary_constr: x5 + y5 >= 0
 binary_constr: x5 - y5 <= 0
 binary_constr: x6 + y6 >= 0
 binary_constr: x6 - y6 <= 0
 max_limit: y1 + y2 + y3 + y4 + y5 + y6 <= 4
Bounds
 -1 <= x0 <= 1
 -1 <= x1 <= 1
 -1 <= x2 <= 1
 -1 <= x3 <= 1
 -1 <= x4 <= 1
 -1 <= x5 <= 1
 -1 <= x6 <= 1
 -1 <= y1 <= 1
 -1 <= y2 <= 1
 -1 <= y3 <= 1
 -1 <= y4 <= 1
 -1 <= y5 <= 1
 -1 <= y6 <= 1
Binaries
 y1 y2 y3 y4 y5 y6
End
