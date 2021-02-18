#include<bits/stdc++.h>
#include <fstream>
#include <fstream>
#define order 3
using namespace std;


struct B_plus_Node
{
    vector<pair<int,int>> keys;
    int isleaf;
    int size;
    vector<B_plus_Node*> ptr;
};

B_plus_Node* create_B_node_obj()
{
    struct B_plus_Node *root=(struct B_plus_Node*)malloc(sizeof(B_plus_Node));
    root->keys.resize(order-1);
    root->ptr.resize(order,NULL);
    return root;
}

class B_plus_tree
{
    public:
        B_plus_Node *root=create_B_node_obj();

    public:
        B_plus_tree()
        {
            root=NULL;
        }

        void printroot()
        {
            print_helper(root);
        }

        void print_helper(B_plus_Node* root)
        {
            for(int i=0; i<=root->size; i++)
            {
                cout<<root->keys[i].first<<" = "<< root->keys[i].second<<endl;
            }
        }

        void linked_list_traversal()
        {
            B_plus_Node *temp=left_leaf(root);
            ll_helper(temp);
        }

        B_plus_Node * left_leaf(B_plus_Node *root)
        {
            if(root==NULL)
                return NULL;
            if(root->ptr[0]==NULL)
                return root;
            return left_leaf(root->ptr[0]);
        }

        void ll_helper(B_plus_Node *temp)
        {
            while(temp!=NULL)
            {
                for(int i=0;i<temp->size;i++)
                {
                    cout<<temp->keys[i].first<<" ";
                }
                temp=temp->ptr[temp->size];
            }
            
        }

        pair<B_plus_Node *,B_plus_Node *> find_leaf(int x, B_plus_Node *root)
        {
            B_plus_Node *temp=root;
            B_plus_Node *parent;
            while(temp->isleaf!=1)
            {
                int sz=temp->size;
                parent=temp;
                for(int i=0;i<sz;i++)
                {
                    if(x>temp->keys[i].first && i!=sz-1)
                    {
                        continue;
                    }
                    else if(x==temp->keys[i].first)
                    {
                        temp=temp->ptr[i+1];
                        break;
                    }
                    else if(x<temp->keys[i].first)
                    {
                        temp=temp->ptr[i];
                        break;
                    }
                    if(i==sz-1)
                    {
                        temp=temp->ptr[i+1];
                        break;
                    }
                }
            }
            return {temp,parent};
        }


        void insert(int x)
        {
            if(root==NULL)
            {
                B_plus_Node *temp=create_B_node_obj();
                temp->keys[0]=make_pair(x,1);
                temp->isleaf=1;
                temp->size=1;
                root=temp;
            }
            else
            {
                pair<B_plus_Node *,B_plus_Node *>p=find_leaf(x,root);
                int flag=0;
                B_plus_Node *leaf=p.first;
                B_plus_Node *parent=p.second;
                /*if(parent!=NULL)
                    cout<<"parent ****= "<<parent->keys[0].first<<endl;*/
                
                for(int i=0;i<leaf->size;i++)
                {
                    if(leaf->keys[i].first==x)
                    {
                        leaf->keys[i].second++;
                        //cout<<"node= "<<leaf->keys[i].first<<"  count= "<<leaf->keys[i].second<<endl;
                        //leaf->size++;
                        flag=1;
                        break;
                    }
                }
                if(flag==0)
                {
                    // directly insert at leaf 
                    if(leaf->size < order-1)
                    {
                        //cout<<"hello"<<endl;
                        //cout<<leaf->size<<endl;
                        int i=0;
                        for(i=0;i<leaf->size;i++)
                        {
                            if(leaf->keys[i].first<x)
                            {
                                continue;
                            }
                            else{
                                leaf->keys.insert(leaf->keys.begin()+i,{x,1});
                                //leaf->size++;
                                leaf->keys.resize(order-1);
                                B_plus_Node* t=leaf->ptr[leaf->size];
                                leaf->ptr[leaf->size]=NULL;
                                leaf->size++;
                                //cout<<leaf->size<<endl;
                                leaf->ptr[leaf->size]=t;
                                //cout<<leaf->keys.size()<<endl;
                                break;
                            }
                        }
                        if(i==leaf->size)
                        {
                            leaf->keys.insert(leaf->keys.begin()+i,{x,1});
                            leaf->keys.resize(order-1);
                            B_plus_Node* t=leaf->ptr[leaf->size];
                            leaf->ptr[leaf->size]=NULL;
                            leaf->size++;
                            //cout<<leaf->size<<endl;
                            //cout<<leaf->keys.size()<<endl;
                            leaf->ptr[leaf->size]=t;
                        }
                        //cout<<leaf->size<<endl;
                    }
                    // leaf pr split krwao
                    
                    else if(leaf->size == order-1)
                    {
                        //cout<<"hello"<<endl;
                        /*if(parent==root)
                            cout<<"parent ****= "<<parent->keys[0].first<<endl;*/
                        vector<pair<int,int>> temp_keys;
                        for(int i=0;i<leaf->size;i++)
                        {
                            temp_keys.push_back(leaf->keys[i]);
                        }
                        int sz=temp_keys.size();
                        int i=0;
                        for(i=0;i<sz;i++)
                        {
                            if(temp_keys[i].first<x)
                            {
                                continue;
                            }
                            else{
                                //temp_keys.resize(sz+1);
                                temp_keys.insert(temp_keys.begin()+i,{x,1});
                                break;
                            }
                        }
                        if(i==sz)
                        {
                            //temp_keys.resize(sz+1);
                            temp_keys.insert(temp_keys.begin()+i,{x,1});
                        }
                        /*for(int j=0;j<temp_keys.size();j++)
                        {
                            cout<<temp_keys[j].first<<" ";
                        }
                        cout<<endl;*/
                        B_plus_Node *new_node=create_B_node_obj();
                        new_node->isleaf=1;
                        new_node->size=order-(order/2);
                        leaf->size=(order)/2;
                        leaf->ptr[leaf->size]=new_node;
                        new_node->ptr[new_node->size]=leaf->ptr[order-1];
                        leaf->ptr[order-1]=NULL;
                        //cout<<leaf->size<<"****"<<new_node->size<<endl;
                        for(int i=0;i<leaf->size;i++)
                        {
                            leaf->keys[i]=temp_keys[i];
                        }
                        int k=leaf->size;
                        for(int i=0;i<new_node->size;i++)
                        {
                            new_node->keys[i]=temp_keys[k];
                            k++;
                        }
                        /*for(int j=0;j<temp_keys.size();j++)
                        {
                            cout<<leaf->keys[j].first<<" ";
                        }
                        cout<<endl;
                        for(int j=0;j<temp_keys.size();j++)
                        {
                            cout<<new_node->keys[j].first<<" ";
                        }
                        cout<<endl;*/
                        if(root==leaf)
                        {
                            B_plus_Node *new_root=create_B_node_obj();
                            new_root->isleaf=0;
                            new_root->keys[0]=new_node->keys[0];
                            new_root->size=1;
                            new_root->ptr[0]=leaf;
                            new_root->ptr[1]=new_node;
                            root=new_root;
                            /*cout<<"######################################################"<<endl;
                            for(int i=0;i<=root->size;i++)
                            {
                                for(int j=0;j<root->ptr[i]->size;j++)
                                {
                                    cout<<root->ptr[i]->keys[j].first<<" ";
                                }
                                cout<<endl;
                            }
                            //cout<<root->ptr[0]->ptr[1]->keys[0].first<<endl;
                            cout<<"######################################################"<<endl;*/
                        }
                        else{
                            /*cout<<"hnwgn- "<<new_node->keys[0].first<<endl;
                            cout<<"hnwgn- "<<new_node->keys[1].first<<endl;
                            cout<<"jb5byrerh- "<<leaf->keys[0].first<<endl;
                            cout<<"wdfmsm - "<<leaf->ptr[leaf->size]->keys[0].first<<endl;
                            cout<<root->size<<endl;
                            cout<<"qthnt "<<root->ptr[root->size]->keys[0].first<<endl;
                            cout<<"qthnt "<<root->ptr[root->size]->keys[1].first<<endl;
                            cout<<"root size = "<<root->ptr[root->size]->size<<endl;
                            cout<<"parent = "<<parent->keys[0].first<<endl;
                            if(parent->size > 1)
                                cout<<"parent = "<<parent->keys[1].first<<endl;
                            cout<<"******************************************";*/
                            /*cout<<"######################################################"<<endl;
                            for(int i=0;i<=root->size;i++)
                            {
                                cout<<root->ptr[i]->size<<endl;
                                for(int j=0;j<root->ptr[i]->size;j++)
                                {
                                    cout<<root->ptr[i]->keys[j].first<<" ";
                                }
                                cout<<endl;
                            }
                            //cout<<root->ptr[0]->ptr[1]->keys[0].first<<endl;
                            cout<<"######################################################"<<endl;*/
                            recursive_internal(parent,new_node,new_node->keys[0]);
                            /*cout<<"######################################################"<<endl;
                            for(int i=0;i<=root->size;i++)
                            {
                                //cout<<root->ptr[i]->size<<endl;
                                for(int j=0;j<root->ptr[i]->size;j++)
                                {
                                    cout<<root->ptr[i]->keys[j].first<<" ";
                                }
                                cout<<endl;
                            }
                            //cout<<root->ptr[0]->ptr[1]->keys[0].first<<endl;
                            cout<<"######################################################"<<endl;*/
                        }

                    }
                }
                
            }
        }


        B_plus_Node* find_parent(B_plus_Node* tree_root, B_plus_Node* child)
        {
            if(tree_root->isleaf==1 || tree_root->ptr[0]->isleaf==1)
            {
                return NULL;
            }
            int s=tree_root->size;
            B_plus_Node* parent;
            int i=0;
            for(;i<=s;i++)
            {
                if(tree_root->ptr[i]==child)
                {
                    parent=tree_root;
                    return tree_root;
                }
                else
                {
                    parent=find_parent(tree_root->ptr[i],child);
                    if(parent!=NULL)
                        return parent;
                }
            }
            return parent;
        }


        void recursive_internal(B_plus_Node* parent, B_plus_Node* child, pair<int,int> x)
        {
            //cout<<"start"<<endl;
            if(parent->size < order-1)
            {
                //cout<<"hello jntj"<<endl;
                //cout<<leaf->size<<endl;
                int i=0;
                int k;
                for(i=0;i<parent->size;i++)
                {
                    if(parent->keys[i].first<x.first)
                    {
                        continue;
                    }
                    else
                    {
                        parent->keys.insert(parent->keys.begin()+i,x);
                        //leaf->size++;
                        parent->keys.resize(order-1);
                        /*B_plus_Node* t=parent->ptr[parent->size];
                        parent->ptr[parent->size]=NULL;*/
                        parent->size++;
                        //cout<<leaf->size<<endl;
                        /*parent->ptr[parent->size]=t;
                        //cout<<leaf->keys.size()<<endl;*/
                        k=i;
                        break;
                    }
                }
                
                if(i==parent->size)
                {
                    parent->keys.insert(parent->keys.begin()+i,x);
                    parent->keys.resize(order-1);
                    /*B_plus_Node* t=parent->ptr[parent->size];
                    parent->ptr[parent->size]=NULL;*/
                    parent->size++;
                    k=i;
                    /*//cout<<leaf->size<<endl;
                    //cout<<leaf->keys.size()<<endl;
                    parent->ptr[parent->size]=t;*/
                }
                //cout<<"hello jntj"<<endl;
                for(int j=parent->size;j>k;j--)
                {
                    parent->ptr[j]=parent->ptr[j-1];
                }
                parent->ptr[k+1]=child;
                //cout<<"hello world"<<endl;
            }
            //cout<<"else ...."<<endl;
            else
            {
                vector<pair<int,int>> temp_keys;
                vector<B_plus_Node*> temp_ptr(order+1);
                for(int i=0;i<parent->size;i++)
                {
                    temp_keys.push_back(parent->keys[i]);
                }
                int sz_key=temp_keys.size();
                for(int i=0;i<=parent->size;i++)
                {
                    temp_ptr[i]=(parent->ptr[i]);
                }
                int sz_ptr=temp_ptr.size();
                int i=0;
                int k;
                for(i=0;i<sz_key;i++)
                {
                    if(temp_keys[i].first<x.first)
                    {
                        continue;
                    }
                    else{
                        //temp_keys.resize(sz+1);
                        temp_keys.insert(temp_keys.begin()+i,x);
                        k=i;
                        break;
                    }
                }
                if(i==sz_key)
                {
                    //temp_keys.resize(sz+1);
                    temp_keys.insert(temp_keys.begin()+i,x);
                    k=i;
                }
                /*for(int j=0;j<temp_keys.size();j++)
                {
                    cout<<temp_keys[j].first<<" ";
                }
                cout<<endl;*/
                //cout<<"hello "<<sz_ptr<<endl;
                for(int j=sz_ptr;j>k;j--)
                {
                    temp_ptr[j]=temp_ptr[j-1];
                }
                temp_ptr[k+1]=child;
                /*cout<<"######################################################"<<endl;
                for(int i=0;i<sz_ptr;i++)
                {
                    for(int j=0;j<temp_ptr[i]->size;j++)
                    {
                        cout<<temp_ptr[i]->keys[j].first<<" ";
                    }
                    cout<<endl;
                }
                cout<<"######################################################"<<endl;*/
                //cout<<"ergn"<<endl;
                B_plus_Node *new_node=create_B_node_obj();
                new_node->isleaf=0;
                new_node->size=(order-1)-(order/2);
                parent->size=(order)/2;
                int j=parent->size+1;
                //cout<<parent->size<<"****"<<new_node->size<<endl;
                //cout<<"yaha tak aa gaya"<<endl;
                for(int i=0;i<new_node->size;i++)
                {
                    new_node->keys[i]=temp_keys[j];
                    j++;
                }
                /*for(int j=0;j<temp_keys.size();j++)
                {
                    cout<<new_node->keys[j].first<<" ";
                }
                cout<<endl;*/
                int m=parent->size+1;
                for(int i=0;i<=new_node->size;i++)
                {
                    new_node->ptr[i]=temp_ptr[m];
                    m++;
                }
                //cout<<"yaha tak aa gaya 2"<<endl;
                if(root==parent)
                {
                    //cout<<"if ke andar"<<endl;
                    B_plus_Node *new_root=create_B_node_obj();
                    new_root->isleaf=0;
                    int d=parent->size;
                    new_root->keys[0]=parent->keys[d];
                    new_root->size=1;
                    new_root->ptr[0]=parent;
                    new_root->ptr[1]=new_node;
                    root=new_root;
                    //cout<<"######################################################"<<endl;
                    /*for(int i=0;i<root->size;i++)
                    {
                        cout<<root->keys[0].first<<" ";
                    }
                    cout<<endl;
                    B_plus_Node * trav=root;
                    while(trav->isleaf!=1)
                    for(int i=0;i<=root->size;i++)
                    {
                        for(int j=0;j<root->ptr[i]->size;j++)
                        {
                            cout<<root->ptr[i]->keys[j].first<<" ";
                        }
                        cout<<endl;
                    }*/
                    //check_helper(root);
                    //cout<<"######################################################"<<endl;
                }
                else
                {
                    //cout<<"rytbty"<<endl;
                    B_plus_Node* new_parent=find_parent(root,parent);
                    //cout<<"chal ja bhsdk"<<endl;
                    //cout<<new_parent->size<<endl;
                    int d=parent->size;
                    //cout<<parent->keys[d].first<<endl;
                    //cout<<new_parent->keys[0].first<<endl;
                    //cout<<new_node->keys[0].first<<endl;
                    //cout<<new_node->ptr[0]->keys[0].first<<endl;
                    //cout<<new_node->ptr[1]->keys[0].first<<endl;
                    //cout<<parent->ptr[0]->keys[0].first<<endl;
                    //cout<<parent->ptr[1]->keys[0].first<<endl;
                    recursive_internal(new_parent,new_node,parent->keys[d]);
                }
            }


        }

        bool find(int x)
        {
            if(root==NULL)
            {
                return false;
            }
            return find_helper(x, root);
        }

        bool find_helper(int x, B_plus_Node* temp)
        {
            if(temp->isleaf==1)
            {
                //cout<<temp->keys[0].first<<endl;
                int n=temp->size;
                int i=0;
                for(i=0;i<n;i++)
                {
                    if(temp->keys[i].first == x)
                    {
                        return true;
                    }
                }
                return false;
            }
            else
            {
                int n=temp->size;
                //cout<<"size= "<<n<<endl;
                int i=0;
                for(i=0;i<n;i++)
                {
                    //cout<<"val= "<<temp->keys[i].first<<endl;
                    if(temp->keys[i].first <= x)
                    {
                        continue;
                    }
                    else
                    {
                        return find_helper(x,temp->ptr[i]);
                    }
                }
                if(i==n)
                {
                    //cout<<"val1= "<<temp->keys[i].first<<endl;
                    return find_helper(x,temp->ptr[i]);
                }
            }
            return false;
        }

        B_plus_Node* search(int x, B_plus_Node* temp)
        {
            if(temp->isleaf==1)
            {
                //cout<<temp->keys[0].first<<endl;
                int n=temp->size;
                int i=0;
                for(i=0;i<n;i++)
                {
                    if(temp->keys[i].first == x)
                    {
                        return temp;
                    }
                }
                return NULL;
            }
            else
            {
                int n=temp->size;
                //cout<<"size= "<<n<<endl;
                int i=0;
                for(i=0;i<n;i++)
                {
                    //cout<<"val= "<<temp->keys[i].first<<endl;
                    if(temp->keys[i].first <= x)
                    {
                        continue;
                    }
                    else
                    {
                        return search(x,temp->ptr[i]);
                    }
                }
                if(i==n)
                {
                    //cout<<"val1= "<<temp->keys[i].first<<endl;
                    return search(x,temp->ptr[i]);
                }
            }
            return NULL;
        }


        int count(int x)
        {
            return count_helper(x, root);
        }

        int count_helper(int x, B_plus_Node* temp)
        {
            B_plus_Node* node=search(x,temp);
            if(node==NULL)
                return 0;
            int n=node->size;
            int c;
            //cout<<"x= "<<x<<endl;
            for(int i=0;i<n;i++)
            {
                //cout<<"node value= "<<node->keys[i].first<<endl;
                if(node->keys[i].first==x)
                {
                    c=node->keys[i].second;
                    //cout<<c<<endl;
                    break;
                }
            }
            return c;
        }

        int range(int x, int y)
        {
            //vector<int> ans;
            if(root==NULL)
                return 0;
            int min_val=find_min(root);
            int max_val=find_max(root);
            if(x<min_val)
                x=min_val;
            if(y>max_val)
                y=max_val;
            B_plus_Node* temp=left_leaf(root);
            int total=0;
            while(temp!=NULL)
            {
                for(int i=0;i<temp->size;i++)
                {
                    //cout<<temp->keys[i].first<<" ";
                    if(temp->keys[i].first>=x && temp->keys[i].first<=y)
                    {
                        total+=temp->keys[i].second;
                    }
                    if(temp->keys[i].first>y)
                    {
                        break;
                    }
                }
                temp=temp->ptr[temp->size];
            }
            //B_plus_Node* left_node=left_range(x, root);
            //B_plus_Node* right_node=right_range(y, root);
            return total;
            

        }

        int min_v()
        {
            return find_min(root);
        }

        int find_min(B_plus_Node* temp)
        {
            while(temp->isleaf!=1)
            {
                temp=temp->ptr[0];
            }
            return temp->keys[0].first;
        }

        
        int max_v()
        {
            return find_max(root);
        }
        int find_max(B_plus_Node* temp)
        {
            while(temp->isleaf!=1)
            {
                temp=temp->ptr[temp->size];
            }
            return temp->keys[temp->size-1].first;
        }



        

        void check()
        {
            check_helper(root);
        }

        void check_helper(B_plus_Node* temp){
 
            if(temp==NULL){
                //cout<<"ROOT IS NULL"<<endl;
                return;
            }
            else{
                //cout<<"COUNT : "<<temp->size<<endl;
                for(int i=0; i<=temp->size; i++){
                    //cout<<temp->keys[i].first<<" = "<< temp->keys[i].second<<endl; 
                    check_helper(temp->ptr[i]);
                    cout<<temp->keys[i].first<<"  "; 
            }
        }

        
    }
    
};


vector<string> parser(string line, char d)
{
    vector<string> ans;
    int n=line.length();
    string temp="";
    for(int i=0;i<n;i++)
    {
        if(line[i]!=d)
        {
            temp+=line[i];
        }
        else
        {
            ans.push_back(temp);
            temp="";
        }
    }
    if(temp!="")
        ans.push_back(temp);
    return ans; 
}



int main(int argc, char *argv[])
{
    B_plus_tree obj;
    string ipfile=argv[1];
    ifstream file(ipfile);
    vector<string> tokens;
    ofstream opfile ("output.txt");
    if(file.is_open())
    {
        string line;
        
        while(getline(file,line))
        {
            if(line != "")
            {
                //cout<<line<<"  ";
                //cout<<line.length()<<endl;
                tokens=parser(line,' ');
                //cout<<tokens.size();
                /*for(int i=0;i<tokens.size();i++)
                {
                    cout<<tokens[i]<<"=";
                }
                cout<<endl;*/
                if(tokens[0]=="INSERT")
                {
                    obj.insert(stoi(tokens[1]));
                }
                else if(tokens[0]=="FIND")
                {
                    bool res=obj.find(stoi(tokens[1]));
                    if(res==true)
                    {
                        cout<<"YES"<<endl;
                        opfile << "YES";
                        opfile << "\n";
                    }
                    else
                    {
                        cout<<"NO"<<endl;
                        opfile << "NO";
                        opfile << "\n";
                    }
                }
                else if(tokens[0]=="COUNT")
                {
                    int res=obj.count(stoi(tokens[1]));
                    cout<<res<<endl;
                    opfile << to_string(res);
                    opfile << "\n";
                }
                else if(tokens[0]=="RANGE")
                {
                    int x=stoi(tokens[1]);
                    int y=stoi(tokens[2]);
                    int res=obj.range(x,y);
                    cout<<res<<endl;
                    opfile << to_string(res);
                    opfile << "\n";
                }
                else
                {
                    cout<<"INVALID QUERY"<<endl;
                    opfile << "INVALID QUERY";
                    opfile << "\n";
                }
            }
        }
        opfile.close();
        file.close();
    }

}

/*obj.insert(20);
    obj.insert(10);
    obj.insert(15);
    obj.insert(30);
    obj.insert(40);*/
    //obj.check();
    /*for(int i=1;i<=100;i++)
    {
        obj.insert(i);
        /*if(i%2==0)
            obj.insert(i);*/
    /*}
    obj.printroot();
    obj.linked_list_traversal();
    cout<<endl;
    cout<<obj.find(33)<<endl;
    cout<<obj.find(-12)<<endl;
    cout<<obj.find(100)<<endl;
    cout<<obj.find(50)<<endl;
    cout<<obj.find(1)<<endl;
    cout<<obj.find(99)<<endl;
    cout<<obj.find(112)<<endl;
    cout<<"****************************"<<endl;
    cout<<obj.count(1)<<endl;
    cout<<obj.count(50)<<endl;
    cout<<obj.count(-12)<<endl;
    cout<<obj.count(100)<<endl;
    cout<<obj.count(99)<<endl;
    cout<<"****************************"<<endl;
    cout<<obj.min_v()<<endl;
    cout<<obj.max_v()<<endl;
    cout<<"****************************"<<endl;
    cout<<obj.range(-1,200)<<endl;
    cout<<obj.range(1,100)<<endl;
    cout<<obj.range(50,200)<<endl;
    cout<<obj.range(-1,99)<<endl;
    cout<<obj.range(10,97)<<endl;
    cout<<obj.range(101,987)<<endl;
    cout<<obj.range(-10,-97)<<endl;
    cout<<"****************************"<<endl;*/


