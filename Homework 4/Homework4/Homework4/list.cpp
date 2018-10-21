void listAll(const Class* c, string path)  // two-parameter overload
{
	if (c == nullptr)
		return;
	// Print the path of this class
	path += c->name();
	cout << path << endl;
	// Print the paths of subclasses
	path += "=>";
	for (vector<Class*>::const_iterator it = c->subclasses().begin(); it != c->subclasses().end(); it++)
		listAll(*it, path);
}