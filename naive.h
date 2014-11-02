/*
@Author:

*/

#pragma once

#include <list>
#include <map>
#include <fstream>
#include <sstream>

namespace welch{

  struct Feature{
    int id;
    double occurance;
  };


  class Document{
  public:
    Document(){feature_count = 0;}

    void add_feature(Feature feature){
      features.push_back(feature);
      feature_count++;
    }

  public:
    int feature_count;
    std::string label;
    std::list<Feature> features;
  };


  class DataProvider{
  public:
    DataProvider(std::string file){
      file_name = file;
      data = new std::list<Document>();
    }

    ~DataProvider(){
      delete data;
    }

    List<Document> get_data(){
      return *data;
    }

    void load_data(){
      std::ifstream file_data(file_name);
      std::sting tmp_line;
      while (std::getline(file_data, tmp_line)){

      }
    }

  private:
    std::string file_name;
    std::list<Document> *data;
  };


  class Naive{
  private:
    double calculate_probability();

  public:
    naive();

  private:
    DataProvider training_data;
    DataProvider testing_data;
    std::map<string, std::list<int>> features_to_category;
  };

}
