//
//  main.cpp
//  neural_net
//
//  Created by remy DEME on 15/10/2017.
//  Copyright © 2017 remy DEME. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <map>
#include <exception>

#include "Net.hpp"
#include "Reader.hpp"

int main(int argc, char *argv[])
{ try
   {
    if (argc >= 2)
    {
               Reader::parseInput(argc, argv);
        std::cout << "Parsing DOne !" << std::endl;
        Net my_net{Reader::topologie_};
        auto i = 0;
        while (i < 6000)
        {
            std::cout << "starting loop " << std::endl;
            std::vector<double> inputs;
            std::vector<double> res;
            std::cout << "Inputs start " << "\n";
            Reader::readInput(inputs);
            std::cout << "Inputs read" << std::endl;;
            my_net.feedForward(inputs);
            std::cout << "feedForward done" << std::endl;
            std::cout << "target get " << std::endl;
            my_net.backProp(Reader::output());
            std::cout << "Backprop done" << std::endl;
            my_net.getResults(res);
            std::cout << "\n \n expected ";
            for (const auto &it : Reader::output())
                std::cout << it << " ";
            std::cout << "\n ";
            std::cout << "\n  res ";
            for (auto it : res)
                std::cout << it << " ";
            std::cout << "\n \n";
            Reader::incIndex();
            i++;
        }
    }
    else
        std::cout << " Not enough arguments launch ./Net --h for help. ";
    }
    catch(std::exception& error)
    {
        std::cout << error.what() << "\n";
    }
}

