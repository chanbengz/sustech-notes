#import "template.typ": *
#import "@preview/cetz:0.2.2"

#show: project.with(
  title: "Compiler Assignment 3",
  authors: (
    (
      name: "陈贲(12212231)",
    ),
  ),
)

#problem[
#set enum(numbering: "1.")
  Consider the following context-free grammar $G$:
  $ S arrow.r a S b S | b S a S | epsilon $
  + Is the string "aabbab" a valid sentence in $L(G)$?
  + Give a leftmost derivation for the string "aabbab".
  + Give a rightmost derivation for the string "aabbab".
  + Give a parse tree for the string "aabbab".
]

#solution[
Yes, the string "aabbab" is a valid sentence in $L(G)$ since it can be derived from the grammar $G$ with details as follows. A leftmost derivation for the string "aabbab":
$
  S arrow.r.double a S b S arrow.r.double a a S b S b S arrow.r.double a a b S b S arrow.r.double a a b b S a S b S arrow.r.double a a b b a S b S arrow.r.double a a b b a b S arrow.r.double a a b b a b
$

A rightmost derivation for the string "aabbab":
$
  S arrow.r.double a S b S arrow.r.double a S b S a S b S arrow.r.double a S b S a S b arrow.r.double a S b S a b arrow.r.double a S b S a b arrow.r.double a S b a b arrow.r.double  a a S b S b a b \ arrow.r.double a a S b S b a b arrow.r.double a a S b b a b arrow.r.double a a b b a b
$

A parse tree of the leftmost derivation for the string "aabbab":
#align(center, cetz.canvas({
  import cetz.tree
  import cetz.draw: *

  set-style(content: (padding: .2))

  cetz.tree.tree((
    [S], [a],
    (
      [S], 
      [a],
      ([S], [$epsilon$]),
      [b], 
      ([S], [b], ([S], [$epsilon$]), [a], ([S], [$epsilon$]) )
    ),
    [b], ( [S], [$epsilon$] )
  ),
)}))
]

#problem[
#set enum(numbering: "1.")
  Consider the above grammar $G$.
  + Conpute the FIRST and FOLLOW sets for $G$.
  + Construct the predictive parsing table for $G$.
  + Is the grammar $L L(1)$?
  + Can an $L L(1)$ parser accept the input string _ababab_? If yes, please list the moves made by the parse; othewise, please state the reason. Before parsing, you may need to resolve the conflicts in the parsing table.
]

#solution[
#set enum(numbering: "1.")
  Calculate the FIRST sets:
  + For terminals, $"FIRST"(a) = {a}$, $"FIRST"(b) = {b}$.
  + From $S arrow.r epsilon$, $epsilon in "FIRST"(S)$.
  + $"FIRST"(a S b S) = "FIRST"(a) = {a}$ and $"FIRST"(b S a S) = "FIRST"(b) = {b}$.
  + From $S arrow.r a S b S | b S a S$, $"FIRST"(S) = {a, b}$. So $"FIRST"(S) = {a, b, epsilon}$.

  and the FOLLOW sets:
  + Add $\$$ to $"FOLLOW"(S)$.
  + From $S arrow.r a S b S$, $"FOLLOW"(S) = {b}$ since $S$ may produce $epsilon$, add $"FIRST"(b) = {b}$.
  + From $S arrow.r b S a S$, $"FOLLOW"(S) = {a}$ since $S$ may produce $epsilon$, add $"FIRST"(a) = {a}$.
  + From both, since $beta = b S | a S$, $"FIRST"(S) in "FOLLOW"(S) = {\$, a, b}$.

  Finally, we can construct the predictive parsing table with the FIRST and FOLLOW sets:
  #align(center, table(
    columns: (auto, 1fr, 1fr, 1fr),
    align: horizon,
    [NON-TERMINAL], [a], [b], [\$],
    [S], [$S arrow.r a S b S$ \ $S arrow.r epsilon$], [$S arrow.r b S a S$ \ $S arrow.r epsilon$], [$S arrow.r epsilon$],
  ))

  Apparently, the grammar is not $L L(1)$ since there are conflicts in the parsing table and it will produce multiple parse trees, which means the grammar is ambiguous. 

  So we have to rewrite the grammar to make it $L L(1)$ and it's neither left-recursive nor left-factored. Informally speaking, the grammar matches any number of paired $a$ and $b$ in any order, meanning that the language is $L = {a^n b^n | n >= 0}$. The grammar can be rewritten as:
$
  S arrow.r a E S | b F S | epsilon \
  E arrow.r b | a E E \ 
  F arrow.r a | b F F
$
so we have
$
  "FIRST sets: " S: {a, b, epsilon}, E: {a, b}, F:{a, b} \
  "FOLLOW sets: " S: {\$}, E: {a, b, \$}, F: {a, b, \$}
$
and we can construct the parsing table:
#align(center, table(
    columns: (auto, 1fr, 1fr, 1fr),
    align: horizon,
    [NON-TERMINAL], [a], [b], [\$],
    [S], [$S arrow.r a E S$], [$S arrow.r b F S$], [$S arrow.r epsilon$],
    [E], [$E arrow.r a E E$], [$E arrow.r b$], [],
    [F], [$E arrow.r a$], [$E arrow.r b F F$], [],
  ))
Finally, parsing the string "ababab" with the $L L(1)$ parser:
#table(
    stroke: none,
    columns: (1fr, 1fr, 1fr, 1fr),
    align: center,
    table.hline(),
    table.header([MATCHED], [STACK], [INPUT], [ACTION]),
    table.hline(),
    [], [$S\$$], [ababab\$], [],
    [], [$a E S\$$], [ababab\$], [output $S arrow.r a E S$],
    [a], [$E S\$$], [babab\$], [match *a*],
    [a], [$b S\$$], [babab\$], [output $E arrow.r b$],
    [ab], [$S\$$], [abab\$], [match *b*],
    [ab], [$a E S\$$], [abab\$], [output $S arrow.r a E S$],
    [aba], [$E S\$$], [bab\$], [match *a*],
    [aba], [$b S\$$], [bab\$], [output $E arrow.r b$],
    [abab], [$S\$$], [ab\$], [match *b*],
    [abab], [$a E S\$$], [ab\$], [output $S arrow.r a E S$],
    [ababa], [$E S\$$], [b\$], [match *a*],
    [ababa], [$b S\$$], [b\$], [output $E arrow.r b$],
    [ababab], [$S\$$], [\$], [match *b*],
    [ababab], [$\$$], [\$], [output $S arrow.r epsilon$],
    table.hline()
  )
]

#problem[
  Consider the following context-free grammar $G$:
  $ S arrow.r S S + | S S - | a $
  Is it possible, by modifying the grammar in any way, to construct a predictive parser for the language $L(G)$? If yes, please modify the grammar and provide the predictive parsing table. Otherwise, please state the reason.
]

#solution[
  The grammar is not ambiguous but it is left-recursive and left-factored, so we can rewrite the grammar by eliminating the left-recursion
  $
  S arrow.r S S A | a \ 
  A arrow.r + | -
  $
  and eliminate left-factored
  $
    S arrow.r a B \ 
    B arrow.r S A B | epsilon \
    A arrow.r + | -
  $
  and finally eliminate the indirect recursion
  $
    S arrow.r a B \ 
    B arrow.r a B A B | epsilon \
    A arrow.r + | -
  $
  so we have the FIRST and FOLLOW sets
  $
    "FIRST sets: " S: {a}, B: {a, epsilon}, A: {+, -} \
    "FOLLOW sets: " S: {\$}, B: {+, -, \$}, A: {\$}
  $
  and the parsing table
  #align(center, table(
    columns: (auto, 1fr, 1fr, 1fr, 1fr),
    align: horizon,
    [NON-TERMINAL], [a], [+], [-], [\$],
    [S], [$S arrow.r a B$], [], [], [],
    [B], [$B arrow.r a B A B$], [$B arrow.r epsilon$], [$B arrow.r epsilon$], [$B arrow.r epsilon$],
    [A], [], [$A arrow.r \+$], [$A arrow.r \-$], []
  ))
]
