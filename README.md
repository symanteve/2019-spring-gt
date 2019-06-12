# Graph-Theory-Project-Template
Project template of course - Graph Theory (EE6622E) in National Cheng Kung University.

## How to run

### Linux

```
# clone first (with submodule)
$ git clone --recursive https://github.com/<your-name>/Graph-Theory-Project-Template.git
# (Option) build libfakemn.a
$ cd fake-mininet && make lib && cd .. && cp fake-mininet/libfakemn.a .
# build
$ make 
# run 
$ ./main.out
```

### Windows

If your PC is `window 10`, then you need to install the related dependencies. For example, if you are using `VSCode` as your IDE, then you can install the plugin of `C/C++` in your vscode. And then install the following programs/packages:
* mingw
* git

Step by step to create `c_cpp_properties.json` and `tasks.json`:
* `Ctrl+Shift+P` -> `C/C++: Edit Configuration` -> then you have the first JSON file.
* `Ctrl+Shift+P` -> `Tasks: Configure Task` -> then you have the second JSON file.

Here is the setting of `c_cpp_properties.json` and `tasks.json`:
```json
# c_cpp_properties.json (If your mingw installed in another folder, then you have to change the value in `compilterPath`)
{
    "configurations": [
        {
            "name": "MinGW",
            "intelliSenseMode": "gcc-x64",
            "includePath": [
                "$(workspaceFolder)"
            ],
            "compilerPath": "C:/mingw/bin/gcc.exe",
            "cStandard": "c11",
            "cppStandard": "c++11"
        }
    ],
    "version": 4
}

# tasks.json
{
    "version": "2.0.0",
    "tasks": [
        {
            "label": "build",
            "type": "shell",
            "command": "g++",
            "args": [
                "-g",
                "main.cc",
                "-o",
                "main.exe",
                "-Ifake-mininet/lib",
                "-g3",
                "-L.",
                "-lfakemn_win",
                "-std=c++11"
            ],
            "group": {
                "kind": "build",
                "isDefault": true
            }
        }
    ]
}
```

After all the settings have been done, we can press `Ctrl+Shift+B` to run the task, if your settings is correct, then there will have an executable file which named `main.exe`.

Or you can just [DOWNLOAD a pre-built VM images](http://gofile.me/39GpL/XU5tznyO6) provided by TA.

> [Guide of environment setting](https://hackmd.io/-5WZQC-1QqOeV3KUX65tEw?view) on Windows.

## TODO - Your descriptions about solutions/algorithms/results

Run the program:

```
$make all
$./main.out filename
```

The solutions is based on 5 step:
1. Check strong connectivity
2. Find imbalanced nodes
3. Find additional paths
4. Insert additional paths
5. Specifying the Euler Tour

### Check strong connectivity

If a graph is not strong connectivity, there is a vertex that at least one other vertex can't reach it.
Therefor, the graph has no solution to the CCP.

### Find imbalanced nodes

Imbalanced nodes means that the degree of the node is not 0.

If a node with degree -1, it means the edges flowing in are one less than the edges flowing out.

Usually, the graph has some imbalanced nodes, otherwise the step can be skipped.

### Find additional paths

With the second step result, the imbalanced nodes are collected.

Now, the nodes have to be paired. One positive degree node pairs with one negative node.

Then, we add an additional path to the paired node.

For example, we have two nodes with degree 1, a and b, and two nodes with degree -1 c and d.

A node with degree 1 has to pair with a node with degree -1.

In the example, there are two possible combinations, which is :

1. a with c and b with d, called pair 1

2. a with d and b with c, called pait 2

The more imbalanced nodes in a graph, the more combinations are needs to find.

The practical method is to use permutation function by STL.

`next_permutation(_BidirectionalIterator __first, _BidirectionalIterator __last, _Compare __comp)`
`prev_permutation(_BidirectionalIterator __first, _BidirectionalIterator __last, _Compare __comp)`

There are no needed to permutate both positive degree and negative degree nodes. One kind is enough.

The final goal is to find the combination with the least weight.

Say, we add a edge to node a and node c, and the weight of edge is the least weight path of a and c.

We compare the edge added from pair 1 and pair 2, and choose a pair with least additional weight edge.

To find the least weight path, we have to compare every path.

I use **BFS** to do the job.

### Insert additional paths

Replaced the edge of the node pair with existed edge.

With the example, supposed that pair 1 has the least additional weight.

We add two additional edge which are a to c and b to d.

The additional edges are actually represent a path with the least weight between a to c and b to d.

In the step, we add those edges that construct the path to the original graph.

### Specifying the Euler Tour

After the above steps are done, the original graph turns into a Eulerian graph.

An Eulerian graph means that every node in the graph has degree 0, and exist an Euler Tour.

**Hierholzer** algorithm is applied here.

