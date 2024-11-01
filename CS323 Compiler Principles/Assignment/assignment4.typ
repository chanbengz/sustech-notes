#import "template.typ": *

#show: project.with(
  title: "Compiler Assignment 4",
  authors: (
    (
      name: "陈贲(12212231)",
    ),
  ),
)

Consider the following context-free grammar $G$:
$
  S arrow.r S A | S B | a \
  A arrow.r S + \
  B arrow.r S - 
$

#problem[
#set enum(numbering: "1.")
*(SLR Algorithm)* Please complete the following tasks or answer the questions:
+ Construct the shift-reduce parsing table for the above grammar $G$ using the SLR algorithm. Please put down the detailed steps, including the calculation of item sets. For the calculation of closures, GOTO targets, and FIRST/FOLLOW sets, you may choose to omit the details.
+ Is the grammar $S L R(1)$?
+ Can an SLR parser accept the input string `aaaa+++`? If yes, please list the moves made by the parser; otherwise, state the reason. Before parsing, please resolve conflicts in the parsing table if any.
]

#solution[
The argumented grammar would be
#set math.equation(numbering: "(1)")
$ S' arrow.r S $
$ S arrow.r S A $
$ S arrow.r S B $
$ S arrow.r a $
$ A arrow.r S + $
$ B arrow.r S - $

Then, the FIRST and FOLLOW sets are

#show table.cell.where(y: 0): strong
#set table(
  stroke: (x, y) => if y == 0 {
    (bottom: 1pt + black)
  }
)
#table(
  columns: (1fr, 1fr, 1fr, 1fr, 1fr),
  align: center,
  table.header([], [$S'$], [$S$], [$A$], [$B$]),
  [FIRST], [${a}$], [${a}$], [${a}$], [${a}$],
  [FOLLOW], [${\$, a, +, -}$], [${\$, a, + , -}$], [${\$, a, +, -}$], [${\$, a, +, -}$],
  table.hline(),
)
#set par(first-line-indent: 0em, justify: true)
Thirdly, the CLOSURE of item sets and GOTO targets are the following.

CLOSURE of item set $I_0$:
- Initially, the set $I_0 = {[S' arrow.r dot S]}$
- Add $[S arrow.r dot S A], [A arrow.r dot S B]$ and $[S arrow.r dot a]$ to the closure
- Reach a fixed point

GOTO targets of item set $I_0$:
- $"GOTO"(I_0, S) = "CLOSURE"{[S' arrow.r S dot], [S arrow.r S dot A], [S arrow.r S dot B]} = I_1$
- $"GOTO"(I_0, a) = "CLOSURE"{[S arrow.r a dot]} = I_2$

CLOSURE of item set $I_1$:
- Initially, the set $I_1 = {[S' arrow.r S dot], [S arrow.r S dot A], [S arrow.r S dot B]}$
- Add $[A arrow.r dot S +]$ and $[B arrow.r dot S -]$ to the closure
- Add $[S arrow.r dot S A], [S arrow.r dot S B]$ and $[S arrow.r dot a]$ to the closure

GOTO targets of item set $I_1$:
- $"GOTO"(I_1, A) = "CLOSURE"{[S arrow.r S A dot]} = I_3$
- $"GOTO"(I_1, B) = "CLOSURE"{[S arrow.r S B dot]} = I_4$
- $"GOTO"(I_1, a) = "CLOSURE"{[S arrow.r a dot]} = I_2$
- $"GOTO"(I_1, S) = "CLOSURE"{[S arrow.r S dot A], [S arrow.r S dot B], [A arrow.r S dot +], [B arrow.r S dot -]} = I_5$

CLOSURE and GOTO of item set $I_2$:
- Nothing to add and nowhere to go, so $I_2 = {[S arrow.r a dot]}$

CLOSURE and GOTO of item set $I_3$:
- Nothing to add and nowhere to go, so $I_3 = {[S arrow.r S A dot]}$

CLOSURE and GOTO of item set $I_4$:
- Nothing to add and nowhere to go, so $I_4 = {[S arrow.r S B dot]}$

CLOSURE of item set $I_5$:
- Initially, the set $I_5 = {[S arrow.r S dot A], [S arrow.r S dot B], [A arrow.r S dot +], [B arrow.r S dot -]}$
- Add $[A arrow.r dot S +]$ and $[B arrow.r dot S -]$ to the closure
- Add $[S arrow.r dot S A], [S arrow.r dot S B]$ and $[S arrow.r dot a]$ to the closure

GOTO targets of item set $I_5$:
- $"GOTO"(I_5, S) = "CLOSURE"{[S arrow.r S dot A], [S arrow.r S dot B], [A arrow.r S dot +], [B arrow.r S dot -]} = I_5$
- $"GOTO"(I_5, A) = "CLOSURE"{[S arrow.r S A dot]} = I_3$
- $"GOTO"(I_5, B) = "CLOSURE"{[S arrow.r S B dot]} = I_4$
- $"GOTO"(I_5, a) = "CLOSURE"{[S arrow.r a dot]} = I_2$
- $"GOTO"(I_5, +) = "CLOSURE"{[A arrow.r S + dot]} = I_6$
- $"GOTO"(I_5, -) = "CLOSURE"{[B arrow.r S - dot]} = I_7$

CLOSURE and GOTO of item set $I_6, I_7$:
- Nothing to add and nowhere to go, so $I_6 = {[A arrow.r S + dot]}$ and $I_7 = {[B arrow.r S - dot]}$

So we can contruct the parsing table as follows

#set table(
  stroke: (x, y) => if y == 0 {
    (top: 1pt + black, bottom: 1pt + black)
  } else if y == 1 {
    (bottom: 1pt + black)
  }
)
#table(
  columns: (2fr, 1fr, 1fr, 1fr, 1fr, 1fr, 1fr, 1fr),
  align: center,
  table.vline(),
  table.cell(rowspan: 2, align:horizon, [State]), table.vline(),
  table.cell(colspan: 4, align:center, [`ACTION`]), table.vline(),
  table.cell(colspan: 3, align:center, [`GOTO`]),
  [a], [+], [-], [\$], [S], [A], [B], table.vline(),
  [0], [s2], [], [], [], [1], [], [],
  [1], [s2], [], [], [accept], [5], [3], [4],
  [2], [r4], [r4], [r4], [r4], [], [], [],
  [3], [r2], [r2], [r2], [r2], [], [], [],
  [4], [r3], [r3], [r3], [r3], [], [], [],
  [5], [s2], [s6], [s7], [], [5], [3], [4],
  [6], [r5], [r5], [r5], [r5], [], [], [],
  [7], [r6], [r6], [r6], [r6], [], [], [],
  table.hline()
)

*Q2* The grammar is $"SLR"(1)$ because the parsing table has no conflicts and the grammar is not ambiguous.

*Q3* To parse the input string `aaaa+++`, the moves made by the SLR parser are as follows.

#set table(stroke: 1pt)
#table(
  columns: (1fr, 1fr, 1fr, 2.5fr),
  align: (left, left, right, left),
  table.cell(align: center, [Stack]), table.cell(align: center, [Symbols]), table.cell(align: center, [Input]), table.cell(align: center, [Action]),
  [0], [\$], [`aaaa+++$`], [shift to 2],
  [0 2], [\$ a], [`aaa+++$`], [reduce by $S arrow.r a$ and push 1 by GOTO],
  [0 1], [\$ S], [`aaa+++$`], [shift to 2],
  [0 1 2], [\$ S a], [`aa+++$`], [reduce by $S arrow.r a$ and push 5 by GOTO],
  [0 1 5], [\$ S S], [`aa+++$`], [shift to 2], 
  [0 1 5 2], [\$ S S a], [`a+++$`], [reduce by $S arrow.r a$ and push 5 by GOTO],
  [0 1 5 5], [\$ S S S], [`a+++$`], [shift to 2],
  [0 1 5 5 2], [\$ S S S a], [`+++$`], [reduce by $S arrow.r a$ and push 5 by GOTO],
  [0 1 5 5 5], [\$ S S S S], [`+++$`], [shift to 6],
  [0 1 5 5 5 6], [\$ S S S S +], [`++$`], [reduce by $A arrow.r S +$ and push 3 by GOTO],
  [0 1 5 5 3], [\$ S S S A], [`++$`], [reduce by $S arrow.r S A$ and push 5 by GOTO],
  [0 1 5 5], [\$ S S S], [`++$`], [shift to 6],
  [0 1 5 5 6], [\$ S S S +], [`+$`], [reduce by $A arrow.r S +$ and push 3 by GOTO],
  [0 1 5 3], [\$ S S A], [`+$`], [reduce by $S arrow.r S A$ and push 5 by GOTO],
  [0 1 5], [\$ S S], [`+$`], [shift to 6],
  [0 1 5 6], [\$ S S +], [`$`], [reduce by $A arrow.r S +$ and push 3 by GOTO],
  [0 1 3], [\$ S A], [`$`], [reduce by $S arrow.r S A$ and push 1 by GOTO],
  [0 1], [\$ S], [`$`], [accept],
)
]

#problem[
#set enum(numbering: "1.")
*(CLR Algorithm)*: Please complete the following tasks or answer the questions:
+ Construct the shift-reduce parsing table for the above grammar G using the CLR algorithm. Please put down the detailed steps, including the calculation of item sets. For the calculation of closures, GOTO targets, and FIRST/FOLLOW sets, you may choose to omit the details or reuse the results in the above exercise.
+ Is the grammar $L R(1)$?
+ Can an CLR parser accept the input string `aaaa---`? If yes, please list the moves made by the parser; otherwise, state the reason. Before parsing, please resolve conflicts in the parsing table if any.
]

#solution[
#set par(first-line-indent: 0em, justify: true)
Reusing the results of argumented grammar and FIRST/FOLLOW sets, the CLOSURE of item sets and GOTO targets are the following.

CLOSURE of item set $I_0$:
- Initially, the set $I_0 = {[S' arrow.r dot S, \$]}$
- Add $[S arrow.r dot S A, \$\/a], [S arrow.r dot S B, \$\/a]$ and $[S arrow.r dot a, \$\/a]$ to the closure

GOTO targets of item set $I_0$:
- $"GOTO"(I_0, S) = "CLOSURE"{[S' arrow.r S dot, \$], [S arrow.r S dot A, \$\/a], [S arrow.r S dot B, \$\/a]} = I_1$
- $"GOTO"(I_0, a) = "CLOSURE"{[S arrow.r a dot, \$\/a]} = I_2$

CLOSURE of item set $I_1$:
- Initially, $I_1 = {[S' arrow.r S dot, \$], [S arrow.r S dot A, \$\/a], [S arrow.r S dot B, \$\/a]}$
- Add $[A arrow.r dot S +, \$\/a], [B arrow.r dot S -, \$\/a]$ to the closure
- Add $[S arrow.r dot S A, a\/+\/-], [S arrow.r dot S B, a\/+\/-]$ and $[S arrow.r dot a, a\/+\/-]$ to the closure

GOTO targets of item set $I_1$:
- $"GOTO"(I_1, S) = "CLOSURE"{[A arrow.r S dot +, \$\/a], [B arrow.r S dot -, \$\/a], [S arrow.r S dot A, a\/+\/-], [S arrow.r S dot B, a\/+\/-]} = I_3$
- $"GOTO"(I_1, A) = "CLOSURE"{[S arrow.r S A dot, \$\/a]} = I_4$
- $"GOTO"(I_1, B) = "CLOSURE"{[S arrow.r S B dot, \$\/a]} = I_5$
- $"GOTO"(I_1, a) = "CLOSURE"{[S arrow.r a dot, a\/+\/-]} = I_6$

CLOSURE and GOTO of item set $I_2$:
- Nothing to add and nowhere to go, so $I_2 = {[S arrow.r a dot, \$\/a]}$

CLOSURE of item set $I_3$:
- $I_3 = {[A arrow.r S dot +, \$\/a], [B arrow.r S dot -, \$\/a], [S arrow.r S dot A, a\/+\/-], [S arrow.r S dot B, a\/+\/-]}$
- Add $[A arrow.r dot S +, a\/+\/-]$ and $[B arrow.r dot S -, a\/+\/-]$ to the closure
- Add $[S arrow.r dot S A, a\/+\/-], [S arrow.r dot S B, a\/+\/-]$ and $[S arrow.r dot a, a\/+\/-]$ to the closure

GOTO targets of item set $I_3$:
- $"GOTO"(I_3, S) = "CLOSURE"{[A arrow.r S dot +, a\/+\/-], [B arrow.r S dot -, a\/+\/-], [S arrow.r S dot A, a\/+\/-], [S arrow.r S dot B, a\/+\/-]} = I_7$
- $"GOTO"(I_3, A) = "CLOSURE"{[S arrow.r S A dot, a\/+\/-]} = I_8$
- $"GOTO"(I_3, B) = "CLOSURE"{[S arrow.r S B dot, a\/+\/-]} = I_9$
- $"GOTO"(I_3, a) = "CLOSURE"{[S arrow.r a dot, a\/+\/-]} = I_6$
- $"GOTO"(I_3, +) = "CLOSURE"{[A arrow.r S + dot, \$\/a]} = I_10$
- $"GOTO"(I_3, -) = "CLOSURE"{[B arrow.r S - dot, \$\/a]} = I_11$

CLOSURE and GOTO of item set $I_4$:
- Nothing to add and nowhere to go, so $I_4 = {[S arrow.r S A dot, \$\/a]}$

CLOSURE and GOTO of item set $I_5$:
- Nothing to add and nowhere to go, so $I_5 = {[S arrow.r S B dot, \$\/a]}$

CLOSURE and GOTO of item set $I_6$:
- Nothing to add and nowhere to go, so $I_6 = {[S arrow.r a dot, a\/+\/-]}$

CLOSURE of item set $I_7$:
- $I_7 = {[A arrow.r S dot +, a\/+\/-], [B arrow.r S dot -, a\/+\/-], [S arrow.r S dot A, a\/+\/-], [S arrow.r S dot B, a\/+\/-]}$
- Add $[A arrow.r dot S +, a\/+\/-]$ and $[B arrow.r dot S -, a\/+\/-]$ to the closure
- Add $[S arrow.r dot S A, a\/+\/-], [S arrow.r dot S B, a\/+\/-]$ and $[S arrow.r dot a, a\/+\/-]$ to the closure

GOTO targets of item set $I_7$:
- $"GOTO"(I_7, S) = "CLOSURE"{[A arrow.r S dot +, a\/+\/-], [B arrow.r S dot -, a\/+\/-], [S arrow.r S dot A, a\/+\/-], [S arrow.r S dot B, a\/+\/-]} = I_7$
- $"GOTO"(I_7, A) = "CLOSURE"{[S arrow.r S A dot, a\/+\/-]} = I_8$
- $"GOTO"(I_7, B) = "CLOSURE"{[S arrow.r S B dot, a\/+\/-]} = I_9$
- $"GOTO"(I_7, a) = "CLOSURE"{[S arrow.r a dot, a\/+\/-]} = I_6$
- $"GOTO"(I_7, +) = "CLOSURE"{[A arrow.r S + dot, a\/+\/-]} = I_12$
- $"GOTO"(I_7, -) = "CLOSURE"{[B arrow.r S - dot, a\/+\/-]} = I_13$

CLOSURE and GOTO of item set $I_8$:
- Nothing to add and nowhere to go, so $I_8 = {[S arrow.r S A dot, a\/+\/-]}$

CLOSURE and GOTO of item set $I_9$:
- Nothing to add and nowhere to go, so $I_9 = {[S arrow.r S B dot, a\/+\/-]}$

CLOSURE and GOTO of item set $I_10$:
- Nothing to add and nowhere to go, so $I_10 = {[A arrow.r S + dot, \$\/a]}$

CLOSURE and GOTO of item set $I_11$:
- Nothing to add and nowhere to go, so $I_11 = {[B arrow.r S - dot, \$\/a]}$

CLOSURE and GOTO of item set $I_12$:
- Nothing to add and nowhere to go, so $I_12 = {[A arrow.r S + dot, a\/+\/-]}$

CLOSURE and GOTO of item set $I_13$:
- Nothing to add and nowhere to go, so $I_13 = {[B arrow.r S - dot, a\/+\/-]}$

So we can contruct the parsing table as follows

#show table.cell.where(y: 0): strong
#set table(
  stroke: (x, y) => if y == 0 {
    (top: 1pt + black, bottom: 1pt + black)
  } else if y == 1 {
    (bottom: 1pt + black)
  }
)
#table(
  columns: (2fr, 1fr, 1fr, 1fr, 1fr, 1fr, 1fr, 1fr),
  align: center,
  table.vline(),
  table.cell(rowspan: 2, align:horizon, [State]), table.vline(),
  table.cell(colspan: 4, align:center, [`ACTION`]), table.vline(),
  table.cell(colspan: 3, align:center, [`GOTO`]),
  [a], [+], [-], [\$], [S], [A], [B], table.vline(),
  [0], [s2], [], [], [], [1], [], [],
  [1], [s6], [], [], [accept], [3], [4], [5],
  [2], [r4], [], [], [r4], [], [], [],
  [3], [s6], [s10], [s11], [], [7], [8], [9],
  [4], [r2], [], [], [r2], [], [], [],
  [5], [r3], [], [], [r3], [], [], [],
  [6], [r4], [r4], [r4], [], [], [], [],
  [7], [s6], [s12], [s13], [], [7], [8], [9],
  [8], [r2], [r2], [r2], [], [], [], [],
  [9], [r3], [r3], [r3], [], [], [], [],
  [10], [r5], [], [], [r5], [], [], [],
  [11], [r6], [], [], [r6], [], [], [],
  [12], [r5], [r5], [r5], [], [], [], [],
  [13], [r6], [r6], [r6], [], [], [], [],
  table.hline()
)

*Q2* The grammar is $"LR"(1)$ because the parsing table has no conflicts and the grammar is not ambiguous.

*Q3* To parse the input string `aaaa---`, the moves made by the CLR parser are as follows.

#set table(stroke: 1pt)
#table(
  columns: (1fr, 1fr, 1fr, 2.5fr),
  align: (left, left, right, left),
  table.cell(align: center, [Stack]), table.cell(align: center, [Symbols]), table.cell(align: center, [Input]), table.cell(align: center, [Action]),
  [0], [\$], [`aaaa---$`], [shift to 2],
  [0 2], [\$ a], [`aaa---$`], [reduce by $S arrow.r a$ and push 1 by GOTO],
  [0 1], [\$ S], [`aaa---$`], [shift to 6],
  [0 1 6], [\$ S a], [`aa---$`], [reduce by $S arrow.r a$ and push 3 by GOTO],
  [0 1 3], [\$ S S], [`aa---$`], [shift to 6],
  [0 1 3 6], [\$ S S a], [`a---$`], [reduce by $S arrow.r a$ and push 7 by GOTO],
  [0 1 3 7], [\$ S S S], [`a---$`], [shift to 6],
  [0 1 3 7 6], [\$ S S S a], [`---$`], [reduce by $S arrow.r a$ and push 7 by GOTO],
  [0 1 3 7 7], [\$ S S S S], [`---$`], [shift to 13],
  [0 1 3 7 7 13], [\$ S S S S -], [`--$`], [reduce by $B arrow.r S -$ and push 9 by GOTO],
  [0 1 3 7 9], [\$ S S S B], [`--$`], [reduce by $S arrow.r S B$],
  [0 1 3 7], [\$ S S S], [`--$`], [shft to 13],
  [0 1 3 7 13], [\$ S S S -], [`-$`], [reduce by $B arrow.r S -$ and push 9 by GOTO],
  [0 1 3 9], [\$ S S B], [`-$`], [reduce by $S arrow.r S B$],
  [0 1 3], [\$ S S], [`-$`], [shift to 11],
  [0 1 3 11], [\$ S S -], [`$`], [reduce by $B arrow.r S -$ and push 5 by GOTO],
  [0 1 5], [\$ S B], [`$`], [reduce by $S arrow.r S B$],
  [0 1], [\$ S], [`$`], [accept],
)
]

#problem[
#set enum(numbering: "1.")
*(LALR Algorithm)*: Please complete the following tasks or answer the questions:
+ Construct the shift-reduce parsing table for the above grammar G using the LALR algorithm. Please put down the detailed steps, including the calculation of item sets. For the calculation of closures, GOTO targets, and FIRST/FOLLOW sets, you may choose to omit the details or reuse the results in the above exercises.
+ Is the grammar $L A L R(1)$? 
+ Can an LALR parser accept the input string `aaaa+-+`? If yes, please list the moves made by the parser; otherwise, state the reason. Before parsing, please resolve conflicts in the parsing table if any.
]

#solution[
#set par(first-line-indent: 0em, justify: true)
#show table.cell.where(y: 0): strong
Reusing the results of argumented grammar and FIRST/FOLLOW sets, the CLOSURE of item sets and GOTO targets are the following.

Merging the states of CLR algorithm, we have the following item sets as pairs

$ I_26 = I_2 and I_6 = {[S arrow.r a dot, \$\/a\/+\/-]} $
$ I_37 = I_3 and I_7 = {&[A arrow.r S dot +, \$\/a\/+\/-], \ 
                        &[B arrow.r S dot -, \$\/a\/+\/-], \
                        &[S arrow.r S dot A, a\/+\/-], \
                        &[S arrow.r S dot B, a\/+\/-]} $
$ I_48 = I_4 and I_8 = {[S arrow.r S A dot, \$\/a\/+\/-]} $
$ I_59 = I_5 and I_9 = {[S arrow.r S B dot, \$\/a\/+\/-]} $
$ I_1012 = I_10 and I_12 = {[A arrow.r S + dot, \/\$a\/+\/-]} $
$ I_1113 = I_11 and I_13 = {[B arrow.r S - dot, \/\$a\/+\/-]} $

And for simplicity, we denote the merged item sets as $I_2, I_3, I_4, I_5, I_6, I_7$.

Then, the parsing table is as follows. Obvious, it much resembles LR(0)

#show table.cell.where(y: 0): strong
#set table(
  stroke: (x, y) => if y == 0 {
    (top: 1pt + black, bottom: 1pt + black)
  } else if y == 1 {
    (bottom: 1pt + black)
  }
)
#table(
  columns: (2fr, 1fr, 1fr, 1fr, 1fr, 1fr, 1fr, 1fr),
  align: center,
  table.vline(),
  table.cell(rowspan: 2, align:horizon, [State]), table.vline(),
  table.cell(colspan: 4, align:center, [`ACTION`]), table.vline(),
  table.cell(colspan: 3, align:center, [`GOTO`]),
  [a], [+], [-], [\$], [S], [A], [B], table.vline(),
  [0], [s2], [], [], [], [1], [], [],
  [1], [s2], [], [], [accept], [3], [4], [5],
  [2], [r4], [r4], [r4], [r4], [], [], [],
  [3], [s2], [s6], [s7], [], [3], [4], [5],
  [4], [r2], [r2], [r2], [r2], [], [], [],
  [5], [r3], [r3], [r3], [r3], [], [], [],
  [6], [r5], [r5], [r5], [r5], [], [], [],
  [7], [r6], [r6], [r6], [r6], [], [], [],
  table.hline(),
)

*Q2* The grammar is $"LALR"(1)$ because the parsing table has reduce/reduce and shift/reduce conflicts.

*Q3* To parse the input string `aaaa+-+`, the moves made by the LALR parser are as follows.

#set table(stroke: 1pt)
#table(
  columns: (1fr, 1fr, 1fr, 2.5fr),
  align: (left, left, right, left),
  table.cell(align: center, [Stack]), table.cell(align: center, [Symbols]), table.cell(align: center, [Input]), table.cell(align: center, [Action]),
  [0], [\$], [`aaaa+-+$`], [shift to 2],
  [0 2], [\$ a], [`aaa+-+$`], [reduce by $S arrow.r a$ and push 1 by GOTO],
  [0 1], [\$ S], [`aaa+-+$`], [shift to 2],
  [0 1 2], [\$ S a], [`aa+-+$`], [reduce by $S arrow.r a$ and push 3 by GOTO],
  [0 1 3], [\$ S S], [`aa+-+$`], [shift to 2], 
  [0 1 3 2], [\$ S S a], [`a+-+$`], [reduce by $S arrow.r a$ and push 3 by GOTO],
  [0 1 3 3], [\$ S S S], [`a+-+$`], [shift to 2],
  [0 1 3 3 2], [\$ S S S a], [`+-+$`], [reduce by $S arrow.r a$ and push 3 by GOTO],
  [0 1 3 3 3], [\$ S S S S], [`+-+$`], [shift to 6],
  [0 1 3 3 3 6], [\$ S S S S +], [`-+$`], [reduce by $A arrow.r S +$ and push 4 by GOTO],
  [0 1 3 3 4], [\$ S S S A], [`-+$`], [reduce by $S arrow.r S A$],
  [0 1 3 3], [\$ S S S], [`-+$`], [shift to 7],
  [0 1 3 3 7], [\$ S S S -], [`+$`], [reduce by $B arrow.r S -$ and push 5 by GOTO],
  [0 1 3 5], [\$ S S B], [`+$`], [reduce by $S arrow.r S B$ and push 3 by GOTO],
  [0 1 3], [\$ S S], [`+$`], [shift to 6],
  [0 1 3 6], [\$ S S +], [`$`], [reduce by $A arrow.r S +$ and push 4 by GOTO],
  [0 1 4], [\$ S A], [`$`], [reduce by $S arrow.r S A$ and push 1 by GOTO],
  [0 1], [\$ S], [`$`], [accept],
)
]
