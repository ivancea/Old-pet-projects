#include<iostream>
#include<vector>
#include<map>
#include<fstream>

using namespace std;

class xml{
    string _tag;
    map<string,string> _attr;
    vector<xml> _tree;
    string _text;

    string tabulateText(string text, int level)const{
        bool erasing=false;
        uint32_t i=0;
        for(;text[i]=='\t' || text[i]==' ';i++);
        string t(level*tabulationSize,' ');
        for(; i<text.size(); i++){
            if(erasing){
                if(text[i]==' ' || text[i]=='\t')
                    continue;
                else erasing=false;
            }
            if(text[i]=='\n'){
                t+="\n" + string(level*tabulationSize,' ');
                erasing = true;
            }else
                t+=text[i];
        }
        return t;
    }

public:
    static int tabulationSize;
    static string version,encoding;

    xml(){}
    xml(const xml &x){
        _tag = x.getTag();
        _attr = x.getAttr();
        _tree = x.getTree();
        _text = x.getText();
    }

    string getTag()const{return _tag;}
    string getText()const{return _text;}
    const map<string,string>& getAttr()const{return _attr;}
    const vector<xml>& getTree()const{return _tree;}

    void setTag(string tag){_tag=tag;}
    void setText(string text){_text=text;}
    void addAttr(string attr, string value){_attr[attr] = value;}
    void addTree(xml &x){
        _tree.push_back(x);
    }
    void delAttr(string attr){
        if(_attr.find(attr)!=_attr.end())
            _attr.erase(_attr.find(attr));
    }
    void delTree(int index){
        if(index>=0 && index<_tree.size())
            _tree.erase(_tree.begin()+index);
    }

    bool toFile(string fileName, int level=0){
        ofstream f(fileName, ios::trunc);
        if(!f) return false;
        if(!level)
            f << "<?xml version=\"" << xml::version << "\" encoding=\"" << xml::encoding << "\" ?>" << endl;
        f << string((level)*tabulationSize,' ') << '<' << _tag;
        for(auto it:_attr)
            f << ' ' << it.first << "=\"" << it.second << '"';
        f << '>' << endl;
        f << /*string((level+1)*tabulationSize,' ') <<*/ tabulateText(_text,level+1) << endl;
        for(auto it:_tree){
            it.toFile(f,level+1);
            f << endl;
        }
        f << string((level)*tabulationSize,' ') << "</" << _tag << '>';
    }

    void toFile(ofstream& f, int level=0){
        if(!f) return;
        if(!level)
            f << "<?xml version=\"" << xml::version << "\" encoding=\"" << xml::encoding << "\" ?>" << endl;
        f << string((level)*tabulationSize,' ') << '<' << _tag;
        for(auto it:_attr)
            f << ' ' << it.first << "=\"" << it.second << '"';
        f << '>' << endl;
        f << /*string((level+1)*tabulationSize,' ') <<*/ tabulateText(_text,level+1) << endl;
        for(auto it:_tree){
            it.toFile(f,level+1);
            f << endl;
        }
        f << string((level)*tabulationSize,' ') << "</" << _tag << '>';
    }

    void clear(){
        _tag.clear();
        _text.clear();
        _attr.clear();
        _tree.clear();
    }
};

int xml::tabulationSize = 4;
string xml::version = "1.0", xml::encoding = "ASCII";

int main(){
    xml x, temp;
    x.setTag("Prueba");
    x.addAttr("Level","0");
    x.setText("Esto es el comienzo.");
    temp.setTag("First");
    temp.addAttr("Level","1");
    temp.setText("Estamos en el primer nivel.\nGuay!\n¡Cómo mola el tabulado! e.e");
    temp.addTree(temp);
    x.addTree(temp);
    if(x.toFile("prueba.xml")) cout << "Guardado.";

}
