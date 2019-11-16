#include <fstream> 
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <stack>
#include "Ngram.h"
#include "Vocab.h"
using namespace std;
string line;
Vocab voc;
typedef const VocabIndex *VocabContext;
Vocab zhu, big5;
const LogP LogP_PseudoZero = -100;
map< string , vector<string> > map_vec;
//static unsigned debug = 0;
struct Edge{
    int state_prev=0;
    VocabIndex index_prev = 0;
    int state_cur=0;
    VocabIndex index_cur = 0;
    double log_prob = 0.0;
};

void viterbi(map<int, vector<VocabIndex> > path,Ngram lm, char* result_file){
    map<int, vector<Edge> > edges;
    // initial when t = 0
    for( int i = 0; i < path[0].size(); i ++){
        Edge e ;
        e.state_prev = -1;
        e.index_prev = -99;
        e.state_cur = 0;
        e.index_cur = path[0][i];
        e.log_prob = 0;
        
        edges[0].push_back(e);
    }
    for(int t = 1; t < path.size(); t ++){
        
        for(int i = 0 ; i < path[t-1].size();i++){
            //double temp_prob = 0.0;
            Edge temp_e = edges[t-1][i];
            for(int j = 0 ; j < path[t].size();j++){
                VocabIndex context[] = {path[t-1][i],path[t][j] , Vocab_None};
                double cur_prob = lm.contextProb(context);
                
                //temp_prob = cur_prob;
                Edge e ;
                e.state_prev = i;
                e.index_prev = path[t-1][i];
                e.state_cur = j;
                e.index_cur = path[t][j];
                
                e.log_prob = temp_e.log_prob + cur_prob;
                edges[t].push_back(e);
                
                
            }
        }
    }
    //vector<VocabIndex> result;
    int prev_state = -1;
    double max_p = -10000;
    int max_state = -1;
    VocabIndex max_word = -1;
    // last state
    int e_len = edges.size()-1;
    //stack<VocabString> result;
    vector<VocabIndex> result;
    for(int i = 0 ; i < edges[e_len].size(); i++){
        
        if(max_p < edges[e_len][i].log_prob){
            max_p = edges[e_len][i].log_prob;
            max_state = edges[e_len][i].state_cur;
            max_word = edges[e_len][i].index_cur;
            //cout << "index: "<<max_word << endl;
            prev_state = edges[e_len][i].state_prev;
        }
    }
    
    //result.push(voc.getWord(max_word));

    result.push_back(max_word);
    for (int t = edges.size()-2 ; t >= 0;t--){
        max_p = -10000;
        max_state = -1;
        max_word = -1;
        int cur_state = prev_state;
        for(int i = 0 ; i < edges[t].size(); i++){
            if(edges[t][i].state_cur != cur_state){
                continue;
            }
            if(max_p < edges[t][i].log_prob){
                max_p = edges[t][i].log_prob;
                max_state = edges[t][i].state_cur;
                max_word = edges[t][i].index_cur;
                //cout << "index: "<<max_word << endl;
                prev_state = edges[t][i].state_prev;
            }
        }
        result.push_back(max_word);
    }
    edges.clear();
    ofstream output;
    output.open (result_file, ofstream::out | ofstream::app);
    for ( int i = result.size()-1 ; i >= 0 ; i--){
        output <<  voc.getWord(result[i]) << " ";
    }
    output  << "\n";
    //cout <<  "Hello" << endl;

}
map<int, vector<VocabIndex> > construct_paths(int numWords){
    map<int, vector<VocabIndex> > path;
        
    // <s> [wids] </s>
    // <s>
    path[0].push_back(voc.getIndex("<s>"));
    
    size_t pos = 0;
    string token;
    string head = "";
    int i = 0;
    while ((pos = line.find(" ")) != std::string::npos) {
        
        token = line.substr(0, pos);
        //cout << "split " << token << endl;
        if(token.compare("") != 0) {
            //cout << "token index " << voc.getIndex(token.c_str()) << " unknown " << voc.unkIndex() << endl;

            if(voc.getIndex(token.c_str()) >100000){
                int m_size = map_vec[token.c_str()].size();
                vector<string> ::iterator m_vec;
                //cout  << "Start" << endl;
                for(int j=0; j<m_size; j++){
                    if (voc.getIndex(map_vec[token.c_str()].at(j).c_str() )>100000){
                        continue;
                    }
                    //cout <<"token : " <<voc.getIndex(map_vec[token.c_str()].at(j).c_str() )<< endl;
                    path[i+1].push_back(voc.getIndex((map_vec[token.c_str()].at(j)).c_str()));
                } 
                
                //cout<< "size " << m_size<< endl;
            }else{
                path[i+1].push_back(voc.getIndex(token.c_str()));
            }
            
            i++;
            //cout << "token " << token << endl; 
        }
        //std::cout << "pos " << pos <<" token "<< token << std::endl;
        line.erase(0, pos + string(" ").length());
        
    }
    //</s>
    //cout << "numWords" << numWords << " path" << path.size();
    path[numWords+1].push_back(voc.getIndex("</s>"));

    //path[numWords + 2].push_back(Vocab_None);

    return path;
}

void read_mapfile(char* map_file){
    ifstream map_s(map_file);
    string map_line;
    
    while (getline(map_s, map_line)){
        //cout << map_line<<endl;
        size_t pos = 0;
        string token;
        string head = "";
        while ((pos = map_line.find(" ")) != std::string::npos) {
            
            token = map_line.substr(0, pos);
            //cout << "split " << token << endl;
            if (head.compare("") == 0){
                head = token;
                //cout << "head " << token << endl;
            }else if(token.compare("") != 0) {
                map_vec[head].push_back(token);
                //cout << "token " << token << endl; 
            }
            //std::cout << "pos " << pos <<" token "<< token << std::endl;
            map_line.erase(0, pos + string(" ").length());
        }
        //cout << map_vec[head].size() << endl;
    }
    map_s.close();
}
int main(int argc,char **argv){
    // argv[1] segmeted file , argv[2] v_map file, argv[3] lang model, argv[4] output
    int ngram_order = 2;
    
    Ngram lm( voc, ngram_order );

    {
        //const char *lm_filename = "./corpus.lm";
        File lmFile( argv[3], "r" );
        lm.read(lmFile);
        lmFile.close();
        read_mapfile(argv[2]);
    }
    ifstream infile(argv[1]);
    while (getline(infile, line))
    {   

        
        //cout << line << endl;
        /*istringstream iss(line);
        int a, b;
        if (!(iss >> a >> b)) { 
            cout << "error" << endl; 
            break; } // error*/
        VocabString sentence[maxWordsPerLine];
        // string to non-const char
        int l_size = line.size();
        char * c_line =  new char[l_size + 1];
        copy(line.begin(), line.end(), c_line);
        c_line[l_size] = '\0';
        
        unsigned numWords = Vocab::parseWords(c_line, sentence, maxWordsPerLine);
        // process pair (a,b)


        // construct zhu - big5 path
        map<int, vector<VocabIndex> > path = construct_paths(numWords);
        //cout<< "path size "<< path.size() << endl;
        for(int t = 0 ; t < path.size() ; t++){
            for(int i = 0 ; i < path[t].size(); i++){
                //cout<< " "<< path[t][i] << " ";
            }
            //cout << endl;
        }
        viterbi(path,lm,argv[4]);
        
        delete[] c_line;
        
    }
    infile.close();
    
    



    
    /*VocabIndex wid = voc.getIndex("囧");
    if(wid == Vocab_None) {
        printf("No word with wid = %d\n", wid);
        printf("where Vocab_None is %d\n", Vocab_None);
        wid = voc.getIndex(Vocab_Unknown);
    }
    //cout << "1"<<endl;
    wid = voc.getIndex("一");
    //cout << "12"<<endl;
    if(wid == Vocab_None) {
        printf("No word with wid = %d\n", wid);
        printf("where Vocab_None is %d\n", Vocab_None);
        wid = voc.getIndex(Vocab_Unknown);
    }else{
        VocabIndex context[] = {voc.getIndex("一") , Vocab_None};
        VocabIndex context2[] = {voc.getIndex("一"),voc.getIndex("一") , Vocab_None};
        cout << "3"<<endl;
        cout << "log Prob(一) + log Prob(一|一) = \n" << lm.contextProb(context2)<<endl;
        cout << "log Prob(一|一) = \n" << lm.wordProb(wid, context);
        
    }*/
    


    /*fstream file;      

    //char str[size] , str1[size];
    string s;
    ifstream infile(argv[1]);
    ofstream outfile(argv[2]);
    vector<string> map_Zhu_Big5;
    
    
    while(infile >> s){
        
        

        outfile << s ;
    }*/
        

}