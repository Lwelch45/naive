/*
@Author: Laurence Welch
@Purpose: Naive Bayes implementation for CS4780
*/



#include <list>
#include <map>
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>

namespace welch{

  struct Feature{
    int id;
    double occurance;
  };


  class Util{
  public:
    static std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
      std::stringstream ss(s);
      std::string item;
      while (std::getline(ss, item, delim)) {
          elems.push_back(item);
      }
      return elems;
    }


    static std::vector<std::string> split(const std::string &s, char delim) {
        std::vector<std::string> elems;
        split(s, delim, elems);
        return elems;
    }

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

    std::list<Document> get_data(){
      return *data;
    }

    void load_data(){
      std::ifstream file_data(file_name);
      std::string tmp_line;
      while (std::getline(file_data, tmp_line)){
        std::vector<std::string> line_tokens = Util::split(tmp_line, ' ');
        int line_iterator = 0; Document doc;

        for( std::vector<std::string>::const_iterator i_line = line_tokens.begin(); i_line != line_tokens.end(); ++i_line, line_iterator++){
          if (line_iterator == 0){
            doc.label = *i_line; break;
          }
          std::vector<std::string> feature_tokens = Util::split(*i_line, ':');

          //std::cout << *i << ' ';
        }
        std::cout << doc.label;
        break;
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
    Naive(std::string training_file, std::string testing_file){
      training_data = new DataProvider(training_file);
      testing_data = new DataProvider(testing_file);
    }

  private:
    DataProvider *training_data;
    DataProvider *testing_data;
    std::map<std::string, std::list<int> > features_to_category;
  };
}

int main(){
  welch::DataProvider test("train.tfidf");
  test.load_data();
}
