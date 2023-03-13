#include <iostream>
#include "rdb.h"
#include "rdb-attr.cpp"
#include "rdb-basics.cpp"
#include "rdb-join.cpp"

using namespace std;

void print_menu(){
    cout << "\n***** ****** Menu ***** ******" << endl;
    cout << " @@@@  Table  @@@@\n";
    cout << "   1. Create a New Table.\n";
    cout << "   2. Delete an existing Table with all data in it.\n";
    cout << "   3. Add a record to a Table.\n";
    cout << "   4. Print a Table.\n";
    cout << " @@@@ Methods @@@@\n";
    cout << "   5. Union.\n";
    cout << "   6. Difference.\n";
    cout << "   7. Cartesian Product.\n";
    cout << "   8. Projection.\n";
    cout << "   9. Selection.\n";
    cout << "  10. Rename.\n";
    cout << "  11. Natural Join.\n";
    cout << "  12. Exit.\n";
}

int main(){
    DataBase * data = new DataBase();
    volatile bool op = true;
    int ch = 1;
    cout << "Welcome to RDBMS in CPP :) ...." << endl;
    while(op){
        print_menu();
        cout << "\nYour choice (other than 1 to 10 will be treated as Exit): ";
        cin >> ch;
        if(ch==1){
            data->addTable();
        }
        else if(ch==2){
            data->delTable();
        }
        else if(ch==3){
            Relation* curr = data->getTable();
            if(curr) curr->addRecord();
        }
        else if(ch==4){
            Relation* curr = data->getTable();
            if(curr) curr->printRelation();
        }
        else if(ch==5){
            data->twoTableOps(1);
        }
        else if(ch==6){
            data->twoTableOps(0);
        }
        else if(ch==7){
            data->twoTableOps(2);
        }
        else if(ch==8){
            data->projectTable();
        }
        else if(ch==9){
            data->selectfromTable();
        }
        else if(ch==10){
            Relation* curr = data->getTable();
            string s1, s2;
            cout << "Enter the name of attribute to rename : " ;
            cin >> s1;
            cout << "Enter the new name of above attribute : " ;
            cin >> s2;
            if(curr) curr = difference(curr, s1, s2);
        }
        else if(ch==11){
            data->naturalJoin();
        }
        else{
            cout << "Are you sure you wanna exit ? (Yes:1 & No:0) : ";
            cin >> ch;
            if(ch==1) op=false;
        }
    }

    cout << endl;
    delete data;
    cout << "\nTerminating :) ...... !!!" << endl;
    return 0;
}