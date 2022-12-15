#include <iostream>
#include <unordered_map>
using std::cout;
using std::unordered_map;

//定义一个链表类维护最近最少访问的，最近最少访问的放在头节点
struct LRUEle{
    LRUEle(){frame_id=-1;}
    LRUEle(int id){frame_id=id;}
    int frame_id;
    LRUEle* prev;
    LRUEle* next;
};

class LRUList{
public:
    LRUList(){
        size = 0;
        head = new LRUEle;
        tail = new LRUEle;
        head->next = tail;
        tail->prev = head;

    }

    
    void InsertEle(LRUEle *ele){
        cache[ele->frame_id] = ele;
        ele->prev = tail->prev; 
        ele->next = tail;
        tail->prev->next = ele;
        tail->prev = ele;
        this->size++;
    }

    //删除元素
    void DeleteEle(LRUEle *ele){
        cache.erase(ele->frame_id);
        ele->prev->next = ele->next;
        ele->next->prev = ele->prev;
        delete ele;
        this->size--;
         
    }

    int DropFirst(){
        //总是删除头部的元素，并将元素的frame_id返回
        if(head->next==tail) return -1;
        LRUEle *first = head->next;
        int frame_id = first->frame_id;
        DeleteEle(first);
        return frame_id;
    }

    //最近访问了已经存在frame中的元素，需要提高其优先级
    void UpdateList(int frame_id){
        //先找到其在表中的位置，
        LRUEle *ele = cache[frame_id];
        DeleteEle(ele);
        LRUEle *node = new LRUEle(frame_id);
        InsertEle(node);
    }

    void PrintEle(){
        LRUEle *ele = head;
        while(ele->next!=tail){
            cout<<ele->next->frame_id<<"-";
            ele=ele->next;
            
        }
    }

private:
    //头尾不包括元素
    int size;
    LRUEle* head;
    LRUEle* tail;
    unordered_map<int, LRUEle*> cache;
};