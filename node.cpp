#ifndef NODE_H
#define NODE_H
#include "whereiam.h"

#endif // NODE_H

class node
{
public:
node() {data = currentPosition(); link = NULL;}
node(const currentPosition& initData, node* initLink = NULL)
{data = initData; link = initLink;}
void setData(const currentPosition& newData) {data = newData;}
void setLink(node* newLink) {link = newLink;}
currentPosition getData() const {return data;}
const node* getLink() const {return link;}
node* getLink() {return link;}

private:
currentPosition data;
node* link;
};
