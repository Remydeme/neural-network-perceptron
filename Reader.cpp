//
//  Reader.cpp
//  neural_net
//
//  Created by remy DEME on 18/10/2017.
//  Copyright © 2017 remy DEME. All rights reserved.
//

#include "Reader.hpp"
#include <fstream>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options.hpp>

std::map<int, std::string> F_NAME;

/**
 *\var index_ variable that indicate the next file
 */
unsigned int Reader::index_ = 0;

const Reader& Reader::getInstance(char *argv[])
{
    static Reader read;
    
    return read;
}

void Reader::parseInput(char *argv[])
{
}


const std::vector<double>& Reader::output()
{
    return Reader::targets_map_[Reader::index_];
}


void Reader::readInput(std::vector<double>& inputs)
{
    auto size =  input_files_[Reader::index_].size() - 1;
    auto back = input_files_[Reader::index_].begin() + size;
   
    
    /* get the next input and move it at the end of the vector */
    inputs = input_files_[Reader::index_][size];
    
    input_files_[Reader::index_].erase(back);
    
    input_files_[Reader::index_].emplace_back(inputs);
    
    if (Reader::index_ >= Reader::max_input_)
        Reader::index_ = 0;
    
}





















/*
 auto file = std::ifstream(file_name, std::ifstream::in);
 if (file)
 {
 bool process = true;
 char garbage = 0;
 file >> garbage;
 while (process && file.good())
 {
 double input = 0.0;
 file >> input; // get the value
 file >> garbage; // get the ','
 if (garbage == END_CHAR<double>)
 {
 process = false;
 }
 inputs.push_back(input); // puts in the vector
 }
 }*/

/*
void Reader::inputOutputGen(std::map<int, std::string>& inputs, std::map<std::string, std::vector<double>>& targets, int range)
{
    std::map<int, std::string> F_NAME;
    f_name_init(F_NAME);
    
    // create our inputs map
    for (int i = 0; i < range; i++)
        inputs[i] = F_NAME.at(i);
    
    // create  our targets map
    std::vector<double> outputs;
    for (int i = 0; i < range; i++)
    {
        for (int j = 0; j < range; j++)
        {
            if (j == i)
                outputs.push_back(1.0);
            else
                outputs.push_back(0.0);
        }
        targets[F_NAME[i]] = outputs;
        outputs.clear();
    }
}*/
