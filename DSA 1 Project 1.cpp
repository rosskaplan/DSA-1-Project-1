//Ross Kaplan
//Due April 7th
//Program 1
//Uses an abstract class called SimpleList with stack and queue derived classes, each with many functions.
//Does string operations to parse the input file to properly create the stacks and queues and to push and pop their values.
//Sends all outputs to a text file out, but also errors that appear along the way if an invalid command is submitted.

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <sstream>
#include <fstream>
#include <stdlib.h>

using namespace std;

//Abstract Class Definition here

template<class T>
class SimpleList {
protected:
	string name;
	unsigned int size;
	class Node{ //all contents of node here (data and pointer to next element)
		public:
		T data;
		Node* next;
		
		Node(T userdata); //Takes in data for new nodes. From StackOverflow.  A little confusing but this is their solution.  Help from Prof. Sable.
	};

	Node *head; //no header so this will be the first node
	Node *tail; //ditto but for last

public: 

	T pop() { //Stacks push to the top and Queues push to the bottom
			  //But they'll both need to pop from the top so I can
			  //Implement it all here for both
		T popped = this->head->data; //Pop the value
		typename SimpleList<T>::Node *temporary = this->head; //make a temporary t to hold pointers. Need initial things to prevent issues on compilation (according to textbook) if a stack or queue has name of Node. 
		this->head = temporary->next; //Store the next pointer in current instance of the class node
		delete(temporary); //the use of "this" is a formality according to the textbook.  not needed but helpful for clarity.
		this->size--;
		return popped;
	};

	virtual void push (T userdata) = 0; //purely virtual function now, will be defined specifically for stacks and queues separately

	int release_size() { //to enable functions outside the scope to see size
		return size;
	}

	string release_name() { //to enable functions outside the scope to see the name
		return name;
	}

	SimpleList(string abstract_input); //constructor
							      //abstract_input is the abstract class's input to the constructor

};

template <class T> 
SimpleList<T>::SimpleList(string abstract_input){	//Constructor for the Base Class
	name = abstract_input;
	size = 0; //Each push will increment size and make the first node (no header) so we should start at 0.
	head = 0; //Depending on whether stack or queue is needed, head and tail will be needed for push and pop differently.
}

template <class T>
SimpleList<T>::Node::Node(T userdata){			//Node constructor
	data = userdata;
	next = 0;
}

// All abstract classes defined and constructed.  Time to create the Stack's class.

template <class T>
class Stack: public SimpleList<T> { //Contents of the stack class
public:

	virtual void push(T userdata) { //Note: This pushes to the front, same side as pop
		typename SimpleList<T>::Node *tempdata = new typename SimpleList<T>::Node(userdata); //store userdata into new node using constructor in class definition

		if (this->size != 0) {
			tempdata->next = this->head;
			this->head=tempdata;
			this->size++;
		}
		else { //case when it's not empty, just move the pointers in a similar logical flow to the pop moving of the temporary pointers.
			this->head=tempdata; //fills with the user's data
			this->size++;
			tempdata->next=nullptr;
			return;
		}
		
	};
	Stack(string abstract_input):SimpleList<T>(abstract_input){}; //stack isnt different upon instantiation-- so base class constructor is fine-- and fill it with the base class'
																  //Sable assisted here.  Needs {} and single : not ::.  Be cautious.
};

template <class T>
class Queue: public SimpleList<T> { //Actual contents of the queue class
public:

	virtual void push(T userdata) {
		typename SimpleList<T>::Node *tempdata = new typename SimpleList<T>::Node(userdata); //make a new node
		if (this->size != 0) { //not the empty case
			this->tail->next = tempdata; 
			this->tail = tempdata;
			this->size++;
			return;
		}
		else {
			this->head=tempdata; //put the temp back into first as before
			this->tail=tempdata; //the tail must be the same to make non-zero case work
			tempdata->next = nullptr;
			this->size++; //keep this updated so that the if statement for size being 0 can tell when it's empty
			return;
		}

	};
	Queue(string abstract_input):SimpleList<T>(abstract_input){}; //the base class' constructor filled with abstract_input from it
};																  //same comments as above from stack

template <class T>
SimpleList<T>* Find(vector<SimpleList<T>*> *finder, string findname) { //the string name from abstract class
	vector<SimpleList<T>*> tempdata = *finder;
	int my_size = tempdata.size()-1;
	for (int i = 0;i <= my_size;my_size--) { //loop to check if any names are the same as the released name. if yes, return it.  if not, return nullptr.
		if (findname == tempdata[my_size]->release_name())
			return tempdata[my_size];
	}
	return nullptr;
}

template <class T>
void PushCheck(vector<SimpleList<T>*> *pusher, T data, string findname, ofstream *outprint) { //checks if you can push to a particular list.  if yes, call the push function. if not, output error
	SimpleList<T> *tempdata = Find(pusher, findname);
	if (tempdata == nullptr) {
		*outprint << "ERROR: This name does not exist!" << endl;
		return;
	}
	tempdata->push(data); //calls the proper push from stack vs queue
}

template <class T>
void PopCheck(vector<SimpleList<T>*> *popper, string findname, ofstream *outprint) { //check whether data can be popped.  if elements exist, it pops them and returns them.  if not, say that it can't be done.
	SimpleList<T> *tempdata = Find<T>(popper, findname);
	if (tempdata == nullptr) {
		*outprint << "ERROR: This name does not exist!" << endl; //print out if name is empty because Find found nothing
		return;
	}
	else if (tempdata->release_size() == 0) {
		*outprint << "ERROR: This list is empty!" << endl; //print out that the list is empty if size is 0
		return;
	}
	else {
		T value = tempdata->pop();
		*outprint << "Value popped: " << value << endl; //the value popped
		return;
	}
}

template <class T>
void Create(vector<SimpleList<T>*> *creater, string makename, string datatype, ofstream *outprint) { //make a new stack/queue if not already there. if it is, don't make one and output errors.
	if(Find<T>(creater,makename)) //checks if the name already exists in the list in function Find
		*outprint << "ERROR: This name already exists!" << endl;
	else { //the name doesn't exist, make it with the proper types
		SimpleList<T> *tempdata;
		if (datatype == "queue") { //if type queue, make one
			tempdata = new Queue<T>(makename);
			creater->push_back(tempdata);
		}
		else { //guaranteed to be stack now, so just make it
			tempdata = new Stack<T>(makename);
			creater->push_back(tempdata);
		} 

	}
}

	void callfcns(const char* infile, const char* outfile) { //takes in the input and output files from main
		vector<SimpleList<int>*> intList;
		vector<SimpleList<double>*> doubleList;
		vector<SimpleList<string>*> stringList;

		ifstream input;
		input.open(infile); //these are the input and output files from main
		ofstream outprint;
		outprint.open(outfile);

		string line;
	while(getline(input, line)) { //while more commands exist in input
		outprint << "PROCESSING COMMAND: " << line << endl;
		stringstream tokenizer;
		tokenizer << line; //tokenize the inputs
		string word1; //Create or Push or Pop
		string word2; //MAKE NAME WHEN USING CREATE
		string word3; //DATATYPE (stack vs queue) WHEN USING CREATE
		tokenizer >> word1 >> word2 >> word3; //Word 1-3 have the values from input file

		char Type = word2.c_str()[0]; //this is the datatype (either "i", "d", or "s")

		if (word1 == "create") { //the first word is create
			//this means word3 has stack or queue in it
			if (Type == 'i') //this'll make an int with the queue/stack based on word3
				Create<int>(&intList, word2, word3, &outprint); //create if possible, if not return error
			else if (Type == 'd')
				Create<double>(&doubleList, word2, word3, &outprint);
			else //it's forced to be "s"
				Create<string>(&stringList, word2, word3, &outprint);
		}
		else if(word1 == "push") {
			//word3 is now the input to be pushed in the push function
			if (Type == 'i')
				PushCheck<int>(&intList, atoi(word3.c_str()), word2, &outprint); //push it possible, if not return error, word3->c_str->atoi
			else if (Type == 'd')
				PushCheck<double>(&doubleList, atof(word3.c_str()), word2, &outprint); //word3->c_str->atof
			else //no need to convert anything as it's already a string
				PushCheck<string>(&stringList, word3, word2, &outprint);
		}
		else { //It's now pop.
			if (Type == 'i') //this'll cover pop --> word3 is meaningless (just blank)
				PopCheck<int>(&intList, word2, &outprint); //pop if possible, if not return error
			else if (Type == 'd') //
				PopCheck<double>(&doubleList, word2, &outprint);
			else //again, it's a string
				PopCheck<string>(&stringList, word2, &outprint);
		}
	}
	input.close();
	outprint.close();
	return;
}

//All definitions finished
int main() {
	string inputfile;
	string outputfile;
	cout << "Input: " << endl;
	cin >> inputfile;
	cout << "Output: " << endl;
	cin >> outputfile;

	callfcns(inputfile.c_str(), outputfile.c_str()); //call function that calls all other stack/queue functions


	return 0; //no error
}