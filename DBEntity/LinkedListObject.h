#ifndef _LINKEDLISTOBJECT_H
#define	_LINKEDLISTOBJECT_H

using namespace std;

//Class to store Header information
class ObjectNodelink {
public:
    /* the data of this link */
    void *ObjPtr;
    /* the next link; EMPTY_LIST if this is the last link */
    ObjectNodelink *nextPtr;
    ObjectNodelink *head;
    ObjectNodelink *tail;

    ObjectNodelink() {
        head = 0, tail = 0, nextPtr = 0;
    };

    ~ObjectNodelink() {
        ObjectNodelink *currPtr = head;
        ObjectNodelink *tmpPtr;
        while (currPtr != 0) {
            tmpPtr = currPtr;
            currPtr = currPtr->nextPtr;
            delete tmpPtr;
        }
    };

    void AddFront(void * Obj) {
        ObjectNodelink *NewPtr = new ObjectNodelink();
        NewPtr->ObjPtr = Obj;
        if (head == 0) {
            head = tail = NewPtr;
        }
        else {
            NewPtr->nextPtr = head;
            head = NewPtr;
        }
    };

    void AddBack(void * Obj) {
        ObjectNodelink *NewPtr = new ObjectNodelink();
        NewPtr->ObjPtr = Obj;
        if (head == 0) {
            head = tail = NewPtr;
        }
        else {
            tail->nextPtr = NewPtr;
            tail = NewPtr;
        }
    };

    void Print() {
        if (head == 0) {
            cout << "The list is empty\n";
            return;
        }

        printf("HTTP Header:\n");
        ObjectNodelink *currPtr = head;
        while(currPtr != 0) {
            cout << currPtr->ObjPtr;// << endl;
            currPtr = currPtr->nextPtr;
        }
    };

    ObjectNodelink *GetNodeHead() { return head; };
    ObjectNodelink *GetNodeTail() { return tail; };
    ObjectNodelink *GetNodeNextPtr() { return nextPtr; };

};
/////////////////////////////END HEADER NODE LINK/////////////////////////////////////////////////

#endif	/* _LINKEDLISTOBJECT_H */
