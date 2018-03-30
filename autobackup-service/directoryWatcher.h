// specifically made for windows
#using <System.dll>
#include <fstream>

using namespace System;
using namespace System::IO;
using namespace System::Security::Permissions;
using namespace System::Threading;

ref struct Globals {
	static String^ BackupDirectory;
};

// not threadsafe
bool createFile(String^ SRC, String^ DST);

// not threadsafe
bool copyFile(String^ SRC, String^ DST);

// not threadsafe
bool moveFile(String^ SRC, String^ DST);

// not threadsafe
bool deleteFile(String^ SRC);

class Watcher
{
private:
	// Define the event handlers.
	static void OnChanged(Object^ source, FileSystemEventArgs^ e);

	static void OnRenamed(Object^ source, RenamedEventArgs^ e);

public:
	//[PermissionSet(SecurityAction::Demand, Name = "FullTrust")]
	int static run();
};