
#include ".\mmodelmanager.h"
#include "MMayModel.h"

using namespace TE;
using namespace Ogre;

MModelManager g_ModelManager;

MModelManager::MModelManager(void):v(0)
{
}

MModelManager::~MModelManager(void)
{
     
}	
	
int MModelManager::addEX(std::string name)
{
 
	EditorModel *model = new EditorModel;

	
	if(!model->LoadModel (name.c_str ()))
	{
		//log error 
		return - 1;
	};
	//model->SetName ( name.c_str () );

    //itemset.push_back ( model );

	int id;
	id = nextID();
	return id;

}

int MModelManager::add(std::string name)
{
	int id;
	if (names.find(name) != names.end()) {
		id = names[name];
		items[id]->addref();
		return id;
	}

	std::string nameEX;
	// load new
	int index = name.rfind ('.');
	if(index == -1)
		return -1;

	nameEX = name.substr ( index + 1 ,name.length ());

	if(nameEX == "mdl")
	{

	}
	else
	{
		EditorModel *model = new EditorModel;

		if(!model->LoadModel (name.c_str ()))
		{
			//log error 
			return - 1;
		};
		//model->SetName(name.c_str ());
		id = nextID();
		//do_add(name, id, model);
	}

    return id;
};


void MModelManager::resetAnim()
{

};
void MModelManager::updateEmitters(float dt)
{

};
void MModelManager::clear()
{
	for (std::map<int, ManagedItem*>::iterator it = items.begin(); it != items.end(); ++it) {
		doDelete(it->first);
		//((IModel*)it->second)->Release();
		delete it->second;
	}

	names.clear();
	itemset.clear () ;
	clearID();
};
