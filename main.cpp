#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include <sstream>
#include <ctime>
#include <cstdlib>
#include <chrono>
#include <cmath>


using namespace std;



bool admin_login_flag=false;

class User{
    
    string password;
    public:
    string user_id;
    int user_type;
    //int find_username_password(string userid,string password);
    int find_id_passwd(string userid,string password,string filename);
    void create_account();
    void user_login();
    friend class Admin;
};

void User::create_account(){
      string temp;
      cout<<"Enter a user_id of your choosing:";
      cin>>user_id;
      cout<<"Enter your password:";
      cin>>temp;
      while(temp.length()<5){
         cout<<"your Password is too short. Please choose another password!"<<endl;
         cout<<"Enter your new password:";
         cin>>temp;
      }
      password=temp;
      if (user_type==3){
        fstream file;
        file.open("customer.csv");
        file<<user_id<<","<<password<<endl;
        file.close();
      }
      
    }

int User :: find_id_passwd(string userid,string password,string filename){
    int value=0;
    ifstream file;
    file.open(filename);
    if (!file) {
        cout <<"Error opening file"<<filename<<endl;
        return -1; 
    }
    string line;
    vector<string>userid_vec;
    vector<string>password_vec;
    while(getline(file,line)){
       int pos=line.find(',');
       string uid= line.substr(0,pos);
       string passwd= line.substr(pos+1);
       userid_vec.push_back(uid);
       password_vec.push_back(passwd);
    }
    
    for(int i=0;i < userid_vec.size();i++){
        if((userid_vec[i]==userid) && (password_vec[i]==password)){
            value=1;
            break;
        }
    }
    return value;   
}

void User::user_login()
{
    string login_id;
    string login_password;
    bool login_success = false; 
    switch (user_type) {
    case 1:
        cout << "Enter your admin id:";
        cin >> login_id;
        cout << "Enter your admin password:";
        cin >> login_password;
        if ((login_id == "admin") && (login_password == "admin")) {
            cout << "Login Successful. Welcome admin!";
            login_success = true;
            admin_login_flag=true;
        }
        else {
            cout << "Incorrect Login ID or password!";
        }
        break;

    case 2:
        while (!login_success) { 
            cout << "Enter your staff id:";
            cin >> login_id;
            cout << "Enter your password:";
            cin >> login_password;
            if (find_id_passwd(login_id, login_password, "staffid.csv" ) == 1) {
                cout << "Login Successful. Welcome staff member!" << endl;
                login_success = true;
            }
            else {
                cout << "Incorrect login ID or password!" << endl;
            }
        }
        break;

    case 3:
        while (!login_success) { 
            cout << "Enter your user id:";
            cin >> login_id;
            cout << "Enter your password:";
            cin >> login_password;
            if (find_id_passwd(login_id, login_password, "customerid.csv" ) == 1) {
                cout << "Login Successful. Welcome user!" << endl;
                login_success = true;
                user_id=login_id;
            }
            else {
                cout << "Incorrect login ID or password!" << endl;
            }
        }
        break;
    }
}




class Books{
    int bookid;
    string bookname;
    string genre;
    string author;
    string publisher;
    int qty;
    public:  
      void increment_qty(string book_name,int increment);
};

void Books::increment_qty(string book_name,int increment){
    fstream file;
    bool book_found;
    file.open("books.csv");
    string lines;
    vector<string>bookdata_vec;
    string book_data;
    while(getline(file,lines)){
        int pos=lines.find(',');
        string bookn=lines.substr(0,pos);
        if(bookn==book_name){
             book_found=true;
             int pos_lastcomma=lines.rfind(',');
             string qty_str=lines.substr(pos_lastcomma+1);
             for (char c : qty_str) {
                if (!isdigit(c)) {
                    throw invalid_argument("Invalid quantity: " + qty_str);
                }
            }
             int qty=stoi(qty_str);
             qty=qty+increment;
             lines=lines.substr(0,pos_lastcomma+1)+ to_string(qty);           
        }
        bookdata_vec.push_back(lines);
    }
    file.close();
    if(!book_found){
        cout<<book_name<<" not found in the books file."<<endl;
        return;
    }
    ofstream file2;
    file2.open("books.csv");
    for(int i=0;i< bookdata_vec.size();i++){
        file2<<bookdata_vec[i]<<endl;
    }
    file2.close();

}  

class Customer: public User{
   string cname;
   int age;
   int phoneno;
   string email;
   double fine;
   string book;
   std::tm due_date;

   public:
   string c_id;
   friend class Staff;
   friend class Books;
   Customer(){
    c_id=user_id;
    user_type=3;
   }
    void register_customer();
    void display_profile();
    int is_book_available(string book);
    int is_book_borrowed(string book);
    void borrow_book(Books& obj);
    void return_book(Books& obj);
    int is_registered(string userid);
};


void Customer:: register_customer(){
           cout<<"Welcome "<<user_id<<endl;
           cout<<"Please furnish your other details!"<<endl;
           cout<<"Enter your name:"<<endl;
           cin>>cname;
           cout<<"Enter your age:"<<endl;
           cin>>age;
           cout<<"Enter your phone number:"<<endl;
           cin>>phoneno;
           cout<<"Enter your email ID:"<<endl;
           cin>>email;
           fstream file;
           file.open("customer.csv",ios::app);
           file<< user_id<<","<<cname<<","<<age<<","<<phoneno<<","<<email<<endl;
           file.close();
}

void Customer:: display_profile(){
      cout<<"     "<<user_id<<" 's Profile"<<endl;
      cout<<" Name: "<<cname<<endl;
      cout<<" Age:  "<<age<<endl;
      cout<<" Phone No: "<<phoneno<<endl;
      cout<<" Email: "<<email<<endl;
    }

void Customer:: borrow_book(Books& obj){
    string borrowed_book;
    cout<<"Enter the book you want to borrow:";
    cin>>borrowed_book;
    if(is_book_available(borrowed_book)==1){
        string date_borrow;
        cout<<"The book you wish to borrow is available!"<<endl;
        cout<<"Enter your date of borrowing in DD/MM/YYYY:";
        cin>>date_borrow;
        obj.increment_qty(borrowed_book,-1);
        ofstream file2;
        file2.open("borrowing.csv",ios::app);
        file2<<user_id<<","<<borrowed_book<<","<<date_borrow<<endl;
        file2.close();

    }
    else{
        cout<<"The book is not available!"<<endl<<"Please borrow some other book.";
    }

}

void Customer::return_book(Books& obj) {
    string returned_book;
    cout << "Enter the book you want to return:";
    cin >> returned_book;
    if (is_book_borrowed(returned_book)) {
        string date_returned;
        cout << "The book you wish to return is borrowed by you." << endl;
        cout << "Enter the date of return in DD/MM/YYYY:";
        cin >> date_returned;
        obj.increment_qty(returned_book, 1);
        ifstream file1("borrowing.csv");
        ofstream file2("temp.csv", ios::app);
        string user_id_borrowed, book_borrowed, date_borrowed;
        while (getline(file1, user_id_borrowed, ',') && getline(file1, book_borrowed, ',') && getline(file1, date_borrowed)) {
            if (user_id_borrowed == user_id && book_borrowed == returned_book) {
                continue;
            }
            file2 << user_id_borrowed << "," << book_borrowed << "," << date_borrowed << endl;
        }
        file1.close();
        file2.close();
        remove("borrowing.csv");
        rename("temp.csv", "borrowing.csv");
        cout << "The book " << returned_book << " has been returned." << endl;
    } else {
        cout << "The book is not borrowed by you!" << endl;
    }
}

int Customer::is_book_available(string book){
    ifstream file;
    file.open("books.csv");
    string lines;
    string reversed_lines;
    vector<string>bookname_vec;
    vector<int>bookqty_vec;
    while(getline(file,lines)){
        int pos1;
        int pos2;
        pos1=lines.find(',');
        string bookn=lines.substr(0,pos1);
        reversed_lines= string(lines.rbegin(),lines.rend());
        pos2=reversed_lines.find(',');
        string rev_qty=reversed_lines.substr(0,pos2);
        string qty= string(rev_qty.rbegin(),rev_qty.rend());
        int qty_int=stoi(qty);
        bookname_vec.push_back(bookn);
        bookqty_vec.push_back(qty_int);
    }
        int req_pos;
        for(int i=0;i< bookname_vec.size();i++){
           if(bookname_vec[i]==book){
             req_pos=i;
           }

        }
        if(bookqty_vec[req_pos]>0){
            return 1;
        } 
        else{
            return 0;
        }

}

int Customer::is_book_borrowed(string book){
    ifstream file;
    file.open("borrowing.csv");
    string lines;
    vector<string>borrowed_books;
    while(getline(file,lines)){
        int pos;
        pos=lines.find(',');
        string borrowed_book=lines.substr(pos+1);
        borrowed_books.push_back(borrowed_book);
    }
    for(int i=0;i< borrowed_books.size();i++){
        if(borrowed_books[i]==book){
            return 1;
        }
    }
    return 0;
}

int Customer::is_registered(string userid){
    int return_value=0;
    ifstream file;
    file.open("customer.csv");
    string lines;
    vector<string>customerids_vec;
    while(getline(file,lines)){
        int pos=lines.find(',');
        string cusids=lines.substr(0,pos);
        customerids_vec.push_back(cusids);
        for(int i=0;i<customerids_vec.size();i++){
            if(customerids_vec[i]==userid){
                 return_value=1;
                 
            }
            
        }

    }
    return return_value;
    
}

class Staff: public User{
   int StaffRno;
   public:
   string C_id;
   Staff(){
    C_id=user_id;
    user_type=2;
   }
   void getuser();
   void viewBorrowingHistory(const string& C_id);
};

void getuser() {
        std::string C_id;
        cout<<"Enter the user id: "<<endl;
        cin>>C_id;
        void viewBorrowingHistory(const string& C_id);

   }

void Staff::viewBorrowingHistory(const string& C_id){
    std::ifstream file("borrowing.csv");

    if (!file) {
        std::cerr << "Error: borrowing.csv file not found" << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string bookId, customerId, borrowDate, returnDate;
        if (!(iss >> bookId >> customerId >> borrowDate >> returnDate)) {
            continue;
        }

        if (customerId == C_id) {
            std::cout << "Book ID: " << bookId << ", Borrow Date: " << borrowDate << ", Return Date: " << returnDate << std::endl;
        }
    }
}




class Admin: public User{
     public:
      void remove_user(); 
      void update_user_info();
      void admin_driver();

};
void Admin::remove_user(){
     string user_id_remove;
     cout<<"Enter the User ID of the user you want to remove:";
     cin>>user_id_remove;
     fstream file;
     file.open("customerid.csv");
     string lines;
     vector<string>id_vec;
     vector<string>passwd_vec;
     while(getline(file,lines)){
        int pos;
        string id;
        string passwd;
        pos=lines.find(',');
        id=lines.substr(0,pos);
        passwd=lines.substr(pos+1);
        id_vec.push_back(id);
        passwd_vec.push_back(passwd);  
     }
     int pos_to_be_removed=-1;
     for(int i=0;i< id_vec.size();i++){
        if(id_vec[i] == user_id_remove){
            pos_to_be_removed=i;
            break;
        }
     }
     if (pos_to_be_removed == -1) {
     cout <<"User ID "<< user_id_remove << " not found." << endl;
     return;
     }
     vector<string>::iterator iter1= id_vec.begin()+ pos_to_be_removed; 
     vector<string>::iterator iter2= passwd_vec.begin()+ pos_to_be_removed;
     id_vec.erase(iter1);
     passwd_vec.erase(iter2);
     file.close();
     ofstream file2;
     file2.open("customerid.csv");
     for(int i=0;i<id_vec.size();i++){
            file2<<id_vec[i]<<","<<passwd_vec[i]<<endl;
      }
     file2.close();
     cout<<user_id_remove<<" has been removed."<<endl;
}

void Admin::update_user_info() {
    string user_id_update;
    cout << "Enter the User ID of the user you want to update: ";
    cin >> user_id_update;

    // Search for the user in the file and get the position
    fstream file;
    file.open("customer.csv");
    string lines;
    vector<string>id_vec;
    vector<string>name_vec;
    vector<int>age_vec;
    vector<string>phone_vec;
    vector<string>email_vec;
    while(getline(file, lines)){
        int pos1, pos2, pos3, pos4;
        string id, name, phone, email;
        int age;

        pos1 = lines.find(',');
        id = lines.substr(0, pos1);

        pos2 = lines.find(',', pos1 + 1);
        name = lines.substr(pos1 + 1, pos2 - pos1 - 1);

        pos3 = lines.find(',', pos2 + 1);
        age = stoi(lines.substr(pos2 + 1, pos3 - pos2 - 1));

        pos4 = lines.find(',', pos3 + 1);
        phone = lines.substr(pos3 + 1, pos4 - pos3 - 1);

        email = lines.substr(pos4 + 1);
        if (id == user_id_update) {
            cout << "Enter new name: ";
            cin >> name;
            cout << "Enter new age: ";
            cin >> age;
            cout << "Enter new phone number: ";
            cin >> phone;
            cout << "Enter new email ID: ";
            cin >> email;
        }
        id_vec.push_back(id);
        name_vec.push_back(name);
        age_vec.push_back(age);
        phone_vec.push_back(phone);
        email_vec.push_back(email);  
     }
     file.close();
     
     // Rewrite the file with updated information
     ofstream file2;
     file2.open("customer.csv");
     for(int i=0;i<id_vec.size();i++){
            file2<<id_vec[i]<<","<<name_vec[i]<<","<<age_vec[i]<<","<<phone_vec[i]<<","<<email_vec[i]<<endl;
      }
     file2.close();
     cout<<"User info for "<< user_id_update << " has been updated."<<endl;
}

void Admin::admin_driver(){
    Admin a;
    a.user_login();
    if(admin_login_flag){
        int ch;
        do{
           cout<<"Welcome to Admin Panel"<<endl;
           cout<<"-----------------------------------------------------------------------"<<endl;
           cout<<"1) Remove User\n2) Update User Info\n3) Logout"<<endl;
           cout<<"Enter your choice: ";
           cin>>ch;
           switch(ch) {
                case 1:
                    a.remove_user();
                    break;
                case 2:
                    a.update_user_info();
                    break;
                default:
                    break;        
           }
        } while(ch!=3);
    }
}


int main() {
    int choice;
    bool exit = false;
    User user;
    Customer customer;
    Staff staff;
    Books books;

    while (!exit) {
        cout << "\nWelcome to the Library Management System!" << endl;
        cout << "1. Login as admin" << endl;
        cout << "2. Login as staff" << endl;
        cout << "3. Login as customer" << endl;
        cout << "4. Create a new user account" << endl;
        cout << "5. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                user.user_type = 1;
                user.user_login();
                if (admin_login_flag) {
                    // admin tasks
                    cout << "You are logged in as an admin." << endl;
                    Admin admin;
                    admin.admin_driver();
                }
                break;

            case 2:
                staff.user_login();
                // staff tasks
                cout << "You are logged in as a staff member." << endl;
                staff.getuser();
                break;

            case 3:{
                customer.user_login();
                // customer tasks
                cout << "You are logged in as a customer." << endl;
                
            int choice,flag;
            Customer cust;
                            do{
                                cout<<"\n\n";
                                cout<<"Welcome "<<endl;
                                cout<<"-----------------------------------------------------------------------"<<endl;
                                cout<<"1. Display Profile\n2. Borrow a Book\n3. Return a Book\n4. Logout"<<endl;
                                cout<<"Enter your choice:";
                                cin>>choice;
                                switch(choice){
                                    case 1:{
                                        string temp_id=cust.c_id;
                                        if(cust.is_registered(temp_id)==1){
                                            cust.display_profile();
                                        }
                                        else{
                                            cout<<"It seems your profile is not registered."<<endl;
                                            cout<<"Don't worry,we'll register you right away!"<<endl;
                                            cust.register_customer();
                                            cout<<"Here's your profile!"<<endl;
                                            cust.display_profile();
                                        }
                                        
                                        break;
                                    }
                                    case 2:{
                                        Books book;
                                        cust.borrow_book(book);
                                        break;
                                    }
                                    case 3:{
                                        Books book;
                                        cust.return_book(book);
                                        break;
                                    }
                                    case 4:{
                                        flag=1;
                                        break;
                                    }
                                    default:{
                                        cout<<"Invalid choice!";
                                        break;
                                    }
                                }
                            }while(flag!=1);
                            break;
                        }
                       
            case 4:
                cout << "Enter user type (1=admin, 2=staff, 3=customer): ";
                cin >> user.user_type;
                user.create_account();
                break;

            case 5:
                exit = true;
                cout << "Thank you for using the Library Management System!" << endl;
                break;

            default:
                cout << "Invalid choice. Please try again." << endl;
                break;
        }
    }

    return 0;
}




