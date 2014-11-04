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
            std::cout << "here";
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
            //iterate over the documents in the training set
            for (std::vector<Document>::iterator i = training_data->get_data().begin(); i != training_data->get_data().end(); ++i){
                //iterate over the features of a given document
                if (i->feature_count != 0){
                    for(std::vector<Feature>::iterator f_i = i->features->begin(); f_i != i->features->end(); ++f_i){
                        //Util::find_and_add(features_to_category[iter->first], *f_i);
                        //std::cout << f_i->occurances << std::endl;
                    }
                }
            }
        }
    private:
        double calculate_probability(std::string label, std::vector<int>){
            return 0.0;
        }



    private:
        DataProvider *training_data;
        DataProvider *testing_data;
        std::map<std::string, std::vector<CategoryFeature> > features_to_category;
    };
}

int main(){
    //welch::DataProvider test("train.tfidf");
    //test.load_data();

    //welch::DataProvider train("test.tfidf");
    //train.load_data();

    welch::Naive bayes("/Users/laurencewelch/Projects/naiveb/naiveb/train.tfidf","test.tfidf");
    bayes.teach();
}


/*
 if (iter == my_map.end() || key < iter->first) {    // not found

 features_to_category.insert(iter, make_pair(key, value));     // hinted insertion
 } else {

 }

 */
