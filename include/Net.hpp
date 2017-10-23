//
//  Net.hpp
//  neural_net
//
//  Created by remy DEME on 15/10/2017.
//  Copyright © 2017 remy DEME. All rights reserved.
//
#pragma once

#include <vector>

#include "Neuron.hpp"

template <typename T>
T THREAD_MAX = T(7);

using Layer = std::vector<Neuron>;

class Net
{
 public :
    /*
         {3, 2, 1}
         means that they will be 3 input nodes
         2 node in the hidden layer and
         one output layer
     */
    Net(std::vector<unsigned>& topologie);
    
 public:
    /**
     *\fn void feedForward(const std::vector<double>& )
     *\param inputVals contains all the inputs vals
     */
    void feedForward(const std::vector<double>& inputVals);
    /**
     *\fn void backProp(const std::vector<double>&)
     *\param targetVals expected values
     *\brief This function implements the backprop update the weight and delta
     */
    void backProp(const std::vector<double>& targetVals);
    /**
     *\fn void getResults(std::vector<double>& )
     */
    void getResults(std::vector<double>& res) const;
private:
    
    void calc_layer_weight(Layer&, Layer& );
    void treat_hidden_neurone(Layer& , Layer&);
    void feed_neurone(Layer&, Layer&);

private:
    
    std::vector<unsigned> topologie_; // network configuration
    std::vector<Layer> layers_; // layers_[layer_num][node_index]
    double mError_ = 0.0;
    unsigned neurone_index_ = 0;
    
};
