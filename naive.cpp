/*
 @Author: Laurence Welch
 @Purpose: Naive Bayes implementation for CS4780
 */



#include <algorithm>
#include <map>
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
#include <math.h>
namespace welch{

    struct Feature{
        int id;
        double occurances;
    };

    struct CategoryFeature{
        int id;
        double sum_weighted_occurances = 0.0;

        CategoryFeature(int i, double w){
            id = i;
            sum_weighted_occurances = w;
        }
        void add(double weight){
            sum_weighted_occurances += weight;
        }
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
        bool feature_is_category_feature(CategoryFeature cf, Feature f){
            return (f.id == cf.id);
        }

        template<typename T=Feature, typename R=std::vector<CategoryFeature> >
        static CategoryFeature& find_and_add(R& Container, T const& item ) {
            typename R::iterator it = std::find_if(Container.begin(), Container.end(),  [item](CategoryFeature&v)->bool{ return item.id == v.id; });
            if (it != Container.end()){
                it->add(item.occurances);
                return *it;
            }else{
                Container.push_back(CategoryFeature(item.id, item.occurances));
                return *Container.end();
            }
        }

        template<typename T=Feature, typename R=std::vector<CategoryFeature> >
        static CategoryFeature& find(R& Container, T const& item ) {
            typename R::iterator it = std::find_if(Container.begin(), Container.end(),  [item](CategoryFeature&v)->bool{ return item.id == v.id; });
            if (it != Container.end()){
                return *it;
            }else{
                return *Container.end();
            }
        }

        template<typename T=Feature, typename R=std::vector<CategoryFeature> >
        static bool contains(R& Container, T const& item ) {
            typename R::iterator it = std::find_if(Container.begin(), Container.end(),  [item](CategoryFeature&v)->bool{ return item.id == v.id; });
            if (it != Container.end()){
                return true;
            }else{
                return false;
            }
        }
    };


    class Document{
    public:
        Document(){feature_count = 0; features = new std::vector<Feature>();}

        void add_feature(Feature feature){
            features->push_back(feature);
            feature_count++;
        }

    public:
        int feature_count;
        std::string label;
        std::vector<Feature> *features;
    };


    class DataProvider{
    public:
        DataProvider(std::string file){
            file_name = file;
            data = new std::vector<Document>();
        }

        ~DataProvider(){
            delete data;
        }

        std::vector<Document> get_data(){
            return *data;
        }
        void shuffle_data(std::string file){
            std::ifstream file_data(file_name);
            std::ofstream write_data(file);

            std::vector<std::string> tmp;

            std::string tmp_line;
            while (std::getline(file_data, tmp_line)) {
                tmp.push_back(tmp_line);
                std::random_shuffle(tmp.begin(), tmp.end());
            }
            std::random_shuffle(tmp.begin(), tmp.end());
            for(std::string line : tmp){
                write_data << line << std::endl;
            }
            write_data.close();
        }

        void load_data(){
            std::ifstream file_data(file_name);
            std::string tmp_line;

            //iterate line by line
            while (std::getline(file_data, tmp_line)){
                std::vector<std::string> line_tokens = Util::split(tmp_line, ' ');
                Document doc; int line_iterator = 0;

                //iterate word by word
                for(std::vector<std::string>::const_iterator i_line = line_tokens.begin(); i_line != line_tokens.end(); ++i_line, line_iterator++){
                    if (line_iterator == 0){
                        doc.label = *i_line; continue;
                    }
                    std::vector<std::string> feature_tokens = Util::split(*i_line, ':');
                    Feature tmp_feature; int feature_iterator = 0;

                    //gracb id and value add featuer to document
                    for(std::vector<std::string>::const_iterator f_line = feature_tokens.begin(); f_line != feature_tokens.end();++f_line, feature_iterator++){
                        if(feature_iterator == 0){
                            tmp_feature.id = stoi(*f_line);
                            continue;
                        }
                        tmp_feature.occurances = stod(*f_line);
                        doc.add_feature(tmp_feature);
                    }
                    feature_iterator =0;
                }
                std::cout << doc.label << " : " << doc.feature_count << std::endl;
                if (doc.label != "")
                    data->push_back(doc);
            }
        }

    private:
        std::string file_name;
        std::vector<Document> *data;
    };


    class Naive{
    public:
        Naive(std::string training_file, std::string testing_file){
            training_data = new DataProvider(training_file);
            testing_data = new DataProvider(testing_file);
        }

        Naive(){ }

        void teach(DataProvider *tmp_training = nullptr){
            if (tmp_training == nullptr && training_data == nullptr){
                std::cerr << "No training data was specified please try again." << std::endl;
            }
            if (tmp_training != nullptr){
                training_data = tmp_training;
            }

            training_data->load_data();
            //initlize two known classes
            features_to_category["1"]; // positive
            features_to_category["-1"]; // negative



            //iterate over documents
            for(Document doc : training_data->get_data()){
                //iterate over each feature
                for (Feature feat : *doc.features){
                    std::cout << feat.occurances << std::endl;
                    Util::find_and_add(features_to_category[doc.label], feat);
                }
            }
        }
        std::string classify(std::vector<Feature> features){
            std::string output = "";
            double highest = -55555.0;
            for (auto& kv : features_to_category) {
                double res = calculate_probability(kv.first, features);
                if (res > highest){
                    highest = res;
                    output = kv.first;
                }else if (res == highest){
                    output = "1";
                }
            }
            return output;
        }

    private:
        // calculate a the probability of a set of features belonging to a particular class
        double calculate_probability(std::string label, std::vector<Feature> features){
            double current_label_count = features_to_category[label].size();
            double label_probability = current_label_count/(double) training_data->get_data().size();
            double result;
            // ddetermine the individual probability of each term
            for(Feature feat : features){
                double occurances = 0.0;
                if (Util::contains(features_to_category[label],feat)){
                    occurances = Util::find(features_to_category[label], feat).sum_weighted_occurances + 1;
                }
                result += log(occurances/((double)current_label_count + (double)training_data->get_data().size()));
            }
            return result * label_probability;
        }



    private:
        DataProvider *training_data;
        DataProvider *testing_data;
        std::map<std::string, std::vector<CategoryFeature> > features_to_category;
    };
}

int main(){
    welch::DataProvider test("/Users/laurencewelch/Projects/naiveb/naiveb/train.tfidf");
    test.shuffle_data("/Users/laurencewelch/Projects/naiveb/naiveb/train_shuffled.tfidf");

    //welch::DataProvider train("test.tfidf");
    //train.load_data();

    //welch::Naive bayes("/Users/laurencewelch/Projects/naiveb/naiveb/train.tfidf","/Users/laurencewelch/Projects/naiveb/naiveb/test.tfidf");
    //bayes.teach();
}
