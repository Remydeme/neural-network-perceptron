# neural-network-perceptron

## Resume
  This an MLP multilayer neural network.

## Compile 
  Download the source code
  launche make
## Train your net

##### ./Net --file "[(eight.jpg), (one.jpg), (seven.jpg)]" --t "[900,100, 4]" --size "[30,30]"
##### ./Net --file "[(eight.jpg, eight_2.jpg, eight_3.jpg), (one.jpg, one_2.jpg, one_3.jpg), (seven.jpg)]" --t "[900,100, 4]" --size "[30,30]"

#### --file () = list of file of the same pattern.
#### --t [L,L,L] = size of each layer. For an output of 3 variables, you have to enter 4 because you have the bias neurone. So L = (number_wanted + 1).
#### --size "[height, width]" size wanted.
#### --path the location of the files input files. If they are in the current workspace path arg is."".


## dependencies
  
  ### Boost
  ### Opencv
