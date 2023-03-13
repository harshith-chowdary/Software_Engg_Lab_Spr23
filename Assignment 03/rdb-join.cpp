Relation * naturaljoin(Relation * R1, Relation * R2, list<string> commonattrs){
    vector<string> new_attrs = R1->get_attrnames();
    vector<int> new_inds = R1->get_attrinds();

    auto r2_attr_names = R2->get_attrnames();
    auto r2_attr_inds = R2->get_attrinds();
    for(int i=0; i<R2->get_nattr(); i++){
        bool flag = false;
        for(auto lol : commonattrs){
            if(lol==r2_attr_names[i]) {flag = true; break;}
        }   
        if(!flag) {new_attrs.push_back(r2_attr_names[i]); new_inds.push_back(r2_attr_inds[i]);}
    }

    int total_nattr = new_attrs.size();

    Relation * join = new Relation(total_nattr, new_attrs, new_inds);

    for(auto rec1 : R1->get_recs()){

        auto r1_attr_ptr = rec1->get_attrptr();

        for(auto rec2 : R2->get_recs()){

            auto r2_attr_ptr = rec2->get_attrptr();

            bool flag = true;

            for(auto name : commonattrs){
                bool subflag = true;
                int i1,i2;
                for(i1=0; i1<R1->get_nattr(); i1++){
                    if(new_attrs[i1]==name) break;
                }
                for(i2=0; i2<R2->get_nattr(); i2++){
                    if(r2_attr_names[i2]==name) break;
                }

                int t;
                float f;
                string s;
                if (const IntAttr* p = dynamic_cast<const IntAttr*>(r1_attr_ptr[i1])){
                    t = p->value();
                    //cout << "t : " << t << endl;
                }
                else if (const FloatAttr* p = dynamic_cast<const FloatAttr*>(r1_attr_ptr[i1])){
                    f = p->value();
                }
                else if (const StringAttr* p = dynamic_cast<const StringAttr*>(r1_attr_ptr[i1])){
                    s = p->value();
                }

                if (const IntAttr* p = dynamic_cast<const IntAttr*>(r2_attr_ptr[i2])){
                    //cout << "t : " << t << ", p->value : " << p->value() << endl;
                    if(t!=p->value()) subflag=false;
                }
                else if (const FloatAttr* p = dynamic_cast<const FloatAttr*>(r2_attr_ptr[i2])){
                    if(f!=p->value()) subflag=false;
                }
                else if (const StringAttr* p = dynamic_cast<const StringAttr*>(r2_attr_ptr[i2])){
                    if(s!=p->value()) subflag=false;
                }

                if(!subflag) {flag = false; break;}
            }

            if(flag){
                vector<Attr *> attrs_join;
                for (auto attr : r1_attr_ptr) {
                    attrs_join.push_back(attr->copy());
                }
                for (int i=0; i<r2_attr_names.size(); i++) {
                    bool flag = true;
                    for(auto name : commonattrs){
                        if(name==r2_attr_names[i]) {flag = false; break;}
                    }
                    if(flag) attrs_join.push_back(r2_attr_ptr[i]->copy());
                }
                Record* rec_copy = new Record(attrs_join);

                if(!join->hasRecord(rec_copy)) join->addRecord(rec_copy);
            }
        }
    }

    return join;
}