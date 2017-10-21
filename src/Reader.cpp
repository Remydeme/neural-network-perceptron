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
#include <boost/regex.hpp>
#include <boost/tokenizer.hpp>
#include <regex>
#include <cstdlib>
#include <exception>
#include <string>

std::map<int, std::vector<std::string>> Reader::f_name_;
/*Init static var */

unsigned Reader::max_input_ = 0;
std::string Reader::path_{"."};
std::vector<unsigned> Reader::topologie_;
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

bool good_file_format_input(const std::string& input)
{
    const boost::regex expr{"\\[(\\(([\\w ]+,?)+\\)+,?)+\\]"};
    boost::smatch what;
    if (boost::regex_search(input, what, expr))
        return true;
    return false;
}

void Reader::token_file(const std::string& file)
{
    using tokenizer = boost::tokenizer<boost::char_separator<char>>;
    boost::char_separator<char> sep{"()"};
    tokenizer tok{file, sep};
    for (const auto &t : tok)
    {
        if (t == "," || t == "" ||t == "]" || t == "[")
            continue;
        boost::char_separator<char> sep{","};
        tokenizer tok_sub{t, sep};
        auto index = 0;
        for (const auto &itt : tok_sub)
        {
            std::string fileName(itt);
            Reader::f_name_[index].push_back(itt);
            ++index;
        }
    }
}

void Reader::save_path(const std::string& path)
{
    Reader::path_ = path;
}

void readImage(std::string& file_name, std::vector<double> inputs)
{
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
    }

}

void Reader::load_file()
{
    for (unsigned i = 0; i < Reader::f_name_.size(); i++)
    {
        for (unsigned j = 0; j < Reader::f_name_[i].size(); j++) /*cross all the file of each index*/
        {
            std::vector<double> image_vector;
            // here we call the function to load
            // information of the picture in the image_vector
            Reader::input_files_[i].push_back(image_vector); /* put the vector read in the file_input*/
        }
    }
}

void Reader::fileProcess(const std::string& file)
{
    try
    {
        if (good_file_format_input(file))
            token_file(file);
    }
    catch(std::exception& exception)
    {
        std::cout << exception.what();
    }
}

bool good_topologie_input(const std::string& input)
{
    const boost::regex rgx("\\[([\\d ]+,?)+\\]");
    boost::smatch what;
    if (boost::regex_search(input, what, rgx))
        return true;
    return false;

}


void Reader::token_topologie(const std::string& topologie)
{
    using tokenizer = boost::tokenizer<boost::char_separator<char>>;
    boost::char_separator<char> sep{"[],"};
    tokenizer tok{topologie, sep};
    for (const auto &t : tok)
    {
        if (t == "," || t == "" ||t == "]" || t == "[")
            continue;
        std::cout << " out " << t << "\n";
        unsigned value = std::stoi(t);
        topologie_.push_back(value);
    }
}

void Reader::topologieProcess(const std::string& topologie)
{
    try
    {
        if (good_topologie_input(topologie))
            token_topologie(topologie);
        else
            throw std::runtime_error("Bad input look for the help part ./Net -h");
    }
    catch(const std::exception& error)
    {
        std::cout << "bad topologie inputs" << "\n";
        std::cout << error.what() << "\n";
    }
}

void Reader::parseInput(int argc, char *argv[])
{
    using namespace boost::program_options;
    try
    {
        options_description optionDesc{"options"};
        optionDesc.add_options()

            ("help, h", "Help Menu : Net programme launch it!\
             --file \"[(files),...,(files)]\" --t \"[s1, S2, ..., Sn]\" \n")

            /* --file flags specified */ 
            ("file", value<std::string>()->notifier(Reader::fileProcess), 
             "arg = \"[(f1,...[,fn),...,(f1.2,..., fn.1)]\" Caution space(s)\
             are not tolerated and the brackets are riquired\n")

            ("path", value<std::string>()->notifier(Reader::save_path), "\"path\"\n")

            /* --t topologie input caracteristics */
            ("t", value<std::string>()->notifier(Reader::topologieProcess),\
             "arg = \"[ layer_size_1, layer_size_2, ...,layer_size_n ]\"");


        variables_map vm;
        store(parse_command_line(argc, argv, optionDesc), vm);
        notify(vm);

        if (vm.count("help"))
            std::cout << optionDesc << "\n";
        Reader::load_file();
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
   }
 */

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
