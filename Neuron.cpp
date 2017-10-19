//
//  Node.cpp
//  neural_net
//
//  Created by remy DEME on 15/10/2017.
//  Copyright © 2017 remy DEME. All rights reserved.
//
/**
 \author Remy DEME
 */

#include <ctime>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include "Neuron.hpp"
/*structure constructor */

connection_s::connection_s(double weight, double delta_weight) : weight_(weight),
delta_weight_(delta_weight)
{
}

/*----------------- static randomweight function --------------------*/

double Neuron::randomWeight()
{
    return rand() / RAND_MAX;
}



/*------------------ constructor --------------*/

Neuron::Neuron(unsigned nextLayerSize, unsigned myIndex) : nextLayerSize_(nextLayerSize),
    myIndex_(myIndex)
{
    for (auto numWeight = 0; numWeight < nextLayerSize; numWeight++)
        outputWeights_.push_back(Connection(Neuron::randomWeight()));
}


/*------- getter and setter --------*/

void Neuron::outputValue_set(double value)
{
    outputValue_ = value;
}

double Neuron::outputValue_get() const
{
    return outputValue_;
}

/*------ backpropagation fucntion ------*/


/*------------------------- feedFunction ----------------------------*/


void Neuron::feedForward(Layer &prevLayer)
{
    double sum = 0;
    for (auto nodeNum = 0; nodeNum < prevLayer.size(); nodeNum++)
    {
        sum += (prevLayer[nodeNum].outputValue_ *
        prevLayer[nodeNum].outputWeights_[myIndex_].weight_);
    }
    outputValue_ = Neuron::transferFunction(sum);
}


/*------------------- Transfert function ---------------------*/


double Neuron::transferFunction(double sum)
{
   //return tanh(sum); /* dont work with this network */
   return 1 / (1 + exp(-sum));
}


/*------------------- Derivative Transfert function ------------------------*/

double Neuron::transferFunctionDerivative(double sum)
{
    //return 1.0 - (sum * sum ); // for tanh
    return sum  * (1.0 - sum); // for 1 / 1 + exp(-X)
}


/*-------------------------- Output Gradient -------------------------------*/

void Neuron::calcOutputGradient(double targetVal) /* compute the delta error */
{
    auto delta = targetVal - outputValue_; /* error compute*/
    gradient_ = delta * Neuron::transferFunctionDerivative(outputValue_); /* delta compute */
}

/*------------------ SUMDOW ------------------*/
 
 double Neuron::sumDow(const Layer &nextLayer)
{
    auto sum = 0.0;
    
    for (auto n = 0; n < nextLayer.size() - 1; ++n)
        sum += (outputWeights_[n].weight_ * nextLayer[n].gradient_);
    return sum;
}


/* ---------- Hidden gradient--------*/
void Neuron::calcHiddenGradient(Layer& nextLayer)
{
    auto dow = sumDow(nextLayer);
    gradient_ = dow  * Neuron::transferFunctionDerivative(outputValue_);
}

/* ----- Update INput weight ---- */

double Neuron::eta_ = 0.7;
double Neuron::alpha_ = 0.5;

void Neuron::updateInputWeight(Layer &prevLayer)
{
    for (auto n = 0; n < prevLayer.size(); ++n)
    {
        auto& neuron = prevLayer[n];
        auto oldDeltaWeight = neuron.outputWeights_[myIndex_].delta_weight_;
        auto newDeltaWeight = eta_
                        *  neuron.outputValue_
                        * gradient_
                        + alpha_
                        * oldDeltaWeight;
        neuron.outputWeights_[myIndex_].delta_weight_= newDeltaWeight;
        neuron.outputWeights_[myIndex_].weight_ += newDeltaWeight;
    }
}
