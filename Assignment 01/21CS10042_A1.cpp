#include <iostream>
#include <cmath>
// including whatever is required

using namespace std;
// using standard namespace

// defining consant double value 1e9 to be used as default value for Epsilon
const double default_epsilon = 1e-9;

// defining struct term as "Polyn" with integer type degree, double type coefficient and pointer to next struct term
struct term{
    int d;
    double c;
    struct term * next;
};

typedef struct term Polyn;

// defining struct P as "Polyns" with pointer to head of the polynomial and pointer to next struct P
struct P{
    Polyn * poly;
    struct P * next;
};

typedef struct P Polyns;

// integer variable to store number of Polynomials stored
int nofps = 0;

// linked listed to store pointers to stored Polynomials
Polyns ** Polys;

// function to print a Polynomial when pointer to starting term is passed
void printpolyn(Polyn * poly){
    // defining a temporary pointer to iterate through the Polynomial
    Polyn * ptr = poly;
    // to print the first term as I considered default first term to be 0, 0, NULL (can be seen in create Polynomial utility)
    if(ptr->next==NULL) {cout << ptr->c << endl; return;}
    if(ptr->c!=0) cout << ptr->c << " ";
    else{
        ptr=ptr->next;
        if(ptr==NULL) return;
        if(ptr->d==0) cout << ptr->c << " ";
        else if(ptr->c!=0) cout << ptr->c << "x^" << ptr->d << " ";
    }
    // iterating through the linked list of terms of the Polynomial uptill pointer is NULL
    while(ptr->next!=NULL){
        // incrementing pointer by passing pointer->next to pointer
        ptr=ptr->next;
        if(ptr->c>0) cout << "+";
        if(ptr->d==0) cout << ptr->c << " ";
        else if(ptr->c!=0) {
            cout << ptr->c << "x^" << ptr->d << " ";
        }
    }
    cout << endl;
}

// defining a function to add a polynomial (passed by pointer to it's head term) to linked list of Stored Polynomials
void addpolyn(Polyns ** Polys, Polyn * p){
    // iterating through linked lists of Polynomials till reached the end
    Polyns * last = *Polys;
    while(last->next!=NULL){
        last=last->next;
    }
    // assigning pointer of the polynomial to a new Polynomial type Polyns and adding it to linked list of Polynomials
    last->next = (Polyns *)malloc(sizeof(Polyns));
    last->next->poly = p;
    last->next->next = NULL;
    return;
}

// defining a function to find correct place for a term with degree 'd' in the current polynomial 
// while adding the term into it
Polyn * findpointer(Polyn * p, int d){
    // this function will be used in "void addremoveterm(Polyn * poly, int d, double c)" function
    // since I well defined cases for current pointer I am going to use pointer's next pointer in whole function
    // if it is NULL returning the pointer so that we can add the term with degree 'd' at the end
    if(p->next==NULL) return p;
    // if it's degree is less than passed 'd' then recursively find degree 'd' place from next pointer 
    if(p->next->d<d) return findpointer(p->next,d);
    // if it's degree is greater than passes 'd' then we'll have to insert the degree 'd' term before it 
    // so return pointer p
    if(p->next->d>d) return p;
    // if it's degree is equal to degree 'd' which is passed then return the pointer
    if(p->next->d==d) return p->next;
    // every case falls in above 4 but in case to makesure no problem just returning null
    return NULL;
}

// the next 3 terms use Function Overloading to add/remove/remove all coefficients with absolute value less than Epsilon 

// defining a funciton to delete a term with degree 'd' from the Polynomial passed with pointer to its head
void addremoveterm(Polyn * poly, int d){
    // initiating two pointers to point to term with degree 'd' if found and its previous term
    Polyn * head = poly;
    Polyn * prev = head;
    // while current degree (head->d) is less than 'd' passed and next pointer is not null we increment
    while(head->d<d && head->next!=NULL){
        // make previous equal to current pointer i.e., head
        prev = head;
        // make head equal to next pointer
        head=head->next;
    }
    // loop breaks if either term wiht degree 'd' is found or no such term exists in the Polynomial
    // if current pointer i.e., head, if it's degree is not 'd' => "NO such term exists in the Polynomial"
    if(head->d!=d){
        // print and return 
        cout << "No term with Degree " << d << " in : "; 
        printpolyn(poly);
        return;
    }
    // since I defined default first term of my Polynomial as 0, 0, NULL handling delete case accordingly
    if(prev->d==0 && prev==head) {prev->c=0; return;}
    if(d>-1) cout << "Succesfully removed term with Degree " << d << endl;
    // to delete data in current pointer
    Polyn * ptr = prev->next;
    // making previous pointer's next point to next term of current pointer
    prev->next = ptr->next;
    // freeing current pointer == deleteing the term
    free(ptr);
    if(poly->c==0 && poly->next==NULL){
        return;
    }
    // to print updated polynomial
    cout << "Updated Polynomial : ";
    printpolyn(poly);
    return;
}

// defining a function to add a term with degree 'd' and coefficient 'c' to polynomial with pointer to it's head passed
void addremoveterm(Polyn * poly, int d, double c){
    // using "Polyn * findpointer(Polyn * p, int d)" to find appropriate place to insert the term
    Polyn * ptr = findpointer(poly, d);
    // if a term already exists with that degree 'd'
    if(ptr->d==d) {
        // and if coefficient passed to be added is negative of current coefficient of current degree then 
        // just remove term does the job
        if(ptr->c==-c) {addremoveterm(poly, d); return;}
        // if not so then just add this given coefficient to already existing coefficient
        double t = ptr->c;
        t+=c;
        ptr->c = t;
        // instead of using else for other case just using return here to terminate function
        return;
    }
    // storing pointer to next part of polynomial to be linked after the degree 'd' term
    Polyn * nxt = ptr->next;
    // if no term exists with degree 'd' create a new term and add it right next to the pointer we got from findpointer
    ptr->next = (Polyn *)malloc(sizeof(Polyn));
    ptr->next->d=d;
    ptr->next->c=c;
    // re-linking stored pointer which stores remaining Polynomial to current term's next pointer
    ptr->next->next=nxt;
}

// defining a function to remove all coefficients with absolute value less than Epsilon from polynomial
void addremoveterm(Polyn * poly, int i, int j, double e = default_epsilon){
    cout << "Removed all terms with absolute coefficients less than " << e << " from ";
    // just printing the polynomial to make sure user entered right thing
    printpolyn(poly);
    // pointer to iterate through the terms of Polynomial
    Polyn * head = poly;
    // since I defined my polynomial with default term 0, 0, NULL handling that case separately
    if(head->d==0){
        if(abs(head->c)<e) head->c=0;
    }
    else if(abs(head->c)<e){
        addremoveterm(poly, head->d);
    }
    // iterating through terms of Polynomial to find terms with coefficients less than epsilon and removing them
    while(head->next!=NULL){
        head=head->next;
        if(abs(head->c)<e){
            addremoveterm(poly, head->d);
        }
    }
    cout << "Removed all terms with absolute coefficients less than " << e << " from ";
    // printing updated Polynomial
    printpolyn(poly);
}

// defining a function to remove a polynomial from stored Polynomials
void deletepoly(Polyns ** Polys, int num){
    // handling the special case 
    // to delete the first polynomial stored
    if(num==1){
        Polyns * head = * Polys;
        // printing the polynomial to be deleted
        printpolyn(head->poly);
        // equating starting pointer to next polynomial's pointer
        * Polys = head->next;
        // decrementing number of polynomials stored by 1 as we delete a polynomial
        nofps--;
        // using return to end function here instead of using else on next part of code
        return;
    }
    // pointer to traverse through Polynomials to previous pointer of polynomial to be deleted
    Polyns * head = * Polys;
    // setting num value according to the way I stored polynomials
    num=num-2;
    while(num--){
        head=head->next;
    }
    Polyns * ptr = head->next;
    // printing the polynomial we wanted to delete
    printpolyn(ptr->poly);
    // connecting previous pointer's next to pointer to next polynomial of current polynomial
    head->next = ptr->next;
    // freeing current polynomial
    free(ptr);
    // decrementing number of polynomials by 1
    nofps--;
    return;
}

// defining a function to evaluate polynomial (whose pointer is passed) with passed value of x
// using default parameter op whether to print calculated value or not ** used while finding root using Newton's method  
long double evaluate(Polyn * poly, double x, bool op=true){
    // to store p(x)   **long double to store large values if given
    long double ev=0;
    // to calculate x^pow effectively by multiplying by difference of current degree and previous degree 
    long double powr=1;
    // pointers to store current and previous pointer while iterating
    Polyn * head = poly;
    Polyn * prev = head;
    // calculating ev here x^head->d by above mentioned approach to save computation costs
    ev+=head->c * pow(x,head->d);
    // iterating till end of polynomial
    while(head->next!=NULL){
        // equating previous to head and head to head->next to access next term of Polynomial
        prev = head;
        head=head->next;
        // calculating powr here x^head->d by above mentioned approach to save computation costs
        powr = powr*pow(x, head->d - prev->d);
        // adding that to evaluated value
        ev+=head->c * powr;
    }
    // if op is false i.e., don't print logs just return evaluated value
    if(!op) return ev;
    // if op is true print the calculation, polynomial and return evaluated value
    cout << "For P(x) = ";
    printpolyn(poly);
    cout << "x = " << x << " => P(" << x << ") = " << ev << endl;
    return ev;
}

// defining a function to calculate root of from start value 'x0' passed and with window of 'e'
void zero_newton(Polyn * poly, double x0, double e = default_epsilon){
    // to store actual value of x0 which will be changed to root later on
    double xi = x0;
    // finding derivative of polynomial using same approach in derivative function below
    Polyn * derp = (Polyn *)malloc(sizeof(Polyn));
    Polyn * p = poly;
    while(p->next!=NULL){
        p = p->next;
        addremoveterm(derp, (p->d)-1, (p->c)*(p->d));
    }

    // asking user if u want to see calculation steps or not 
    int op=0;
    cout << "Do you want to see the Calculation Steps ? (if so, enter 1): ";
    cin >> op;
    if(op!=1) op=0;
    cout << "Calculating ......... !!\n";
    // applying Newton's Method
    long double ev = evaluate(poly, x0, op);

    // if polynomial has no roots then it will terminate in 100 iterations
    int ct=0;
    // while f(x0) is more than epsilon and # of iterations is <100 go to next iteration
    while(abs(ev)>e && ct<100){
        // evaluate the derivative for x0
        long double evderp = evaluate(derp, x0, op);
        // if derivative say f'(x0) is 0 just decrementing x0 or incrementing works to handle divide by 0 error
        if(evderp!=0) x0-= ev/evderp;
        else x0--;
        // evaluate updated f(x0)
        ev = evaluate(poly, x0, op);
        // increment count of iterations
        ct++;
    }

    // loop can break in two ways if iterations crossed 100 or root is found
    if(ct>=100) cout << "No Real Root for the Polynomial : ";
    else cout << "Nearest Root to " << xi << " is '" << x0 << "' for Polynomial : ";
    printpolyn(poly);
}

// just a function to add terms at end used at some place to make adding terms effective
void addatend(Polyn * ptr, int d, double c){
    ptr->next = (Polyn *)malloc(sizeof(Polyn));
    ptr->next->d = d;
    ptr->next->c = c;
    ptr->next->next = NULL;
}

// defining an operator overloaded function to add two Polynomials
Polyn * operator+(Polyn poly1, Polyn poly2){
    // creating a polynomial to store sum of two Polynomials poly1 and poly 2 here
    Polyn * poly = (Polyn *)malloc(sizeof(Polyn));
    Polyn * ptr = poly;
    Polyn * p1 = &poly1;
    // usual logs
    cout << "Adding " << p1->c << " x^" << p1->d << endl;
    ptr->d = 0;
    ptr->c = p1->c;
    // using addatend here to make a copy of poly1 in new polynomial
    while(p1->next!=NULL){
        p1 = p1->next;
        cout << "Adding " << p1->c << " x^" << p1->d << endl;
        addatend(ptr, p1->d, p1->c);
        ptr = ptr->next;
    }
    // usual logs
    cout << "After adding Polynomial 1 : ";
    printpolyn(poly);
    Polyn * p2 = &poly2;
    // adding poly 2 term by term to new polynomial which stores the sum of both Polynomials
    cout << "Adding " << p2->c << " x^" << p2->d << endl;
    addremoveterm(poly, p2->d, p2->c);
    while(p2->next!=NULL){
        p2 = p2->next;
        cout << "Adding " << p2->c << " x^" << p2->d << endl;
        // using addremoveterm here to add terms to new polynomial
        addremoveterm(poly, p2->d, p2->c);
    }
    // usual logs
    cout << "After adding Polynomial 2 : ";
    printpolyn(poly);
    // returning addition of two Polynomials 
    return poly;
}

// defining a function to find derivative of a Polynomial
Polyn * derivative(Polyn * p){
    Polyn * derp = (Polyn *)malloc(sizeof(Polyn));
    // common derivative approach 
    while(p->next!=NULL){
        p = p->next;
        // adding c*dx^(d-1) for every cx^d term in actual Polynomial
        addremoveterm(derp, (p->d)-1, (p->c)*(p->d));
    }
    // returning derivative polynomial
    return derp;
}

int main(){
    // usual logs
    int ex=1;
    cout << "WELCOME !!! :)" << endl;
    while(ex!=0){
        // usual logs
        cout << "MENU : (enter your query number)" << endl;
        cout << "0. Display Polynomials\n1. Create a Polynomial\n2. Delete a Polynomial\n3. Add term to Polynomial\n4. Remove term from Polynomial\n5. Remove all terms with absolute coefficient values less than E\n6. Evaluate a Polynomial\n7. Add two Polynomials\n8. Differentiate a Polynomial\n9. Calculate a zero(root) of Polynomial\n10. Exit" << endl;
        // op to store what action user want to perform
        int op;
        cin >> op;
        if(op==0){
            // if no polynomials displays the same
            if(nofps==0) cout << "No Polynomials to Display !!!" << endl;
            else{
                // displays all polynomials
                cout << "Polynomials stored are : " << endl;
                Polyns * ptr = *Polys;
                int ct=1;
                // iterate through linked list of Polynomials
                while(ptr->next!=NULL){
                    cout << "Polynomial " << ct++ << " : ";
                    // using printpolyn function to print a polynomial
                    printpolyn(ptr->poly);
                    ptr=ptr->next;
                }
                // to print last polynomial
                cout << "Polynomial " << ct++ << " : ";
                printpolyn(ptr->poly);
            }
        }
        else if(op==1){
            // asking number of terms user want in Polynomial while creating later used for creating polynomial
            int n;
            cout << "Enter the number of terms in the Polynomial : ";
            cin >> n;
            // ensuring 'n' is a valid natural number
            if(n<1) {cout << "Invalid Number of terms for the Polynomial !!!"<< endl; continue;}
            Polyn * poly = (Polyn *)malloc(sizeof(Polyn));
            poly->c = 0;
            poly->d = 0;
            poly->next = NULL;
            cout << "Input degrees of x and their coefficients : "<< endl;
            int ind=0;
            while(ind<n){
                cout << "Enter the degree : ";
                double d;
                cin >> d;
                // to ensure whether degree given is valid if no prints the same
                if(floor(d)==d && d>-1){
                    cout << "Enter the coefficient of x^" << d << " : ";
                    double c;
                    cin >> c;
                    // if valid uses addremoveterm to add the term cx^d to polynomial created now
                    addremoveterm(poly, d, c);
                    // incrementing number of terms added by '1'
                    ind++;
                }
                else {
                    // if not valid printing and asks for the degree again
                    cout << "Invalid Degree !! A polynomial cannot have a term with Fractional Degree or a Negative Degree\n";
                    cout << "Please again, ";
                }
                // after creation and adding all terms into it
                cout << "Current Polynomial : ";
                printpolyn(poly);
            }
            // if no polynomaials ahve been stored till now creating a linked lists to store pointers to Polynomials
            if(nofps==0){
                // basic linekd list implementation
                Polys = (Polyns **)malloc(sizeof(Polyns *));
                Polyns * tmp = (Polyns *)malloc(sizeof(Polyns));
                tmp->poly = poly;
                tmp->next = NULL;
                *Polys = tmp;
            }
            // or if linked list of polynomials exists just add this polynomial at the end
            else addpolyn(Polys, poly);
            nofps++;
            cout << "Successfully Added : ";
            printpolyn(poly);
        }
        else if(op==2){
            // if no polynomials prints the same
            if(nofps==0) cout << "No Polynomials to Delete !!!" << endl;
            else{
                // displays and asks user what polynomial to delete similar to display 'op=0'
                cout << "Polynomials stored are : " << endl;
                Polyns * ptr = *Polys;
                int ct=1;
                while(ptr->next!=NULL){
                    cout << "Polynomial " << ct++ << " : ";
                    printpolyn(ptr->poly);
                    ptr=ptr->next;
                }
                cout << "Polynomial " << ct++ << " : ";
                printpolyn(ptr->poly);
                // asking user which polynomial to delete
                int num;
                cout << "Enter the number of Polynomial to be Deleted : ";
                cin >> num;
                // ensuring input is valid natural number
                if(num<1 || num>nofps) cout << "Invalid Polynomial Number !!!\n";
                else{
                    // if valid deleting polynomial
                    cout << "Successfully Deleted Polynomial : ";
                    deletepoly(Polys, num);
                }
            }
        }
        else if(op==3){
            // if no polynomials prints the same
            if(nofps==0) {cout << "No Polynomials to add a term to !!!" << endl; continue;}
            // just prinitng some cautions
            cout << "Note : Adding negative of last term of the Polynomial makes it empty i,e., zero !!!\n";
            // printing all polynomials for user to choose
            cout << "Polynomials stored are : " << endl;
            Polyns * ptr = *Polys;
            int ct=1;
            while(ptr->next!=NULL){
                cout << "Polynomial " << ct++ << " : ";
                printpolyn(ptr->poly);
                ptr=ptr->next;
            }
            cout << "Polynomial " << ct++ << " : ";
            printpolyn(ptr->poly);
            
            // asking user for polynomial to which they want to add a term to
            int num;
            cout << "Enter the number of Polynomial : ";
            cin >> num;
            // ensuring input is valid here
            if(num<1 || num>nofps) cout << "Invalid Polynomial Number !!!\n";
            else{
                // if valid going to pointer of asked polynomial
                Polyns * head = * Polys;
                num--;
                while(num--){
                    head = head->next;
                }
                // printing the polynomial
                cout << "Chosen Polynomial : ";
                printpolyn(head->poly);
                // asking for degree
                cout << "Enter the degree : ";
                double d;
                cin >> d;
                // ensuring degree is valid
                if(floor(d)==d && d>-1){
                    cout << "Enter the coefficient of x^" << d << " : ";
                    double c;
                    cin >> c;
                    // using addremoveterm to add term to requested polynomial 
                    addremoveterm(head->poly, d, c);
                    cout << "Added Successfully !!!\n";
                    cout << "Updated Polynomial : ";
                    printpolyn(head->poly);
                }
                else {
                    // if not prints the same
                    cout << "Invalid Degree !! A polynomial cannot have a term with Fractional Degree\n";
                }
            }
        }
        else if (op==4){
            // if no polynomials prints the same
            if(nofps==0) {cout << "No Polynomials to remove a term from !!!" << endl; continue;}
            // just prinitng some cautions
            cout << "Note : Removing all terms causes a Polynomial to become empty i,e., zero !!!\n";
            // displaying all polynmials for user to choose
            cout << "Polynomials stored are : " << endl;
            Polyns * ptr = *Polys;
            int ct=1;
            while(ptr->next!=NULL){
                cout << "Polynomial " << ct++ << " : ";
                printpolyn(ptr->poly);
                ptr=ptr->next;
            }
            cout << "Polynomial " << ct++ << " : ";
            printpolyn(ptr->poly);

            // asking user for polynomial to which they want to add a term to
            int num;
            cout << "Enter the number of Polynomial : ";
            cin >> num;
            // checking if it's valid
            if(num<1 || num>nofps) cout << "Invalid Polynomial Number !!!\n";
            else{
                // if valid going to the pointer of requested polynomial
                Polyns * head = * Polys;
                num--;
                while(num--){
                    head = head->next;
                }
                cout << "Chosen Polynomial : ";
                printpolyn(head->poly);
                // taking degree input and cheking its validity
                cout << "Enter the degree : ";
                double d;
                cin >> d;
                if(floor(d)!=d || d<-1) cout << "Invalid Degree !!\n";
                else{
                    // if valid using addremoveterm to delete from the polynomial
                    addremoveterm(head->poly, d);
                }
            }
        }
        else if(op==5){
            // if no polynomials prints the same
            if(nofps==0) {cout << "No Polynomials stored !!!\n"; continue;}
            // to display all polynomials for user to choose
            cout << "Polynomials stored are : " << endl;
                Polyns * ptr = *Polys;
                int ct=1;
                while(ptr->next!=NULL){
                    cout << "Polynomial " << ct++ << " : ";
                    printpolyn(ptr->poly);
                    ptr=ptr->next;
                }
                cout << "Polynomial " << ct++ << " : ";
                printpolyn(ptr->poly);

            // asking user to choose polynomial
            int num;
            cout << "Enter the number of Polynomial : ";
            cin >> num;
            // checking if the polynomial number is valid here or not
            if(num<1 || num>nofps) cout << "Invalid Polynomial Number !!!\n";
            else{
                // if valid going to pointer of chosen Polynomial
                Polyns * head = * Polys;
                num--;
                while(num--){
                    head = head->next;
                }
                cout << "Chosen Polynomial : ";
                printpolyn(head->poly);
                num=1;
                // askign if user wants to choose Epsilon
                cout << "To Choose the value of E press 1 : ";
                cin >> num;
                // if so asking user and calling addremoveterm with e from user
                if(num==1){
                    double e;
                    cout << "Enter the value of E to remove terms with absolute coefficient less than E : ";
                    cin >> e;
                    addremoveterm(head->poly, 0, 0, e);
                }
                // if not passing without e taken care by default epsilon
                else addremoveterm(head->poly, 0, 0);
            }
        }
        else if(op==6){
            // if no polynomials prints the same
            if(nofps==0) {cout << "No Polynomials stored !!!\n"; continue;}
            // asking for user to enter value of x for which f(x) needed to be calculated
            double x;
            cout << "Enter the value (x) : ";
            cin >> x;
            // displaying all polynomials for user to choose
            cout << "Polynomials stored are : " << endl;
            Polyns * ptr = *Polys;
            int ct=1;
            while(ptr->next!=NULL){
                cout << "Polynomial " << ct++ << " : ";
                printpolyn(ptr->poly);
                ptr=ptr->next;
            }
            cout << "Polynomial " << ct++ << " : ";
            printpolyn(ptr->poly);
            // asking for number and going to the pointer of Polynomial if number is valid
            int num;
            cout << "Enter the number of Polynomial : ";
            cin >> num;
            if(num<1 || num>nofps) cout << "Invalid Polynomial Number !!!\n";
            else{
                // if valid go to the pointer of polynomial and call the evaluate function
                Polyns * head = * Polys;
                num--;
                while(num--){
                    head = head->next;
                }
                cout << "Chosen Polynomial : ";
                printpolyn(head->poly);
                evaluate(head->poly, x);
            }
        }
        else if(op==7){
            // if no polynomials prints the same
            if(nofps==0) {cout << "No Polynomials stored !!!\n"; continue;}
            // displaying all polynomials for the user to choose two from them 
            cout << "Polynomials stored are : " << endl;
            Polyns * ptr = *Polys;
            int ct=1;
            while(ptr->next!=NULL){
                cout << "Polynomial " << ct++ << " : ";
                printpolyn(ptr->poly);
                ptr=ptr->next;
            }
            cout << "Polynomial " << ct++ << " : ";
            printpolyn(ptr->poly);
            // asking user number of Polynomial 1 and 2
            int n1,n2;
            cout << "Enter the numbers of Polynomials to add : (ex: 1 2) ";
            cin >> n1 >> n2;

            // checking if they are valid or not
            if(n1<1 || n2<1 || n1>nofps || n2>nofps){
                cout << "Invalid Polynomial Number !!!\n";
            }

            else{
                // if valid go to the pointers of Polynomial numbers entered and printing them 
                Polyns * head1 = * Polys;
                n1--;
                while(n1--){
                    head1 = head1->next;
                }
                cout << "Chosen Polynomial 1 : ";
                printpolyn(head1->poly);
                Polyns * head2 = * Polys;
                n2--;
                while(n2--){
                    head2 = head2->next;
                }
                cout << "Chosen Polynomial 2 : ";
                printpolyn(head2->poly);

                // creating a new polynomial to store addition of both polynomials 
                Polyn * poly = (Polyn *)malloc(sizeof(Polyn));
                poly = (*(head1->poly))+(*(head2->poly));

                // adding sum of two polynomials to linked list that stores Polynomials
                addpolyn(Polys, poly);
                // incrementing number of polynomials
                nofps++;

                cout << "Successfully added given two Polynomials !!!\n";
            }
        }
        else if (op==8){
            // if no polynomials prints the same
            if(nofps==0) {cout << "No Polynomials stored !!!\n"; continue;}
            // dislpaying all polynomials for the user to choose
            cout << "Polynomials stored are : " << endl;
            Polyns * ptr = *Polys;
            int ct=1;
            while(ptr->next!=NULL){
                cout << "Polynomial " << ct++ << " : ";
                printpolyn(ptr->poly);
                ptr=ptr->next;
            }
            cout << "Polynomial " << ct++ << " : ";
            printpolyn(ptr->poly);

            // asking user to choose polynomial to differentiate
            int num;
            cout << "Enter the number of Polynomial to differentiate : ";
            cin >> num;
            
            // checking it's validity here
            if(num<1 || num>nofps) cout << "Invalid Polynomial Number !!!\n";
            else{
                // if valid going to its pointer
                ptr = * Polys;
                num--;
                while(num--){
                    ptr = ptr->next;
                }
            }

            // printing polynomial and asking whether to store derivative seperately or edit current polynomial
            cout << "Choosen Polynomial : ";
            printpolyn(ptr->poly);
            int op=1;
            cout << "1. Store derivative separately\n2. To differentiate current Polynomial and modify it\nEnter your option : ";
            cin >> op;
            if(op==1){
                // to store separately creating a new polynomial 
                Polyn * derp;
                Polyn * p = ptr->poly;

                // calling derivative funciton on polynomial
                derp = derivative(p);

                cout << "Differentiated Polynomial : ";
                printpolyn(derp);

                // adding new derivative polynomial to linked list which stores all Polynomials
                addpolyn(Polys, derp);
                // incrementing # of polynomials
                nofps++;
            }
            else{
                // to modify current polynomail
                Polyn * p = ptr->poly;
                p->c=0;
                // iterating through to repalce cx^d by c*dx^(d-1) **elementary differentiation on each term
                while(p->next!=NULL){
                    p = p->next;
                    // specially handling term with degree 1 according to my Polynomial structure
                    if(p->d==1){
                        double c = p->c;
                        addremoveterm(ptr->poly, 0, c);
                        addremoveterm(ptr->poly, 1);
                        continue;
                    }
                    p->c = (p->c) * (p->d);
                    p->d = (p->d) - 1;
                }
                // printing differentiated polynomial
                cout << "Differentiated Polynomial : ";
                printpolyn(ptr->poly);
            }
        }
        else if (op==9){
            // if no Polynomials prints the same
            if(nofps==0) {cout << "No Polynomials stored !!!\n"; continue;}
            // displays all polynomials for user to choose
            cout << "Polynomials stored are : " << endl;
            Polyns * ptr = *Polys;
            int ct=1;
            while(ptr->next!=NULL){
                cout << "Polynomial " << ct++ << " : ";
                printpolyn(ptr->poly);
                ptr=ptr->next;
            }
            cout << "Polynomial " << ct++ << " : ";
            printpolyn(ptr->poly);

            // asking user to choose polynomial
            int num;
            cout << "Enter the number of Polynomial to find Root using Newton's Method : ";
            cin >> num;
            
            // checking validity of polynomial number choosen
            if(num<1 || num>nofps) cout << "Invalid Polynomial Number !!!\n";
            else{
                // if valid going to pointer of polynomial choosen
                ptr = * Polys;
                num--;
                while(num--){
                    ptr = ptr->next;
                }
            }

            cout << "Choosen Polynomial : ";
            printpolyn(ptr->poly);

            // asking for start value for finding root
            double x0;
            cout << "Enter the Value of x0 for Newton's Root Method : ";
            cin >> x0;

            num=1;
            // asking if user wants to choose epsilon or not
            cout << "To Choose the value of Epsilon press 1 : ";
            cin >> num;
            // if so taking input and calling zero_newton with e
            if(num==1){
                double e;
                cout << "Enter the value of Epsilon : ";
                cin >> e;
                zero_newton(ptr->poly, x0, e);
            }
            // if not calling without e handled by default value
            else zero_newton(ptr->poly, x0);
        }
        else{
            // asking user again if he really wants to exit
            cout << "Are you sure you want to exit ?\nEnter 1 to return back to Menu : ";
            cin >> ex;
            // if yes while loop breaks
            if(ex!=1) ex=0;
            // if no it continues next menu print and asks for operation
        }

    }
    // End of Program :)))
    cout << "Terminated !!! :)" << endl;
    return 0;
}