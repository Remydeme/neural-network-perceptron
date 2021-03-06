//
//  Net.cpp
//  neural_net
//
//  Created by remy DEME on 15/10/2017.
//  Copyright © 2017 remy DEME. All rights reserved.
//

#include <cassert>
#include <cmath>
#include <iostream>
#include "Net.hpp"
#include <thread>
#include <mutex>

/* OK */

std::mutex mu_;

Net::Net(std::vector<unsigned >& topologie) : topologie_(topologie)
{
    /* init our layer */
    const auto layer_size = topologie.size();
    for (unsigned layer_num = 0; layer_num < layer_size; layer_num++)
    {
        layers_.push_back(Layer());

        /* add our nodes to the layers */


        /* get the index of the number of neurone in the next layer*/
        auto NextLayerSize = layer_num == layer_size - 1 ? 0 : topologie_[layer_num + 1];


        for (unsigned node_num = 0; node_num < topologie[layer_num]; node_num++)
            layers_.back().push_back(Neuron(NextLayerSize, node_num));
        //if (layer_num + 1 < layer_size)
        layers_.back().back().outputValue_set(1.0);
    }
}

void Net::feed_neurone(Layer& layer, Layer& prevLayer)
{
    bool end = false;
    while (!end)
    {
      std::unique_lock<std::mutex> lock(mu_);
        if (neurone_index_ >= layer.size() - 1)
        {
            end = true;
            continue;
        }
        auto &next = layer[neurone_index_];
        neurone_index_++;
        lock.unlock();
        next.feedForward(prevLayer);
    }
}

/*NOT*/
void Net::feedForward(const std::vector<double> &inputVals)
{
#if defined(DEBUG)
    //  assert(inputVals.size() == (layers_.size() - 1));
#endif
    /* the output of the first layer is the input value */
    for (unsigned nodeNum = 0; nodeNum < inputVals.size(); nodeNum++)
        layers_[0][nodeNum].outputValue_set(inputVals[nodeNum]);
    /* feedForward on the other nodes */
    for (unsigned layerNum = 1; layerNum < layers_.size(); layerNum++) /*fix here remove - 1*/

    {
        auto prevLayer = layers_[layerNum - 1];
        /*Here we call the function feedForward that will computer the output of each node base on the previous layer
rem: we put - 1 because the bias node is not take in count */
         std::vector<std::thread> pool;
         for (auto i = 0; i < THREAD_MAX<int>; i++)
            pool.push_back(std::thread(&Net::feed_neurone, this,
            std::ref(layers_[layerNum]), std::ref(prevLayer)));
        for (auto i = 0; i < THREAD_MAX<int>; i++)
            pool[i].join();
        neurone_index_ = 0;
    }
}


void Net::treat_hidden_neurone(Layer& layer, Layer& nextLayer)
{
    bool end = false;
    while (!end)
    {
      std::unique_lock<std::mutex> lock(mu_);
        if (neurone_index_ >= layer.size())
        {
            end = true;
            continue;
        }
        auto &next = layer[neurone_index_];
        neurone_index_++;
        lock.unlock();
        next.calcHiddenGradient(nextLayer);
    }
}

void Net::calc_layer_weight(Layer& layer, Layer& prevLayer)
{
    bool end = false;
    while (!end)
    {
      std::unique_lock<std::mutex> lock(mu_);
        if (neurone_index_ >= layer.size() - 1)
        {
            end = true;
            continue;
        }
        auto &next = layer[neurone_index_];
        neurone_index_++;
        lock.unlock();
        next.updateInputWeight(prevLayer);
    }
}


/*Update the weight */

void Net::backProp(const std::vector<double> &targetVals)
{
    /*RMS error compute */
    Layer& outputLayer = layers_.back();
    mError_ = 0.0;
    for (unsigned numNode = 0; numNode < outputLayer.size(); numNode++)
    {
        auto delta = targetVals[numNode] - outputLayer[numNode].outputValue_get();
        mError_ += (delta * delta);
    }
    mError_ /= double(outputLayer.size());
    /* we have compute the delta error */
    mError_ = sqrt(mError_);

    /*Output layer gradient */

    for (unsigned n = 0; n < outputLayer.size() - 1; n++)
    {
        outputLayer[n].calcOutputGradient(targetVals[n]); /* To check */
    }

    /*calculate gradient for the hidden layer */
    /*
       We no have delta_error for the output layer so we can now compute
       the error of the hidden layer
rem: - 2 because we start at the hidden layer ides : outputlayer_index - 1
     */
    for (unsigned layerNum = layers_.size() - 2; layerNum > 0; --layerNum)
    {
        auto& layerHidden = layers_[layerNum];
        auto& layerNext = layers_[layerNum + 1];

       std::vector<std::thread> pool;
        for (auto i = 0; i < THREAD_MAX<int>; i++)
            pool.push_back(std::thread(&Net::treat_hidden_neurone, this, std::ref(layerHidden)
            , std::ref(layerNext)));
        for (auto i = 0; i < THREAD_MAX<int>; i++)
            pool[i].join();
            neurone_index_ = 0;
        /*for (unsigned nodeNum = 0; nodeNum < layerHidden.size(); nodeNum++)
           layerHidden[nodeNum].calcHiddenGradient(layerNext);*/
    }

    /*forward prop*/

    for (unsigned layerNum = layers_.size() - 1; layerNum > 0; --layerNum)
    {
        auto& layer = layers_[layerNum];
        auto& prevLayer = layers_[layerNum - 1];

        /* update the weight of all the neuron but the bias neurons*/
        std::vector<std::thread> pool;
         for (auto i = 0; i < THREAD_MAX<int>; i++)
            pool.push_back(std::thread(&Net::calc_layer_weight, this,std::ref(layer),
            std::ref(prevLayer)));
        for (auto i = 0; i < THREAD_MAX<int>; i++)
            pool[i].join();
        neurone_index_ = 0;
      /*  for (unsigned n = 0; n < layer.size() - 1; n++)
            layer[n].updateInputWeight(prevLayer);*/
    }
}

void Net::getResults(std::vector<double>& res) const
{
    res.clear();
    for (unsigned n = 0; n < layers_.back().size(); n++)
        res.push_back(layers_.back()[n].outputValue_get());
}

