#include "SymbolTable.h"
struct kieutestidentifier_name
{
    string tenlenh = "";
    string identifier_name = "";
    string type = "";
    int bac = 0;
};
bool isParam(string name)
{
    int dai = name.size();
    if(dai == 0) return 0;
    for(int i = 0; i < dai ; i++)
    {
        if(!(name[i] <= '9' && name[i] >= '0')) return 0;
    }
    return 1;
}
bool isstring(string name)
{
    int dai = name.size();
    if(dai < 2) return 0;
    if(name[0] == (char)39 && name[dai -1] == (char)39)
    {
        for(int i = 1 ; i < dai - 1 ; i++)
        {
            if(!((name[i] <= 'z' && name[i] >= 'a')|| (name[i] <= 'Z' 
            && name[i] >= 'A' )|| (name[i] <= '9' && name[i] >= '0')
            || name[i] == ' ')) return 0;
        }
        return 1;
    }
    return 0;
}

void SymbolTable::run(string filename) {
    ifstream infile(filename);
    int sodong = 0;
    int loi = -1;
    string dongloi = "";

    string line;
    while (getline(infile, line))
    {
        sodong++;
    }
    infile.close();
    
    ifstream infile2(filename);
    kieutestidentifier_name* dong = new(nothrow) kieutestidentifier_name[sodong];
    
    ifstream infile1(filename);
    
    for(int i = 0 ; i < sodong; i++)
    {
        infile1 >> dong[i].tenlenh;
        
        if(dong[i].tenlenh == "INSERT") 
        {
            infile1 >> dong[i].identifier_name;
            infile1 >> dong[i].type;
            int bacdangtim = dong[i].bac;
            
            for(int j = i - 1 ; j >= 0; j--)
            {
                if(dong[i].identifier_name == dong[j].identifier_name) 
                {
                    if(dong[j].tenlenh == "INSERT")
                    if(dong[i].bac == dong[j].bac)
                    {
                        loi = 4;
                        dongloi = dong[i].tenlenh + " " + dong[i].identifier_name + " " + dong[i].type;
                        break;
                    }
                }
                if(dong[j].bac < bacdangtim) break;
            }
            if(loi != -1) break;
            cout << "success" << endl;
        } 
        else if(dong[i].tenlenh == "ASSIGN") 
        {
            infile1 >> dong[i].identifier_name;
            
            string line1;
            infile1.ignore(1,'\n');
            getline(infile1, line1);
            dong[i].type = line1;
            
            int dem = 0;
            int bacdangtim = dong[i].bac;
            
            for(int j = i - 1 ; j >= 0; j--)
            {
                if(dong[i].identifier_name == dong[j].identifier_name && dong[j].bac <= bacdangtim)
                {
                    if(dong[i].identifier_name == dong[j].identifier_name)
                    {
                        dem = 1;
                        break;
                    }
                }    
                if(dong[j].bac < bacdangtim) bacdangtim = dong[j].bac;
            }
            if(!dem)
            {
                loi = 3;
                dongloi = dong[i].tenlenh + " " + dong[i].identifier_name + " " + dong[i].type;
                break;
            }
            
            bacdangtim = dong[i].bac;
            int kq = 0;
            for(int j = i - 1 ; j >= 0; j--)
            {
                if(dong[i].identifier_name == dong[j].identifier_name && dong[j].bac <= bacdangtim && dong[j].tenlenh == "INSERT")
                {
                    if(isParam(dong[i].type))
                    {
                        if(dong[j].type == "number")
                        {
                            kq = 1;
                            break;
                        } 
                        else
                        {
                            kq = 0;
                            break;
                        }
                    }
                    else if(isstring(dong[i].type))
                    {
                        if(dong[j].type == "string")
                        {
                            kq = 1;
                            break;
                        }
                        else
                        {
                            kq = 0;
                            break;
                        }
                    }
                    else 
                    {
                        int bacdangtimne = dong[i].bac;
                        for(int k = i - 1 ; k >= 0 ; k--)
                        { 
                            if(dong[i].type == dong[k].identifier_name && dong[k].bac <= bacdangtimne ) 
                            { 
                                if((dong[k].type == "number" && dong[j].type == "number") || (dong[k].type == "string" && dong[j].type == "string"))
                                {
                                    kq = 1;
                                    break;
                                }
                            }
                            if(dong[k].bac < bacdangtimne) bacdangtimne = dong[k].bac;
                        }
                    }
                }
                bacdangtim = dong[j].bac;
            }
            if(kq) cout << "success" << endl;
            else
            {
                loi = 2;
                dongloi = dong[i].tenlenh + " " + dong[i].identifier_name + " " + dong[i].type;
                break;
            }
        } 
        else if(dong[i].tenlenh == "LOOKUP")
        {
            infile1 >> dong[i].identifier_name;
            int demchua = 0;
            int bacdangtim = dong[i].bac;
            
            for(int j = i - 1 ; j >= 0 ; j--)
            {
                if(dong[i].identifier_name == dong[j].identifier_name && dong[j].tenlenh == "INSERT" && dong[j].bac <= bacdangtim)
                {
                    cout << dong[j].bac << endl;
                    demchua = 1;
                    break;
                }
                if(dong[j].bac < bacdangtim) bacdangtim = dong[j].bac;
            }
            
            if(demchua == 0)
            {
                loi = 3;
                dongloi = dong[i].tenlenh + " " + dong[i].identifier_name;
                break;
            }
        } 
        else if(dong[i].tenlenh == "BEGIN") 
        {
            for(int j = i ; j < sodong ; j++)
            {
                dong[j].bac++;
            }
        } 
        else if(dong[i].tenlenh == "END") 
        {
            if(dong[i].bac <= 0)
            {
                loi = 6;
                break;
            }
            for(int j = i ; j < sodong ; j++)
            {
                dong[j].bac--;
            }
        } 
        else if(dong[i].tenlenh == "PRINT")
        {
            string kq = "";
            int tamvuc = dong[i].bac;
            for(int j = i - 1 ; j >= 0 ; j--)
            {
                if(dong[j].identifier_name != "" && dong[j].tenlenh == "INSERT" && dong[j].bac <= tamvuc)
                {
                    int trung = 0;
                    int tamvucmoi = dong[i].bac;
                    for(int k = i - 1 ; k > j ; k--)
                    {
                        if(dong[j].identifier_name == dong[k].identifier_name && dong[k].tenlenh == "INSERT" && dong[k].bac <= tamvucmoi) trung = 1;
                        if(dong[k].bac < tamvucmoi) tamvucmoi = dong[k].bac;
                    }
                    if(trung) continue;
                    kq = dong[j].identifier_name + "//" + to_string(dong[j].bac) + " " + kq;
                }
                if(dong[j].bac < tamvuc) tamvuc = dong[j].bac;
            }
            if(kq.substr(0, kq.size()-1) != "")
            cout << kq.substr(0, kq.size()-1) << endl;
        }
        else if(dong[i].tenlenh == "RPRINT")
        {
            string kq = "";
            int tamvuc = dong[i].bac;
            for(int j = i - 1 ; j >= 0 ; j--)
            {
                if(dong[j].identifier_name != "" && dong[j].tenlenh == "INSERT" && dong[j].bac <= tamvuc)
                {
                    int trung = 0;
                    int tamvucmoi = dong[i].bac;
                    for(int k = i - 1 ; k > j ; k--)
                    {
                        if(dong[j].identifier_name == dong[k].identifier_name && dong[k].tenlenh == "INSERT" && dong[k].bac <= tamvucmoi) trung = 1;
                        if(dong[k].bac < tamvucmoi) tamvucmoi = dong[k].bac;
                    }
                    if(trung) continue;
                    kq = kq + dong[j].identifier_name + "//" + to_string(dong[j].bac) + " ";
                }
                if(dong[j].bac < tamvuc) tamvuc = dong[j].bac;
            }
            if(kq.substr(0, kq.size()-1) != "")
            cout << kq.substr(0, kq.size()-1) << endl;
        } else
        {
            string a = "";
            getline(infile1, a);
            loi = 1;
            dongloi = dong[i].tenlenh + a;
            break;
        }
        if(i == sodong - 1 && dong[i].bac > 0)
        {
            loi = 5;
            dongloi = to_string(dong[i].bac);
            break;
        }
    }
    delete[] dong;
    infile1.close();
    if(loi == 1)
    {
        throw InvalidInstruction(dongloi);
    } else if(loi == 2)
    {
        throw TypeMismatch(dongloi);
    } else if(loi == 3)
    {
        throw Undeclared(dongloi);
    } else if(loi == 4)
    {
        throw Redeclared(dongloi);
    } else if(loi == 5)
    {
        throw UnclosedBlock(dongloi[0] - '0');
    } else if(loi == 6)
    {
        throw UnknownBlock();
    }
}