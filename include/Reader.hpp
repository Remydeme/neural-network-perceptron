//
//  Reader.hpp
//  neural_net
//
//  Created by remy DEME on 18/10/2017.
//  Copyright © 2017 remy DEME. All rights reserved.
//

#pragma once

#include <boost/algorithm/string.hpp>
#include <map>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>

template <typename T>
constexpr T END_CHAR = T(']');

template <typename T>
constexpr T DIGIT_LIM = T(4);

/**
*\brief regex use for the the file parsing  
*/
template <typename S>
constexpr S INPUT_REGEX = "\\[(\\([\\w.]+(, ?[\\w.]+)*\\)(,[ ]*\\([ ]*[\\w.]+([ ]*,[ ]*[\\w.]+[ ]*)*[ ]*\\))*[ ]*?)+\\]";

/**
*\brief regex use for the topologie parsing 
*/
template <typename S>
constexpr S TOPOLOGIE_REGEX = "\\[([ ]*(\\d)+[ ]*([ ]*,[ ]*[\\d]+[ ]*)*)+\\]";
/**
*\brief regex use for the save parsing 
*/
template <typename S>
constexpr S SAVE_REGEX = "\\[[ ]*\\d[ ]*,[ ]*\\d[ ]*\\]";


/**
*\class 
*/
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
    *\fn
    * \brief increment the index_ var 
    */
    static void incIndex();

private:
   
    /**
    *\fn static void fileProcess(const std::string& )
    *\pram inputs string that contains the list of image 
    *\brief process the --file arguments 
    */

    static void fileProcess(const std::string& );
    
    /**
    *\fn static void fileProcess(const std::string& )
    *\param topologie the Network topologie 
    *\brief process the --t args and save the topologie 
    * in the corresponding vector
    */
    
    static void topologieProcess(const std::string& );
    
    /**
    *\fn static void token_file(const std::string& )
    *\param file the --file argument that was parse
    *\brief the token_file function cut the string using "()"
    * as separator. At the end we get a list of file name that
    are stored in the f_name_ vector.
    */
    
    static void token_file(const std::string& );
    
    /**
    *\fn static void token_topologie(const std::string& )
    *\param the --t args argument that was parse
    *\brief the token_topologie function cut the string using
    * "()" separator after the token task step we obtain a list of
    * unsigned integer that are store in topologie vector
    */
    
    static void token_topologie(const std::string& );
    
    /**
    *\fn static void save_path(const std::string& )
    *\param path is --path argument that was parse
    *\brief store the path parameter in the path_ variable
   */
    
    static void save_path(const std::string& );
    
    /**
    *\fn static load_file()
    *\brief this function use all the parse informations in order
    * to load the files to train the network.
    *During the exectution of this fucntion the target_val vector is filled 
    the input_files_ map is filled with the pictures pixel.
    */
    
    static void load_file();
    
    /**
    *\fn static void createImageVector(std::string&, std::vector<double& >)
    *\param file_name the picture name
    *\param inputs vector in wich we will store the pixel of the pic
    *\bief This function cross the picture pixel and store them in the inputs
    * vector. 
    */
    
    static void createImageVector(std::string&&, std::vector<double>& );
   
   /**
   *\fn static void save_size(const std::string& )
   *\param size this string contains the --size argument that was parse
   *\brief this function is use to store the image  
   */
   
   static void save_size(const std::string& );

public :
   
   /**
    *\bief A public variable 
    /var topologie_
    */
   
   static std::vector<unsigned> topologie_;

private:
   
   /**
    * \brief index_ using to know the next file name, the next ouputs vector,
    * the next inputs in the network
    */
   
   static unsigned int index_;
   
   /**
    *\brief store the path to the source images
    */
   
   static std::string path_; 
   
   /**
    *\brief private fields that store all the file name
    * each index is a categorie
    */
   
   static std::map<int, std::vector<std::string>> f_name_;
   
   /**
    *\brief input_files map that store all the image vector pixels
    */
   
   static std::map<int, inputPool> input_files_;
   
   /**
    *\brief store the expected output 
    */
   
   static std::map<int, std::vector<double>> targets_map_;
   
   /**
    * \brief the dimension of the picture
    */
   
   static cv::Size size_; 
};
