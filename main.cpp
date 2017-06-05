#include <iostream>
#include <string>
#include <windows.h>
#include <mysql.h>
#include <sstream>
#include <limits>
#include <algorithm>

using namespace std;

MYSQL* conn;
MYSQL_ROW row;
MYSQL_RES *res;
int choice;

class planner{
public:
    void addActivity();
    void deletedata(MYSQL* conn, int);
    void searchBySubject();
    void addByDay();
    void searchByImportance();
    void connect();
    void searchByDate();

private:
    string id, date, time, subject, activity, importance;
    int importanceInt;
    int qstate;
    int importanceChoice;

};

//connecting to database
void planner::connect() {
    conn = mysql_init(0);
    conn = mysql_real_connect(conn,"localhost","root","","planner",3306,NULL,0);  //enter database location

    if(conn)
        cout<< "connection object ok, conn=" << conn << endl;
    else
        cout<< "conn object problem: " << mysql_error(conn);
}

// Adding activity
void planner::addActivity() {
    cout << "Enter details of planned event: " << endl;
    cout << "date YYYY-MM-DD  !: "; getline(cin, date);
    cout << "time hours:minutes(eg. 12:12): "; getline(cin, time);
    cout << "subject of activity (max 50 characters): "; getline(cin, subject);
    cout << "enter activity: "; getline(cin, activity);
    cout << "enter importance 0-100: "; getline (cin, importance);

    qstate = mysql_query(conn,"SELECT * FROM plannertab");
    int idAux=0;
    int idAux2=0;
    int check=0;

    if(!qstate) {
        res = mysql_store_result(conn);

        while(row=mysql_fetch_row(res))
        {
            if(check == 0) {
                istringstream iss(row[0]); // convert string to int
                iss >> idAux;
                check = 1;
            }
            istringstream iss(row[0]); // convert string to int
            iss >> idAux2;
            if(idAux2 >= idAux)
                idAux = idAux2;
        }

    }
    else
        {
            cout<<"query error: "<<mysql_error(conn)<<endl;
        }
    idAux++;
    ostringstream ss; // convert back int to string
    ss << idAux;
    id = ss.str();

    string query = "INSERT INTO plannertab(id,date,time,subject,activity,importance) VALUES('"+id+"','"+date+"','"+time+"','"+subject+"','"+activity+"','"+importance+"')";

    const char* q = query.c_str();

    if(!qstate) {
        cout << "record inserted successfully..." << endl;
        cout << "query is: " << q <<endl;
        qstate = mysql_query(conn,q);
    }
    else
        cout << "query problem: " << mysql_error(conn) << endl;

    qstate = mysql_query(conn,"SELECT * FROM plannertab");
    planner activityUser;
    mysql_close(conn);
    activityUser.connect();
    cout << "\nPress enter to continue" << endl;
    cin.get();
}

// Searching activity by subject
void planner::searchBySubject() {
    cout << "Enter subject which you want to find: ";
    string subjectChoice;
    getline(cin, subjectChoice);

    qstate = mysql_query(conn,"SELECT * FROM plannertab");


      if(!qstate) {
            res = mysql_store_result(conn);
            while(row=mysql_fetch_row(res))
            {
                string subjectSearch = row[3]; // assigning value of row to auxiliary string
                string subLower; // auxiliary string ready to receive lower-case letters
                subLower.resize(subjectSearch.length()); // resizing auxiliary string for lower-case letters to string obtained from table in database
                transform(subjectSearch.begin(),subjectSearch.end(),subLower.begin(),::tolower); // convert to lower-case letters
                if(subLower == subjectChoice) {
                    string timeShort;
                    timeShort = row[2]; // assigning time from table to  auxiliary string
                    timeShort.resize(5); //resizing auxiliary string in order to cut off seconds and milliseconds
                    cout<<"id: "<<row[0]<< " "
                       <<"date: "<<row[1]<< " "
                       <<"time: "<<timeShort<< " "
                       <<"subject: "<<row[3]<< " "
                       <<"active: "<<row[4]<< " "
                       <<"importance: "<<row[5]<<endl;
                }
            }
        }
        else
        {
            cout<<"query error: "<<mysql_error(conn)<<endl;
        }
        cout << "\nPress enter to continue" << endl;
        cin.get();
        cin.get();
}

// Searching by importance
void planner::searchByImportance() {
    cout << "Enter importance to search: "; cin >> importanceChoice;
    qstate = mysql_query(conn,"select * from plannertab");

    if(!qstate) {
        res = mysql_store_result(conn);
        while(row=mysql_fetch_row(res))
        {
            int i;
            string importance = row[5];
            istringstream iss(importance);
            iss >> i;
            string timeShort;
            timeShort = row[2]; // assigning time from table to  auxiliary string
            timeShort.resize(5); //resizing auxiliary string in order to cut off seconds and milliseconds
            if(importanceChoice==i) {
               cout<<"id: "<<row[0]<< " "
                   <<"date: "<<row[1]<< " "
                   <<"time: "<<timeShort<< " "
                   <<"subject: "<<row[3]<< " "
                   <<"active: "<<row[4]<< " "
                   <<"importance: "<<row[5]<<endl;
            }
        }
    }
    else {
        cout<<"query error: "<<mysql_error(conn)<<endl;
    }
    cout << "\nPress enter to continue" << endl;
    cin.get();
    cin.get();
    }

    // Searching by date
void planner::searchByDate() {
     cout << "Enter date which you want to find YYYY-MM-DD: ";
    string dateChoice;
    getline(cin, dateChoice);

    qstate = mysql_query(conn,"SELECT * FROM plannertab");


      if(!qstate) {
            res = mysql_store_result(conn);
            while(row=mysql_fetch_row(res))
            {
                string dateSearch = row[1]; // assigning value of row to auxiliary string
                string subLower; // auxiliary string ready to receive lower-case letters
                subLower.resize(dateSearch.length()); // resizing auxiliary string for lower-case letters to string obtained from table in database
                transform(dateSearch.begin(),dateSearch.end(),subLower.begin(),::tolower); // convert to lower-case letters
                if(subLower == dateChoice) {
                    string timeShort;
                    timeShort = row[2]; // assigning time from table to  auxiliary string
                    timeShort.resize(5); //resizing auxiliary string in order to cut off seconds and milliseconds
                    cout<<"id: "<<row[0]<< " "
                       <<"date: "<<row[1]<< " "
                       <<"time: "<<timeShort<< " "
                       <<"subject: "<<row[3]<< " "
                       <<"active: "<<row[4]<< " "
                       <<"importance: "<<row[5]<<endl;
                }
            }
        }
        else
        {
            cout<<"query error: "<<mysql_error(conn)<<endl;
        }
        cout << "\nPress enter to continue" << endl;
        cin.get();
        cin.get();
}

//deleting record (only from 1-9 !)
void planner::deletedata(MYSQL* conn, int rowToDelete){

    char *importance;
    char *q;
    string g= "DELETE FROM plannertab WHERE id=";
    char aba = rowToDelete + '0';
    string addedString;
    addedString = g;
    addedString.append(1, aba);
    int lengthOfString;
    lengthOfString=addedString.length();

    char characters[lengthOfString];
    addedString.copy( characters, lengthOfString );


   for(int i=0; i<sizeof(characters)/(sizeof(characters[0])); i++)
    cout << characters[i] << endl;

    q=characters;
    mysql_query(conn,q);
}

int main()
{
    system("title Personal planner");
    planner user;
    user.connect();

    while(1) {
    if(conn)
    {
        cout<<"connected to database planner"<<endl;
        cout<<"What would you like to do?"<<endl;
        cout<<"1 - Add new activity to the calendar"<<endl;
        cout<<"2 - Search for activity by it's subject name (full name)"<<endl;
        cout<<"3 - Search for activities by importance"<<endl;
        cout<<"4 - Search for activities by date YYYY-MM-DD"<<endl;
        cout<<"5 - Delete record"<<endl;

        cout << "Your choice: ";
        cin >> choice;
        cin.clear();
        cin.ignore(std::numeric_limits < std::streamsize >::max(), '\n');

        switch(choice){
        case 1:
            user.addActivity();
        break;

        case 2:
            user.searchBySubject();
        break;

        case 3:
            user.searchByImportance();
        break;

        case 4:
            user.searchByDate();
        break;

        case 5:
            int selectRow;
            cout << "Select ID of record to delete: "; cin >> selectRow;
            user.deletedata(conn, selectRow);
        break;
        }
    }
}
    mysql_close(conn);

    return 0;
}
