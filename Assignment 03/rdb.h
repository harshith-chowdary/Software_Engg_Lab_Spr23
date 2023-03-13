#include <vector>
#include <list>
#include <string>

using namespace std;

//Data Structures
class Attr { //Base class for attributes
    // Add operators for different attribute type in derived classes
    public: virtual bool operator== (const Attr & right) = 0;
    virtual Attr * copy() const = 0;
};

typedef struct { 
    list <list <tuple <string, char, Attr *> >> ops; 
}DNFformula;

class Record { //storing data for each record
    vector<Attr *> attrptr;
    //methods
    public:
    Record(vector<Attr *> attrptr) : attrptr(attrptr) {}
    void printRecord();
    vector<Attr *> get_attrptr();

    Record(const Record& rec){
        vector<Attr*> attrs_copy;
        for (auto attr : rec.attrptr) {
            attrs_copy.push_back(attr->copy());
        }
        this->attrptr = attrs_copy;
    }

    ~Record(){
        for(auto ptr: attrptr){
            delete ptr;
        }
    }

    friend class Attr;
};

class Relation { // storing a relation
    int nattr, nrecs; // number of attributes and records
    vector<string> attrnames; // schema
    vector<int> attrinds; // mapping schema to indices
    list <Record *> recs; // list of records

    // methods
    public:
    // constructor
    Relation(int nattrs, vector<string> attrnames, vector<int> attrinds) : 
    nattr(nattrs), nrecs(0), attrnames(attrnames),  attrinds(attrinds) {}

    Relation(const Relation & Rel){
        this->nattr = Rel.nattr;
        this->nrecs = Rel.nrecs;
        this->attrnames = Rel.attrnames;
        this->attrinds = Rel.attrinds;

        for (auto rec : Rel.recs) {
            vector<Attr*> attrs_copy;
            for (auto attr : rec->get_attrptr()) {
                attrs_copy.push_back(attr->copy());
            }
            Record* rec_copy = new Record(attrs_copy);
            this->recs.push_back(rec_copy);
        }
    }

    ~Relation() {
        for (auto r : recs) {
            cout << "Deleting Record ... !! " << endl;
            r->~Record();
            //delete r;
        }
    }
    
    void addRecord();
    void addRecord(Record * );
    bool hasRecord(Record * );
    void printRelation();
    int get_nattr();
    int get_nrecs();
    vector<string> get_attrnames();
    vector<int> get_attrinds();
    list<Record *> get_recs();
    void set_attrnames(vector<string>);

    friend class Attr;
    friend class Record;
    friend class DataBase;
};

class DataBase {
    int ntables;
    vector<string> tablenames;
    list<Relation *> tables;

    public:
    DataBase(int num=0): ntables(num) {}
    void addTable();
    void addTable(Relation * R, string);
    void delTable();
    void twoTableOps(int );
    void projectTable();
    void selectfromTable();
    void naturalJoin();
    Relation* getTable();

    ~DataBase() {
        for (auto t : tables) {
            cout << "Deleting Relation ... !! " << endl;
            t->~Relation();
            //delete t;
        }
    }

    friend class Attr;
    friend class Record;
    friend class Relation;
};

//operations
Relation * Union(Relation * , Relation * );
Relation * difference (Relation * , Relation * );
Relation * cartesianproduct(Relation * , Relation * );
Relation * projection(Relation * , list<string> );
Relation * Union (Relation * , DNFformula * );
Relation * difference (Relation * , string , string );
Relation * naturaljoin(Relation * , Relation * , list<string>);