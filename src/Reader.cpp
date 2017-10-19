//
//  Reader.cpp
//  neural_net
//
//  Created by remy DEME on 18/10/2017.
//  Copyright © 2017 remy DEME. All rights reserved.
//

#include "Reader.hpp"
#include <fstream>
#include <iostream>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options.hpp>

std::map<int, std::string> F_NAME;
/*Init static var */

unsigned Reader::max_input_ = 0;
std::map<int, Reader::inputPool> Reader::input_files_;
std::map<int, std::vector<double>> Reader::targets_map_;

/**
 *\var index_ variable that indicate the next file
 */
unsigned int Reader::index_ = 0;

const Reader& Reader::getInstance()
{
    static Reader read;
    
    return read;
}

bool good_format(const std::string& input)
{
    boost::regex expr{"\[(\(([\w ]+,?)+\)*,?)+\]"};
    std
   /*
    boost::macth_results<std::string::const_iterator> what;
    auto options = boost::match_default | boost::match_partial;
    if (0 == boost::regex_match(input, what, e, options))
        throw std::runtime_error("The input is not valid. See the help command ");
    if (what[0].matched)
        return true;
    else
        return false;
        */
}
void Reader::fileProcess(const std::string& file)
{
  if (good_format(file))
      std::cout << "file" <<file << "\n";
  else
      std::cout << "Not" << "\n";
}

void Reader::topologieProcess(const std::string& topologie)
{
    std::cout << "file " << topologie << "\n"; 
}

void Reader::parseInput(int argc, char *argv[])
{
    using namespace boost::program_options;
    try
    {
        options_description optionDesc{"options"};
        optionDesc.add_options()
        ("help, h", "Help Menu ")
        ("file", value<std::string>()->notifier(Reader::fileProcess), "File")
        ("t", value<std::string>()->notifier(Reader::topologieProcess), "Topologie");

        variables_map vm;
        store(parse_command_line(argc, argv, optionDesc), vm);
        notify(vm);

        if (vm.count("help"))
            std::cout << optionDesc << "\n";
    }
    catch (const error& exception)
    {
        std::cout << exception.what();
    }
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
