#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <deque>
#include <algorithm>
#include <sstream>
#include <iomanip>
using namespace std;

class Table
{
public:

    Table(int id) :id(id), status(true) {};

    int getID() {
        return id;
    }
    void setID(int newID) {
        id = newID;
    }

    void setStartTime(int newTime) {
        StartTime = newTime;
    }

    int getSumTime() {
        return SumTime;
    }

    int getStatus() {
        return status;
    }
    void setStatus(bool newStatus) {
        status = newStatus;
    }

    string getUser() {
        return user;
    }
    void setUser(string newUser) {
        user = newUser;
    }

    int getSummary() {
        return summary;
    }

    void timeAndMoney(int time) {
        int a = time - StartTime;
        SumTime += a;
        summary += (a / 60) + ( (a % 60) > 0 ? 1 : 0);
        status = true;
    }

    void newUser(string username, int sTime)
    {

        user = username;
        StartTime = sTime;
        status = false;

    }

private:
    int id;
    int StartTime = 0, SumTime = 0 ;
    int summary = 0;
    bool status;
    string user;
};

//компоратор по пользователям
bool comp(Table i, Table j) {
    return i.getUser() < j.getUser();
}
//компоратор по ID
bool comp_1(Table i, Table j) {
    return i.getID() < j.getID();
}
//перевод целочисленного значения минут во временной стандарт
string timestr(int time) {

    stringstream ss;
    string out;
    ss << setfill('0') << setw(2) << time / 60 << ":" << setfill('0') << setw(2) << time % 60;
    ss >> out;
    return out;
}

class Club
{

public:

    Club(int tableAmount, int payForHour, int endTime) :payment(payForHour), endTime(endTime)
    {

        for (int i = 1; i <= tableAmount; ++i)
            tables.push_back(Table(i));

    }
    int newUser(string username)
    {

        if (isUserHere(username)) {
            return 11;
        }
 
        else {
            users.push_back(username);
        }
    }

    int userWait(string username)
    {
        if (waitingUsers.size() == tables.size())
            return 404;
       
        if (findEmptyTable() != tables.end()) 
            return 505;
       
            waitingUsers.push_back(username);
  
        return 0;
    }

    bool isUserHere(string username)
    {
        return find(users.begin(), users.end(), username) != users.end();

    }

    vector<Table>::iterator findEmptyTable()
    {

        return find_if(tables.begin(), tables.end(), [](Table& t) { return t.getStatus(); });

    }

    int sitUser(string username, int time, int id)
    {

        if (!isUserHere(username)) return 20; //usera netu


        if (!tables[id - 1].getStatus()) return 22; //zanyato

        getUpUser(username, time);

        tables[id - 1].newUser(username, time);


        return 2;

    }

    int getUpUser(string username, int time)
    {

        vector<Table>::iterator it = find_if(tables.begin(), tables.end(), [username](Table& t)
            { return  (username == t.getUser()); });

        if (it == tables.end()) return 13;

        (*it).timeAndMoney(time);

        int tmpId = (*it).getID();

        if (!waitingUsers.empty())
        {
            sitUser(waitingUsers.front(), time, tmpId);

            cout << timestr(time) << " 12 " << waitingUsers.front() <<' '<< tmpId << endl;
            waitingUsers.pop_front();
            
            return 12; //
        }

        return 4;
    }

    void Vihod()
    {
        waitingUsers.clear();
        sort(tables.begin(),tables.end(),comp);

        for (Table pc : tables)
        {
            if (!pc.getStatus())
            {
                getUpUser(pc.getUser(), endTime);
                cout << timestr(endTime) << " 11 " << pc.getUser() << endl;

            }
        }
    }

    void printResults()
    {   
        sort(tables.begin(), tables.end(), comp_1);
        cout << timestr(endTime) << endl;
        for (Table pc : tables)
        {

            cout << pc.getID() << " " << pc.getSummary() * payment<< " " << timestr(pc.getSumTime()) << endl;

        }
    }
 
private:

    vector<Table> tables;
    deque<string> waitingUsers;
    vector<string> users;
    int payment;
    int sTime, endTime;

};

//перевод временого стандарта в минуты, для вычисления
int getTime(string time) {
    int hours, minutes;
    hours = stoi(time.substr(0, 2));
    if (hours > 24 || hours < 0) return 0;
    minutes = stoi(time.substr(3, 2));
    if (minutes > 60 || minutes < 0) return 0;
    return (hours * 60 + minutes);
}

int main(int argc, char* argv[])
{

    if (argc < 2)
    {

        cout << " Too few arguments\n";

        return 1;

    }
    if (argc > 2)
    {

        cout << " Too much arguments\n";
        return 1;
    }

    string input;
    int kolstolov, money;
    string path = argv[1];
    ifstream fin;
    fin.open(path);
    int i = 0;
    string time,temp,id,client,changeid;
    int startTime, closeTime;
    string start, stop;
    int ID;
    int current_line = 0;
    //цикл считывыания колличества столов,времени открытия и закрытия и ставки за час использования
    if (!fin.is_open())
    {
        cout << "can't open file" << endl;
    }
    else
    {
        while (getline(fin, input))
        {
            current_line++;
            if (current_line == 1)
            {
                //колличество столов
                kolstolov = stoi(input);
                if (kolstolov == 0 || kolstolov < 0)
                {
                    cout << "stolov ne mojet bit 0 ili <0 !" << endl;
                    system("pause");
                    return 1;
                }

            }
            if (current_line == 2)
            {
                //время открытия и закрытия
                time = input;
                start = input.substr(0, 5);
                cout << start << endl;
                stop = input.substr(6, 10);
                startTime = getTime(start);
                closeTime = getTime(stop);
                if (startTime == 0 || closeTime == 0 || (startTime >= closeTime))
                {
                    cout << "Invalid time period!";
                    system("pause");
                    return 1;
                }
            }
            if (current_line == 3)
            {
                //ставка за час использования
                money = stoi(input);
                if (money == 0 || money < 0)
                {
                    cout << "money ne mojet bit 0 ili <0 !" << endl;
                    system("pause");
                    return 1;
                }
            }
        }

    };
    //цикл обработки входящих сообщений
    fin.close();
    fin.open(path);
    if (!fin.is_open())
    {
        cout << "ошибка открытия файла!" << endl;
    }
    else
    {
        Club day(kolstolov, money,closeTime);
        current_line = 0;
        while (!fin.eof())
        {
            current_line++;
                getline(fin, input);
                if (current_line >= 3) {
                fin >> time; cout << time << " "; fin >> id; cout << id << " "; fin >> client; cout << client << " ";
                ID = stoi(id);
                if (ID == 2) { fin >> changeid; cout << changeid << endl; }
                else cout << endl;

                getTime(time);
                int Temp = 0;
                switch (ID)
                {
                case 1:
                    if (getTime(time) < startTime) cout << time << " 13 " << "NotOpenYet" << endl;
                    else if (day.newUser(client) == 11) cout << time << " 13 " << "YouShallNotPass" << endl;
                    break;
                case 2:
                    Temp = day.sitUser(client, getTime(time), stoi(changeid));      
                    break;
                case 3:
                    
                    Temp = day.userWait(client);            
                    break;

                case 4:
                    Temp = day.getUpUser(client, getTime(time));
                    break;

                }
                switch (Temp)
                {

                case 404:
                    cout << time << " 4 " << client << endl;
                    break;
                case 505:

                    break;
                case 20:
                    cout << time << " 13 " << client << endl;
                    break;
                case 22:
                    cout << time << " 13 " << "PlaceIsBusy" << endl;
                    break;
                case 13:
                    cout << time << " 13 " << "ClientUnknown" << endl;
                    break;
                }
            }
        }
        // вывод окончания рабочего дня
        day.Vihod();
        day.printResults();
    }
    system("pause");
    fin.close();
    return 0;
}
