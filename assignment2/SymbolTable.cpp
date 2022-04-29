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

struct data
{
    string identifier;
    int level;
    string type;
    data(string identifier = "", int level = 0, string type = "", string dungsai = "")
    {
        this->identifier = identifier;
        this->level = level;
        this->type = type;
        if (dungsai == "true")
            this->level = 0;
    }
    bool operator>(data &b)
    {
        if (this->level > b.level)
            return 1;
        if (this->level < b.level)
            return 0;
        int a = strcmp(this->identifier.c_str(), b.identifier.c_str());
        if (a > 0)
            return 1;
        else
            return 0;
    }
    bool operator<(data &b)
    {
        if (this->level < b.level)
            return 1;
        if (this->level > b.level)
            return 0;
        int a = strcmp(this->identifier.c_str(), b.identifier.c_str());
        if (a < 0)
            return 1;
        else
            return 0;
    }
    bool operator<=(data &b)
    {
        return *this < b || *this == b;
    }
    bool operator==(data &b)
    {
        int a = strcmp(this->identifier.c_str(), b.identifier.c_str());
        if (a == 0 && this->level == b.level)
            return 1;
        else
            return 0;
    }
};
struct Node
{
    data val;     // holds the key
    Node *parent; // pointer to the parent
    Node *left;   // pointer to left child
    Node *right;  // pointer to right child
};
typedef Node *NodePtr;
class SplayTree
{
private:
    // rotate left at node x
    void leftRotate(NodePtr x)
    {
        NodePtr y = x->right;
        x->right = y->left;
        if (y->left != nullptr)
        {
            y->left->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == nullptr)
        {
            this->root = y;
        }
        else if (x == x->parent->left)
        {
            x->parent->left = y;
        }
        else
        {
            x->parent->right = y;
        }
        y->left = x;
        x->parent = y;
    }

    // rotate right at node x
    void rightRotate(NodePtr x)
    {
        NodePtr y = x->left;
        x->left = y->right;
        if (y->right != nullptr)
        {
            y->right->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == nullptr)
        {
            this->root = y;
        }
        else if (x == x->parent->right)
        {
            x->parent->right = y;
        }
        else
        {
            x->parent->left = y;
        }
        y->right = x;
        x->parent = y;
    }

    // splaying
    void splay(NodePtr x, int &xoay)
    {
        int coxoay = 0;
        while (x->parent)
        {
            if (!x->parent->parent)
            {
                if (x == x->parent->left)
                {
                    // zig rotation
                    rightRotate(x->parent);
                    coxoay++;
                }
                else
                {
                    // zag rotation
                    leftRotate(x->parent);
                    coxoay++;
                }
            }
            else if (x == x->parent->left && x->parent == x->parent->parent->left)
            {
                // zig-zig rotation
                rightRotate(x->parent->parent);
                rightRotate(x->parent);
                coxoay++;
            }
            else if (x == x->parent->right && x->parent == x->parent->parent->right)
            {
                // zag-zag rotation
                leftRotate(x->parent->parent);
                leftRotate(x->parent);
                coxoay++;
            }
            else if (x == x->parent->right && x->parent == x->parent->parent->left)
            {
                // zig-zag rotation
                leftRotate(x->parent);
                rightRotate(x->parent);
                coxoay++;
            }
            else
            {
                // zag-zig rotation
                rightRotate(x->parent);
                leftRotate(x->parent);
                coxoay++;
            }
        }
        if (coxoay)
            xoay++;
    }
    void deleteNodeHelper(NodePtr node, data key)
    {
        NodePtr x = nullptr;
        NodePtr t, s;
        while (node != nullptr)
        {
            if (node->val == key)
            {
                x = node;
            }

            if (node->val <= key)
            {
                node = node->right;
            }
            else
            {
                node = node->left;
            }
        }

        if (x == nullptr)
        {
            return;
        }
        split(x, s, t); // split the tree
        if (s->left)
        { // remove x
            s->left->parent = nullptr;
        }
        root = join(s->left, t);
        delete (s);
        s = nullptr;
    }
    NodePtr join(NodePtr s, NodePtr t)
    {
        if (!s)
        {
            return t;
        }

        if (!t)
        {
            return s;
        }
        NodePtr x = maximum(s);
        int a = 0;
        splay(x, a);
        x->right = t;
        t->parent = x;
        return x;
    }
    void split(NodePtr &x, NodePtr &s, NodePtr &t)
    {
        int a = 0;
        splay(x, a);
        if (x->right)
        {
            t = x->right;
            t->parent = nullptr;
        }
        else
        {
            t = nullptr;
        }
        s = x;
        s->right = nullptr;
        x = nullptr;
    }
    NodePtr searchTreeHelper(NodePtr node, data key, int &sosanh)
    {
        if (node == nullptr)
            return node;
        else if (key == node->val)
        {
            sosanh++;
            return node;
        }
        else if (key < node->val)
        {
            sosanh++;
            return searchTreeHelper(node->left, key, sosanh);
        }
        sosanh++;
        return searchTreeHelper(node->right, key, sosanh);
    }

public:
    NodePtr root;
    void deleteBinaryTree(NodePtr &root)
    {
        // Base case: empty tree
        if (root == nullptr) {
            return;
        }
    
        // delete left and right subtree first (Postorder)
        deleteBinaryTree(root->left);
        deleteBinaryTree(root->right);
    
        // delete the current node after deleting its left and right subtree
        delete root;
    
        // root as null before returning
        root = nullptr;
    }
    void deleteBinaryTree()
    {
        deleteBinaryTree(root);
    }
    SplayTree()
    {
        root = nullptr;
    }
    NodePtr searchTree(data k, int &sosanh, int &xoay)
    {
        NodePtr x = searchTreeHelper(this->root, k, sosanh);
        if (x)
        {
            splay(x, xoay);
        }
        return x;
    }

    NodePtr maximum(NodePtr node)
    {
        while (node->right != nullptr)
        {
            node = node->right;
        }
        return node;
    }
    void deleteNode(data val)
    {
        deleteNodeHelper(this->root, val);
    }
    void insert(data key, int &sosanh, int &xoay)
    {
        // normal BST insert

        NodePtr node = new Node;
        node->parent = nullptr;
        node->left = nullptr;
        node->right = nullptr;
        node->val = key;
        NodePtr y = nullptr;
        NodePtr x = this->root;

        while (x != nullptr)
        {
            y = x;
            if (node->val < x->val)
            {
                sosanh++;
                x = x->left;
            }
            else
            {
                sosanh++;
                x = x->right;
            }
        }

        // y is parent of x

        node->parent = y;
        if (y == nullptr)
        {
            root = node;
        }
        else if (node->val < y->val)
        {
            y->left = node;
        }
        else
        {
            y->right = node;
        }

        // splay the node
        splay(node, xoay);
    }
    void in(Node *root, string &a)
    {
        if (root)
        {
            a += root->val.identifier + "//" + to_string(root->val.level) + " ";
            in(root->left, a);
            in(root->right, a);
        }
    }

    void in()
    {
        string kq = "";
        in(this->root, kq);
        if (kq.substr(0, kq.size() - 1) != "")
            cout << kq.substr(0, kq.size() - 1) << endl;
    }
};
struct donglenh
{
    string ma = "";
    string thunhat = "";
    string thuhai = "";
    string thuba = "";
    int bac = 0;
};
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
void SymbolTable::run(string filename)
{
    ifstream myfile1(filename);
    int sodong = 0;
    int loi = -1;
    string line;
    while (getline(myfile1, line))
    {
        sodong++;
    }

    ifstream myfile(filename);
    donglenh *dong = new donglenh[sodong];
    string dongloi = "";
    SplayTree bst;
    int i = 0;
    for (i = 0; i < sodong; i++)
    {
        myfile >> dong[i].ma;
        if (dong[i].ma == "INSERT")
        {
            int sosanh = 0;
            int xoay = 0;

            myfile >> dong[i].thunhat;
            myfile >> dong[i].thuhai;
            myfile >> dong[i].thuba;

            data moi(dong[i].thunhat, dong[i].bac, dong[i].thuhai, dong[i].thuba);
            if (moi.level > 0 && strstr(dong[i].thuhai.c_str(), "->"))
            {
                loi = 7;
                dongloi = dong[i].ma + " " + dong[i].thunhat + " " + dong[i].thuhai + " " + dong[i].thuba;
                break;
            }
            int sosanhthu = 0;
            int xoaythu = 0;
            if (bst.searchTree(moi, sosanhthu, xoaythu))
            {
                loi = 4;
                dongloi = dong[i].ma + " " + dong[i].thunhat + " " + dong[i].thuhai + " " + dong[i].thuba;
                break;
            }

            if (loi != -1)
                break;
            if (bst.root == nullptr)
                bst.insert(moi, sosanh, xoay);
            else
                bst.insert(moi, sosanh, xoay);
            cout << sosanh << " " << xoay << endl;
        }
        else if (dong[i].ma == "ASSIGN")
        {
            myfile >> dong[i].thunhat;

            string line1 = "";
            myfile.ignore(1, '\n');
            getline(myfile, line1);
            dong[i].thuhai = line1;

            int xoay = 0;
            int sosanh = 0;
            int dem = 0;
            int bacdangtim = dong[i].bac;
            int kq1 = -1;

            for (int j = i - 1; j >= 0; j--)
            {
                if (!isParam(dong[i].thuhai) && !isstring(dong[i].thuhai))
                {
                    int bacdangtimne = dong[i].bac;
                    for (int k = i - 1; k >= 0; k--)
                    {
                        if (dong[i].thuhai == dong[k].thunhat && dong[k].ma == "INSERT" && (dong[k].bac <= bacdangtimne || dong[k].thuba == "true"))
                        {
                        }
                        else if ((laydendaungoac(dong[i].thuhai) == dong[k].thunhat) && dong[k].ma == "INSERT" && dong[k].thunhat != "" && (dong[k].bac <= bacdangtimne || dong[k].thuba == "true"))
                        {
                            string gan = "";
                            string bandau = "";

                            gan = strchr(dong[i].thuhai.c_str(), '(');
                            bandau = strchr(dong[k].thuhai.c_str(), '(');

                            gan = lamgon(gan);
                            bandau = lamgon(bandau);
                            int dodaim = dodai(bandau);
                            int testtrongngoac = 1;

                            for (int m = 0; m < dodaim; m++)
                            {
                                int bacdangtimnene = dong[i].bac;
                                string ane = laydendauphay(bandau);
                                string bne = laydendauphay(gan);

                                if (!isParam(bne) && !isstring(bne))
                                {
                                    int coten = 0;
                                    for (int n = i - 1; n >= 0; n--)
                                    {
                                        if (dong[n].thunhat == bne && (dong[n].bac <= bacdangtimnene || dong[n].thuba == "true") && dong[n].ma == "INSERT")
                                        {
                                            coten++;
                                        }
                                        if (dong[n].bac <= bacdangtimnene)
                                            bacdangtimnene = dong[n].bac;
                                    }

                                    if (coten == 0)
                                    {
                                        goto loi3ne;
                                    }
                                    bacdangtimnene = dong[i].bac;
                                    for (int n = i - 1; n >= 0; n--)
                                    {
                                        if (dong[n].thunhat == bne && (dong[n].bac <= bacdangtimnene || dong[n].thuba == "true") && dong[n].ma == "INSERT")
                                        {
                                            if (dong[n].thuhai == "number" && !(ane == "number"))
                                            {
                                                testtrongngoac = 0;
                                                break;
                                            }
                                            else if (dong[n].thuhai == "string" && !(ane == "string"))
                                            {
                                                testtrongngoac = 0;
                                                break;
                                            }
                                            else if (dong[n].thuhai != "string" && dong[n].thuhai != "number")
                                            {
                                                testtrongngoac = 0;
                                                break;
                                            }
                                        }
                                        if (dong[n].bac <= bacdangtimnene)
                                            bacdangtimnene = dong[n].bac;
                                    }
                                    if (testtrongngoac == 0)
                                        break;
                                }
                                else if (ane == "number" && !isParam(bne))
                                {
                                    testtrongngoac = 0;
                                    break;
                                }
                                else if (ane == "string" && !isstring(bne))
                                {
                                    testtrongngoac = 0;
                                    break;
                                }
                            }
                            if (dodai(gan) == dodai(bandau))
                            {
                                if (testtrongngoac)
                                {
                                    kq1 = 1;
                                    break;
                                }
                                else
                                {
                                    kq1 = 0;
                                    break;
                                }
                            }
                            else
                            {
                                kq1 = 0;
                                break;
                            }
                        }
                        if (dong[k].bac < bacdangtimne)
                            bacdangtimne = dong[k].bac;
                    }
                    if (kq1)
                        break;
                }
                bacdangtim = dong[j].bac;
            }
            if (kq1 == 0)
            {
                dongloi = dong[i].ma + " " + dong[i].thunhat + " " + dong[i].thuhai;
                loi = 2;
                break;
            }
            bacdangtim = dong[i].bac;
            for (int j = i - 1; j >= 0; j--)
            {
                if (dong[i].thunhat == dong[j].thunhat && (dong[j].bac <= bacdangtim || dong[j].thuba == "true") && dong[j].ma == "INSERT")
                {
                    dem = 1;
                    break;
                }
                if (dong[j].bac < bacdangtim)
                    bacdangtim = dong[j].bac;
            }
            if (!dem)
            {
            loi3ne:
                dongloi = dong[i].ma + " " + dong[i].thunhat + " " + dong[i].thuhai;
                loi = 3;
                break;
            }
            dem = 0;
            bacdangtim = dong[i].bac;
            for (int j = i - 1; j >= 0; j--)
            {
                data moi(dong[j].thunhat, dong[j].bac, dong[j].thuhai, dong[j].thuba);
                if (dong[i].thuhai == dong[j].thunhat && (dong[j].bac <= bacdangtim || dong[j].thuba == "true") && dong[j].ma == "INSERT")
                {
                    dem = 1;
                    break;
                }
                if (laydendaungoac(dong[i].thuhai) == dong[j].thunhat && dong[j].ma == "INSERT" && (dong[j].bac <= bacdangtim || dong[j].thuba == "true"))
                {
                    dem = 1;
                    break;
                }
                if (dong[j].bac < bacdangtim)
                    bacdangtim = dong[j].bac;
            }
            if (!dem)
            {
                dongloi = dong[i].ma + " " + dong[i].thunhat + " " + dong[i].thuhai;
                loi = 3;
                break;
            }

            bacdangtim = dong[i].bac;
            int kq = 0;
            int timkiem = 0;
            for (int j = i - 1; j >= 0; j--)
            {
                if (dong[i].thunhat == dong[j].thunhat && (dong[j].bac <= bacdangtim || dong[j].thuba == "true") && dong[j].ma == "INSERT")
                {
                    if (isParam(dong[i].thuhai))
                    {
                        if (dong[j].thuhai == "number")
                        {
                            timkiem = j;
                            kq = 1;
                            break;
                        }
                        else
                        {
                            kq = 0;
                            break;
                        }
                    }
                    else if (isstring(dong[i].thuhai))
                    {
                        if (dong[j].thuhai == "string")
                        {
                            timkiem = j;
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
                        int timkiem2 = 0;
                        int bacdangtimne = dong[i].bac;
                        for (int k = i - 1; k >= 0; k--)
                        {
                            if (dong[i].thuhai == dong[k].thunhat && dong[k].ma == "INSERT" && (dong[k].bac <= bacdangtimne || dong[k].thuba == "true"))
                            {
                                if ((dong[k].thuhai == "number" && dong[j].thuhai == "number") || (dong[k].thuhai == "string" && dong[j].thuhai == "string"))
                                {
                                    kq = 1;
                                    timkiem = j;
                                    timkiem2 = k;
                                    int sosanh2 = 0;
                                    int xoay2 = 0;
                                    data moi2(dong[timkiem2].thunhat, dong[timkiem2].bac, dong[timkiem2].thuhai, dong[timkiem2].thuba);
                                    bst.searchTree(moi2, sosanh2, xoay2);
                                    sosanh += sosanh2;
                                    xoay += xoay2;
                                    break;
                                }
                            }
                            else if ((laydendaungoac(dong[i].thuhai) == dong[k].thunhat) && dong[k].ma == "INSERT" && dong[k].thunhat != "" && (dong[k].bac <= bacdangtimne || dong[k].thuba == "true"))
                            {
                                if (strstr(dong[j].thuhai.c_str(), "->"))
                                {
                                    kq = 0;
                                    break;
                                }
                                if (k == j)
                                {
                                    kq = 0;
                                    break;
                                }
                                if (!(dong[k].thuhai.substr(dong[k].thuhai.size() - 6) == dong[j].thuhai.substr(dong[j].thuhai.size() - 6)))
                                {
                                    kq = 0;
                                    break;
                                }
                                string gan = "";
                                string bandau = "";

                                timkiem2 = k;
                                int sosanh2 = 0;
                                int xoay2 = 0;
                                data moi2(dong[timkiem2].thunhat, dong[timkiem2].bac, dong[timkiem2].thuhai, dong[timkiem2].thuba);
                                bst.searchTree(moi2, sosanh2, xoay2);
                                sosanh += sosanh2;
                                xoay += xoay2;

                                gan = strchr(dong[i].thuhai.c_str(), '(');
                                bandau = strchr(dong[k].thuhai.c_str(), '(');

                                gan = lamgon(gan);
                                bandau = lamgon(bandau);
                                int dodaim = dodai(bandau);
                                int testtrongngoac = 1;
                                int bacdangtimnene = dong[i].bac;

                                for (int m = 0; m < dodaim; m++)
                                {
                                    string ane = laydendauphay(bandau);
                                    string bne = laydendauphay(gan);

                                    if (!isParam(bne) && !isstring(bne))
                                    {
                                        for (int n = i - 1; n >= 0; n--)
                                        {
                                            if (dong[n].thunhat == bne && (dong[n].bac <= bacdangtimnene || dong[n].thuba == "true") && dong[n].ma == "INSERT")
                                            {
                                                if (dong[n].thuhai == "number" && !(ane == "number"))
                                                {
                                                    testtrongngoac = 0;
                                                    break;
                                                }
                                                else if (dong[n].thuhai == "string" && !(ane == "string"))
                                                {
                                                    testtrongngoac = 0;
                                                    break;
                                                }
                                                else if (dong[n].thuhai != "string" && dong[n].thuhai != "number")
                                                {
                                                    testtrongngoac = 0;
                                                    break;
                                                }
                                                timkiem2 = n;
                                                int sosanh2 = 0;
                                                int xoay2 = 0;
                                                data moi2(dong[timkiem2].thunhat, dong[timkiem2].bac, dong[timkiem2].thuhai, dong[timkiem2].thuba);
                                                bst.searchTree(moi2, sosanh2, xoay2);
                                                sosanh += sosanh2;
                                                xoay += xoay2;
                                            }
                                            if (dong[n].bac <= bacdangtimnene)
                                                bacdangtimnene = dong[n].bac;
                                        }
                                        if (testtrongngoac == 0)
                                            break;
                                    }
                                    else if (ane == "number" && !isParam(bne))
                                    {
                                        testtrongngoac = 0;
                                        break;
                                    }
                                    else if (ane == "string" && !isstring(bne))
                                    {
                                        testtrongngoac = 0;
                                        break;
                                    }
                                }
                                if (dodai(gan) == dodai(bandau))
                                {
                                    if (testtrongngoac)
                                    {
                                        timkiem = j;
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
                                    kq = 0;
                                    break;
                                }
                            }
                            if (dong[k].bac < bacdangtimne)
                                bacdangtimne = dong[k].bac;
                        }
                        if (kq)
                            break;
                    }
                }
                bacdangtim = dong[j].bac;
            }
            if (!kq)
            {
                dongloi = dong[i].ma + " " + dong[i].thunhat + " " + dong[i].thuhai;
                loi = 2;
                break;
            }

            data moi(dong[timkiem].thunhat, dong[timkiem].bac, dong[timkiem].thuhai, dong[timkiem].thuba);
            bst.searchTree(moi, sosanh, xoay);
            cout << sosanh << " " << xoay << endl;
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
            int j = i;
            for (j = i; j >= 0; j--)
            {
                if (!(dong[j].bac >= dong[i].bac))
                    break;
            }
            for (++j; j <= i && dong[j].bac >= dong[i].bac; j++)
            {
                if (dong[j].ma == "INSERT" && dong[j].thuba != "true")
                {
                    data moi(dong[j].thunhat, dong[j].bac, dong[j].thuhai, dong[j].thuba);
                    if (moi.level >= dong[i].bac)
                        bst.deleteNode(moi);
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
            int demchua = 0;
            int bacdangtim = dong[i].bac;
            int timkiem = 0;
            for (int j = i - 1; j >= 0; j--)
            {
                if (dong[i].thunhat == dong[j].thunhat && dong[j].ma == "INSERT" && dong[j].bac <= bacdangtim)
                {
                    if (dong[j].thuba == "true")
                        cout << 0 << endl;
                    else
                        cout << dong[j].bac << endl;
                    demchua = 1;
                    timkiem = j;
                    break;
                }
                else if (dong[i].thunhat == dong[j].thunhat && dong[j].ma == "INSERT" && dong[j].thuba == "true")
                {
                    cout << 0 << endl;
                    demchua = 1;
                    timkiem = j;
                }
                if (dong[j].bac < bacdangtim)
                    bacdangtim = dong[j].bac;
            }

            if (demchua == 0)
            {
                loi = 3;
                dongloi = dong[i].ma + " " + dong[i].thunhat;
                break;
            }
            int sosanh = 0;
            int xoay = 0;
            data moi(dong[timkiem].thunhat, dong[timkiem].bac, dong[timkiem].thuhai, dong[timkiem].thuba);
            bst.searchTree(moi, sosanh, xoay);
        }
        else if (dong[i].ma == "PRINT")
        {
            bst.in();
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
    // for(int j = 0 ; j <= i ; j++)
    // {
    //     if(dong[j].ma == "INSERT") 
    //     {
    //         data moi(dong[j].thunhat, dong[j].bac, dong[j].thuhai, dong[j].thuba);
    //         bst.deleteNode(moi);
    //     }
    // }
    bst.deleteBinaryTree();
    delete[] dong;
    myfile.close();
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
}
