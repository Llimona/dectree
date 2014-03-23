dectree
=======

This is a C++ threaded implementation for the generation of a decision tree.

The implementation is based on the methodology as explained within this set of slides:

http://www.autonlab.org/tutorials/dtree18.pdf

The purpose of this was refreshing my C++ skills, as well as to get familiar with some of the features introduced by C++11.

##Building

To build using g++ a Makefile is provided, the project has no need of external libraries.

##Usage

```
dectree input_file output_attribute_index [n_threads]
```

- *input_file* being a CSV file with a header row and using a semicolon as delimiter.

- *output_attribute_index* is indexed from 0

- *n_threads* defaults to 1 if not provided

##Output

The tool's output is a list of all of the tree's branches in the following format:

```
Attr1 = val1 & Attr2 = val2 & ... & AttrN = valN => OutputAttr = val
```