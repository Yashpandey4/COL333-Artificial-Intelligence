#include<iostream>
#include<vector>

using namespace std;

int main(int argc, char const *argv[])
{
    /* code */
    vector<int*>p;
    int x = 2;
    int y = 3;
    int z = 4;
    p.push_back(&x);
    p.push_back(&y);
    p.push_back(&z);

    for (int i = 0; i < p.size(); i++)
    {
        cout << *p[i] << " ";
    }
    cout << endl;

    p.erase(p.begin(),p.end());
    // p.clear();
    // p.shrink_to_fit();
    // p.push_back(&x);
    p.push_back(&x);
    p.push_back(&y);
    p.push_back(&z);
    cout << p.capacity() <<endl;

    return 0;
}
