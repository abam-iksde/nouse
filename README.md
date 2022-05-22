# NOUSE
Nouse is a scripting language and interpreter/virtual machine
written as an exercise. It's not meant to be used in real life
(not in it's current state at least), due to how slow and unreliable
it can be. Hence its name: Nouse - no use.

Despite Nouse's drawbacks, its syntax/structure is still rather interesting.

## Overview
Variables in Nouse are created by simply assiging a value to a name;
```
my_variable = 3;
```
You can use the ``let`` keyword to declare your variable in the current scope.
```
let my_variable = 5;
true ? {
  let my_variable = 45; /* this is a different variable */
};
print(my_variable); /* will print 5 */
```

The most important thing about Nouse is that every ``{}`` is a function declaration.
All code blocks are functions. To declare a function, you can declare a function by storing it in a variable.
```
print_hello = {
  println("Hello, World!");
};
print_hello();
```

You can make your function take arguments with built in ``args`` function.
```
print_hello = {
  args(let name);
  println(format("Hello, %v!", name));
};
print_hello("Brian");
```

2 constructs can currently be used to control program flow.
``?`` operator works like an ``if`` statement.
```
(b > 20) ? {
  println("b is greater than 20");
};
```
``do`` operator works like a ``while`` loop.
```
let i = 0;
{i < 20} do {
  println(i);
  i = i + 1;
};
```

Functions return the value of their last instruction.
```
my_function = {
  2115;
};
print(my_function()); /* will print 2115 */
```

Regularly calling a function creates a new scope for it. Using ``@`` and ``@@`` operators, functions can be called in the current scope.
```
my_function = {
  println(v);
  let x = 5;
};
let v = "Hello";
my_function(); /* will print 'none' */
my_function@(); /* will print 'Hello' */
my_function@@(); /* will print 'Hello' and create a variable called 'x' with the value of 5 in the current scope */
```

Objects can be created with ``obj`` function.
```
my_object = obj();
my_object.thing = "Hello";

Car = {
  args(let honk_sound);
  result = obj();
  result.honk_sound = honk_sound;
  result.honk = {
    println(this().honk_sound);
  };
  result;
};
my_car = Car("meep meep");
my_car:honk(); /* you can call methods using ':' notation */
```

This is a summary of most basic Nouse features. For more examples, see the "examples" directory.
