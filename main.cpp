// Данная программа написана неопытными программистами на С++. Любое совпадение со StackOverFlow - не более чем совпадение.

// Версия программы: 0.01 aplha. Возможны баги, вылеты и прочие аспекты подобного типа. Не исключено, что программа будет есть памяти больше,
// чем вся доступная оперативная память.

#include <bits/stdc++.h>
#include <stdexcept>

using namespace std;

struct Node;

Node* XOR(Node* prev, Node* next) {
    return (Node *)((uintptr_t)(prev) ^ (uintptr_t)(next)); // Возвращает XOR 2 элементов. uintptr_t - все равно, что функция Math.abs() в Java
}

struct Node{ // В этом контейнере будут храниться элементы
    int data;
    Node* npx;

    Node(int data, Node* prev, Node * next): data(data), npx(XOR(prev, next)){}
};

class XORList{

    private:
    Node* head;
    int elementCount; // Общее количество элементов

    // Передали массив и его размер, потому что C++ не умеет определять размер массива, а лишь получет ссылку на его 1 элемент
    public:
    XORList(int arr[], int arrSize){
        head = NULL;
        for(int i = 0; i < arrSize; i++){
            addEl(arr[i]);
        }
        elementCount = arrSize;
    }

    int getElementCount(){
        return elementCount;
    }

    void addEl(int data) {
        if (head == NULL) {
            head = new Node(data, NULL, NULL);
            elementCount++;
        }
        else {
            Node* prev = NULL;
            Node* cur = head;
            Node* next = XOR(prev, cur->npx);

            while (next != NULL) {
                prev = cur;
                cur = next;
                next = XOR(prev, cur->npx);
            }

            Node* newNode = new Node(data, cur, NULL);
            cur->npx = XOR(prev, newNode);
            elementCount++;
        }
    }

    int getEl(int index) {

        if(index >= elementCount || index < 0) throw out_of_range("index out of range");

        Node* cur = head;
        Node* prev = NULL;

        while (index && cur != NULL) {
            Node* next = XOR(prev, cur->npx);
            prev = cur;
            cur = next;
            index--;
        }

        if (index != 0){
            throw out_of_range("index out of range");
        }
        return cur->data;
    }

    void deleteEl(int data){

        Node* cur = head;
        Node* prev = NULL;
        int idx = 0;

        if(head->data == data && head != NULL){
            Node* next = XOR(NULL, head->npx);
            Node* preNext = XOR(head, next->npx);

            delete (head);
            head = next;
            head->npx = XOR(NULL, preNext);
            elementCount--;
            return;
        }

        while (cur != NULL) {
            Node* next = XOR(prev, cur->npx);
            prev = cur;
            cur = next;
            idx++;

            if(cur->data == data){
                if(idx == elementCount - 1){
                    Node* prePrev = XOR(cur, prev->npx);
                    prev->npx = XOR(prePrev, NULL);
                    delete (cur);
                    elementCount--;
                    return;
                } else{
                    next = XOR(prev, cur->npx);

                    Node* preNext = XOR(cur, next->npx);
                    Node* prePrev = XOR(cur, prev->npx);

                    prev->npx = XOR(prePrev, next);
                    next->npx = XOR(prev, preNext);
                    delete (cur);
                    elementCount--;
                    return;
                }
            }
        }
    }

    XORList mergeList(XORList ll1){
        int arr[elementCount];
        for(int i = 0; i < elementCount; i++){
            arr[i] = getEl(i);
        }
        XORList returnList = XORList(arr, sizeof(arr)/sizeof(arr[0]));
        for(int i = 0; i < ll1.getElementCount(); i++){
            returnList.addEl(ll1.getEl(i));
        }
        return returnList;
    }

    vector<XORList*> divide(){
        int len1, len2;
        if(elementCount % 2 == 0){
            len1 = len2 = elementCount / 2;
        } else {
            len1 = elementCount / 2;
            len2 = len1 + 1;
        }

        int firstArr[len1], secondArr[len2];
        for(int i = 0; i < len1; i++) firstArr[i] = getEl(i);
        for(int i = 0; i < len2; i++) secondArr[i] = getEl(i + len1);

        XORList* returnXORfirst = new XORList(firstArr, len1);
        XORList* returnXORsecond = new XORList(secondArr, len2);

        vector<XORList*> returnXORS;
        returnXORS.push_back(returnXORfirst);
        returnXORS.push_back(returnXORsecond);

        delete returnXORfirst;
        delete returnXORsecond;
        return returnXORS;
    }

    int maxNum() {
        int maxi = 0, cur;
        for(int i = 0; i < elementCount - 1; i++){
            cur = getEl(i);
            int counter = 0;
            for(int j = i + 1; j < elementCount; j++){
                if(getEl(j) == cur) counter++;
            }
            if(counter > maxi) maxi = counter;
        }
        return maxi + 1;
    }
};

int main()
{
    int arr[7] = {5,5,5,4,4,6,1};
    // Передали массив и его размер. sizeof(a) высчитывает размер массива в битах. sizeof(a[0]) - размер в битах одного элемента.
    XORList lister = XORList(arr, sizeof(arr)/sizeof(arr[0]));
    cout << lister.getEl(0);
    cout << lister.getEl(1);
    cout << lister.getEl(2);
    cout << lister.getEl(3);
    cout << lister.getEl(4);
    cout << lister.getEl(5);
    cout << lister.getEl(6);

    vector<XORList*> ll = lister.divide();
    XORList* l1 = ll.back();
    ll.pop_back();
    XORList* l2 = ll.back();
    cout << endl;
    for(int i = 0; i < l1->getElementCount(); i++) cout << l1->getEl(i) << " ";
    cout << endl;
    for(int i = 0; i < l2->getElementCount(); i++) cout << l2->getEl(i) << " ";

}
