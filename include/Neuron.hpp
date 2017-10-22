//
//  Node.hpp
//  neural_net
//
//  Created by remy DEME on 15/10/2017.
//  Copyright © 2017 remy DEME. All rights reserved.
//
/**
 \author Remy DEME
 \version 0.1
*/

#pragma once

#include <vector>

/* MACRO */

template <typename T>
constexpr T RD_MAX = T(1);



/* ------- Connection structure ------------- */

/**
 \struct connection_s Neuron.hpp "connection struct represent a synapse."
 */
struct connection_s
{
    connection_s(double weight = 0, double delta_weight = 0);
    double weight_;
    double delta_weight_;
};

using Connection = struct connection_s;

/*----------------- NEURON --------------------*/

class Neuron;

using Layer = std::vector<Neuron>;

class Neuron
{
   
public:
    
    Neuron(unsigned nextLayerSize, unsigned myIndex);
    
public:
    
    
    void outputValue_set(double);
    double outputValue_get() const;

public:
    
    /**
     \fn
     \brief void this function compute the opuputvalue by suming the previous layer output cross
     the weight
     \param prevLayer Layer
     */
    void feedForward(Layer& prevLayer);
    
    /**
     *\fn void calcOutputGradient(double )
     *\param targetVal contains the value expected
     *\brief compute the delta for each node of the output layer
     */
    void calcOutputGradient(double targetVal);
    
    /**
     *\fn void calcHiddenGradient(Layer& )
     *\param nextLayer Next layer
     *\brief this function compute the gradient
     *of the neuron in the Hidden layer
     */
    
    void calcHiddenGradient(Layer& nextLayer);
    /**
     *\fn  void updateInputWeight(Layer&)
     *\param prevLayer this is the previous layer
     *\brief this function update the weight of each node of the
     * previous layer. The weight and the deltaWaight that linked the current node
     * to the previous layer are updated
     */
    void updateInputWeight(Layer& prevLayer);
    /**
     *\fn void sumDow (const Layer& )
     *\param nextLayer this argument is the previous layer starting from the output layer
     *\brief this function is called by updateInputWeight func
     * this function compute the sum of the product of the weight of the current layer
     * node cross by the gradient of all the previous layer node
     */
     /*
              O ->
              O ->  O -> output
              O ->
      */
    double sumDow(const Layer& nextLayer);
    
private :
    /**
     \fn static double randomWeight(void)
     \brief this function return a random number [0 - 1.0]
     */
    static double randomWeight();
    /**
     * \fn static double transferFunction(double )
     * \param sum is the sum of the previous layer weight and outputValue
     * \brief this function is a sigmoid function that simulate the synapse output
     *
    */
    static double transferFunction(double sum);
    /**
     *\fn static double transferFunctionDerivative(double )
     *\param sum is the sum of the previous layer weight and outputValue
     *\brief this function is used to compute the delta
     */
    static double transferFunctionDerivative(double sum);

public :
    /**
     *\var static double eta_ speed learning parameter
     */
    static double eta_; /* the speed of learning */
    /**
     *\var static double alpha_ Momentum argument
     */
    static double alpha_; /* Momentum argument*/
    
private:
    
    double outputValue_;
    double gradient_;
    std::vector<Connection> outputWeights_;
    unsigned nextLayerSize_ ;
    unsigned myIndex_;
};
