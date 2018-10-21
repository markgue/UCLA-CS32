#include <iostream>
#include <string>
using namespace std;

class Investment 
{
public:
	Investment(string name, int price)
		: name_(name), price_(price) {}
	virtual ~Investment() 
		{}
	
	int purchasePrice() const
		{ return price_; }
	string name() const
		{ return name_; }
	virtual bool fungible() const
		{return false; }

	virtual string description() const = 0;

private:
	string name_;
	int price_;
};

class Painting : public Investment 
{
public:
	Painting(string name, int price) 
		: Investment(name, price) {};
	~Painting()
		{ cout << "Destroying " << name() << ", a painting" << endl; }
	virtual string description() const
	 { return "painting"; }
};

class Stock : public Investment 
{
public:
	Stock(string name, int price, string ticker)
		: Investment(name, price), ticker_(ticker) {};
	~Stock()
		{ cout << "Destroying " << name() << ", a stock holding" << endl; }
	virtual bool fungible() const
		{ return true; }
	virtual string description() const
		{ return "stock trading as " + ticker_; }

private:
	string ticker_;
};

class House : public Investment 
{
public:
	House(string name, int price)
		: Investment(name, price) {};
	~House()
		{ cout << "Destroying the house " << name() << endl; }
	virtual string description() const
		{ return "house"; }
};

///////////////////////////////////////////////////////////////////////////////////////////

void display(const Investment* inv)
{
	cout << inv->name();
	if (inv->fungible()) 
		cout << " (fungible)";
	cout << ": " << inv->description() << " bought for $" << inv->purchasePrice() << endl;
}

int main()
{
	Investment* portfolio[4];
	portfolio[0] = new Painting("Salvator Mundi", 450300000);
	// Stock holdings have a name, value, and ticker symbol
	portfolio[1] = new Stock("Alphabet", 100000, "GOOGL");
	portfolio[2] = new Stock("Symantec", 50000, "SYMC");
	portfolio[3] = new House("4 Privet Drive", 660000);

	for (int k = 0; k < 4; k++)
		display(portfolio[k]);

	// Clean up the investments before exiting
	cout << "Cleaning up" << endl;
	for (int k = 0; k < 4; k++)
		delete portfolio[k];
}
