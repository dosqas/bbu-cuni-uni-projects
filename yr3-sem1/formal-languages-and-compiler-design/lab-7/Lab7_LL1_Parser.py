# -----------------------
# FILE READING
# -----------------------

def load_grammar_from_file(filename):
    grammar = {
        "NONTERMINALS": set(),
        "TERMINALS": set(),
        "STARTSYMBOL": None,
        "PRODUCTIONS": {},
        "ORDEREDPRODUCTIONS": []
    }

    with open(filename, 'r') as f:
        lines = f.readlines()

    reading_productions = False

    for line in lines:
        line = line.strip()
        if not line:
            continue

        if line.startswith("N ="):
            grammar["NONTERMINALS"] = set(line.split("=")[1].strip().split())
            continue

        if line.startswith("E ="):
            grammar["TERMINALS"] = set(line.split("=")[1].strip().split())
            continue

        if line.startswith("S ="):
            grammar["STARTSYMBOL"] = line.split("=")[1].strip()
            continue

        if line.startswith("P ="):
            reading_productions = True
            continue

        if reading_productions and "->" in line:
            lhs, rhs = line.split("->")
            lhs = lhs.strip()
            rhs_parts = [x.strip() for x in rhs.strip().split()]

            if lhs not in grammar["PRODUCTIONS"]:
                grammar["PRODUCTIONS"][lhs] = []

            grammar["PRODUCTIONS"][lhs].append(rhs_parts)
            grammar["ORDEREDPRODUCTIONS"].append((lhs, rhs_parts))

    # Auto-add terminals
    for lhs, rules in grammar["PRODUCTIONS"].items():
        for rhs in rules:
            for sym in rhs:
                if sym != 'epsilon' and sym not in grammar["NONTERMINALS"]:
                    grammar["TERMINALS"].add(sym)

    return grammar

def read_sequence_file(filename):
    with open(filename) as f:
        return f.read().split()


def read_pif_file(filename):
    tokens = []
    with open(filename) as f:
        for line in f:
            line = line.strip()
            if not line:
                continue

            # Remove parentheses
            line = line[1:-1]

            # Split only on the last comma (ST position)
            token, _ = line.rsplit(",", 1)

            token = token.strip()
            tokens.append(token)  # store the terminal name as string

    return tokens

# -----------------------
# FIRST SET
# -----------------------

def compute_first_seq(sequence, grammar, first_set):
    if not sequence:
        return set()

    s = sequence[0]

    if s == 'epsilon':
        return {'epsilon'}

    if s in grammar["TERMINALS"]:
        return {s}

    if s in grammar["NONTERMINALS"]:
        result = set(first_set[s])
        # If epsilon is in FIRST(s), include FIRST of the rest of the sequence
        if 'epsilon' in result and len(sequence) > 1:
            result.remove('epsilon')
            result |= compute_first_seq(sequence[1:], grammar, first_set)
        return result

    return set()


def generate_first(grammar):
    first = {nt: set() for nt in grammar["NONTERMINALS"]}

    changed = True
    while changed:
        changed = False
        for lhs, rules in grammar["PRODUCTIONS"].items():
            for rhs in rules:
                f = compute_first_seq(rhs, grammar, first)
                before = len(first[lhs])
                first[lhs] |= f # Add all new symbols
                if len(first[lhs]) > before:
                    changed = True

    return first

# -----------------------
# FOLLOW SET
# -----------------------

def generate_follow(grammar, first_set):
    follow = {nt: set() for nt in grammar["NONTERMINALS"]}
    follow[grammar["STARTSYMBOL"]].add('$')

    changed = True
    while changed:
        changed = False

        for lhs, rules in grammar["PRODUCTIONS"].items():
            for rhs in rules:
                for i, symbol in enumerate(rhs):

                    if symbol not in grammar["NONTERMINALS"]:
                        continue

                    # Consider the sequence after the symbol
                    beta = rhs[i+1:]
                    beta_first = compute_first_seq(beta, grammar, first_set)

                    before = len(follow[symbol])

                    # Add FIRST(beta) minus epsilon
                    follow[symbol] |= (beta_first - {'epsilon'})

                    # If beta is empty or can derive epsilon, add FOLLOW(lhs)
                    if not beta or 'epsilon' in beta_first:
                        follow[symbol] |= follow[lhs]

                    if len(follow[symbol]) > before:
                        changed = True

    return follow

# -----------------------
# PARSING TABLE
# -----------------------

def add_to_table(table, non_term, term, prod_idx, has_conflicts):
    key = (non_term, term)
    if key in table and table[key] != prod_idx:
        print(f"[CONFLICT] Table[{non_term}, {term}] has {table[key]} vs {prod_idx}")
        has_conflicts[0] = True
    else:
        table[key] = prod_idx


def build_parsing_table(grammar, first_set, follow_set):
    table = {}
    has_conflicts = [False]

    for idx, (lhs, rhs) in enumerate(grammar["ORDEREDPRODUCTIONS"], start=1):
        first_alpha = compute_first_seq(rhs, grammar, first_set)

        # Add entries for terminals in FIRST(rhs)
        for t in first_alpha:
            if t != 'epsilon':
                add_to_table(table, lhs, t, idx, has_conflicts)

        # If epsilon in FIRST(rhs), add entries for terminals in FOLLOW(lhs)
        if 'epsilon' in first_alpha:
            for t in follow_set[lhs]:
                add_to_table(table, lhs, t, idx, has_conflicts)

    return table, has_conflicts[0]

# -----------------------
# PARSER
# -----------------------

def parse(grammar, table, seq):
    input_stack = seq + ['$']
    work = [grammar["STARTSYMBOL"], '$']
    output = []

    while True:
        a = input_stack[0]
        X = work[0]

        if a == '$' and X == '$':
            return output

        if X == a:
            input_stack.pop(0)
            work.pop(0)

        elif (X, a) in table:
            prod_idx = table[(X, a)]
            output.append(prod_idx)
            _, rhs = grammar["ORDEREDPRODUCTIONS"][prod_idx-1]
            work.pop(0)
            if rhs != ['epsilon']:
                for s in reversed(rhs):
                    work.insert(0, s)
        else:
            print(f"ERROR at token {a}, expected from {X}")
            print("Expected:", [t for (nt,t) in table if nt == X])
            return None

class Node:
    def __init__(self, symbol):
        self.symbol = symbol
        self.id = 0
        self.children = []
        self.parent = None


def build_tree(grammar, prod_indices):
    prod_iter = iter(prod_indices)
    return build_rec(grammar, grammar["STARTSYMBOL"], prod_iter)


def build_rec(grammar, symbol, prod_iter):
    node = Node(symbol)

    if symbol in grammar["TERMINALS"] or symbol == "epsilon":
        return node

    try:
        prod_idx = next(prod_iter)
    except StopIteration:
        return node

    _, rhs = grammar["ORDEREDPRODUCTIONS"][prod_idx - 1]

    if rhs == ['epsilon']:
        child = Node('epsilon')
        child.parent = node
        node.children.append(child)
    else:
        for s in rhs:
            c = build_rec(grammar, s, prod_iter)
            c.parent = node
            node.children.append(c)

    return node


def print_tree_table(root):
    queue = [root]
    counter = 1
    while queue:
        n = queue.pop(0)
        n.id = counter
        counter += 1
        queue.extend(n.children)

    print(f"{'Index':<10}{'Symbol':<20}{'Father':<10}{'Sibling':<10}")
    print("-"*55)

    queue = [root]
    while queue:
        n = queue.pop(0)
        father = n.parent.id if n.parent else 0
        sib = 0
        if n.parent:
            i = n.parent.children.index(n)
            if i+1 < len(n.parent.children):
                sib = n.parent.children[i+1].id

        print(f"{n.id:<10}{n.symbol:<20}{father:<10}{sib:<10}")

        queue.extend(n.children)


if __name__ == "__main__":

    print("\n=== REQUIREMENT 1 ===")

    g1 = load_grammar_from_file("Lab7_SeminarGrammar.txt")
    first1 = generate_first(g1)
    follow1 = generate_follow(g1, first1)
    table1, conflict1 = build_parsing_table(g1, first1, follow1)

    print("Conflicts?" , conflict1)

    seq1 = read_sequence_file("Lab7_SeminarSequence.txt")
    res1 = parse(g1, table1, seq1)
    print("Parsed:", res1)

    print("\n=== REQUIREMENT 2 ===")

    g2 = load_grammar_from_file("Lab7_MiniDSLGrammar.txt")
    first2 = generate_first(g2)
    follow2 = generate_follow(g2, first2)
    table2, conflict2 = build_parsing_table(g2, first2, follow2)

    print("Conflicts?" , conflict2)

    seq2 = read_pif_file("Lab7_MiniDSLPIF.txt")
    res2 = parse(g2, table2, seq2)

    if res2:
        root = build_tree(g2, res2)
        print_tree_table(root)
