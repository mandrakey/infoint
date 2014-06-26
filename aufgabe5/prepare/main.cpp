#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include <cstring>

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

    char buffer[1024];
    int i = 0;
    while(!in.eof())
    {
        in.getline(buffer, 1024);
        cout<<buffer<<endl;
        if (find(tuple.begin(), tuple.end(), i)!= tuple.end())
        {
            char *tmp = strtok(buffer, ";");
            int j = 1;
            while(tmp)
            {
                if (find(attribute.begin(), attribute.end(), j)!= attribute.end())
                {
                    if(j != attribut_rand_set)
                    {
                        out<<tmp<<";";
                    }
                    else
                    {
                        out<<tmp;
                    }

                }
                strtok(0, ";");
                j++;
            }
            out<<endl;
        }
        i++;
    }
    in.close();
    out.close();

    cout << "Feddisch!" << endl;
    return 0;
}

