## LL(1) grammar
```
<expr> -> <term> <expr_tail>
<expr> -> λ <var> . <expr>
<expr_tail> -> <term> <expr_tail>
<expr_tail> -> $

<term> -> ( <expr2>
<term> -> <var>
<expr2> -> <term> <expr2_tail>
<expr2> -> λ <var> . <expr2>
<expr2_tail> -> <term> <expr2_tail>
<expr2_tail> -> )

<var> -> a
<var> -> b
<var> -> c
...
<var> -> z
```

## first set
```
<expr>:
	{"(", <var>, "λ"}

<expr_tail>:
	{"(", <var>, $}

<term>:
	{"(", <var>}

<expr2>:
	{"(", <var>, "λ"}

<expr2_tail>:
	{"(", <var>, ")"}
```

---
## SDD
```python
def expr_merge(left, right):
	if right.first == $:
		return left
	else:
		return expr_merge(
				App(left, right.first),
				right.second)
```
```
1  <expr> -> <term> <expr_tail>
[
<expr>.expr = expr_merge(
				<term>.expr,
				<expr_tail>.pair)
]

2  <expr> -> λ <var> . <expr>
[
<expr>.expr = Abs(<var>.id, <expr>.expr)
]

3  <expr_tail> -> <term> <expr_tail>#0
[
<expr_tail>.pair =
tuple(<term>.expr, <expr_tail>#0.pair)
]

4  <expr_tail> -> $
[
<expr_tail>.pair =
tuple($, $)
]

5  <term> -> ( <expr2>
[<term>.expr = <expr2>.expr]

6  <term> -> <var>
[<term>.expr = <var>.id]

7  <expr2> -> λ <var> . <expr2>#0
[
<expr2>.expr = Abs(<var>.id, <expr2>#0.expr)
]

8  <expr2> -> <term> <expr2_tail>
[
<expr2>.expr = expr_merge(
				<term>.expr,
				<expr2_tail>.pair)
]

9  <expr2_tail> ->
	<term> <expr2_tail>#0
[
<expr2_tail>.pair =
	tuple(<term>.expr,
	  	<expr2_tail>#0.pair)
]

10 <expr2_tail> -> )
[
<expr2_tail>.pair =
tuple($, $)
]
11 <var> -> a | b | c | ... | z
```
