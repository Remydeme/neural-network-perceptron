//
//  Reader.hpp
//  neural_net
//
//  Created by remy DEME on 18/10/2017.
//  Copyright © 2017 remy DEME. All rights reserved.
//

#pragma once
#include <vector>
#include <map>

template <typename T>
constexpr T END_CHAR = T(']');

template <typename T>
constexpr T DIGIT_LIM = T(4);

class Reader
{

public:
    
    using inputPool = std::vector<std::vector<double>>;
    
public:
    
    static const Reader& getInstance();
    Reader(Reader& ) = delete;
    Reader(const Reader& ) = delete;

private :

    Reader() = default;
  
    
public:
    
    
    static void parseInput(int argc, char *argv[]);
    /**
     \brief this function should only fills the inputs vector no neads of  file name ...}
     */
    
    static void readInput(std::vector<double>&);
    
    /**
     * \brief get the next expected output
     */
    static const std::vector<double>& output();

private:
   

    static void fileProcess(const std::string& );

    static void topologieProcess(const std::string& );

    static void token_file(const std::string& );

    static void token_topologie(const std::string& );

    static void save_path(const std::string& );

    static void load_file();

public :
    
    static std::vector<unsigned> topologie_;

private:
    static std::string path_; 
    static unsigned int index_; 
    static unsigned int max_input_;
    static std::map<int, std::vector<std::string>> f_name_;
    static std::map<int, inputPool> input_files_;
    static std::map<int, std::vector<double>> targets_map_;
};
