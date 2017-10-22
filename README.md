# neural-network-perceptron

##Resume
  
  This is an MLP multilayer neural network.
Note that the implementation is not optimised. The complexity for 3 layers with L1, L2 and L3 containing x, y, z neurons respectively the complexity will be C = (x * y + y * z + z) 

##Compile 
Download the source code
launche make
## Train your net
./Net --file "[(eight.jpg), (one.jpg), (seven.jpg)]" --t "[900,100, 4]" --size "[30,30]"

###--file () = list of file of the same pattern
###--t [L,L,L] = size of each layer. For an output of 3 variables, you have to enter 4 because you have the bias neurone.
### So L = (number_wanted + 1)
###--size "[height, width]" size wanted
###--path the location of the files input files. If they are in the current workspace path arg is.""
