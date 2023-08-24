## Performance test on: Abstract Linked list vs Concrete Linked list

### An abstract linked list consist on using a ``` void * ``` to representate the content, making it versatile.

### A concrete linked list is when you use the ``` struct * ``` durectly to representate the content of the abstract list.

***

### Why?

>	One day I woke up and I had this question on my mind...
>	I believe that an abstract linked list should be faster to iterate than a concrete abstract list.
>	True or False?
>
>	I had this feeling because I think that c works faster with void * rather than with your own data structure *;
>	I had nothing to back up my hipothesis, it is just an ich I have...

***

So I built a simple program to test it.

To test the abstract list I used my Libft, where we developed an abstract linked list.
In orther to run you must comment the function:

> ```void handle_abstraction(simple_struct * ptr);```


> Notice that the argument that it takes is a *simple_struct* pointer.

Then you can compile and run your tests:

./execute <number of iterations>

To test the abstract list I modified my Libft and implemented a concrete struct as content for the linked list.
In orther to run you must replace the Libft with _Libft and comment the function and the struct:

>	```mv Libft _originalLibft```
>	```mb _Libft Libft```

> ```void handle_abstraction(void * ptr);```

> ```simple struct // from my main.c```

> Notice that the argument that it takes is a *void* pointer
> You must comment the simple struct from my main, because I already declared on the Libft headers.

***

## Results

My results are actually shocking:

- Abstract tests
- Concrete tests
- Comparison Table

#### Abstract tests

>	Test with [100000] - Abstract Usage
>	./ft_lst_performance 100000  6.15s user 0.00s system 99% cpu 6.152 total


>	Test with [100000] - Abstract Usage
>	./ft_lst_performance 100000  6.17s user 0.00s system 99% cpu 6.175 total


>	Test with [200000] - Abstract Usage
>	./ft_lst_performance 200000  25.49s user 0.00s system 99% cpu 25.492 total


>	Test with [300000] - Abstract Usage
>	./ft_lst_performance 300000  64.82s user 0.00s system 99% cpu 1:04.83 total

#### Concrete tests

>	Test with [100000] - Concrete Usage
>	./ft_lst_performance 100000  6.13s user 0.00s system 99% cpu 6.135 total


>	Test with [100000] - Concrete Usage
>	./ft_lst_performance 100000  6.12s user 0.00s system 99% cpu 6.126 total


>	Test with [200000] - Concrete Usage
>	./ft_lst_performance 200000  25.45s user 0.00s system 99% cpu 25.477 total


>	Test with [300000] - Concrete Usage
>	./ft_lst_performance 300000  66.61s user 0.03s system 99% cpu 1:06.64 total

#### Table of results

| Iterations | Abstract | Concrete |
| ------ | ------ | ------ |
| 100000(1) | 6.152 total | 6.135 total |
| 100000(2) | 6.175 total | 6.126 total |
| 200000 | 25.492 total | 25.477 total |
| 300000 | 1:04.83 total | 1:06.64 total |

## Conclusion

#### Don't create concrete linked lists, it's absolutely a waste of time.
#### Just use your Libft...





