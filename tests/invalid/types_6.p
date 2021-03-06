# vim: ft=plasma
# This is free and unencumbered software released into the public domain.
# See ../LICENSE.unlicense

module Types_6

func main() uses IO -> Int {
    list1 = MyCons(1, MyCons(2, MyCons(3, MyCons(4, MyNil))))
    list2 = MyCons("A", MyCons("B", MyCons("C", MyNil)))

    # Type error here because the return type of append isn't constrained
    # enough.  The typechecker should fail to find a unique solution.
    print!(int_to_string(list_length(append(list1, list2))) ++ "\n")
    
    return 0
}

# Demonstrate an abstract type.
type MyList(a) = MyNil | MyCons ( head : a, tail : MyList(a) )

func list_length(l : MyList(t)) -> Int {
    match (l) {
        MyNil -> { return 0 }
        MyCons(_, rest) -> { return 1 + list_length(rest) }
    }
}

func append(l1 : MyList(a), l2 : MyList(b)) -> MyList(c) {
    return MyNil
}

