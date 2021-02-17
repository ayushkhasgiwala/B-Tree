#include<bits/stdc++.h>
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
                for(int i=0;i<leaf->size;i++)
                {
                    if(leaf->keys[i].first==x)
                    {
                        leaf->keys[i].second++;
                        leaf->size++;
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
                        new_node->ptr[order-1]=NULL;
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
                        }
                        else{
                            recursive_internal(parent,new_node,new_node->keys[0]);
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
                    return parent;
                }
            }
            return parent;
        }


        void recursive_internal(B_plus_Node* parent, B_plus_Node* child, pair<int,int> x)
        {
            if(parent->size < order-1)
            {
                //cout<<"hello"<<endl;
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

                for(int j=parent->size+1;j>=k;j--)
                {
                    parent->ptr[j]=parent->ptr[j-1];
                }
                parent->ptr[k+1]=child;
            }

            else
            {
                vector<pair<int,int>> temp_keys;
                vector<B_plus_Node*> temp_ptr;
                for(int i=0;i<parent->size;i++)
                {
                    temp_keys.push_back(parent->keys[i]);
                }
                int sz_key=temp_keys.size();
                for(int i=0;i<=parent->size;i++)
                {
                    temp_ptr.push_back(parent->ptr[i]);
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
                for(int j=sz_ptr;j>=k;j--)
                {
                    temp_ptr[j]=temp_ptr[j-1];
                }
                temp_ptr[k+1]=child;

                B_plus_Node *new_node=create_B_node_obj();
                new_node->isleaf=0;
                new_node->size=(order-1)-(order/2);
                parent->size=(order)/2;
                int j=parent->size+1;
                //cout<<parent->size<<"****"<<new_node->size<<endl;
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

                if(root==parent)
                {
                    B_plus_Node *new_root=create_B_node_obj();
                    new_root->isleaf=0;
                    int d=parent->size;
                    new_root->keys[0]=parent->keys[d];
                    new_root->size=1;
                    new_root->ptr[0]=parent;
                    new_root->ptr[1]=new_node;
                    root=new_root;
                }
                else
                {
                    B_plus_Node* new_parent=find_parent(root,parent);
                    int d=parent->size;
                    recursive_internal(new_parent,new_node,parent->keys[d]);
                }
            }


        }

        void check()
        {
            check_helper(root);
        }

        void check_helper(B_plus_Node* temp){
 
            if(!temp){
                cout<<"ROOT IS NULL"<<endl;
            }
            else{
                cout<<"COUNT : "<<temp->size<<endl;
                for(int i=0; i<=temp->size; i++){
                    //cout<<temp->keys[i].first<<" = "<< temp->keys[i].second<<endl; 
                    check_helper(temp->ptr[i]);
                    cout<<temp->keys[i].first<<" = "<< temp->keys[i].second<<endl; 
            }
        }

        
    }
    
};


int main()
{
    B_plus_tree obj;
    obj.insert(20);
    obj.insert(10);
    obj.insert(15);
    obj.insert(30);
    obj.insert(40);
    obj.check();
    //obj.printroot();

}


