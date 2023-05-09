#include<bits/stdc++.h>
using namespace std;
ifstream f("abce.in");
ofstream g("abce.out");

class Node
{
public:
     long long  key;
    Node *left;
    Node *right;
    int height;
};

int height(Node *N)
{
    if (N==NULL)
        return 0;
    return N->height;
}

Node* newNode(long long  key)
{
    Node* node=new Node();
    node->key=key;
    node->left=NULL;
    node->right=NULL;
    node->height=1;
    return node;
}

Node *rightRotate(Node *y)
{
    Node *x=y->left;
    Node *a=x->right;

    x->right=y;
    y->left=a;

    y->height=max(height(y->left),height(y->right))+1;
    x->height=max(height(x->left),height(x->right))+1;
    return x;///noua radacina
}

Node *leftRotate(Node *x)
{
    Node *y=x->right;
    Node *a=y->left;

    y->left=x;
    x->right=a;

    x->height=max(height(x->left),height(x->right))+1;
    y->height=max(height(y->left),height(y->right))+1;
    return y;///noua radacina
}

///balanta
int balanta(Node *N)
{
    if (N==NULL)
        return 0;
    return height(N->left)-height(N->right);
}

bool cauta(Node* node,long long  x)
{
    if(node==NULL)
        return false;
    else if(x==node->key)
        return true;
    else if(x<node->key)
        return cauta(node->left,x); ///caut la stanga nodului daca esrte mai mic decat x
    else
        return cauta(node->right,x); ///la dreapta daca este mai mare
}



///inserare nod in avl
Node* insert(Node* node,  long long  key)
{
    if (node==NULL)
        return newNode(key);///daca e gol
    ///inserare basic
    if (key<node->key)
        node->left=insert(node->left,key);
    else if (key>node->key)
        node->right=insert(node->right,key);
    else
        return node; ///nu putem avea egale in bst

    node->height=max(height(node->left), height(node->right))+1; ///inaltimea noua dupa ce am inserat un nod

    ///verific daca e balansat sau nu
    int b=balanta(node);

    ///caz LL fac rotatie la dreapta ca sa il balansez
    if (b>1 && key<node->left->key)
        return rightRotate(node);

    ///caz RR fac rotatie la stanga ca sa il balansez
    if (b<-1 && key>node->right->key)
        return leftRotate(node);

    ///caz LR fac rotatie la dreapta ca sa il balansez
    if (b>1 && key>node->left->key)
    {
        node->left=leftRotate(node->left);
        return rightRotate(node);
    }

    ///caz RL fac rotatie la stanga ca sa il balansez
    if (b<-1 && key<node->right->key)
    {
        node->right=rightRotate(node->right);
        return leftRotate(node);
    }

    ///pointer catre noua radacina
    return node;
}

///pentru stergere
Node* minValueNode(Node* node)
{
    Node* cautat=node;
    while (cautat->left!=NULL)
        cautat=cautat->left;
    return cautat;
}

Node* deleteNode(Node* root,  long long  key)
{
    if(root==NULL)
        return root;

    ///vad unde se afla
    if(key<root->key)
        root->left=deleteNode(root->left,key);
    else if(key>root->key)
        root->right=deleteNode(root->right,key);
    else
    {
        if( root->left==NULL || root->right==NULL) ///maxim un fiu
        {
            Node *temp;

            if(root->left!=0)
                temp=root->left;
            else
                temp=root->right;

            if(temp!=NULL)
                *root=*temp;
            else
            {
                temp=root;
                root=NULL;
            }
            free(temp);
        }
        else ///doi fii
        {
            Node* temp = minValueNode(root->right);

            ///copiez in radacina
            root->key = temp->key;

            ///sterg succesorul
            root->right = deleteNode(root->right,temp->key);
        }
    }

    ///cazul pt un nod
    if (root==NULL)
        return root;

    ///inaltimea
    root->height=max(height(root->left),height(root->right))+1;

    ///verific daca e balansat
    int balance=balanta(root);


    ///caz LL
    if (balance>1 && balanta(root->left)>=0)
        return rightRotate(root);

     ///caz RR
    if (balance<-1 && balanta(root->right) <= 0)
        return leftRotate(root);

    /// caz LR
    if (balance>1 && balanta(root->left)<0)
    {
        root->left=leftRotate(root->left);
        return rightRotate(root);
    }

    ///caz RL
    if (balance<-1 && balanta(root->right)>0)
    {
        root->right=rightRotate(root->right);
        return leftRotate(root);
    }

    return root;///radacina
}

int mare(Node* root,  long long  x)
{
    int mare1=-1;
    while (root!=NULL)
    {
        if (root->key==x)
        {
            return root->key;
        }
        else if (root->key>x)
        {
            root=root->left; ///daca e mai mare, cautam in stanga
        }
        else
        {
            mare1=root->key;
            root=root->right; ///cautam in continuare dupa ce am retinut valoarea gasita buna
        }
    }
    return mare1;
}

int mic(Node* root,  long long  x)
{
    int mic1=-1;
    while (root!=NULL)
    {
        if (root->key==x)
        {
            return root->key;
        }
        else if (root->key<x)
        {
            root=root->right;///daca e mai mic cautam
        }
        else
        {
            mic1=root->key;
            root=root->left;
        }
    }
    return mic1;
}

void intre(Node *root,  long long  x, long long y)
{
    if(root==NULL)
        return;

    if(root->key<x)
        intre(root->right,x,y); ///daca e prea mic merg la dreapta
    else if(root->key<=y && root->key >=x) ///trebuie sa fie fix in interval, ca altfel nu mi arata capetele
    {
        intre(root->left,x,y);
        g<<root->key<<" ";///e bun
        intre(root->right,x,y);
    }
    else
        intre(root->left,x,y); ///daca e prea mare merg la stanga

}

int main()
{
    Node *root=NULL;
    int nrop;
    f>>nrop;

    for(long long i=0; i<nrop; i++)
    {
        long long int op;
        f>>op;
        switch(op)
        {
        case 1:
        {
            long long int x;
            f>>x;
            root=insert(root, x);
            break;
        }
        case 2:
        {
            long long int x;
            f>>x;
            root=deleteNode(root, x);
            break;
        }
        case 3:
        {
            long long int x;
            f>>x;
            g<<cauta(root,x)<<endl;
            break;
        }
        case 4:
        {
            long long int x;
            f>>x;
            g<<mare(root,x)<<endl;
            break;
        }
        case 5:
        {
            long long int x;
            f>>x;
            g<<mic(root,x)<<endl;
            break;
        }
        case 6:
        {
            long long int x,y;
            f>>x>>y;
            intre(root,x,y);
            g<<endl;
            break;
        }
        }
    }
    f.close();
    g.close();

    return 0;
}
