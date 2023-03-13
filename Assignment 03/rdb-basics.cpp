#include <iomanip>

using namespace std;

void printValue(const Attr& attr) {
    if (const IntAttr* p = dynamic_cast<const IntAttr*>(&attr)){
        cout << p->value();
    }
    else if (const FloatAttr* p = dynamic_cast<const FloatAttr*>(&attr)){
        cout << p->value();
    }
    else if (const StringAttr* p = dynamic_cast<const StringAttr*>(&attr)){
        cout << p->value();
    }
}

void Record::printRecord(){
    for(auto rec: this->attrptr){
        cout << setw(20);
        printValue(*rec);
        cout << " ";
    }
    cout << endl;
}

vector<Attr *> Record::get_attrptr(){
    return this->attrptr;
}

int Relation::get_nattr(){ return nattr;}

int Relation::get_nrecs(){ return nrecs;}

vector<int> Relation::get_attrinds(){ return attrinds;}

vector<string> Relation::get_attrnames(){ return attrnames;}

list<Record *> Relation::get_recs(){ return recs;}

void Relation::addRecord(){
    vector<Attr *> attrs;
    int t;
    float f;
    string s;
    cout << "Enter the values to Record one by one or separated by a space : " << endl;
    for(int i=0; i<this->nattr; i++){
        t = (this->attrinds)[i];
        if(t==1){
            cin >> t;
            IntAttr * T = new IntAttr(t);
            attrs.push_back(T);
        }
        else if(t==2){
            cin >> f;
            FloatAttr * F = new FloatAttr(f);
            attrs.push_back(F);
        }
        else{
            cin >> s;
            StringAttr * S = new StringAttr(s);
            attrs.push_back(S);
        }
    }
    Record * R = new Record(attrs);
    if(!this->hasRecord(R)){
        (this->recs).push_back(R);
        (this->nrecs)++;
    }
    else{
        cout << "Duplicate Data Entry !!! Same record exists in the Table !!!" << endl;
    }
}

void Relation::addRecord(Record * rec){
    (this->recs).push_back(rec);
    (this->nrecs)++;
}

bool Relation::hasRecord(Record * rec){
    auto pas = rec->get_attrptr();
    int t;
    float f;
    string s;
    for(auto recin: this->get_recs()){
        auto in = recin->get_attrptr();
        bool flag = true;
        for(int i=0;i<nattr;i++){
            if (const IntAttr* p = dynamic_cast<const IntAttr*>(pas[i])){
                t = p->value();
            }
            else if (const FloatAttr* p = dynamic_cast<const FloatAttr*>(pas[i])){
                f = p->value();
            }
            else if (const StringAttr* p = dynamic_cast<const StringAttr*>(pas[i])){
                s = p->value();
            }
            if (const IntAttr* p = dynamic_cast<const IntAttr*>(in[i])){
                if(t!=p->value()) flag=false;
            }
            else if (const FloatAttr* p = dynamic_cast<const FloatAttr*>(in[i])){
                if(f!=p->value()) flag=false;
            }
            else if (const StringAttr* p = dynamic_cast<const StringAttr*>(in[i])){
                if(s!=p->value()) flag=false;
            }
        }
        if(flag) return true;
    }
    return false;
}

void Relation::printRelation(){
    for(auto s:attrnames){
        cout << setw(20) << s << " ";
    }
    cout << endl;
    for(auto R:recs){
        R->printRecord();
    }
}

void Relation::set_attrnames(vector<string> names){
    attrnames.clear();
    attrnames = names;
}

void DataBase::addTable(){
    int na;
    vector<string> attrname;
    vector<int> attrinds;
    cout << "Enter the number of Attributes / Columns in your new Table : ";
    cin >> na;
    cout << "Enter the names of Attributes and type one by one :\n{ Note : Enter its type (1 for int, 2 for float, 3 for string) }\n";
    string s;
    int t;
    for(int i=0;i<na;i++){
        cin >> s >> t;
        if(t!=1 && t!=2) t=3;
        attrinds.push_back(t);
        attrname.push_back(s);
    }
    Relation* table = new Relation(na, attrname, attrinds);

    //cout << "Location of Table 1 : " << table << endl;

    (this->tables).push_back(table);

    // /auto itr = tables.begin();
    //cout << "Location of Table 1 in DataBase while adding : " << *itr << endl; 

    cout << "Enter the name of this Table : ";
    cin >> s;
    (this->tablenames).push_back(s);
    (this->ntables)++;
}

void DataBase::addTable(Relation * table, string name){
    (this->tables).push_back(table);
    (this->tablenames).push_back(name);
    (this->ntables)++;

    cout << "Succesfully added the Table !!!" << endl;
}

void DataBase::delTable(){
    if(this->ntables==0) {cout << "No Tables to Delete !!!" << endl; return;}
    cout << "Existing Data in current DataBase : \n";
    for(int i=0; i<this->ntables; i++){
        cout << i+1 << ". " << this->tablenames[i] << endl;
    }
    int nt;
    cout << "Enter the table number you want to delete : ";
    cin >> nt;
    nt--;
    if(nt<0 || nt>=ntables) {cout << "Invalid Table Number !!!\n"; return;}
    auto itr = (this->tables).begin();
    auto itrn = (this->tablenames).begin();
    while(nt--) {itr++; itrn++;}
    (*itr)->~Relation();
    (this->tables).erase(itr);
    (this->tablenames).erase(itrn);
    (this->ntables)--;

    cout << "Succesfully deleted the Table !!!" << endl;
}

Relation* DataBase::getTable(){
    if(this->ntables==0) {cout << "No Tables to Get !!!" << endl; return NULL;}
    cout << "Existing Data in current DataBase : \n";
    for(int i=0; i<ntables; i++){
        cout << i+1 << ". " << tablenames[i] << endl;
    }
    int nt;
    cout << "Enter the table number : ";
    cin >> nt;
    nt--;
    int n=nt;
    if(nt<0 || nt>=ntables) {cout << "Invalid Table Number !!!\n"; return NULL;}
    auto itr = (this->tables).begin();
    while(nt--) itr++;
    //cout << "Location of Table 1 in Database : " << *itr << endl;
    return (*itr);
}

void DataBase::twoTableOps(int op){
    if(this->ntables==0) {cout << "No Tables to Get !!!" << endl; return;}
    vector<string> ops = {"Difference", "Union", "Product"};
    cout << "Existing Data in current DataBase : \n";
    for(int i=0; i<ntables; i++){
        cout << i+1 << ". " << tablenames[i] << endl;
    }
    int nt1, nt2;
    cout << "Enter the table numbers for Table 1 and Table 2 : ";
    cin >> nt1 >> nt2;
    nt1--;
    nt2--;
    if(nt1<0 || nt1>=ntables) {cout << "Invalid Table Number for Table 1 !!!\n"; return;}
    if(nt2<0 || nt2>=ntables) {cout << "Invalid Table Number for Table 2 !!!\n"; return;}
    auto itr1 = this->tables.begin();
    while(nt1--) itr1++;
    auto itr2 = this->tables.begin();
    while(nt2--) itr2++;
    Relation * R1opR2;
    if(op==0) R1opR2 = difference(*itr1, *itr2);
    else if(op==1) R1opR2 = Union(*itr1, *itr2);
    else if(op==2) R1opR2 = cartesianproduct(*itr1, *itr2);
    string s;
    cout << "Enter the name of this " << ops[op] << " Table : ";
    cin >> s;
    this->addTable(R1opR2, s);
    cout << "Successfully Added " << ops[op] << " of Tables !!!" << endl;
}

void DataBase::projectTable(){
    Relation * curr = getTable();
    if(!curr) return;
    int npa;
    cout << "Enter the number of Projected Attributes : ";
    cin >> npa;
    if(npa<1 || npa>curr->get_nattr()) {cout << "Invalid number of attributes in Projection !!!\n"; return;}
    list<string> projectattrs;
    string s;
    while(npa--){
        cin >> s;
        projectattrs.push_back(s);
    }
    Relation * proj = projection(curr, projectattrs);
    if(!proj) return;
    cout << "Enter the name of this Projection : ";
    cin >> s;
    this->addTable(proj, s);
    cout << "Successfully Added Projection of Table !!!" << endl;
}

void DataBase::selectfromTable(){
    Relation * curr = getTable();
    if(!curr) return;

    Relation * select;

    int nminterms;
    cout << "Enter number of min-terms in DNF : ";
    cin >> nminterms;
    if(nminterms<0) {cout << "Invalid Number of min-terms in DNF !!!\n"; return;}
    if(nminterms==0){
        select = new Relation(*curr);
    }
    else{
        auto schema = curr->get_attrnames();
        auto inds = curr->get_attrinds();
        DNFformula * f = new DNFformula;
        for(int i=0; i<nminterms; i++){
            list<tuple<string, char, Attr *>> minterm;
            int nliterals;
            cout << "Enter number of comparisons in this min-term : ";
            cin >> nliterals;
            if(nminterms<0) {cout << "Invalid Number of comparisons in a min-term !!!\n"; return;}
            for(int j=0; j<nliterals; j++){

                tuple<string, char, Attr *> comp;

                string s,name;
                int t;
                char op;
                float f;

                cout << "Enter name of Attribute ComparisonOperator Value (ex: Price < 500) : ";
                cin >> name >> op;

                for(t=0; t<curr->get_nattr(); t++){
                    if(schema[t]==name) break;
                }

                t = inds[t];

                if(t==1){
                cin >> t;
                    IntAttr * T = new IntAttr(t);
                    comp = make_tuple(name, op, T);
                }
                else if(t==2){
                    cin >> f;
                    FloatAttr * F = new FloatAttr(f);
                    comp = make_tuple(name, op, F);
                }
                else{
                    cin >> s;
                    StringAttr * S = new StringAttr(s);
                    comp = make_tuple(name, op, S);
                }

                minterm.push_back(comp);
            }

            (f->ops).push_back(minterm);
        }

        select = Union(curr, f);
    }

    if(!select) return;
    cout << "Enter the name of this Projection : ";
    string s;
    cin >> s;
    this->addTable(select, s);
    cout << "Successfully Added Selection of the Table !!!" << endl;
}

void DataBase::naturalJoin(){
    if(this->ntables==0) {cout << "No Tables to Get !!!" << endl; return;}
    vector<string> ops = {"Difference", "Union", "Product"};
    cout << "Existing Data in current DataBase : \n";
    for(int i=0; i<ntables; i++){
        cout << i+1 << ". " << tablenames[i] << endl;
    }
    int nt1, nt2;
    cout << "Enter the table numbers for Table 1 and Table 2 : ";
    cin >> nt1 >> nt2;
    nt1--;
    nt2--;
    if(nt1<0 || nt1>=ntables) {cout << "Invalid Table Number for Table 1 !!!\n"; return;}
    if(nt2<0 || nt2>=ntables) {cout << "Invalid Table Number for Table 2 !!!\n"; return;}
    auto itr1 = this->tables.begin();
    while(nt1--) itr1++;
    auto itr2 = this->tables.begin();
    while(nt2--) itr2++;

    list<string> commonattrnames;
    auto attr1 = (*itr1)->get_attrnames();
    auto attr2 = (*itr2)->get_attrnames();
    cout << "Enter number of attributes in joinattr : ";
    cin >> nt1;
    cout << "Enter name of each common attribute separated by space or in new line : \n";
    while(nt1--){
        string s;
        cin >> s;
        bool flag = false;
        for(auto attr: attr1){
            if(attr==s) {flag= true; break;}
        }
        if(!flag){
            cout << "Invalid Attribute :: such attr doesn't exist !!!" << endl;
            return;
        }
        flag = false;
        for(auto attr: attr2){
            if(attr==s) {flag= true; break;}
        }
        if(!flag){
            cout << "Invalid Attribute :: such attr isn't common !!!" << endl;
            return;
        }
        commonattrnames.push_back(s);
    }

    Relation * r1njr2 = naturaljoin(*itr1, *itr2, commonattrnames);
    
    if(!r1njr2) return;
    cout << "Enter the name of this Projection : ";
    string s;
    cin >> s;
    this->addTable(r1njr2, s);
    cout << "Successfully Added Natural Join of the Tables !!!" << endl;
}

// operations
Relation * Union(Relation * R1, Relation * R2){
    if (R1->get_nattr()!=R2->get_nattr() || R1->get_attrnames()!=R2->get_attrnames()) {
        cout << "The input relations have different schemas." << endl;
        return NULL;
    }
    Relation * r1Ur2 = new Relation(*R1);
    
    for (auto rec : R2->get_recs()) {
        if (!r1Ur2->hasRecord(rec)) {
            r1Ur2->addRecord(rec);
        }
    }
    
    return r1Ur2;
}


Relation * difference (Relation * R1, Relation * R2){
    Relation * r1_r2 = new Relation (R1->get_nattr(), R1->get_attrnames(), R1->get_attrinds());

    for(auto rec: R1->get_recs()) {
        if(!R2->hasRecord(rec)) {
            r1_r2->addRecord(rec);
        }
    }

    return r1_r2;
}


Relation * cartesianproduct(Relation * r1, Relation * r2) {
    vector<string> new_attrnames;
    new_attrnames.reserve(r1->get_nattr() + r2->get_nattr());

    // Combine attribute names from both relations
    for (auto& attrname : r1->get_attrnames()) {
        new_attrnames.push_back(attrname);
    }
    for (auto& attrname : r2->get_attrnames()) {
        new_attrnames.push_back(attrname);
    }

    // Create new attribute indices for combined relation
    vector<int> new_attrinds;
    new_attrinds.reserve(r1->get_nattr() + r2->get_nattr());
    for (int i = 0; i < r1->get_nattr(); i++) {
        new_attrinds.push_back(i);
    }
    for (int i = 0; i < r2->get_nattr(); i++) {
        new_attrinds.push_back(i + r1->get_nattr());
    }

    // Create a new relation for the cartesian product
    Relation * result = new Relation(new_attrnames.size(), new_attrnames, new_attrinds);

    // Combine every record from r1 with every record from r2
    for (auto& rec1 : r1->get_recs()) {
        for (auto& rec2 : r2->get_recs()) {
            vector<Attr *> new_attrs;
            new_attrs.reserve(r1->get_nattr() + r2->get_nattr());

            // Combine attributes from both records
            for (auto& attrptr : rec1->get_attrptr()) {
                new_attrs.push_back(attrptr->copy());
            }
            for (auto& attrptr : rec2->get_attrptr()) {
                new_attrs.push_back(attrptr->copy());
            }

            // Create a new record for the combined attributes
            Record * new_rec = new Record(new_attrs);

            // Add the new record to the cartesian product relation
            result->addRecord(new_rec);
        }
    }

    return result;
}


Relation * projection(Relation * R1, list<string> projectattrs){
    auto schema = R1->get_attrnames();
    auto inds = R1->get_attrinds();
    for(auto name: projectattrs){
        bool f = false;
        for(auto attr: schema){
            if(attr==name) {f = true; break;}
        }
        if(!f) {cout << "Invalid Attribute Name :: such attribute(/s) do(/es)n't exist in Schema !!!\n"; return NULL;}
    }
    
    vector<int> indxs;
    vector<string> attrnames;
    vector<int> attrins;
    for(auto name: projectattrs) attrnames.push_back(name);
    int na = attrnames.size();

    for(int i=0,j=0; j<na,i<R1->get_nattr(); i++){
        if(attrnames[j]==schema[i]) {attrins.push_back(inds[i]); indxs.push_back(i); j++;}
    }

    Relation * curr = new Relation (na, attrnames, attrins);

    for(auto rec: R1->get_recs()){
        vector<Attr *> attrs_copy;
        auto attrs = rec->get_attrptr();
        for(auto i:indxs){
            attrs_copy.push_back(attrs[i]->copy());
        }
        Record * nrecord = new Record (attrs_copy);
        if(!curr->hasRecord(nrecord)) curr->addRecord(nrecord);
    }

    return curr;
}


Relation * Union (Relation * R1, DNFformula * f){
    Relation * Union = new Relation(R1->get_nattr(), R1->get_attrnames(), R1->get_attrinds());

    for(auto rec : R1->get_recs()){
        bool flag = false;
        for(auto clause : f->ops){
            bool eachSOPflag = true;
            for(auto cond : clause){
                string attrName = get<0>(cond);
                char op = get<1>(cond);
                Attr* val = get<2>(cond);

                int indx;
                auto attrnames = R1->get_attrnames();
                for(indx=0; indx<R1->get_nattr(); indx++){
                    if(attrnames[indx]==attrName) break;
                }

                if(indx==R1->get_nattr()){
                    cout << "Inavlid request :: No such Attribute found in one of DNFs !!!" << endl;
                    return NULL;
                }

                auto attr = rec->get_attrptr()[indx];

                int t;
                float f;
                string s;
                if (const IntAttr* p = dynamic_cast<const IntAttr*>(attr)){
                    t = p->value();
                    //cout << "t : " << t << endl;
                }
                else if (const FloatAttr* p = dynamic_cast<const FloatAttr*>(attr)){
                    f = p->value();
                }
                else if (const StringAttr* p = dynamic_cast<const StringAttr*>(attr)){
                    s = p->value();
                }

                if(op=='='){
                    if (const IntAttr* p = dynamic_cast<const IntAttr*>(val)){
                        //cout << "t : " << t << ", p->value : " << p->value() << endl;
                        if(t!=p->value()) eachSOPflag=false;
                    }
                    else if (const FloatAttr* p = dynamic_cast<const FloatAttr*>(val)){
                        if(f!=p->value()) eachSOPflag=false;
                    }
                    else if (const StringAttr* p = dynamic_cast<const StringAttr*>(val)){
                        if(s!=p->value()) eachSOPflag=false;
                    }
                }
                else if(op=='<'){
                    if (const IntAttr* p = dynamic_cast<const IntAttr*>(val)){
                        //cout << "t : " << t << ", p->value : " << p->value() << endl;
                        if(t>=p->value()) eachSOPflag=false;
                    }
                    else if (const FloatAttr* p = dynamic_cast<const FloatAttr*>(val)){
                        if(f>=p->value()) eachSOPflag=false;
                    }
                    else if (const StringAttr* p = dynamic_cast<const StringAttr*>(val)){
                        if(s>=p->value()) eachSOPflag=false;
                    }
                }
                else if(op=='>'){
                    if (const IntAttr* p = dynamic_cast<const IntAttr*>(val)){
                        //cout << "t : " << t << ", p->value : " << p->value() << endl;
                        if(t<=p->value()) eachSOPflag=false;
                    }
                    else if (const FloatAttr* p = dynamic_cast<const FloatAttr*>(val)){
                        if(f<=p->value()) eachSOPflag=false;
                    }
                    else if (const StringAttr* p = dynamic_cast<const StringAttr*>(val)){
                        if(s<=p->value()) eachSOPflag=false;
                    }
                }

                //cout << "eachSOPfalg = " << eachSOPflag << endl;

                if(!eachSOPflag) break;
            }
            if(eachSOPflag) { flag = true; break;}
        }

        if(flag) {
            Record * recpassed = new Record(*rec);
            Union->addRecord(recpassed);
        }
    }

    return Union;
}


Relation * difference (Relation * R1, string s1, string s2){
    auto schema = R1->get_attrnames();
    bool f = false;
    for(int i=0; i<schema.size(); i++){
        if(schema[i]==s1) {schema[i]= s2; f = true;}
    }
    R1->set_attrnames(schema);
    if(f) cout << "Successfully changed Attr Name of " << s1 << " to " << s2 << " !!!\n";
    else cout << "No Attribute matches the given name " << s1 << " !!!\n";

    return R1;
}