#using <System.dll>
#include <fstream>
//#include <boost/filesystem.hpp> // doesn't work with C++/CLI

using namespace System;
using namespace System::IO;
using namespace System::Security::Permissions;
using namespace System::Threading;

// copy in binary mode
bool copyFile(const char *SRC, const char* DEST)
{
	std::ifstream src(SRC, std::ios::binary);
	std::ofstream dest(DEST, std::ios::binary);
	dest << src.rdbuf();
	return src && dest;
}

class Watcher
{
private:
	// Define the event handlers.
	static void OnChanged(Object^ source, FileSystemEventArgs^ e)
	{
		// Specify what is done when a file is changed, created, or deleted.
		Console::WriteLine("File: {0} {1} Start", e->FullPath, e->ChangeType);

		Thread::Sleep(6000); // 1 min

		//copyFile(e->FullPath);
		// not threadsafe

		Console::WriteLine("File: {0} {1} End", e->FullPath, e->ChangeType);
	}

	static void OnRenamed(Object^ source, RenamedEventArgs^ e)
	{
		// Specify what is done when a file is renamed.
		Console::WriteLine("File: {0} renamed to {1}", e->OldFullPath, e->FullPath);
	}

public:
	[PermissionSet(SecurityAction::Demand, Name = "FullTrust")]
	int static run()
	{
		array<String^>^args = System::Environment::GetCommandLineArgs();

		// If a directory is not specified, exit program.
		if (args->Length != 2)
		{
			// Display the proper way to call the program.
			Console::WriteLine("Usage: Watcher.exe (directory)");
			return 1;
		}

		// Create a new FileSystemWatcher and set its properties.
		FileSystemWatcher^ watcher = gcnew FileSystemWatcher;
		watcher->Path = args[1];
		Console::WriteLine("Watching directory: {0}", args[1]);

		/* Watch for changes in LastAccess and LastWrite times, and
		the renaming of files or directories. */
		watcher->NotifyFilter = static_cast<NotifyFilters>(NotifyFilters::LastAccess |
			NotifyFilters::LastWrite | NotifyFilters::FileName | NotifyFilters::DirectoryName);

		// Watch all files and directories
		watcher->Filter = "*";

		// Add event handlers.
		watcher->Changed += gcnew FileSystemEventHandler(Watcher::OnChanged);
		watcher->Created += gcnew FileSystemEventHandler(Watcher::OnChanged);
		watcher->Deleted += gcnew FileSystemEventHandler(Watcher::OnChanged);
		watcher->Renamed += gcnew RenamedEventHandler(Watcher::OnRenamed);

		// Begin watching.
		watcher->EnableRaisingEvents = true;

		// Wait for the user to quit the program.
		Console::WriteLine("Press \'q\' to quit the sample.");
		while (Console::Read() != 'q')
			;
	}
};

int main() {
	return Watcher::run();
}