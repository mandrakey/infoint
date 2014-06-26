#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include <cstring>
#include <QString>
#include <QStringList>

using namespace std;

int rnd(int lowerbounds, int upperbounds)
{
   return lowerbounds + rand() % (upperbounds - lowerbounds + 1);
}

vector<int> create_list(int count, int lowerbound, int upperbound)
{
    vector<int> list;
    for (int i = 0; i < count; i++)
    {
        int k = rnd(lowerbound, upperbound);

        while(find(list.begin(), list.end(), k)!= list.end())
        {
            k = rnd(lowerbound, upperbound);
        }

        list.push_back(k);
    }

    return list;

}

int main()
{
    srand(time(0));

    //Random choice of attributes and tuples
    int tuple_rand_set = rnd(70, 150);
    int attribut_rand_set = rnd(5, 7);

    //creats  vectors for preparing dataset
    vector<int> tuple = create_list(tuple_rand_set, 1, 483);
    vector<int> attribute = create_list(attribut_rand_set, 1, 23);

    //cut off data
    ifstream in("5EnginesA.csv", ios::in);
    ofstream out("horst.csv", ios::out);
    if (!in.is_open())
    {

        cout << "Fuck, ein Fehler!" << endl;
        return 1;
    }

    if (!out.is_open())
    {
        cout << "Fuck, die dumme Datei will nicht!" << endl;
        return 1;
    }

    int i = 0;
    while(!in.eof())
    {
        char* buffer = new char[1024];
        in.getline(buffer, 1024);
        if (find(tuple.begin(), tuple.end(), i)!= tuple.end())
        {
            //char *tmp;
            int j = 1, numout = 1;
            QStringList tmp = QString(buffer).split(";");
            for (j = 1; j <= tmp.size(); ++j) {
                if (find(attribute.begin(), attribute.end(), j)!= attribute.end())
                {
                    out << tmp[j - 1].toStdString();
                    if (numout++ < attribut_rand_set) {
                        out << ";";
                    }

                }
            }

//            while((tmp = strsep(&buffer, ";")) != 0)
//            {
//                if (find(attribute.begin(), attribute.end(), j)!= attribute.end())
//                {
//                    out << tmp;
//                    if (numout++ < attribut_rand_set) {
//                        out << ";";
//                    }

//                }
//                j++;
//            }
            out<<endl;
        }
        delete buffer;
        i++;
    }
    in.close();
    out.close();

    cout << "Feddisch!" << endl;
    cout << "Tupel: " << tuple_rand_set << endl;
    return 0;
}

