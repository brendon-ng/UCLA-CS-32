void listAll(const MenuItem* m, string path) // two-parameter overload
{
    if (m == nullptr) // Base Case
        return;
    
    if(path == "")  // If it is the first in the path do not prepend a slash
        path += m->name();
    else
        path += "/" + m->name();
    
    if(path != "")
        cout << path << endl;
    
    if(m->menuItems() == nullptr)   // For Plain Menu Items
        return;
    
    vector<MenuItem*>::const_iterator it;   // Recursively call listAll on submenu items.
    it = m->menuItems()->begin();
    while (it != m->menuItems()->end()) {
        listAll(*it, path);
        it++;
    }
}
