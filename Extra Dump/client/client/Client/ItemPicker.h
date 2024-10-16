#pragma once

class ItemPicker//tolua_export
{//tolua_export
public:
	class Hold
	{
	private:
		int itemid;

	public:
		Hold(int id);
		void operator () ();
	};
	class Collect
	{
	private:
		int itemid;

	public:
		Collect(int id);
		void operator () ();
	};

public:
	ItemPicker(void);
	~ItemPicker(void);

	static ItemPicker & getSingleton();

public:
	//tolua_begin

	void pickItem(int itemid);
	bool tryPickItem(int itemid);
	void pickAllItems();
	bool PickItemsForAutoAssist(int m_nId);

	//tolua_end
};//tolua_export
