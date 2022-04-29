#include "SymbolTable.h"
bool isParam(string name)
{
    int dai = name.size();
    if (dai == 0)
        return 0;
    for (int i = 0; i < dai; i++)
    {
        if (!(name[i] <= '9' && name[i] >= '0'))
            return 0;
    }
    return 1;
}
bool isstring(string name)
{
    int dai = name.size();
    if (dai < 2)
        return 0;
    if (name[0] == (char)39 && name[dai - 1] == (char)39)
    {
        for (int i = 1; i < dai - 1; i++)
        {
            if (!((name[i] <= 'z' && name[i] >= 'a') || (name[i] <= 'Z' && name[i] >= 'A') || (name[i] <= '9' && name[i] >= '0') || name[i] == ' '))
                return 0;
        }
        return 1;
    }
    return 0;
}
long long chuyenKieu(string s)
{
    long long res = 0;
    for (int i = 0; i < (int)s.size(); i++)
    {
        res = res * 10 + (s[i] - '0');
    }
    return res;
}
struct dat
{
    string identifier;
    int level;
    string type[100];
    int sokieuham = 0;
    long long key;
    // int trongmang = 0;
    long long taokey()
    {
        string kq = "";
        kq += to_string(level);
        for (int i = 0; i < (int)identifier.size(); i++)
        {
            kq += to_string(identifier[i] - 48);
        }
        return chuyenKieu(kq);
    }
    dat(string identifier = "", int level = 0, string type = "")
    {
        this->identifier = identifier;
        this->level = level;
        if (type == "")
        {
            this->type[0] = "";
            sokieuham = 1;
        }
        else
        {
            // if (atoi(type.c_str()) + 1 > 100)
            // {
            // this->type = new string[atoi(type.c_str()) + 1];
            for (int i = 0; i < 100; i++)
            {
                this->type[i] = "";
            }
            sokieuham = atoi(type.c_str()) + 1;
            // }
            // else
            // {
            //     string a[100];
            //     for (int i = 0; i < 100; i++)
            //     {
            //         a[i] = "";
            //     }
            //     this->type = &a[0];
            //     sokieuham = atoi(type.c_str()) + 1;
            // }
        }

        key = taokey();
        // trongmang = 0;
    }
    ~dat()
    {
        // if (sokieuham > 100)
        //     delete[] this->type;
    }
};
enum STATUS_TYPE
{
    NIL,
    NON_EMPTY
};
class OpenHash
{
public:
    int (*hp)(long long, int, int, int, int);
    STATUS_TYPE *status;
    dat *dulieu;
    int m;
    int c1;
    int c2;

public:
    OpenHash(int (*hp)(long long, int, int, int, int), int m, int c1, int c2)
    {
        this->hp = hp;
        this->m = m;
        this->c1 = c1;
        this->c2 = c2;
        this->dulieu = new dat[m];
        this->status = new STATUS_TYPE[m];
        for (int i = 0; i < m; i++)
        {
            this->status[i] = NIL;
        }
        // for (int i = 0; i < m; i++)
        // {
        //     this->dulieu[i].trongmang = 1;
        // }
    }

    ~OpenHash()
    {
        delete[] this->dulieu;
        delete[] this->status;
    }

    int insert(dat moi)
    { // chuyen thanh string va int
        long long key = moi.key;

        int i = 0;
        while (i != m)
        {
            int a = hp(key, m, c1, c2, i);
            if (status[a] == NIL)
            {
                dulieu[a] = moi;
                status[a] = NON_EMPTY;
                return i;
            }
            i++;
        }
        return -1;
    }

    void remove(dat moi)
    {
        for (int j = moi.level; j >= 0; j--)
        {
            dat moihon(moi.identifier, j);
            long long key = moihon.key;
            int i = 0;
            while (i != m)
            {
                int a = hp(key, m, c1, c2, i);
                if (status[a] == NON_EMPTY && dulieu[a].key == key)
                {
                    dulieu[a].key = 0;
                    status[a] = NIL;
                    return;
                }
                i++;
            }
        }
    }

    int search(dat moi)
    {
        for (int j = moi.level; j >= 0; j--)
        {
            dat moihon(moi.identifier, j);
            long long key = moihon.key;
            int i = 0;
            while (i != m)
            {
                int a = hp(key, m, c1, c2, i);
                if (status[a] == NON_EMPTY && dulieu[a].key == key)
                {
                    return a;
                }
                i++;
            }
        }
        return -1;
    }

    int search(dat moi, int &sokhe)
    {
        int sokhebandau = sokhe;
        for (int j = moi.level; j >= 0; j--)
        {
            dat moihon(moi.identifier, j);
            long long key = moihon.key;
            int i = 0;
            while (i != m)
            {
                int a = hp(key, m, c1, c2, i);
                if (status[a] == NON_EMPTY && dulieu[a].key == key)
                {
                    return a;
                }
                sokhe++;
                i++;
            }
            sokhe = sokhebandau;
        }
        return -1;
    }

    void dump()
    {
        string kq = "";
        int dem = 0;
        for (int i = 0; i < this->m; i++)
        {
            if (this->status[i] == NON_EMPTY)
            {
                dem++;
            }
        }
        int dangdem = 1;
        for (int i = 0; i < this->m; i++)
        {
            if (this->status[i] == NON_EMPTY)
            {
                if (dangdem != dem)
                    kq += to_string(i) + " " + this->dulieu[i].identifier + "//" + to_string(dulieu[i].level) + ";";
                else
                    kq += to_string(i) + " " + this->dulieu[i].identifier + "//" + to_string(dulieu[i].level);
                dangdem++;
            }
        }
        if (kq != "")
            cout << kq << endl;
    }
};
int LINEAR(long long key, int m, int c1, int c2, int i)
{
    int h1 = key % m;
    int h2 = c1 * i;
    return (h1 + h2) % m;
}
int QUADRATIC(long long key, int m, int c1, int c2, int i)
{
    int h1 = key % m;
    int h2 = c1 * i + c2 * i * i;
    return (h1 + h2) % m;
}
int DOUBLE(long long key, int m, int c1, int c2, int i)
{
    int h1 = key % m;
    int h2 = 1 + key % (m - 2);
    return (h1 + c1 * i * h2) % m;
}
struct donglenh
{
    string ma = "";
    string thunhat = "";
    string thuhai = "";
    int bac = 0;
};

string laydendaungoac(string a)
{
    string kq = "";
    for (unsigned int i = 0; i < a.size(); i++)
    {
        if (a[i] == '(')
        {
            string kq = a.substr(0, i);
            return kq;
        }
    }
    return a;
}

int dodai(string a)
{
    int dem = 0;
    if (a.size() == 0)
        return 0;
    for (unsigned int i = 0; i < a.size(); i++)
    {
        if (a[i] == ',')
            dem++;
    }
    return dem + 1;
}

string lamgon(string a)
{
    int batdau = 0;
    int ketthuc = 0;

    for (unsigned int i = 0; i < a.size(); i++)
    {
        if (a[i] == '(')
            batdau = i + 1;
        if (a[i] == ')')
            ketthuc = i;
    }
    return a.substr(batdau, ketthuc - batdau);
}

string laydendauphay(string &a)
{
    for (unsigned int i = 0; i < a.size(); i++)
    {
        if (a[i] == ',')
        {
            string kq = a.substr(0, i);
            a = a.substr(i + 1);
            return kq;
        }
    }

    string kq = a;
    a = "";
    return kq;
}

bool testid(string name)
{
    if (isstring(name) || isParam(name))
        return false;
    for (int i = 0; i < (int)name.size(); i++)
    {
        if (name[i] == '(')
            return false;
    }
    return true;
}
void SymbolTable::run(string filename)
{
    ifstream myfile0(filename);
    int sodong = 0;
    int loi = -1;
    string dongloi = "";

    string line;
    while (getline(myfile0, line))
    {
        sodong++;
    }
    myfile0.close();

    ifstream myfile(filename);
    donglenh *dong = new donglenh[sodong];

    int giatrim = 0;
    int giatric1 = 0;
    int giatric2 = 0;
    int (*ham)(long long, int, int, int, int);

    myfile >> dong[0].ma;
    if (dong[0].ma == "LINEAR")
    {
        ham = LINEAR;
        myfile >> giatrim;
        myfile >> giatric1;
        giatric2 = 0;
    }
    else if (dong[0].ma == "QUADRATIC")
    {
        ham = QUADRATIC;
        myfile >> giatrim;
        myfile >> giatric1;
        myfile >> giatric2;
    }
    else if (dong[0].ma == "DOUBLE")
    {
        ham = DOUBLE;
        myfile >> giatrim;
        myfile >> giatric1;
        giatric2 = 0;
    }

    OpenHash giatri(ham, giatrim, giatric1, giatric2);

    for (int i = 1; i < sodong; i++)
    {
        myfile >> dong[i].ma;
        if (dong[i].ma == "INSERT")
        {
            string line1 = "";
            myfile.ignore(1, '\n');
            getline(myfile, line1);

            int co2so = -1;
            for (int j = 0; j < (int)line1.size(); j++)
            {
                if (line1[j] == ' ')
                    co2so = j;
            }

            if (co2so != -1)
            {
                dong[i].thunhat = line1.substr(0, co2so);
                dong[i].thuhai = line1.substr(co2so + 1);
            }
            else
            {
                dong[i].thunhat = line1;
            }

            int bacdangtim = dong[i].bac;
            for (int j = i - 1; j >= 0; j--)
            {
                if (dong[i].thunhat == dong[j].thunhat)
                {
                    if (dong[j].ma == "INSERT")
                        if (dong[i].bac == dong[j].bac)
                        {
                            loi = 4;
                            dongloi = dong[i].thunhat;
                            break;
                        }
                }
                if (dong[j].bac < bacdangtim)
                    break;
            }

            if (loi != -1)
                break;

            if (dong[i].thuhai != "" && dong[i].bac > 0)
            {
                loi = 7;
                dongloi = dong[i].thunhat;
                break;
            }

            dat moi(dong[i].thunhat, dong[i].bac, dong[i].thuhai);
            int test = giatri.insert(moi);

            if (test != -1)
                cout << test << endl;
            else
            {
                loi = 8;
                dongloi = dong[i].ma + " " + dong[i].thunhat + " " + dong[i].thuhai;
                break;
            }
        }
        else if (dong[i].ma == "ASSIGN")
        {
            myfile >> dong[i].thunhat;

            string line1;
            myfile.ignore(1, '\n');
            getline(myfile, line1);
            dong[i].thuhai = line1;

            int sokhe = 0;
            if (dong[i].thuhai != "string" && dong[i].thuhai != "number")
            {
                if (testid(dong[i].thuhai))
                {
                    dat bientim(dong[i].thuhai, dong[i].bac);
                    if (giatri.search(bientim) == -1)
                    {
                        loi = 3;
                        dongloi = dong[i].thuhai;
                        break;
                    }
                }
                else
                {
                    string gan = laydendaungoac(dong[i].thuhai);
                    dat biengan(gan, dong[i].bac);
                    if (giatri.dulieu[giatri.search(biengan)].sokieuham == 1)
                    {
                        loi = 2;
                        dongloi = dong[i].ma + " " + dong[i].thunhat + " " + dong[i].thuhai;
                        break;
                    }
                    string bien = lamgon(dong[i].thuhai);
                    int m = dodai(bien);

                    for (int j = 0; j < m; j++)
                    {
                        string ane = laydendauphay(bien);
                        dat bientim(ane, dong[i].bac);

                        if (!isstring(ane) && !isParam(ane))
                            if (giatri.search(bientim) == -1)
                            {
                                loi = 3;
                                dongloi = ane;
                                break;
                            }
                    }
                    if (loi != -1)
                        break;
                }
            }

            if (isstring(dong[i].thuhai))
            {
                dat bientim(dong[i].thunhat, dong[i].bac);
                if (giatri.search(bientim) == -1)
                {
                    loi = 3;
                    dongloi = dong[i].thunhat;
                    break;
                }

                if (giatri.dulieu[giatri.search(bientim)].sokieuham == 1)
                {
                    if (giatri.dulieu[giatri.search(bientim)].type[0] == "string")
                    {
                        int tim = giatri.search(bientim, sokhe);
                        sokhe += tim;
                        sokhe -= tim;
                        // cout << "dacokieu";
                    }
                    else if (giatri.dulieu[giatri.search(bientim)].type[0] == "")
                    {
                        int tim = giatri.search(bientim, sokhe);
                        sokhe += tim;
                        sokhe -= tim;
                        giatri.dulieu[giatri.search(bientim)].type[0] = "string";
                        // cout << "moigankieu";
                    }
                    else
                    {
                        loi = 2;
                        dongloi = dong[i].ma + " " + dong[i].thunhat + " " + dong[i].thuhai;
                        break;
                    }
                }
                else
                {
                    loi = 2;
                    dongloi = dong[i].ma + " " + dong[i].thunhat + " " + dong[i].thuhai;
                    break;
                }
            }
            else if (isParam(dong[i].thuhai))
            {
                dat bientim(dong[i].thunhat, dong[i].bac);
                if (giatri.search(bientim) == -1)
                {
                    loi = 3;
                    dongloi = dong[i].thunhat;
                    break;
                }
                if (giatri.dulieu[giatri.search(bientim)].sokieuham == 1)
                {
                    if (giatri.dulieu[giatri.search(bientim)].type[0] == "number")
                    {
                        int tim = giatri.search(bientim, sokhe);
                        sokhe += tim;
                        sokhe -= tim;
                        // cout << "dacokieu";
                    }
                    else if (giatri.dulieu[giatri.search(bientim)].type[0] == "")
                    {
                        int tim = giatri.search(bientim, sokhe);
                        sokhe += tim;
                        sokhe -= tim;
                        giatri.dulieu[giatri.search(bientim)].type[0] = "number";
                        // cout << "moigankieu";
                    }
                    else
                    {
                        loi = 2;
                        dongloi = dong[i].ma + " " + dong[i].thunhat + " " + dong[i].thuhai;
                        break;
                    }
                }
                else
                {
                    loi = 2;
                    dongloi = dong[i].ma + " " + dong[i].thunhat + " " + dong[i].thuhai;
                    break;
                }
            }
            else
            {
                string gan = laydendaungoac(dong[i].thuhai);
                dat biengan(gan, dong[i].bac);

                if (testid(dong[i].thuhai))
                {
                    dat ganvao(dong[i].thunhat, dong[i].bac);
                    if (giatri.search(ganvao) == -1)
                    {
                        loi = 3;
                        dongloi = dong[i].thunhat;
                        break;
                    }
                    if (giatri.dulieu[giatri.search(biengan)].sokieuham != 1)
                    {
                        loi = 2;
                        dongloi = dong[i].ma + " " + dong[i].thunhat + " " + dong[i].thuhai;
                        break;
                    }
                    if (giatri.dulieu[giatri.search(ganvao)].sokieuham != 1)
                    {
                        loi = 2;
                        dongloi = dong[i].ma + " " + dong[i].thunhat + " " + dong[i].thuhai;
                        break;
                    }

                    if (giatri.dulieu[giatri.search(ganvao)].type[0] == giatri.dulieu[giatri.search(biengan)].type[0] && giatri.dulieu[giatri.search(ganvao)].type[0] != "")
                    {
                        int tim = giatri.search(ganvao, sokhe);
                        sokhe += tim;
                        sokhe -= tim;
                        // cout << "dung";
                    }
                    else if (giatri.dulieu[giatri.search(ganvao)].type[0] == "" && giatri.dulieu[giatri.search(biengan)].type[0] != "")
                    {
                        int tim = giatri.search(ganvao, sokhe);
                        sokhe += tim;
                        sokhe -= tim;
                        // cout << "moigankieu";
                        giatri.dulieu[giatri.search(ganvao)].type[0] = giatri.dulieu[giatri.search(biengan)].type[0];
                    }
                    else if (giatri.dulieu[giatri.search(ganvao)].type[0] != "" && giatri.dulieu[giatri.search(biengan)].type[giatri.dulieu[giatri.search(biengan)].sokieuham - 1] == "")
                    {
                        int tim = giatri.search(ganvao, sokhe);
                        sokhe += tim;
                        sokhe -= tim;
                        // cout << "moigankieu";
                        giatri.dulieu[giatri.search(biengan)].type[0] = giatri.dulieu[giatri.search(ganvao)].type[0];
                    }
                    else
                    {
                        loi = 9;
                        dongloi = dong[i].ma + " " + dong[i].thunhat + " " + dong[i].thuhai;
                        break;
                    }
                }
                else
                {
                    string gan = laydendaungoac(dong[i].thuhai);
                    string bien = lamgon(dong[i].thuhai);
                    dat biengan(gan, dong[i].bac);
                    int m = dodai(bien);

                    int tim = giatri.search(gan, sokhe);
                    sokhe += tim;
                    sokhe -= tim;

                    for (int j = 1; j <= m; j++)
                    {
                        string ane = laydendauphay(bien);
                        dat bientim(ane, dong[i].bac);

                        if (isParam(ane))
                        {
                            if (giatri.dulieu[giatri.search(biengan)].type[j] == "number")
                            {
                                // cout << "dung";
                            }
                            else if (giatri.dulieu[giatri.search(biengan)].type[j] == "")
                            {
                                giatri.dulieu[giatri.search(biengan)].type[j] = "number";
                                // cout << "moigankieu";
                            }
                            else
                            {
                                loi = 2;
                                dongloi = dong[i].ma + " " + dong[i].thunhat + " " + dong[i].thuhai;
                                break;
                            }
                        }
                        else if (isstring(ane))
                        {
                            if (giatri.dulieu[giatri.search(biengan)].type[j] == "string")
                            {
                                // cout << "dung";
                            }
                            else if (giatri.dulieu[giatri.search(biengan)].type[j] == "")
                            {
                                giatri.dulieu[giatri.search(biengan)].type[j] = "string";
                                // cout << "moigankieu";
                            }
                            else
                            {
                                loi = 2;
                                dongloi = dong[i].ma + " " + dong[i].thunhat + " " + dong[i].thuhai;
                                break;
                            }
                        }
                        else
                        {
                            if (giatri.dulieu[giatri.search(bientim)].sokieuham != 1)
                            {
                                loi = 2;
                                dongloi = dong[i].ma + " " + dong[i].thunhat + " " + dong[i].thuhai;
                                break;
                            }
                            else if (giatri.dulieu[giatri.search(bientim)].type[0] == giatri.dulieu[giatri.search(biengan)].type[j] && giatri.dulieu[giatri.search(bientim)].type[0] != "")
                            {
                                int tim = giatri.search(bientim, sokhe);
                                sokhe += tim;
                                sokhe -= tim;
                                // cout << "dung";
                            }
                            else if (giatri.dulieu[giatri.search(bientim)].type[0] == "" && giatri.dulieu[giatri.search(biengan)].type[j] != "")
                            {
                                int tim = giatri.search(bientim, sokhe);
                                sokhe += tim;
                                sokhe -= tim;
                                // cout << "moigankieu";
                                giatri.dulieu[giatri.search(bientim)].type[0] = giatri.dulieu[giatri.search(biengan)].type[j];
                            }
                            else if (giatri.dulieu[giatri.search(bientim)].type[0] != "" && giatri.dulieu[giatri.search(biengan)].type[j] == "")
                            {
                                int tim = giatri.search(bientim, sokhe);
                                sokhe += tim;
                                sokhe -= tim;
                                // cout << "moigankieu";
                                giatri.dulieu[giatri.search(biengan)].type[j] = giatri.dulieu[giatri.search(bientim)].type[0];
                            }
                            else if (giatri.dulieu[giatri.search(bientim)].type[0] != giatri.dulieu[giatri.search(biengan)].type[j])
                            {
                                loi = 2;
                                dongloi = dong[i].ma + " " + dong[i].thunhat + " " + dong[i].thuhai;
                                break;
                            }
                            else
                            {
                                loi = 9;
                                dongloi = dong[i].ma + " " + dong[i].thunhat + " " + dong[i].thuhai;
                                break;
                            }
                        }
                    }
                }
                if (loi != -1)
                    break;
                dat ganvao(dong[i].thunhat, dong[i].bac);
                if (giatri.search(ganvao) == -1)
                {
                    loi = 3;
                    dongloi = dong[i].thunhat;
                    break;
                }

                if (giatri.dulieu[giatri.search(ganvao)].sokieuham != 1)
                {
                    loi = 2;
                    dongloi = dong[i].ma + " " + dong[i].thunhat + " " + dong[i].thuhai;
                    break;
                }
                else if (giatri.dulieu[giatri.search(ganvao)].type[0] == giatri.dulieu[giatri.search(biengan)].type[0] && giatri.dulieu[giatri.search(ganvao)].type[0] != "")
                {
                    int tim = giatri.search(ganvao, sokhe);
                    sokhe += tim;
                    sokhe -= tim;
                    // cout << "dung";
                }
                else if (giatri.dulieu[giatri.search(ganvao)].type[0] == "" && giatri.dulieu[giatri.search(biengan)].type[0] != "")
                {
                    int tim = giatri.search(ganvao, sokhe);
                    sokhe += tim;
                    sokhe -= tim;
                    // cout << "moigankieu";
                    giatri.dulieu[giatri.search(ganvao)].type[0] = giatri.dulieu[giatri.search(biengan)].type[0];
                }
                else if (giatri.dulieu[giatri.search(ganvao)].type[0] != "" && giatri.dulieu[giatri.search(biengan)].type[0] == "")
                {
                    int tim = giatri.search(ganvao, sokhe);
                    sokhe += tim;
                    sokhe -= tim;
                    // cout << "moigankieu";
                    giatri.dulieu[giatri.search(biengan)].type[0] = giatri.dulieu[giatri.search(ganvao)].type[0];
                }
                else if (giatri.dulieu[giatri.search(ganvao)].type[0] != giatri.dulieu[giatri.search(biengan)].type[0])
                {
                    loi = 2;
                    dongloi = dong[i].ma + " " + dong[i].thunhat + " " + dong[i].thuhai;
                    break;
                }
                else
                {
                    loi = 9;
                    dongloi = dong[i].ma + " " + dong[i].thunhat + " " + dong[i].thuhai;
                    break;
                }
            }
            cout << sokhe << endl;
        }
        else if (dong[i].ma == "CALL")
        {
            string line1;
            myfile.ignore(1, '\n');
            getline(myfile, line1);
            dong[i].thuhai = line1;

            int sokhe = 0;
            dong[i].thunhat = laydendaungoac(dong[i].thuhai);

            if (testid(dong[i].thuhai))
            {
                loi = 1;
                dongloi = dong[i].ma + " " + dong[i].thuhai;
                break;
            }

            string gan = laydendaungoac(dong[i].thuhai);
            string bien = lamgon(dong[i].thuhai);
            int m = dodai(bien);

            for (int j = 0; j < m; j++)
            {
                string ane = laydendauphay(bien);
                dat bientim(ane, dong[i].bac);

                if (!isstring(ane) && !isParam(ane))
                    if (giatri.search(bientim) == -1)
                    {
                        loi = 3;
                        dongloi = ane;
                        break;
                    }
            }
            if (loi != -1)
                break;

            dat biendangtim(dong[i].thunhat, dong[i].bac);
            if (giatri.search(biendangtim) == -1)
            {
                loi = 3;
                dongloi = dong[i].thunhat;
                break;
            }

            if (isstring(dong[i].thuhai))
            {
            }
            else if (isParam(dong[i].thuhai))
            {
            }
            else
            {
                string gan = laydendaungoac(dong[i].thuhai);
                string bien = lamgon(dong[i].thuhai);
                dat biengan(gan, dong[i].bac);
                int m = dodai(bien);

                int tim = giatri.search(gan, sokhe);
                sokhe += tim;
                sokhe -= tim;

                if (giatri.dulieu[giatri.search(biengan)].sokieuham == 1)
                {
                    loi = 2;
                    dongloi = dong[i].ma + " " + dong[i].thuhai;
                    break;
                }

                for (int j = 1; j <= m; j++)
                {
                    string ane = laydendauphay(bien);
                    dat bientim(ane, dong[i].bac);

                    if (isParam(ane))
                    {
                        if (giatri.dulieu[giatri.search(biengan)].type[j] == "number")
                        {
                            // cout << "dung";
                        }
                        else if (giatri.dulieu[giatri.search(biengan)].type[j] == "")
                        {
                            giatri.dulieu[giatri.search(biengan)].type[j] = "number";
                            // cout << "moigankieu";
                        }
                        else
                        {
                            loi = 2;
                            dongloi = dong[i].ma + " " + dong[i].thuhai;
                            break;
                        }
                    }
                    else if (isstring(ane))
                    {
                        if (giatri.dulieu[giatri.search(biengan)].type[j] == "string")
                        {
                            // cout << "dung";
                        }
                        else if (giatri.dulieu[giatri.search(biengan)].type[j] == "")
                        {
                            giatri.dulieu[giatri.search(biengan)].type[j] = "string";
                            // cout << "moigankieu";
                        }
                        else
                        {
                            loi = 2;
                            dongloi = dong[i].ma + " " + dong[i].thuhai;
                            break;
                        }
                    }
                    else
                    {
                        if (giatri.dulieu[giatri.search(bientim)].sokieuham != 1)
                        {
                            loi = 2;
                            dongloi = dong[i].ma + " " + dong[i].thuhai;
                            break;
                        }
                        else if (giatri.dulieu[giatri.search(bientim)].type[0] == giatri.dulieu[giatri.search(biengan)].type[j] && giatri.dulieu[giatri.search(bientim)].type[0] != "")
                        {
                            int tim = giatri.search(bientim, sokhe);
                            sokhe += tim;
                            sokhe -= tim;
                            // cout << "dung";
                        }
                        else if (giatri.dulieu[giatri.search(bientim)].type[0] == "" && giatri.dulieu[giatri.search(biengan)].type[j] != "")
                        {
                            int tim = giatri.search(bientim, sokhe);
                            sokhe += tim;
                            sokhe -= tim;
                            // cout << "moigankieu";
                            giatri.dulieu[giatri.search(bientim)].type[0] = giatri.dulieu[giatri.search(biengan)].type[j];
                        }
                        else if (giatri.dulieu[giatri.search(bientim)].type[0] != "" && giatri.dulieu[giatri.search(biengan)].type[j] == "")
                        {
                            int tim = giatri.search(bientim, sokhe);
                            sokhe += tim;
                            sokhe -= tim;
                            // cout << "moigankieu";
                            giatri.dulieu[giatri.search(biengan)].type[j] = giatri.dulieu[giatri.search(bientim)].type[0];
                        }
                        else if (giatri.dulieu[giatri.search(bientim)].type[0] != giatri.dulieu[giatri.search(biengan)].type[j])
                        {
                            loi = 2;
                            dongloi = dong[i].ma + " " + dong[i].thuhai;
                            break;
                        }
                        else
                        {
                            loi = 9;
                            dongloi = dong[i].ma + " " + dong[i].thuhai;
                            break;
                        }
                    }
                }
                if (loi != -1)
                    break;
            }
            cout << sokhe << endl;
        }
        else if (dong[i].ma == "BEGIN")
        {
            for (int j = i; j < sodong; j++)
            {
                dong[j].bac++;
            }
        }
        else if (dong[i].ma == "END")
        {
            if (dong[i].bac <= 0)
            {
                dongloi = dong[i].ma;
                loi = 6;
                break;
            }

            for (int j = i; j >= 0 && dong[j].bac >= dong[i].bac; j--)
            {
                if (dong[j].ma == "INSERT")
                {
                    dat moi(dong[j].thunhat, dong[j].bac);
                    if (moi.level >= dong[i].bac)
                        giatri.remove(moi);
                }
            }
            for (int j = i; j < sodong; j++)
            {
                dong[j].bac--;
            }
        }
        else if (dong[i].ma == "LOOKUP")
        {
            myfile >> dong[i].thunhat;

            dat moi(dong[i].thunhat, dong[i].bac);
            if (giatri.search(moi) != -1)
            {
                cout << giatri.search(moi) << endl;
            }
            else
            {
                loi = 3;
                dongloi = dong[i].thunhat;
                break;
            }
        }
        else if (dong[i].ma == "PRINT")
        {
            giatri.dump();
        }
        else
        {
            string a = "";
            getline(myfile, a);
            loi = 1;
            dongloi = dong[i].ma + a;
            break;
        }
        if (i == sodong - 1 && dong[i].bac > 0)
        {
            loi = 5;
            dongloi = to_string(dong[i].bac);
            break;
        }
    }

    delete[] dong;
    myfile.close();
    for (int i = 0; i < giatri.m; i++)
    {
        // if (giatri.dulieu[i].sokieuham > 100)
        // delete[] giatri.dulieu[i].type;
    }

    if (loi == 1)
    {
        throw InvalidInstruction(dongloi);
    }
    else if (loi == 2)
    {
        throw TypeMismatch(dongloi);
    }
    else if (loi == 3)
    {
        throw Undeclared(dongloi);
    }
    else if (loi == 4)
    {
        throw Redeclared(dongloi);
    }
    else if (loi == 5)
    {
        throw UnclosedBlock(dongloi[0] - '0');
    }
    else if (loi == 6)
    {
        throw UnknownBlock();
    }
    else if (loi == 7)
    {
        throw InvalidDeclaration(dongloi);
    }
    else if (loi == 8)
    {
        throw Overflow(dongloi);
    }
    else if (loi == 9)
    {
        throw TypeCannotBeInferred(dongloi);
    }
}
