//
//  Reader.cpp
//  neural_net
//
//  Created by remy DEME on 18/10/2017.
//  Copyright © 2017 remy DEME. All rights reserved.
//

#include "Reader.hpp"
#include <array>
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
cv::Size Reader::size_(100,100);

unsigned int Reader::input_vector_index_ = 0;
/**
 *\var index_ variable that indicate the next file
 */
unsigned int Reader::index_ = 0;


bool good_file_format_input(const std::string& input)
{
    const boost::regex expr(INPUT_REGEX<const char*>);
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
    auto index = 0;
    for (const auto &t : tok)
    {
        if (t == "," || t == "" ||t == "]" || t == "[")
            continue;
        boost::char_separator<char> sep{","};
        tokenizer tok_sub{t, sep};
        for (const auto &itt : tok_sub)
            Reader::f_name_[index].push_back(itt);
        ++index;
    }
}

void Reader::save_path(const std::string& path)
{
    Reader::path_ = path;
}

void Reader::createTxtFile(std::string&& file_name, std::vector<double>& inputs)
{
    cv::Mat image ;
    image = cv::imread(file_name.c_str(), CV_LOAD_IMAGE_GRAYSCALE);
    cv::Mat image_tmp;
    image_tmp = image;
    cv::resize(image, image_tmp, Reader::size_);
    if (image_tmp.data)
    {
        for (int i = 0; i < image_tmp.size().height ; i++)
        {
            for (int j = 0; j < image_tmp.size().width; j++)
            {
                double var = double(image_tmp.at<uint8_t>(i, j));
                if (var > 100)
                    var = -1.0;
                else
                    var = 1.0;
                std::cout << var << " ";
                inputs.push_back(var);
            }
            std::cout << "\n";
        }
            
    }
    else
        throw std::runtime_error("File not found for training. Stop the loading !");
}

void Reader::load_file()
{
    for (unsigned i = 0; i < Reader::f_name_.size(); i++)
        for (unsigned j = 0; j < Reader::f_name_[i].size(); j++)
        {
            std::vector<double> image_vector;
            auto image_name (std::string(path_ + "/" +Reader::f_name_[i][j]));
            Reader::input_files_[i].push_back(std::vector<double>());
            createTxtFile(image_name.c_str(), input_files_[i].back());
        }
    /* save the name of all the input files for the training part */

    for (unsigned i = 0; i < input_files_.size(); i++)
    {
        std::vector<double> outputs;
        for (unsigned j = 0; j < input_files_.size(); j++)
        {
            if (j == i)
                outputs.push_back(1.0);
            else
                outputs.push_back(0.0);
        }
        targets_map_[i] = outputs;
    }
}






void Reader::fileProcess(const std::string& file)
{
        if (good_file_format_input(file))
        {
            token_file(file);
        }
        else
            throw std::runtime_error("Bad input format enter ./Net --h for help !");
}

bool good_topologie_input(const std::string& input)
{
    const boost::regex rgx(TOPOLOGIE_REGEX<const char*>);
    boost::smatch what;
    if (boost::regex_search(input, what, rgx))
        return true;
    return false;

}

void Reader::save_size(const std::string& size)
{
    const boost::regex rgx(SAVE_REGEX<const char*>);
    boost::smatch what;
    if (boost::regex_search(size, what, rgx))
        throw std::runtime_error("Bad size argument enter ./Net --h for help");

    using tokenizer = boost::tokenizer<boost::char_separator<char>>;

    boost::char_separator<char> sep{"[],"};
    tokenizer tok{size, sep};
    auto index = 0;
    std::array<int, 2> dims;
    for (const auto &t : tok)
    {
        dims[index] = std::stol(t);
        index++;
    }
    Reader::size_ = cv::Size(dims[0], dims[1]);
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
        if (good_topologie_input(topologie))
            token_topologie(topologie);
        else
            throw std::runtime_error("Bad input look for the help part ./Net -h");
}

void Reader::parseInput(int argc, char *argv[])
{
    using namespace boost::program_options;
    options_description optionDesc{"options"};
    optionDesc.add_options()

        ("help, h", "Help Menu : Net programme launch it!\
         --file \"[(files),...,(files)]\" --t \"[s1, S2, ..., Sn]\" \n")

        /* --file flags specified */ 
        ("file", value<std::string>()->notifier(Reader::fileProcess), 
         "arg = \"[(f1,...[,fn),...,(f1.2,..., fn.1)]\" Caution space(s)\
         are not tolerated and the brackets are riquired\n")

        ("path", value<std::string>()->notifier(Reader::save_path), "Inputs files location.\n")
        ("size", value<std::string>()->notifier(Reader::save_size), "Size of the input file --size \"[h, w]\"")

        /* --t topologie input caracteristics */
        ("t", value<std::string>()->notifier(Reader::topologieProcess),\
         "arg = \"[ layer_size_1, layer_size_2, ...,layer_size_n ]\"");


    variables_map vm;
    store(parse_command_line(argc, argv, optionDesc), vm);
    notify(vm);

    if (vm.count("help"))
        std::cout << optionDesc << "\n";
    if (!(vm.count("file") && vm.count("size") && vm.count("t")))
        throw std::runtime_error("bad input arguments ./Net --help");
    Reader::load_file();
}


const std::vector<double>& Reader::output()
{
    return Reader::targets_map_[Reader::index_];
}

void Reader::incIndex()
{
    Reader::index_++;
    if (Reader::index_ >= input_files_.size())
        Reader::index_ = 0;
}

void Reader::readInput(std::vector<double>& inputs)
{
    auto size = input_files_[Reader::index_].size() - 1;
    auto back = input_files_[Reader::index_].begin() + size;
    inputs = input_files_[Reader::index_][size];
    input_files_[Reader::index_].erase(back);
    input_files_[Reader::index_].push_back(inputs);
}

