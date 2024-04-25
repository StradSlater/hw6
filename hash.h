#ifndef HASH_H
#define HASH_H

#include <iostream>
#include <cmath>
#include <random>
#include <chrono>

#include <map>

typedef std::size_t HASH_INDEX_T;

struct MyStringHash {
    HASH_INDEX_T rValues[5] { 983132572, 1468777056, 552714139, 984953261, 261934300 };
    MyStringHash(bool debug = true)
    {
        if(false == debug){
            generateRValues();
        }
    }
    // hash function entry point (i.e. this is h(k))
    HASH_INDEX_T operator()(const std::string& k) const
    {
        // Add your code here
        int length = k.size();
        std::vector<std::vector<HASH_INDEX_T>> groupings;
        bool extra = false;
        bool full = false;

        if (length/6 > 0){
          full = true;
          for (int i = 0; i < (length/6); ++i){
            std::vector<HASH_INDEX_T> group;
            groupings.push_back(group);
          }
        }

        if (length % 6 > 0){
          std::vector<HASH_INDEX_T> group;
          groupings.push_back(group);
          extra = true;
        }
        
        int i = -1;
        int j = length - 1;

        if (full){
          for (j; j >= 0; j -= 6){
            i += 1;
            if (j < 5){
              break;
            }
            groupings[i].push_back(letterDigitToNumber(k[j-5]));
            groupings[i].push_back(letterDigitToNumber(k[j-4]));
            groupings[i].push_back(letterDigitToNumber(k[j-3]));
            groupings[i].push_back(letterDigitToNumber(k[j-2]));
            groupings[i].push_back(letterDigitToNumber(k[j-1]));
            groupings[i].push_back(letterDigitToNumber(k[j]));
          }
        }

        else{
          i = 0;
        }

        if (extra){
          for (int w = 0; w <6; ++w){
            groupings[i].push_back(0);
          }
          for (int w = 0; w <= j; ++w){
            groupings[i][5-j+w] = letterDigitToNumber(k[w]);
          }
        }
        
        std::vector<HASH_INDEX_T> w_;

        for (HASH_INDEX_T t = 0; t < 5; ++t){
          w_.push_back(0);
        }

        for (int t = 0; t < groupings.size(); ++t){
          w_[4-t] = pow(36,5)*groupings[t][0]+pow(36,4)*groupings[t][1]+pow(36,3)*groupings[t][2]+pow(36,2)*groupings[t][3]+36*groupings[t][4]+groupings[t][5]; 
        }
        
        return (rValues[0]*w_[0] + rValues[1]*w_[1] + rValues[2]*w_[2] + rValues[3]*w_[3] + rValues[4]*w_[4]);

    }

    // A likely helper function is to convert a-z,0-9 to an integral value 0-35
    HASH_INDEX_T letterDigitToNumber(char letter) const
    {
        // Add code here or delete this helper function if you do not want it
        std::map<char, HASH_INDEX_T> conversion = {};
        for (HASH_INDEX_T i = 97; i < 123; ++i){
          conversion[static_cast<char>(i)] = i - 97;
        }
        for (HASH_INDEX_T i = 48; i < 58; ++i){
          conversion[static_cast<char>(i)] = i - 22;
        }

        char letter_;
        for (HASH_INDEX_T i = 65; i < 91; ++i){
          if (static_cast<HASH_INDEX_T>(letter) == i){
            letter_ = static_cast<char>(i+32);
            break; 
          }
          else{
            letter_ = letter;
          }
        }
        return conversion[letter_];

    }

    // Code to generate the random R values
    void generateRValues()
    {
        // obtain a seed from the system clock:
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 generator (seed);  // mt19937 is a standard random number generator

        // Simply call generator() [it has an operator()] to get another random number
        for(int i{ 0 }; i < 5; ++i)
        {
            rValues[i] = generator();
        }
    }
};

#endif
