***

# Super Fun Day \<RELATIONSHIP\>

There is some silly MetaData, which I never used, but I was required to do:

```
Position: (x,y,z)
Statistic: (level, exp)

```

> The metadata in the Worker must be composed (composition relationship)
>
> The life of this values is dependent on the object itself
>
> It describes a living part of the object, it represents an object component


***

#### Now it gets fun!

> There are Tools -> Hammer & Shovel
>
> There are Workers
>
> There are Workshops


`NOW BECOME A <VALA/MAIA> AND CREATE THE WORLD!`

***

### Rules

- Tool is an abstract concept, you can only have instances of concrete Tools, such as Hammers and Shovels
- A Worker might own none or many tools. (Shovels and Hammers in this universe) (Yes, you should use the polymorphic properties of a Tool) (This is an aggregation -one to many- One Worker can own many tools)
- You must be able to give and remove tools od a worker (Workers often loose their tools and must get a new one... or maybe they find their old tools)
- Tools can be `stolen` or `taken away` from a Worker. If a tool is owned by a worker and it's given to a different worker, the previous will loose this tool
- Workers can't own the ~same~ tool twice (this is impossible in our world for example), a worker might own many different hammers, but he can only own each hammer once.
- If a worker is given the instruction to loose a tool he doesn't own, this has no effect (Show a warning is a good idea)
- A Tool can have one owner (which must be the current worker who has this tool) (This is an aggregation -one to one /w accepts null- A Tool can have one owner and an owner can own the tool once)
- A Tool can also not be owned by anyone (The tool existence is independent of Workers) (This is an aggregation -one to one /w accepts null- For each tool there can be 0/1 owners, owners can have 0/1 times the tool)
- Workers can have no tools (The Workers existence is independent of Tools)
- A Workshop can have many workers (-many to many relationship- association)
- Workshops can register and release workers
- A Workshop is independant of the Workers, but it uses the workers when they are registered
- A Worker doesn't necesarily depends on the Workshop
- A Workshop is can condition the register based on the worker's tools (A workshop may only allow workers with hammers, or workers with shovels, or workers with 'tools')
- Ideally a Workshop should take a Template class type of "Tool", so a workshop can filter workers on the give type: Ex `Workshop<Hammer>()` or `Workshop<Tool>()` unfortunately this is not possible, or at least to my knowledge. Because of `template covariance`. A template cannot convert implemented classes even if they share a common factor (like a Base class). This would be the ideal solution, if only it was possible, see more on why this is not possible [here][https://stackoverflow.com/questions/2203388/templates-polymorphism]
- To solve this I had to implement concrete Workshops.
- The generic Workshop accepts workers with a `Tool` (any kind will do)
- The WoodWorking Workshop accept worker with `Hammer`
- The Shoveling101 Workshop accept worker with `Shovel`
- I hope one day I can think of a better solution
- If a worker looses his tool which is required for a give workshop the worker should automatically be released from the workhops where he can no longer participate

***
