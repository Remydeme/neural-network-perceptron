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
#include <boost/algorithm/string.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

template <typename T>
constexpr T END_CHAR = T(']');

template <typename T>
constexpr T DIGIT_LIM = T(4);

/**/
template <typename S>
constexpr S INPUT_REGEX = "\\[(\\([\\w.]+(, ?[\\w.]+)*\\)(,[ ]*\\([ ]*[\\w.]+([ ]*,[ ]*[\\w.]+[ ]*)*[ ]*\\))*[ ]*?)+\\]";

template <typename S>
constexpr S TOPOLOGIE_REGEX = "\\[([ ]*(\\d)+[ ]*([ ]*,[ ]*[\\d]+[ ]*)*)+\\]";

template <typename S>
constexpr S SAVE_REGEX = "\\[[ ]*\\d[ ]*,[ ]*\\d[ ]*\\]";

class Reader
{

public:
    
    using inputPool = std::vector<std::vector<double>>;
    
public:
    
    Reader(Reader& ) = delete;
    Reader(Reader&& ) = delete;
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
    
    /**
    *\fnc 
    * \brief increment the index_ var 
    */
    static void incIndex();

private:
   
   
    static void fileProcess(const std::string& );

    static void topologieProcess(const std::string& );

    static void token_file(const std::string& );

    static void token_topologie(const std::string& );

    static void save_path(const std::string& );

    static void load_file();

    static void createTxtFile(std::string&&, std::vector<double>& );

    static void save_size(const std::string& );

public :
    
    static std::vector<unsigned> topologie_;

private:
    static unsigned int index_;
    static std::string path_; 
    static unsigned int input_vector_index_;
    static unsigned int max_input_;
    static std::map<int, std::vector<std::string>> f_name_;
    static std::map<int, inputPool> input_files_;
    static std::map<int, std::vector<double>> targets_map_;
    static cv::Size size_; 
};
