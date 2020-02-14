# Problem statement mathematically

## Given  

```math
{\displaystyle Vocabulary\: = V}
```

```math
{\displaystyle Conversion \: Cost = CC}
```

```math
{\displaystyle Character \: Cost \: Matrix = MC^{K+1 \: \times \: K+1}}
```

```math
{\displaystyle X_0,X_1,...X_{K-1} \in V^{*}}
```

```math
{\displaystyle |X_i| = N_i}
```

## Find  

```math
{\displaystyle Find \: F : V^{*} \to \bf \{x | (|x| = N, x \in (V \cup \{-\})^{*})\}}
```

```math
{\displaystyle F(X_i) = S_i}
```

## Minimize

```math
{\displaystyle C = \sum_{i=0}^{i<K}(CC \times (|S_i| - |X_i|) + \sum_{j=i+1}^{j<K}(\sum_{k=0}^{k<N}(MC(S_i[k],S_j[k]))))} 
```

----

## Ideas : 
*****MUST READ***** - https://arxiv.org/pdf/1109.6029.pdf

****IGNORE EVERYTHING ELSE LETS FOCUS ON THIS****^^^^^^

For local search as a base algorithm, use Random walk.
Complexity of O(nk^2) or O(kn^2) should be achievable by heuristics.
Read - 
1. Will help - https://en.wikipedia.org/wiki/Needleman%E2%80%93Wunsch_algorithm 
2. Also read - https://en.wikipedia.org/wiki/Smith%E2%80%93Waterman_algorithm
3. The broad thing - https://en.wikipedia.org/wiki/Sequence_alignment
4. RunTime improving algorithms - https://en.wikipedia.org/wiki/Method_of_Four_Russians